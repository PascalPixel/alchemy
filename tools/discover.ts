// Tool role: library; imported by tools/decompile_batch.ts, tools/emit_function.ts, tools/executable_gap_sources.ts (+2 more).
import { readFileSync, writeFileSync } from "node:fs";

export let ROM_BASE = 0x08000000;
export type Mode = "arm" | "thumb";
export type Call = [source: number, target: number, mode: Mode];
export type Constants = Map<number, number>;

export function sx(value: number, bits: number): number {
  const sign = 2 ** (bits - 1);
  return (value ^ sign) - sign;
}

function hex(value: number): string {
  return value.toString(16).padStart(8, "0");
}

function numeric(values: Iterable<number>): number[] {
  return [...values].sort((a, b) => a - b);
}

class CallSet implements Iterable<Call> {
  private readonly values = new Map<string, Call>();
  get size(): number { return this.values.size; }
  add(value: Call): this {
    this.values.set(`${value[0]}:${value[1]}:${value[2]}`, value);
    return this;
  }
  [Symbol.iterator](): Iterator<Call> { return this.values.values(); }
}

export interface Instruction {
  size: number;
  mode: Mode;
  kind: string;
}

export interface FunctionInfo {
  entry: number;
  mode: Mode;
  sources: Set<string>;
  blocks: Set<number>;
  instructions: Set<number>;
  callees: Set<number>;
  external_callees: Set<number>;
  unresolved: Set<number>;
}

type Work = [number, Constants];

export class Discovery {
  readonly data: Buffer;
  readonly base: number;
  readonly limit: number;
  functions = new Map<number, FunctionInfo>();
  instructions = new Map<number, Instruction>();
  calls = new CallSet();
  external_calls = new CallSet();
  unresolved = new Set<number>();
  conflicts: Record<string, unknown>[] = [];
  data_refs = new Set<number>();
  literal_slots = new Set<number>();
  pointer_tables = new Map<number, number[]>();
  jump_tables = new Map<number, number[]>();
  jump_table_sites = new Map<number, number>();
  blocked_data = new Set<number>();

  constructor(data: Uint8Array, base = ROM_BASE) {
    this.data = Buffer.from(data);
    this.base = base;
    this.limit = base + data.length;
  }

  inside(address: number, size = 1): boolean {
    return this.base <= address && address + size <= this.limit;
  }

  u16(address: number): number {
    return this.data.readUInt16LE(address - this.base);
  }

  u32(address: number): number {
    return this.data.readUInt32LE(address - this.base);
  }

  add_seed(address: number, mode: Mode, source: string): boolean {
    if (mode === "thumb") address &= ~1;
    if (!this.inside(address, mode === "thumb" ? 2 : 4)) return false;
    if (this.blocked_data.has(address)) return false;
    const old = this.functions.get(address);
    if (old && old.mode !== mode) {
      this.conflicts.push({ address, old: old.mode, new: mode, source });
      return false;
    }
    if (old) {
      old.sources.add(source);
      return false;
    }
    this.functions.set(address, {
      entry: address,
      mode,
      sources: new Set([source]),
      blocks: new Set(),
      instructions: new Set(),
      callees: new Set(),
      external_callees: new Set(),
      unresolved: new Set(),
    });
    return true;
  }

  initial_seeds(): number {
    const reset = this.u32(this.base);
    if ((reset >>> 24) !== 0xea) throw new Error("reset vector is not an unconditional ARM branch");
    const entry = this.base + 8 + (sx(reset & 0xffffff, 24) << 2);
    this.add_seed(entry, "arm", "reset");
    for (let address = entry; address < Math.min(entry + 0x100, this.limit); address += 4) {
      const value = this.u32(address);
      if ((value & 1) && this.inside(value & ~1, 2)) this.add_seed(value, "thumb", "startup-literal");
    }
    let veneer = this.base + 0xc0;
    while (veneer + 8 <= entry && this.u32(veneer) === 0x47204c00) {
      this.add_seed(veneer, "thumb", "header-veneer");
      const target = this.u32(veneer + 4);
      if (target & 1) this.add_seed(target, "thumb", "header-veneer-target");
      veneer += 8;
    }
    return entry;
  }

  note_instruction(fn: FunctionInfo, address: number, size: number, kind: string): boolean {
    const old = this.instructions.get(address);
    if (old && old.mode !== fn.mode) {
      this.conflicts.push({ address, old: old.mode, new: fn.mode, source: "flow" });
      return false;
    }
    this.instructions.set(address, { size, mode: fn.mode, kind });
    fn.instructions.add(address);
    return true;
  }

  call(fn: FunctionInfo, source: number, target: number, mode: Mode): void {
    if (mode === "thumb") target &= ~1;
    if (!this.inside(target, mode === "thumb" ? 2 : 4)) {
      fn.external_callees.add(target);
      this.external_calls.add([source, target, mode]);
      return;
    }
    fn.callees.add(target);
    this.calls.add([source, target, mode]);
    this.add_seed(target, mode, `call:${hex(source)}`);
  }

  walk_arm(start: number, fn: FunctionInfo, queue: Work[], seen: Set<number>, initial: Constants): void {
    let pc = start;
    const constants = new Map(initial);
    while (this.inside(pc, 4) && !seen.has(pc)) {
      if ([0, 2].some((offset) => this.blocked_data.has(pc + offset))) return;
      seen.add(pc);
      const word = this.u32(pc);
      const cond = word >>> 28;
      let kind = "linear";
      const successors: number[] = [];
      let stop = false;

      if (((word >>> 26) & 3) === 1 && !(word & (1 << 25))) {
        const load = Boolean(word & (1 << 20));
        const baseRegister = (word >>> 16) & 15;
        const destination = (word >>> 12) & 15;
        const preindexed = Boolean(word & (1 << 24));
        const writeback = Boolean(word & (1 << 21));
        if (load) constants.delete(destination);
        if (load && baseRegister === 15 && preindexed && !writeback) {
          const displacement = word & 0xfff;
          let literal = pc + 8;
          literal += word & (1 << 23) ? displacement : -displacement;
          if (this.inside(literal, 4)) {
            this.literal_slots.add(literal);
            const value = this.u32(literal);
            constants.set(destination, value);
            if (this.inside(value & ~1, 2)) this.data_refs.add(value & ~1);
          }
        }
      }

      if ((word & 0x0e000000) === 0x0a000000) {
        const link = Boolean(word & 0x01000000);
        const target = pc + 8 + (sx(word & 0xffffff, 24) << 2);
        kind = link ? "call" : "branch";
        if (link) {
          this.call(fn, pc, target, "arm");
          for (const register of [0, 1, 2, 3, 12, 14]) constants.delete(register);
        } else {
          successors.push(target);
          stop = cond === 0xe;
        }
        if (cond !== 0xe) {
          successors.push(pc + 4);
          stop = true;
        }
      } else if ((word & 0x0ffffff0) === 0x012fff10) {
        const register = word & 15;
        if (register === 14) kind = "return";
        else {
          kind = "indirect";
          const value = constants.get(register);
          if (value !== undefined) this.call(fn, pc, value, value & 1 ? "thumb" : "arm");
          else {
            fn.unresolved.add(pc);
            this.unresolved.add(pc);
          }
        }
        stop = true;
      } else if ((word & 0x0fffffff) === 0x01a0f00e) {
        kind = "return";
        stop = true;
      } else if ((word & 0x0e10f000) === 0x0410f000) {
        kind = "indirect";
        fn.unresolved.add(pc);
        this.unresolved.add(pc);
        stop = true;
      } else if ((word & 0x0e100000) === 0x08100000 && (word & (1 << 15))) {
        kind = "return";
        stop = true;
      } else if (((word >>> 26) & 3) === 0) {
        const opcode = (word >>> 21) & 15;
        const destination = (word >>> 12) & 15;
        if (destination === 15 && ![8, 9, 10, 11].includes(opcode)) {
          kind = "indirect";
          fn.unresolved.add(pc);
          this.unresolved.add(pc);
          stop = true;
        }
      }
      if (!this.note_instruction(fn, pc, 4, kind)) return;
      for (const target of successors) if (this.inside(target, 4)) queue.push([target, new Map(constants)]);
      if (stop) return;
      pc += 4;
    }
  }

  walk_thumb(start: number, fn: FunctionInfo, queue: Work[], seen: Set<number>, initial: Constants): void {
    let pc = start;
    const constants = new Map(initial);
    while (this.inside(pc, 2) && !seen.has(pc)) {
      if (this.blocked_data.has(pc)) return;
      seen.add(pc);
      const half = this.u16(pc);
      let kind = "linear";
      let size = 2;
      const successors: number[] = [];
      let stop = false;
      const written = new Set<number>();
      if ((half & 0xe000) === 0x0000) written.add(half & 7);
      else if ((half & 0xe000) === 0x2000) {
        if ((half & 0xf800) !== 0x2800) written.add((half >>> 8) & 7);
      } else if ((half & 0xfc00) === 0x4000) {
        const operation = (half >>> 6) & 15;
        if (![8, 10, 11].includes(operation)) written.add(half & 7);
      } else if ((half & 0xfc00) === 0x4400) {
        if (((half >>> 8) & 3) !== 1) written.add((half & 7) | ((half >>> 4) & 8));
      } else if ((half & 0xf000) === 0x5000) {
        if (((half >>> 9) & 7) >= 3) written.add(half & 7);
      } else if ((half & 0xe000) === 0x6000 && (half & 0x0800)) written.add(half & 7);
      else if ((half & 0xf000) === 0x8000 && (half & 0x0800)) written.add(half & 7);
      else if ((half & 0xf000) === 0x9000 && (half & 0x0800)) written.add((half >>> 8) & 7);
      else if ((half & 0xff00) === 0xbc00) {
        for (let r = 0; r < 8; r++) if (half & (1 << r)) written.add(r);
      } else if ((half & 0xf800) === 0xc800) {
        for (let r = 0; r < 8; r++) if (half & (1 << r)) written.add(r);
      }
      for (const register of written) constants.delete(register);

      if ((half & 0xf800) === 0x4800) {
        const register = (half >>> 8) & 7;
        const literal = ((pc + 4) & ~3) + ((half & 0xff) << 2);
        if (this.inside(literal, 4)) {
          this.literal_slots.add(literal);
          const value = this.u32(literal);
          constants.set(register, value);
          if (this.inside(value & ~1, 2)) this.data_refs.add(value & ~1);
        }
      } else if ((half & 0xf800) === 0xa000) {
        constants.set((half >>> 8) & 7, ((pc + 4) & ~3) + ((half & 0xff) << 2));
      } else if ((half & 0xf800) === 0x6800) {
        const destination = half & 7;
        const baseRegister = (half >>> 3) & 7;
        let address = constants.get(baseRegister);
        if (address !== undefined) {
          address += ((half >>> 6) & 0x1f) << 2;
          if (this.inside(address, 4)) {
            const value = this.u32(address);
            constants.set(destination, value);
            if (this.inside(value & ~1, 2)) this.data_refs.add(value & ~1);
          }
        }
      } else if ((half & 0xfe00) === 0x5800) {
        const destination = half & 7;
        const base = constants.get((half >>> 3) & 7);
        const offset = constants.get((half >>> 6) & 7);
        if (base !== undefined && offset !== undefined) {
          const address = base + offset;
          if (this.inside(address, 4)) {
            const value = this.u32(address);
            constants.set(destination, value);
            if (this.inside(value & ~1, 2)) this.data_refs.add(value & ~1);
          }
        }
      }

      if ((half & 0xf800) === 0xf000 && this.inside(pc, 4)) {
        const low = this.u16(pc + 2);
        if ((low & 0xf800) === 0xf800) {
          const offset = sx(((half & 0x7ff) << 12) | ((low & 0x7ff) << 1), 23);
          const target = pc + 4 + offset;
          kind = "call";
          size = 4;
          this.call(fn, pc, target, "thumb");
          for (const register of [0, 1, 2, 3, 12, 14]) constants.delete(register);
        }
      } else if ((half & 0xf800) === 0xe000) {
        kind = "branch";
        successors.push(pc + 4 + (sx(half & 0x7ff, 11) << 1));
        stop = true;
      } else if ((half & 0xf000) === 0xd000) {
        const condition = (half >>> 8) & 0xf;
        if (condition < 0xe) {
          kind = "conditional";
          successors.push(pc + 4 + (sx(half & 0xff, 8) << 1), pc + 2);
          stop = true;
        } else if (condition === 0xf) kind = "software-interrupt";
      } else if ((half & 0xff87) === 0x4700) {
        const register = (half >>> 3) & 15;
        const previous = this.inside(pc - 2, 2) ? this.u16(pc - 2) : 0;
        const popped = register < 8 && (previous & 0xff00) === 0xbc00 && Boolean(previous & (1 << register));
        if (register === 14 || popped || (register === 12 && this.u16(fn.entry) === 0x46f4)) kind = "return";
        else {
          kind = "indirect";
          const value = constants.get(register);
          if (value !== undefined) this.call(fn, pc, value, value & 1 ? "thumb" : "arm");
          else {
            fn.unresolved.add(pc);
            this.unresolved.add(pc);
          }
        }
        stop = true;
      } else if ((half & 0xff00) === 0xbd00) {
        kind = "return";
        stop = true;
      } else if ((half & 0xfc00) === 0x4400 && (half & 0x0087) === 0x0087) {
        const register = (half >>> 3) & 15;
        const targets = this.thumb_jump_table(pc, register, constants);
        if (targets) {
          kind = "switch";
          successors.push(...targets);
        } else if (register === 12 && this.u16(fn.entry) === 0x46f4) kind = "return";
        else {
          kind = "indirect";
          fn.unresolved.add(pc);
          this.unresolved.add(pc);
        }
        stop = true;
      }

      if ((half & 0xfc00) === 0x4400 && ((half >>> 8) & 3) === 2) {
        const destination = (half & 7) | ((half >>> 4) & 8);
        const source = (half >>> 3) & 15;
        constants.delete(destination);
        if (constants.has(source)) constants.set(destination, constants.get(source)!);
      }
      if (!this.note_instruction(fn, pc, size, kind)) return;
      for (const target of successors) if (this.inside(target, 2)) queue.push([target, new Map(constants)]);
      if (stop) return;
      pc += size;
    }
  }

  note_jump_table(pc: number, table: number, count: number): number[] | null {
    if (count < 1 || count > 256 || (table & 3) !== 0 || Math.abs(table - pc) > 0x10000 || !this.inside(table, count * 4)) return null;
    const targets = Array.from({ length: count }, (_, index) => this.u32(table + index * 4));
    if (targets.some((target) => (target & 1) !== 0 || !this.inside(target, 2) || Math.abs(target - pc) > 0x10000)) return null;
    this.data_refs.add(table);
    this.jump_tables.set(table, targets);
    this.jump_table_sites.set(pc, table);
    return targets;
  }

  thumb_jump_table(pc: number, register: number, constants: Constants = new Map()): number[] | null {
    const table = this.jump_table_sites.get(pc);
    if (table !== undefined) return this.jump_tables.get(table)!;
    return this.thumb_jump_table_compact(pc, register) ?? this.thumb_jump_table_extended(pc, register, constants);
  }

  thumb_jump_table_compact(pc: number, register: number): number[] | null {
    if (!this.inside(pc - 12, 14)) return null;
    const setup: [number, number][] = [[pc - 6, this.u16(pc - 6)], [pc - 4, this.u16(pc - 4)]];
    const tableLoad = this.u16(pc - 2);
    const literals = setup.filter((item) => (item[1] & 0xf800) === 0x4800);
    const shifts = setup.filter((item) => (item[1] & 0xf800) === 0 && (item[1] & 7) === register && ((item[1] >>> 6) & 0x1f) === 2);
    if (literals.length !== 1 || shifts.length !== 1 || (tableLoad & 0xfe00) !== 0x5800 || (tableLoad & 7) !== register) return null;
    const [literalPc, literalLoad] = literals[0];
    const shift = shifts[0][1];
    const indexRegister = (shift >>> 3) & 7;
    const directCompare = this.u16(pc - 10);
    const directBranch = this.u16(pc - 8);
    const guardedCompare = this.u16(pc - 12);
    const guardedBranch = this.u16(pc - 10);
    const guardedDefault = this.u16(pc - 8);
    let compare: number;
    if ((directCompare & 0xf800) === 0x2800 && ((directCompare >>> 8) & 7) === indexRegister && (directBranch & 0xff00) === 0xd800) compare = directCompare;
    else if ((guardedCompare & 0xf800) === 0x2800 && ((guardedCompare >>> 8) & 7) === indexRegister && (guardedBranch & 0xff00) === 0xd900 && (guardedDefault & 0xf800) === 0xe000 && pc - 10 + 4 + (sx(guardedBranch & 0xff, 8) << 1) === pc - 6) compare = guardedCompare;
    else return null;
    const baseRegister = (literalLoad >>> 8) & 7;
    const loadRegisters = new Set([(tableLoad >>> 3) & 7, (tableLoad >>> 6) & 7]);
    if (loadRegisters.size !== 2 || !loadRegisters.has(register) || !loadRegisters.has(baseRegister)) return null;
    const literal = ((literalPc + 4) & ~3) + ((literalLoad & 0xff) << 2);
    if (!this.inside(literal, 4)) return null;
    return this.note_jump_table(pc, this.u32(literal), (compare & 0xff) + 1);
  }

  thumb_jump_table_extended(pc: number, register: number, constants: Constants): number[] | null {
    const dispatches: [number, number, number][] = [];
    for (let loadPc = pc - 2; loadPc > Math.max(pc - 42, this.base); loadPc -= 2) {
      const load = this.u16(loadPc);
      if ((load & 0xfe00) !== 0x5800 || (load & 7) !== register) continue;
      const addressRegisters = new Set([(load >>> 3) & 7, (load >>> 6) & 7]);
      for (let shiftPc = loadPc - 2; shiftPc > Math.max(loadPc - 10, this.base); shiftPc -= 2) {
        const shift = this.u16(shiftPc);
        if ((shift & 0xf800) !== 0 || ((shift >>> 6) & 0x1f) !== 2 || !addressRegisters.has(shift & 7)) continue;
        const indexRegister = (shift >>> 3) & 7;
        const shiftedRegister = shift & 7;
        for (const baseRegister of [...addressRegisters].filter((item) => item !== shiftedRegister)) {
          let table = constants.get(baseRegister);
          for (let literalPc = loadPc - 2; literalPc > Math.max(loadPc - 18, this.base); literalPc -= 2) {
            const literalLoad = this.u16(literalPc);
            if ((literalLoad & 0xf800) === 0x4800 && ((literalLoad >>> 8) & 7) === baseRegister) {
              const literal = ((literalPc + 4) & ~3) + ((literalLoad & 0xff) << 2);
              if (this.inside(literal, 4)) table = this.u32(literal);
              break;
            }
          }
          if (table !== undefined) dispatches.push([shiftPc, indexRegister, table]);
        }
      }
    }

    const load = this.u16(pc - 2);
    if ((load & 0xf800) === 0x6800 && (load & 7) === register && ((load >>> 3) & 7) === register && ((load >>> 6) & 0x1f) === 0) {
      const add = this.u16(pc - 4);
      const shift = this.u16(pc - 8);
      const literalLoad = this.u16(pc - 6);
      if ((add & 0xfe00) === 0x1800 && (add & 7) === register && (shift & 0xf800) === 0 && ((shift >>> 6) & 0x1f) === 2 && (shift & 7) === register && (literalLoad & 0xf800) === 0x4800) {
        const addRegisters = new Set([(add >>> 3) & 7, (add >>> 6) & 7]);
        const baseRegister = (literalLoad >>> 8) & 7;
        if (addRegisters.size === 2 && addRegisters.has(register) && addRegisters.has(baseRegister)) {
          const literal = ((pc - 6 + 4) & ~3) + ((literalLoad & 0xff) << 2);
          if (this.inside(literal, 4)) dispatches.push([pc - 8, (shift >>> 3) & 7, this.u32(literal)]);
        }
      }
    }

    for (const [shiftPc, , table] of dispatches) {
      for (let comparePc = shiftPc - 2; comparePc > Math.max(shiftPc - 50, this.base); comparePc -= 2) {
        const compare = this.u16(comparePc);
        if ((compare & 0xf800) !== 0x2800) continue;
        const branch = this.u16(comparePc + 2);
        if (![(0xd800), (0xd900)].includes(branch & 0xff00)) continue;
        const branchTarget = comparePc + 6 + (sx(branch & 0xff, 8) << 1);
        if ((branch & 0xff00) === 0xd900 && !(comparePc + 4 <= branchTarget && branchTarget <= shiftPc)) continue;
        const targets = this.note_jump_table(pc, table, (compare & 0xff) + 1);
        if (targets) return targets;
      }
    }
    return null;
  }

  walk_function(address: number): void {
    const fn = this.functions.get(address);
    if (!fn) throw new Error(`unknown function: ${hex(address)}`);
    const queue: Work[] = [[address, new Map()]];
    const seen = new Set<number>();
    for (let head = 0; head < queue.length; head++) {
      const [block, constants] = queue[head];
      if (seen.has(block)) continue;
      fn.blocks.add(block);
      if (fn.mode === "arm") this.walk_arm(block, fn, queue, seen, constants);
      else this.walk_thumb(block, fn, queue, seen, constants);
    }
  }

  private code_banks(): Set<number> {
    const counts = new Map<number, number>();
    for (const entry of this.functions.keys()) {
      const bank = Math.floor((entry - this.base) / 0x10000);
      counts.set(bank, (counts.get(bank) ?? 0) + 1);
    }
    return new Set([...counts].filter(([, count]) => count >= 5).map(([bank]) => bank));
  }

  discover_global_pointer_tables(): boolean {
    const codeBanks = this.code_banks();
    const plausible = (address: number): boolean => {
      const half = this.u16(address);
      return (half & 0xff00) === 0xb500 || half === 0x4770 || (half & 0xf800) === 0x2000;
    };
    let added = false;
    let offset = 0;
    while (offset + 8 <= this.data.length) {
      if (!codeBanks.has(Math.floor(offset / 0x10000))) {
        offset += 4;
        continue;
      }
      const targets: number[] = [];
      let cursor = offset;
      while (cursor + 4 <= this.data.length && targets.length < 1024) {
        const value = this.data.readUInt32LE(cursor);
        const target = value & ~1;
        const targetBank = Math.floor((target - this.base) / 0x10000);
        if (!(value & 1) || !this.inside(target, 2) || !codeBanks.has(targetBank) || !plausible(target)) break;
        targets.push(target);
        cursor += 4;
      }
      if (targets.length < 2) {
        offset += 4;
        continue;
      }
      const table = this.base + offset;
      this.pointer_tables.set(table, targets);
      for (const target of targets) if (!this.instructions.has(target)) added = this.add_seed(target, "thumb", `global-pointer-table:${hex(table)}`) || added;
      offset = cursor;
    }
    return added;
  }

  discover_individual_function_pointers(): boolean {
    const codeBanks = this.code_banks();
    let added = false;
    for (let offset = 0; offset < this.data.length - 3; offset += 4) {
      if (!codeBanks.has(Math.floor(offset / 0x10000))) continue;
      const value = this.data.readUInt32LE(offset);
      const target = value & ~1;
      if (!(value & 1) || !this.inside(target, 2) || !codeBanks.has(Math.floor((target - this.base) / 0x10000)) || (this.u16(target) & 0xff00) !== 0xb500 || this.instructions.has(target)) continue;
      added = this.add_seed(target, "thumb", `function-pointer:${hex(this.base + offset)}`) || added;
    }
    return added;
  }

  discover_prologue_boundaries(): boolean {
    const codeBanks = this.code_banks();
    let added = false;
    for (const bank of codeBanks) {
      const start = this.base + bank * 0x10000;
      const end = Math.min(start + 0x10000, this.limit);
      for (let address = start + 2; address < end; address += 2) {
        if (this.instructions.has(address) || this.literal_slots.has(address) || (this.u16(address) & 0xff00) !== 0xb500) continue;
        if (!this.inside(address + 2, 2) || [0, 0xffff].includes(this.u16(address + 2))) continue;
        const previous = this.u16(address - 2);
        if ([0, 0xffff].includes(previous) || previous === 0x4770 || (previous & 0xff00) === 0xbd00) {
          added = this.add_seed(address, "thumb", `prologue-boundary:${hex(address)}`) || added;
        }
      }
    }
    return added;
  }

  reset_flow(): void {
    this.functions = new Map();
    this.instructions = new Map();
    this.calls = new CallSet();
    this.external_calls = new CallSet();
    this.unresolved = new Set();
    this.conflicts = [];
    this.data_refs = new Set();
    this.literal_slots = new Set();
    this.pointer_tables = new Map();
    this.jump_tables = new Map();
    this.jump_table_sites = new Map();
  }

  block_structural_overlaps(): boolean {
    const blocked = new Set<number>();
    for (const address of this.literal_slots) {
      if (this.instructions.has(address) || this.instructions.has(address + 2)) {
        blocked.add(address);
        blocked.add(address + 2);
      }
    }
    for (const [address, targets] of this.jump_tables) {
      for (let item = address; item < address + 4 * targets.length; item += 2) {
        if (this.instructions.has(item)) {
          for (let value = address; value < address + 4 * targets.length; value += 2) blocked.add(value);
          break;
        }
      }
    }
    let added = false;
    for (const address of blocked) if (!this.blocked_data.has(address)) {
      this.blocked_data.add(address);
      added = true;
    }
    return added;
  }

  discover_once(): number {
    const entry = this.initial_seeds();
    const walked = new Set<number>();
    while (true) {
      const pending = numeric([...this.functions.keys()].filter((address) => !walked.has(address)));
      for (let head = 0; head < pending.length; head++) {
        const address = pending[head];
        if (walked.has(address)) continue;
        this.walk_function(address);
        walked.add(address);
        for (const discovered of numeric([...this.functions.keys()].filter((item) => !walked.has(item)))) {
          if (!pending.includes(discovered, head + 1)) pending.push(discovered);
        }
      }
      let added = false;
      for (const table of numeric(this.data_refs)) {
        if ((table & 3) !== 0 || !this.inside(table, 8)) continue;
        const targets: number[] = [];
        let cursor = table;
        while (this.inside(cursor, 4) && targets.length < 1024) {
          const value = this.u32(cursor);
          if (!(value & 1) || !this.inside(value & ~1, 2)) break;
          targets.push(value & ~1);
          cursor += 4;
        }
        if (targets.length < 2) continue;
        this.pointer_tables.set(table, targets);
        for (const target of targets) added = this.add_seed(target, "thumb", `pointer-table:${hex(table)}`) || added;
      }
      added = this.discover_global_pointer_tables() || added;
      added = this.discover_individual_function_pointers() || added;
      added = this.discover_prologue_boundaries() || added;
      if (!added) break;
    }
    return entry;
  }

  run(): number {
    let entry: number;
    while (true) {
      entry = this.discover_once();
      if (!this.block_structural_overlaps()) break;
      this.reset_flow();
    }
    for (const site of this.jump_table_sites.keys()) this.unresolved.delete(site);
    for (const fn of this.functions.values()) for (const site of this.jump_table_sites.keys()) fn.unresolved.delete(site);
    return entry;
  }

  report(entry: number, details = false): Record<string, unknown> {
    const functions = [...this.functions].sort((a, b) => a[0] - b[0]).map(([address, fn]) => {
      const insns = numeric(fn.instructions);
      return {
        entry: address,
        mode: fn.mode,
        sources: [...fn.sources].sort(),
        blocks: fn.blocks.size,
        instruction_count: insns.length,
        min_address: insns.length ? insns[0] : null,
        max_address: insns.length ? insns.at(-1)! + this.instructions.get(insns.at(-1)!)!.size : null,
        callees: numeric(fn.callees),
        external_callees: numeric(fn.external_callees),
        unresolved: numeric(fn.unresolved),
      };
    });
    const report: Record<string, unknown> = {
      rom_base: this.base,
      rom_size: this.data.length,
      reset_entry: entry,
      function_count: functions.length,
      instruction_count: this.instructions.size,
      call_count: this.calls.size,
      external_call_count: this.external_calls.size,
      unresolved_count: this.unresolved.size,
      pointer_table_count: this.pointer_tables.size,
      jump_table_count: this.jump_tables.size,
      conflicts: this.conflicts,
      functions,
    };
    if (details) {
      report.instructions = [...this.instructions].sort((a, b) => a[0] - b[0]).map(([address, instruction]) => ({ address, ...instruction }));
      report.calls = [...this.calls].sort((a, b) => a[0] - b[0] || a[1] - b[1] || a[2].localeCompare(b[2])).map(([source, target, mode]) => ({ source, target, mode }));
      report.external_calls = [...this.external_calls].sort((a, b) => a[0] - b[0] || a[1] - b[1] || a[2].localeCompare(b[2])).map(([source, target, mode]) => ({ source, target, mode }));
      report.unresolved = numeric(this.unresolved);
      report.data_refs = numeric(this.data_refs);
      report.literal_slots = numeric(this.literal_slots);
      report.pointer_tables = [...this.pointer_tables].sort((a, b) => a[0] - b[0]).map(([address, targets]) => ({ address, targets }));
      report.jump_tables = [...this.jump_tables].sort((a, b) => a[0] - b[0]).map(([address, targets]) => ({
        address,
        targets,
        sites: numeric([...this.jump_table_sites].filter(([, table]) => table === address).map(([site]) => site)),
      }));
    }
    return report;
  }
}

function usage(): never {
  console.log("usage: discover.ts [-h] -o OUTPUT [--details] rom");
  console.log("\npositional arguments:\n  rom");
  console.log("\noptions:\n  -h, --help            show this help message and exit\n  -o OUTPUT, --output OUTPUT\n  --details");
  process.exit(0);
}

export function main(argv = process.argv.slice(2)): void {
  if (argv.includes("-h") || argv.includes("--help")) usage();
  let output: string | undefined;
  let details = false;
  let rom: string | undefined;
  for (let index = 0; index < argv.length; index++) {
    const arg = argv[index];
    if (arg === "-o" || arg === "--output") output = argv[++index];
    else if (arg === "--details") details = true;
    else if (!arg.startsWith("-")) rom = arg;
    else throw new Error(`unrecognized arguments: ${arg}`);
  }
  if (!rom || !output) throw new Error("the following arguments are required: rom, -o/--output");
  const discovery = new Discovery(readFileSync(rom));
  const entry = discovery.run();
  const report = discovery.report(entry, details) as Record<string, any>;
  writeFileSync(output, `${JSON.stringify(report, null, 2)}\n`);
  console.log(`functions=${report.function_count} instructions=${report.instruction_count} calls=${report.call_count} external_calls=${report.external_call_count} unresolved=${report.unresolved_count} jump_tables=${report.jump_table_count} conflicts=${report.conflicts.length}`);
}

if (import.meta.main) main();
