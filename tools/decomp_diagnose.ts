#!/usr/bin/env bun
// Tool role: both; imported by tools/decomp_queue.ts; invoked by PLAYBOOK.md, tools/alchemy_gcc.ts.
// Exact-C candidate diagnostics. Candidate and reference bytes remain in ignored
// scratch space; the durable report contains only aggregate mismatch classes.
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { verifyCandidate } from "./match_m2c.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;
let assemblySizes: Map<string, number> | null = null;

function expectedSize(stem: string, fallback: number): number {
  if (assemblySizes === null) {
    assemblySizes = new Map();
    const path = [join(ROOT, "out/full/asm/manifest.json"), join(ROOT, "out/asm/manifest.json")].find(existsSync);
    if (path) {
      const document = JSON.parse(readFileSync(path, "utf8")) as { regions?: Array<{ source: string; size: number }> };
      for (const region of document.regions ?? []) assemblySizes.set(basename(region.source, ".s"), region.size);
    }
  }
  return assemblySizes.get(stem) ?? fallback;
}

export type MismatchKind =
  | "register_only" | "instruction_reorder" | "literal_or_address"
  | "control_flow" | "semantic" | "missing_instruction" | "extra_instruction";

export interface DecodedInstruction {
  address: number;
  size: number;
  mnemonic: string;
  operands: string;
}

export interface CandidateDiagnosis {
  stem: string;
  byte_mismatches: number;
  actual_size: number;
  expected_size: number;
  instruction_mismatches: number;
  counts: Record<MismatchKind, number>;
  dominant: "exact" | MismatchKind;
  register_fraction: number;
  semantic_fraction: number;
}

const KINDS: MismatchKind[] = [
  "register_only", "instruction_reorder", "literal_or_address", "control_flow",
  "semantic", "missing_instruction", "extra_instruction",
];

function baseMnemonic(value: string): string {
  const bare = value.toLowerCase().replace(/\.(?:n|w)$/, "");
  return /^(mov|mvn|add|adc|sub|sbc|rsb|and|orr|eor|bic|lsl|lsr|asr|ror|neg|mul)s$/.test(bare)
    ? bare.slice(0, -1) : bare;
}

function normalizedOperands(value: string, registers: boolean): string {
  let text = value.replace(/\s*[;@].*$/, "").replace(/\s+/g, " ")
    .replace(/0x[0-9a-f]+\s+<[^>]+>/gi, "<address>").trim();
  if (!registers) text = text.replace(/\b(?:r(?:1[0-2]|[0-9])|sp|lr|pc|ip|fp|sl)\b/gi, "<reg>");
  return text;
}

function signature(instruction: DecodedInstruction, registers: boolean): string {
  return `${baseMnemonic(instruction.mnemonic)} ${normalizedOperands(instruction.operands, registers)}`;
}

function isControl(instruction: DecodedInstruction): boolean {
  return /^(?:b(?:l|x|eq|ne|cs|cc|hs|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le)?|cbz|cbnz|pop)$/.test(baseMnemonic(instruction.mnemonic));
}

function isLiteralOrAddress(instruction: DecodedInstruction): boolean {
  const mnemonic = baseMnemonic(instruction.mnemonic);
  return (mnemonic === "ldr" && /\bpc\b/i.test(instruction.operands)) || /<address>|0x08[0-9a-f]{6}/i.test(instruction.operands);
}

// Minimum-edit alignment keeps an insertion from making every later instruction
// appear to be a semantic mismatch.
function align(actual: DecodedInstruction[], expected: DecodedInstruction[]): Array<[DecodedInstruction | null, DecodedInstruction | null]> {
  const rows = actual.length + 1, columns = expected.length + 1;
  const cost = Array.from({ length: rows }, () => new Uint16Array(columns));
  const move = Array.from({ length: rows }, () => new Uint8Array(columns));
  for (let row = 1; row < rows; row++) { cost[row][0] = row; move[row][0] = 1; }
  for (let column = 1; column < columns; column++) { cost[0][column] = column; move[0][column] = 2; }
  for (let row = 1; row < rows; row++) {
    for (let column = 1; column < columns; column++) {
      const substitution = signature(actual[row - 1], false) === signature(expected[column - 1], false) ? 0 : 1;
      const choices = [cost[row - 1][column - 1] + substitution, cost[row - 1][column] + 1, cost[row][column - 1] + 1];
      let choice = 0;
      if (choices[1] < choices[choice]) choice = 1;
      if (choices[2] < choices[choice]) choice = 2;
      cost[row][column] = choices[choice]; move[row][column] = choice;
    }
  }
  const result: Array<[DecodedInstruction | null, DecodedInstruction | null]> = [];
  let row = actual.length, column = expected.length;
  while (row > 0 || column > 0) {
    const choice = move[row][column];
    if (row > 0 && column > 0 && choice === 0) result.push([actual[--row], expected[--column]]);
    else if (row > 0 && (column === 0 || choice === 1)) result.push([actual[--row], null]);
    else result.push([null, expected[--column]]);
  }
  return result.reverse();
}

export function diagnoseInstructions(
  stem: string, actual: DecodedInstruction[], expected: DecodedInstruction[],
  actualBytes = 0, expectedBytes = 0, byteMismatches = 0,
): CandidateDiagnosis {
  const counts = Object.fromEntries(KINDS.map((kind) => [kind, 0])) as Record<MismatchKind, number>;
  const pairs = align(actual, expected);
  const actualSignatures = actual.map((item) => signature(item, false));
  const expectedSignatures = expected.map((item) => signature(item, false));
  let mismatches = 0;
  for (let index = 0; index < pairs.length; index++) {
    const [left, right] = pairs[index];
    if (left === null) { counts.missing_instruction++; mismatches++; continue; }
    if (right === null) { counts.extra_instruction++; mismatches++; continue; }
    if (signature(left, true) === signature(right, true)) continue;
    mismatches++;
    if (signature(left, false) === signature(right, false)) { counts.register_only++; continue; }
    const a = actualSignatures.indexOf(signature(right, false));
    const e = expectedSignatures.indexOf(signature(left, false));
    if ((a >= 0 && Math.abs(a - index) <= 3) || (e >= 0 && Math.abs(e - index) <= 3)) counts.instruction_reorder++;
    else if (isLiteralOrAddress(left) || isLiteralOrAddress(right)) counts.literal_or_address++;
    else if (isControl(left) || isControl(right)) counts.control_flow++;
    else counts.semantic++;
  }
  const dominant = mismatches === 0 ? "exact" : KINDS.reduce((best, kind) => counts[kind] > counts[best] ? kind : best, KINDS[0]);
  const semantic = counts.semantic + counts.control_flow + counts.literal_or_address;
  return {
    stem, byte_mismatches: byteMismatches, actual_size: actualBytes, expected_size: expectedBytes,
    instruction_mismatches: mismatches, counts, dominant,
    register_fraction: mismatches === 0 ? 0 : (counts.register_only + counts.instruction_reorder) / mismatches,
    semantic_fraction: mismatches === 0 ? 0 : semantic / mismatches,
  };
}

async function disassemble(bytes: Buffer, address: number, path: string): Promise<DecodedInstruction[]> {
  writeFileSync(path, bytes);
  const child = Bun.spawn([
    "arm-none-eabi-objdump", "-D", "-b", "binary", "-m", "arm", "-M", "force-thumb",
    `--adjust-vma=0x${address.toString(16)}`, path,
  ], { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  const [stdout, code] = await Promise.all([new Response(child.stdout).text(), child.exited]);
  if (code !== 0) throw new Error("objdump failed");
  const result: DecodedInstruction[] = [];
  for (const row of stdout.split(/\r?\n/)) {
    const match = row.match(/^\s*([0-9a-f]+):\s+([0-9a-f ]+?)\s+\t(\S+)\s*(.*)$/i);
    if (match === null) continue;
    const encoded = match[2].trim().split(/\s+/).join("");
    result.push({ address: Number.parseInt(match[1], 16), size: encoded.length / 2, mnemonic: match[3], operands: match[4].trim() });
  }
  return result;
}

export async function diagnoseCandidate(source: string, rom: Buffer, scratch: string): Promise<CandidateDiagnosis> {
  mkdirSync(scratch, { recursive: true });
  const stem = basename(source, ".c");
  const address = Number.parseInt(stem, 16);
  const verification = await verifyCandidate(source, rom, scratch);
  const targetSize = expectedSize(stem, verification.expected.length);
  const target = rom.subarray(address - ROM_BASE, address - ROM_BASE + targetSize);
  const [actual, expected] = await Promise.all([
    disassemble(verification.actual, address, join(scratch, `${stem}.actual.bin`)),
    disassemble(target, address, join(scratch, `${stem}.expected.bin`)),
  ]);
  let bytes = Math.abs(verification.actual.length - target.length);
  for (let index = 0; index < Math.min(verification.actual.length, target.length); index++) {
    if (verification.actual[index] !== target[index]) bytes++;
  }
  return diagnoseInstructions(stem, actual, expected, verification.actual.length, target.length, bytes);
}

function selfTest(): void {
  const insn = (mnemonic: string, operands: string, address: number): DecodedInstruction => ({ address, size: 2, mnemonic, operands });
  const expected = [insn("ldr", "r3, [r0]", 0), insn("str", "r2, [r1]", 2), insn("bx", "lr", 4)];
  const registers = [insn("ldr", "r2, [r0]", 0), insn("str", "r3, [r1]", 2), insn("bx", "lr", 4)];
  const first = diagnoseInstructions("test", registers, expected);
  if (first.counts.register_only !== 2 || first.dominant !== "register_only") throw new Error("register diagnosis self-test failed");
  const inserted = [insn("mov", "r3, r2", 0), ...expected.map((item) => ({ ...item, address: item.address + 2 }))];
  const second = diagnoseInstructions("test", inserted, expected);
  if (second.counts.extra_instruction !== 1) throw new Error("alignment diagnosis self-test failed");
  console.log("decomp diagnosis self-test passed");
}

async function main(): Promise<void> {
  const arguments_ = Bun.argv.slice(2);
  if (arguments_.length === 1 && arguments_[0] === "--self-test") { selfTest(); return; }
  const source = arguments_.find((item) => item.endsWith(".c"));
  if (source === undefined) throw new Error("usage: decomp_diagnose.ts --self-test | CANDIDATE.c [ROM]");
  const romPath = arguments_.find((item) => item.endsWith(".gba")) ?? join(ROOT, "roms", "gs1-en.gba");
  const scratch = join(ROOT, "out", "decomp", "diagnose", basename(source, ".c"));
  console.log(JSON.stringify(await diagnoseCandidate(source, readFileSync(romPath), scratch), null, 2));
}

if (import.meta.main) await main();
