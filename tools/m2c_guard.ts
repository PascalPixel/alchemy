#!/usr/bin/env bun
// Refuse unsafe m2c seeding, and measure what m2c actually covered.
//
// Ruling (2026-08-01): m2c is a CROSS-CHECK after a hand draft, never the
// source. It is accurate on values -- in a three-row experiment every constant,
// offset, flag id and call target matched the bytes -- but it has one silent
// failure that disqualifies it as a seed, and one quiet failure that costs
// cross-row insight:
//
//   HAZARD 1, silent truncation. m2c treats `bx rN` as a return. In ARMv4T
//   Thumb, `mov ip, pc / bx rN` is ALSO the call-through-register idiom, so m2c
//   ends the function at the first such call and drops everything after it --
//   behind a clean closing brace, with no marker. On resource_396's driver at
//   0x02001004 it emitted 174 of 470 code bytes and looked complete; 296 bytes
//   (63%) vanished. Measured incidence: 26 sites across 8 of the 96 overlays.
//
//   HAZARD 2, address folding. m2c folds base+offset into absolute addresses:
//   `0x02000240 + 448` becomes `*(s16 *)0x02000400`, and the workspace pointer
//   becomes an indirection off 0x03001e70. Both are CORRECT. Both are
//   uncross-readable: the finding that four overlay drivers read the loader's
//   same scene id and sub-selector is invisible in that rendering. Nothing
//   flags it, so this tool only warns.
//
// The same `bx rN` mistake, and jump-table arms, also make a naive
// walk-to-first-return under-measure a function. `measureSpan` here is aware of
// both; the driver size table published on 2026-08-01 was NOT, and was a lower
// bound only (resource_39c's driver is a 15-arm dispatcher, not ~154 bytes;
// resource_396's is 512 bytes, not ~172).
//
//   bun tools/m2c_guard.ts resource_396 1004          # measure and screen
//   bun tools/m2c_guard.ts resource_396 1004 --draft out.c   # + coverage check
//   bun tools/m2c_guard.ts --self-test
import { readFileSync } from "node:fs";
import { dirname } from "node:path";
import { classify, overlayImage, targetOffset, OVERLAY_BASE } from "./overlay_call_targets.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

/** `mov ip, pc` -- the first halfword of the call-through-register idiom. */
export const MOV_IP_PC = 0x46fc;
const BX_MASK = 0xff87;
const BX = 0x4700;
/** `mov pc, rN` -- a jump-table dispatch, whose arms follow the table. */
const MOV_PC_MASK = 0xff87;
const MOV_PC = 0x4687;

export interface Span {
  end: number;
  /** Sites of `mov ip, pc / bx rN`: calls that a naive walk reads as returns. */
  callThroughRegister: number[];
  /** Sites of `mov pc, rN`: the body continues into arms past the table. */
  jumpTable: number[];
}

/**
 * Walk a function from `start` to its real end, treating the
 * call-through-register idiom as a CALL and following branches past a
 * candidate return. Never runs past `limit`.
 */
export function measureSpan(image: Uint8Array, start: number, limit?: number): Span {
  const data = Buffer.from(image.buffer, image.byteOffset, image.length);
  const stop = Math.min(limit ?? data.length, data.length);
  const callThroughRegister: number[] = [];
  const jumpTable: number[] = [];
  let furthest = start;
  let end = stop;
  for (let at = start; at + 2 <= stop; ) {
    const half = data.readUInt16LE(at);
    // The idiom, checked BEFORE the return test -- this is the whole point.
    if (half === MOV_IP_PC && at + 4 <= stop && (data.readUInt16LE(at + 2) & BX_MASK) === BX) {
      callThroughRegister.push(at);
      at += 4;
      continue;
    }
    if ((half & 0xf800) === 0xf000 && at + 4 <= stop) { at += 4; continue; }   // bl pair
    if ((half & 0xf800) === 0xe000) {                                          // b
      const target = at + 4 + ((((half & 0x7ff) << 21) >> 21) << 1);
      if (target > furthest && target < stop) furthest = target;
      at += 2;
      continue;
    }
    if ((half & 0xf000) === 0xd000 && (half & 0x0f00) !== 0x0f00) {            // b<cond>
      const target = at + 4 + ((((half & 0xff) << 24) >> 24) << 1);
      if (target > furthest && target < stop) furthest = target;
      at += 2;
      continue;
    }
    if ((half & MOV_PC_MASK) === MOV_PC) { jumpTable.push(at); at += 2; continue; }
    const isReturn = (half & 0xff00) === 0xbd00 || (half & BX_MASK) === BX;
    if (isReturn && at + 2 > furthest) { end = at + 2; break; }
    at += 2;
  }
  return { end, callThroughRegister, jumpTable };
}

/** Call targets a span makes, by name, resolved with the +2 rule. */
export function callNames(image: Uint8Array, start: number, end: number, overlay: string): string[] {
  const data = Buffer.from(image.buffer, image.byteOffset, image.length);
  const inventoryPath = `${ROOT}/out/decomp/overlays.json`;
  let prologues = new Set<number>();
  try {
    const rows = JSON.parse(readFileSync(inventoryPath, "utf8")).functions as {
      overlay: string; offset: number; starts_with_prologue: boolean;
    }[];
    prologues = new Set(rows.filter((r) => r.overlay === overlay && r.starts_with_prologue).map((r) => r.offset));
  } catch { /* inventory is optional here */ }
  const names: string[] = [];
  for (let at = start; at + 4 <= end; at += 2) {
    const target = targetOffset(data.readUInt16LE(at), data.readUInt16LE(at + 2));
    if (target === null || target < 0 || target >= data.length) continue;
    const detail = classify(image, target, prologues);
    // A branch-skipped literal word can have the f0xx/f8xx bit shape of BL.
    // If its decoded target is neither a known prologue/import nor code that
    // reaches a return, classify() leaves it unknown. Requiring a C spelling
    // for that target manufactures a call: resource_3bb:3458 has exactly this
    // shape at 0x35c4 inside its skipped pool. The multiset audit applies the
    // same rule. Genuine calls remain visible because their target resolves to
    // a veneer, prologue, leaf, or call_via slot.
    if (detail.kind === "unknown") {
      at += 2;
      continue;
    }
    // A BL into the overlay's `bx rN` bank names the callback value held in a
    // register, not the trampoline itself. The draft represents it as an
    // explicit function-pointer call, so requiring a fictitious Func_0200xxxx
    // symbol is a false shortfall. Ordered-call checking audits the call_via
    // position separately.
    if (detail.kind === "call_via") {
      at += 2;
      continue;
    }
    const name = detail.imported !== undefined
      ? `Func_${detail.imported.toString(16).padStart(8, "0")}`
      : `Func_${(OVERLAY_BASE + target).toString(16).padStart(8, "0")}`;
    if (!names.includes(name)) names.push(name);
    at += 2;
  }
  return names;
}

/** Call names the span makes that never appear in an m2c draft. */
export function missingFromDraft(draft: string, names: string[]): string[] {
  const seen = draft.toLowerCase();
  return names.filter((name) => !seen.includes(name.toLowerCase().replace(/^func_0*/, "func_0")) &&
    !seen.includes(name.toLowerCase()) &&
    !seen.includes(name.toLowerCase().replace("func_0", "func_")));
}

function selfTest(): void {
  const halves = (list: number[]): Uint8Array => {
    const data = Buffer.alloc(list.length * 2);
    list.forEach((half, index) => data.writeUInt16LE(half, index * 2));
    return new Uint8Array(data);
  };
  // A body whose only `bx` is the second half of `mov ip, pc / bx r4`, followed
  // by more code and a real `pop {pc}`. A naive walk stops at offset 4; this
  // one must not.
  const image = halves([0xb500, 0x2001, 0x46fc, 0x4720, 0x2002, 0x2003, 0xbd00]);
  const span = measureSpan(image, 0);
  if (span.callThroughRegister.length !== 1 || span.callThroughRegister[0] !== 4) {
    throw new Error("m2c guard self-test: call-through-register not detected");
  }
  if (span.end !== 14) throw new Error(`m2c guard self-test: truncated at ${span.end}, expected 14`);
  // A plain `bx r1` return still ends the function.
  const plain = measureSpan(halves([0xb500, 0x2001, 0xbc02, 0x4708, 0x2002]), 0);
  if (plain.end !== 8) throw new Error(`m2c guard self-test: plain return mis-measured (${plain.end})`);
  if (plain.callThroughRegister.length !== 0) throw new Error("m2c guard self-test: false idiom hit");
  // A forward branch past a candidate return keeps the walk going.
  const branched = measureSpan(halves([0xb500, 0xe002, 0xbd00, 0x2000, 0x2001, 0xbd00]), 0);
  if (branched.end !== 12) throw new Error(`m2c guard self-test: branch not followed (${branched.end})`);
  // `mov pc, r3` is recorded as a jump table rather than treated as a return.
  const table = measureSpan(halves([0xb500, 0x469f, 0x2000, 0xbd00]), 0);
  if (table.jumpTable.length !== 1) throw new Error("m2c guard self-test: jump table not detected");
  // Coverage: a name absent from the draft is reported.
  if (missingFromDraft("void f(void) { Func_0808a010(1); }", ["Func_0808a010", "Func_0808a020"]).length !== 1) {
    throw new Error("m2c guard self-test: coverage check wrong");
  }
  console.log("m2c guard self-test passed");
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) return selfTest();
  const overlay = argv.find((a) => /^resource_[0-9a-f]+$/.test(a));
  const numeric = argv.filter((a) => /^[0-9a-f]{1,5}$/i.test(a)).map((a) => Number.parseInt(a, 16));
  if (overlay === undefined || numeric.length === 0) {
    console.log("usage: m2c_guard.ts <resource_NNN> <startHex> [endHex] [--draft FILE]");
    process.exitCode = 1;
    return;
  }
  const image = overlayImage(overlay);
  const start = numeric[0];
  const span = measureSpan(image, start, numeric[1]);
  const size = span.end - start;
  console.log(`${overlay} 0x${(OVERLAY_BASE + start).toString(16)} measured span ${size} bytes` +
    ` (ends 0x${(OVERLAY_BASE + span.end).toString(16)})`);
  let refuse = false;
  if (span.callThroughRegister.length > 0) {
    refuse = true;
    console.log(`  REFUSED for m2c seeding: ${span.callThroughRegister.length} call-through-register site(s) at ` +
      span.callThroughRegister.map((s) => "0x" + (OVERLAY_BASE + s).toString(16)).join(", ") +
      "\n  m2c ends the function at the first of these and drops the rest silently.");
  }
  if (span.jumpTable.length > 0) {
    console.log(`  CAUTION: ${span.jumpTable.length} jump-table dispatch(es) at ` +
      span.jumpTable.map((s) => "0x" + (OVERLAY_BASE + s).toString(16)).join(", ") +
      " -- arms follow the table, so any naive size is a lower bound.");
  }
  const draftPath = argv[argv.indexOf("--draft") + 1];
  if (argv.includes("--draft") && draftPath !== undefined) {
    const names = callNames(image, start, span.end, overlay);
    const missing = missingFromDraft(readFileSync(draftPath, "utf8"), names);
    console.log(`  call coverage: ${names.length - missing.length}/${names.length} of the span's callees appear in the draft`);
    if (missing.length > 0) {
      refuse = true;
      console.log(`  SHORTFALL -- absent from the draft: ${missing.join(", ")}`);
    }
  }
  if (refuse) process.exitCode = 1;
  else console.log("  no seeding hazard found (still cross-check only: m2c is never the source)");
}

if (import.meta.main) main();
