#!/usr/bin/env bun
// Resolve the TRUE callee of every `__call_via_rN` veneer call in a main-band
// function.
//
// THE PROBLEM THIS EXISTS FOR. 0x080072e4 begins the GCC ARMv4T indirect-call
// veneer bank: fifteen four-byte entries, `bx rN; nop`, in register order
// r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, sp, lr, ending at
// 0x08007320. A `bl` landing in that range is NOT a call to a function at the
// branch target -- it is an indirect call through the named register, and the
// instruction that last wrote that register is the callee load.
//
// Decompiled drafts across this tree read those branches as ordinary calls and
// declared prototypes like `void Func_080072f0(void *, const void *, s32,
// const void *)`. The giveaway needs no disassembly: Func_080072f0 alone is
// declared with eleven mutually incompatible signatures. A real function has
// one. The fourth "argument" in those drafts is usually the callee address
// itself, and the `ldr` that produced it looks exactly like dead code.
//
// WHAT THIS TOOL DOES. For a function entry address it decodes Thumb forward,
// finds every `bl` into the veneer bank, reads the dispatched register off the
// bank index, then walks backwards to the instruction that last wrote that
// register. Pc-relative literal loads are resolved to the pooled constant, and
// register-to-register moves are followed up to a bounded depth.
//
// WHAT IT REFUSES TO DO. It never guesses. The backward walk decodes a
// deliberately narrow subset of Thumb -- the forms that actually appear as
// callee loads -- and any halfword outside that subset that could write the
// register terminates the walk with an explicit "unknown writer" result rather
// than a plausible-looking answer. A site whose callee comes from memory (a
// function-pointer table entry, a struct field, a stack slot) is reported as
// INDIRECT and must be read by a human with the surrounding context; those must
// never be pattern-matched across files.
//
//   bun tools/veneer_resolve.ts 0x080ddde0          # one function
//   bun tools/veneer_resolve.ts semantic/main/080a2680.c ...
//   bun tools/veneer_resolve.ts --summary <paths>   # counts by resolution kind
//   bun tools/veneer_resolve.ts --self-test
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { basename, dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;

/** First entry of the `__call_via_rN` bank, and one past its last entry. */
export const VENEER_BASE = 0x080072e4;
export const VENEER_END = 0x08007320;
/** Bank order. Index i lives at VENEER_BASE + 4 * i. */
export const VENEER_REGISTERS = [
  "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
  "r8", "r9", "sl", "fp", "ip", "sp", "lr",
] as const;

export type Register = (typeof VENEER_REGISTERS)[number];

/** Register number 0..14 for a bank register name. */
function registerNumber(name: Register): number {
  return VENEER_REGISTERS.indexOf(name);
}

/**
 * The register a veneer entry dispatches, or null when the address is not a
 * bank entry. Addresses inside the bank but not on a four-byte boundary are
 * rejected rather than rounded.
 */
export function veneerRegister(target: number): Register | null {
  if (target < VENEER_BASE || target >= VENEER_END) return null;
  const offset = target - VENEER_BASE;
  if (offset % 4 !== 0) return null;
  return VENEER_REGISTERS[offset / 4];
}

/**
 * Decode a Thumb BL pair. `pc` is the address of the first halfword. Returns
 * the branch target, or null when the two halfwords are not a BL pair.
 */
export function decodeBl(high: number, low: number, pc: number): number | null {
  if ((high & 0xf800) !== 0xf000) return null;
  if ((low & 0xf800) !== 0xf800) return null;
  let offset = ((high & 0x7ff) << 12) | ((low & 0x7ff) << 1);
  if (offset & 0x400000) offset -= 0x800000; // sign-extend the 23-bit field
  return pc + 4 + offset;
}

export type Writer =
  | { kind: "literal"; register: number; pool: number }
  | { kind: "move"; register: number; from: number }
  | { kind: "memory"; register: number; detail: string; slot?: number; base?: number; disp?: number }
  | { kind: "computed"; register: number }
  | { kind: "unknown"; register: number };

/**
 * Which register a single Thumb halfword writes, and how -- or null when the
 * instruction writes no register this walk cares about.
 *
 * Only the forms that genuinely appear as callee loads are decoded. Anything
 * else that writes a register returns `unknown`, which stops the walk. That is
 * the difference between a tool that reports and one that guesses.
 */
export function decodeWriter(hw: number, pc: number): Writer | null {
  // ldr rD, [pc, #imm8*4]  -- the literal pool load
  if ((hw & 0xf800) === 0x4800) {
    const register = (hw >> 8) & 7;
    const pool = ((pc + 4) & ~3) + (hw & 0xff) * 4;
    return { kind: "literal", register, pool };
  }
  // mov rD, rM (high-register form) -- 0x4600..0x46ff
  if ((hw & 0xff00) === 0x4600) {
    const register = (hw & 7) | ((hw & 0x80) >> 4);
    const from = (hw >> 3) & 15;
    return { kind: "move", register, from };
  }
  // adds rD, rN, #0  /  adds rD, rN, rM  -- 0x1800..0x1fff
  if ((hw & 0xf800) === 0x1800 || (hw & 0xf800) === 0x1c00) {
    const register = hw & 7;
    const from = (hw >> 3) & 7;
    // `adds rD, rN, #0` is a plain register copy; anything else is arithmetic.
    if ((hw & 0xfe00) === 0x1c00 && ((hw >> 6) & 7) === 0) {
      return { kind: "move", register, from };
    }
    return { kind: "computed", register };
  }
  // ldr rD, [rN, #imm5*4]
  if ((hw & 0xf800) === 0x6800) {
    const register = hw & 7;
    const base = (hw >> 3) & 7;
    const disp = ((hw >> 6) & 31) * 4;
    return { kind: "memory", register, detail: `[r${base}, #${disp}]`, base, disp };
  }
  // ldr rD, [sp, #imm8*4]
  if ((hw & 0xf800) === 0x9800) {
    const register = (hw >> 8) & 7;
    return { kind: "memory", register, detail: `[sp, #${(hw & 0xff) * 4}]`, slot: (hw & 0xff) * 4 };
  }
  // ldr rD, [rN, rM]
  if ((hw & 0xfe00) === 0x5800) {
    const register = hw & 7;
    return { kind: "memory", register, detail: `[r${(hw >> 3) & 7}, r${(hw >> 6) & 7}]` };
  }
  // movs rD, #imm8
  if ((hw & 0xf800) === 0x2000) return { kind: "computed", register: (hw >> 8) & 7 };
  // pop {...} -- may restore any listed low register, and r15 via the P bit
  if ((hw & 0xfe00) === 0xbc00) {
    for (let r = 0; r < 8; r++) if (hw & (1 << r)) return { kind: "unknown", register: r };
    return null;
  }
  // Remaining data-processing forms that write a low register. Decoded only far
  // enough to know a write happened; the value is not reconstructed.
  if ((hw & 0xe000) === 0x0000 && (hw & 0xf800) !== 0x1800) {
    return { kind: "computed", register: hw & 7 }; // shifts / add-sub immediate
  }
  if ((hw & 0xfc00) === 0x4000) return { kind: "computed", register: hw & 7 }; // ALU ops
  if ((hw & 0xf800) === 0x3000) return { kind: "computed", register: (hw >> 8) & 7 }; // adds rD,#imm
  if ((hw & 0xf800) === 0x3800) return { kind: "computed", register: (hw >> 8) & 7 }; // subs rD,#imm
  if ((hw & 0xf800) === 0xa000) return { kind: "computed", register: (hw >> 8) & 7 }; // add rD,pc
  if ((hw & 0xf800) === 0xa800) return { kind: "computed", register: (hw >> 8) & 7 }; // add rD,sp
  return null;
}

export type Resolution =
  | { kind: "literal"; pool: number; value: number; via: string[] }
  | { kind: "memory"; detail: string; at: number; via: string[] }
  | { kind: "global"; address: number; at: number; via: string[] }
  | { kind: "computed"; at: number; via: string[] }
  | { kind: "call-return"; at: number; target: number | null; via: string[] }
  | { kind: "incoming"; register: number; via: string[] }
  | { kind: "stack"; slot: number; storedAt: number; inner: Resolution; via: string[] }
  | { kind: "unknown"; reason: string };

/**
 * A callee address must live in ROM, EWRAM or IWRAM. Anything else means the
 * backward walk latched onto a constant that is not a function -- a size, a
 * mask, an offset -- and the answer must be rejected rather than reported.
 */
export function plausibleCallee(value: number): boolean {
  const region = value >>> 24;
  return region === 0x08 || region === 0x09 || region === 0x02 || region === 0x03;
}

export interface Site {
  address: number;
  register: Register;
  resolution: Resolution;
}

/** How far back the walk looks, and how many register hops it follows. */
export const WALK_LIMIT = 60;
export const HOP_LIMIT = 3;

function readHalf(rom: Uint8Array, address: number): number {
  const i = address - ROM_BASE;
  return rom[i] | (rom[i + 1] << 8);
}

function readWord(rom: Uint8Array, address: number): number {
  const i = address - ROM_BASE;
  return (rom[i] | (rom[i + 1] << 8) | (rom[i + 2] << 16) | (rom[i + 3] << 24)) >>> 0;
}

/**
 * Walk backwards from `site` for the last write to `register`. Pure halfword
 * scanning: Thumb is fixed-width apart from BL, and a BL's halves never decode
 * as a writer form that would mislead this walk, so no instruction-boundary
 * reconstruction is needed.
 */
function resolveRegister(
  rom: Uint8Array,
  entry: number,
  site: number,
  register: number,
  via: string[] = [],
): Resolution {
  if (via.length > HOP_LIMIT) return { kind: "unknown", reason: "register hop limit" };
  for (let pc = site - 2; pc >= entry && pc > site - 2 - WALK_LIMIT * 2; pc -= 2) {
    // r0-r3 are caller-saved. A `bl` crossed while tracing one of them means
    // the value is that call's RETURN, and anything written before the call is
    // no longer live. Walking past it invents a callee: four sites in this
    // tree resolved to 0xe0, 0x7c, 0x214 and 0x318 -- allocation sizes -- until
    // this check existed, because the walk skipped a `bl` and picked up the
    // argument that had been loaded into r0 for it.
    if (register <= 3 && pc - 2 >= entry) {
      const low = readHalf(rom, pc);
      const high = readHalf(rom, pc - 2);
      if ((low & 0xf800) === 0xf800 && (high & 0xf000) === 0xf000) {
        return { kind: "call-return", at: pc - 2, target: decodeBl(high, low, pc - 2), via };
      }
    }
    const writer = decodeWriter(readHalf(rom, pc), pc);
    if (!writer || writer.register !== register) continue;
    if (writer.kind === "literal") {
      const value = readWord(rom, writer.pool);
      if (!plausibleCallee(value)) {
        return { kind: "unknown", reason: `pooled 0x${value.toString(16)} at 0x${writer.pool.toString(16)} is not a code address` };
      }
      return { kind: "literal", pool: writer.pool, value, via };
    }
    if (writer.kind === "move") {
      const name = VENEER_REGISTERS[writer.from] ?? `r${writer.from}`;
      return resolveRegister(rom, entry, pc, writer.from, [...via, `${name}@0x${pc.toString(16)}`]);
    }
    if (writer.kind === "memory") {
      // A stack slot is not an opaque memory read: the function filled it, and
      // the `str` that did is findable. Chase it one level.
      if (writer.slot !== undefined) {
        const chased = resolveStackSlot(rom, entry, pc, writer.slot, via);
        if (chased) return chased;
      }
      // When the BASE register is itself a pooled constant, the load has a
      // fixed address: the callee is whatever lives at that global. That is a
      // real answer, not an opaque memory read -- it is how the renderer slots
      // at 0x03001f08 / 0x03001f0c are reached.
      if (writer.base !== undefined && writer.disp !== undefined) {
        const baseResolution = resolveRegister(rom, entry, pc, writer.base, [...via, `r${writer.base}@0x${pc.toString(16)}`]);
        if (baseResolution.kind === "literal") {
          return { kind: "global", address: baseResolution.value + writer.disp, at: pc, via };
        }
      }
      return { kind: "memory", detail: writer.detail, at: pc, via };
    }
    if (writer.kind === "computed") return { kind: "computed", at: pc, via };
    return { kind: "unknown", reason: `unknown writer at 0x${pc.toString(16)}` };
  }
  // Reaching the entry without finding a write is not a failure: the register
  // was never assigned in this function, so its value came from the CALLER.
  // 0x08006f6c is the clearest case in the tree -- its `bl __call_via_r1` is
  // the second instruction, so r1 is plainly an incoming function pointer and
  // the function's declared `(void)` signature is wrong.
  if (site - 2 - WALK_LIMIT * 2 < entry) return { kind: "incoming", register, via };
  return { kind: "unknown", reason: `no write to r${register} within ${WALK_LIMIT} instructions` };
}

/**
 * Trace a value that came out of a stack slot back to the `str` that filled it.
 *
 * Only sound while sp is unchanged between the store and the load: an
 * `add sp` / `sub sp` in between re-bases the slot and the same displacement
 * means a different location. Any sp adjustment aborts the chase rather than
 * producing a confident wrong answer. `push`/`pop` move sp too and abort it
 * for the same reason.
 */
function resolveStackSlot(
  rom: Uint8Array,
  entry: number,
  load: number,
  slot: number,
  via: string[],
): Resolution | null {
  for (let pc = load - 2; pc >= entry; pc -= 2) {
    const hw = readHalf(rom, pc);
    // add sp,#imm / sub sp,#imm (0xb000) and push/pop (0xb400/0xbc00) re-base sp
    if ((hw & 0xff00) === 0xb000) return null;
    if ((hw & 0xfe00) === 0xb400 || (hw & 0xfe00) === 0xbc00) return null;
    // str rD, [sp, #imm8*4]
    if ((hw & 0xf800) === 0x9000 && ((hw & 0xff) * 4) === slot) {
      const stored = (hw >> 8) & 7;
      const inner = resolveRegister(rom, entry, pc, stored, [...via, `[sp,#${slot}]@0x${pc.toString(16)}`]);
      return { kind: "stack", slot, storedAt: pc, inner, via };
    }
  }
  return null;
}

/** Every veneer call site in [entry, end), with its callee resolved. */
export function resolveFunction(rom: Uint8Array, entry: number, end: number): Site[] {
  const sites: Site[] = [];
  for (let pc = entry; pc + 4 <= end; pc += 2) {
    const target = decodeBl(readHalf(rom, pc), readHalf(rom, pc + 2), pc);
    if (target === null) continue;
    const register = veneerRegister(target);
    if (!register) continue;
    sites.push({
      address: pc,
      register,
      resolution: resolveRegister(rom, entry, pc, registerNumber(register)),
    });
  }
  return sites;
}

/** Sorted entry addresses of every owner with a C source, for bounding. */
export function ownerEntries(): number[] {
  const seen = new Set<number>();
  for (const dir of [join(ROOT, "semantic", "main"), join(ROOT, "src")]) {
    if (!existsSync(dir)) continue;
    for (const name of readdirSync(dir)) {
      const stem = basename(name, ".c");
      if (/^08[0-9a-f]{6}$/.test(stem) && name.endsWith(".c")) seen.add(parseInt(stem, 16));
    }
  }
  return [...seen].sort((a, b) => a - b);
}

/** Where to stop decoding: the next owner, capped so a gap cannot run away. */
export function boundOf(entry: number, entries: number[], cap = 0x1000): number {
  for (const other of entries) if (other > entry) return Math.min(other, entry + cap);
  return entry + cap;
}

function describe(resolution: Resolution): string {
  const via = resolution.kind !== "unknown" && resolution.via.length
    ? `  (via ${resolution.via.join(" <- ")})`
    : "";
  switch (resolution.kind) {
    case "literal":
      return `pool 0x${resolution.pool.toString(16)} -> 0x${resolution.value.toString(16).padStart(8, "0")}${via}`;
    case "memory":
      return `INDIRECT memory ${resolution.detail} at 0x${resolution.at.toString(16)}${via}`;
    case "computed":
      return `COMPUTED at 0x${resolution.at.toString(16)}${via}`;
    case "call-return":
      return `RETURN of the call at 0x${resolution.at.toString(16).padStart(8, "0")}${
        resolution.target !== null ? ` -> 0x${resolution.target.toString(16).padStart(8, "0")}` : ""
      }${via}`;
    case "incoming":
      return `INCOMING r${resolution.register} -- never written here, supplied by the caller${via}`;
    case "stack":
      return `via [sp,#${resolution.slot}] stored at 0x${resolution.storedAt.toString(16).padStart(8, "0")}: ${describe(resolution.inner)}`;
    case "global":
      return `the value stored at GLOBAL 0x${resolution.address.toString(16).padStart(8, "0")} (loaded at 0x${resolution.at.toString(16).padStart(8, "0")})${via}`;
    default:
      return `UNRESOLVED (${resolution.reason})`;
  }
}

function selfTest(): void {
  // --- bank geometry -------------------------------------------------------
  if (veneerRegister(0x080072e4) !== "r0") throw new Error("veneer self-test: r0 entry");
  if (veneerRegister(0x080072f0) !== "r3") throw new Error("veneer self-test: r3 entry");
  if (veneerRegister(0x080072f4) !== "r4") throw new Error("veneer self-test: r4 entry");
  if (veneerRegister(0x08007318) !== "sp") throw new Error("veneer self-test: sp entry");
  if (veneerRegister(0x0800731c) !== "lr") throw new Error("veneer self-test: lr entry");
  // One past the end, one before the start, and a misaligned interior address
  // must all be refused rather than rounded into a neighbouring entry.
  if (veneerRegister(0x08007320) !== null) throw new Error("veneer self-test: past end accepted");
  if (veneerRegister(0x080072e0) !== null) throw new Error("veneer self-test: before start accepted");
  if (veneerRegister(0x080072f2) !== null) throw new Error("veneer self-test: misaligned accepted");

  // --- BL decode -----------------------------------------------------------
  // The real encoding at 0x080ddde0's first veneer-free call, checked by hand:
  // `f7ef fbc4  bl 0x80cd594` sitting at 0x080dde08.
  if (decodeBl(0xf7ef, 0xfbc4, 0x080dde08) !== 0x080cd594) {
    throw new Error("veneer self-test: backward BL misdecoded");
  }
  // And a forward one: `f001 fffa  bl 0x8007318` at 0x08005320.
  if (decodeBl(0xf001, 0xfffa, 0x08005320) !== 0x08007318) {
    throw new Error("veneer self-test: forward BL misdecoded");
  }
  if (decodeBl(0x4770, 0xfbc4, 0) !== null) throw new Error("veneer self-test: non-BL accepted");
  if (decodeBl(0xf7ef, 0x4770, 0) !== null) throw new Error("veneer self-test: half BL accepted");

  // --- writer decode -------------------------------------------------------
  // `ldr r5, [pc, #668]` at 0x080dddee resolves to the pool at 0x080de08c.
  const literal = decodeWriter(0x4da7, 0x080dddee);
  if (!literal || literal.kind !== "literal" || literal.register !== 5 || literal.pool !== 0x080de08c) {
    throw new Error("veneer self-test: pc-relative load misdecoded");
  }
  // `mov fp, r1` (0x468b) writes r11 from r1 -- the high-register bit must be
  // folded into the destination, not dropped.
  const move = decodeWriter(0x468b, 0);
  if (!move || move.kind !== "move" || move.register !== 11 || move.from !== 1) {
    throw new Error("veneer self-test: high-register move misdecoded");
  }
  // `adds r6, r0, #0` is a copy; `adds r3, r5, r2` is arithmetic.
  const copy = decodeWriter(0x1c06, 0);
  if (!copy || copy.kind !== "move" || copy.register !== 6 || copy.from !== 0) {
    throw new Error("veneer self-test: adds-#0 copy misdecoded");
  }
  const arith = decodeWriter(0x18ab, 0);
  if (!arith || arith.kind !== "computed") throw new Error("veneer self-test: add-register misdecoded");
  // `ldr r3, [r5, #0]` and `ldr r0, [sp, #48]` are memory, never guessable.
  const mem = decodeWriter(0x682b, 0);
  if (!mem || mem.kind !== "memory" || mem.register !== 3) {
    throw new Error("veneer self-test: register-offset load misdecoded");
  }
  const stack = decodeWriter(0x980c, 0);
  if (!stack || stack.kind !== "memory" || stack.register !== 0) {
    throw new Error("veneer self-test: stack load misdecoded");
  }

  // --- ROM regression ------------------------------------------------------
  // The strongest check available: semantic/main/080e15e8.c accounted for its
  // own veneer sites BY HAND, before this tool existed, as 2x r3, 12x r4,
  // 2x r5, 1x r6, 1x r8 -- eighteen in total. Reproducing that split from the
  // ROM independently is what makes the tool's output on unchecked files
  // trustworthy. Skipped when the ROM is absent.
  const rom = join(ROOT, "roms", "gs1-en.gba");
  if (existsSync(rom)) {
    const image = new Uint8Array(readFileSync(rom));
    const entries = ownerEntries();
    const sites = resolveFunction(image, 0x080e15e8, boundOf(0x080e15e8, entries, 0x1000));
    const counts: Record<string, number> = {};
    for (const site of sites) counts[site.register] = (counts[site.register] ?? 0) + 1;
    const expected: Record<string, number> = { r3: 2, r4: 12, r5: 2, r6: 1, r8: 1 };
    if (sites.length !== 18) {
      throw new Error(`veneer self-test: 080e15e8 has ${sites.length} sites, hand count says 18`);
    }
    for (const [register, want] of Object.entries(expected)) {
      if (counts[register] !== want) {
        throw new Error(
          `veneer self-test: 080e15e8 dispatches ${register} ${counts[register] ?? 0} times, hand count says ${want}`,
        );
      }
    }
    // The two r3 sites are pooled IWRAM helpers, not memory reads.
    const pooled = sites.filter((s) => s.register === "r3" && s.resolution.kind === "literal");
    if (pooled.length !== 2) throw new Error("veneer self-test: 080e15e8 r3 sites not pooled");
    const values = pooled
      .map((s) => (s.resolution as { value: number }).value)
      .sort((a, b) => a - b);
    if (values[0] !== 0x0300013c || values[1] !== 0x03001388) {
      throw new Error("veneer self-test: 080e15e8 r3 callees are not the documented IWRAM helpers");
    }
    // Caller-saved clobber regression. At 0x08003e36 the dispatched r6 is
    // copied from r0 at 0x08003e22, and that r0 is the RETURN of the `bl` at
    // 0x08003e1c -- not the 0xe0 that was loaded into r0 as that call's own
    // argument. Walking past the call reported 0xe0 as the callee.
    const clobber = resolveFunction(image, 0x08003e10, 0x08003e4c);
    const site = clobber.find((s) => s.address === 0x08003e36);
    if (!site) throw new Error("veneer self-test: 08003e10 r6 dispatch not found");
    if (site.resolution.kind !== "call-return" || site.resolution.target !== 0x08004938) {
      throw new Error("veneer self-test: caller-saved clobber not detected at 0x08003e36");
    }
    // No literal resolution anywhere in the tree may be a non-code address.
    for (const entry of entries) {
      for (const s of resolveFunction(image, entry, boundOf(entry, entries))) {
        if (s.resolution.kind === "literal" && !plausibleCallee(s.resolution.value)) {
          throw new Error(
            `veneer self-test: implausible callee 0x${s.resolution.value.toString(16)} at 0x${s.address.toString(16)}`,
          );
        }
      }
    }
    // The stack-slot and pooled-base chases, checked against a file that
    // documented its own answer by hand. 080e15e8.c records that its renderers
    // come from 0x03001f08 and 0x03001f0c; the site at 0x080e1b80 loads one
    // through a stack slot, so resolving it end to end must land on 0x03001f0c.
    const chased = sites.find((s) => s.address === 0x080e1b80);
    if (!chased) throw new Error("veneer self-test: 080e1b80 site missing");
    if (chased.resolution.kind !== "stack") {
      throw new Error("veneer self-test: stack slot not chased to its store");
    }
    const inner = chased.resolution.inner;
    if (inner.kind !== "global" || inner.address !== 0x03001f0c) {
      throw new Error("veneer self-test: pooled-base load not resolved to its global");
    }
    // The sp guard: a slot chase must abort rather than answer across an sp
    // adjustment, because the same displacement then means a different place.
    if (resolveStackSlot(image, 0x080e15e8, 0x080e15ea, 0, []) !== null) {
      throw new Error("veneer self-test: stack chase crossed a push/sp adjustment");
    }

    // An unwritten register is an incoming argument, not a failed walk.
    // 0x08006f6c's `bl __call_via_r1` is its SECOND instruction.
    const thunk = resolveFunction(image, 0x08006f6c, 0x08006f80);
    if (thunk.length !== 1 || thunk[0].resolution.kind !== "incoming") {
      throw new Error("veneer self-test: unwritten register not reported as incoming");
    }
    // 0x080052f4 dispatches sp into a DMA-filled stack buffer: the walk must
    // land on the `mov sp, r1` that retargets the stack, not wander past it.
    const stackKernel = resolveFunction(image, 0x080052f4, 0x08005334);
    if (stackKernel.length !== 1 || stackKernel[0].register !== "sp") {
      throw new Error("veneer self-test: 080052f4 sp dispatch not found");
    }
  }
  console.log("veneer resolve self-test passed");
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) return selfTest();
  const summary = argv.includes("--summary");
  const targets = argv.filter((a) => !a.startsWith("--"));
  if (targets.length === 0) {
    console.log("usage: veneer_resolve.ts <0xADDRESS | path/to/08xxxxxx.c> ... [--summary]");
    console.log("       veneer_resolve.ts --self-test");
    process.exitCode = 1;
    return;
  }
  const image = new Uint8Array(readFileSync(join(ROOT, "roms", "gs1-en.gba")));
  const entries = ownerEntries();
  const kinds: Record<string, number> = {};
  const values: Record<string, number> = {};
  let total = 0;
  for (const target of targets) {
    const stem = target.startsWith("0x") ? target.slice(2) : basename(target, ".c");
    if (!/^0?8[0-9a-f]{6}$/.test(stem)) continue;
    const entry = parseInt(stem, 16);
    const sites = resolveFunction(image, entry, boundOf(entry, entries));
    if (sites.length === 0) continue;
    total += sites.length;
    if (!summary) console.log(`=== 0x${entry.toString(16).padStart(8, "0")}  ${target}`);
    for (const site of sites) {
      kinds[site.resolution.kind] = (kinds[site.resolution.kind] ?? 0) + 1;
      if (site.resolution.kind === "literal") {
        const key = "0x" + site.resolution.value.toString(16).padStart(8, "0");
        values[key] = (values[key] ?? 0) + 1;
      }
      if (!summary) {
        console.log(
          `   0x${site.address.toString(16).padStart(8, "0")}  via ${site.register.padEnd(3)}  <-  ${describe(site.resolution)}`,
        );
      }
    }
  }
  if (summary) {
    console.log(`sites=${total}`);
    for (const [kind, count] of Object.entries(kinds).sort((a, b) => b[1] - a[1])) {
      console.log(`  ${kind.padEnd(10)} ${count}`);
    }
    for (const [value, count] of Object.entries(values).sort((a, b) => b[1] - a[1])) {
      console.log(`  callee ${value}  x${count}`);
    }
  }
}

main();
