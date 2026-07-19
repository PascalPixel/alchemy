#!/usr/bin/env bun
// 置換探索v1。m2c下書きを文モデルで変形し、焼きなましと重み付き
// 編集距離で承認コンパイラのバイト差分を縮める。対象ごとの成績を
// 保存して次波の予算を配分し、成功手順は他関数へ再演する。
// 一致した関数は完全リンク検証と領域長検査の上でsrc/へ設置する。
import { appendFileSync, existsSync, mkdirSync, readFileSync, readdirSync, rmSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { M2C_PREAMBLE } from "./match_m2c.ts";
import { candidates as seedCandidates, replaceableAssembly } from "./permute_m2c.ts";
import { verify } from "./verify.ts";
import { CFLAGS, compilerCommand, externalSymbol, externalSymbolAssembly } from "./alchemy_gcc.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const STATE_DIR = join(ROOT, "out/permute1/state");
const RECIPES = join(ROOT, "out/permute1/recipes.json");

interface Options {
  rom: string;
  drafts: string;
  report: string;
  steps: number;
  restarts: number;
  seed: number;
  maxMismatch: number;
  limit?: number;
  shard?: [number, number];
  targetsFile?: string;
}

function parseArguments(argv: string[]): Options {
  const options: Options = {
    rom: join(ROOT, "gs1-en.gba"),
    drafts: join(ROOT, "work/m2c-ctx"),
    report: join(ROOT, "work/matches/m2c.json"),
    steps: 3000,
    restarts: 6,
    seed: 0,
    maxMismatch: 100000,
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--drafts") options.drafts = argv[++index];
    else if (argument === "--report") options.report = argv[++index];
    else if (argument === "--steps") options.steps = Number(argv[++index]);
    else if (argument === "--restarts") options.restarts = Number(argv[++index]);
    else if (argument === "--seed") options.seed = Number(argv[++index]);
    else if (argument === "--max-mismatch") options.maxMismatch = Number(argv[++index]);
    else if (argument === "--limit") options.limit = Number(argv[++index]);
    else if (argument === "--targets") options.targetsFile = argv[++index];
    else if (argument === "--shard") {
      const [part, total] = argv[++index].split("/").map(Number);
      options.shard = [part, total];
    } else if (argument === "-h" || argument === "--help") {
      console.log("usage: permute_v1.ts [--drafts DIR] [--report FILE] [--steps N] [--restarts N] [--seed N] [--shard K/N] [--limit N] [--targets FILE]");
      process.exit(0);
    } else options.rom = argument;
  }
  return options;
}

function makeRandom(seed: number): () => number {
  let state = seed >>> 0 || 1;
  return () => {
    state ^= state << 13; state >>>= 0;
    state ^= state >> 17;
    state ^= state << 5; state >>>= 0;
    return state / 0x100000000;
  };
}

// ---- 採点 -------------------------------------------------------------

async function run(command: string[]): Promise<{ code: number; stdout: string }> {
  const child = Bun.spawn(command, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  const [stdout, code] = await Promise.all([new Response(child.stdout).text(), child.exited]);
  return { code, stdout };
}

class Scorer {
  private symbolsObject: string | null = null;
  private symbolsKey = "";
  static cache = new Map<string, number>();
  static cacheFile = "";
  static appended = 0;
  constructor(
    readonly stem: string,
    readonly expected: Buffer,
    readonly scratch: string,
  ) {}

  // 完全リンクでバイト列を得る。外部シンボル束は未定義集合が変わった
  // ときだけ作り直す。失敗は巨大コストとして返す。
  async bytes(source: string): Promise<Buffer | null> {
    const prefix = join(this.scratch, this.stem);
    const cFile = `${prefix}.c`, sFile = `${prefix}.s`, oFile = `${prefix}.o`;
    const elf = `${prefix}.elf`, bin = `${prefix}.bin`;
    writeFileSync(cFile, source);
    if ((await run(compilerCommand(...CFLAGS, "-S", "-o", sFile, cFile))).code !== 0) return null;
    if ((await run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", oFile, sFile])).code !== 0) return null;
    const address = `0x${this.stem}`;
    const link = () => run([
      "arm-none-eabi-ld", `-Ttext=${address}`, "-e", `Func_${this.stem}`,
      "-o", elf, oFile, ...(this.symbolsObject ? [this.symbolsObject] : []),
    ]);
    let linked = await link();
    if (linked.code !== 0) {
      // 未定義シンボルの束を再生成して一度だけ再試行する。
      const undefinedNames: string[] = [];
      for (const line of (await run(["arm-none-eabi-nm", "-u", oFile])).stdout.split(/\r?\n/).filter(Boolean)) {
        const name = line.trim().split(/\s+/).at(-1)!;
        if (externalSymbol(name) === null) return null;
        undefinedNames.push(name);
      }
      const key = undefinedNames.sort().join(",");
      const symbolsSource = `${prefix}.symbols.s`;
      writeFileSync(symbolsSource, ".syntax unified\n.thumb\n" + undefinedNames.map(externalSymbolAssembly).join(""));
      const symbolsObject = `${prefix}.symbols.o`;
      if ((await run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", symbolsObject, symbolsSource])).code !== 0) return null;
      this.symbolsObject = symbolsObject;
      this.symbolsKey = key;
      linked = await link();
      if (linked.code !== 0) return null;
    }
    if ((await run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, bin])).code !== 0) return null;
    return readFileSync(bin).subarray(0, Math.max(this.expected.length, 4));
  }

  // 重み付き半語編集距離。同系オペコードのレジスタ差は軽く、
  // 系統違いは重く、長さずれは最も重く数える。
  async score(source: string): Promise<number> {
    const key = `${this.stem}:${Bun.hash(source).toString(36)}`;
    const cached = Scorer.cache.get(key);
    if (cached !== undefined) return cached;
    const actual = await this.bytes(source);
    const value = actual === null ? Number.MAX_SAFE_INTEGER : weightedDiff(actual, this.expected);
    Scorer.cache.set(key, value);
    if (Scorer.cacheFile !== "" && value !== Number.MAX_SAFE_INTEGER) {
      appendFileSync(Scorer.cacheFile, `${key} ${value}\n`);
      Scorer.appended++;
    }
    return value;
  }
}

export function weightedDiff(actual: Buffer, expected: Buffer): number {
  const a: number[] = [], e: number[] = [];
  for (let index = 0; index + 1 < actual.length; index += 2) a.push(actual.readUInt16LE(index));
  for (let index = 0; index + 1 < expected.length; index += 2) e.push(expected.readUInt16LE(index));
  if (a.length === e.length) {
    let cost = 0;
    for (let index = 0; index < a.length; index++) {
      if (a[index] === e[index]) continue;
      cost += (a[index] & 0xf800) === (e[index] & 0xf800) ? 1 : 3;
    }
    return cost;
  }
  // 長さが違うときは帯付き編集距離で滑らかに罰する。
  const band = 24, INDEL = 6;
  const rows = a.length + 1, cols = e.length + 1;
  let previous = new Array<number>(cols).fill(0).map((_, index) => index * INDEL);
  for (let row = 1; row < rows; row++) {
    const current = new Array<number>(cols).fill(Number.MAX_SAFE_INTEGER / 2);
    const low = Math.max(1, row - band), high = Math.min(cols - 1, row + band);
    if (row - band <= 0) current[0] = row * INDEL;
    for (let col = low; col <= high; col++) {
      const substitution = a[row - 1] === e[col - 1] ? 0 : (a[row - 1] & 0xf800) === (e[col - 1] & 0xf800) ? 1 : 3;
      current[col] = Math.min(
        previous[col - 1] + substitution,
        previous[col] + INDEL,
        current[col - 1] + INDEL,
      );
    }
    previous = current;
  }
  return previous[cols - 1];
}

// ---- 文モデルと変形 ---------------------------------------------------

interface Statement { indent: string; text: string; depth: number; line: number }

export function parseStatements(body: string): { rows: string[]; statements: Statement[] } {
  const rows = body.split("\n");
  const statements: Statement[] = [];
  let depth = 0;
  for (let index = 0; index < rows.length; index++) {
    const row = rows[index];
    const trimmed = row.trim();
    const opens = (trimmed.match(/\{/g) ?? []).length;
    const closes = (trimmed.match(/\}/g) ?? []).length;
    if (trimmed.endsWith(";") && !trimmed.startsWith("do") && opens === 0 && closes === 0 &&
        !/^(if|while|for|else|return|case|default)\b/.test(trimmed) && trimmed.includes("=") || /^\w[\w>.\[\]*]*\(.*\);$/.test(trimmed)) {
      statements.push({ indent: row.slice(0, row.length - trimmed.length), text: trimmed, depth, line: index });
    }
    depth += opens - closes;
  }
  return { rows, statements };
}

function identifiers(text: string): Set<string> {
  const result = new Set<string>();
  for (const name of text.match(/[A-Za-z_]\w*/g) ?? []) {
    if (!/^(s8|u8|s16|u16|s32|u32|void|M2C_FIELD|if|else|while|do|return|volatile|extern)$/.test(name)) result.add(name);
  }
  return result;
}

function writes(text: string): Set<string> {
  const result = new Set<string>();
  const match = /^\**(\w+)\s*(=|\+=|-=|\|=|&=|\^=|<<=|>>=)[^=]/.exec(text);
  if (match) result.add(match[1]);
  return result;
}

function conflicts(a: Statement, b: Statement): boolean {
  if (a.text.includes("(") && b.text.includes("(") &&
      /\bFunc_/.test(a.text) && /\bFunc_/.test(b.text)) return true;
  if (/M2C_FIELD|\*\(/.test(a.text) && /M2C_FIELD|\*\(/.test(b.text) &&
      (/=\s*[^=]/.test(a.text) || /=\s*[^=]/.test(b.text))) {
    // 双方がメモリ操作を含み、どちらかが書込みなら順序を保つ。
    const aStores = /^(M2C_FIELD|\*)/.test(a.text), bStores = /^(M2C_FIELD|\*)/.test(b.text);
    if (aStores || bStores) return true;
  }
  const aIdentifiers = identifiers(a.text), bIdentifiers = identifiers(b.text);
  const aWrites = writes(a.text), bWrites = writes(b.text);
  for (const name of aWrites) if (bIdentifiers.has(name)) return true;
  for (const name of bWrites) if (aIdentifiers.has(name)) return true;
  return false;
}

type Operator = (body: string, random: () => number) => string | null;

function swapStatements(body: string, random: () => number): string | null {
  const { rows, statements } = parseStatements(body);
  const pairs: Array<[Statement, Statement]> = [];
  for (let first = 0; first < statements.length; first++) {
    for (let second = first + 1; second < Math.min(statements.length, first + 4); second++) {
      const a = statements[first], b = statements[second];
      if (a.depth !== b.depth || b.line - a.line > 6) continue;
      let blocked = false;
      for (let middle = first; middle <= second && !blocked; middle++) {
        for (let other = middle + 1; other <= second; other++) {
          if (conflicts(statements[middle], statements[other]) && !(middle === first && other === second)) blocked = true;
        }
      }
      if (!blocked && !conflicts(a, b)) pairs.push([a, b]);
    }
  }
  if (pairs.length === 0) return null;
  const [a, b] = pairs[Math.floor(random() * pairs.length)];
  const copy = [...rows];
  [copy[a.line], copy[b.line]] = [copy[b.line].replace(b.indent, a.indent), copy[a.line].replace(a.indent, b.indent)];
  return copy.join("\n");
}

let temporaryCounter = 0;
function hoistSubexpression(body: string, random: () => number): string | null {
  const pattern = /M2C_FIELD\([^;{}]*?\)|\*\(\w+ \*\)\w+|\([^()]{6,40}\)/g;
  const { statements } = parseStatements(body);
  const usable = statements.filter((statement) => pattern.test(statement.text) && !statement.text.includes("_v1t"));
  if (usable.length === 0) return null;
  const statement = usable[Math.floor(random() * usable.length)];
  const found = statement.text.match(pattern);
  if (!found || found.length === 0) return null;
  const expression = found[Math.floor(random() * found.length)];
  if (/=\s*$/.test(statement.text.split(expression)[0] ?? "")) { /* 左辺は動かさない */ }
  const name = `_v1t${temporaryCounter++ % 97}`;
  const declared = body.replace(/(\)\s*\{\n)/, `$1    s32 ${name};\n`);
  const rewritten = declared.replace(statement.text, `${name} = (s32) (${expression}); ${statement.text.replace(expression, `${name}`)}`);
  return rewritten === declared ? null : rewritten;
}

function inlineTemporary(body: string, random: () => number): string | null {
  const pattern = /^(\s+)(\w+) = ([^;]{3,60});\n/gm;
  const matches = [...body.matchAll(pattern)];
  const single = matches.filter((match) => {
    const uses = body.split(match[2]).length - 1;
    return uses === 3 && !match[3].includes("Func_");
  });
  if (single.length === 0) return null;
  const target = single[Math.floor(random() * single.length)];
  const removed = body.replace(target[0], "");
  const replaced = removed.replace(new RegExp(`\\b${target[2]}\\b`), `(${target[3]})`);
  return replaced === removed ? null : replaced;
}

function reassociate(body: string, random: () => number): string | null {
  const pattern = /\(([\w>.\[\]* ]+) (\+|\||&|\^) ([\w>.\[\]* ]+)\) \2 ([\w>.\[\]* ]+)/g;
  const matches = [...body.matchAll(pattern)];
  if (matches.length === 0) return null;
  const target = matches[Math.floor(random() * matches.length)];
  const rewritten = `${target[1]} ${target[2]} (${target[3]} ${target[2]} ${target[4]})`;
  return body.slice(0, target.index) + rewritten + body.slice(target.index! + target[0].length);
}

function commutativeFlip(body: string, random: () => number): string | null {
  const pattern = /\(([\w>\[\]().* ]{1,26}) (\+|\||&|\^) ([\w>\[\]().* ]{1,26})\)/g;
  const matches = [...body.matchAll(pattern)];
  if (matches.length === 0) return null;
  const target = matches[Math.floor(random() * matches.length)];
  return body.slice(0, target.index) + `(${target[3]} ${target[2]} ${target[1]})` + body.slice(target.index! + target[0].length);
}

const TYPE_FLIPS: Array<[RegExp, string]> = [
  [/\bs32 (\w+);/, "u32 $1;"],
  [/\bu32 (\w+);/, "s32 $1;"],
  [/\bu8 (\w+);/, "s32 $1;"],
  [/\bs16 (\w+);/, "s32 $1;"],
  [/\bu16 (\w+);/, "s32 $1;"],
];
function typeFlip(body: string, random: () => number): string | null {
  const [pattern, replacement] = TYPE_FLIPS[Math.floor(random() * TYPE_FLIPS.length)];
  const matches = [...body.matchAll(new RegExp(pattern.source, "g"))];
  if (matches.length === 0) return null;
  const target = matches[Math.floor(random() * matches.length)];
  return body.slice(0, target.index) + target[0].replace(pattern, replacement) + body.slice(target.index! + target[0].length);
}

function declarationShuffle(body: string, random: () => number): string | null {
  const pattern = /^(\s+(?:s8|u8|s16|u16|s32|u32|void \*+|\w+ \*+)\w+;\n)+/m;
  const match = pattern.exec(body);
  if (match === null) return null;
  const rows = match[0].split("\n").filter(Boolean);
  if (rows.length < 2) return null;
  const shuffled = [...rows];
  const from = Math.floor(random() * shuffled.length);
  const to = Math.floor(random() * shuffled.length);
  const [row] = shuffled.splice(from, 1);
  shuffled.splice(to, 0, row);
  return body.replace(match[0], shuffled.join("\n") + "\n");
}

function dummyLocal(body: string, random: () => number): string | null {
  // 死ぬ代入で擬似レジスタの生成順をずらす。-O2で命令は残らない。
  const count = (body.match(/_v1d\d+/g) ?? []).length;
  if (count >= 3) return null;
  const name = `_v1d${Math.floor(random() * 89)}`;
  if (body.includes(name)) return null;
  const { statements } = parseStatements(body);
  if (statements.length === 0) return null;
  const at = statements[Math.floor(random() * statements.length)];
  const declared = body.replace(/(\)\s*\{\n)/, `$1    s32 ${name};\n`);
  return declared.replace(at.text, `${name} = 0; ${at.text}`);
}

function castNudge(body: string, random: () => number): string | null {
  const pattern = /\((s32|u32)\) /g;
  const matches = [...body.matchAll(pattern)];
  if (matches.length === 0 || random() < 0.5) {
    // 追加: 変数参照の前へ (s32) を挿す
    const identifiersFound = [...body.matchAll(/ (\w{2,12}) ([-+|&^<>=]{1,2}) /g)];
    if (identifiersFound.length === 0) return null;
    const target = identifiersFound[Math.floor(random() * identifiersFound.length)];
    return body.slice(0, target.index) + ` (s32) ${target[1]} ${target[2]} ` + body.slice(target.index! + target[0].length);
  }
  const target = matches[Math.floor(random() * matches.length)];
  return body.slice(0, target.index) + body.slice(target.index! + target[0].length);
}

function conditionInvert(body: string, random: () => number): string | null {
  const pattern = /if \(([^)]+)\) \{\n([\s\S]+?\n)(\s+)\} else \{\n([\s\S]+?\n)(\s+)\}/;
  const match = pattern.exec(body);
  if (match === null) return null;
  return body.replace(pattern, `if (!(${match[1]})) {\n${match[4]}${match[3]}} else {\n${match[2]}${match[5]}}`);
}

function loopRotate(body: string, random: () => number): string | null {
  const whilePattern = /(\s+)while \((.+)\) \{\n([\s\S]+?)\n(\s+)\}/;
  const match = whilePattern.exec(body);
  if (match === null) return null;
  return body.replace(whilePattern, `$1if ($2) {\n$1    do {\n$3\n$1    } while ($2);\n$4}`);
}

function compoundAssign(body: string, random: () => number): string | null {
  if (random() < 0.5) {
    const pattern = /(\w+) \+= ([^;]+);/;
    const match = pattern.exec(body);
    if (match === null) return null;
    return body.replace(pattern, `${match[1]} = ${match[1]} + ${match[2]};`);
  }
  const pattern = /(\w+) = \1 \+ ([^;]+);/;
  const match = pattern.exec(body);
  if (match === null) return null;
  return body.replace(pattern, `${match[1]} += ${match[2]};`);
}

// 慣用句辞書からの借用: 同じ錨を持つ証明済み雛形へ文の形を差し替える。
let IDIOMS: Record<string, string[]> | null = null;
function loadIdioms(): Record<string, string[]> {
  if (IDIOMS === null) {
    const path = join(ROOT, "out/permute1/idioms.json");
    IDIOMS = existsSync(path) ? (JSON.parse(readFileSync(path, "utf8")).dictionary as Record<string, string[]>) : {};
  }
  return IDIOMS;
}

function borrowIdiom(body: string, random: () => number): string | null {
  const dictionary = loadIdioms();
  const { statements } = parseStatements(body);
  const anchored = statements.map((statement) => {
    const anchors: string[] = [];
    for (const match of statement.text.matchAll(/Func_08[0-9a-f]{6}/g)) anchors.push(`call:${match[0]}`);
    for (const match of statement.text.matchAll(/M2C_FIELD\([^,]+, (\w+) \*, (0x[0-9A-Fa-f]+|\d+)\)/g)) {
      anchors.push(`field:${match[1]}:${Number(match[2]).toString(16)}`);
    }
    return { statement, anchors };
  }).filter((entry) => entry.anchors.length > 0);
  if (anchored.length === 0) return null;
  const pick = anchored[Math.floor(random() * anchored.length)];
  const anchor = pick.anchors[Math.floor(random() * pick.anchors.length)];
  const templates = dictionary[anchor];
  if (!templates || templates.length === 0) return null;
  const template = templates[Math.floor(random() * templates.length)];
  // 現在文の識別子を出現順に雛形の%vNへ流し込む。
  const names = [...new Set(pick.statement.text.match(/\b[a-z_]\w*\b/g) ?? [])]
    .filter((name) => !/^(s8|u8|s16|u16|s32|u32|void|do|if|else|while|return|volatile)$/.test(name));
  let instantiated = template;
  for (let index = 0; index < 8; index++) {
    instantiated = instantiated.replaceAll(`%v${index}`, names[index] ?? `_bv${index}`);
  }
  if (instantiated.includes("%v") || instantiated === pick.statement.text) return null;
  const declared = /_bv\d/.test(instantiated)
    ? body.replace(/(\)\s*\{\n)/, `$1    s32 ${[...new Set(instantiated.match(/_bv\d/g) ?? [])].join(", ")};\n`)
    : body;
  return declared.replace(pick.statement.text, instantiated);
}

const OPERATORS: Array<[string, Operator]> = [
  ["borrow", borrowIdiom],
  ["swap", swapStatements],
  ["hoist", hoistSubexpression],
  ["inline", inlineTemporary],
  ["reassoc", reassociate],
  ["commute", commutativeFlip],
  ["typeflip", typeFlip],
  ["declshuffle", declarationShuffle],
  ["dummy", dummyLocal],
  ["cast", castNudge],
  ["condinvert", conditionInvert],
  ["looprotate", loopRotate],
  ["compound", compoundAssign],
];

// ---- 状態と予算 -------------------------------------------------------

interface TargetState {
  best: { body: string; score: number } | null;
  population: Array<{ body: string; score: number }>;
  operators: Record<string, { tried: number; accepted: number }>;
  rounds: Array<{ seed: number; before: number; after: number }>;
}

function loadState(stem: string): TargetState {
  const path = join(STATE_DIR, `${stem}.json`);
  if (existsSync(path)) return JSON.parse(readFileSync(path, "utf8")) as TargetState;
  return { best: null, population: [], operators: {}, rounds: [] };
}

function saveState(stem: string, state: TargetState): void {
  mkdirSync(STATE_DIR, { recursive: true });
  writeFileSync(join(STATE_DIR, `${stem}.json`), JSON.stringify(state));
}

function budget(state: TargetState, base: number): number {
  const recent = state.rounds.slice(-2);
  if (recent.length === 2 && recent.every((round) => round.after >= round.before)) {
    return Math.max(200, Math.floor(base / 8)); // 高原には小突きだけ入れる
  }
  return base;
}

function pickOperator(state: TargetState, random: () => number): [string, Operator] {
  const weights = OPERATORS.map(([name]) => {
    const record = state.operators[name] ?? { tried: 0, accepted: 0 };
    return (record.accepted + 1) / (record.tried + 8);
  });
  const total = weights.reduce((sum, value) => sum + value, 0);
  let roll = random() * total;
  for (let index = 0; index < OPERATORS.length; index++) {
    roll -= weights[index];
    if (roll <= 0) return OPERATORS[index];
  }
  return OPERATORS[OPERATORS.length - 1];
}

interface Recipe { operators: string[] }

function loadRecipes(): Recipe[] {
  if (!existsSync(RECIPES)) return [];
  return JSON.parse(readFileSync(RECIPES, "utf8")) as Recipe[];
}

function appendRecipe(operatorNames: string[]): void {
  const recipes = loadRecipes();
  recipes.push({ operators: operatorNames.slice(-12) });
  mkdirSync(dirname(RECIPES), { recursive: true });
  writeFileSync(RECIPES, JSON.stringify(recipes.slice(-200)));
}

// ---- 主探索 -----------------------------------------------------------

function install(stem: string, source: string, rom: Buffer, scratch: string): boolean {
  const candidate = join(scratch, `${stem}.c`);
  writeFileSync(candidate, source);
  let okay = false, size = 0;
  try {
    [okay, size] = verify(candidate, rom, scratch);
  } catch {
    return false;
  }
  if (!okay) return false;
  if (!replaceableAssembly(stem, size, scratch)) {
    console.log(`skip ${stem}: asm region longer than matched C`);
    return false;
  }
  writeFileSync(join(ROOT, "src", `${stem}.c`), source);
  rmSync(join(ROOT, "asm", `${stem}.s`), { force: true });
  return true;
}

async function main(): Promise<void> {
  const options = parseArguments(Bun.argv.slice(2));
  const rom = readFileSync(options.rom);
  const output = join(ROOT, "out/permute1/work");
  mkdirSync(output, { recursive: true });
  const tracked = new Set(readdirSync(join(ROOT, "src")).filter((name) => name.endsWith(".c")).map((name) => basename(name, ".c")));
  let stems: string[];
  if (options.targetsFile !== undefined) {
    stems = readFileSync(options.targetsFile, "utf8").split(/\s+/).filter(Boolean);
  } else {
    const rows = JSON.parse(readFileSync(options.report, "utf8")) as Array<{ entry: number; matched?: boolean; mismatched_bytes?: number }>;
    stems = rows
      .filter((row) => !row.matched && (row.mismatched_bytes ?? 100001) <= options.maxMismatch)
      .sort((left, right) => (left.mismatched_bytes ?? 0) - (right.mismatched_bytes ?? 0))
      .map((row) => row.entry.toString(16).padStart(8, "0"));
  }
  stems = stems.filter((stem) => !tracked.has(stem) &&
    existsSync(join(ROOT, "asm", `${stem}.s`)) && existsSync(join(options.drafts, `${stem}.c`)));
  if (options.shard) stems = stems.filter((_, index) => index % options.shard![1] === options.shard![0]);
  if (options.limit !== undefined) stems = stems.slice(0, options.limit);
  console.log(`targets=${stems.length} steps=${options.steps} restarts=${options.restarts} seed=${options.seed}`);

  // 過去の全採点をRAMへ読み戻し、同一候補の再コンパイルを省く。
  const cacheDir = join(ROOT, "out/permute1/cache");
  mkdirSync(cacheDir, { recursive: true });
  for (const name of readdirSync(cacheDir).filter((entry) => entry.endsWith(".log"))) {
    for (const line of readFileSync(join(cacheDir, name), "utf8").split("\n")) {
      const space = line.indexOf(" ");
      if (space > 0) Scorer.cache.set(line.slice(0, space), Number(line.slice(space + 1)));
    }
  }
  Scorer.cacheFile = join(cacheDir, `shard${options.shard ? options.shard[0] : 0}.log`);
  console.log(`cache=${Scorer.cache.size}`);
  const recipes = loadRecipes();
  let matchedCount = 0;
  let cursor = 0;
  const concurrency = Math.max(4, Math.min(20, (navigator.hardwareConcurrency || 8) - 2));
  async function processTarget(stem: string): Promise<void> {
    const scratch = join(output, stem);
    mkdirSync(scratch, { recursive: true });
    // 期待バイトはROMの当該領域そのもの。完全リンク採点なので直接使える。
    const address = Number.parseInt(stem, 16) - 0x08000000;
    const state = loadState(stem);
    const raw = readFileSync(join(options.drafts, `${stem}.c`), "utf8");
    const bases: string[] = [];
    if (state.best !== null) bases.push(state.best.body);
    for (const entry of state.population) bases.push(entry.body);
    const unknowns = raw.includes("M2C_UNK") ? ["void", "s32", "u32", "u8"] : [""];
    for (const unknown of unknowns) {
      const seeded = unknown === "" ? raw : raw.replaceAll("M2C_UNK", unknown);
      bases.push(seeded, ...seedCandidates(seeded));
    }
    // 参照長は最初に確定させる: 素の下書きを一度リンクして期待長を得る。
    let expected: Buffer | null = null;
    let scorer: Scorer | null = null;
    let best: { body: string; score: number } | null = null;
    for (const base of new Set(bases)) {
      const probeSource = base.startsWith("typedef") ? base : M2C_PREAMBLE + base;
      if (expected === null) {
        const probePath = join(scratch, `${stem}.c`);
        writeFileSync(probePath, probeSource);
        try {
          const [actual, expectedBytes] = verify(probePath, rom, scratch, true);
          expected = Buffer.from(rom.subarray(address, address + expectedBytes.length));
          scorer = new Scorer(stem, expected, scratch);
        } catch { continue; }
      }
      const value = await scorer!.score(probeSource);
      const body = probeSource.replace(M2C_PREAMBLE, "");
      if (best === null || value < best.score) best = { body, score: value };
    }
    if (best === null || scorer === null || best.score > 400) { console.log(`unusable ${stem}`); return; }

    // 成功手順の再演を最初に試す。
    let done = false;
    for (const recipe of recipes) {
      let body = best.body;
      const random = makeRandom(Number.parseInt(stem, 16) ^ 0x5f3759df);
      for (const operatorName of recipe.operators) {
        const operator = OPERATORS.find(([name]) => name === operatorName)?.[1];
        if (!operator) continue;
        const next = operator(body, random);
        if (next !== null) body = next;
      }
      const value = await scorer.score(M2C_PREAMBLE + body);
      if (value === 0 && install(stem, M2C_PREAMBLE + body, rom, scratch)) {
        console.log(`matched ${stem} (recipe)`);
        matchedCount++; done = true; break;
      }
      if (value < best.score) best = { body, score: value };
    }

    const stepsBudget = budget(state, options.steps);
    const acceptedNames: string[] = [];
    for (let restart = 0; restart < options.restarts && !done; restart++) {
      const random = makeRandom(Number.parseInt(stem, 16) ^ ((restart + options.seed * 131) * 0x9e3779b9));
      const start = state.population.length > 0 && restart % 2 === 1
        ? state.population[Math.floor(random() * state.population.length)]
        : best;
      let current = { ...start };
      let temperature = Math.max(2, current.score / 4);
      let sinceImprovement = 0;
      for (let step = 0; step < stepsBudget; step++) {
        if (sinceImprovement >= 400) break;
        const [operatorName, operator] = pickOperator(state, random);
        const next = operator(current.body, random);
        const record = (state.operators[operatorName] ??= { tried: 0, accepted: 0 });
        record.tried++;
        if (next === null || next === current.body) continue;
        const value = await scorer.score(M2C_PREAMBLE + next);
        sinceImprovement++;
        const delta = value - current.score;
        if (delta <= 0 || random() < Math.exp(-delta / temperature)) {
          if (delta < 0) { record.accepted++; acceptedNames.push(operatorName); sinceImprovement = 0; }
          current = { body: next, score: value };
          if (value < best.score) best = { ...current };
        }
        temperature = Math.max(0.5, temperature * 0.995);
        if (value === 0) {
          if (install(stem, M2C_PREAMBLE + next, rom, scratch)) {
            console.log(`matched ${stem} (start=${state.best?.score ?? "fresh"})`);
            appendRecipe(acceptedNames);
            matchedCount++;
          }
          done = true;
          break;
        }
      }
      // 個体群には差分位置が異なる上位を残す。
      if (!done) {
        state.population = [...state.population, { ...current }]
          .sort((left, right) => left.score - right.score)
          .filter((entry, index, list) => index === 0 || entry.body !== list[index - 1].body)
          .slice(0, 3);
      }
    }
    if (!done) {
      const before = state.best?.score ?? Number.MAX_SAFE_INTEGER;
      state.rounds.push({ seed: options.seed, before, after: best.score });
      state.rounds = state.rounds.slice(-8);
      state.best = best;
      saveState(stem, state);
      console.log(`floor ${stem}: ${best.score}`);
    } else {
      rmSync(join(STATE_DIR, `${stem}.json`), { force: true });
    }
  }
  async function pump(): Promise<void> {
    while (true) {
      const index = cursor++;
      if (index >= stems.length) return;
      await processTarget(stems[index]);
    }
  }
  await Promise.all(Array.from({ length: concurrency }, pump));
  console.log(`matched=${matchedCount} of ${stems.length}`);
}

if (import.meta.main) await main();
