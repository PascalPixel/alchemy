#!/usr/bin/env bun
// Compare ordinary overlay functions with Alchemy's own byte-verified C corpus.
// Exact fingerprints retain compiler register allocation and instruction choice;
// structural fingerprints normalize registers and immediates for candidate
// ranking only. Generated binaries and reports remain below ignored out/.
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { assembleOverlay, OVERLAY_BASE } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface Options { inventory: string; objects: string; output: string; jobs: number; top: number }
interface FunctionRow {
  id: string;
  overlay: string;
  entry: number;
  offset: number;
  span_bytes: number;
  instruction_offsets: number[];
  fingerprint: string;
}
interface Template { stem: string; source: string; instructions: string[]; exact: string; structural: string }

function optionsOf(argv: string[]): Options {
  const options: Options = {
    inventory: join(ROOT, "out/decomp/overlays.json"),
    objects: join(ROOT, "out/full/claimed/obj"),
    output: join(ROOT, "out/decomp/compiler-templates.json"),
    jobs: Math.min(16, navigator.hardwareConcurrency || 1),
    top: 30,
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--inventory") options.inventory = argv[++index];
    else if (argument === "--objects") options.objects = argv[++index];
    else if (argument === "--output" || argument === "-o") options.output = argv[++index];
    else if (argument === "--jobs") options.jobs = Number(argv[++index]);
    else if (argument === "--top") options.top = Number(argv[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: compiler_template_index.ts [--inventory FILE] [--objects DIR] [--jobs N] [--top N] [-o FILE]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  if (![options.jobs, options.top].every((value) => Number.isInteger(value) && value > 0)) {
    throw new Error("--jobs and --top must be positive integers");
  }
  return options;
}

async function run(command: string[]): Promise<string> {
  const child = Bun.spawn(command, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  const [stdout, stderr, code] = await Promise.all([
    new Response(child.stdout).text(), new Response(child.stderr).text(), child.exited,
  ]);
  if (code !== 0) throw new Error(stderr.trim() || `${basename(command[0])} failed`);
  return stdout;
}

const ROW = /^\s*([0-9a-f]+):\s+(?:[0-9a-f]{4}\s+)+(\.?[a-z][a-z0-9.]*)\s*(.*)$/i;
const BRANCH = /^(?:b|bl|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble|bhs|blo)(?:\.n|\.w)?$/;

function rows(text: string): Map<number, string> {
  const result = new Map<number, string>();
  for (const raw of text.split(/\r?\n/)) {
    const match = ROW.exec(raw);
    if (match === null || match[2].startsWith(".")) continue;
    const mnemonic = match[2].toLowerCase();
    let operands = match[3].replace(/\s*@.*$/, "").replace(/<[^>]+>/g, "TARGET").trim().toLowerCase();
    if (BRANCH.test(mnemonic)) operands = "TARGET";
    result.set(Number.parseInt(match[1], 16), `${mnemonic}${operands ? ` ${operands}` : ""}`);
  }
  return result;
}

function structural(instructions: string[]): string[] {
  const registers = new Map<string, string>();
  let next = 0;
  return instructions.map((instruction) => instruction
    .replace(/\b(?:r(?:1[0-2]|[0-9])|sl|fp|ip)\b/gi, (register) => {
      const key = register.toLowerCase();
      if (!registers.has(key)) registers.set(key, `v${next++}`);
      return registers.get(key)!;
    })
    .replace(/#-?(?:0x[0-9a-f]+|\d+)/gi, "#IMM")
    .replace(/\b0x[0-9a-f]+\b/gi, "CONST")
    .replace(/\s+/g, " "));
}

function digest(instructions: string[]): string {
  return Bun.hash(instructions.join("\n")).toString(36);
}

async function parallelMap<T, R>(items: T[], jobs: number, operation: (item: T) => Promise<R>): Promise<R[]> {
  const results = new Array<R>(items.length);
  let cursor = 0;
  async function worker(): Promise<void> {
    while (true) {
      const index = cursor++;
      if (index >= items.length) return;
      results[index] = await operation(items[index]);
    }
  }
  await Promise.all(Array.from({ length: Math.min(jobs, items.length) }, worker));
  return results;
}

async function main(): Promise<void> {
  const options = optionsOf(Bun.argv.slice(2));
  const inventory = JSON.parse(readFileSync(options.inventory, "utf8")) as {
    functions: FunctionRow[];
    families: Array<{ fingerprint: string; count: number }>;
  };
  const duplicate = new Set(inventory.families.filter((family) => family.count > 1).map((family) => family.fingerprint));
  const overlayFunctions = inventory.functions.filter((fn) => duplicate.has(fn.fingerprint));

  const objectNames = readdirSync(options.objects).filter((name) => /^08[01][0-9a-f]{5}\.o$/.test(name)).sort();
  const templates = await parallelMap(objectNames, options.jobs, async (name): Promise<Template> => {
    const stem = basename(name, ".o");
    const decoded = rows(await run(["arm-none-eabi-objdump", "-dr", join(options.objects, name)]));
    const instructions = [...decoded].sort((a, b) => a[0] - b[0]).map((item) => item[1]);
    return {
      stem,
      source: `src/${stem}.c`,
      instructions,
      exact: digest(instructions),
      structural: digest(structural(instructions)),
    };
  });
  const exactTemplates = new Map<string, Template[]>(), structuralTemplates = new Map<string, Template[]>();
  for (const template of templates) {
    if (!exactTemplates.has(template.exact)) exactTemplates.set(template.exact, []);
    exactTemplates.get(template.exact)!.push(template);
    if (!structuralTemplates.has(template.structural)) structuralTemplates.set(template.structural, []);
    structuralTemplates.get(template.structural)!.push(template);
  }

  const work = join(dirname(options.output), "compiler-template-work");
  mkdirSync(work, { recursive: true });
  const overlayRows = new Map<string, Map<number, string>>();
  for (const overlay of new Set(overlayFunctions.map((fn) => fn.overlay))) {
    const binary = join(work, `${overlay}.bin`);
    writeFileSync(binary, assembleOverlay(join(ROOT, "assets/code", overlay, "overlay.s"), OVERLAY_BASE));
    const decoded = await run([
      "arm-none-eabi-objdump", "-D", "-b", "binary", "-marmv4t", "-Mforce-thumb",
      `--adjust-vma=0x${OVERLAY_BASE.toString(16)}`, binary,
    ]);
    overlayRows.set(overlay, rows(decoded));
  }

  const results = overlayFunctions.map((fn) => {
    const decoded = overlayRows.get(fn.overlay)!;
    const instructions = fn.instruction_offsets
      .map((offset) => decoded.get(fn.entry + offset))
      .filter((instruction): instruction is string => instruction !== undefined);
    const exact = exactTemplates.get(digest(instructions)) ?? [];
    const shape = structuralTemplates.get(digest(structural(instructions))) ?? [];
    return {
      id: fn.id,
      instruction_count: instructions.length,
      exact_templates: exact.map((item) => item.source),
      structural_templates: shape.map((item) => item.source),
    };
  });
  const exactHits = results.filter((item) => item.exact_templates.length > 0);
  const structuralHits = results.filter((item) => item.structural_templates.length > 0);
  const report = {
    format: 1,
    totals: {
      c_templates: templates.length,
      overlay_functions: results.length,
      exact_hits: exactHits.length,
      structural_hits: structuralHits.length,
    },
    exact_hits: exactHits,
    structural_hits: structuralHits,
  };
  mkdirSync(dirname(options.output), { recursive: true });
  writeFileSync(options.output, JSON.stringify(report, null, 2) + "\n");
  console.log(`c_templates=${templates.length} overlay_functions=${results.length} exact_hits=${exactHits.length} structural_hits=${structuralHits.length}`);
  for (const hit of [...exactHits, ...structuralHits.filter((item) => item.exact_templates.length === 0)].slice(0, options.top)) {
    console.log(`${hit.id}\texact=${hit.exact_templates.join(",") || "-"}\tstructural=${hit.structural_templates.slice(0, 5).join(",") || "-"}`);
  }
  console.log(`report=${options.output}`);
}

if (import.meta.main) await main();
