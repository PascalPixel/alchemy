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

export function candidates(body: string): string[] {
  return [stateBlock(body, false), stateBlock(body, true), liftConstants(body)];
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
  const document = JSON.parse(readFileSync(options.report, "utf8")) as MatchRow[] | { results?: MatchRow[] };
  const rows = Array.isArray(document) ? document : (document.results ?? []);
  const near = rows
    .filter((row) => !row.matched && (row.mismatched_bytes ?? 999) <= 4)
    .sort((left, right) => (left.mismatched_bytes ?? 999) - (right.mismatched_bytes ?? 999));

  let matched = 0;
  for (const row of near) {
    const stem = row.entry.toString(16).padStart(8, "0");
    if (tracked.has(stem)) continue;
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
        try {
          [okay] = verify(candidate, rom, output);
        } catch {
          continue;
        }
        if (!okay) continue;
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
