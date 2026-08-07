#!/usr/bin/env bun
// Recompile a deterministic slice of the exact-C corpus under proposed extra
// compiler flags and report every byte regression. This is a read-only
// validator: it never changes source routing or promotes candidates.
import { createHash } from "node:crypto";
import {
  existsSync,
  mkdirSync,
  readFileSync,
  renameSync,
  writeFileSync,
} from "node:fs";
import { basename, dirname, isAbsolute, join } from "node:path";
import { canonicalJson } from "../lib/canonical_json.ts";
import { compilerBundleSignature } from "../lib/alchemy_gcc.ts";
import { linkedFunctionExtent } from "../lib/integrate_matches.ts";
import {
  ROM_BASE,
  verifyCandidate,
  type CandidateCompilerConfiguration,
  type CandidateCompilerFamily,
} from "../lib/match_m2c.ts";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
const FORMAT = 3;

interface Region {
  source: string;
  size: number;
}

export interface Member {
  stem: string;
  source: string;
  address: number;
  size: number;
}

interface Options {
  flags: string[];
  configPath: string | null;
  compilerConfig: CandidateCompilerConfiguration;
  rom: string;
  manifest: string;
  cache: string;
  sample: number;
  seed: string;
  jobs: number;
  family: string | null;
  near: number | null;
  radius: number;
  sources: Set<string>;
  report: string | null;
}

interface Result {
  stem: string;
  source: string;
  cache_key: string;
  cached: boolean;
  compiled: boolean;
  exact: boolean;
  expected_size: number;
  actual_size?: number;
  differing_bytes?: number;
  first_difference?: number;
  error?: string;
}

function hash(...parts: Array<string | Uint8Array>): string {
  const digest = createHash("sha256");
  for (const part of parts) {
    digest.update(part);
    digest.update("\0");
  }
  return digest.digest("hex");
}

function hexadecimalStem(value: string): string {
  const stem = basename(value).replace(/\.[^.]+$/, "").toLowerCase();
  if (!/^[0-9a-f]{8}$/.test(stem)) throw new Error(`invalid source/address: ${value}`);
  return stem;
}

export function flagsOf(value: unknown): string[] {
  if (!Array.isArray(value) || !value.every((item) => typeof item === "string" && item.length > 0)) {
    throw new Error("compiler flags must be an array of non-empty strings");
  }
  return [...new Set(value)];
}

function compilerFamilyOf(value: unknown): CandidateCompilerFamily {
  if (
    value === "routed" || value === "gcc296" || value === "old-agbcc" ||
    value === "pret-early-thumb" || value === "gcc2951" || value === "gcc3"
  ) return value;
  throw new Error("compiler config family must be routed, gcc296, old-agbcc, pret-early-thumb, gcc2951, or gcc3");
}

export function compilerConfigurationOf(value: unknown): CandidateCompilerConfiguration {
  if (value === null || typeof value !== "object" || Array.isArray(value)) {
    throw new Error("compiler config must be a JSON object");
  }
  const document = value as Record<string, unknown>;
  if (document.config !== undefined) return compilerConfigurationOf(document.config);
  const allowed = new Set([
    "ids", "family", "addFlags", "removeFlags",
    "compiler_family", "flags", "remove_flags",
  ]);
  const unexpected = Object.keys(document).filter((key) => !allowed.has(key));
  if (unexpected.length > 0) {
    throw new Error(`unknown compiler config field${unexpected.length === 1 ? "" : "s"}: ${unexpected.join(", ")}`);
  }
  if (document.family !== undefined && document.compiler_family !== undefined &&
      document.family !== document.compiler_family) {
    throw new Error("compiler config contains conflicting family fields");
  }
  if (document.addFlags !== undefined && document.flags !== undefined) {
    throw new Error("compiler config contains conflicting added-flag fields");
  }
  if (document.removeFlags !== undefined && document.remove_flags !== undefined) {
    throw new Error("compiler config contains conflicting removed-flag fields");
  }
  const familyValue = document.family ?? document.compiler_family;
  const family = familyValue === undefined ? "routed" : compilerFamilyOf(familyValue);
  const addValue = document.addFlags ?? document.flags;
  const removeValue = document.removeFlags ?? document.remove_flags;
  const addFlags = addValue === undefined ? [] : flagsOf(addValue);
  const removeFlags = removeValue === undefined ? [] : flagsOf(removeValue);
  const overlap = addFlags.filter((flag) => removeFlags.includes(flag));
  if (overlap.length > 0) throw new Error(`compiler config both adds and removes: ${overlap.join(", ")}`);
  return { family, addFlags, removeFlags };
}

function parseOptions(argv: string[]): Options {
  const options: Options = {
    flags: [],
    configPath: null,
    compilerConfig: { family: "routed", addFlags: [], removeFlags: [] },
    rom: join(ROOT, "roms/gs1-en.gba"),
    manifest: join(ROOT, "out/full/claimed/manifest.json"),
    cache: join(ROOT, "out/compiler-corpus-regression"),
    sample: 64,
    seed: "exact-c-corpus-v1",
    jobs: 4,
    family: null,
    near: null,
    radius: 0x10000,
    sources: new Set(),
    report: null,
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    const take = (): string => {
      const value = argv[++index];
      if (value === undefined) throw new Error(`missing value after ${argument}`);
      return value;
    };
    if (argument === "--flags") options.flags.push(...take().split(",").filter(Boolean));
    else if (argument === "--config") options.configPath = take();
    else if (argument === "--rom") options.rom = take();
    else if (argument === "--manifest") options.manifest = take();
    else if (argument === "--cache") options.cache = take();
    else if (argument === "--sample") options.sample = Number.parseInt(take(), 10);
    else if (argument === "--seed") options.seed = take();
    else if (argument === "--jobs") options.jobs = Number.parseInt(take(), 10);
    else if (argument === "--family") options.family = take().toLowerCase();
    else if (argument === "--near") options.near = Number.parseInt(hexadecimalStem(take()), 16);
    else if (argument === "--radius") options.radius = Number.parseInt(take(), 0);
    else if (argument === "--sources") {
      for (const source of take().split(",").filter(Boolean)) options.sources.add(hexadecimalStem(source));
    } else if (argument === "--report") options.report = take();
    else if (argument === "-h" || argument === "--help") {
      console.log([
        "usage: compiler_corpus_regression.ts (--flags FLAG[,FLAG...] | --config FILE) [options]",
        "  --config FILE       CandidateCompilerConfiguration JSON:",
        "                      {\"family\":\"routed|gcc296|old-agbcc|pret-early-thumb|gcc2951|gcc3\",",
        "                       \"addFlags\":[...],\"removeFlags\":[...]}",
        "  --flags FLAGS       compatibility shorthand for additional flags",
        "  --sample N          deterministic sample size; 0 selects all (default 64)",
        "  --seed TEXT         deterministic sampling seed",
        "  --family PREFIX     restrict source stems to an address prefix",
        "  --near ADDRESS      restrict to address neighbors",
        "  --radius BYTES      neighbor radius (default 0x10000)",
        "  --sources A,B       restrict to explicit source stems",
        "  --jobs N            parallel compiler jobs (default 4)",
        "  --report FILE       also write the canonical JSON report",
      ].join("\n"));
      process.exit(0);
    } else throw new Error(`unknown argument: ${argument}`);
  }
  if (options.configPath !== null) {
    options.compilerConfig = compilerConfigurationOf(
      JSON.parse(readFileSync(options.configPath, "utf8")),
    );
  }
  options.flags = [...new Set(options.flags)];
  if (options.configPath === null &&
      options.flags.length === 0 &&
      options.compilerConfig.family === "routed" &&
      options.compilerConfig.addFlags?.length === 0 &&
      options.compilerConfig.removeFlags?.length === 0) {
    throw new Error("provide --flags or a compiler configuration that changes the routed baseline");
  }
  if (!Number.isInteger(options.sample) || options.sample < 0) throw new Error("--sample must be >= 0");
  if (!Number.isInteger(options.jobs) || options.jobs < 1 || options.jobs > 32) {
    throw new Error("--jobs must be between 1 and 32");
  }
  if (!Number.isInteger(options.radius) || options.radius < 0) throw new Error("--radius must be >= 0");
  return options;
}

export function deterministicSample(members: readonly Member[], count: number, seed: string): Member[] {
  const ordered = [...members].sort((left, right) => {
    const a = hash(seed, left.stem);
    const b = hash(seed, right.stem);
    return a.localeCompare(b) || left.stem.localeCompare(right.stem);
  });
  return (count === 0 ? ordered : ordered.slice(0, count)).sort((a, b) => a.address - b.address);
}

export function byteDifference(actual: Uint8Array, expected: Uint8Array): {
  count: number;
  first: number | null;
} {
  const extent = Math.max(actual.length, expected.length);
  let count = 0;
  let first: number | null = null;
  for (let offset = 0; offset < extent; offset++) {
    if (actual[offset] === expected[offset] && offset < actual.length && offset < expected.length) continue;
    count++;
    if (first === null) first = offset;
  }
  return { count, first };
}

function corpus(options: Options): Member[] {
  const document = JSON.parse(readFileSync(options.manifest, "utf8")) as { regions?: Region[] };
  const found = new Map<string, Member>();
  for (const region of document.regions ?? []) {
    if (!region.source.startsWith("src/") || !region.source.endsWith(".c")) continue;
    const stem = hexadecimalStem(region.source);
    const address = Number.parseInt(stem, 16);
    const source = isAbsolute(region.source) ? region.source : join(ROOT, region.source);
    if (!existsSync(source)) continue;
    if (options.family !== null && !stem.startsWith(options.family)) continue;
    if (options.near !== null && Math.abs(address - options.near) > options.radius) continue;
    if (options.sources.size > 0 && !options.sources.has(stem)) continue;
    found.set(stem, { stem, source, address, size: region.size });
  }
  return [...found.values()].sort((a, b) => a.address - b.address);
}

function cachedResult(document: unknown, key: string): Result | null {
  if (document === null || typeof document !== "object") return null;
  const result = document as Partial<Result>;
  if (result.cache_key !== key || typeof result.stem !== "string" ||
      typeof result.compiled !== "boolean" || typeof result.exact !== "boolean") return null;
  return result as Result;
}

function atomicJson(path: string, value: unknown): void {
  mkdirSync(dirname(path), { recursive: true });
  const temporary = `${path}.${process.pid}.tmp`;
  writeFileSync(temporary, canonicalJson(value) + "\n");
  renameSync(temporary, path);
}

async function parallelMap<T, R>(items: readonly T[], jobs: number, operation: (item: T) => Promise<R>): Promise<R[]> {
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

function selfTest(): void {
  const members: Member[] = [
    { stem: "08000010", source: "b", address: 0x08000010, size: 4 },
    { stem: "08000000", source: "a", address: 0x08000000, size: 4 },
    { stem: "08000020", source: "c", address: 0x08000020, size: 4 },
  ];
  const first = deterministicSample(members, 2, "seed").map((member) => member.stem);
  const second = deterministicSample([...members].reverse(), 2, "seed").map((member) => member.stem);
  if (first.join(",") !== second.join(",") || first.length !== 2) {
    throw new Error("deterministic sampling self-test failed");
  }
  const difference = byteDifference(Buffer.from([1, 2, 3]), Buffer.from([1, 4]));
  if (difference.count !== 2 || difference.first !== 1) throw new Error("byte difference self-test failed");
  if (flagsOf(["-O2", "-O2", "-fno-gcse"]).join(",") !== "-O2,-fno-gcse") {
    throw new Error("flag normalization self-test failed");
  }
  const config = compilerConfigurationOf({
    family: "old-agbcc",
    addFlags: ["-O2", "-O2"],
    removeFlags: ["-fcall-used-r4"],
  });
  if (config.family !== "old-agbcc" ||
      config.addFlags?.join(",") !== "-O2" ||
      config.removeFlags?.join(",") !== "-fcall-used-r4") {
    throw new Error("structured compiler configuration self-test failed");
  }
  const sweepConfig = compilerConfigurationOf({
    ids: ["compiler-old-agbcc"],
    compiler_family: "old-agbcc",
    flags: ["-O1"],
    remove_flags: [],
  });
  if (sweepConfig.family !== "old-agbcc" || sweepConfig.addFlags?.join(",") !== "-O1") {
    throw new Error("mode-sweep compiler configuration compatibility failed");
  }
  for (const invalid of [
    { family: "unknown" },
    { addFlags: "-O2" },
    { addFlags: ["-O2"], removeFlags: ["-O2"] },
    { family: "gcc296", compiler_family: "old-agbcc" },
  ]) {
    let rejected = false;
    try {
      compilerConfigurationOf(invalid);
    } catch {
      rejected = true;
    }
    if (!rejected) throw new Error("invalid compiler configuration was accepted");
  }
  const example: Result = {
    stem: "08000000", source: "src/08000000.c", cache_key: "key",
    cached: false, compiled: true, exact: true, expected_size: 4,
  };
  if (cachedResult(example, "key") === null || cachedResult(example, "stale") !== null ||
      cachedResult({ cache_key: "key" }, "key") !== null) {
    throw new Error("cache validation self-test failed");
  }
  console.log("compiler corpus regression self-test passed");
}

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const options = parseOptions(Bun.argv.slice(2));
  const available = corpus(options);
  const selected = deterministicSample(available, options.sample, options.seed);
  if (selected.length === 0) throw new Error("no exact-C sources matched the selection filters");

  const rom = readFileSync(options.rom);
  const compilerSignature = hash(
    compilerBundleSignature(),
    ...["alchemy_gcc.ts", "match_m2c.ts", "integrate_matches.ts", "compiler_corpus_regression.ts"]
      .map((name) => readFileSync(join(ROOT, "tools", name))),
  );
  mkdirSync(options.cache, { recursive: true });

  const results = await parallelMap(selected, options.jobs, async (member): Promise<Result> => {
    const expected = rom.subarray(member.address - ROM_BASE, member.address - ROM_BASE + member.size);
    const key = hash(
      String(FORMAT), readFileSync(member.source), expected, compilerSignature,
      canonicalJson({ flags: options.flags, compiler_config: options.compilerConfig }),
    );
    const cachePath = join(options.cache, "cache", `${key}.json`);
    if (existsSync(cachePath)) {
      const accepted = cachedResult(JSON.parse(readFileSync(cachePath, "utf8")), key);
      if (accepted !== null) return { ...accepted, cached: true };
    }

    const scratch = join(options.cache, "scratch", key);
    mkdirSync(scratch, { recursive: true });
    let result: Result;
    try {
      const verification = await verifyCandidate(
        member.source, rom, scratch, options.flags, ROM_BASE, "gs1", options.compilerConfig,
      );
      const linked = readFileSync(join(scratch, `${member.stem}.bin`));
      const symbols = Bun.spawnSync(
        ["arm-none-eabi-nm", "-S", "--defined-only", join(scratch, `${member.stem}.elf`)],
        { stdout: "pipe", stderr: "pipe" },
      );
      if (symbols.exitCode !== 0) throw new Error(symbols.stderr.toString().trim() || "nm failed");
      const extent = options.compilerConfig.family === "gcc2951"
        ? verification.actual.length
        : linkedFunctionExtent(
          symbols.stdout.toString(), `Func_${member.stem}`, member.address, linked.length,
        );
      const actual = linked.subarray(0, extent);
      const difference = byteDifference(actual, expected);
      result = {
        stem: member.stem,
        source: member.source.slice(ROOT.length + 1),
        cache_key: key,
        cached: false,
        compiled: true,
        exact: difference.count === 0,
        expected_size: expected.length,
        actual_size: actual.length,
        differing_bytes: difference.count,
        ...(difference.first === null ? {} : { first_difference: difference.first }),
      };
    } catch (error) {
      result = {
        stem: member.stem,
        source: member.source.slice(ROOT.length + 1),
        cache_key: key,
        cached: false,
        compiled: false,
        exact: false,
        expected_size: expected.length,
        error: error instanceof Error ? error.message.slice(0, 600) : String(error).slice(0, 600),
      };
    }
    atomicJson(cachePath, result);
    return result;
  });

  const regressions = results.filter((result) => !result.exact);
  const report = {
    format: FORMAT,
    flags: options.flags,
    compiler_config: options.compilerConfig,
    filters: {
      sample: options.sample,
      seed: options.seed,
      family: options.family,
      near: options.near === null ? null : `0x${options.near.toString(16)}`,
      radius: options.radius,
      sources: [...options.sources].sort(),
    },
    available: available.length,
    selected: selected.length,
    cached: results.filter((result) => result.cached).length,
    exact: results.length - regressions.length,
    regressions: regressions.length,
    results,
  };
  if (options.report !== null) atomicJson(options.report, report);

  console.log(
    `family=${options.compilerConfig.family} flags=${[
      ...(options.compilerConfig.addFlags ?? []), ...options.flags,
    ].join(",")} remove=${(options.compilerConfig.removeFlags ?? []).join(",")}` +
    ` available=${available.length} selected=${selected.length} cached=${report.cached}`,
  );
  console.log(`exact=${report.exact} regressions=${report.regressions}`);
  for (const result of regressions) {
    if (!result.compiled) console.log(`REGRESSION ${result.stem} compile_error=${result.error}`);
    else {
      console.log(
        `REGRESSION ${result.stem} size=${result.actual_size}/${result.expected_size}` +
        ` differing_bytes=${result.differing_bytes} first=0x${result.first_difference!.toString(16)}`,
      );
    }
  }
  if (regressions.length > 0) process.exitCode = 1;
}

if (import.meta.main) await main();
