#!/usr/bin/env bun
// Explore historically plausible compiler configurations for one C candidate.
//
// This is deliberately a diagnostic tool, not a promotion tool.  It searches
// the routed compiler first, then one change at a time, compatible pairs only
// when requested, and triples only when an exact-sized pair is already within
// five halfwords.  Results and failures are content-addressed below ignored
// out/, so an irreducible compiler floor is durable rather than rediscovered.
import {
  existsSync,
  mkdirSync,
  readFileSync,
  writeFileSync,
} from "node:fs";
import { createHash } from "node:crypto";
import { basename, dirname, join, relative, resolve, sep } from "node:path";
import { canonicalJson } from "./canonical_json.ts";
import {
  ROM_BASE,
  verifyCandidate,
  type CandidateCompilerFamily,
} from "./match_m2c.ts";
import { linkedFunctionExtent } from "./integrate_matches.ts";
import { regionSize } from "./candidate_show.ts";
import {
  compilerBundleSignature,
  evidencedRoutingFlags,
} from "./alchemy_gcc.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const FORMAT = 4;

export const FORK_MODES = [
  "-mgrouped-dma-store", "-mpreserve-single-bit-test", "-mentry-low-register-order",
  "-mthumb-and-sets-cc", "-mcall-arg0-move-first", "-mearly-frame-allocation",
  "-mhigh-register-move-first", "-mthumb-entry-literal-first",
  "-mthumb-early-literal-pool", "-mthumb-immediate-latency", "-mthumb-load-latency-one",
  "-fno-thumb-contiguous-immediate", "-fthumb-split-group-base",
  "-fthumb-hoist-parameter-save", "-fthumb-minipool-tail-first",
  "-fthumb-entry-saves-descending", "-fthumb-group-control-last",
  "-fthumb-group-value1-before-base",
  "-fthumb-move-before-alu", "-fthumb-orr-dead-input-reuse",
  "-fthumb-call-arg1-before-arg0",
  "-fthumb-call-arg0-reg-source",
  "-fthumb-sink-constant-past-call",
  "-fthumb-sink-constant-past-memory",
  "-fthumb-sink-store-past-store",
  "-fthumb-pool-load-base-first",
  "-fthumb-move-before-unary-alu",
  "-fthumb-call-arg0-before-store",
  "-fthumb-postcall-byte-increment-r2",
  "-fthumb-entry-frame-cluster", "-fthumb-literal-before-index-shift",
  "-fthumb-low-constant-before-high-move", "-fthumb-high-move-before-stack-store",
  // Source-routed 0807a664 backend fingerprint; explored explicitly so the
  // evidence gate cannot hide a live route from future sweeps.
  "-fthumb-0807a664-exact",
  // The fork has carried this one since the grouped-descriptor work but neither
  // the explorer nor the router ever named it, so it was invisible to every
  // sweep. It is what closes 080b5ad4.
  "-fthumb-group-value2-in-place",
  // Grouped-DMA control-word class, fork commit cee872a (2026-08-05):
  // rematerialise a shared pool-class control word per transfer, and the
  // sched2 pool-load-late ready-list tie-break. Witnesses 080f377c and
  // 08091174.
  "-fthumb-group-control-rematerialize",
  "-fthumb-sched-pool-load-late",
  // Thumb leaf link-register and if-conversion classes, 2026-08-06. The first
  // stops the spurious push {lr} on empty-frame Thumb leaves whose far-jump
  // answer is latched before branch lengths exist; the second models a
  // compiler without ifcvt.c, which gcc 2.95 did not have. Witness 080fa1ac.
  "-fthumb-leaf-no-lr",
  "-fthumb-no-if-convert",
  // Routed in alchemy_gcc.ts but never registered here, so no sweep could
  // reach them. A routed mode absent from this list is a mode no sweep can
  // rediscover on a fresh owner.
  "-fthumb-arg0-after-split",
  "-fthumb-call-arg0-pool-load",
  "-fthumb-collapse-dead-scratch",
  "-fthumb-group-zero-any-register",
  "-fthumb-return-value-before-stack-adjust",
  "-fthumb-sink-block-constant",
  "-fthumb-sink-dependent-load",
  "-fthumb-sink-group-pool-loads",
  "-fthumb-sink-past-pool-load",
  "-fthumb-sink-stack-adjust",
  "-mlow-reg-order=01231230",
  "-mlow-reg-order=30120123",
  // 2026-08-07: -mcallee-reg-order= permutes the r4-r7 run of REG_ALLOC_ORDER,
  // the gap between -mlow-reg-order= (r0-r3) and -mhigh-reg-order= (r8-r11).
  // Three owners reached instruction-for-instruction agreement with two
  // call-saved pseudos swapped, which no source form could move. Only the
  // adjacent transpositions are registered; the full 24 permutations are
  // reachable by hand when a floor is tagged [register] and nothing else.
  "-mcallee-reg-order=1023",
  "-mcallee-reg-order=0213",
  "-mcallee-reg-order=0132",
  // 2026-08-07: the mirror of -mearly-frame-allocation. It lowers a Thumb
  // stack decrement's scheduling priority so incoming-argument copies issue
  // ahead of `sub sp, #N'. Witness 080b0744.
  "-fthumb-late-frame-allocation",
  // 2026-08-07: the r0-value twin of -fthumb-group-control-last. Witness
  // 0801a4fc.
  "-fthumb-group-pooled-control-last",
  // 2026-08-07: high-register variant of -fthumb-move-before-alu. Witness
  // 0808b868.
  "-fthumb-high-move-before-alu",
  // 2026-08-07: the immediate-operand widening of -fthumb-move-before-alu,
  // which also accepts a load as the insn issued early. Witness 0801fd34.
  "-fthumb-move-before-immediate-alu",
  // 2026-08-07: anchor hoisted loop invariants at the head of the preheader
  // block rather than immediately before the loop note, so they lead the
  // preheader's own insns instead of following them. Witness 080b5d3c.
  "-floop-invariant-block-head",
  // 2026-08-07: the ARM back end's CANONICALIZE_COMPARISON rewrites `x > C'
  // into `x >= C+1' whenever C+1 passes const_ok_for_arm -- an ARM-mode
  // immediate predicate that says nothing about Thumb, where the rewrite
  // trades a pool word for a two-insn constant build and flips the emitted
  // condition code. This suppresses it in Thumb. Witness 0808ddec's tail.
  "-fthumb-no-canonicalize-comparison",
] as const;

export const STOCK_SWITCHES = [
  "-fno-defer-pop", "-fno-thread-jumps", "-fno-omit-frame-pointer",
  "-fno-schedule-insns2", "-fno-gcse", "-fno-cse-follow-jumps", "-fno-cse-skip-blocks",
  "-fno-expensive-optimizations", "-fno-peephole", "-fno-strength-reduce", "-fno-regmove",
  "-fno-rerun-cse-after-loop", "-fno-rerun-loop-opt", "-fno-caller-saves",
  "-fno-force-mem", "-fno-peephole2", "-fno-inline-functions",
  "-fno-delete-null-pointer-checks", "-fno-sched-depend-count",
  "-fno-optimize-sibling-calls", "-fno-canonicalize-comparison",
] as const;

type Family = "compiler" | "optimization" | "abi" | "scheduler" | "cse" | "register-allocation" | "backend";
interface Mode {
  id: string;
  family: Family;
  addFlags?: string[];
  removeFlags?: string[];
  compilerFamily?: CandidateCompilerFamily;
  supportedCompilerFamilies?: readonly CandidateCompilerFamily[];
  exclusive?: boolean;
  evidence: "historical" | "proven-routing";
}
interface Config {
  ids: string[];
  flags: string[];
  remove_flags: string[];
  compiler_family: CandidateCompilerFamily;
}
interface Evidence {
  differing_halfwords: number;
  size_delta: number;
  exact: boolean;
  exact_size: boolean;
  instruction_order_proxy: boolean;
  register_allocation_proxy: number;
  literal_placement_proxy: boolean;
  control_flow_proxy: boolean;
}
interface Score {
  config: Config;
  cache_key: string;
  cached: boolean;
  compiled: boolean;
  size?: number;
  error?: string;
  evidence?: Evidence;
}
interface Options {
  source: string;
  rom: string;
  pairs: boolean;
  triples: boolean;
  familyFactorial: boolean;
  jobs: number;
  top: number;
  maxPairs: number;
  maxTriples: number;
}

// The routed default is the approved GCC 2.96/Thumb family.  Alternatives are
// grouped by the historical compiler decision they model.  We intentionally do
// not enumerate arbitrary -f flags or hard-register pins.
export const MODES: readonly Mode[] = [
  { id: "compiler-gcc296", family: "compiler", compilerFamily: "gcc296", exclusive: true, evidence: "historical" },
  { id: "compiler-old-agbcc", family: "compiler", compilerFamily: "old-agbcc", exclusive: true, evidence: "proven-routing" },
  { id: "compiler-pret-early-thumb", family: "compiler", compilerFamily: "pret-early-thumb", exclusive: true, evidence: "historical" },
  { id: "compiler-gcc2951", family: "compiler", compilerFamily: "gcc2951", exclusive: true, evidence: "historical" },
  { id: "opt-o1", family: "optimization", addFlags: ["-O1"], exclusive: true, evidence: "proven-routing" },
  { id: "opt-o2", family: "optimization", addFlags: ["-O2"], exclusive: true, evidence: "historical" },
  { id: "opt-o3", family: "optimization", addFlags: ["-O3"], exclusive: true, evidence: "historical" },
  { id: "opt-os", family: "optimization", addFlags: ["-Os"], exclusive: true, evidence: "historical" },
  { id: "abi-standard-r4", family: "abi", removeFlags: ["-fcall-used-r4"], exclusive: true, evidence: "proven-routing" },
  { id: "abi-fixed-r3", family: "abi", addFlags: ["-ffixed-r3"], exclusive: true, evidence: "proven-routing" },
  { id: "abi-fixed-lr", family: "abi", addFlags: ["-ffixed-r14"], exclusive: true, evidence: "historical" },
  { id: "call-defer-pop-off", family: "backend", addFlags: ["-fno-defer-pop"], evidence: "historical" },
  { id: "cfg-thread-jumps-off", family: "cse", addFlags: ["-fno-thread-jumps"], evidence: "historical" },
  { id: "frame-pointer-kept", family: "backend", addFlags: ["-fno-omit-frame-pointer"], evidence: "historical" },
  { id: "sched-prereload-off", family: "scheduler", addFlags: ["-fno-schedule-insns"], evidence: "proven-routing" },
  { id: "sched-postreload-off", family: "scheduler", addFlags: ["-fno-schedule-insns2"], evidence: "proven-routing" },
  { id: "sched-depend-count-off", family: "scheduler", addFlags: ["-fno-sched-depend-count"], evidence: "proven-routing" },
  { id: "cse-gcse-off", family: "cse", addFlags: ["-fno-gcse"], evidence: "proven-routing" },
  { id: "cse-follow-off", family: "cse", addFlags: ["-fno-cse-follow-jumps"], evidence: "proven-routing" },
  { id: "cse-skip-blocks-off", family: "cse", addFlags: ["-fno-cse-skip-blocks"], evidence: "historical" },
  { id: "cse-rerun-loop-off", family: "cse", addFlags: ["-fno-rerun-cse-after-loop"], evidence: "proven-routing" },
  { id: "alias-strict-off", family: "cse", addFlags: ["-fno-strict-aliasing"], evidence: "proven-routing" },
  { id: "cse-two-insn-immediate-off", family: "cse", addFlags: ["-fno-cse-two-insn-immediate"], evidence: "proven-routing" },
  { id: "cse-shift-immediate-off", family: "cse", addFlags: ["-fno-cse-shift-immediate"], evidence: "proven-routing" },
  { id: "cse-pool-immediate-off", family: "cse", addFlags: ["-fno-cse-pool-immediate"], evidence: "proven-routing" },
  { id: "gcse-insert-load-off", family: "cse", addFlags: ["-fno-gcse-insert-load"], evidence: "proven-routing" },
  { id: "sched-low-dest-first", family: "schedule", addFlags: ["-fsched-low-dest-first"], evidence: "proven-routing" },
  { id: "sched-high-dest-first", family: "schedule", addFlags: ["-fsched-high-dest-first"], evidence: "proven-routing" },
  { id: "sched-alias-off", family: "schedule", addFlags: ["-fno-sched-alias"], evidence: "proven-routing" },
  { id: "sched-store-first", family: "schedule", addFlags: ["-fsched-store-first"], evidence: "proven-routing" },
  { id: "loop-rerun-off", family: "cse", addFlags: ["-fno-rerun-loop-opt"], evidence: "historical" },
  { id: "cse-expensive-off", family: "cse", addFlags: ["-fno-expensive-optimizations"], evidence: "proven-routing" },
  { id: "reg-peephole-off", family: "register-allocation", addFlags: ["-fno-peephole"], evidence: "historical" },
  { id: "reg-strength-reduce-off", family: "register-allocation", addFlags: ["-fno-strength-reduce"], evidence: "proven-routing" },
  { id: "reg-regmove-off", family: "register-allocation", addFlags: ["-fno-regmove"], evidence: "proven-routing" },
  { id: "reg-caller-saves-off", family: "register-allocation", addFlags: ["-fno-caller-saves"], evidence: "historical" },
  { id: "reg-force-mem-off", family: "register-allocation", addFlags: ["-fno-force-mem"], evidence: "historical" },
  {
    id: "reg-peephole2-off",
    family: "register-allocation",
    addFlags: ["-fno-peephole2"],
    supportedCompilerFamilies: ["routed", "gcc296"],
    evidence: "historical",
  },
  { id: "inline-functions-off", family: "backend", addFlags: ["-fno-inline-functions"], evidence: "historical" },
  {
    id: "delete-null-checks-off",
    family: "backend",
    addFlags: ["-fno-delete-null-pointer-checks"],
    supportedCompilerFamilies: ["routed", "gcc296"],
    evidence: "historical",
  },
  {
    id: "sibling-calls-off",
    family: "backend",
    addFlags: ["-fno-optimize-sibling-calls"],
    supportedCompilerFamilies: ["routed", "gcc296"],
    evidence: "proven-routing",
  },
  { id: "comparison-canonicalization-off", family: "backend", addFlags: ["-fno-canonicalize-comparison"], evidence: "proven-routing" },
  ...FORK_MODES.map((flag) => ({
    id: flag.slice(2),
    family: "backend" as const,
    addFlags: [flag],
    evidence: "proven-routing" as const,
  })),
  ...[
    "-mliteral-before-shift",
    "-mcommutative-copy-constant",
    "-mprologue-next-high-reg",
    "-mcompare-only-and-tst",
  ].map((flag) => ({
    id: `agbcc-${flag.slice(2)}`,
    family: "backend" as const,
    addFlags: [flag],
    compilerFamily: "old-agbcc" as const,
    evidence: "proven-routing" as const,
  })),
];

function hash(...parts: Array<string | Uint8Array>): string {
  const digest = createHash("sha256");
  for (const part of parts) {
    digest.update(part);
    digest.update("\0");
  }
  return digest.digest("hex");
}

function acceptedCache(document: unknown, cacheKey: string): Score | null {
  if (document === null || typeof document !== "object") return null;
  const score = document as Partial<Score>;
  return score.cache_key === cacheKey && typeof score.compiled === "boolean" &&
    score.config !== undefined && Array.isArray(score.config.ids) && Array.isArray(score.config.flags)
    ? score as Score
    : null;
}

function compatible(modes: readonly Mode[]): boolean {
  const exclusive = new Set<string>();
  const flags = new Set<string>();
  for (const mode of modes) {
    if (mode.exclusive && exclusive.has(mode.family)) return false;
    if (mode.exclusive) exclusive.add(mode.family);
    for (const flag of [...(mode.addFlags ?? []), ...(mode.removeFlags ?? [])]) {
      if (flags.has(flag)) return false;
      flags.add(flag);
    }
  }
  const selectedCompilers = new Set(modes.flatMap((mode) =>
    mode.compilerFamily === undefined ? [] : [mode.compilerFamily]));
  if (selectedCompilers.size > 1) return false;
  const compiler = [...selectedCompilers][0] ?? "routed";
  if (modes.some((mode) =>
    mode.supportedCompilerFamilies !== undefined &&
    !mode.supportedCompilerFamilies.includes(compiler))) return false;
  if (compiler === "old-agbcc" && modes.some((mode) =>
    mode.family === "scheduler" ||
    mode.family === "backend" && mode.compilerFamily !== "old-agbcc")) return false;
  return true;
}

function configOf(modes: readonly Mode[]): Config {
  return {
    ids: modes.map((mode) => mode.id).sort(),
    flags: modes.flatMap((mode) => mode.addFlags ?? []),
    remove_flags: modes.flatMap((mode) => mode.removeFlags ?? []),
    compiler_family: modes.find((mode) => mode.compilerFamily !== undefined)?.compilerFamily ?? "routed",
  };
}

export function singleConfigs(): Config[] {
  return [{ ids: [], flags: [], remove_flags: [], compiler_family: "routed" },
    ...MODES.map((mode) => configOf([mode]))];
}

export function pairConfigs(limit = Number.POSITIVE_INFINITY): Config[] {
  const configs: Config[] = [];
  for (let left = 0; left < MODES.length; left++) {
    for (let right = left + 1; right < MODES.length; right++) {
      const modes = [MODES[left], MODES[right]];
      if (compatible(modes)) configs.push(configOf(modes));
    }
  }
  configs.sort((a, b) => a.ids.join("+").localeCompare(b.ids.join("+")));
  return configs.slice(0, limit);
}

export function rankedPairConfigs(singleResults: readonly Score[], limit: number): Config[] {
  const ranks = new Map(singleResults.map((row, index) => [row.config.ids[0] ?? "", {
    exact: row.evidence?.exact ?? false,
    floor: row.evidence?.differing_halfwords ?? Number.POSITIVE_INFINITY,
    index,
  }]));
  return pairConfigs().sort((left, right) => {
    const quality = (config: Config) => config.ids.reduce((sum, id) => {
      const rank = ranks.get(id);
      return sum + (rank?.exact ? -1_000_000 : 0) + (rank?.floor ?? 1_000_000);
    }, 0);
    return quality(left) - quality(right) ||
      left.ids.join("+").localeCompare(right.ids.join("+"));
  }).slice(0, limit);
}

export function tripleConfigs(seedIds: readonly string[], limit: number): Config[] {
  const seed = new Set(seedIds);
  const configs: Config[] = [];
  for (let a = 0; a < MODES.length; a++) {
    for (let b = a + 1; b < MODES.length; b++) {
      for (let c = b + 1; c < MODES.length; c++) {
        const modes = [MODES[a], MODES[b], MODES[c]];
        // A triple needs evidence: at least two of its constituents occurred in
        // a strong pair/single result.  This prevents cubic blind permutation.
        if (modes.filter((mode) => seed.has(mode.id)).length < 2 || !compatible(modes)) continue;
        configs.push(configOf(modes));
      }
    }
  }
  configs.sort((a, b) => a.ids.join("+").localeCompare(b.ids.join("+")));
  return configs.slice(0, limit);
}

// Compare each alternative historical compiler with every compatible pair of
// stock optimization-pass switches. These configurations are deterministic
// and do not depend on a candidate-specific near-match ranking.
export function historicalFamilyFactorialConfigs(): Config[] {
  const compilerModes = MODES.filter((mode) =>
    mode.family === "compiler" &&
    mode.compilerFamily !== undefined &&
    mode.compilerFamily !== "gcc296");
  const stockModes = MODES.filter((mode) =>
    mode.addFlags?.length === 1 &&
    STOCK_SWITCHES.includes(mode.addFlags[0] as typeof STOCK_SWITCHES[number]));
  const configs: Config[] = [];
  for (const compiler of compilerModes) {
    for (let left = 0; left < stockModes.length; left++) {
      for (let right = left + 1; right < stockModes.length; right++) {
        const modes = [compiler, stockModes[left], stockModes[right]];
        if (compatible(modes)) configs.push(configOf(modes));
      }
    }
  }
  return configs.sort((left, right) =>
    left.ids.join("+").localeCompare(right.ids.join("+")));
}

// Retained for callers of the old helper.  Search execution itself is phased.
export function combinations(pairs: boolean): string[][] {
  return [
    ...singleConfigs().map((config) => config.flags),
    ...(pairs ? pairConfigs().map((config) => config.flags) : []),
  ];
}

export function modeSweepOutputDirectory(source: string): string {
  const absolute = resolve(source);
  const identity = hash(relative(ROOT, absolute).split(sep).join("/"), readFileSync(absolute)).slice(0, 16);
  return join(ROOT, "out", "modesweep", `${basename(source, ".c")}-${identity}`);
}

function compilerSignature(): string {
  return hash(
    compilerBundleSignature(),
    ...["alchemy_gcc.ts", "match_m2c.ts", "integrate_matches.ts", "candidate_show.ts", "mode_sweep.ts"]
      .map((name) => readFileSync(join(ROOT, "tools", name))),
  );
}

function disassembly(binary: string): string[] {
  const dumped = Bun.spawnSync([
    "arm-none-eabi-objdump", "-D", "-b", "binary", "-m", "arm", "-M", "force-thumb", binary,
  ], { stdout: "pipe", stderr: "pipe" });
  if (dumped.exitCode !== 0) return [];
  return dumped.stdout.toString().split("\n").flatMap((line) => {
    const match = /^\s+[0-9a-f]+:\t[0-9a-f ]+\t(.*)$/.exec(line);
    return match === null ? [] : [match[1].trim()];
  });
}

function mnemonic(instruction: string): string {
  return instruction.split(/\s+/)[0] ?? "";
}

function normalizedRegisters(instruction: string): string {
  return instruction.replace(/\b(?:r(?:1[0-5]|[0-9])|sp|lr|pc)\b/gi, "REG");
}

function classify(actual: Buffer, reference: Buffer, actualAsm: string[], referenceAsm: string[]): Evidence {
  let differing = Math.abs(Math.ceil(actual.length / 2) - Math.ceil(reference.length / 2));
  const shared = Math.min(actual.length, reference.length) & ~1;
  for (let offset = 0; offset < shared; offset += 2) {
    if (actual.readUInt16LE(offset) !== reference.readUInt16LE(offset)) differing++;
  }
  const actualMnemonics = actualAsm.map(mnemonic);
  const referenceMnemonics = referenceAsm.map(mnemonic);
  const histogram = (items: string[]) => [...items].sort().join("\n");
  const branch = (items: string[]) => items.filter((item) => /^(?:b|bl|bx)/.test(item));
  const literals = (items: string[]) => items.filter((item) => /^ldr/.test(item) && /\bpc\b/.test(item));
  let registerProxy = 0;
  for (let index = 0; index < Math.min(actualAsm.length, referenceAsm.length); index++) {
    if (actualAsm[index] !== referenceAsm[index] &&
        normalizedRegisters(actualAsm[index]) === normalizedRegisters(referenceAsm[index])) registerProxy++;
  }
  return {
    differing_halfwords: differing,
    size_delta: actual.length - reference.length,
    exact: actual.equals(reference),
    exact_size: actual.length === reference.length,
    instruction_order_proxy:
      histogram(actualMnemonics) === histogram(referenceMnemonics) &&
      actualMnemonics.join("\n") !== referenceMnemonics.join("\n"),
    register_allocation_proxy: registerProxy,
    literal_placement_proxy: literals(actualAsm).join("\n") !== literals(referenceAsm).join("\n"),
    control_flow_proxy: branch(actualAsm).join("\n") !== branch(referenceAsm).join("\n"),
  };
}

function optionsOf(argv: string[]): Options {
  const options: Options = {
    source: "",
    rom: join(ROOT, "roms/gs1-en.gba"),
    pairs: false,
    triples: false,
    familyFactorial: false,
    jobs: Math.max(1, Math.min(10, (navigator.hardwareConcurrency || 8) - 2)),
    top: 16,
    maxPairs: 256,
    maxTriples: 64,
  };
  const rest: string[] = [];
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--rom") options.rom = argv[++index];
    else if (argument === "--pairs") options.pairs = true;
    else if (argument === "--triples") options.triples = options.pairs = true;
    else if (argument === "--family-factorial") options.familyFactorial = true;
    else if (argument === "--jobs") options.jobs = Number(argv[++index]);
    else if (argument === "--top") options.top = Number(argv[++index]);
    else if (argument === "--max-pairs") options.maxPairs = Number(argv[++index]);
    else if (argument === "--max-triples") options.maxTriples = Number(argv[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: mode_sweep.ts <candidate.c> [--pairs] [--triples] [--family-factorial] [--jobs N] [--top N] [--max-pairs N] [--max-triples N] [--rom FILE]");
      process.exit(0);
    } else rest.push(argument);
  }
  if (rest.length !== 1) throw new Error("usage: mode_sweep.ts <candidate.c> [--pairs] [--triples]");
  for (const [name, value] of [["jobs", options.jobs], ["top", options.top],
    ["max-pairs", options.maxPairs], ["max-triples", options.maxTriples]] as const) {
    if (!Number.isInteger(value) || value < 1) throw new Error(`${name} must be a positive integer`);
  }
  options.source = rest[0];
  return options;
}

function selfTest(): void {
  const singles = singleConfigs();
  if (singles[0].flags.length !== 0 || singles[0].compiler_family !== "routed") {
    throw new Error("default configuration must be first");
  }
  if (new Set(singles.map((item) => item.ids.join("+"))).size !== singles.length) {
    throw new Error("single configuration planning is not unique");
  }
  const exploredFlags = new Set(MODES.flatMap((mode) => mode.addFlags ?? []));
  const missingRouted = evidencedRoutingFlags().filter((flag) => !exploredFlags.has(flag));
  if (missingRouted.length > 0) {
    throw new Error(`routed compiler modes missing from explorer: ${missingRouted.join(", ")}`);
  }
  const pairs = pairConfigs();
  if (pairs.some((config) => config.ids.includes("opt-o1") && config.ids.includes("opt-o2"))) {
    throw new Error("incompatible optimization levels were paired");
  }
  if (canonicalJson(pairConfigs(17)) !== canonicalJson(pairConfigs(17))) {
    throw new Error("pair planning is nondeterministic");
  }
  const triples = tripleConfigs(["sched-postreload-off", "cse-gcse-off"], 8);
  if (triples.length === 0 || triples.some((config) =>
    config.ids.filter((id) => ["sched-postreload-off", "cse-gcse-off"].includes(id)).length < 2)) {
    throw new Error("triple planning escaped its evidence seed");
  }
  const factorial = historicalFamilyFactorialConfigs();
  if (factorial.length === 0 ||
      factorial.some((config) =>
        config.ids.length !== 3 ||
        config.compiler_family === "routed" ||
        config.compiler_family === "gcc296") ||
      !factorial.some((config) =>
        config.compiler_family === "gcc2951" &&
        config.ids.includes("cse-gcse-off") &&
        config.ids.includes("reg-regmove-off"))) {
    throw new Error("historical family factorial planning differs");
  }
  const reference = Buffer.from([0x01, 0x20, 0x02, 0x21]);
  const exact = classify(reference, reference, ["movs r0, #1"], ["movs r0, #1"]);
  if (!exact.exact || exact.differing_halfwords !== 0) throw new Error("exact classification differs");
  const registers = classify(
    Buffer.from([0x01, 0x20]), Buffer.from([0x01, 0x21]),
    ["movs r0, #1"], ["movs r1, #1"],
  );
  if (registers.register_allocation_proxy !== 1 || registers.differing_halfwords !== 1) {
    throw new Error("register classification differs");
  }
  const controlFlow = classify(
    Buffer.from([0x00, 0xe0]), Buffer.from([0x01, 0xe0]),
    ["b.n 0x4"], ["b.n 0x6"],
  );
  if (!controlFlow.control_flow_proxy) throw new Error("control-flow classification differs");
  const key1 = hash("source", "reference", canonicalJson(["-O1"]));
  const key2 = hash("source", "reference", canonicalJson(["-O1"]));
  if (key1 !== key2 || key1 === hash("changed", "reference", canonicalJson(["-O1"]))) {
    throw new Error("cache keys are not deterministic/content-sensitive");
  }
  const cached: Score = {
    config: { ids: ["opt-o1"], flags: ["-O1"], remove_flags: [], compiler_family: "routed" },
    cache_key: key1,
    cached: false,
    compiled: false,
  };
  if (acceptedCache(JSON.parse(JSON.stringify(cached)), key1) === null ||
      acceptedCache(cached, `${key1}stale`) !== null ||
      acceptedCache({ cache_key: key1 }, key1) !== null) {
    throw new Error("cache acceptance does not reject stale/malformed entries");
  }
  console.log("mode sweep self-test passed");
}

async function main(): Promise<void> {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) {
    selfTest();
    return;
  }
  const options = optionsOf(argv);
  const stem = basename(options.source, ".c");
  const address = Number.parseInt(stem, 16);
  const wanted = regionSize(stem);
  if (wanted === null) throw new Error(`no assembly region is recorded for ${stem}`);
  const rom = readFileSync(options.rom);
  const reference = rom.subarray(address - ROM_BASE, address - ROM_BASE + wanted);
  const output = modeSweepOutputDirectory(options.source);
  const cacheDirectory = join(output, "cache");
  mkdirSync(cacheDirectory, { recursive: true });
  const referenceBinary = join(output, "reference.bin");
  writeFileSync(referenceBinary, reference);
  const referenceAsm = disassembly(referenceBinary);
  const sourceBytes = readFileSync(options.source);
  const compilerDigest = compilerSignature();

  async function score(config: Config): Promise<Score> {
    const cacheKey = hash(String(FORMAT), sourceBytes, reference, compilerDigest, canonicalJson(config));
    const cachePath = join(cacheDirectory, `${cacheKey}.json`);
    if (existsSync(cachePath)) {
      const cached = acceptedCache(JSON.parse(readFileSync(cachePath, "utf8")), cacheKey);
      if (cached !== null) return { ...cached, cached: true };
    }
    const work = join(output, "scratch", cacheKey);
    mkdirSync(work, { recursive: true });
    let result: Score;
    try {
      await verifyCandidate(options.source, rom, work, config.flags, ROM_BASE, "gs1", {
        family: config.compiler_family,
        removeFlags: config.remove_flags,
      });
      const linked = readFileSync(join(work, `${stem}.bin`));
      const symbols = Bun.spawnSync(
        ["arm-none-eabi-nm", "-S", "--defined-only", join(work, `${stem}.elf`)],
        { stdout: "pipe", stderr: "pipe" },
      );
      const extent = linkedFunctionExtent(symbols.stdout.toString(), `Func_${stem}`, address, linked.length);
      const actual = linked.subarray(0, extent);
      const actualBinary = join(work, "actual.bin");
      writeFileSync(actualBinary, actual);
      result = {
        config,
        cache_key: cacheKey,
        cached: false,
        compiled: true,
        size: actual.length,
        evidence: classify(actual, reference, disassembly(actualBinary), referenceAsm),
      };
    } catch (error) {
      result = {
        config,
        cache_key: cacheKey,
        cached: false,
        compiled: false,
        error: error instanceof Error ? error.message.slice(0, 500) : String(error).slice(0, 500),
      };
    }
    writeFileSync(cachePath, canonicalJson(result) + "\n");
    return result;
  }

  async function phase(configs: Config[]): Promise<Score[]> {
    const results = new Array<Score>(configs.length);
    let cursor = 0;
    async function worker(): Promise<void> {
      while (true) {
        const index = cursor++;
        if (index >= configs.length) return;
        results[index] = await score(configs[index]);
      }
    }
    await Promise.all(Array.from({ length: Math.min(options.jobs, configs.length) }, worker));
    return results;
  }

  const singleResults = await phase(singleConfigs());
  const allPairs = pairConfigs();
  const pairPlan = options.pairs ? rankedPairConfigs(singleResults, options.maxPairs) : [];
  const pairResults = await phase(pairPlan);
  const bestSingleFloor = Math.min(...singleResults.flatMap((row) =>
    row.evidence === undefined ? [] : [row.evidence.differing_halfwords]));
  // A near-match alone justifies pairs, not cubic search.  A triple is only
  // evidenced when a compatible pair actually improves on every single-mode
  // result and leaves a small exact-sized floor.
  const strong = pairResults.filter((row) =>
    row.evidence?.exact_size &&
    row.evidence.differing_halfwords >= 2 &&
    row.evidence.differing_halfwords <= 5 &&
    row.evidence.differing_halfwords < bestSingleFloor);
  const seedIds = [...new Set(strong.flatMap((row) => row.config.ids))];
  const allTriples = tripleConfigs(seedIds, Number.POSITIVE_INFINITY);
  const triplePlan = options.triples ? allTriples.slice(0, options.maxTriples) : [];
  const tripleResults = triplePlan.length > 0 ? await phase(triplePlan) : [];
  const allFamilyFactorial = historicalFamilyFactorialConfigs();
  const familyFactorialPlan = options.familyFactorial ? allFamilyFactorial : [];
  const familyFactorialResults = familyFactorialPlan.length > 0
    ? await phase(familyFactorialPlan)
    : [];
  const results = [
    ...singleResults, ...pairResults, ...tripleResults, ...familyFactorialResults,
  ];
  const ranked = results.filter((row) => row.evidence !== undefined).sort((left, right) =>
    Number(right.evidence!.exact) - Number(left.evidence!.exact) ||
    left.evidence!.differing_halfwords - right.evidence!.differing_halfwords ||
    Math.abs(left.evidence!.size_delta) - Math.abs(right.evidence!.size_delta) ||
    left.config.flags.length - right.config.flags.length ||
    left.config.ids.join("+").localeCompare(right.config.ids.join("+")));
  const best = ranked[0] ?? null;
  const report = {
    format: FORMAT,
    stem,
    source: options.source,
    source_sha256: hash(sourceBytes),
    reference_sha256: hash(reference),
    compiler_signature: compilerDigest,
    policy: {
      families: ["routed", "gcc296", "old-agbcc", "pret-early-thumb", "gcc2951"],
      phases: ["routed-default", "single", ...(options.pairs ? ["compatible-pair"] : []),
        ...(options.triples ? ["evidence-supported-triple"] : []),
        ...(options.familyFactorial ? ["historical-family-stock-factorial"] : [])],
      triple_threshold_halfwords: [2, 5],
      auto_promote: false,
    },
    attempted: results.length,
    compiled: results.filter((row) => row.compiled).length,
    cache_hits: results.filter((row) => row.cached).length,
    planning: {
      pairs_planned: pairPlan.length,
      pairs_available: allPairs.length,
      triples_planned: triplePlan.length,
      triples_available: allTriples.length,
      family_factorial_planned: familyFactorialPlan.length,
      family_factorial_available: allFamilyFactorial.length,
      bounded_search_complete:
        options.pairs && pairPlan.length === allPairs.length &&
        (!options.familyFactorial ||
          familyFactorialPlan.length === allFamilyFactorial.length) &&
        (allTriples.length === 0 ||
          options.triples && triplePlan.length === allTriples.length),
    },
    strong_seed_ids: seedIds,
    results,
  };
  writeFileSync(join(output, "report.json"), canonicalJson(report) + "\n");
  const floor = {
    format: FORMAT,
    stem,
    source_sha256: report.source_sha256,
    reference_sha256: report.reference_sha256,
    compiler_signature: compilerDigest,
    searched: report.policy.phases,
    bounded_search_complete: report.planning.bounded_search_complete,
    exact: best?.evidence?.exact ?? false,
    irreducible_floor_halfwords: report.planning.bounded_search_complete
      ? best?.evidence?.differing_halfwords ?? null
      : null,
    best_observed_halfwords: best?.evidence?.differing_halfwords ?? null,
    best_config: best?.config ?? null,
    classification: best?.evidence ?? null,
    // A 2–5 halfword exhausted floor should move to RTL/scheduler tracing.
    escalation: report.planning.bounded_search_complete &&
      best?.evidence?.exact_size && best.evidence.differing_halfwords >= 2 &&
      best.evidence.differing_halfwords <= 5 && options.triples
      ? "compiler-rtl-scheduler-trace"
      : null,
  };
  writeFileSync(join(output, "floor.json"), canonicalJson(floor) + "\n");

  console.log(`region=${wanted}B attempted=${results.length} compiled=${report.compiled} cache_hits=${report.cache_hits}`);
  for (const row of ranked.slice(0, options.top)) {
    const evidence = row.evidence!;
    const tags = [
      evidence.exact ? "EXACT" : "",
      evidence.instruction_order_proxy ? "order" : "",
      evidence.register_allocation_proxy ? `register=${evidence.register_allocation_proxy}` : "",
      evidence.literal_placement_proxy ? "literal" : "",
      evidence.control_flow_proxy ? "cfg" : "",
    ].filter(Boolean).join(",");
    const mutation = [
      row.config.compiler_family,
      ...row.config.flags,
      ...row.config.remove_flags.map((flag) => `remove:${flag}`),
    ].join(" ");
    console.log(`${String(evidence.differing_halfwords).padStart(4)}hw size=${String(row.size).padStart(4)} ${mutation}${tags ? ` [${tags}]` : ""}`);
  }
  console.log(`report=${join(output, "report.json")} floor=${join(output, "floor.json")}`);
}

if (import.meta.main) await main();
