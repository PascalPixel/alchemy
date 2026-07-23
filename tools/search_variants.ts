#!/usr/bin/env bun
// Tool role: library; imported by tools/overlay_match.ts, tools/search_queue_variants.ts.
import { canonicalJson } from "./canonical_json.ts";
import {
  copyFileSync,
  existsSync,
  mkdirSync,
  readFileSync,
  readdirSync,
  rmSync,
  writeFileSync,
} from "node:fs";
import { basename, dirname, join } from "node:path";
import { ROM_BASE, verifyCandidate } from "./match_m2c.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const TYPES = ["s32", "u32", "s16", "u16", "s8", "u8", "void *"];
const DECLARATION = /^(?<return>[A-Za-z_][A-Za-z0-9_ ]*?\*?)\s+(?<name>Func_[0-9a-f]{8})\((?<params>[^;{}]*)\);$/gm;
const DEFINITION = /^(?<return>[A-Za-z_][A-Za-z0-9_ ]*?\*?)\s+(?<name>Func_[0-9a-f]{8})\((?<params>[^;{}]*)\)\s*\{/m;
const LOCAL = /^(?<indent>\s{4})(?<type>s32|u32|s16|u16|s8|u8|void \*)(?<space>\s+)(?<name>[A-Za-z_][A-Za-z0-9_]*)\s*;$/gm;
const SMALL_INTEGER = /^(?:\([su](?:8|16|32)\)\s*)?(?<literal>0[xX][0-9a-fA-F]+|[1-9][0-9]*)$/;
const COMPARISON_INTEGER = /(?:==|!=)\s*(?<literal>0[xX][0-9a-fA-F]+|[1-9][0-9]*)\b/g;

type Operation = [string, (source: string) => string];
type Task = [number, number, string, string];
type Result = Record<string, unknown> & {
  entry: number;
  variant: number;
  label: string;
  error?: string;
  size?: number;
  span?: number;
  mismatched_bytes?: number;
  common_prefix?: number;
  matched?: boolean;
  source?: string;
  match_source?: string;
};

interface Options {
  rom: string;
  candidates: string;
  spans: string;
  ranking: string | null;
  limit: number;
  variants: number;
  jobs: number;
  keep: number;
  output: string;
}

function replaceOnce(oldValue: string, newValue: string): (source: string) => string {
  return (source) => source.replace(oldValue, newValue);
}

export function declarationOperations(source: string): Operation[] {
  const operations: Operation[] = [];
  for (const match of source.matchAll(DECLARATION)) {
    const groups = match.groups!;
    const line = match[0];
    const returnType = groups.return.trim();
    for (const replacement of TYPES) {
      if (replacement === returnType) continue;
      const changed = line.replace(groups.return, replacement);
      operations.push([`${groups.name}:return=${replacement}`, replaceOnce(line, changed)]);
    }
    const parameters = groups.params.trim();
    if (!parameters || parameters === "void") continue;
    const fields = parameters.split(",").map((item) => item.trim());
    for (const [index, field] of fields.entries()) {
      if (!TYPES.includes(field)) continue;
      for (const replacement of TYPES) {
        if (replacement === field) continue;
        const changedFields = [...fields];
        changedFields[index] = replacement;
        const changed = line.replace(groups.params, changedFields.join(", "));
        operations.push([`${groups.name}:arg${index}=${replacement}`, replaceOnce(line, changed)]);
      }
    }
  }
  return operations;
}

export function definitionOperations(source: string): Operation[] {
  const operations: Operation[] = [];
  const match = DEFINITION.exec(source);
  if (match === null) return operations;
  const groups = match.groups!;
  const line = match[0];
  const returnType = groups.return.trim();
  for (const replacement of ["void", "s32", "u32", "s16", "u16"]) {
    if (replacement === returnType) continue;
    const changed = line.replace(groups.return, replacement);
    operations.push([`definition:return=${replacement}`, replaceOnce(line, changed)]);
  }
  const parameters = groups.params.trim();
  const fields = !parameters || parameters === "void"
    ? []
    : parameters.split(",").map((item) => item.trim());
  for (const [index, field] of fields.entries()) {
    const split = field.lastIndexOf(" ");
    if (split < 0) continue;
    const type = field.slice(0, split);
    const name = field.slice(split + 1);
    if (!TYPES.includes(type)) continue;
    for (const replacement of TYPES) {
      if (replacement === type) continue;
      const changedFields = [...fields];
      changedFields[index] = `${replacement} ${name}`;
      const changed = line.replace(groups.params, changedFields.join(", "));
      operations.push([`definition:arg${index}=${replacement}`, replaceOnce(line, changed)]);
    }
  }
  for (const count of [1, 2]) {
    const prefix = Array.from({ length: count }, (_, index) => `s32 unused${index}`).join(", ");
    const changedParameters = fields.length === 0 ? prefix : `${prefix}, ${parameters}`;
    const changed = line.replace(groups.params, changedParameters);
    operations.push([`definition:prepend=${count}`, replaceOnce(line, changed)]);
  }
  for (let index = 1; index <= fields.length; index++) {
    const changedFields = [...fields];
    changedFields.splice(index, 0, `s32 unused${index}`);
    const changed = line.replace(groups.params, changedFields.join(", "));
    operations.push([`definition:insert-unused=${index}`, replaceOnce(line, changed)]);
  }
  return operations;
}

export function localOperations(source: string): Operation[] {
  const operations: Operation[] = [];
  for (const match of source.matchAll(LOCAL)) {
    const groups = match.groups!;
    const line = match[0];
    const changed = line.replace(groups.type, `volatile ${groups.type}`);
    operations.push([`local:${groups.name}=volatile`, replaceOnce(line, changed)]);
  }
  return operations;
}

interface AddressLiteralSite {
  kind: "call" | "compare";
  start: number;
  end: number;
  value: number;
}

function directCallLiteralSites(source: string): AddressLiteralSite[] {
  const sites: AddressLiteralSite[] = [];
  const calls = /\bFunc_[0-9a-f]{8}\s*\(/g;
  for (const call of source.matchAll(calls)) {
    const open = call.index + call[0].lastIndexOf("(");
    let argumentStart = open + 1;
    let depth = 0;
    for (let cursor = open + 1; cursor < source.length; cursor++) {
      const character = source[cursor];
      if (character === "(") depth++;
      else if (character === ")" && depth > 0) depth--;
      else if ((character === "," || character === ")") && depth === 0) {
        const argument = source.slice(argumentStart, cursor);
        const trimmed = argument.trim();
        const integer = SMALL_INTEGER.exec(trimmed);
        if (integer?.groups !== undefined) {
          const literal = integer.groups.literal;
          const relative = argument.indexOf(literal);
          const start = argumentStart + relative;
          const value = Number.parseInt(literal, literal.toLowerCase().startsWith("0x") ? 16 : 10);
          if (value > 0 && value <= 0xffff) {
            sites.push({ kind: "call", start, end: start + literal.length, value });
          }
        }
        argumentStart = cursor + 1;
        if (character === ")") break;
      }
    }
  }
  return sites;
}

function addressLiteralSites(source: string): AddressLiteralSite[] {
  const sites = directCallLiteralSites(source);
  for (const comparison of source.matchAll(COMPARISON_INTEGER)) {
    const literal = comparison.groups!.literal;
    const relative = comparison[0].lastIndexOf(literal);
    const start = comparison.index + relative;
    const value = Number.parseInt(literal, literal.toLowerCase().startsWith("0x") ? 16 : 10);
    if (value > 0 && value <= 0xffff) {
      sites.push({ kind: "compare", start, end: start + literal.length, value });
    }
  }
  sites.sort((left, right) => left.start - right.start);
  return sites;
}

function declareAddressValue(source: string, value: number): string {
  const symbol = `Value_${value.toString(16).padStart(8, "0")}`;
  if (new RegExp(`^extern (?:u8|unsigned char) ${symbol};$`, "m").test(source)) return source;
  const declaration = `extern ${/^typedef unsigned char u8;$/m.test(source) ? "u8" : "unsigned char"} ${symbol};`;
  const typedefs = [...source.matchAll(/^typedef [^;]+;$/gm)];
  if (typedefs.length === 0) return `${declaration}\n\n${source}`;
  const last = typedefs[typedefs.length - 1];
  const insertion = last.index + last[0].length;
  return `${source.slice(0, insertion)}\n${declaration}${source.slice(insertion)}`;
}

// Address-valued small constants preserve relocation-backed pool loads
// (LAWS.md). Generate only call-argument and equality-test probes; byte
// verification remains the gate that distinguishes addresses from integers.
export function addressValueOperations(source: string): Operation[] {
  const operations: Operation[] = [];
  const ordinals = new Map<string, number>();
  for (const site of addressLiteralSites(source)) {
    const key = `${site.kind}:${site.value}`;
    const ordinal = ordinals.get(key) ?? 0;
    ordinals.set(key, ordinal + 1);
    const symbol = `Value_${site.value.toString(16).padStart(8, "0")}`;
    operations.push([`address:${site.kind}=${site.value.toString(16)}`, (body) => {
      const current = addressLiteralSites(body)
        .filter((candidate) => candidate.kind === site.kind && candidate.value === site.value)[ordinal];
      if (current === undefined) return body;
      const cast = /^typedef signed int s32;$/m.test(body) ? "s32" : "signed int";
      const changed = `${body.slice(0, current.start)}(${cast})&${symbol}${body.slice(current.end)}`;
      return declareAddressValue(changed, site.value);
    }]);
  }
  return operations;
}

// 揮発ストア柵の法則(LAWS.md)に基づく記憶アクセス限定子の変種。
// スケジューラはvolatileストアを越えて後続ロードを持ち上げない。
const CAST_TYPE = "(?:s8|u8|s16|u16|s32|u32|s64|u64|void|[A-Za-z_][A-Za-z0-9_]*)";
const BALANCED = "(?:[^()]|\\((?:[^()]|\\([^()]*\\))*\\))*";
const ASSIGN = "(?:=(?!=)|\\+=|-=|\\|=|&=|\\^=|<<=|>>=)";
const CAST_STORE = new RegExp(
  `\\*\\((${CAST_TYPE}) \\*\\)(\\(${BALANCED}\\)|0x[0-9a-fA-F]+|[A-Za-z_][A-Za-z0-9_]*)(\\s*)${ASSIGN}`,
  "g",
);
const FIELD_STORE = new RegExp(
  `M2C_FIELD\\(([^,]+), (${CAST_TYPE}) \\*, ([^)]+)\\)(\\s*)${ASSIGN}`,
  "g",
);
const CAST_ANY = new RegExp(`\\*\\((${CAST_TYPE}) \\*\\)`, "g");
const FIELD_ANY = new RegExp(`M2C_FIELD\\(([^,]+), (${CAST_TYPE}) \\*,`, "g");
const CAST_ABSOLUTE = new RegExp(`\\*\\((${CAST_TYPE}) \\*\\)(0x[0-9a-fA-F]+)`, "g");
const CAST_POINTER_ABSOLUTE = new RegExp(`\\*\\((${CAST_TYPE}) \\*\\*\\)(0x[0-9a-fA-F]+)`, "g");

function withoutVolatile(operation: (source: string) => string): (source: string) => string {
  return (source) => operation(source).replaceAll("volatile volatile ", "volatile ");
}

// 生成ドラフト正規化: 早置きされた一時ロード文を最初の使用直前へ沈める。
// バイト同一性検証が正しさの神託なので、別名解析は不要(誤変換は不一致で落ちる)。
const SIMPLE_STATEMENT = /^\s{4}(?:\([a-z0-9 ]+\)\s*)?(?<name>_v1[a-z]*\d+|temp_r\d+|var_r\d+)\s*=\s*(?<expr>[^;]*(?:M2C_FIELD|\*\s*\()[^;]*);\s*$/;
const BLOCK_BARRIER = /[{}]|^\s*(?:if|for|while|do|else|switch|return|goto)\b|:\s*$/;

export function sinkLoads(source: string): string {
  const lines = source.split("\n");
  for (let pass = 0; pass < 64; pass++) {
    let moved = false;
    for (let index = 0; index < lines.length; index++) {
      const statement = SIMPLE_STATEMENT.exec(lines[index]);
      if (statement === null) continue;
      const name = statement.groups!.name;
      const reference = new RegExp(`\\b${name}\\b`);
      let destination = index;
      for (let ahead = index + 1; ahead < lines.length; ahead++) {
        if (BLOCK_BARRIER.test(lines[ahead]) || reference.test(lines[ahead])) break;
        if (lines[ahead].trim() !== "") destination = ahead;
      }
      if (destination > index) {
        const [line] = lines.splice(index, 1);
        lines.splice(destination, 0, line);
        moved = true;
      }
    }
    if (!moved) break;
  }
  return lines.join("\n");
}

export function memoryOperations(source: string): Operation[] {
  const operations: Operation[] = [];
  const stores = (body: string): string => body
    .replace(CAST_STORE, (whole, type) => whole.replace(`*(${type} *)`, `*(volatile ${type} *)`))
    .replace(FIELD_STORE, (whole, _base, type) => whole.replace(`${type} *,`, `volatile ${type} *,`));
  const everything = (body: string): string => body
    .replace(CAST_ANY, "*(volatile $1 *)")
    .replace(FIELD_ANY, "M2C_FIELD($1, volatile $2 *,");
  const absolutes = (body: string): string => body
    .replace(CAST_POINTER_ABSOLUTE, "*($1 *volatile *)$2")
    .replace(CAST_ABSOLUTE, "*(volatile $1 *)$2");
  for (const [label, operation] of [
    ["memory:stores=volatile", stores],
    ["memory:all=volatile", everything],
    ["memory:absolute=volatile", absolutes],
    ["memory:sink-loads", sinkLoads],
    ["memory:sink-loads+stores=volatile", (body: string) => stores(sinkLoads(body))],
  ] as const) {
    const changed = withoutVolatile(operation);
    if (changed(source) !== source) operations.push([label, changed]);
  }
  return operations;
}

export function variants(source: string, maximum: number): Array<[string, string]> {
  const operations = [
    ...addressValueOperations(source),
    ...declarationOperations(source),
    ...definitionOperations(source),
    ...localOperations(source),
    ...memoryOperations(source),
  ];
  const generated: Array<[string, string]> = [["baseline", source]];
  for (const [label, operation] of operations) generated.push([label, operation(source)]);
  for (let leftIndex = 0; leftIndex < operations.length; leftIndex++) {
    const [leftLabel, left] = operations[leftIndex];
    for (const [rightLabel, right] of operations.slice(leftIndex + 1)) {
      generated.push([`${leftLabel}+${rightLabel}`, right(left(source))]);
      if (generated.length >= maximum * 2) break;
    }
    if (generated.length >= maximum * 2) break;
  }
  const unique: Array<[string, string]> = [];
  const seen = new Set<string>();
  for (const [label, body] of generated) {
    const digest = new Bun.CryptoHasher("sha256").update(body).digest("hex");
    if (seen.has(digest)) continue;
    seen.add(digest);
    unique.push([label, body]);
    if (unique.length >= maximum) break;
  }
  return unique;
}

function selfTest(): void {
  const source = [
    "typedef unsigned char u8;",
    "typedef signed int s32;",
    "",
    "s32 Func_08000000(s32, s32);",
    "s32 Func_08000004(void) {",
    "    if (Func_08000000(0xA, 2) != 0)",
    "        return 7;",
    "    return *(s32 *)0x02000000 == 1;",
    "}",
    "",
  ].join("\n");
  const generated = addressValueOperations(source).map(([label, operation]) => [label, operation(source)] as const);
  if (generated.length !== 3) throw new Error("address-value operation count self-test failed");
  const call = generated.find(([label]) => label === "address:call=a")?.[1] ?? "";
  const compare = generated.find(([label]) => label === "address:compare=1")?.[1] ?? "";
  if (!call.includes("extern u8 Value_0000000a;") || !call.includes("Func_08000000((s32)&Value_0000000a, 2)")) {
    throw new Error("address-value call self-test failed");
  }
  if (!compare.includes("extern u8 Value_00000001;") || !compare.includes("== (s32)&Value_00000001")) {
    throw new Error("address-value comparison self-test failed");
  }
  if (generated.some(([, body]) => body.includes("Value_00000007"))) {
    throw new Error("address-value arithmetic self-test failed");
  }
  console.log("self-test=ok");
}

async function parallelMap<T, R>(items: T[], jobs: number, operation: (item: T) => Promise<R>): Promise<R[]> {
  const results = new Array<R>(items.length);
  let next = 0;
  async function worker(): Promise<void> {
    while (true) {
      const index = next++;
      if (index >= items.length) return;
      results[index] = await operation(items[index]);
    }
  }
  await Promise.all(Array.from({ length: Math.min(jobs, items.length) }, worker));
  return results;
}

function numberOption(argument: string, value: string | undefined): number {
  if (value === undefined) throw new Error(`argument ${argument}: expected one argument`);
  const parsed = Number.parseInt(value, 10);
  if (!Number.isInteger(parsed)) throw new Error(`invalid int value: ${value}`);
  return parsed;
}

function usage(): void {
  console.log("usage: search_variants.ts [-h] --spans SPANS [--ranking RANKING] [--limit LIMIT] [--variants VARIANTS] [--jobs JOBS] [--keep KEEP] [-o OUTPUT] rom candidates");
}

function parseArguments(arguments_: string[]): Options {
  const positional: string[] = [];
  let spans: string | null = null;
  let ranking: string | null = null;
  let limit = 20;
  let variantCount = 128;
  let jobs = Math.min(18, navigator.hardwareConcurrency || 1);
  let keep = 5;
  let output = join(ROOT, "work/variants");
  for (let index = 0; index < arguments_.length; index++) {
    let argument = arguments_[index];
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    }
    const equal = argument.indexOf("=");
    const inline = equal >= 0 ? argument.slice(equal + 1) : undefined;
    if (equal >= 0) argument = argument.slice(0, equal);
    if (["--spans", "--ranking", "--limit", "--variants", "--jobs", "--keep", "-o", "--output"].includes(argument)) {
      const value = inline ?? arguments_[++index];
      if (value === undefined) throw new Error(`argument ${argument}: expected one argument`);
      if (argument === "--spans") spans = value;
      else if (argument === "--ranking") ranking = value;
      else if (argument === "--limit") limit = numberOption(argument, value);
      else if (argument === "--variants") variantCount = numberOption(argument, value);
      else if (argument === "--jobs") jobs = numberOption(argument, value);
      else if (argument === "--keep") keep = numberOption(argument, value);
      else output = value;
      continue;
    }
    if (argument.startsWith("-")) throw new Error(`unrecognized argument: ${argument}`);
    positional.push(argument);
  }
  if (positional.length !== 2) throw new Error("the following arguments are required: rom, candidates");
  if (spans === null) throw new Error("the following arguments are required: --spans");
  if (jobs < 1) throw new Error("--jobs must be at least 1");
  return {
    rom: positional[0], candidates: positional[1], spans, ranking,
    limit, variants: variantCount, jobs, keep, output,
  };
}

function compareNumbers(...values: number[]): number {
  for (const value of values) if (value !== 0) return value;
  return 0;
}

async function main(): Promise<void> {
  const options = parseArguments(Bun.argv.slice(2));
  const rom = readFileSync(options.rom);
  const spanRows = JSON.parse(readFileSync(options.spans, "utf8")) as Array<{ entry: number; size: number }>;
  const spans = new Map(spanRows.map((item) => [item.entry, item.size]));
  const tracked = new Set(
    readdirSync(join(ROOT, "src"))
      .filter((name) => name.endsWith(".c"))
      .map((name) => Number.parseInt(basename(name, ".c"), 16)),
  );
  const manifestPath = join(ROOT, "out/full/asm/manifest.json");
  const eligible = existsSync(manifestPath)
    ? new Set((JSON.parse(readFileSync(manifestPath, "utf8")) as {
        regions: Array<{ address: number; retention: string }>;
      }).regions.filter((region) => region.retention === "c_candidate")
        .map((region) => region.address))
    : null;
  let addresses = readdirSync(options.candidates)
    .filter((name) => name.endsWith(".c"))
    .sort()
    .map((name) => Number.parseInt(basename(name, ".c"), 16))
    .filter((address) => spans.has(address) && !tracked.has(address) &&
      (eligible === null || eligible.has(address)));
  if (options.ranking !== null) {
    const ranking = JSON.parse(readFileSync(options.ranking, "utf8")) as Array<Record<string, unknown>>;
    const scores = new Map<number, [number, number, number]>();
    for (const item of ranking) {
      if (item.matched === true) continue;
      scores.set(Number(item.entry), [
        Number(item.mismatched_bytes ?? 1 << 30),
        -Number(item.common_prefix ?? 0),
        Number(item.size ?? 1 << 30),
      ]);
    }
    const missing: [number, number, number] = [1 << 30, 0, 1 << 30];
    addresses.sort((left, right) => {
      const leftScore = scores.get(left) ?? missing;
      const rightScore = scores.get(right) ?? missing;
      return compareNumbers(
        leftScore[0] - rightScore[0],
        leftScore[1] - rightScore[1],
        leftScore[2] - rightScore[2],
      );
    });
  }
  addresses = addresses.slice(0, options.limit);

  const scratch = join(options.output, "scratch");
  const matches = join(options.output, "matches");
  rmSync(scratch, { recursive: true, force: true });
  mkdirSync(scratch, { recursive: true });
  mkdirSync(matches, { recursive: true });
  const tasks: Task[] = [];
  for (const address of addresses) {
    const stem = address.toString(16).padStart(8, "0");
    const source = readFileSync(join(options.candidates, `${stem}.c`), "utf8");
    for (const [index, [label, body]] of variants(source, options.variants).entries()) {
      tasks.push([address, index, label, body]);
    }
  }

  const results = await parallelMap(tasks, options.jobs, async ([address, index, label, body]): Promise<Result> => {
    const stem = address.toString(16).padStart(8, "0");
    const directory = join(scratch, stem, index.toString().padStart(4, "0"));
    mkdirSync(directory, { recursive: true });
    const candidate = join(directory, `${stem}.c`);
    writeFileSync(candidate, body);
    let actual: Buffer;
    let size: number;
    try {
      ({ actual, size } = await verifyCandidate(candidate, rom, directory));
    } catch (error) {
      return {
        entry: address,
        variant: index,
        label,
        error: error instanceof Error ? error.constructor.name : "Error",
      };
    }
    const span = spans.get(address)!;
    const expected = rom.subarray(address - ROM_BASE, address - ROM_BASE + span);
    const overlap = Math.min(actual.length, expected.length);
    let mismatch = 0;
    for (let offset = 0; offset < overlap; offset++) if (actual[offset] !== expected[offset]) mismatch++;
    mismatch += Math.abs(actual.length - expected.length);
    let prefix = 0;
    while (prefix < overlap && actual[prefix] === expected[prefix]) prefix++;
    return {
      entry: address,
      variant: index,
      label,
      size,
      span,
      mismatched_bytes: mismatch,
      common_prefix: prefix,
      matched: size === span && actual.equals(expected),
      source: candidate,
    };
  });

  const grouped = new Map<number, Result[]>();
  for (const result of results) {
    const rows = grouped.get(result.entry) ?? [];
    rows.push(result);
    grouped.set(result.entry, rows);
  }
  const report: Array<{ entry: number; tested: number; best: Result[] }> = [];
  let exact = 0;
  for (const address of addresses) {
    const rows = (grouped.get(address) ?? []).filter((row) => row.error === undefined);
    rows.sort((left, right) => compareNumbers(
      Number(!left.matched) - Number(!right.matched),
      left.mismatched_bytes! - right.mismatched_bytes!,
      -left.common_prefix! + right.common_prefix!,
      Math.abs(left.size! - left.span!) - Math.abs(right.size! - right.span!),
      left.variant - right.variant,
    ));
    const best = rows.slice(0, options.keep);
    if (best.length > 0 && best[0].matched) {
      exact++;
      const destination = join(matches, `${address.toString(16).padStart(8, "0")}.c`);
      copyFileSync(best[0].source!, destination);
      best[0].match_source = destination;
    }
    report.push({ entry: address, tested: (grouped.get(address) ?? []).length, best });
  }
  mkdirSync(options.output, { recursive: true });
  writeFileSync(join(options.output, "report.json"), canonicalJson(report) + "\n");
  console.log(`targets=${addresses.length} variants=${tasks.length} exact=${exact}`);
  for (const item of report) {
    if (item.best.length === 0) continue;
    const best = item.best[0];
    console.log(
      `${item.entry.toString(16).padStart(8, "0")} matched=${best.matched ? "True" : "False"} ` +
      `mismatch=${best.mismatched_bytes} prefix=${best.common_prefix} label=${best.label}`,
    );
  }
}

if (import.meta.main) {
  if (Bun.argv.length === 3 && Bun.argv[2] === "--self-test") selfTest();
  else await main();
}
