// Typed parsing of one arm-none-eabi-objdump Thumb disassembly line into a
// mnemonic plus a list of typed operands, so candidate_explain.ts's aligner
// can compare a real instruction against an RtlInsn structurally (same
// destination register, same instruction class) instead of by raw text.
export type Operand =
  | { kind: "reg"; name: string; number: number; writeback: boolean }
  | { kind: "imm"; value: number }
  | { kind: "mem"; base: string; offsetText: string }
  | { kind: "reglist"; names: string[] }
  | { kind: "label"; text: string }
  | { kind: "other"; text: string };

export interface Instruction {
  offset: number;
  mnemonic: string;
  operands: Operand[];
  raw: string;
}

const REG_NAME = /^(r\d{1,2}|sp|lr|pc)$/i;

export function registerNumber(name: string): number | null {
  const lower = name.toLowerCase();
  if (lower === "sp") return 13;
  if (lower === "lr") return 14;
  if (lower === "pc") return 15;
  const match = /^r(\d{1,2})$/.exec(lower);
  return match === null ? null : Number(match[1]);
}

// Splits an objdump operand string on top-level commas only, respecting
// `[...]` memory operands and `{...}` register lists so their internal
// commas (`[r0, #9]`, `{r5, lr}`) do not get treated as operand separators.
function splitOperands(text: string): string[] {
  const parts: string[] = [];
  let depth = 0;
  let current = "";
  for (const char of text) {
    if (char === "[" || char === "{") depth++;
    if (char === "]" || char === "}") depth--;
    if (char === "," && depth === 0) {
      parts.push(current.trim());
      current = "";
      continue;
    }
    current += char;
  }
  if (current.trim().length > 0) parts.push(current.trim());
  return parts;
}

function parseOperand(text: string): Operand {
  const trimmed = text.trim();
  if (trimmed.startsWith("{")) {
    const names = trimmed.replace(/[{}]/g, "").split(",").map((piece) => piece.trim()).filter(Boolean);
    return { kind: "reglist", names };
  }
  if (trimmed.startsWith("[")) {
    const inner = trimmed.slice(1, trimmed.indexOf("]"));
    const [base, ...rest] = inner.split(",").map((piece) => piece.trim());
    return { kind: "mem", base, offsetText: rest.join(",") };
  }
  const withWriteback = /^(\w+)!$/.exec(trimmed);
  if (withWriteback !== null && REG_NAME.test(withWriteback[1])) {
    const number = registerNumber(withWriteback[1]);
    return number === null
      ? { kind: "other", text: trimmed }
      : { kind: "reg", name: withWriteback[1], number, writeback: true };
  }
  if (REG_NAME.test(trimmed)) {
    const number = registerNumber(trimmed);
    return number === null ? { kind: "other", text: trimmed } : { kind: "reg", name: trimmed, number, writeback: false };
  }
  if (trimmed.startsWith("#")) {
    const value = Number(trimmed.slice(1).split(/\s/)[0]);
    return Number.isFinite(value) ? { kind: "imm", value } : { kind: "other", text: trimmed };
  }
  if (/^0x[0-9a-f]+$/i.test(trimmed) || /^[0-9a-f]+\s*<.*>$/i.test(trimmed)) {
    return { kind: "label", text: trimmed };
  }
  return { kind: "other", text: trimmed };
}

// `objdumpLine` is the third capture group candidate_show.ts/
// overlay_candidate_show.ts already extract per row (mnemonic + operands,
// with any trailing `@ (0x38)`-style address comment still attached).
export function parseInstruction(offset: number, objdumpLine: string): Instruction {
  const withoutComment = objdumpLine.split("\t@")[0].trim();
  const spaceIndex = withoutComment.search(/\s/);
  const mnemonic = spaceIndex === -1 ? withoutComment : withoutComment.slice(0, spaceIndex);
  const operandText = spaceIndex === -1 ? "" : withoutComment.slice(spaceIndex + 1).trim();
  const operands = operandText.length === 0 ? [] : splitOperands(operandText).map(parseOperand);
  return { offset, mnemonic, operands, raw: objdumpLine };
}

// The register an instruction's own encoding treats as its "primary
// destination": for a data-processing/load op, its first reg operand; a
// store's first reg operand is a SOURCE, not a destination, and reglist/
// branch forms have no single answer -- both return null deliberately.
export function primaryDestRegister(instruction: Instruction): number | null {
  if (isStoreMnemonic(instruction.mnemonic)) return null;
  const first = instruction.operands[0];
  return first?.kind === "reg" ? first.number : null;
}

export function isStoreMnemonic(mnemonic: string): boolean {
  return /^(str|strb|strh|stm|stmia|push)/.test(mnemonic);
}
export function isLoadMnemonic(mnemonic: string): boolean {
  return /^(ldr|ldrb|ldrh|ldrsb|ldrsh|ldm|ldmia|pop)/.test(mnemonic);
}
export function isCallMnemonic(mnemonic: string): boolean {
  return /^bl[x]?$/.test(mnemonic);
}
export function isBranchMnemonic(mnemonic: string): boolean {
  return /^(b|bx|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble)$/.test(mnemonic);
}

function selfTest(): void {
  const add = parseInstruction(0x6, "adds\tr3, r3, r2");
  if (add.mnemonic !== "adds" || add.operands.length !== 3) throw new Error(`bad add parse: ${JSON.stringify(add)}`);
  if (add.operands[0].kind !== "reg" || add.operands[0].number !== 3) throw new Error("expected first operand r3");
  if (primaryDestRegister(add) !== 3) throw new Error(`expected dest r3, got ${primaryDestRegister(add)}`);

  const store = parseInstruction(0x10, "strb\tr3, [r0, #9]");
  if (store.operands[0].kind !== "reg" || store.operands[1].kind !== "mem") {
    throw new Error(`bad store parse: ${JSON.stringify(store)}`);
  }
  if ((store.operands[1] as { base: string }).base !== "r0") throw new Error("expected mem base r0");
  if (primaryDestRegister(store) !== null) throw new Error("a store's operand[0] is a source, not a destination");

  const load = parseInstruction(0x0, "ldr\tr3, [pc, #12]\t@ (0x38)");
  if (load.mnemonic !== "ldr" || load.operands.length !== 2) throw new Error(`bad load parse: ${JSON.stringify(load)}`);
  if (primaryDestRegister(load) !== 3) throw new Error(`expected dest r3, got ${primaryDestRegister(load)}`);

  const push = parseInstruction(0x0, "push\t{r5, lr}");
  if (push.operands[0].kind !== "reglist" || (push.operands[0] as { names: string[] }).names.length !== 2) {
    throw new Error(`bad push parse: ${JSON.stringify(push)}`);
  }

  const writeback = parseInstruction(0x2, "ldmia\tr0!, {r5}");
  if (writeback.operands[0].kind !== "reg" || !(writeback.operands[0] as { writeback: boolean }).writeback) {
    throw new Error(`expected writeback register operand: ${JSON.stringify(writeback)}`);
  }

  const call = parseInstruction(0x2c, "bl\t0x11f4");
  if (isCallMnemonic(call.mnemonic) !== true) throw new Error("bl must be a call mnemonic");
  if (call.operands[0].kind !== "label") throw new Error(`expected label operand, got ${JSON.stringify(call.operands[0])}`);

  if (registerNumber("sp") !== 13 || registerNumber("lr") !== 14 || registerNumber("pc") !== 15) {
    throw new Error("sp/lr/pc must resolve to r13/r14/r15");
  }
  if (registerNumber("r10") !== 10 || registerNumber("notareg") !== null) throw new Error("registerNumber edge cases failed");

  console.log("self-test=ok tool=thumb-disasm");
}

if (import.meta.main && Bun.argv.includes("--self-test")) selfTest();
