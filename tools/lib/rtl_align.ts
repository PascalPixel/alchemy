// Aligns a gcc RTL insn sequence (rtl_insn.ts) against a disassembled Thumb
// instruction sequence (thumb_disasm.ts) by CONTENT, not position.
//
// A strict 1:1 zip (candidate_explain.ts's first version) breaks near
// prologues/epilogues, where one RTL insn (`{*epilogue_insns}`) expands into
// several real instructions, or a synthesized pattern collapses several RTL
// insns into one. This module instead classifies each side into a shared,
// coarse instruction class (alu/load/store/call/branch/other) plus, where
// meaningful, a destination register, and finds the longest common
// compatible subsequence -- the same idea as a text diff, tolerant of
// insertions on either side. Unmatched entries on either side are reported,
// not hidden: an unmatched RTL insn or instruction row is itself useful
// information (it is exactly where the correlation broke down).
import type { RtlInsn } from "./rtl_insn.ts";
import { destRegister } from "./rtl_insn.ts";
import type { Instruction } from "./thumb_disasm.ts";
import { isBranchMnemonic, isCallMnemonic, isLoadMnemonic, isStoreMnemonic, primaryDestRegister } from "./thumb_disasm.ts";

export type InsnClass = "alu" | "load" | "store" | "call" | "branch" | "other";

export function classifyRtl(insn: RtlInsn): InsnClass {
  if (insn.kind === "call_insn" || insn.callTarget !== null) return "call";
  if (insn.kind === "jump_insn") return "branch";
  const dest = insn.set?.dest;
  if (dest !== undefined && dest !== null) {
    if (dest.kind === "mem") return "store";
    if (dest.kind === "reg" || dest.kind === "subreg") {
      return insn.set?.src.kind === "mem" || (insn.set?.src.kind === "unary" && insn.set.src.a.kind === "mem")
        ? "load"
        : "alu";
    }
  }
  return "other";
}

export function classifyInstruction(instruction: Instruction): InsnClass {
  if (isCallMnemonic(instruction.mnemonic)) return "call";
  if (isStoreMnemonic(instruction.mnemonic)) return "store";
  if (isLoadMnemonic(instruction.mnemonic)) return "load";
  if (isBranchMnemonic(instruction.mnemonic)) return "branch";
  if (/^(adds?|subs?|muls|ands?|orrs?|eors?|asrs?|lsls?|lsrs?|movs?|mvns?|negs?|cmps?|tst|bics?)$/.test(instruction.mnemonic)) {
    return "alu";
  }
  return "other";
}

// True when an RTL insn and a real instruction are plausible partners: same
// coarse class, and — when both sides expose one — the same destination
// register. Neither side exposing a destination register (stores, calls,
// branches) falls back to class agreement alone, which is weaker but still
// meaningful: it is what lets a store correlate with its scheduler trace
// even though "which store" cannot be pinned down further without simulating
// the memory address.
export function compatible(insn: RtlInsn, instruction: Instruction): boolean {
  const insnClass = classifyRtl(insn);
  const instructionClass = classifyInstruction(instruction);
  if (insnClass !== instructionClass) return false;
  if (insnClass === "alu" || insnClass === "load") {
    const rtlDest = destRegister(insn);
    const realDest = primaryDestRegister(instruction);
    if (rtlDest !== null && realDest !== null) return rtlDest === realDest;
  }
  return true;
}

export interface AlignedPair {
  insn: RtlInsn;
  instruction: Instruction;
}

export interface Alignment {
  pairs: AlignedPair[];
  unmatchedInsns: RtlInsn[];
  unmatchedInstructions: Instruction[];
}

// Standard longest-common-compatible-subsequence DP (Needleman-Wunsch
// without a gap penalty beyond "skip"), scored by `compatible`. O(n*m); n and
// m are function-sized (tens of entries), never a performance concern here.
export function align(insns: readonly RtlInsn[], instructions: readonly Instruction[]): Alignment {
  const n = insns.length;
  const m = instructions.length;
  const score: number[][] = Array.from({ length: n + 1 }, () => new Array<number>(m + 1).fill(0));
  for (let i = 1; i <= n; i++) {
    for (let j = 1; j <= m; j++) {
      const match = compatible(insns[i - 1], instructions[j - 1]) ? score[i - 1][j - 1] + 1 : -Infinity;
      score[i][j] = Math.max(match, score[i - 1][j], score[i][j - 1]);
    }
  }
  const pairs: AlignedPair[] = [];
  const matchedInsns = new Set<number>();
  const matchedInstructions = new Set<number>();
  let i = n;
  let j = m;
  while (i > 0 && j > 0) {
    if (
      compatible(insns[i - 1], instructions[j - 1]) &&
      score[i][j] === score[i - 1][j - 1] + 1
    ) {
      pairs.push({ insn: insns[i - 1], instruction: instructions[j - 1] });
      matchedInsns.add(i - 1);
      matchedInstructions.add(j - 1);
      i--; j--;
    } else if (score[i][j] === score[i - 1][j]) {
      i--;
    } else {
      j--;
    }
  }
  pairs.reverse();
  return {
    pairs,
    unmatchedInsns: insns.filter((_, index) => !matchedInsns.has(index)),
    unmatchedInstructions: instructions.filter((_, index) => !matchedInstructions.has(index)),
  };
}

function selfTest(): void {
  const aluInsn = (uid: number, destReg: number): RtlInsn => ({
    uid, kind: "insn", code: "set", callTarget: null, raw: "", dependencies: [],
    set: { dest: { kind: "reg", number: destReg, name: `r${destReg}` }, src: { kind: "unknown", raw: "" } },
  });
  const callInsn = (uid: number): RtlInsn =>
    ({ uid, kind: "call_insn", code: "parallel", set: null, callTarget: { kind: "unknown", raw: "", dependencies: [] }, raw: "" });
  const instr = (offset: number, mnemonic: string, destReg: number | null): Instruction => ({
    offset, mnemonic, raw: "",
    operands: destReg === null ? [] : [{ kind: "reg", name: `r${destReg}`, number: destReg, writeback: false }],
  });

  // Two RTL insns writing r3 then r2, matched against real instructions --
  // but with TWO extra epilogue-expansion instructions inserted on the real
  // side that have no RTL counterpart (the exact failure mode a strict 1:1
  // position zip cannot survive). The alignment must still find both real
  // matches and report the extras as unmatched, not misalign everything
  // after the gap.
  const insns = [aluInsn(15, 3), aluInsn(17, 2), callInsn(19)];
  const instructions = [
    instr(0, "adds", 3),
    instr(2, "pop", null), // extra: epilogue expansion artifact, no RTL insn
    instr(4, "pop", null), // extra: same
    instr(6, "movs", 2),
    instr(8, "bl", null),
  ];
  const result = align(insns, instructions);
  if (result.pairs.length !== 3) throw new Error(`expected 3 aligned pairs, got ${result.pairs.length}`);
  if (result.pairs[0].insn.uid !== 15 || result.pairs[0].instruction.offset !== 0) {
    throw new Error(`wrong first pair: ${JSON.stringify(result.pairs[0])}`);
  }
  if (result.pairs[1].insn.uid !== 17 || result.pairs[1].instruction.offset !== 6) {
    throw new Error(`alignment did not skip the gap correctly: ${JSON.stringify(result.pairs[1])}`);
  }
  if (result.pairs[2].insn.uid !== 19 || result.pairs[2].instruction.offset !== 8) {
    throw new Error(`wrong third pair: ${JSON.stringify(result.pairs[2])}`);
  }
  if (result.unmatchedInstructions.length !== 2) {
    throw new Error(`expected 2 unmatched (the pop pair), got ${result.unmatchedInstructions.length}`);
  }
  if (result.unmatchedInsns.length !== 0) throw new Error("every RTL insn should have found a partner here");

  // A destination-register mismatch must NOT be treated as compatible even
  // when the class matches -- this is what makes the alignment meaningful
  // rather than just "same mnemonic class, pick the next one."
  if (compatible(aluInsn(1, 3), instr(0, "adds", 4))) {
    throw new Error("alu insns writing different registers must not be compatible");
  }
  if (!compatible(aluInsn(1, 3), instr(0, "adds", 3))) {
    throw new Error("alu insns writing the same register must be compatible");
  }

  console.log("self-test=ok tool=rtl-align");
}

if (import.meta.main && Bun.argv.includes("--self-test")) selfTest();
