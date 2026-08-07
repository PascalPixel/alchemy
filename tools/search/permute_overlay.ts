#!/usr/bin/env bun
// 重ね合わせ像向け置換探索。permute_v1の作用素表と焼きなましをそのまま借り、
// 期待バイトだけを重ね合わせ参照像から取る。候補は承認コンパイラで組み立て、
// 未定義参照は伴走オブジェクトへ絶対番地として束ね、関数番地でリンクする。
// 完全一致した下書きはout/permute-overlay/hitsへ保存し、採用は別工程に任せる。
import { appendFileSync, existsSync, mkdirSync, readFileSync, readdirSync, rmSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import {
  OPERATORS,
  anneal,
  conflicts,
  makeRandom,
  parseStatements,
  weightedDiff,
  type Individual,
  type Operator,
  type TargetState,
} from "./permute_v1.ts";
import { OVERLAY_BASE, assembleOverlay } from "../lib/overlay_disasm.ts";
import {
  compilerBundleSignature,
  externalSymbol,
  externalSymbolAssembly,
  sourceToAssemblyPlan,
} from "../lib/alchemy_gcc.ts";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
const OUT = join(ROOT, "out/permute-overlay");
const STATE_DIR = join(OUT, "state");
const HITS_DIR = join(OUT, "hits");
const CACHE_DIR = join(OUT, "cache");
const CODE = join(ROOT, "assets/code");
const EXACT = join(ROOT, "exact");

function contentSignature(paths: readonly string[]): string {
  const digest = new Bun.CryptoHasher("sha256");
  digest.update(compilerBundleSignature());
  for (const path of paths) {
    digest.update(path);
    digest.update("\0");
    digest.update(readFileSync(path));
    digest.update("\0");
  }
  return digest.digest("hex");
}

// 採点も個体群も、この署名が指すコンパイラ・作用素・採点実装でしか意味を持たない。
const SIGNATURE = contentSignature([
  join(ROOT, "tools/lib/alchemy_gcc.ts"),
  join(ROOT, "tools/search/permute_v1.ts"),
  join(ROOT, "tools/search/permute_overlay.ts"),
  join(ROOT, "tools/lib/decomp_constraints.ts"),
]);

interface Options {
  targetsFile?: string;
  ids: string[];
  steps: number;
  restarts: number;
  seed: number;
  jobs: number;
  limit?: number;
  probe: boolean;
  reduce?: string;
}

function parseArguments(argv: string[]): Options {
  const options: Options = {
    ids: [],
    steps: 3000,
    restarts: 6,
    seed: 0,
    jobs: 2,
    probe: false,
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--targets") options.targetsFile = argv[++index];
    else if (argument === "--steps") options.steps = Number(argv[++index]);
    else if (argument === "--restarts") options.restarts = Number(argv[++index]);
    else if (argument === "--seed") options.seed = Number(argv[++index]);
    else if (argument === "--jobs") options.jobs = Number(argv[++index]);
    else if (argument === "--limit") options.limit = Number(argv[++index]);
    else if (argument === "--probe") options.probe = true;
    else if (argument === "--reduce") options.reduce = argv[++index];
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: permute_overlay.ts [--targets FILE] [overlay:offset ...] [--steps N] [--restarts N] [--jobs N] [--seed N] [--limit N] [--probe] [--self-test]");
      process.exit(0);
    } else options.ids.push(argument);
  }
  return options;
}

// ---- 対象の解決 -------------------------------------------------------

interface Target {
  id: string;
  overlay: string;
  offset: number;
  stem: string;
  span: number;
  draft: string;
  // 旗はソース経路で振り分けられるので、採点は採用後の資産経路で行う。
  routing: string;
}

export function parseTargetSpecification(line: string): {
  overlay: string;
  offset: number;
  stem: string;
  span?: number;
  draft?: string;
  routing?: string;
} | null {
  const text = line.replace(/#.*$/, "").trim();
  if (text === "") return null;
  const fields = text.split(/[\s,]+/).filter(Boolean);
  const identifier = fields[0];
  const match = /^(resource_[0-9a-f]{3}):([0-9a-fA-F]{1,6})$/.exec(identifier);
  if (match === null) throw new Error(`unparseable overlay target: ${identifier}`);
  const offset = Number.parseInt(match[2], 16);
  const result: { overlay: string; offset: number; stem: string; span?: number; draft?: string; routing?: string } = {
    overlay: match[1],
    offset,
    stem: (OVERLAY_BASE + offset).toString(16).padStart(8, "0"),
  };
  for (const field of fields.slice(1)) {
    const [key, value] = field.split("=");
    if (key === "span") result.span = value.startsWith("0x") ? Number.parseInt(value.slice(2), 16) : Number(value);
    else if (key === "source") result.draft = value;
    else if (key === "routing") result.routing = value;
    else throw new Error(`unknown target field: ${field}`);
  }
  return result;
}

let inventory: Map<string, number> | null = null;
function inventorySpan(id: string): number | undefined {
  if (inventory === null) {
    inventory = new Map();
    const path = join(ROOT, "out/decomp/overlays.json");
    if (existsSync(path)) {
      const document = JSON.parse(readFileSync(path, "utf8")) as { functions?: Array<{ id: string; span_bytes: number }> };
      for (const row of document.functions ?? []) inventory.set(row.id, row.span_bytes);
    }
  }
  return inventory.get(id);
}

function resolveDraft(overlay: string, offsetText: string): string | undefined {
  const candidates = [
    join(ROOT, "work/notes", `${overlay}-${offsetText}-best.c`),
    join(ROOT, "work/drafts", `${overlay}-${offsetText}.c`),
    join(EXACT, `${overlay}_c_${(OVERLAY_BASE + Number.parseInt(offsetText, 16)).toString(16).padStart(8, "0")}.c`),
  ];
  return candidates.find(existsSync);
}

function resolveTargets(options: Options): Target[] {
  // 位置引数は語ごとに届くので、識別子で区切って一行にまとめ直す。
  const lines: string[] = [];
  for (const word of options.ids) {
    if (word.includes("=") && lines.length > 0) lines[lines.length - 1] += ` ${word}`;
    else lines.push(word);
  }
  if (options.targetsFile !== undefined) lines.push(...readFileSync(options.targetsFile, "utf8").split("\n"));
  const targets: Target[] = [];
  for (const line of lines) {
    const specification = parseTargetSpecification(line);
    if (specification === null) continue;
    const offsetText = specification.offset.toString(16).padStart(4, "0");
    const id = `${specification.overlay}:${offsetText}`;
    const span = specification.span ?? inventorySpan(id);
    if (span === undefined) {
      console.log(`skip ${id}: no span (pass span=BYTES)`);
      continue;
    }
    const draft = specification.draft ?? resolveDraft(specification.overlay, offsetText);
    if (draft === undefined || !existsSync(draft)) {
      console.log(`skip ${id}: no draft`);
      continue;
    }
    // 採用済み関数の参照像には自分の出力が貼り込まれているので、比較は自明に
    // 一致する。探索対象にすると偽の当たりになるため外す。
    const adopted = join(EXACT, `${specification.overlay}_c_${specification.stem}.c`);
    if (existsSync(adopted)) {
      console.log(`skip ${id}: already adopted (${basename(adopted)})`);
      continue;
    }
    targets.push({
      id,
      overlay: specification.overlay,
      offset: specification.offset,
      stem: specification.stem,
      span,
      draft,
      routing: specification.routing ?? `exact/${specification.overlay}_c_${specification.stem}.c`,
    });
  }
  return targets;
}

// ---- 参照像 -----------------------------------------------------------

// 参照像は重ね合わせ毎に一度だけ組み立てる。すでに採用済みのC区間を
// 貼り戻す作業を含むため、内側の輪で作り直すと採点費用が進捗に比例して膨らむ。
const references = new Map<string, Buffer>();
const referenceBuilds = new Map<string, Promise<Buffer>>();

async function reference(overlay: string): Promise<Buffer> {
  const held = references.get(overlay);
  if (held !== undefined) return held;
  let pending = referenceBuilds.get(overlay);
  if (pending === undefined) {
    pending = (async () => {
      const image = assembleOverlay(join(CODE, `${overlay}_overlay.s`), OVERLAY_BASE);
      references.set(overlay, image);
      return image;
    })();
    referenceBuilds.set(overlay, pending);
  }
  return pending;
}

export function expectedSlice(image: Buffer, offset: number, span: number): Buffer {
  if (offset < 0 || offset + span > image.length) throw new Error(`span outside overlay image: +0x${offset.toString(16)}+${span}`);
  return image.subarray(offset, offset + span);
}

// ---- 採点 -------------------------------------------------------------

async function run(command: readonly string[], cwd: string): Promise<{ code: number; stdout: string }> {
  const child = Bun.spawn([...command], { cwd, stdout: "pipe", stderr: "pipe" });
  const [stdout, code] = await Promise.all([new Response(child.stdout).text(), child.exited]);
  return { code, stdout };
}

export function differingHalfwords(actual: Buffer, expected: Buffer): number {
  let differing = 0;
  const limit = Math.max(actual.length, expected.length);
  for (let index = 0; index + 1 < limit; index += 2) {
    const a = index + 1 < actual.length ? actual.readUInt16LE(index) : null;
    const e = index + 1 < expected.length ? expected.readUInt16LE(index) : null;
    if (a !== e) differing++;
  }
  return differing;
}

export class OverlayScorer {
  static cache = new Map<string, number>();
  static cacheFile = "";
  static evaluated = 0;
  private symbolsObject: string | null = null;
  private symbolsKey = "";
  constructor(
    readonly target: Target,
    readonly expected: Buffer,
    readonly scratch: string,
  ) {}

  // 候補を組み立て、リンクした.textを関数の大きさへ切って返す。
  // 未定義参照は伴走オブジェクトで絶対番地に束ねる。束は集合が変わったときだけ作る。
  async bytes(source: string): Promise<Buffer | null> {
    const stem = this.target.stem;
    const prefix = join(this.scratch, stem);
    const cFile = `${prefix}.c`;
    writeFileSync(cFile, source);
    const plan = sourceToAssemblyPlan({
      target: "gs1",
      routingSource: this.target.routing,
      input: cFile,
      output: `${prefix}.s`,
      preprocessedOutput: `${prefix}.i`,
    });
    for (const step of plan.steps) {
      if ((await run(step.command, this.scratch)).code !== 0) return null;
    }
    const object = `${prefix}.o`;
    if ((await run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", object, `${prefix}.s`], this.scratch)).code !== 0) return null;
    const undefinedNames: string[] = [];
    const listed = await run(["arm-none-eabi-nm", "-u", object], this.scratch);
    if (listed.code !== 0) return null;
    for (const line of listed.stdout.split(/\r?\n/).filter(Boolean)) {
      const name = line.trim().split(/\s+/).at(-1)!;
      if (externalSymbol(name) === null) return null;
      undefinedNames.push(name);
    }
    const key = [...undefinedNames].sort().join(",");
    if (key !== this.symbolsKey || this.symbolsObject === null) {
      const symbolsSource = `${prefix}.symbols.s`;
      const symbolsObject = `${prefix}.symbols.o`;
      writeFileSync(symbolsSource, ".syntax unified\n.thumb\n" + undefinedNames.map((name) => externalSymbolAssembly(name)).join(""));
      if ((await run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", symbolsObject, symbolsSource], this.scratch)).code !== 0) return null;
      this.symbolsObject = symbolsObject;
      this.symbolsKey = key;
    }
    const elf = `${prefix}.elf`;
    const binary = `${prefix}.bin`;
    const symbol = `Func_${stem}`;
    if ((await run(["arm-none-eabi-ld", `-Ttext=0x${stem}`, "-e", symbol, "-o", elf, object, this.symbolsObject], this.scratch)).code !== 0) return null;
    if ((await run(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, binary], this.scratch)).code !== 0) return null;
    // 関数の大きさはnmの記号表から取る。検証器と採用器と同じ切り方にする。
    const symbols = await run(["arm-none-eabi-nm", "-S", elf], this.scratch);
    if (symbols.code !== 0) return null;
    const row = symbols.stdout.split(/\r?\n/).find((line) => line.endsWith(` ${symbol}`));
    if (row === undefined) return null;
    const fields = row.trim().split(/\s+/);
    const image = readFileSync(binary);
    const size = fields.length >= 4 ? Number.parseInt(fields[1], 16) : image.length;
    return image.subarray(0, size);
  }

  async score(source: string): Promise<number> {
    const key = [
      "o1", SIGNATURE, this.target.routing, this.target.stem,
      new Bun.CryptoHasher("sha256").update(this.expected).digest("hex"),
      new Bun.CryptoHasher("sha256").update(source).digest("hex"),
    ].join(":");
    const cached = OverlayScorer.cache.get(key);
    if (cached !== undefined) return cached;
    const actual = await this.bytes(source);
    const value = actual === null ? Number.MAX_SAFE_INTEGER : weightedDiff(actual, this.expected);
    OverlayScorer.cache.set(key, value);
    OverlayScorer.evaluated++;
    if (OverlayScorer.cacheFile !== "" && value !== Number.MAX_SAFE_INTEGER) {
      appendFileSync(OverlayScorer.cacheFile, `${key} ${value}\n`);
    }
    return value;
  }

  // 報告用の素の計量: 半語差の個数と組み立てた大きさ。
  async measure(source: string): Promise<{ size: number; differing: number } | null> {
    const actual = await this.bytes(source);
    if (actual === null) return null;
    return { size: actual.length, differing: differingHalfwords(actual, this.expected) };
  }
}

// ---- 状態 -------------------------------------------------------------

function statePath(target: Target): string {
  return join(STATE_DIR, `${target.overlay}-${target.offset.toString(16).padStart(4, "0")}.json`);
}

function loadState(target: Target): TargetState {
  const path = statePath(target);
  if (existsSync(path)) {
    const state = JSON.parse(readFileSync(path, "utf8")) as Partial<TargetState>;
    if (state.signature === SIGNATURE && state.best !== undefined && state.population !== undefined &&
        state.operators !== undefined && state.rounds !== undefined) {
      return state as TargetState;
    }
  }
  return { signature: SIGNATURE, best: null, population: [], operators: {}, rounds: [] };
}

function saveState(target: Target, state: TargetState): void {
  state.signature = SIGNATURE;
  mkdirSync(STATE_DIR, { recursive: true });
  writeFileSync(statePath(target), JSON.stringify(state));
}

// 作用素表から名前で借りる。permute_v1と同じ実装を使うためのただの索引。
function operatorNamed(name: string): (body: string, random: () => number) => string | null {
  const found = OPERATORS.find(([key]) => key === name);
  if (found === undefined) throw new Error(`missing operator: ${name}`);
  return found[1];
}

// PROVENANCE.mdの規則: インラインアセンブリ、レジスタ固定、volatileを一致の
// 道具に使ってはならない。名前で禁じるだけでなく、出力に禁制語が現れた変形も
// 捨てる。下書きが元から持つ語は数に入れない。
// 参照像の並びは「元の順を保つ」側に落ちることが多く、近傍交換だけでは
// 届かない。同じ深さの文を数個ぶん跨いで動かす作用素を足す。跨ぐ文すべてと
// 依存が無いことを確かめてから動かす。
function reorderStatement(body: string, random: () => number): string | null {
  const { rows, statements } = parseStatements(body);
  if (statements.length < 2) return null;
  const index = Math.floor(random() * statements.length);
  const moved = statements[index];
  const destinations = statements.filter((statement, other) =>
    other !== index && statement.depth === moved.depth && Math.abs(other - index) <= 8);
  if (destinations.length === 0) return null;
  const destination = destinations[Math.floor(random() * destinations.length)];
  const low = Math.min(moved.line, destination.line);
  const high = Math.max(moved.line, destination.line);
  for (const statement of statements) {
    if (statement.line >= low && statement.line <= high && statement.line !== moved.line &&
        conflicts(statement, moved)) return null;
  }
  const copy = [...rows];
  const text = copy[moved.line].replace(moved.indent, destination.indent);
  copy.splice(moved.line, 1);
  copy.splice(moved.line < destination.line ? destination.line : destination.line, 0, text);
  const result = copy.join("\n");
  return result === body ? null : result;
}

// 初期化付き宣言を宣言と代入に割る。代入が動けるようになり、
// 参照像の並びへ寄せる余地が生まれる。
function splitInitializer(body: string, random: () => number): string | null {
  const pattern = /^(\s+)((?:s8|u8|s16|u16|s32|u32|void|[A-Z]\w*)\s*\**\s*)(\w+) = ([^;]+);$/gm;
  const matches = [...body.matchAll(pattern)];
  if (matches.length === 0) return null;
  const target = matches[Math.floor(random() * matches.length)];
  const declaration = `${target[1]}${target[2]}${target[3]};`;
  const assignment = `${target[1]}${target[3]} = ${target[4]};`;
  // 代入は宣言群の直後、最初の文の前に置く。以後reorderが動かす。
  const replaced = body.slice(0, target.index) + declaration + body.slice(target.index! + target[0].length);
  const { statements } = parseStatements(replaced);
  const first = statements.find((statement) => statement.line * 0 === 0);
  if (first === undefined) return null;
  const rows = replaced.split("\n");
  rows.splice(first.line, 0, assignment);
  return rows.join("\n");
}

export const OVERLAY_OPERATORS: Array<[string, Operator]> = [
  ["reorder", reorderStatement],
  ["initsplit", splitInitializer],
];

const BANNED_OPERATORS = new Set(["volatile", "volatilize"]);
const BANNED_TOKEN = /\b(volatile|asm|__asm__|__attribute__|register)\b/g;

export function cleanRoomOperators(
  draft: string,
  table: ReadonlyArray<[string, (body: string, random: () => number) => string | null]> = [...OPERATORS, ...OVERLAY_OPERATORS],
): Array<[string, (body: string, random: () => number) => string | null]> {
  const allowance = (draft.match(BANNED_TOKEN) ?? []).length;
  return table
    .filter(([name]) => !BANNED_OPERATORS.has(name))
    .map(([name, operator]) => [name, (body: string, random: () => number) => {
      const next = operator(body, random);
      if (next === null) return null;
      return (next.match(BANNED_TOKEN) ?? []).length > allowance ? null : next;
    }] as [string, (body: string, random: () => number) => string | null]);
}

function fixedPoint(body: string, name: string, rounds = 12): string {
  const operator = operatorNamed(name);
  let current = body;
  for (let round = 0; round < rounds; round++) {
    const next = operator(current, makeRandom(round + 1 + name.length * 31));
    if (next === null) break;
    current = next;
  }
  return current;
}

// ---- 当たりの整理 -----------------------------------------------------
// 焼きなましは死んだ局所変数や不要な入れ物を残す。採用する前に、
// 一致を保てる範囲で下書きの綴りへ差し戻し、痕跡を落とす。

export function lineHunks(from: readonly string[], to: readonly string[]): Array<[number, number, number, number]> {
  const rows = from.length, cols = to.length;
  const lengths: number[][] = Array.from({ length: rows + 1 }, () => new Array<number>(cols + 1).fill(0));
  for (let row = rows - 1; row >= 0; row--) {
    for (let col = cols - 1; col >= 0; col--) {
      lengths[row][col] = from[row] === to[col]
        ? lengths[row + 1][col + 1] + 1
        : Math.max(lengths[row + 1][col], lengths[row][col + 1]);
    }
  }
  const hunks: Array<[number, number, number, number]> = [];
  let row = 0, col = 0;
  while (row < rows && col < cols) {
    if (from[row] === to[col]) { row++; col++; continue; }
    const startRow = row, startCol = col;
    while (row < rows && col < cols && from[row] !== to[col]) {
      if (lengths[row + 1][col] >= lengths[row][col + 1]) row++; else col++;
    }
    hunks.push([startRow, row, startCol, col]);
  }
  if (row < rows || col < cols) hunks.push([row, rows, col, cols]);
  return hunks;
}

async function reduce(target: Target, scorer: OverlayScorer, draft: string, hit: string): Promise<string> {
  if (await scorer.score(hit) !== 0) throw new Error("source to reduce is not byte-exact");
  const from = draft.split("\n");
  let current = hit.split("\n");
  // 差分の塊を後ろから順に下書きへ差し戻す。前の塊の位置は動かない。
  const hunks = lineHunks(from, current);
  let reverted = 0;
  for (const [fromStart, fromEnd, toStart, toEnd] of [...hunks].reverse()) {
    const trial = [...current.slice(0, toStart), ...from.slice(fromStart, fromEnd), ...current.slice(toEnd)];
    if (await scorer.score(trial.join("\n")) === 0) { current = trial; reverted++; }
  }
  // 使われなくなった宣言と死んだ代入を落とす。
  let dropped = 0;
  for (let pass = 0; pass < 3; pass++) {
    let changed = false;
    for (let index = 0; index < current.length; index++) {
      if (!/_v1[dt]\d+/.test(current[index])) continue;
      const trial = [...current];
      trial[index] = current[index].replace(/\s*_v1[dt]\d+ = [^;]+;/g, "").replace(/^\s*(?:s32|u32|s16|u16|u8|void \*+)\s*_v1[dt]\d+;\s*$/, "");
      if (trial[index] === current[index]) continue;
      if (trial[index].trim() === "") trial.splice(index, 1);
      if (await scorer.score(trial.join("\n")) === 0) { current = trial; dropped++; changed = true; index--; }
    }
    if (!changed) break;
  }
  console.log(`reduce ${target.id}: reverted=${reverted}/${hunks.length} artefacts-dropped=${dropped}`);
  return current.join("\n");
}

// ---- 自己検査 ---------------------------------------------------------

async function selfTest(): Promise<void> {
  const specification = parseTargetSpecification("resource_3b8:00c8 span=0x40 source=work/drafts/x.c");
  if (specification === null || specification.overlay !== "resource_3b8" || specification.offset !== 0xc8 ||
      specification.stem !== "020000c8" || specification.span !== 64 || specification.draft !== "work/drafts/x.c") {
    throw new Error("target specification self-test failed");
  }
  if (parseTargetSpecification("  # comment") !== null) throw new Error("comment self-test failed");
  let threw = false;
  try { parseTargetSpecification("resource_3b8-00c8"); } catch { threw = true; }
  if (!threw) throw new Error("malformed target self-test failed");

  const image = Buffer.alloc(64);
  for (let index = 0; index < image.length; index++) image[index] = index;
  const slice = expectedSlice(image, 8, 4);
  if (slice.length !== 4 || slice[0] !== 8 || slice[3] !== 11) throw new Error("expected slice self-test failed");
  let outside = false;
  try { expectedSlice(image, 60, 8); } catch { outside = true; }
  if (!outside) throw new Error("out-of-range span self-test failed");

  const left = Buffer.from([0x00, 0x20, 0x01, 0x21]);
  const right = Buffer.from([0x00, 0x20, 0x02, 0x21]);
  if (differingHalfwords(left, left) !== 0 || differingHalfwords(left, right) !== 1) {
    throw new Error("halfword count self-test failed");
  }
  if (differingHalfwords(left, left.subarray(0, 2)) !== 1) throw new Error("halfword length self-test failed");
  if (weightedDiff(left, left) !== 0 || weightedDiff(left, right) === 0) throw new Error("weighted diff self-test failed");

  // 禁制語を持ち込む変形は捨てられること。
  const filtered = cleanRoomOperators("s32 a;\n", [
    ["volatile", () => "banned by name"],
    ["sneak", () => "volatile s32 a;\n"],
    ["clean", () => "u32 a;\n"],
  ]);
  if (filtered.length !== 2 || filtered[0][0] !== "sneak") throw new Error("banned operator self-test failed");
  if (filtered[0][1]("s32 a;\n", () => 0) !== null) throw new Error("banned token self-test failed");
  if (filtered[1][1]("s32 a;\n", () => 0) !== "u32 a;\n") throw new Error("clean operator self-test failed");

  // 長距離の文移動が依存を跨がないこと。
  const orderable = [
    "void Func_02000000(void) {",
    "    a = 1;",
    "    b = 2;",
    "    c = 3;",
    "}",
  ].join("\n");
  const reorderer = OVERLAY_OPERATORS.find(([name]) => name === "reorder")![1];
  let sawMove = false;
  for (let trial = 0; trial < 40; trial++) {
    const next = reorderer(orderable, makeRandom(trial + 1));
    if (next === null) continue;
    const lines = next.split("\n").filter((line) => /= \d;/.test(line));
    if (lines.length !== 3) throw new Error("reorder lost a statement");
    if (next !== orderable) sawMove = true;
  }
  if (!sawMove) throw new Error("reorder self-test failed");
  const dependent = [
    "void Func_02000000(void) {",
    "    a = 1;",
    "    b = a;",
    "}",
  ].join("\n");
  for (let trial = 0; trial < 40; trial++) {
    if (reorderer(dependent, makeRandom(trial + 1)) !== null) throw new Error("reorder crossed a dependence");
  }
  const splitter = OVERLAY_OPERATORS.find(([name]) => name === "initsplit")![1];
  const split = splitter("void Func_02000000(void) {\n    s32 a = 1;\n    b = 2;\n}", makeRandom(3));
  if (split === null || !split.includes("    s32 a;") || !/^    a = 1;$/m.test(split)) {
    throw new Error("initsplit self-test failed");
  }

  const hunks = lineHunks(["a", "b", "c", "d"], ["a", "x", "y", "d"]);
  if (hunks.length !== 1 || hunks[0].join(",") !== "1,3,1,3") throw new Error("hunk self-test failed");
  if (lineHunks(["a", "b"], ["a", "b"]).length !== 0) throw new Error("empty hunk self-test failed");
  const appended = lineHunks(["a"], ["a", "b"]);
  if (appended.length !== 1 || appended[0].join(",") !== "1,1,1,2") throw new Error("tail hunk self-test failed");

  // 焼きなましがpermute_v1の作用素表を回して的に届くこと。
  const body = [
    "void Func_02000000(void) {",
    "    s32 a;",
    "    s32 b;",
    "    a = 1;",
    "    b = 2;",
    "    Func_02000010(a, b);",
    "}",
  ].join("\n");
  const state: TargetState = { signature: SIGNATURE, best: null, population: [], operators: {}, rounds: [] };
  let matched = "";
  const outcome = await anneal({
    key: 0x02000000,
    seed: 1,
    steps: 600,
    restarts: 2,
    state,
    best: { body, score: 4 },
    score: async (candidate) => (/_v1d\d+ = 0;/.test(candidate) ? 0 : 4),
    onMatch: (candidate) => { matched = candidate; return true; },
  });
  if (!outcome.done || outcome.best.score !== 0 || !/_v1d\d+ = 0;/.test(matched)) {
    throw new Error("anneal self-test failed");
  }
  if (outcome.evaluated === 0) throw new Error("anneal evaluation self-test failed");
  console.log("self-test=ok");
}

// ---- 主探索 -----------------------------------------------------------

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) {
    await selfTest();
    return;
  }
  const options = parseArguments(Bun.argv.slice(2));
  let targets = resolveTargets(options);
  if (options.limit !== undefined) targets = targets.slice(0, options.limit);
  if (targets.length === 0) {
    console.log("targets=0");
    return;
  }
  const work = join(OUT, "work");
  mkdirSync(work, { recursive: true });
  mkdirSync(CACHE_DIR, { recursive: true });
  mkdirSync(HITS_DIR, { recursive: true });
  for (const name of readdirSync(CACHE_DIR).filter((entry) => entry.endsWith(".log"))) {
    for (const line of readFileSync(join(CACHE_DIR, name), "utf8").split("\n")) {
      const space = line.indexOf(" ");
      if (space > 0 && line.startsWith(`o1:${SIGNATURE}:`)) {
        OverlayScorer.cache.set(line.slice(0, space), Number(line.slice(space + 1)));
      }
    }
  }
  OverlayScorer.cacheFile = join(CACHE_DIR, `${SIGNATURE.slice(0, 16)}-seed${options.seed}.log`);
  console.log(`targets=${targets.length} steps=${options.steps} restarts=${options.restarts} seed=${options.seed} jobs=${options.jobs} cache=${OverlayScorer.cache.size}`);

  const started = Date.now();
  let hits = 0;
  let cursor = 0;

  async function processTarget(target: Target): Promise<void> {
    const scratch = join(work, `${target.overlay}-${target.offset.toString(16)}`);
    mkdirSync(scratch, { recursive: true });
    const image = await reference(target.overlay);
    let expected: Buffer;
    try {
      expected = expectedSlice(image, target.offset, target.span);
    } catch (error) {
      console.log(`unusable ${target.id}: ${(error as Error).message}`);
      return;
    }
    const scorer = new OverlayScorer(target, expected, scratch);
    const state = loadState(target);
    const draft = readFileSync(target.draft, "utf8");
    const bases = new Set<string>([draft, fixedPoint(draft, "inline"), fixedPoint(draft, "uncse")]);
    if (state.best !== null) bases.add(state.best.body);
    for (const entry of state.population) bases.add(entry.body);
    let best: Individual | null = null;
    for (const base of bases) {
      const value = await scorer.score(base);
      if (best === null || value < best.score) best = { body: base, score: value };
    }
    if (best === null || best.score === Number.MAX_SAFE_INTEGER) {
      console.log(`unusable ${target.id}: draft does not build`);
      return;
    }
    const baseline = await scorer.measure(best.body);
    console.log(`base ${target.id}: score=${best.score} hw=${baseline?.differing ?? "?"} size=${baseline?.size ?? "?"}/${target.span} draft=${basename(target.draft)}`);
    if (options.probe) return;
    if (options.reduce !== undefined) {
      const reduced = await reduce(target, scorer, draft, readFileSync(options.reduce, "utf8"));
      const measured = await scorer.measure(reduced);
      if (measured === null || measured.differing !== 0 || measured.size !== target.span) throw new Error("reduction lost the match");
      const path = join(HITS_DIR, `${target.overlay}-${target.offset.toString(16).padStart(4, "0")}-reduced.c`);
      writeFileSync(path, reduced);
      console.log(`reduced ${target.id} span=${target.span} source=${path}`);
      return;
    }

    const outcome = await anneal({
      key: Number.parseInt(target.stem, 16),
      seed: options.seed,
      steps: options.steps,
      restarts: options.restarts,
      state,
      best,
      operators: cleanRoomOperators(draft),
      score: (body) => scorer.score(body),
      onMatch: async (body) => {
        const measured = await scorer.measure(body);
        if (measured === null || measured.differing !== 0 || measured.size !== target.span) {
          console.log(`near ${target.id}: score=0 but size=${measured?.size ?? "?"}/${target.span} hw=${measured?.differing ?? "?"}`);
          return false;
        }
        const path = join(HITS_DIR, `${target.overlay}-${target.offset.toString(16).padStart(4, "0")}.c`);
        writeFileSync(path, body);
        console.log(`hit ${target.id} span=${target.span} source=${path}`);
        hits++;
        return true;
      },
    });
    best = outcome.best;
    if (outcome.done) {
      rmSync(statePath(target), { force: true });
      return;
    }
    const measured = await scorer.measure(best.body);
    state.rounds.push({ seed: options.seed, before: state.best?.score ?? Number.MAX_SAFE_INTEGER, after: best.score });
    state.rounds = state.rounds.slice(-8);
    state.best = best;
    saveState(target, state);
    if (best.score < (baseline === null ? Number.MAX_SAFE_INTEGER : best.score + 1)) {
      writeFileSync(join(OUT, `${target.overlay}-${target.offset.toString(16).padStart(4, "0")}-best.c`), best.body);
    }
    console.log(`floor ${target.id}: score=${best.score} hw=${measured?.differing ?? "?"} size=${measured?.size ?? "?"}/${target.span} evaluated=${outcome.evaluated}`);
  }

  async function pump(): Promise<void> {
    while (true) {
      const index = cursor++;
      if (index >= targets.length) return;
      try {
        await processTarget(targets[index]);
      } catch (error) {
        console.log(`failed ${targets[index].id}: ${(error as Error).message}`);
      }
    }
  }
  await Promise.all(Array.from({ length: Math.max(1, options.jobs) }, pump));
  const seconds = (Date.now() - started) / 1000;
  console.log(`hits=${hits} of ${targets.length} candidates=${OverlayScorer.evaluated} in ${seconds.toFixed(0)}s (${(OverlayScorer.evaluated / Math.max(seconds, 0.001)).toFixed(1)}/s)`);
}

if (import.meta.main) await main();
