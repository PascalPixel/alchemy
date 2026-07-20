#!/usr/bin/env bun
// ブロック合成。再構築器の第三部。対象関数の命令列を辞書雛形で被覆し、
// ビーム探索で先頭からの説明語数を最大化する。選んだ雛形のC文を連結し、
// 変数を通しで束縛して前文と推定シグネチャで包み、承認コンパイラの
// コンパイル・リンク・差分路で実バイトを採点する。完全一致のみ、領域長
// 検査を経てsrc/へ設置する。ROMバイトには触れず、出力はout/配下のみ。
//
// これは試作。辞書被覆・変数束縛・レジスタ結合のどこで合成が破れるかを
// 明らかにするのが目的で、負の結果でも診断が明確なら成功とみなす。
import { existsSync, mkdirSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { M2C_PREAMBLE } from "./match_m2c.ts";
import { weightedDiff } from "./permute_v1.ts";
import { replaceableAssembly } from "./permute_m2c.ts";
import { verify } from "./verify.ts";
import { CFLAGS, compilerCommand, externalSymbol, externalSymbolAssembly } from "./alchemy_gcc.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;
const SCRATCH = process.env.CLAUDE_SCRATCH ?? join(ROOT, "out", "rebuild", "synth");

// ---- 命令の正規化 -----------------------------------------------------
// mine_blocksのabstract()と同じ規約: レジスタを出現順のrA,rB…へ、即値は
// 残す。ここでは辞書側と対象側を同一規約へ落として比較できるようにする。
interface Norm {
  mnemonic: string; // .n/.w接尾辞を除去した小文字
  operands: string; // レジスタ抽象化済み、分岐ラベルは<label>へ
  isBranch: boolean;
}

const BRANCH = /^(b|bl|blx|bx|beq|bne|bgt|blt|bge|ble|bhi|bls|bcs|bcc|bmi|bpl)$/;

// Thumbのフラグ設定形は逆アセンブラが末尾sを付す（movs/adds/subs…）が、
// 承認コンパイラの-S出力は付さない（mov/add/sub…）。同一命令なので辞書側と
// 対象側を突き合わせる前に、この末尾sを剥がして正規名へ揃える。
const FLAG_S = /^(mov|mvn|add|adc|sub|sbc|rsb|and|orr|eor|bic|lsl|lsr|asr|ror|neg|mul)s$/;
function stripSuffix(mnemonic: string): string {
  const base = mnemonic.toLowerCase().replace(/\.(n|w)$/, "");
  return FLAG_S.test(base) ? base.slice(0, -1) : base;
}

// 一区間のレジスタを局所的にrA,rB…へ束ね直し、正規命令列を返す。
function normalize(insns: Array<{ mnemonic: string; operands: string }>): Norm[] {
  const map = new Map<string, string>();
  const alpha = (n: number) => `r${String.fromCharCode(65 + (n % 26))}${n >= 26 ? Math.floor(n / 26) : ""}`;
  const rename = (text: string) =>
    text.replace(/\b(r(?:1[0-2]|[0-9])|ip|fp|sl)\b/g, (reg) => {
      if (!map.has(reg)) map.set(reg, alpha(map.size));
      return map.get(reg)!;
    });
  return insns.map((i) => {
    const mnemonic = stripSuffix(i.mnemonic);
    const isBranch = BRANCH.test(mnemonic);
    // 分岐先ラベルは辞書と対象で一致しないので<label>へ潰す。
    const operands = isBranch ? rename(i.operands).replace(/\.?[.$A-Za-z_][\w.$]*$/, "<label>") : rename(i.operands);
    return { mnemonic, operands: operands.trim(), isBranch };
  });
}

// 既に正規化済みの辞書文字列（"mov rA, #1" 等）をNormへ写す。
function parseNormLine(line: string): Norm {
  const space = line.indexOf(" ");
  const rawMnemonic = space === -1 ? line : line.slice(0, space);
  const mnemonic = stripSuffix(rawMnemonic);
  const isBranch = BRANCH.test(mnemonic);
  let operands = space === -1 ? "" : line.slice(space + 1).trim();
  if (isBranch) operands = operands.replace(/\.?[.$A-Za-z_][\w.$]*$/, "<label>").trim();
  return { mnemonic, operands, isBranch };
}

// 単一命令の一致判定。分岐はラベル無視で機種のみ、それ以外は機種＋
// オペランド（レジスタ抽象化済み・即値保持）で厳密に照合する。
function sameInsn(a: Norm, b: Norm): boolean {
  if (a.mnemonic !== b.mnemonic) return false;
  if (a.isBranch) return true; // ラベルは<label>同士。分岐は機種一致で可
  return a.operands === b.operands;
}

// ---- 対象の取り込み ---------------------------------------------------

interface CfgBlock {
  id: number;
  instructions: Array<{ mnemonic: string; operands: string }>;
}

// CFGブロックを開始位置順に平坦化して対象の線形命令列を得る。
function flattenTarget(cfg: { blocks: Array<CfgBlock & { start: number }> }): Array<{ mnemonic: string; operands: string }> {
  const ordered = [...cfg.blocks].sort((a, b) => a.start - b.start);
  const out: Array<{ mnemonic: string; operands: string }> = [];
  for (const block of ordered) for (const insn of block.instructions) out.push(insn);
  return out;
}

// プロローグからシグネチャを推定する。push集合で退避レジスタを、r0..r3の
// 初出参照で引数個数を、末尾のmovs r0/戻り値有無で戻り型を見積もる。
function inferSignature(stem: string, insns: Array<{ mnemonic: string; operands: string }>): { returns: boolean; args: number } {
  // 引数個数: 初めて書かれる前に読まれるr0..r3の最大番号。
  const written = new Set<string>();
  let maxArg = -1;
  for (const insn of insns) {
    const mnemonic = stripSuffix(insn.mnemonic);
    // 分岐・復帰・退避は引数読取りに数えない（bx r1等の戻り規約を誤検出しない）。
    if (BRANCH.test(mnemonic) || mnemonic === "push" || mnemonic === "pop") continue;
    const parts = insn.operands.split(",").map((s) => s.trim());
    // 第一オペランドが書込み先の代表的な機種。
    const writesFirst = /^(mov|movs|add|adds|sub|subs|ldr|ldrb|ldrh|ldrsh|ldrsb|lsl|lsr|asr|neg|and|orr|eor|mul)$/.test(mnemonic);
    parts.forEach((part, index) => {
      const match = part.match(/^r([0-3])$/);
      if (!match) return;
      const isDest = writesFirst && index === 0;
      if (!isDest && !written.has(part)) maxArg = Math.max(maxArg, Number(match[1]));
      if (isDest) written.add(part);
    });
  }
  // 戻り値: 復帰前にr0へ書く命令があれば戻り型あり。
  const returns = insns.some((i) => /^r0\b/.test(i.operands) && /^(mov|movs|ldr|add|adds|sub|subs)/.test(stripSuffix(i.mnemonic)));
  return { returns, args: maxArg + 1 };
}

// ---- 辞書 -------------------------------------------------------------

interface Template {
  template: string; // C文
  pattern: Norm[]; // 正規化命令列
  anchor: string;
}

function loadDictionary(): Template[] {
  const path = join(ROOT, "out", "rebuild", "idiom_blocks.jsonl");
  const templates: Template[] = [];
  for (const line of readFileSync(path, "utf8").split("\n").filter(Boolean)) {
    const record = JSON.parse(line) as { template: string; normalized_instructions: string[]; anchor: string };
    templates.push({
      template: record.template,
      pattern: record.normalized_instructions.map(parseNormLine),
      anchor: record.anchor,
    });
  }
  return templates;
}

// 対象の位置kから雛形patternが前方一致するなら消費語数を返す。合わなければ0。
function matchAt(target: Norm[], k: number, pattern: Norm[]): number {
  if (pattern.length === 0 || k + pattern.length > target.length) return 0;
  for (let i = 0; i < pattern.length; i++) if (!sameInsn(target[k + i], pattern[i])) return 0;
  return pattern.length;
}

// ---- ビーム探索 -------------------------------------------------------

interface Beam {
  covered: number; // 説明済み先頭語数
  statements: string[]; // 選んだC文
}

function beamSearch(target: Norm[], dictionary: Template[], width: number): { beams: Beam[]; expansions: number } {
  let frontier: Beam[] = [{ covered: 0, statements: [] }];
  const seen = new Set<string>();
  let best: Beam[] = [...frontier];
  let expansions = 0;
  // 各段で全ビームを各雛形で拡張し、被覆語数で上位widthを残す。
  for (let round = 0; round < target.length && frontier.length > 0; round++) {
    const next: Beam[] = [];
    for (const beam of frontier) {
      if (beam.covered >= target.length) continue;
      for (const template of dictionary) {
        const consumed = matchAt(target, beam.covered, template.pattern);
        if (consumed === 0) continue;
        expansions++;
        const candidate: Beam = { covered: beam.covered + consumed, statements: [...beam.statements, template.template] };
        const key = `${candidate.covered}|${candidate.statements.join("§")}`;
        if (seen.has(key)) continue;
        seen.add(key);
        next.push(candidate);
      }
    }
    if (next.length === 0) break;
    next.sort((a, b) => b.covered - a.covered || a.statements.length - b.statements.length);
    frontier = next.slice(0, width);
    best = frontier;
  }
  best.sort((a, b) => b.covered - a.covered || a.statements.length - b.statements.length);
  return { beams: best, expansions };
}

// ---- 採点（permute_v1のScorerを簡約再現）-----------------------------

function runSync(command: string[]): number {
  const result = Bun.spawnSync(command, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  return result.exitCode ?? 1;
}

// 完全リンクで.textバイトを得る。未定義外部シンボルは束を生成して解決。
function assemble(stem: string, source: string, expectedLength: number): Buffer | null {
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
  return readFileSync(bin).subarray(0, Math.max(expectedLength, 4));
}

// ---- 合成本体 ---------------------------------------------------------

function typeName(count: number): string {
  return count === 0 ? "void" : "s32";
}

// 推定シグネチャと本文からC関数を組み立てる。雛形の%vNプレースホルダは
// 出現順に束縛する。先頭args個は引数argNへ、残りはローカルへ宣言する。
function wrapFunction(stem: string, signature: { returns: boolean; args: number }, statements: string[]): string {
  const parameters = signature.args === 0 ? "void" : Array.from({ length: signature.args }, (_, i) => `s32 arg${i}`).join(", ");
  const returnType = signature.returns ? "s32" : "void";
  // 全文を走査して%vNを一意に集め、出現順に名前を割り当てる。
  const slots: number[] = [];
  for (const s of statements) for (const m of s.matchAll(/%v(\d+)/g)) if (!slots.includes(Number(m[1]))) slots.push(Number(m[1]));
  const binding = new Map<number, string>();
  const locals: string[] = [];
  slots.forEach((slot, index) => {
    if (index < signature.args) binding.set(slot, `arg${index}`);
    else {
      const name = `v${index}`;
      binding.set(slot, name);
      locals.push(`    s32 ${name};`);
    }
  });
  const bind = (s: string) => s.replace(/%v(\d+)/g, (_, n) => binding.get(Number(n)) ?? `arg0`);
  const declarations = locals.length > 0 ? locals.join("\n") + "\n" : "";
  const body = statements.map((s) => `    ${bind(s)}`).join("\n");
  const tail = signature.returns ? "\n    return 0;" : "";
  return `${M2C_PREAMBLE}${returnType} Func_${stem}(${parameters}) {\n${declarations}${body}${tail}\n}\n`;
}

function main(): void {
  const argv = Bun.argv.slice(2);
  const width = Number(argv.find((a, i) => argv[i - 1] === "--width") ?? 200);
  const stem = argv.find((a) => /^[0-9a-f]{8}$/.test(a)) ?? "080bbae8";
  const install = argv.includes("--install");

  const cfgPath = join(ROOT, "out", "rebuild", "cfg", `${stem}.json`);
  if (!existsSync(cfgPath)) throw new Error(`missing cfg ${cfgPath} (run tools/cfg_extract.ts ${stem})`);
  const cfg = JSON.parse(readFileSync(cfgPath, "utf8"));
  const rawTarget = flattenTarget(cfg);
  const target = normalize(rawTarget);
  const signature = inferSignature(stem, rawTarget);
  const dictionary = loadDictionary();

  console.log(`stem=${stem}`);
  console.log(`target instructions=${target.length}`);
  console.log(`  ${target.map((n) => `${n.mnemonic} ${n.operands}`).join(" | ")}`);
  console.log(`inferred signature: ${typeName(signature.returns ? 1 : 0)} Func_${stem}(${signature.args} arg)`);
  console.log(`dictionary templates=${dictionary.length}`);

  // 辞書被覆の診断: 対象の各先頭位置で一致する雛形が何本あるか。
  const perPosition: number[] = [];
  for (let k = 0; k < target.length; k++) {
    let count = 0;
    for (const template of dictionary) if (matchAt(target, k, template.pattern) > 0) count++;
    perPosition.push(count);
  }
  console.log(`templates matching at each target position: [${perPosition.join(", ")}]`);
  const firstUnmatchable = perPosition.findIndex((c) => c === 0);
  console.log(`first position with zero matching templates: ${firstUnmatchable}`);

  const { beams, expansions } = beamSearch(target, dictionary, width);
  console.log(`beam width=${width} expansions=${expansions} beams=${beams.length}`);
  const bestCoverage = beams.length > 0 ? beams[0].covered : 0;
  console.log(`best coverage: ${bestCoverage}/${target.length} leading instructions`);
  if (beams.length > 0 && beams[0].statements.length > 0) {
    console.log(`best beam statements: ${JSON.stringify(beams[0].statements)}`);
  }

  // 期待バイトはROM当該領域。素の空本文を一度リンクして期待長を得る。
  const rom = readFileSync(join(ROOT, "gs1-en.gba"));
  const address = Number.parseInt(stem, 16) - ROM_BASE;

  // 採点対象の候補を組む: 上位ビーム由来の本文と、比較のための素の下書き。
  const candidateBodies = new Map<string, string>();
  const topBeams = beams.slice(0, Math.min(beams.length, 16));
  for (const beam of topBeams) {
    const source = wrapFunction(stem, signature, beam.statements);
    candidateBodies.set(source, `beam(cov=${beam.covered},stmts=${beam.statements.length})`);
  }
  // 素の候補: 何も被覆できないときの下限。空predicate/空手続き。
  candidateBodies.set(wrapFunction(stem, signature, []), "empty");

  // まず期待長を確定する。空候補をリンクして期待バイトを切り出す。
  let expected: Buffer | null = null;
  {
    const probe = wrapFunction(stem, signature, []);
    const bytes = assemble(stem, probe, 4);
    if (bytes !== null) {
      // 期待長はROM上の当該関数の実長。CFGの命令数×2を目安に、ROMから読む。
      const romLength = rawTarget.length * 2; // thumb命令は2バイト（分岐.wは稀）
      expected = Buffer.from(rom.subarray(address, address + romLength));
    }
  }
  if (expected === null) {
    console.log("could not establish expected bytes (probe link failed) — aborting scoring");
    return;
  }
  console.log(`expected region length: ${expected.length} bytes (target ROM 0x${stem})`);

  let best: { label: string; score: number; source: string; bytes: Buffer } | null = null;
  for (const [source, label] of candidateBodies) {
    const bytes = assemble(stem, source, expected.length);
    if (bytes === null) {
      continue;
    }
    const score = bytes.length === expected.length ? weightedDiff(bytes, expected) : Number.MAX_SAFE_INTEGER;
    if (best === null || score < best.score) best = { label, score, source, bytes };
  }

  if (best === null) {
    console.log("no candidate compiled+linked successfully");
    return;
  }

  // バイト単位の生の不一致（重み無し）も併記する。
  const rawMismatch = best.bytes.length === expected.length
    ? Array.from(expected).filter((byte, index) => byte !== best!.bytes[index]).length
    : `len ${best.bytes.length} vs ${expected.length}`;
  console.log(`best candidate: ${best.label}`);
  console.log(`  weighted score: ${best.score === Number.MAX_SAFE_INTEGER ? "length-mismatch" : best.score}`);
  console.log(`  raw byte mismatch: ${rawMismatch} / ${expected.length}`);
  const exact = best.bytes.length === expected.length && best.bytes.equals(expected);
  console.log(`  exact match: ${exact}`);

  if (exact && install) {
    // 完全一致時のみ: 領域長検査を経てsrc/へ設置しasmを削除する。
    const scratch = SCRATCH;
    const candidatePath = join(scratch, `${stem}.c`);
    writeFileSync(candidatePath, best.source);
    const [okay, size] = verify(candidatePath, rom, scratch);
    if (okay && replaceableAssembly(stem, size, scratch)) {
      writeFileSync(join(ROOT, "src", `${stem}.c`), best.source);
      rmSync(join(ROOT, "asm", `${stem}.s`), { force: true });
      console.log(`installed src/${stem}.c and removed asm/${stem}.s (size=${size})`);
    } else {
      console.log(`exact match but install guard failed (okay=${okay} size=${size}) — leaving asm in place`);
    }
  } else if (exact) {
    console.log("exact match — rerun with --install to write src/ and remove asm/");
  }
}

if (import.meta.main) main();
