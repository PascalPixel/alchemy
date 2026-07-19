#!/usr/bin/env bun
// ブロック採掘。一致済みCの各関数を承認コンパイラで-S出力し、命令列を
// 錨（呼出し先・フィールド変位・大定数プール）で文へ対応付ける。錨を
// 終端とする命令区間を各文へ帰属させ、雛形と抽象化命令列の対を辞書
// として書き出す。狙いは容易な6〜8割の被覆で、曖昧な関数は捨てて雑音
// を出さない。ROMバイトは触れず、出力はout/配下のみ。
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { CFLAGS, compilerCommand } from "./alchemy_gcc.ts";
import { parseInstructions } from "./cfg_extract.ts";
import { anchorsOf, templateOf } from "./mine_idioms.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface Flat {
  mnemonic: string;
  operands: string;
}

// コンパイル出力.sを平坦な命令列と、プールラベル→語の対応へ写す。
function compileFlat(stem: string): { insns: Flat[]; pool: Map<string, number> } {
  const scratch = process.env.CLAUDE_SCRATCH ?? join(ROOT, "out", "rebuild", "tmp");
  mkdirSync(scratch, { recursive: true });
  const asmOut = join(scratch, `${stem}.mine.s`);
  const r = Bun.spawnSync(compilerCommand(...CFLAGS, "-S", "-o", asmOut, join(ROOT, "src", `${stem}.c`)), {
    cwd: ROOT,
    stdout: "pipe",
    stderr: "pipe",
  });
  if (r.exitCode !== 0) throw new Error(`compile ${stem} failed: ${Buffer.from(r.stderr).toString().trim()}`);
  const text = readFileSync(asmOut, "utf8");
  const pool = new Map<string, number>();
  // .Ln: .word N 形式のプールを収集する（10進・16進とも）。
  const lines = text.split(/\r?\n/);
  for (let i = 0; i < lines.length; i += 1) {
    const lm = lines[i].trim().match(/^([.$A-Za-z_][\w.$]*)\s*:$/);
    if (lm === null) continue;
    for (let j = i + 1; j < lines.length; j += 1) {
      const t = lines[j].trim();
      if (t === "" || t.startsWith(".align")) continue;
      const wm = t.match(/^\.(word|4byte|long)\s+(0x[0-9a-fA-F]+|\d+)/);
      if (wm !== null) pool.set(lm[1], Number(wm[2]) >>> 0);
      break;
    }
  }
  const insns = parseInstructions(text).map((x) => ({ mnemonic: x.mnemonic, operands: x.operands }));
  return { insns, pool };
}

// Cソースから文（;終端）を出現順に取り出す。mine_idiomsの選別に合わせる。
function statementsOf(src: string): string[] {
  const out: string[] = [];
  for (const raw of src.split("\n")) {
    const s = raw.trim();
    if (!s.endsWith(";") || s.startsWith("typedef") || s.startsWith("#") || s.startsWith("extern")) continue;
    if (/^\w[\w* ]*Func_08[0-9a-f]{6}\(/.test(s)) continue;
    out.push(s);
  }
  return out;
}

// 錨の種別を判定する。呼出し・フィールド・絶対番地。
type Anchor =
  | { kind: "call"; symbol: string }
  | { kind: "field"; width: "b" | "h" | "w" | "any"; offset: number }
  | { kind: "abs"; value: number };

// 文字列の錨表現（mine_idiomsの形）を突合用の構造へ写す。
function parseAnchor(raw: string): Anchor | null {
  if (raw.startsWith("call:")) return { kind: "call", symbol: raw.slice(5) };
  if (raw.startsWith("field:")) {
    const m = raw.match(/^field:(\w+):([0-9a-f]+)$/);
    if (m === null) return null;
    return { kind: "field", width: widthOf(m[1]), offset: Number.parseInt(m[2], 16) };
  }
  if (raw.startsWith("abs:")) {
    const m = raw.match(/^abs:\w+:(0x[0-9a-f]+)$/);
    if (m === null) return null;
    return { kind: "abs", value: Number(m[1]) >>> 0 };
  }
  return null;
}

// C型からロード幅を推定する。ポインタや語は w、短は h、バイトは b。
function widthOf(type: string): "b" | "h" | "w" | "any" {
  if (/\*/.test(type)) return "w";
  if (/^(s8|u8|char)$/.test(type)) return "b";
  if (/^(s16|u16|short)$/.test(type)) return "h";
  if (/^(s32|u32|int|long|bool)$/.test(type)) return "w";
  return "any";
}

// 命令の即値を取り出す（[rX, #N] や #N、無ければ0＝変位無し接近）。
function immediateOf(operands: string): number | null {
  const m = operands.match(/#(-?\d+|0x[0-9a-fA-F]+)/);
  if (m !== null) return Number(m[1]) | 0;
  // [rX] は変位0のフィールド接近。
  if (/^\[[^,\]]+\]$/.test(operands.trim())) return 0;
  return null;
}

const LOAD_STORE = /^(ldr|str)(b|h|sb|sh)?$/;

// この命令が或る錨に合致するか。
function matches(insn: Flat, anchor: Anchor, pool: Map<string, number>): boolean {
  const m = insn.mnemonic.toLowerCase();
  if (anchor.kind === "call") {
    return (m === "bl" || m === "blx") && insn.operands.includes(anchor.symbol);
  }
  if (anchor.kind === "field") {
    const ls = m.match(LOAD_STORE);
    if (ls === null) return false;
    const imm = immediateOf(insn.operands);
    if (imm === null || imm !== anchor.offset) return false;
    if (anchor.width === "any") return true;
    const suffix = ls[2] ?? "";
    const width = suffix.includes("b") ? "b" : suffix.includes("h") ? "h" : "w";
    return width === anchor.width;
  }
  // abs: ldr rX, .Ln でプール語が絶対番地に一致。
  const lm = insn.operands.match(/,\s*([.$A-Za-z_][\w.$]*)\s*$/);
  if (m === "ldr" && lm !== null && pool.has(lm[1])) return (pool.get(lm[1])! >>> 0) === anchor.value;
  return false;
}

// レジスタを出現順の rA,rB... へ抽象化し、即値は残す。
function abstract(insns: Flat[]): string[] {
  const map = new Map<string, string>();
  const alpha = (n: number) => `r${String.fromCharCode(65 + (n % 26))}${n >= 26 ? Math.floor(n / 26) : ""}`;
  const rename = (text: string) =>
    text.replace(/\b(r(?:1[0-2]|[0-9])|ip|fp|sl)\b/g, (reg) => {
      if (!map.has(reg)) map.set(reg, alpha(map.size));
      return map.get(reg)!;
    });
  return insns.map((i) => `${i.mnemonic} ${rename(i.operands)}`.trim());
}

interface Aligned {
  template: string;
  normalized_instructions: string[];
  stem: string;
  anchor: string;
}

// 一関数を整列する。各文の主錨を命令列で前方走査して同定し、錨を終端と
// する区間を前の錨の直後まで遡って帰属させる。曖昧なら関数ごと捨てる。
function alignFunction(
  stem: string,
): { records: Aligned[]; attributed: number; total: number; skipped: boolean } {
  const src = readFileSync(join(ROOT, "src", `${stem}.c`), "utf8");
  const { insns, pool } = compileFlat(stem);
  const statements = statementsOf(src);
  const total = insns.length;

  // 錨を持つ文だけを対象に、(文, 錨) の並びを作る。
  const wanted: Array<{ stmt: string; anchorRaw: string; anchor: Anchor }> = [];
  for (const stmt of statements) {
    for (const raw of anchorsOf(stmt)) {
      const a = parseAnchor(raw);
      if (a !== null) wanted.push({ stmt, anchorRaw: raw, anchor: a });
    }
  }
  if (wanted.length === 0) return { records: [], attributed: 0, total, skipped: true };

  // 前方走査で各錨の命令位置を確定する。順序が崩れたら曖昧として捨てる。
  const hits: Array<{ index: number; item: (typeof wanted)[number] }> = [];
  let cursor = 0;
  let missed = 0;
  for (const item of wanted) {
    let found = -1;
    for (let i = cursor; i < insns.length; i += 1) {
      if (matches(insns[i], item.anchor, pool)) {
        found = i;
        break;
      }
    }
    if (found === -1) {
      missed += 1;
      continue;
    }
    hits.push({ index: found, item });
    cursor = found + 1;
  }
  // 半分以上の錨を取り逃せば信頼できないので関数ごと捨てる。
  if (hits.length === 0 || missed > wanted.length / 2) {
    return { records: [], attributed: 0, total, skipped: true };
  }

  // 各錨命令を終端に、前の錨の直後から遡って区間を切り、文へ帰属させる。
  const records: Aligned[] = [];
  let prev = 0;
  let attributed = 0;
  for (const hit of hits) {
    const span = insns.slice(prev, hit.index + 1);
    attributed += span.length;
    records.push({
      template: templateOf(hit.item.stmt).template,
      normalized_instructions: abstract(span),
      stem,
      anchor: hit.item.anchorRaw,
    });
    prev = hit.index + 1;
  }
  return { records, attributed, total, skipped: false };
}

const SELF_TEST_C = `typedef unsigned char u8;
typedef unsigned int u32;
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
void Func_08000200(u8 *arg0);
u32 Func_08000000(u8 *arg0) {
    u32 var_r0;
    var_r0 = M2C_FIELD(arg0, u8 *, 0x10);
    Func_08000200(arg0);
    return var_r0;
}
`;

const SELF_TEST_S = `	.code 16
	.thumb_func
Func_08000000:
	push	{r4, lr}
	mov	r4, r0
	ldrb	r0, [r4, #16]
	mov	r1, r4
	bl	Func_08000200
	pop	{r4}
	pop	{r1}
	bx	r1
`;

function selfTest(): void {
  const stmts = statementsOf(SELF_TEST_C);
  const insns = parseInstructions(SELF_TEST_S).map((x) => ({ mnemonic: x.mnemonic, operands: x.operands }));
  const pool = new Map<string, number>();
  const problems: string[] = [];
  // フィールド接近 0x10 は ldrb ...,#16 に合致すべき。
  const fieldStmt = stmts.find((s) => s.includes("0x10"))!;
  const fieldAnchor = parseAnchor(anchorsOf(fieldStmt)[0])!;
  const fieldIdx = insns.findIndex((i) => matches(i, fieldAnchor, pool));
  if (fieldIdx !== 2) problems.push(`field anchor idx ${fieldIdx} != 2`);
  // 呼出し文は bl Func_08000200 に合致すべき。
  const callStmt = stmts.find((s) => s.includes("Func_08000200"))!;
  const callAnchor = parseAnchor(anchorsOf(callStmt).find((a) => a.startsWith("call:"))!)!;
  const callIdx = insns.findIndex((i) => matches(i, callAnchor, pool));
  if (callIdx !== 4) problems.push(`call anchor idx ${callIdx} != 4`);
  // 抽象化でr4がrB等の位置記号へ、即値#16は保持されること。
  const abs = abstract([insns[2]]);
  if (!/#16/.test(abs[0]) || /r4/.test(abs[0])) problems.push(`abstract wrong: ${abs[0]}`);
  if (problems.length !== 0) {
    console.error("self-test FAILED:");
    for (const p of problems) console.error(`  - ${p}`);
    process.exitCode = 1;
    return;
  }
  console.log(`self-test ok: field@${fieldIdx} call@${callIdx} abstract="${abs[0]}"`);
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: mine_blocks.ts [--self-test]");
    return;
  }
  if (args.includes("--self-test")) {
    selfTest();
    return;
  }
  const outDir = join(ROOT, "out", "rebuild");
  mkdirSync(outDir, { recursive: true });
  const outPath = join(outDir, "idiom_blocks.jsonl");
  const stems = readdirSync(join(ROOT, "src"))
    .filter((n) => /^[0-9a-f]{8}\.c$/.test(n))
    .map((n) => basename(n, ".c"))
    .sort();
  const lines: string[] = [];
  const dictionary = new Set<string>();
  let attributed = 0;
  let totalInsn = 0;
  let emitted = 0;
  let skipped = 0;
  let considered = 0;
  const errors: Record<string, number> = {};
  for (const stem of stems) {
    let result;
    try {
      result = alignFunction(stem);
    } catch (error) {
      const key = String((error as Error).message).slice(0, 40);
      errors[key] = (errors[key] ?? 0) + 1;
      continue;
    }
    if (result.total === 0) continue;
    considered += 1;
    if (result.skipped) {
      skipped += 1;
      continue;
    }
    attributed += result.attributed;
    totalInsn += result.total;
    for (const record of result.records) {
      lines.push(JSON.stringify(record));
      dictionary.add(`${record.template} ${record.normalized_instructions.join(";")}`);
      emitted += 1;
    }
  }
  writeFileSync(outPath, lines.join("\n") + (lines.length ? "\n" : ""));
  const coverage = totalInsn === 0 ? 0 : (100 * attributed) / totalInsn;
  console.log(
    `functions=${stems.length} considered=${considered} aligned=${considered - skipped} skipped=${skipped}`,
  );
  console.log(`records=${emitted} dictionary=${dictionary.size} coverage=${coverage.toFixed(1)}% -> ${outPath}`);
  const errKeys = Object.keys(errors);
  if (errKeys.length > 0) {
    console.log("errors:");
    for (const k of errKeys.sort((a, b) => errors[b] - errors[a])) console.log(`  ${errors[k]}x ${k}`);
  }
}

if (import.meta.main) main();
