#!/usr/bin/env bun
// Compare the clean, already-exposed compiler configurations across a small
// cohort of code-overlay C candidates.  This is an observer: it never routes,
// adopts, or changes a candidate.  Every cached row is keyed by source bytes,
// the target bytes, the compiler bundle, and the complete configuration.
import { createHash } from "node:crypto";
import { existsSync, mkdirSync, readFileSync, renameSync, writeFileSync } from "node:fs";
import { basename, dirname, isAbsolute, join, relative, resolve, sep } from "node:path";
import { canonicalJson } from "./canonical_json.ts";
import {
  compilerBundleSignature,
  externalSymbol,
  externalSymbolAssembly,
  overlayCallViaBase,
  sourceToAssemblyPlan,
} from "./alchemy_gcc.ts";
import { type CandidateCompilerFamily } from "./match_m2c.ts";
import { singleConfigs, pairConfigs, tripleConfigs } from "./mode_sweep.ts";
import { assembleOverlay, biasInImageLabelWords, OVERLAY_BASE } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const FORMAT = 1;

type Config = {
  ids: string[];
  flags: string[];
  remove_flags: string[];
  compiler_family: CandidateCompilerFamily;
};
type InventoryRow = { id: string; overlay: string; entry: number; offset: number; span_bytes: number };
type Candidate = InventoryRow & { source: string };
type Score = {
  id: string;
  source: string;
  config: Config;
  cache_key: string;
  cached: boolean;
  compiled: boolean;
  exact: boolean;
  expected_size: number;
  actual_size?: number;
  differing_bytes?: number;
  first_difference?: number;
  error?: string;
};
type Options = {
  candidates: string[];
  inventory: string;
  output: string;
  scope: "families" | "singles";
  pairs: number;
  tripleSeeds: string[];
  triples: number;
  jobs: number;
  top: number;
};

function hash(...parts: Array<string | Uint8Array>): string {
  const digest = createHash("sha256");
  for (const part of parts) { digest.update(part); digest.update("\0"); }
  return digest.digest("hex");
}

export function byteDifference(actual: Uint8Array, expected: Uint8Array): { count: number; first: number | null } {
  let count = 0, first: number | null = null;
  const extent = Math.max(actual.length, expected.length);
  for (let offset = 0; offset < extent; offset++) {
    if (offset < actual.length && offset < expected.length && actual[offset] === expected[offset]) continue;
    count++;
    if (first === null) first = offset;
  }
  return { count, first };
}

function candidateSpec(value: string): { id: string; source: string } {
  const marker = value.indexOf("=");
  if (marker < 1) throw new Error(`candidate must be OVERLAY:OFFSET=PATH, got: ${value}`);
  const id = value.slice(0, marker).toLowerCase();
  const source = value.slice(marker + 1);
  if (!/^[a-z0-9_]+:[0-9a-f]{4,}$/i.test(id) || source.length === 0) {
    throw new Error(`candidate must be OVERLAY:OFFSET=PATH, got: ${value}`);
  }
  return { id, source };
}

function optionsOf(argv: string[]): Options {
  const options: Options = {
    candidates: [], inventory: join(ROOT, "out/decomp/overlays.json"),
    output: join(ROOT, "out/overlay-mode-cohort"), scope: "families", pairs: 0,
    tripleSeeds: [], triples: 0,
    jobs: Math.max(1, Math.min(12, (navigator.hardwareConcurrency || 8) - 2)), top: 12,
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    const take = (): string => {
      const value = argv[++index];
      if (value === undefined) throw new Error(`missing value after ${argument}`);
      return value;
    };
    if (argument === "--candidate" || argument === "-c") options.candidates.push(take());
    else if (argument === "--inventory") options.inventory = take();
    else if (argument === "--output" || argument === "-o") options.output = take();
    else if (argument === "--scope") {
      const scope = take();
      if (scope !== "families" && scope !== "singles") throw new Error("--scope must be families or singles");
      options.scope = scope;
    } else if (argument === "--pairs") options.pairs = Number.parseInt(take(), 10);
    else if (argument === "--triple-seeds") options.tripleSeeds = take().split(",").filter(Boolean);
    else if (argument === "--triples") options.triples = Number.parseInt(take(), 10);
    else if (argument === "--jobs") options.jobs = Number.parseInt(take(), 10);
    else if (argument === "--top") options.top = Number.parseInt(take(), 10);
    else if (argument === "-h" || argument === "--help") {
      console.log([
        "usage: overlay_mode_cohort.ts --candidate OVERLAY:OFFSET=PATH [--candidate ...] [options]",
        "  --candidate, -c SPEC  target and existing C path, e.g. resource_394:07e0=semantic/overlays/resource_394_c_020007e0.c",
        "  --inventory FILE      overlay inventory containing measured owner spans (default out/decomp/overlays.json)",
        "  --scope families|singles  compiler families only (default) or every exposed one-mode configuration",
        "  --pairs N             additionally test at most N deterministic compatible pairs (default 0)",
        "  --triple-seeds IDS    comma-separated mode ids with pair/single evidence; enables evidence-gated triples",
        "  --triples N           cap on seeded triples (default 0 = all seeded triples when seeds given)",
        "  --jobs N              concurrent compiler jobs (default min(12, CPUs-2))",
        "  --output, -o DIR      ignored cache and report directory (default out/overlay-mode-cohort)",
        "  --top N               per-region and aggregate rows to print (default 12)",
        "  --self-test           run parser, scoring, and cache-key checks without compiling",
      ].join("\n"));
      process.exit(0);
    } else throw new Error(`unknown argument: ${argument}`);
  }
  if (options.candidates.length === 0) throw new Error("at least one --candidate is required");
  if (!Number.isInteger(options.pairs) || options.pairs < 0) throw new Error("--pairs must be >= 0");
  if (!Number.isInteger(options.triples) || options.triples < 0) throw new Error("--triples must be >= 0");
  if (options.triples > 0 && options.tripleSeeds.length < 2) {
    throw new Error("--triples needs --triple-seeds with at least two evidenced mode ids (tripleConfigs is evidence-gated)");
  }
  if (!Number.isInteger(options.jobs) || options.jobs < 1 || options.jobs > 32) throw new Error("--jobs must be 1..32");
  if (!Number.isInteger(options.top) || options.top < 1) throw new Error("--top must be positive");
  return options;
}

function acceptedScore(value: unknown, key: string): Score | null {
  if (value === null || typeof value !== "object") return null;
  const row = value as Partial<Score>;
  if (row.cache_key !== key || typeof row.id !== "string" || typeof row.compiled !== "boolean" ||
      typeof row.exact !== "boolean" || typeof row.expected_size !== "number") return null;
  return row as Score;
}

function writeJson(path: string, value: unknown): void {
  mkdirSync(dirname(path), { recursive: true });
  const temporary = `${path}.${process.pid}.tmp`;
  writeFileSync(temporary, canonicalJson(value) + "\n");
  renameSync(temporary, path);
}

async function run(command: string[]): Promise<string> {
  const child = Bun.spawn(command, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  const [code, stdout, stderr] = await Promise.all([
    child.exited, new Response(child.stdout).text(), new Response(child.stderr).text(),
  ]);
  if (code !== 0) throw new Error(`${basename(command[0])} failed: ${(stderr || stdout).trim().slice(0, 500)}`);
  return stdout;
}

// match_m2c's verifier intentionally makes the candidate filename double as
// its routing identity.  Overlay candidates cannot do that: their established
// source name contains both overlay and address.  This local adapter keeps the
// address-stemmed scratch input for linking while asking the compiler policy to
// route from the original candidate path.  It is otherwise the same public
// compile/link boundary used by match_m2c.
async function verifyOverlayCandidate(
  input: string, candidate: Candidate, image: Uint8Array, work: string, config: Config,
): Promise<Buffer> {
  const address = addressFromSource(candidate);
  const routingSource = relative(ROOT, candidate.source).split(sep).join("/");
  const callViaBase = overlayCallViaBase(candidate.overlay, routingSource);
  const stem = address.toString(16).padStart(8, "0");
  const assembly = join(work, `${stem}.s`), object = join(work, `${stem}.o`);
  const symbolsSource = join(work, `${stem}.symbols.s`), symbolsObject = join(work, `${stem}.symbols.o`);
  const elf = join(work, `${stem}.elf`), binary = join(work, `${stem}.bin`);
  const plan = sourceToAssemblyPlan({ target: "gs1", routingSource, input, output: assembly,
    family: config.compiler_family, flags: { addFlags: config.flags, removeFlags: config.remove_flags },
    preprocessedOutput: join(work, `${stem}.i`) });
  for (const step of plan.steps) await run([...step.command]);
  writeFileSync(assembly, biasInImageLabelWords(readFileSync(assembly, "utf8")).text);
  await run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", object, assembly]);
  const names: string[] = [];
  for (const line of (await run(["arm-none-eabi-nm", "-u", object])).split(/\r?\n/).filter(Boolean)) {
    const name = line.trim().split(/\s+/).at(-1)!;
    if (externalSymbol(name, callViaBase) === null) throw new Error(`unsupported external symbol: ${name}`);
    names.push(name);
  }
  writeFileSync(symbolsSource, ".syntax unified\n.thumb\n" + names.map((name) => externalSymbolAssembly(name, callViaBase)).join(""));
  await run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", symbolsObject, symbolsSource]);
  await run(["arm-none-eabi-ld", `-Ttext=0x${stem}`, "-e", `Func_${stem}`, "-o", elf, object, symbolsObject]);
  await run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary]);
  const rows = (await run(["arm-none-eabi-nm", "-S", elf])).split(/\r?\n/);
  const row = rows.find((line) => line.endsWith(` Func_${stem}`));
  if (row === undefined) throw new Error(`missing linked function Func_${stem}`);
  const fields = row.trim().split(/\s+/);
  const data = readFileSync(binary);
  const size = fields.length >= 4 ? Number.parseInt(fields[1], 16) : data.length;
  // `image` is deliberately an argument: keeping it in this adapter makes it
  // impossible to accidentally score a main-image reference against an overlay.
  if (address - OVERLAY_BASE < 0 || address - OVERLAY_BASE >= image.length) throw new Error("overlay address outside image");
  return data.subarray(0, size);
}

function configsOf(options: Options): Config[] {
  const all = singleConfigs() as Config[];
  // Families are the useful first diagnostic: a hit which only appears under
  // one historical family across unrelated owners is a hypothesis; a pile of
  // arbitrary one-off flags is not.
  const base = options.scope === "families"
    ? all.filter((config) => config.ids.length === 0 || config.ids[0].startsWith("compiler-"))
    : all;
  const pairs = options.pairs === 0 ? [] : pairConfigs(options.pairs) as Config[];
  // Triples stay evidence-gated: tripleConfigs only emits combinations where
  // at least two constituents carry the caller-supplied single/pair evidence,
  // so cubic blind permutation is impossible by construction.
  const triples = options.tripleSeeds.length === 0
    ? []
    : tripleConfigs(options.tripleSeeds, options.triples === 0 ? Number.POSITIVE_INFINITY : options.triples) as Config[];
  const keyed = new Map<string, Config>();
  for (const config of [...base, ...pairs, ...triples]) keyed.set(canonicalJson(config), config);
  return [...keyed.values()];
}

function loadCandidates(options: Options): Candidate[] {
  const document = JSON.parse(readFileSync(options.inventory, "utf8")) as { functions?: InventoryRow[] };
  const rows = new Map((document.functions ?? []).map((row) => [row.id.toLowerCase(), row]));
  const found: Candidate[] = [];
  for (const raw of options.candidates) {
    const spec = candidateSpec(raw);
    const row = rows.get(spec.id);
    if (row === undefined) throw new Error(`candidate owner is missing from inventory: ${spec.id}`);
    const source = resolve(spec.source);
    if (!existsSync(source)) throw new Error(`candidate source does not exist: ${spec.source}`);
    found.push({ ...row, source });
  }
  const duplicate = found.find((candidate, index) => found.findIndex((other) => other.id === candidate.id) !== index);
  if (duplicate) throw new Error(`duplicate candidate owner: ${duplicate.id}`);
  return found.sort((left, right) => left.id.localeCompare(right.id));
}

function addressFromSource(candidate: Candidate): number {
  const match = /(?:^|_)c_(020[0-9a-f]{5})\.c$/i.exec(basename(candidate.source));
  const address = match === null ? candidate.entry : Number.parseInt(match[1], 16);
  if (address !== candidate.entry) {
    throw new Error(`${candidate.id}: source address 0x${address.toString(16)} does not match inventory entry 0x${candidate.entry.toString(16)}`);
  }
  return address;
}

async function parallelMap<T, R>(items: readonly T[], jobs: number, operation: (item: T) => Promise<R>): Promise<R[]> {
  const results = new Array<R>(items.length); let cursor = 0;
  async function worker(): Promise<void> {
    while (true) { const index = cursor++; if (index >= items.length) return; results[index] = await operation(items[index]); }
  }
  await Promise.all(Array.from({ length: Math.min(jobs, items.length) }, worker));
  return results;
}

function selfTest(): void {
  const parsed = candidateSpec("resource_394:07e0=semantic/overlays/resource_394_c_020007e0.c");
  if (parsed.id !== "resource_394:07e0" || !parsed.source.startsWith("semantic/")) throw new Error("candidate parser failed");
  const difference = byteDifference(Buffer.from([1, 2, 3]), Buffer.from([1, 4]));
  if (difference.count !== 2 || difference.first !== 1) throw new Error("byte scorer failed");
  const key = hash("source", "target", canonicalJson({ family: "gcc296" }));
  const row: Score = { id: "x:0000", source: "x.c", config: { ids: [], flags: [], remove_flags: [], compiler_family: "gcc296" }, cache_key: key, cached: false, compiled: true, exact: false, expected_size: 2, differing_bytes: 1 };
  if (acceptedScore(row, key) === null || acceptedScore(row, `${key}x`) !== null) throw new Error("cache validation failed");
  const familiesOnly: Options = { candidates: ["x:0000=x.c"], inventory: "x", output: "x", scope: "families", pairs: 0, tripleSeeds: [], triples: 0, jobs: 1, top: 1 };
  if (configsOf(familiesOnly).length < 5) throw new Error("family configuration planning failed");
  const seeded = configsOf({ ...familiesOnly, tripleSeeds: ["cse-two-insn-immediate-off", "sched-low-dest-first"] });
  const tripleCount = seeded.filter((config) => config.ids.length === 3).length;
  if (tripleCount === 0) throw new Error("seeded triple planning produced no triples");
  if (!seeded.some((config) => config.ids.includes("cse-two-insn-immediate-off") && config.ids.length === 3)) {
    throw new Error("seeded triples do not include their own seeds");
  }
  const biased = biasInImageLabelWords(".Ltable:\n\t.word\t.Ltable\n");
  if (biased.biased !== 1 || !biased.text.includes(".Ltable + 0x8000")) throw new Error("overlay label-word bias is not applied");
  const defaultCallVia = overlayCallViaBase("resource_373", "exact/not-an-override.c");
  const overriddenCallVia = overlayCallViaBase("resource_373", "exact/resource_373_c_02005610.c");
  if (defaultCallVia === overriddenCallVia ||
      externalSymbolAssembly("_call_via_r3", overriddenCallVia) === externalSymbolAssembly("_call_via_r3", defaultCallVia)) {
    throw new Error("overlay call-via source routing is not applied");
  }
  console.log("overlay mode cohort self-test passed");
}

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const options = optionsOf(Bun.argv.slice(2));
  const candidates = loadCandidates(options);
  const configs = configsOf(options);
  const compilerSignature = hash(compilerBundleSignature(), readFileSync(Bun.fileURLToPath(import.meta.url)));
  const overlays = new Map<string, Buffer>();
  for (const overlay of new Set(candidates.map((candidate) => candidate.overlay))) {
    overlays.set(overlay, assembleOverlay(join(ROOT, "assets/code", `${overlay}_overlay.s`), OVERLAY_BASE));
  }
  const tasks = candidates.flatMap((candidate) => configs.map((config) => ({ candidate, config })));
  const scores = await parallelMap(tasks, options.jobs, async ({ candidate, config }): Promise<Score> => {
    const address = addressFromSource(candidate);
    const reference = overlays.get(candidate.overlay)!.subarray(candidate.offset, candidate.offset + candidate.span_bytes);
    const sourceBytes = readFileSync(candidate.source);
    const key = hash(String(FORMAT), sourceBytes, reference, compilerSignature, canonicalJson({ id: candidate.id, config }));
    const cachePath = join(options.output, "cache", `${key}.json`);
    if (existsSync(cachePath)) {
      const cached = acceptedScore(JSON.parse(readFileSync(cachePath, "utf8")), key);
      if (cached !== null) return { ...cached, cached: true };
    }
    const scratch = join(options.output, "scratch", key);
    mkdirSync(scratch, { recursive: true });
    // The verifier requires an address-stemmed translation unit.  Including the
    // original file keeps relative includes rooted at its real directory and
    // leaves the candidate itself untouched.
    const input = join(scratch, `${address.toString(16).padStart(8, "0")}.c`);
    writeFileSync(input, `#include ${JSON.stringify(candidate.source)}\n`);
    let score: Score;
    try {
      const actual = await verifyOverlayCandidate(input, candidate, overlays.get(candidate.overlay)!, scratch, config);
      const difference = byteDifference(actual, reference);
      score = { id: candidate.id, source: isAbsolute(candidate.source) ? candidate.source.slice(ROOT.length + 1) : candidate.source,
        config, cache_key: key, cached: false, compiled: true, exact: difference.count === 0,
        expected_size: reference.length, actual_size: actual.length, differing_bytes: difference.count,
        ...(difference.first === null ? {} : { first_difference: difference.first }) };
    } catch (cause) {
      score = { id: candidate.id, source: candidate.source.slice(ROOT.length + 1), config, cache_key: key,
        cached: false, compiled: false, exact: false, expected_size: reference.length,
        error: cause instanceof Error ? cause.message.slice(0, 500) : String(cause).slice(0, 500) };
    }
    writeJson(cachePath, score);
    return score;
  });
  const baseline = new Map(scores.filter((row) => row.config.ids.length === 0).map((row) => [row.id, row]));
  const byConfig = configs.map((config) => {
    const rows = scores.filter((score) => canonicalJson(score.config) === canonicalJson(config));
    const compiled = rows.filter((row) => row.compiled);
    const exact = compiled.filter((row) => row.exact);
    const improved = compiled.filter((row) => {
      const floor = baseline.get(row.id);
      return floor?.compiled === true && (row.differing_bytes ?? Infinity) < (floor.differing_bytes ?? Infinity);
    });
    return { config, attempted: rows.length, compiled: compiled.length, exact: exact.length,
      exact_regions: exact.map((row) => row.id), improves_baseline_regions: improved.map((row) => row.id),
      improvement_bytes: improved.reduce((sum, row) => sum + (baseline.get(row.id)!.differing_bytes! - row.differing_bytes!), 0),
      total_differing_bytes: compiled.reduce((sum, row) => sum + (row.differing_bytes ?? 0), 0) };
  }).sort((left, right) => right.exact - left.exact || right.improves_baseline_regions.length - left.improves_baseline_regions.length || right.improvement_bytes - left.improvement_bytes || left.total_differing_bytes - right.total_differing_bytes || left.config.ids.join("+").localeCompare(right.config.ids.join("+")));
  const bestByRegion = candidates.map((candidate) => scores.filter((row) => row.id === candidate.id && row.compiled)
    .sort((left, right) => Number(right.exact) - Number(left.exact) || (left.differing_bytes ?? Infinity) - (right.differing_bytes ?? Infinity))[0] ?? null);
  const report = { format: FORMAT, policy: { clean_room: true, auto_promote: false, compiler_configurations: "already exposed by tools/mode_sweep.ts" }, candidates: candidates.map(({ id, source, span_bytes }) => ({ id, source: source.slice(ROOT.length + 1), span_bytes })), configurations: configs, attempted: scores.length, compiled: scores.filter((row) => row.compiled).length, exact: scores.filter((row) => row.exact).length, shared: byConfig, best_by_region: bestByRegion, results: scores };
  writeJson(join(options.output, "report.json"), report);
  console.log(`candidates=${candidates.length} configurations=${configs.length} attempted=${scores.length} compiled=${report.compiled} exact=${report.exact}`);
  console.log("shared configurations (improvements relative to routed baseline):");
  for (const row of byConfig.slice(0, options.top)) console.log(`exact=${row.exact} improved=${row.improves_baseline_regions.length} gain=${row.improvement_bytes}B differing=${row.total_differing_bytes} ${row.config.compiler_family} ${row.config.ids.join("+") || "routed-default"}`);
  console.log("best per region:");
  for (const row of bestByRegion.slice(0, options.top)) console.log(row === null ? "uncompiled" : `${row.id} exact=${row.exact} differing=${row.differing_bytes ?? "?"} ${row.config.compiler_family} ${row.config.ids.join("+") || "routed-default"}`);
  console.log(`report=${join(options.output, "report.json")}`);
}

if (import.meta.main) await main();
