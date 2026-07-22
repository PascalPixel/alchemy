// Tool role: both; imported by tools/permute_v1.ts; invoked by tools/decomp_overnight.ts.
// Compiler-aware source constraints used by permute_v1. Each mutation preserves
// the likely intent or ABI search space while changing type inference, live ranges,
// pointer scheduling, or expression lowering.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

export type ConstraintOperator = (body: string, random: () => number) => string | null;

export interface AssemblyConstraints {
  stem: string;
  instructions: number;
  inferred_arguments: number;
  calls: number;
  memory: { byte: number; halfword: number; word: number; signed: number };
  conditions: { signed: number; unsigned: number; equality: number };
  distinct_offsets: number;
  longest_register_span: number;
  stack_argument_accesses: number;
  suggested_operators: string[];
}

export function inferAssemblyConstraints(stem: string, source: string): AssemblyConstraints {
  const rows: Array<{ mnemonic: string; operands: string }> = [];
  let started = false;
  for (const raw of source.split(/\r?\n/)) {
    const line = raw.replace(/@.*$/, "").trim();
    if (line === `Func_${stem}:`) { started = true; continue; }
    if (!started || line === "" || line.startsWith(".") || line.endsWith(":")) continue;
    const match = line.match(/^(\S+)\s*(.*)$/);
    if (match) rows.push({ mnemonic: match[1].toLowerCase().replace(/\.(?:n|w)$/, ""), operands: match[2] });
  }
  const written = new Set<string>();
  let maximumArgument = -1, calls = 0, stack = 0;
  const memory = { byte: 0, halfword: 0, word: 0, signed: 0 };
  const conditions = { signed: 0, unsigned: 0, equality: 0 };
  const offsets = new Set<number>();
  const first = new Map<string, number>(), last = new Map<string, number>();
  const writesFirst = /^(?:movs?|mvns?|adds?|adcs?|subs?|sbcs?|rsbs?|ands?|orrs?|eors?|bics?|lsls?|lsrs?|asrs?|rors?|negs?|muls?|ldr|ldrb|ldrh|ldrsb|ldrsh)$/;
  rows.forEach((row, index) => {
    if (/^blx?$/.test(row.mnemonic)) calls++;
    if (/^(?:ldr|str)/.test(row.mnemonic)) {
      if (/b$/.test(row.mnemonic)) memory.byte++;
      else if (/h$/.test(row.mnemonic)) memory.halfword++;
      else memory.word++;
      if (/s[bh]$/.test(row.mnemonic)) memory.signed++;
      if (/\[sp(?:,|\])/.test(row.operands)) stack++;
      const immediate = /#(0x[0-9a-f]+|\d+)/i.exec(row.operands);
      if (immediate) offsets.add(Number(immediate[1]));
    }
    if (/^b(?:eq|ne)$/.test(row.mnemonic)) conditions.equality++;
    else if (/^b(?:gt|lt|ge|le|mi|pl)$/.test(row.mnemonic)) conditions.signed++;
    else if (/^b(?:hi|ls|cs|cc|hs|lo)$/.test(row.mnemonic)) conditions.unsigned++;
    const registers = [...row.operands.matchAll(/\b(r(?:1[0-2]|[0-9])|sp|lr|pc|ip|fp|sl)\b/g)].map((match) => match[1]);
    const destination = writesFirst.test(row.mnemonic) ? registers[0] : undefined;
    registers.forEach((register, position) => {
      if (!first.has(register)) first.set(register, index);
      last.set(register, index);
      if (/^r[0-3]$/.test(register) && !(position === 0 && destination === register) && !written.has(register)) {
        maximumArgument = Math.max(maximumArgument, Number(register[1]));
      }
    });
    if (destination) written.add(destination);
  });
  const longest = Math.max(0, ...[...first].map(([register, at]) => (last.get(register) ?? at) - at));
  const suggestions = new Set<string>();
  if (memory.signed > 0) suggestions.add("fieldtype");
  if (offsets.size > 0) { suggestions.add("fieldsyntax"); suggestions.add("splitload"); }
  if (stack > 0 || maximumArgument >= 3) suggestions.add("argshift");
  if (calls > 0) suggestions.add("signature");
  if (longest >= 8) { suggestions.add("declshuffle"); suggestions.add("splitload"); suggestions.add("postincrement"); }
  if (conditions.signed + conditions.unsigned + conditions.equality > 0) suggestions.add("condinvert");
  return {
    stem, instructions: rows.length, inferred_arguments: maximumArgument + 1, calls, memory, conditions,
    distinct_offsets: offsets.size, longest_register_span: longest, stack_argument_accesses: stack,
    suggested_operators: [...suggestions],
  };
}

let localCounter = 0;

function pick<T>(items: T[], random: () => number): T | null {
  return items.length === 0 ? null : items[Math.floor(random() * items.length)];
}

function signatureType(body: string, random: () => number): string | null {
  const pattern = /\b(s8|u8|s16|u16|s32|u32|void \*)\s+(Func_08[0-9a-f]{6})\s*\(([^)]*)\)/;
  const match = pattern.exec(body);
  if (!match) return null;
  const slots: Array<{ text: string; type: string }> = [{ text: match[1], type: match[1] }];
  for (const parameter of match[3].split(",")) {
    const typed = parameter.trim().match(/^(s8|u8|s16|u16|s32|u32|void \*)\b/);
    if (typed) slots.push({ text: typed[0], type: typed[1] });
  }
  const slot = pick(slots, random);
  if (!slot) return null;
  const families: Record<string, string[]> = {
    s8: ["u8", "s32"], u8: ["s8", "u32"], s16: ["u16", "s32"], u16: ["s16", "u32"],
    s32: ["u32", "s16", "void *"], u32: ["s32", "u16", "void *"], "void *": ["s32", "u32"],
  };
  const replacement = pick(families[slot.type] ?? [], random);
  if (!replacement) return null;
  const offset = slot === slots[0] ? match.index : body.indexOf(slot.text, match.index + match[1].length);
  return body.slice(0, offset) + replacement + body.slice(offset + slot.text.length);
}

function unusedLeadingArgument(body: string, random: () => number): string | null {
  const pattern = /(Func_08[0-9a-f]{6}\s*\()([^)]*)(\)\s*\{)/;
  const match = pattern.exec(body);
  if (!match) return null;
  if (/\b_v1arg\b/.test(match[2])) {
    return body.replace(pattern, (_, open, parameters, close) =>
      open + parameters.replace(/^\s*s32 _v1arg\s*,?\s*/, "") .replace(/^\s*$/, "void") + close);
  }
  const parameters = match[2].trim();
  const count = parameters === "" || parameters === "void" ? 0 : parameters.split(",").length;
  if (count >= 4) return null;
  const tail = count === 0 ? "" : `, ${parameters}`;
  return body.replace(pattern, `$1s32 _v1arg${tail}$3`);
}

function pointerStep(body: string, random: () => number): string | null {
  const forms: Array<[RegExp, string]> = [
    [/\b(\w+) \+= 1;/, "$1++;"], [/\b(\w+)\+\+;/, "$1 = $1 + 1;"], [/\b(\w+) = \1 \+ 1;/, "$1 += 1;"],
  ];
  const usable = forms.filter(([pattern]) => pattern.test(body));
  const operation = pick(usable, random);
  return operation ? body.replace(operation[0], operation[1]) : null;
}

function fusePostIncrement(body: string, random: () => number): string | null {
  const fuse = /(\w+) = \*(\w+);\n(\s*)\2(?: \+= 1|\+\+| = \2 \+ 1);/;
  const match = fuse.exec(body);
  if (match) return body.replace(fuse, `${match[1]} = *${match[2]}++;`);
  const split = /(\w+) = \*(\w+)\+\+;/;
  const reverse = split.exec(body);
  if (!reverse) return null;
  return body.replace(split, `${reverse[1]} = *${reverse[2]};\n    ${reverse[2]} += 1;`);
}

function volatileConstraint(body: string, random: () => number): string | null {
  const declarations = [...body.matchAll(/^(\s+)(volatile )?(s8|u8|s16|u16|s32|u32|void \*+)\s+(\w+)\s*;/gm)];
  const target = pick(declarations, random);
  if (!target) return null;
  const replacement = `${target[1]}${target[2] ? "" : "volatile "}${target[3]} ${target[4]};`;
  return body.slice(0, target.index) + replacement + body.slice(target.index! + target[0].length);
}

function fieldTypeConstraint(body: string, random: () => number): string | null {
  const matches = [...body.matchAll(/M2C_FIELD\(([^,()]+),\s*(s8|u8|s16|u16|s32|u32)(\s*\*?),\s*(0x[0-9a-fA-F]+|\d+)\)/g)];
  const target = pick(matches, random);
  if (!target) return null;
  const flips: Record<string, string[]> = { s8: ["u8"], u8: ["s8"], s16: ["u16"], u16: ["s16"], s32: ["u32"], u32: ["s32"] };
  const type = pick(flips[target[2]], random)!;
  const replacement = `M2C_FIELD(${target[1]}, ${type}${target[3]}, ${target[4]})`;
  return body.slice(0, target.index) + replacement + body.slice(target.index! + target[0].length);
}

function fieldSyntaxConstraint(body: string, random: () => number): string | null {
  const pattern = /M2C_FIELD\(([^,()]+),\s*(s8|u8|s16|u16|s32|u32)\s*\*,\s*(0x[0-9a-fA-F]+|\d+)\)/g;
  const matches = [...body.matchAll(pattern)];
  const target = pick(matches, random);
  if (!target) return null;
  const replacement = `*(${target[2]} *)((u8 *)(${target[1]}) + ${target[3]})`;
  return body.slice(0, target.index) + replacement + body.slice(target.index! + target[0].length);
}

function splitMemoryLoad(body: string, random: () => number): string | null {
  const pattern = /^(\s+)(\w+) = (M2C_FIELD\([^;]+\)|\*\([^;]+\));$/gm;
  const matches = [...body.matchAll(pattern)];
  const target = pick(matches, random);
  if (!target) return null;
  const name = `_v1live${localCounter++ % 97}`;
  const declared = body.replace(/(Func_08[0-9a-f]{6}\s*\([^)]*\)\s*\{\n)/, `$1    s32 ${name};\n`);
  return declared.replace(target[0], `${target[1]}${name} = (s32)(${target[3]});\n${target[1]}${target[2]} = ${name};`);
}

function roundingConstraint(body: string, random: () => number): string | null {
  const shift = /\((\w+) \+ 3\) >> 2/;
  const match = shift.exec(body);
  if (match) return body.replace(shift, `((${match[1]} + 3) / 4)`);
  const divide = /\(\((\w+) \+ 3\) \/ 4\)/;
  const reverse = divide.exec(body);
  return reverse ? body.replace(divide, `(${reverse[1]} + 3) >> 2`) : null;
}

export const CONSTRAINT_OPERATORS: Array<[string, ConstraintOperator]> = [
  ["signature", signatureType], ["argshift", unusedLeadingArgument], ["pointerstep", pointerStep],
  ["postincrement", fusePostIncrement], ["volatile", volatileConstraint], ["fieldtype", fieldTypeConstraint],
  ["fieldsyntax", fieldSyntaxConstraint], ["splitload", splitMemoryLoad], ["rounding", roundingConstraint],
];

function main(): void {
  const arguments_ = Bun.argv.slice(2);
  if (arguments_.includes("--self-test")) {
    const sample = `Func_08000000:\n\tmov r3, r1\n\tldrsh r2, [r0, r3]\n\tcmp r2, #0\n\tblt .L0\n\tbl Func_08000100\n.L0:\n\tbx lr\n`;
    const result = inferAssemblyConstraints("08000000", sample);
    if (result.inferred_arguments !== 2 || result.memory.signed !== 1 || result.conditions.signed !== 1 || !result.suggested_operators.includes("fieldtype")) {
      throw new Error("assembly constraint self-test failed");
    }
    console.log("decomp constraint self-test passed");
    return;
  }
  const stems = arguments_.filter((argument) => /^[0-9a-f]{8}$/.test(argument));
  if (stems.length === 0) throw new Error("usage: decomp_constraints.ts STEM [STEM ...]");
  const output = join(ROOT, "out", "decomp", "constraints");
  mkdirSync(output, { recursive: true });
  for (const stem of stems) {
    const path = join(ROOT, "asm", `${stem}.s`);
    const result = inferAssemblyConstraints(stem, readFileSync(path, "utf8"));
    writeFileSync(join(output, `${stem}.json`), JSON.stringify(result, null, 2) + "\n");
    console.log(`${stem}: insns=${result.instructions} args=${result.inferred_arguments} calls=${result.calls} suggestions=${result.suggested_operators.join(",")}`);
  }
}

if (import.meta.main) main();
