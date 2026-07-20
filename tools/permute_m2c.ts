#!/usr/bin/env bun
import {
  existsSync,
  mkdirSync,
  readFileSync,
  readdirSync,
  rmSync,
  writeFileSync,
} from "node:fs";
import { basename, dirname, join } from "node:path";
import { M2C_PREAMBLE } from "./match_m2c.ts";
import { verify } from "./verify.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const STATEMENT = /( *)((?:[\w\[\]().*>-]+ = )|(?:return ))M2C_FIELD\(\(\*\(s32 \*\)(0x[0-9A-Fa-f]+) \+ \((\w+) \* (\d+)\)\), (s32 \*|void \*\*|u32 \*|s16 \*|u8 \*|void \*), (0x[0-9A-Fa-f]+)\);/g;
const ADDRESS = /0x0[238][0-9A-Fa-f]{6}/g;

interface Options {
  rom: string;
  drafts: string;
  report: string;
}

interface MatchRow {
  entry: number;
  matched?: boolean;
  mismatched_bytes?: number;
}

export function stateBlock(body: string, swap: boolean): string {
  return body.replace(
    STATEMENT,
    (_whole, indent: string, target: string, address: string, index: string, stride: string, kind: string, offset: string) => {
      const terms = swap ? ["(s32)_mo", "(s32)_mb"] : ["(s32)_mb", "(s32)_mo"];
      const total = [...terms, `(s32)${offset}`].join(" + ");
      return `${indent}{ s32 _mb = *(s32 *)${address}; s32 _mo = ${index} * ${stride}; ${target}*(${kind})(${total}); }`;
    },
  );
}

export function liftConstants(body: string): string {
  const opening = /\)\s*\{/.exec(body);
  if (opening === null) return body;
  const cut = opening.index + opening[0].length;
  const constants = [...new Set(body.match(ADDRESS) ?? [])].sort();
  const declarations = constants.map((value, index) => `\n    s32 _c${index} = ${value};`).join("");
  let rest = body.slice(cut);
  for (const [index, value] of constants.entries()) rest = rest.replaceAll(value, `_c${index}`);
  return body.slice(0, cut) + declarations + rest;
}

// m2cは未使用の先頭引数を落とすため、arg0を先頭に補い署名を一つずらした
// 変種も試す。引数ゼロで呼ぶ既知関数への呼び出しにはarg0を転送する。
export function threadLeadingArgument(body: string): string {
  const signature = /(\w[\w* ]*)(Func_08[0-9a-f]{6})\(([^)]*)\) \{/;
  const match = signature.exec(body);
  if (match === null || /\barg0\b/.test(match[3])) return body;
  const parameters = match[3].trim() === "" || match[3].trim() === "void"
    ? "s32 arg0"
    : `s32 arg0, ${match[3]}`;
  let result = body.replace(signature, `$1$2(${parameters}) {`);
  const bareCall = /(\w[\w* ]*)(Func_08[0-9a-f]{6})\(\);/;
  const callee = bareCall.exec(result);
  if (callee !== null) {
    result = result
      .replace(`${callee[1]}${callee[2]}();`, `${callee[1]}${callee[2]}(s32);`)
      .replaceAll(`${callee[2]}();`, `${callee[2]}(arg0);`);
  }
  return result;
}

// asmファイルを組み立てて.textのバイト数を数え、C一致範囲と同じ長さの
// ときだけ置換可能と判定する。長い場合は末尾断片の分離が先に必要。
export function replaceableAssembly(stem: string, linkedSize: number, scratch: string): boolean {
  const source = join(ROOT, "asm", `${stem}.s`);
  if (!existsSync(source)) return true;
  const object = join(scratch, `${stem}.asmcheck.o`);
  const binary = join(scratch, `${stem}.asmcheck.bin`);
  const assembled = Bun.spawnSync(
    ["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", object, source],
    { stdout: "pipe", stderr: "pipe" },
  );
  if (assembled.exitCode !== 0) return false;
  const copied = Bun.spawnSync(
    ["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", object, binary],
    { stdout: "pipe", stderr: "pipe" },
  );
  if (copied.exitCode !== 0) return false;
  return readFileSync(binary).length === linkedSize;
}

// Regions with positive evidence for a permanent non-C source form. Explicit
// modules are listed in the classification; the numerous fixed long-call
// veneers are recognized from their canonical source form instead of copying
// 608 generated filenames into that document.
export function retainedAssemblyStems(): Set<string> {
  const result = new Set<string>();
  const classification = JSON.parse(
    readFileSync(join(ROOT, "asm", "classification.json"), "utf8"),
  ) as { groups?: Array<{ files?: string[] }> };
  for (const group of classification.groups ?? []) {
    for (const stem of group.files ?? []) result.add(stem.toLowerCase());
  }
  for (const name of readdirSync(join(ROOT, "asm")).filter((entry) => /^[0-9a-f]{8}\.s$/.test(entry))) {
    const stem = basename(name, ".s");
    const rows = readFileSync(join(ROOT, "asm", name), "utf8")
      .split(/\r?\n/)
      .map((row) => row.replace(/@.*$/, "").trim())
      .filter(Boolean);
    const entry = rows.indexOf(`Func_${stem}:`);
    if (entry < 0 || entry + 3 >= rows.length) continue;
    const load = /^ldr\s+r4,\s*\[pc,\s*#0\]$/.test(rows[entry + 1]);
    const branch = /^bx\s+r4$/.test(rows[entry + 2]);
    const literal = rows[entry + 3].match(/^\.4byte\s+(0x[0-9a-f]+)$/i);
    if (load && branch && literal !== null && (Number(literal[1]) & 1) !== 0) result.add(stem);
  }
  return result;
}

// ハードウェアレジスタとカートリッジRAMへの直接アクセスは原典では
// volatileであることが多く、命令の並びと持ち上げ抑制を変える。
export function volatileHardware(body: string): string {
  return body.replace(/\*\s*\((u8|s8|u16|s16|u32|s32) \*\)\s*(0x0[4E][0-9A-Fa-f]{6})/g, "*(volatile $1 *)$2");
}

// 「値の読み出しとポインタ前進」の二文はGCCの一命令ldmia（*p++）に
// ならないため、後置インクリメント形へ畳み込んだ変種も試す。
export function postIncrementWalk(body: string): string {
  return body.replace(
    /(\w+) = (\*\w+);\n(\s*)(\w+) \+= \d+;/g,
    (whole, target: string, load: string, _indent: string, pointer: string) =>
      load === `*${pointer}` ? `${target} = *${pointer}++;` : whole,
  );
}

export function candidates(body: string): string[] {
  const direct = [stateBlock(body, false), stateBlock(body, true), liftConstants(body), volatileHardware(body), postIncrementWalk(body)];
  return [...direct, threadLeadingArgument(body), ...direct.map(threadLeadingArgument)];
}

function usage(): void {
  console.log("usage: permute_m2c.ts [-h] [--drafts DRAFTS] [--report REPORT] [rom]");
}

function parseArguments(arguments_: string[]): Options {
  let rom = "gs1-en.gba";
  let drafts = join(ROOT, "work/m2c-all");
  let report = join(ROOT, "work/matches/m2c.json");
  let foundRom = false;
  for (let index = 0; index < arguments_.length; index++) {
    const argument = arguments_[index];
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    }
    if (argument === "--drafts" || argument === "--report") {
      const value = arguments_[++index];
      if (value === undefined) throw new Error(`argument ${argument}: expected one argument`);
      if (argument === "--drafts") drafts = value;
      else report = value;
      continue;
    }
    if (argument.startsWith("--drafts=") || argument.startsWith("--report=")) {
      const value = argument.slice(argument.indexOf("=") + 1);
      if (argument.startsWith("--drafts=")) drafts = value;
      else report = value;
      continue;
    }
    if (argument.startsWith("-")) throw new Error(`unrecognized argument: ${argument}`);
    if (foundRom) throw new Error(`unrecognized argument: ${argument}`);
    rom = argument;
    foundRom = true;
  }
  return { rom, drafts, report };
}

function main(): void {
  const options = parseArguments(Bun.argv.slice(2));
  const rom = readFileSync(options.rom);
  const output = join(ROOT, "out/permute");
  mkdirSync(output, { recursive: true });
  const tracked = new Set(
    readdirSync(join(ROOT, "src"))
      .filter((name) => name.endsWith(".c"))
      .map((name) => basename(name, ".c")),
  );
  const retained = retainedAssemblyStems();
  const document = JSON.parse(readFileSync(options.report, "utf8")) as MatchRow[] | { results?: MatchRow[] };
  const rows = Array.isArray(document) ? document : (document.results ?? []);
  const near = rows
    .filter((row) => !row.matched && (row.mismatched_bytes ?? 999) <= 24)
    .sort((left, right) => (left.mismatched_bytes ?? 999) - (right.mismatched_bytes ?? 999));

  let matched = 0;
  for (const row of near) {
    const stem = row.entry.toString(16).padStart(8, "0");
    if (tracked.has(stem) || retained.has(stem)) continue;
    const draft = join(options.drafts, `${stem}.c`);
    if (!existsSync(draft)) continue;
    const raw = readFileSync(draft, "utf8");
    const replacements: Array<string | null> = raw.includes("M2C_UNK")
      ? ["s32", "u32", "void *", "s16", "u16", "s8"]
      : [null];
    let hit = false;
    for (const replacement of replacements) {
      const base = replacement === null ? raw : raw.replaceAll("M2C_UNK", replacement);
      for (const body of candidates(base)) {
        if (body === base) continue;
        const candidate = join(output, `${stem}.c`);
        writeFileSync(candidate, M2C_PREAMBLE + body);
        let okay = false;
        let linkedSize = 0;
        try {
          [okay, linkedSize] = verify(candidate, rom, output);
        } catch {
          continue;
        }
        if (!okay) continue;
        // 旧asm領域がCの一致範囲より長い場合、末尾（別断片やプール）が
        // 無所属になる。組み立てバイト数が一致するときだけ置換する。
        if (!replaceableAssembly(stem, linkedSize, output)) {
          console.log(`skip ${stem}: asm region longer than matched C`);
          continue;
        }
        writeFileSync(join(ROOT, "src", `${stem}.c`), M2C_PREAMBLE + body);
        rmSync(join(ROOT, "asm", `${stem}.s`), { force: true });
        matched++;
        hit = true;
        console.log(`matched ${stem}`);
        break;
      }
      if (hit) break;
    }
  }
  console.log(`matched=${matched} of ${near.length} near-misses`);
}

if (import.meta.main) main();
