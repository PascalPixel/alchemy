#!/usr/bin/env bun
// 式合成。再構築器の第四部。synthesize_block（第三部）が診断したとおり、
// 即値一意の述語や小さな算術関数は採掘した区間の移植では再構築できない。
// 本器はそれらを命令の意味から直接組み立てる。asm/<stem>.sのcmpと条件分岐
// の木、movsと戻り値を解釈して引数上の真理値式（等値集合・範囲検査・既定値）
// を復元し、複数のC雛形（前後どちらの並びのif連鎖・switch・||連鎖・入れ子
// 比較・決定木の構造写像）を生成する。GCC 2.96はこれらを別々に降ろすので
// 変種を全て出し、承認コンパイラのコンパイル・リンク・差分路で全採点する。
// 完全一致のみ、領域長検査（組立asmのバイト数＝一致長）と全検証を経て
// src/へ設置し、対応するasm/を除く。ROMバイトには触れず、出力はout/配下のみ。
//
// これは試作。合成が破れる箇所（多引数述語・記憶参照・語プール・到達不能な
// 基本ブロック配置）を明示するのが目的で、負の結果でも診断が明確なら成功。
import { existsSync, mkdirSync, readFileSync, readdirSync, rmSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { M2C_PREAMBLE } from "./match_m2c.ts";
import { weightedDiff } from "./permute_v1.ts";
import { replaceableAssembly } from "./permute_m2c.ts";
import { verify } from "./verify.ts";
import { CFLAGS, compilerCommand, externalSymbol, externalSymbolAssembly } from "./alchemy_gcc.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;
const SCRATCH = process.env.CLAUDE_SCRATCH ?? join(ROOT, "out", "rebuild", "expr");
const NEG_INF = Number.NEGATIVE_INFINITY;
const POS_INF = Number.POSITIVE_INFINITY;

// ---- asm解析 ----------------------------------------------------------

interface Insn {
  mnemonic: string; // .n/.w接尾辞と末尾フラグsを除いた小文字基本形
  raw: string; // 元の機種（分岐種別判定用）
  operands: string[]; // カンマ区切り、各要素をtrim
  label: string | null; // 直前に付いていたラベル
}

// フラグ設定形の末尾sを剥がして基本名へ揃える（movs→mov 等）。
const FLAG_S = /^(mov|mvn|add|adc|sub|sbc|rsb|and|orr|eor|bic|lsl|lsr|asr|ror|neg|mul|tst|cmp|cmn)s$/;
function baseName(mnemonic: string): string {
  const bare = mnemonic.toLowerCase().replace(/\.(n|w)$/, "");
  return FLAG_S.test(bare) ? bare.slice(0, -1) : bare;
}

// 関数本体（Func_<stem>:から復帰まで）の命令列を線形に取り出す。
// 語プール（.4byte等のデータ指令）とアセンブラ指令は落とす。ラベルは
// 直後の命令に添える。解析できないデータ混在なら例外。
function parseAsm(stem: string): Insn[] {
  const text = readFileSync(join(ROOT, "asm", `${stem}.s`), "utf8");
  const insns: Insn[] = [];
  let pendingLabel: string | null = null;
  let started = false;
  const entry = `Func_${stem}`;
  for (const rawLine of text.split("\n")) {
    const line = rawLine.replace(/@.*$/, "").trim();
    if (line === "") continue;
    const labelMatch = line.match(/^(\.?[A-Za-z_][\w.$]*):$/);
    if (labelMatch) {
      const name = labelMatch[1];
      if (name === entry) started = true;
      pendingLabel = name;
      continue;
    }
    if (line.startsWith(".")) continue; // .syntax/.thumb/.global/.4byte/.thumb_func 等
    if (!started) continue;
    const space = line.search(/\s/);
    const rawMnemonic = space === -1 ? line : line.slice(0, space);
    const operandText = space === -1 ? "" : line.slice(space + 1).trim();
    const operands = operandText === "" ? [] : splitOperands(operandText);
    // rawは分岐条件の判定に使うので.n/.w接尾辞のみ剥がす（末尾sは残す）。
    const raw = rawMnemonic.toLowerCase().replace(/\.(n|w)$/, "");
    insns.push({ mnemonic: baseName(rawMnemonic), raw, operands, label: pendingLabel });
    pendingLabel = null;
  }
  return insns;
}

// レジスタ集合{r0, r1}のような波括弧をひとまとまりに保ちつつカンマ分割する。
function splitOperands(text: string): string[] {
  const out: string[] = [];
  let depth = 0;
  let current = "";
  for (const character of text) {
    if (character === "{" || character === "[") depth++;
    if (character === "}" || character === "]") depth--;
    if (character === "," && depth === 0) {
      out.push(current.trim());
      current = "";
    } else current += character;
  }
  if (current.trim() !== "") out.push(current.trim());
  return out;
}

// ---- 命令の意味 -------------------------------------------------------

class Unsupported extends Error {}

const REGISTER = /^(r(?:1[0-2]|[0-9])|sp|lr|pc|ip|fp|sl)$/;
function isRegister(token: string): boolean {
  return REGISTER.test(token);
}
function parseImmediate(token: string): number {
  const body = token.startsWith("#") ? token.slice(1) : token;
  const value = Number(body);
  if (!Number.isFinite(value)) throw new Unsupported(`即値解析不能: ${token}`);
  return value | 0;
}

interface Flags {
  z: boolean;
  n: boolean;
  c: boolean;
  v: boolean;
}

// cmp/subの減算からフラグを立てる（符号付き・符号無しの両条件を保持）。
function subtractFlags(a: number, b: number): Flags {
  const ua = a >>> 0;
  const ub = b >>> 0;
  const result = (a - b) | 0;
  return {
    z: result === 0,
    n: result < 0,
    c: ua >= ub, // 借り無し
    v: (((a ^ b) & (a ^ result)) >>> 31) === 1,
  };
}
function addFlags(a: number, b: number): Flags {
  const result = (a + b) | 0;
  return {
    z: result === 0,
    n: result < 0,
    c: (a >>> 0) + (b >>> 0) > 0xffffffff,
    v: ((~(a ^ b) & (a ^ result)) >>> 31) === 1,
  };
}

const CONDITION = new Set([
  "beq", "bne", "bgt", "blt", "bge", "ble", "bhi", "bls", "bcs", "bcc", "bmi", "bpl", "bvs", "bvc", "bhs", "blo",
]);
function branchTaken(mnemonic: string, flags: Flags): boolean {
  switch (mnemonic) {
    case "beq": return flags.z;
    case "bne": return !flags.z;
    case "bge": return flags.n === flags.v;
    case "blt": return flags.n !== flags.v;
    case "bgt": return !flags.z && flags.n === flags.v;
    case "ble": return flags.z || flags.n !== flags.v;
    case "bhi": return flags.c && !flags.z;
    case "bls": return !flags.c || flags.z;
    case "bcs": case "bhs": return flags.c;
    case "bcc": case "blo": return !flags.c;
    case "bmi": return flags.n;
    case "bpl": return !flags.n;
    case "bvs": return flags.v;
    case "bvc": return !flags.v;
    default: throw new Unsupported(`条件分岐未対応: ${mnemonic}`);
  }
}

// 条件接尾辞（beq→==）を関係演算子へ写す。符号無し種は別扱いで印を付す。
function conditionOperator(mnemonic: string): { operator: string; unsigned: boolean } {
  switch (mnemonic) {
    case "beq": return { operator: "==", unsigned: false };
    case "bne": return { operator: "!=", unsigned: false };
    case "bgt": return { operator: ">", unsigned: false };
    case "blt": return { operator: "<", unsigned: false };
    case "bge": return { operator: ">=", unsigned: false };
    case "ble": return { operator: "<=", unsigned: false };
    case "bhi": return { operator: ">", unsigned: true };
    case "bls": return { operator: "<=", unsigned: true };
    case "bcs": case "bhs": return { operator: ">=", unsigned: true };
    case "bcc": case "blo": return { operator: "<", unsigned: true };
    default: throw new Unsupported(`関係化不能な条件: ${mnemonic}`);
  }
}

// ---- 具体解釈器（標本化による特性化に用いる）-------------------------
// r0..r3を入力レジスタとし、cmp/条件分岐/算術/mov/復帰の部分集合を実行する。
// 記憶参照（ldr/str…）や未対応命令に触れたら例外を投げ、当該対象を非対応と
// して弾く。これが述語・算術以外を自然に除外する関門になる。

interface Program {
  insns: Insn[];
  labelIndex: Map<string, number>;
}
function buildProgram(insns: Insn[]): Program {
  const labelIndex = new Map<string, number>();
  insns.forEach((insn, index) => {
    if (insn.label !== null) labelIndex.set(insn.label, index);
  });
  return { insns, labelIndex };
}

function registerSlot(token: string): number {
  const match = token.match(/^r(\d+)$/);
  if (match) return Number(match[1]);
  if (token === "ip" || token === "fp" || token === "sl" || token === "lr" || token === "sp") return -1;
  throw new Unsupported(`レジスタ解釈不能: ${token}`);
}

// r0..r3のうち「書かれる前に読まれる」最大番号+1を引数個数とみなす。
function inputArgumentCount(insns: Insn[]): number {
  const written = new Set<number>();
  let maxArg = -1;
  for (const insn of insns) {
    if (insn.mnemonic === "push" || insn.mnemonic === "pop" || insn.mnemonic === "b" || CONDITION.has(insn.raw) || insn.mnemonic === "bx") continue;
    const writesFirst = /^(mov|mvn|add|sub|neg|rsb|and|orr|eor|bic|lsl|lsr|asr|ror|mul|ldr|ldrb|ldrh|ldrsh|ldrsb)$/.test(insn.mnemonic);
    insn.operands.forEach((operand, index) => {
      const match = operand.match(/^r([0-3])$/);
      if (!match) return;
      const slot = Number(match[1]);
      const isDestination = writesFirst && index === 0;
      if (!isDestination && !written.has(slot)) maxArg = Math.max(maxArg, slot);
      if (isDestination) written.add(slot);
    });
  }
  return maxArg + 1;
}

// 入力レジスタ番号→値の割当てで関数を実行し、r0の戻り値を返す。
function evaluate(program: Program, inputs: number[]): number {
  const registers = new Array<number>(16).fill(0);
  for (let index = 0; index < inputs.length; index++) registers[index] = inputs[index] | 0;
  let flags: Flags = { z: false, n: false, c: false, v: false };
  let popTarget = -1; // pop {..,rN} で退避したlrの行き先
  let steps = 0;
  let pc = 0;
  const source = (token: string): number => {
    if (isRegister(token)) return registers[registerSlot(token)] | 0;
    return parseImmediate(token);
  };
  while (pc < program.insns.length) {
    if (++steps > 100000) throw new Unsupported("反復上限（ループの疑い）");
    const insn = program.insns[pc];
    const [d, a, b] = insn.operands;
    const dst = () => registerSlot(d);
    switch (insn.mnemonic) {
      case "nop": break;
      case "mov": case "mvn": {
        let value = insn.operands.length >= 2 ? source(a) : source(a);
        if (insn.mnemonic === "mvn") value = ~value;
        registers[dst()] = value | 0;
        break;
      }
      case "movw": registers[dst()] = source(a) | 0; break;
      case "cmp": flags = subtractFlags(source(d), source(a)); break;
      case "cmn": flags = addFlags(source(d), source(a)); break;
      case "tst": {
        const result = source(d) & source(a);
        flags = { z: result === 0, n: result < 0, c: flags.c, v: flags.v };
        break;
      }
      case "add": {
        const x = insn.operands.length === 3 ? source(a) : source(d);
        const y = insn.operands.length === 3 ? source(b) : source(a);
        registers[dst()] = (x + y) | 0; break;
      }
      case "sub": {
        const x = insn.operands.length === 3 ? source(a) : source(d);
        const y = insn.operands.length === 3 ? source(b) : source(a);
        registers[dst()] = (x - y) | 0; break;
      }
      case "rsb": registers[dst()] = (source(b === undefined ? "#0" : b) - source(a)) | 0; break;
      case "neg": registers[dst()] = (-source(a)) | 0; break;
      case "mul": {
        const x = insn.operands.length === 3 ? source(a) : source(d);
        const y = insn.operands.length === 3 ? source(b) : source(a);
        registers[dst()] = Math.imul(x, y) | 0; break;
      }
      case "lsl": registers[dst()] = (source(a) << (source(b ?? a) & 31)) | 0; break;
      case "lsr": registers[dst()] = (source(a) >>> (source(b ?? a) & 31)) | 0; break;
      case "asr": registers[dst()] = (source(a) >> (source(b ?? a) & 31)) | 0; break;
      case "ror": { const s = source(b ?? a) & 31; const x = source(a) >>> 0; registers[dst()] = ((x >>> s) | (x << (32 - s))) | 0; break; }
      case "and": registers[dst()] = (source(insn.operands.length === 3 ? a : d) & source(insn.operands.length === 3 ? b : a)) | 0; break;
      case "orr": registers[dst()] = (source(insn.operands.length === 3 ? a : d) | source(insn.operands.length === 3 ? b : a)) | 0; break;
      case "eor": registers[dst()] = (source(insn.operands.length === 3 ? a : d) ^ source(insn.operands.length === 3 ? b : a)) | 0; break;
      case "bic": registers[dst()] = (source(insn.operands.length === 3 ? a : d) & ~source(insn.operands.length === 3 ? b : a)) | 0; break;
      case "push": break;
      case "pop": {
        // pop {..., rN} でlr相当を退避した行き先を控える（bxで使う）。
        const set = insn.operands.map((token) => token.replace(/[{}]/g, "").trim()).filter(Boolean);
        if (set.includes("pc")) return registers[0] | 0; // pop {..,pc} は復帰
        const last = set.at(-1);
        if (last && isRegister(last)) popTarget = registerSlot(last);
        break;
      }
      case "bx": {
        return registers[0] | 0; // 戻り値はr0
      }
      case "b": {
        const target = insn.operands[0];
        const index = program.labelIndex.get(target);
        if (index === undefined) throw new Unsupported(`分岐先不明: ${target}`);
        pc = index; continue;
      }
      default: {
        if (CONDITION.has(insn.raw)) {
          if (branchTaken(insn.raw, flags)) {
            const target = insn.operands[0];
            const index = program.labelIndex.get(target);
            if (index === undefined) throw new Unsupported(`分岐先不明: ${target}`);
            pc = index; continue;
          }
          break;
        }
        throw new Unsupported(`命令未対応: ${insn.mnemonic}`);
      }
    }
    pc++;
  }
  void popTarget;
  throw new Unsupported("復帰に到達せず");
}

// ---- 特性化（単一引数の区分定数関数へ）--------------------------------

interface Interval {
  lo: number; // -Inf 可
  hi: number; // +Inf 可
  value: number;
}

// cmp/add/sub等に現れる即値を境界候補として集める。
function immediateBreakpoints(insns: Insn[]): number[] {
  const points = new Set<number>([0]);
  for (const insn of insns) {
    for (const operand of insn.operands) {
      if (operand.startsWith("#")) {
        try {
          points.add(parseImmediate(operand));
        } catch {
          /* 無視 */
        }
      }
    }
  }
  return [...points].sort((a, b) => a - b);
}

// 単一引数の関数を境界近傍で標本化し、区分定数の区間列へ畳む。
function characterize(program: Program, argumentCount: number): Interval[] {
  if (argumentCount > 1) throw new Unsupported("多引数述語（標本化は単一引数のみ）");
  const breakpoints = immediateBreakpoints(program.insns);
  const samples = new Set<number>([-2147483648, 2147483647, 0]);
  for (const point of breakpoints) {
    for (const delta of [-1, 0, 1]) {
      const value = point + delta;
      if (value >= -2147483648 && value <= 2147483647) samples.add(value);
    }
  }
  const xs = [...samples].sort((a, b) => a - b);
  const evaluated = xs.map((x) => ({ x, value: evaluate(program, [x]) }));
  // 連続する同値標本を一区間へ束ねる。端は±無限へ伸ばす。
  const intervals: Interval[] = [];
  for (const point of evaluated) {
    const last = intervals.at(-1);
    if (last && last.value === point.value) last.hi = point.x;
    else intervals.push({ lo: point.x, hi: point.x, value: point.value });
  }
  if (intervals.length > 0) {
    intervals[0].lo = NEG_INF;
    intervals[intervals.length - 1].hi = POS_INF;
  }
  return intervals;
}

// ---- 述語のC表現 ------------------------------------------------------

// 区間[lo,hi]を引数argに対する述語文字列へ。複数の表現を返す。
function intervalPredicates(argument: string, interval: Interval): string[] {
  const { lo, hi } = interval;
  if (lo === NEG_INF && hi === POS_INF) return ["1"];
  if (lo === hi) return [`${argument} == ${lo}`];
  if (lo === NEG_INF) return [`${argument} <= ${hi}`, `${argument} < ${hi + 1}`];
  if (hi === POS_INF) return [`${argument} >= ${lo}`, `${argument} > ${lo - 1}`];
  return [
    `${argument} >= ${lo} && ${argument} <= ${hi}`,
    `${argument} > ${lo - 1} && ${argument} < ${hi + 1}`,
  ];
}

// 区間が全て単一点なら列挙可能（switch生成に使う）。
function enumeratePoints(interval: Interval, limit = 8): number[] | null {
  if (interval.lo === NEG_INF || interval.hi === POS_INF) return null;
  if (interval.hi - interval.lo + 1 > limit) return null;
  const out: number[] = [];
  for (let value = interval.lo; value <= interval.hi; value++) out.push(value);
  return out;
}

interface Candidate {
  label: string;
  body: string;
}

// 特性化された区間から多様なC本文を生成する。
function candidatesFromIntervals(argument: string, intervals: Interval[]): Candidate[] {
  const candidates: Candidate[] = [];
  if (intervals.length === 0) return candidates;
  // 既定値: ±無限の裾の値。両裾が異なるなら+側を採る。
  const tailValue = intervals[intervals.length - 1].value;
  const special = intervals.filter((interval) => interval.value !== tailValue);
  if (special.length === 0) {
    // 定数関数。
    candidates.push({ label: "const", body: `  return ${tailValue};` });
    return candidates;
  }
  // 値ごとに区間をまとめる。
  const byValue = new Map<number, Interval[]>();
  for (const interval of special) {
    if (!byValue.has(interval.value)) byValue.set(interval.value, []);
    byValue.get(interval.value)!.push(interval);
  }
  const orderings: Array<{ tag: string; list: Interval[] }> = [
    { tag: "asc", list: [...special].sort((a, b) => a.lo - b.lo) },
    { tag: "desc", list: [...special].sort((a, b) => b.lo - a.lo) },
  ];
  const groupValues = [...byValue.keys()];
  const singleGroup = groupValues.length === 1;
  const groupValue = groupValues[0];

  // (1) 単一特殊値: 論理和による return。表現の別と並びの別を出す。
  if (singleGroup) {
    for (const rendering of [0, 1]) {
      for (const ordering of orderings) {
        const terms = ordering.list.map((interval) => {
          const options = intervalPredicates(argument, interval);
          const text = options[Math.min(rendering, options.length - 1)];
          return options.length > 1 || text.includes("&&") ? `(${text})` : text;
        });
        const disjunction = terms.join(" || ");
        if (groupValue === 1 && tailValue === 0) {
          candidates.push({ label: `disj-r${rendering}-${ordering.tag}`, body: `  return ${disjunction};` });
        }
        candidates.push({ label: `tern-r${rendering}-${ordering.tag}`, body: `  return (${disjunction}) ? ${groupValue} : ${tailValue};` });
        candidates.push({ label: `if1-r${rendering}-${ordering.tag}`, body: `  if (${disjunction}) return ${groupValue};\n  return ${tailValue};` });
      }
    }
  }

  // (2) 各区間ごとのif連鎖（早期return）。値混在にも対応。
  for (const rendering of [0, 1]) {
    for (const ordering of orderings) {
      const lines = ordering.list.map((interval) => {
        const options = intervalPredicates(argument, interval);
        return `  if (${options[Math.min(rendering, options.length - 1)]}) return ${interval.value};`;
      });
      candidates.push({ label: `chain-r${rendering}-${ordering.tag}`, body: `${lines.join("\n")}\n  return ${tailValue};` });
    }
  }

  // (3) 範囲の入れ子比較形。
  {
    const lines = orderings[0].list.map((interval) => {
      if (interval.lo !== NEG_INF && interval.hi !== POS_INF && interval.lo !== interval.hi) {
        return `  if (${argument} >= ${interval.lo}) { if (${argument} <= ${interval.hi}) return ${interval.value}; }`;
      }
      return `  if (${intervalPredicates(argument, interval)[0]}) return ${interval.value};`;
    });
    candidates.push({ label: "nested", body: `${lines.join("\n")}\n  return ${tailValue};` });
  }

  // (4) switch: 全特殊区間が小さな点集合に展開できるとき。
  const enumerated = special.map((interval) => ({ interval, points: enumeratePoints(interval) }));
  if (enumerated.every((entry) => entry.points !== null)) {
    for (const withDefault of [true, false]) {
      const arms = [...byValue.entries()].map(([value, list]) => {
        const cases = list.flatMap((interval) => enumeratePoints(interval)!).sort((a, b) => a - b);
        return `${cases.map((point) => `    case ${point}:`).join("\n")}\n      return ${value};`;
      });
      const tail = withDefault ? `    default:\n      return ${tailValue};\n` : "";
      const closing = withDefault ? "" : `\n  return ${tailValue};`;
      candidates.push({
        label: `switch-${withDefault ? "def" : "fall"}`,
        body: `  switch (${argument}) {\n${arms.join("\n")}\n${tail}  }${closing}`,
      });
    }
  }
  return candidates;
}

// ---- 決定木の構造写像 -------------------------------------------------
// ブロック単位でasmの分岐構造をそのままC（if/return）へ写す。GCCが同じ
// ブロック配置を選べば完全一致に最も近い候補になる。cmp対象は引数の別名で
// なければならず、記憶や別名不明があれば例外を投げて本生成を諦める。

interface Block {
  key: string;
  insns: Insn[]; // 端末を除く計算命令
  terminator: Insn; // 端末（分岐・復帰）
  fallthrough: string | null;
}

// ラベルと分岐で区切って基本ブロック列を作る。
function buildBlocks(program: Program): { blocks: Map<string, Block>; order: string[] } {
  const boundaries = new Set<number>([0]);
  program.insns.forEach((insn, index) => {
    if (insn.label !== null) boundaries.add(index);
    if (insn.mnemonic === "b" || insn.mnemonic === "bx" || CONDITION.has(insn.raw) || insn.mnemonic === "pop") {
      if (index + 1 < program.insns.length) boundaries.add(index + 1);
    }
  });
  const starts = [...boundaries].sort((a, b) => a - b);
  const keyOf = (index: number): string => program.insns[index].label ?? `@${index}`;
  const blocks = new Map<string, Block>();
  const order: string[] = [];
  for (let s = 0; s < starts.length; s++) {
    const start = starts[s];
    const end = s + 1 < starts.length ? starts[s + 1] : program.insns.length;
    const slice = program.insns.slice(start, end);
    const terminator = slice[slice.length - 1];
    const body = slice.slice(0, slice.length - 1);
    const isControl = terminator.mnemonic === "b" || terminator.mnemonic === "bx" || CONDITION.has(terminator.raw) || terminator.mnemonic === "pop";
    const key = keyOf(start);
    order.push(key);
    blocks.set(key, {
      key,
      insns: isControl ? body : slice,
      terminator: isControl ? terminator : { mnemonic: "fall", raw: "fall", operands: [], label: null },
      fallthrough: end < program.insns.length ? keyOf(end) : null,
    });
  }
  return { blocks, order };
}

// あるブロックから始めて条件分岐に出会わずに復帰するなら、そのr0定数を返す。
// 条件分岐に出会えば葉ではない（nullを返す）。
function resolveLeaf(program: Program, key: string, blocks: Map<string, Block>): number | null {
  let current: string | null = key;
  let value: number | null = null;
  const seen = new Set<string>();
  while (current !== null) {
    if (seen.has(current)) return null;
    seen.add(current);
    const block: Block | undefined = blocks.get(current);
    if (block === undefined) return null;
    for (const insn of block.insns) {
      if (insn.mnemonic === "mov" && insn.operands[0] === "r0" && insn.operands[1]?.startsWith("#")) {
        value = parseImmediate(insn.operands[1]);
      } else if (insn.mnemonic === "push") {
        /* 枠のみ、無視 */
      } else {
        return null; // r0=定数以外の計算がある葉は扱わない
      }
    }
    const terminator = block.terminator;
    if (terminator.mnemonic === "bx" || (terminator.mnemonic === "pop" && terminator.operands.some((o) => o.includes("pc")))) return value;
    if (terminator.mnemonic === "pop") {
      // pop {..,rN}; 直後の bx rN が同ブロック末でなければfallthroughを追う
      current = block.fallthrough;
      continue;
    }
    if (terminator.mnemonic === "b") {
      current = terminator.operands[0];
      continue;
    }
    if (terminator.mnemonic === "fall") {
      current = block.fallthrough;
      continue;
    }
    return null; // 条件分岐 → 葉ではない
  }
  return value;
}

// 引数の別名を各命令位置で追跡する（mov/adds #0 の連鎖のみ）。
function aliasMap(program: Program, argumentCount: number): Map<string, string>[] {
  const snapshots: Map<string, string>[] = [];
  const alias = new Map<string, string>();
  for (let index = 0; index < argumentCount; index++) alias.set(`r${index}`, `arg${index}`);
  for (const insn of program.insns) {
    snapshots.push(new Map(alias));
    const destination = insn.operands[0];
    if (destination === undefined || !/^r\d+$/.test(destination)) continue;
    if (insn.mnemonic === "mov" && /^r\d+$/.test(insn.operands[1] ?? "")) {
      const source = alias.get(insn.operands[1]);
      if (source) alias.set(destination, source);
      else alias.delete(destination);
    } else if ((insn.mnemonic === "add" || insn.mnemonic === "sub") && insn.operands.length === 3 && insn.operands[2] === "#0" && /^r\d+$/.test(insn.operands[1])) {
      const source = alias.get(insn.operands[1]);
      if (source) alias.set(destination, source);
      else alias.delete(destination);
    } else if (!(CONDITION.has(insn.raw) || insn.mnemonic === "b" || insn.mnemonic === "bx" || insn.mnemonic === "cmp" || insn.mnemonic === "push" || insn.mnemonic === "pop")) {
      alias.delete(destination);
    }
  }
  return snapshots;
}

// 決定木を構造写像する。cmpの直後に条件分岐を持つブロックをif/elseへ、
// 葉を return 定数へ。共有ブロックは複製する（葉は定数なので意味不変）。
function mirrorCandidate(program: Program, argumentCount: number): Candidate | null {
  try {
    const { blocks } = buildBlocks(program);
    const aliases = aliasMap(program, argumentCount);
    const indexOf = new Map<string, number>();
    program.insns.forEach((insn, index) => {
      if (insn.label !== null) indexOf.set(insn.label, index);
    });

    const emit = (key: string, depth: number): string => {
      if (depth > 256) throw new Unsupported("再帰深すぎ（ループの疑い）");
      const leaf = resolveLeaf(program, key, blocks);
      if (leaf !== null) return `return ${leaf};`;
      const block = blocks.get(key);
      if (block === undefined) throw new Unsupported(`ブロック不明: ${key}`);
      // 計算部にr0定数代入以外があれば構造写像は諦める。
      for (const insn of block.insns) {
        if (insn.mnemonic === "push") continue;
        if (insn.mnemonic === "mov" && insn.operands[0] === "r0" && insn.operands[1]?.startsWith("#")) continue;
        if (insn.mnemonic === "cmp") continue;
        throw new Unsupported(`写像不能な命令: ${insn.mnemonic}`);
      }
      const terminator = block.terminator;
      if (!CONDITION.has(terminator.raw)) {
        if (terminator.mnemonic === "b") return emit(terminator.operands[0], depth + 1);
        if (terminator.mnemonic === "fall" && block.fallthrough) return emit(block.fallthrough, depth + 1);
        throw new Unsupported(`端末写像不能: ${terminator.mnemonic}`);
      }
      // 端末は条件分岐。直前のcmpを探して条件を作る。
      const compare = [...block.insns].reverse().find((insn) => insn.mnemonic === "cmp");
      if (compare === undefined) throw new Unsupported("cmp無し条件分岐");
      const terminatorIndex = program.insns.indexOf(terminator);
      const alias = aliases[terminatorIndex] ?? new Map<string, string>();
      const left = alias.get(compare.operands[0]);
      if (left === undefined) throw new Unsupported(`cmp左辺が引数別名でない: ${compare.operands[0]}`);
      const rightToken = compare.operands[1];
      const right = rightToken.startsWith("#") ? String(parseImmediate(rightToken)) : alias.get(rightToken);
      if (right === undefined) throw new Unsupported(`cmp右辺解釈不能: ${rightToken}`);
      const { operator, unsigned } = conditionOperator(terminator.raw);
      const leftExpr = unsigned ? `(u32)${left}` : left;
      const condition = `${leftExpr} ${operator} ${right}`;
      const taken = emit(terminator.operands[0], depth + 1);
      const fall = block.fallthrough === null ? "" : emit(block.fallthrough, depth + 1);
      return `if (${condition}) { ${taken} } else { ${fall} }`;
    };

    const body = emit(program.insns[0].label ?? "@0", 0);
    return { label: "mirror", body: `  ${body}` };
  } catch (error) {
    if (error instanceof Unsupported) return null;
    throw error;
  }
}

// ---- 採点（synthesize_blockのassembleを再現）--------------------------

function runSync(command: string[]): number {
  const result = Bun.spawnSync(command, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  return result.exitCode ?? 1;
}

// 承認コンパイラでC→アセンブル→リンク→.text抽出。未定義外部は束で解決。
function assemble(stem: string, source: string): Buffer | null {
  mkdirSync(SCRATCH, { recursive: true });
  const prefix = join(SCRATCH, stem);
  const cFile = `${prefix}.c`, sFile = `${prefix}.s`, oFile = `${prefix}.o`;
  const elf = `${prefix}.elf`, bin = `${prefix}.bin`;
  writeFileSync(cFile, source);
  if (runSync(compilerCommand(...CFLAGS, "-S", "-o", sFile, cFile)) !== 0) return null;
  if (runSync(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", oFile, sFile]) !== 0) return null;
  const address = `0x${stem}`;
  const undefinedNames: string[] = [];
  const nm = Bun.spawnSync(["arm-none-eabi-nm", "-u", oFile], { cwd: ROOT, stdout: "pipe" });
  for (const line of Buffer.from(nm.stdout).toString().split(/\r?\n/).filter(Boolean)) {
    const name = line.trim().split(/\s+/).at(-1)!;
    if (externalSymbol(name) === null) return null;
    undefinedNames.push(name);
  }
  const symbolArgs: string[] = [];
  if (undefinedNames.length > 0) {
    const symbolsSource = `${prefix}.symbols.s`, symbolsObject = `${prefix}.symbols.o`;
    writeFileSync(symbolsSource, ".syntax unified\n.thumb\n" + undefinedNames.map(externalSymbolAssembly).join(""));
    if (runSync(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", symbolsObject, symbolsSource]) !== 0) return null;
    symbolArgs.push(symbolsObject);
  }
  if (runSync(["arm-none-eabi-ld", `-Ttext=${address}`, "-e", `Func_${stem}`, "-o", elf, oFile, ...symbolArgs]) !== 0) return null;
  if (runSync(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", elf, bin]) !== 0) return null;
  return readFileSync(bin);
}

// 参照asmを組んで.textバイト長（期待長）を得る。語プールも含む実長。
function expectedLength(stem: string): number | null {
  mkdirSync(SCRATCH, { recursive: true });
  const object = join(SCRATCH, `${stem}.ref.o`);
  const binary = join(SCRATCH, `${stem}.ref.bin`);
  if (runSync(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", object, join(ROOT, "asm", `${stem}.s`)]) !== 0) return null;
  if (runSync(["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", object, binary]) !== 0) return null;
  return readFileSync(binary).length;
}

// ---- 包み ------------------------------------------------------------

function wrapFunction(stem: string, argumentCount: number, argumentType: string, returnType: string, body: string): string {
  const parameters = argumentCount === 0
    ? "void"
    : Array.from({ length: argumentCount }, (_, index) => `${argumentType} arg${index}`).join(", ");
  return `${M2C_PREAMBLE}${returnType} Func_${stem}(${parameters}) {\n${body}\n}\n`;
}

// ---- 対象選定 ---------------------------------------------------------

// リンカベニア（ldr r4,[pc]; bx r4; .4byte）か。
function isVeneer(insns: Insn[]): boolean {
  return insns.length === 2
    && insns[0].mnemonic === "ldr" && insns[0].operands[0] === "r4"
    && insns[1].mnemonic === "bx" && insns[1].operands[0] === "r4";
}

function structuralStems(): Set<string> {
  const classification = JSON.parse(readFileSync(join(ROOT, "asm", "classification.json"), "utf8"));
  const stems = new Set<string>();
  for (const group of classification.groups ?? []) {
    for (const file of group.files ?? []) if (typeof file === "string") stems.add(file.toLowerCase());
  }
  return stems;
}

function selectTargets(): string[] {
  const matches = JSON.parse(readFileSync(join(ROOT, "work", "matches", "m2c.json"), "utf8")) as Array<{ entry: number; matched?: boolean }>;
  const structural = structuralStems();
  const targets: string[] = [];
  for (const entry of matches) {
    if (entry.matched) continue;
    const stem = (entry.entry >>> 0).toString(16).padStart(8, "0");
    if (structural.has(stem)) continue;
    if (!existsSync(join(ROOT, "asm", `${stem}.s`))) continue;
    let insns: Insn[];
    try {
      insns = parseAsm(stem);
    } catch {
      continue;
    }
    if (insns.length === 0) continue;
    if (isVeneer(insns)) continue;
    if (insns.some((insn) => insn.mnemonic === "bl" || insn.mnemonic === "blx")) continue;
    if (insns.length > 40) continue;
    targets.push(stem);
  }
  return targets.sort();
}

// ---- 一対象の合成 -----------------------------------------------------

interface Outcome {
  stem: string;
  status: "installed" | "exact-guard-failed" | "mismatch" | "unsupported" | "compile-failed";
  reason?: string;
  bestLabel?: string;
  bestMismatch?: number | "length";
  bestLength?: number; // 最良候補の組立バイト数（length不一致の距離報告用）
  expected?: number;
  candidates?: number;
}

function synthesizeOne(stem: string, rom: Buffer, install: boolean, verbose: boolean): Outcome {
  let insns: Insn[];
  try {
    insns = parseAsm(stem);
  } catch (error) {
    return { stem, status: "unsupported", reason: `解析失敗: ${(error as Error).message}` };
  }
  const program = buildProgram(insns);
  const argumentCount = inputArgumentCount(insns);

  // 未対応（記憶参照・多引数・ループ）はここで弾く。特性化が例外を投げる。
  let intervals: Interval[];
  try {
    intervals = characterize(program, argumentCount);
  } catch (error) {
    return { stem, status: "unsupported", reason: (error as Error).message };
  }

  // 符号: 符号無し条件分岐があれば引数をu32とみなす。
  const unsigned = insns.some((insn) => ["bhi", "bls", "bcs", "bcc", "bhs", "blo"].includes(insn.raw));
  const argumentType = unsigned ? "u32" : "s32";
  const returnType = "s32";

  const bodies = new Map<string, string>();
  const record = (candidate: Candidate) => bodies.set(candidate.body, candidate.label);
  for (const candidate of candidatesFromIntervals("arg0", intervals)) record(candidate);
  const mirror = mirrorCandidate(program, argumentCount);
  if (mirror) record(mirror);
  if (bodies.size === 0) return { stem, status: "unsupported", reason: "候補生成なし" };

  const length = expectedLength(stem);
  if (length === null) return { stem, status: "unsupported", reason: "参照asm組立失敗" };
  const address = Number.parseInt(stem, 16) - ROM_BASE;
  const expected = Buffer.from(rom.subarray(address, address + length));

  let best: { label: string; score: number; mismatch: number | "length"; source: string; bytes: Buffer } | null = null;
  let compiled = 0;
  for (const [body, label] of bodies) {
    const source = wrapFunction(stem, argumentCount, argumentType, returnType, body);
    const bytes = assemble(stem, source);
    if (bytes === null) continue;
    compiled++;
    const sameLength = bytes.length === expected.length;
    const score = sameLength ? weightedDiff(bytes, expected) : Number.MAX_SAFE_INTEGER;
    const mismatch: number | "length" = sameLength
      ? Array.from(expected).filter((byte, index) => byte !== bytes[index]).length
      : "length";
    if (process.env.EXPR_DEBUG) console.log(`      [dbg] ${label}: len=${bytes.length} mismatch=${mismatch}`);
    if (best === null || score < best.score) best = { label, score, mismatch, source, bytes };
  }
  if (best === null) return { stem, status: "compile-failed", candidates: bodies.size, expected: length };

  if (verbose) {
    console.log(`  ${stem}: args=${argumentCount}(${argumentType}) intervals=${intervals.map((i) => `[${i.lo === NEG_INF ? "-∞" : i.lo},${i.hi === POS_INF ? "+∞" : i.hi}]=${i.value}`).join(" ")}`);
    console.log(`    candidates=${bodies.size} compiled=${compiled} best=${best.label} mismatch=${best.mismatch}/${expected.length} score=${best.score === Number.MAX_SAFE_INTEGER ? "len" : best.score}`);
  }

  const exact = best.bytes.length === expected.length && best.bytes.equals(expected);
  if (!exact) {
    return { stem, status: "mismatch", bestLabel: best.label, bestMismatch: best.mismatch, bestLength: best.bytes.length, expected: length, candidates: bodies.size };
  }

  if (!install) {
    return { stem, status: "mismatch", reason: "完全一致（--installで設置）", bestLabel: best.label, bestMismatch: 0, expected: length, candidates: bodies.size };
  }
  // 完全一致かつ設置指定: 領域長検査＋全検証を経てsrc/へ。
  const candidatePath = join(SCRATCH, `${stem}.c`);
  writeFileSync(candidatePath, best.source);
  const [okay, size] = verify(candidatePath, rom, SCRATCH);
  if (okay && replaceableAssembly(stem, size, SCRATCH)) {
    writeFileSync(join(ROOT, "src", `${stem}.c`), best.source);
    rmSync(join(ROOT, "asm", `${stem}.s`), { force: true });
    return { stem, status: "installed", bestLabel: best.label, expected: size };
  }
  return { stem, status: "exact-guard-failed", reason: `okay=${okay} size=${size}`, bestLabel: best.label, expected: length };
}

// ---- 入口 ------------------------------------------------------------

function main(): void {
  const argv = Bun.argv.slice(2);
  const install = argv.includes("--install");
  const verbose = argv.includes("--verbose") || argv.includes("-v");
  const only = argv.find((argument) => /^[0-9a-f]{8}$/.test(argument));

  const rom = readFileSync(join(ROOT, "gs1-en.gba"));
  const targets = only ? [only] : selectTargets();
  console.log(`targets=${targets.length}${only ? ` (single ${only})` : ""} install=${install}`);

  const outcomes: Outcome[] = [];
  for (const stem of targets) {
    try {
      outcomes.push(synthesizeOne(stem, rom, install, verbose || Boolean(only)));
    } catch (error) {
      outcomes.push({ stem, status: "unsupported", reason: `例外: ${(error as Error).message}` });
    }
  }

  // 集計。
  const installed = outcomes.filter((outcome) => outcome.status === "installed");
  const exact = outcomes.filter((outcome) => outcome.status === "mismatch" && outcome.bestMismatch === 0);
  const mismatch = outcomes.filter((outcome) => outcome.status === "mismatch" && outcome.bestMismatch !== 0);
  const guardFailed = outcomes.filter((outcome) => outcome.status === "exact-guard-failed");
  const unsupported = outcomes.filter((outcome) => outcome.status === "unsupported");
  const compileFailed = outcomes.filter((outcome) => outcome.status === "compile-failed");

  console.log("");
  console.log("==== 集計 ====");
  console.log(`attempted (candidates generated): ${outcomes.length - unsupported.length - compileFailed.length}`);
  console.log(`installed: ${installed.length}`);
  for (const outcome of installed) console.log(`  ${outcome.stem} via ${outcome.bestLabel} (${outcome.expected}B)`);
  if (exact.length > 0) {
    console.log(`exact (not installed): ${exact.length}`);
    for (const outcome of exact) console.log(`  ${outcome.stem} via ${outcome.bestLabel}`);
  }
  if (guardFailed.length > 0) {
    console.log(`exact-guard-failed: ${guardFailed.length}`);
    for (const outcome of guardFailed) console.log(`  ${outcome.stem} ${outcome.reason}`);
  }
  console.log(`unsupported (approach does not apply): ${unsupported.length}`);
  console.log(`compile-failed: ${compileFailed.length}`);

  // 最良不一致の分布（length別・バイト差別）。
  const lengthMismatch = mismatch.filter((outcome) => outcome.bestMismatch === "length").length;
  const byteMismatch = mismatch.filter((outcome) => typeof outcome.bestMismatch === "number") as Array<Outcome & { bestMismatch: number }>;
  const buckets = new Map<string, number>();
  const bucketOf = (value: number): string => (value <= 2 ? "1-2" : value <= 4 ? "3-4" : value <= 8 ? "5-8" : value <= 16 ? "9-16" : "17+");
  for (const outcome of byteMismatch) {
    const key = bucketOf(outcome.bestMismatch);
    buckets.set(key, (buckets.get(key) ?? 0) + 1);
  }
  console.log("");
  console.log("==== 最良不一致分布（本文を組めた対象）====");
  console.log(`length-mismatch (候補長≠期待長): ${lengthMismatch}`);
  const lengthOnly = mismatch.filter((outcome) => outcome.bestMismatch === "length" && outcome.bestLength !== undefined && outcome.expected !== undefined) as Array<Outcome & { bestLength: number; expected: number }>;
  const closestByLength = [...lengthOnly].sort((a, b) => Math.abs(a.bestLength - a.expected) - Math.abs(b.bestLength - b.expected)).slice(0, 8);
  if (closestByLength.length > 0) {
    console.log("closest length-mismatch near-misses (|Δbytes|):");
    for (const outcome of closestByLength) console.log(`  ${outcome.stem} got ${outcome.bestLength}B vs ${outcome.expected}B (Δ${outcome.bestLength - outcome.expected}) via ${outcome.bestLabel}`);
  }
  console.log(`same-length byte mismatch buckets:`);
  for (const key of ["1-2", "3-4", "5-8", "9-16", "17+"]) {
    if (buckets.has(key)) console.log(`  ${key} bytes off: ${buckets.get(key)}`);
  }
  // 最も惜しい上位を列挙。
  const closest = [...byteMismatch].sort((a, b) => a.bestMismatch - b.bestMismatch).slice(0, 12);
  console.log("closest same-length near-misses:");
  for (const outcome of closest) console.log(`  ${outcome.stem} mismatch=${outcome.bestMismatch}/${outcome.expected} via ${outcome.bestLabel}`);

  // 非対応理由の内訳。
  const reasons = new Map<string, number>();
  for (const outcome of unsupported) {
    const key = (outcome.reason ?? "?").replace(/[0-9a-fx#]+/g, "N").slice(0, 48);
    reasons.set(key, (reasons.get(key) ?? 0) + 1);
  }
  console.log("");
  console.log("==== 非対応理由 ====");
  for (const [reason, count] of [...reasons.entries()].sort((a, b) => b[1] - a[1])) console.log(`  ${count}× ${reason}`);
}

if (import.meta.main) main();
