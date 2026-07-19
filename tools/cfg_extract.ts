#!/usr/bin/env bun
// 制御流抽出。逆アセンブル済みThumb命令列を基本ブロックへ分割する。
// 先頭・分岐先・分岐直後を指導子とし、各ブロックの後続と終端種別
// （分岐・復帰・呼出し・素通り）を求める。asm/<stem>.s を第一の入力
// とし、一致済みで.sが無い関数のみROM該当区間を局所逆アセンブルする。
// ROMのバイトは追跡対象へ複写せず、出力はout/配下のみへ書き出す。
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { CFLAGS, compilerCommand } from "./alchemy_gcc.ts";

export const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
export const ROM_BASE = 0x08000000;

// 一命令の表現。ニーモニックと素の被演算子文字列を保持する。
export interface Insn {
  mnemonic: string;
  operands: string;
  text: string;
  labels: string[]; // この命令の直前に付いた行内ラベル
}

export type EndKind = "branch" | "return" | "call" | "fallthrough";

export interface Block {
  id: number;
  start: number; // 関数内での命令序数（先頭命令の位置）
  label: string | null; // ブロック先頭のラベル（あれば）
  instructions: Array<{ mnemonic: string; operands: string }>;
  successors: number[]; // 後続ブロックのid
  end: EndKind;
}

const CONDITION = "eq|ne|cs|cc|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al|hs|lo";
// 条件付き・無条件の相対分岐。blやbx、bic系は含めない。
const BRANCH_RE = new RegExp(`^b(${CONDITION})?(\\.n|\\.w)?$`);
const UNCONDITIONAL = new Set(["b", "b.n", "b.w", "bal", "bal.n", "bal.w"]);

// ニーモニックの分類。呼出しbl/blx、復帰bx、pop{...pc}を区別する。
function classify(insn: Insn): { branch: boolean; call: boolean; ret: boolean; conditional: boolean } {
  const m = insn.mnemonic.toLowerCase();
  if (m === "bl" || m === "bl.w" || m === "blx") return { branch: false, call: true, ret: false, conditional: false };
  if (m === "bx" || m === "bx.n") return { branch: false, call: false, ret: true, conditional: false };
  if (m === "pop" && /\bpc\b/.test(insn.operands)) return { branch: false, call: false, ret: true, conditional: false };
  if ((m === "mov" || m === "movs") && /^pc\b/.test(insn.operands.trim())) {
    return { branch: false, call: false, ret: true, conditional: false };
  }
  if (BRANCH_RE.test(m)) {
    const conditional = !UNCONDITIONAL.has(m);
    return { branch: true, call: false, ret: false, conditional };
  }
  return { branch: false, call: false, ret: false, conditional: false };
}

// 分岐命令の飛び先ラベルを取り出す。被演算子末尾のラベル字句を採る。
function branchTarget(insn: Insn): string | null {
  const tokens = insn.operands.split(/[,\s]+/).filter(Boolean);
  for (let i = tokens.length - 1; i >= 0; i -= 1) {
    const token = tokens[i];
    if (/^[.$A-Za-z_][\w.$]*$/.test(token) && !/^(r\d+|sp|lr|pc|ip|fp|sl)$/i.test(token)) return token;
  }
  return null;
}

// データ疑似命令か。定数プールや整列はブロックへ入れない。
function isData(mnemonic: string): boolean {
  return /^\.(4byte|2byte|byte|word|hword|short|long|ascii|asciz|space|zero|fill)$/.test(mnemonic);
}

// 命令行を分解する。先頭字句がニーモニック、残余が被演算子。
function splitInsn(line: string): { mnemonic: string; operands: string } {
  const trimmed = line.trim();
  const m = trimmed.match(/^(\S+)\s*([\s\S]*)$/);
  if (m === null) return { mnemonic: trimmed, operands: "" };
  return { mnemonic: m[1], operands: m[2].replace(/\s+/g, " ").trim() };
}

// .sテキスト行から関数本体の命令列を取り出す。ラベルは次命令へ束ねる。
export function parseInstructions(source: string): Insn[] {
  const insns: Insn[] = [];
  let pending: string[] = [];
  let started = false;
  for (const raw of source.split(/\r?\n/)) {
    // 行内コメントを除去。@ 以降と /* */ は捨てる。
    let line = raw.replace(/@.*$/, "").replace(/\/\*.*?\*\//g, "");
    line = line.replace(/;.*$/, "");
    const trimmed = line.trim();
    if (trimmed === "") continue;
    // 行頭ラベル（label: 形式）を切り出す。複数連続も許す。
    let rest = trimmed;
    let matched = true;
    while (matched) {
      matched = false;
      const lm = rest.match(/^([.$A-Za-z_][\w.$]*|\d+)\s*:\s*(.*)$/);
      if (lm !== null) {
        pending.push(lm[1]);
        rest = lm[2];
        matched = true;
        if (/^Func_[0-9a-f]{8}$/.test(lm[1])) started = true;
      }
    }
    if (rest === "") continue;
    const { mnemonic, operands } = splitInsn(rest);
    // 指示子は読み飛ばす。ただしデータ疑似命令はプールとして本体後に置かれる。
    if (mnemonic.startsWith(".")) {
      if (isData(mnemonic)) {
        // プールデータに達したら以降は命令ではない。保留ラベルは破棄。
        pending = [];
        continue;
      }
      if (/^\.(macro|endm)$/.test(mnemonic)) {
        pending = [];
        continue;
      }
      // その他の指示子（.thumb等）は無視。
      pending = [];
      continue;
    }
    if (!started && insns.length === 0) started = true;
    insns.push({ mnemonic, operands, text: rest, labels: pending });
    pending = [];
  }
  return insns;
}

// 命令列から基本ブロックを構築する。
export function buildBlocks(insns: Insn[]): Block[] {
  const n = insns.length;
  if (n === 0) return [];
  const info = insns.map(classify);
  // 指導子集合を求める。先頭・分岐先ラベル・終端直後。
  const labelIndex = new Map<string, number>();
  for (let i = 0; i < n; i += 1) for (const l of insns[i].labels) labelIndex.set(l, i);
  const leaders = new Set<number>([0]);
  for (let i = 0; i < n; i += 1) {
    const c = info[i];
    if (c.branch) {
      const target = branchTarget(insns[i]);
      if (target !== null && labelIndex.has(target)) leaders.add(labelIndex.get(target)!);
    }
    if (c.branch || c.ret) {
      if (i + 1 < n) leaders.add(i + 1);
    }
  }
  // ラベルが付く命令も指導子扱い（分岐先や合流点）。
  for (let i = 0; i < n; i += 1) if (insns[i].labels.length > 0) leaders.add(i);

  const starts = [...leaders].sort((a, b) => a - b);
  const blockOf = new Map<number, number>(); // 命令index -> block id
  starts.forEach((s, id) => blockOf.set(s, id));
  const blocks: Block[] = [];
  for (let id = 0; id < starts.length; id += 1) {
    const start = starts[id];
    const end = id + 1 < starts.length ? starts[id + 1] : n;
    const body = insns.slice(start, end);
    const last = body[body.length - 1];
    const lastInfo = classify(last);
    let kind: EndKind;
    const successors: number[] = [];
    if (lastInfo.ret) {
      kind = "return";
    } else if (lastInfo.branch) {
      kind = "branch";
      const target = branchTarget(last);
      if (target !== null && labelIndex.has(target)) successors.push(blockOf.get(labelIndex.get(target)!)!);
      if (lastInfo.conditional && id + 1 < starts.length) successors.push(id + 1);
    } else if (lastInfo.call) {
      kind = "call";
      if (id + 1 < starts.length) successors.push(id + 1);
    } else {
      kind = "fallthrough";
      if (id + 1 < starts.length) successors.push(id + 1);
    }
    blocks.push({
      id,
      start,
      label: body[0].labels[0] ?? null,
      instructions: body.map((b) => ({ mnemonic: b.mnemonic, operands: b.operands })),
      successors: [...new Set(successors)],
      end: kind,
    });
  }
  return blocks;
}

// stemからCFGを得る。asm/<stem>.s があればそれを、無ければROMを逆アセンブル。
export function cfgOf(stem: string): { stem: string; blocks: Block[]; source: "asm" | "rom" } {
  const asmPath = join(ROOT, "asm", `${stem}.s`);
  if (existsSync(asmPath)) {
    const insns = parseInstructions(readFileSync(asmPath, "utf8"));
    return { stem, blocks: buildBlocks(insns), source: "asm" };
  }
  return { stem, blocks: buildBlocks(disassembleFromRom(stem)), source: "rom" };
}

// 一致済みで.sが無い関数のみ。src/<stem>.c を検証経路で組み、寸法を得て
// ROM該当区間を局所逆アセンブルする。ROMバイトは一時領域に留める。
function disassembleFromRom(stem: string): Insn[] {
  const address = Number.parseInt(stem, 16);
  const srcPath = join(ROOT, "src", `${stem}.c`);
  if (!existsSync(srcPath)) throw new Error(`no asm and no src for ${stem}`);
  const scratch = process.env.CLAUDE_SCRATCH ?? join(ROOT, "out", "rebuild", "tmp");
  mkdirSync(scratch, { recursive: true });
  const asmOut = join(scratch, `${stem}.rom.s`);
  const objOut = join(scratch, `${stem}.rom.o`);
  const elfOut = join(scratch, `${stem}.rom.elf`);
  const binOut = join(scratch, `${stem}.rom.bin`);
  const spawn = (cmd: string[]) => {
    const r = Bun.spawnSync(cmd, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
    if (r.exitCode !== 0) throw new Error(`${basename(cmd[0])} failed: ${Buffer.from(r.stderr).toString().trim()}`);
    return Buffer.from(r.stdout).toString();
  };
  // 寸法算出のため一度リンクして関数長を得る（verify.tsと同じ手順の縮約）。
  spawn(compilerCommand(...CFLAGS, "-S", "-o", asmOut, srcPath));
  spawn(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", objOut, asmOut]);
  const symbol = `Func_${stem}`;
  spawn(["arm-none-eabi-ld", `-Ttext=0x${stem}`, "-e", symbol, "--defsym", `${symbol}=0x${stem}`, "-o", elfOut, objOut]);
  const syms = spawn(["arm-none-eabi-nm", "-S", elfOut]);
  const row = syms.split(/\r?\n/).find((l) => l.endsWith(` ${symbol}`));
  if (row === undefined) throw new Error(`cannot size ${stem}`);
  const size = Number.parseInt(row.trim().split(/\s+/)[1], 16);
  const rom = readFileSync(join(ROOT, "gs1-en.gba"));
  const span = rom.subarray(address - ROM_BASE, address - ROM_BASE + size);
  writeFileSync(binOut, span);
  const dis = spawn([
    "arm-none-eabi-objdump", "-D", "-b", "binary", "-m", "arm", "-M", "force-thumb",
    `--adjust-vma=0x${stem}`, binOut,
  ]);
  return parseObjdump(dis);
}

// objdump -D の逐次行を命令列へ写す。アドレス欄・コード欄を除いて本文を採る。
function parseObjdump(text: string): Insn[] {
  const insns: Insn[] = [];
  for (const raw of text.split(/\r?\n/)) {
    const m = raw.match(/^\s*([0-9a-f]+):\s+([0-9a-f ]+?)\t(.*)$/);
    if (m === null) continue;
    let body = m[3].replace(/;.*$/, "").replace(/\/\/.*$/, "").trim();
    if (body === "" || body.startsWith(".")) continue;
    const { mnemonic, operands } = splitInsn(body);
    insns.push({ mnemonic, operands, text: body, labels: [] });
  }
  return insns;
}

const SELF_TEST = `
	.thumb
	.global Func_08000000
	.thumb_func
Func_08000000:
	push	{r4, lr}
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L1
	movs	r0, #1
	bl	Func_08000100
	b.n	.L2
.L1:
	movs	r0, #0
.L2:
	adds	r0, r4, #0
	pop	{r4}
	pop	{r1}
	bx	r1
	.4byte 0x12345678
`;

function selfTest(): void {
  const insns = parseInstructions(SELF_TEST);
  const blocks = buildBlocks(insns);
  const problems: string[] = [];
  // 期待: 4ブロック。入口(条件分岐)・then(呼出し+無条件)・else・合流(復帰)。
  if (blocks.length !== 4) problems.push(`block count ${blocks.length} != 4`);
  const entry = blocks[0];
  if (entry.end !== "branch") problems.push(`entry end ${entry.end} != branch`);
  if (entry.successors.length !== 2) problems.push(`entry successors ${entry.successors.length} != 2`);
  const thenB = blocks[1];
  if (thenB.end !== "branch") problems.push(`then end ${thenB.end} != branch`);
  if (thenB.successors.length !== 1) problems.push(`then successors ${thenB.successors.length} != 1`);
  // 呼出しblが本体内に在ることを確認（終端でないこと）。
  if (!thenB.instructions.some((i) => i.mnemonic === "bl")) problems.push("bl not inside then block");
  const merge = blocks[3];
  if (merge.end !== "return") problems.push(`merge end ${merge.end} != return`);
  if (merge.successors.length !== 0) problems.push(`merge successors ${merge.successors.length} != 0`);
  // データプールが命令に混入していないこと。
  if (insns.some((i) => i.mnemonic === ".4byte")) problems.push("pool leaked into instructions");
  if (problems.length !== 0) {
    console.error("self-test FAILED:");
    for (const p of problems) console.error(`  - ${p}`);
    process.exitCode = 1;
    return;
  }
  console.log(`self-test ok: blocks=${blocks.length} entry.succ=${entry.successors} merge.end=${merge.end}`);
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: cfg_extract.ts [--self-test] [<stem> ...]");
    return;
  }
  if (args.includes("--self-test")) {
    selfTest();
    return;
  }
  const outDir = join(ROOT, "out", "rebuild", "cfg");
  mkdirSync(outDir, { recursive: true });
  const stems = args.filter((a) => !a.startsWith("-"));
  if (stems.length === 0) throw new Error("no stems given");
  for (const stem of stems) {
    const cfg = cfgOf(stem);
    const path = join(outDir, `${stem}.json`);
    writeFileSync(path, JSON.stringify({ format: 1, ...cfg }, null, 1));
    console.log(`${stem}: blocks=${cfg.blocks.length} source=${cfg.source} -> ${path}`);
  }
}

if (import.meta.main) main();
