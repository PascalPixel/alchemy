#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md.
// Generate and byte-test m2c drafts for high-confidence duplicate overlay
// functions. Private compiler products and generated drafts remain ignored.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { M2C } from "./alchemy_gcc.ts";
import { emitDiscovery } from "./emit_function.ts";
import { M2C_PREAMBLE, verifyCandidate } from "./match_m2c.ts";
import { assembleOverlay, OVERLAY_BASE } from "./overlay_disasm.ts";
import { discoverOverlay } from "./overlay_inventory.ts";
import { variants } from "./search_variants.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const REJECT = ["M2C_ERROR", "M2C_BITFIELD", "M2C_MEMSET", "M2C_MEMCPY"];

interface Options {
  inventory: string;
  limit: number;
  maxBytes: number;
  jobs: number;
  variants: number;
  all: boolean;
  targets: Set<string> | null;
  output: string;
}
interface FunctionRow {
  id: string;
  overlay: string;
  entry: number;
  offset: number;
  code_bytes: number;
  span_bytes: number;
  unresolved: number;
  jump_tables: number;
  fingerprint: string;
}

function optionsOf(argv: string[]): Options {
  const options: Options = {
    inventory: join(ROOT, "out/decomp/overlays.json"),
    limit: 300,
    maxBytes: 96,
    jobs: Math.min(16, navigator.hardwareConcurrency || 1),
    variants: 1,
    all: false,
    targets: null,
    output: join(ROOT, "out/decomp/overlay-matches"),
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--inventory") options.inventory = argv[++index];
    else if (argument === "--limit") options.limit = Number(argv[++index]);
    else if (argument === "--max-bytes") options.maxBytes = Number(argv[++index]);
    else if (argument === "--jobs") options.jobs = Number(argv[++index]);
    else if (argument === "--variants") options.variants = Number(argv[++index]);
    else if (argument === "--all") options.all = true;
    else if (argument === "--targets") options.targets = new Set(argv[++index].split(",").filter(Boolean));
    else if (argument === "--output" || argument === "-o") options.output = argv[++index];
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: overlay_match.ts [--inventory FILE] [--limit N] [--max-bytes N] [--jobs N] [--variants N] [--all] [--targets ID,...] [-o DIR]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  for (const value of [options.limit, options.maxBytes, options.jobs, options.variants]) {
    if (!Number.isInteger(value) || value < 1) throw new Error("numeric options must be positive integers");
  }
  return options;
}

async function m2c(assembly: string): Promise<string | null> {
  const child = Bun.spawn(
    [String(M2C), "-t", "gba", "--valid-syntax", "--comment-style", "none", assembly],
    { cwd: ROOT, stdout: "pipe", stderr: "pipe" },
  );
  const [stdout, code] = await Promise.all([new Response(child.stdout).text(), child.exited]);
  return code === 0 ? stdout : null;
}

async function main(): Promise<void> {
  const options = optionsOf(Bun.argv.slice(2));
  const inventory = JSON.parse(readFileSync(options.inventory, "utf8")) as {
    functions: FunctionRow[];
    families: Array<{ fingerprint: string; count: number }>;
  };
  const duplicate = new Set(inventory.families.filter((family) => family.count > 1).map((family) => family.fingerprint));
  const selected = inventory.functions
    .filter((fn) => (options.all || duplicate.has(fn.fingerprint)) && fn.unresolved === 0 && fn.jump_tables === 0 &&
      fn.code_bytes >= 8 && fn.code_bytes <= options.maxBytes && fn.span_bytes - fn.code_bytes <= 64 &&
      (options.targets === null || options.targets.has(fn.id)))
    .sort((left, right) => left.code_bytes - right.code_bytes || left.id.localeCompare(right.id))
    .slice(0, options.limit);
  const overlays = new Map<string, { data: Buffer; discovery: ReturnType<typeof discoverOverlay> }>();
  for (const overlay of new Set(selected.map((fn) => fn.overlay))) {
    const source = join(ROOT, "assets/code", overlay, "overlay.s");
    const data = assembleOverlay(source, OVERLAY_BASE);
    overlays.set(overlay, { data, discovery: discoverOverlay(data) });
  }
  mkdirSync(options.output, { recursive: true });
  const results = new Array<Record<string, unknown>>(selected.length);
  let cursor = 0, compiled = 0, matched = 0;
  async function worker(): Promise<void> {
    while (true) {
      const index = cursor++;
      if (index >= selected.length) return;
      const fn = selected[index];
      const overlay = overlays.get(fn.overlay)!;
      const directory = join(options.output, fn.overlay, fn.offset.toString(16).padStart(4, "0"));
      mkdirSync(directory, { recursive: true });
      const assembly = join(directory, "input.s");
      try { emitDiscovery(overlay.data, overlay.discovery, fn.entry, assembly); }
      catch (error) {
        results[index] = { id: fn.id, matched: false, error: `emit: ${(error as Error).message}` };
        continue;
      }
      const draft = await m2c(assembly);
      if (draft === null || REJECT.some((marker) => draft.includes(marker))) {
        results[index] = { id: fn.id, matched: false, error: "m2c" };
        continue;
      }
      const replacements: Array<string | null> = draft.includes("M2C_UNK")
        ? ["void", "s32", "u32", "void *", "s16", "u16", "s8", "u8"]
        : [null];
      let best = Number.MAX_SAFE_INTEGER;
      for (const replacement of replacements) {
        const body = replacement === null ? draft : draft.replaceAll("M2C_UNK", replacement);
        for (const [label, source] of variants(M2C_PREAMBLE + body, options.variants)) {
          const candidate = join(directory, `${fn.entry.toString(16).padStart(8, "0")}.c`);
          writeFileSync(candidate, source);
          try {
            const build = join(directory, "build");
            mkdirSync(build, { recursive: true });
            const verification = await verifyCandidate(candidate, overlay.data, build, [], OVERLAY_BASE);
            compiled++;
            const expected = overlay.data.subarray(fn.offset, fn.offset + fn.span_bytes);
            let mismatch = Math.abs(verification.actual.length - expected.length);
            for (let byte = 0; byte < Math.min(verification.actual.length, expected.length); byte++) {
              if (verification.actual[byte] !== expected[byte]) mismatch++;
            }
            best = Math.min(best, mismatch);
            if (verification.size === fn.span_bytes && verification.actual.equals(expected)) {
              matched++;
              results[index] = { id: fn.id, matched: true, size: verification.size, source: candidate, unknown_type: replacement, label };
              console.log(`exact ${fn.id} bytes=${verification.size} label=${label}`);
              break;
            }
          } catch {}
        }
        if (results[index]?.matched === true) break;
      }
      results[index] ??= {
        id: fn.id,
        matched: false,
        mismatched_bytes: best === Number.MAX_SAFE_INTEGER ? null : best,
      };
    }
  }
  await Promise.all(Array.from({ length: Math.min(options.jobs, selected.length) }, worker));
  const report = { format: 1, targets: selected.length, compiled, matched, results };
  writeFileSync(join(options.output, "report.json"), JSON.stringify(report, null, 2) + "\n");
  console.log(`targets=${selected.length} compiled=${compiled} matches=${matched} report=${join(options.output, "report.json")}`);
}

if (import.meta.main) await main();
