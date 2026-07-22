// Tool role: library; imported by tools/decompile_batch.ts, tools/overlay_match.ts.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { Discovery, ROM_BASE, sx, type Instruction } from "./discover.ts";

function hex(value: number): string {
  return value.toString(16).padStart(8, "0");
}

export function branchTarget(data: Uint8Array, address: number, kind: string, base = ROM_BASE): number | null {
  const offset = address - base;
  const half = Buffer.from(data).readUInt16LE(offset);
  if (kind === "branch") return address + 4 + (sx(half & 0x7ff, 11) << 1);
  if (kind === "conditional") return address + 4 + (sx(half & 0xff, 8) << 1);
  return null;
}
export const branch_target = branchTarget;

export function disassemble(
  data: Uint8Array,
  start: number,
  end: number,
  instructions: Map<number, Instruction>,
  outputDir: string,
  base = ROM_BASE,
): Map<number, string> {
  const binary = join(outputDir, `${hex(start)}.bin`);
  const masked = Buffer.alloc(Math.ceil((end - start) / 2) * 2);
  for (let offset = 0; offset < masked.length; offset += 2) {
    masked[offset] = 0xc0;
    masked[offset + 1] = 0x46;
  }
  const sourceData = Buffer.from(data);
  for (const [address, instruction] of instructions) {
    const source = address - base;
    sourceData.copy(masked, address - start, source, source + instruction.size);
  }
  writeFileSync(binary, masked.subarray(0, end - start));
  const command = ["arm-none-eabi-objdump", "-D", "-b", "binary", "-marm", "-Mforce-thumb", `--adjust-vma=0x${hex(start)}`, binary];
  const process = Bun.spawnSync(command, { stdout: "pipe", stderr: "pipe" });
  if (process.exitCode !== 0) throw new Error(process.stderr.toString().trim());
  const result = new Map<number, string>();
  const pattern = /^\s*([0-9a-f]+):\s+(?:[0-9a-f]{4}\s+)+(.*)$/;
  for (const line of process.stdout.toString().split(/\r?\n/)) {
    const match = pattern.exec(line);
    if (match) result.set(Number.parseInt(match[1], 16), match[2].trim());
  }
  return result;
}

export function emitDiscovery(rom: Uint8Array, discovery: Discovery, address: number, output: string): void {
  const fn = discovery.functions.get(address);
  if (!fn) throw new Error(`unknown function: ${hex(address)}`);
  if (fn.mode !== "thumb") throw new Error("only Thumb functions are currently supported");
  const instructions = [...fn.instructions].sort((a, b) => a - b);
  const start = instructions[0];
  const end = Math.max(...instructions.map((item) => item + discovery.instructions.get(item)!.size));
  const outputDir = dirname(output);
  mkdirSync(outputDir, { recursive: true });
  const functionInstructions = new Map(instructions.map((item) => [item, discovery.instructions.get(item)!]));
  const decoded = disassemble(rom, start, end, functionInstructions, outputDir, discovery.base);
  const calls = new Map<number, number>();
  for (const [source, target, mode] of discovery.calls) if (fn.instructions.has(source) && mode === "thumb") calls.set(source, target);
  const jumpTables = new Map<number, number[]>();
  for (const [site, table] of discovery.jump_table_sites) if (fn.instructions.has(site)) jumpTables.set(table, discovery.jump_tables.get(table)!);
  const labels = new Set<number>();
  for (const item of instructions) {
    const target = branchTarget(rom, item, discovery.instructions.get(item)!.kind, discovery.base);
    if (target !== null && fn.instructions.has(target)) labels.add(target);
  }
  for (const targets of jumpTables.values()) for (const target of targets) if (fn.instructions.has(target)) labels.add(target);
  const literals = new Map<number, number>();
  const lines = [".syntax unified", ".thumb", `glabel Func_${hex(address)}`];
  for (const item of instructions) {
    if (labels.has(item)) lines.push(`.L_${hex(item)}:`);
    let instruction = decoded.get(item)!.split(";", 1)[0].trim();
    const kind = discovery.instructions.get(item)!.kind;
    if (kind === "call" && calls.has(item)) instruction = `${instruction.split(/\s+/, 1)[0]} Func_${hex(calls.get(item)!)}`;
    else if (kind === "branch" || kind === "conditional") {
      const target = branchTarget(rom, item, kind, discovery.base)!;
      if (fn.instructions.has(target)) instruction = `${instruction.split(/\s+/, 1)[0]} .L_${hex(target)}`;
    }
    const half = discovery.u16(item);
    if ((half & 0xf800) === 0x4800) {
      const register = (half >>> 8) & 7;
      const literal = ((item + 4) & ~3) + ((half & 0xff) << 2);
      literals.set(literal, discovery.u32(literal));
      instruction = `ldr r${register}, .L_${hex(literal)}`;
    }
    lines.push(`    ${instruction}`);
  }
  for (const [literal, value] of [...literals].sort((a, b) => a[0] - b[0])) {
    const word = jumpTables.has(value) ? `.L_${hex(value)}` : `0x${hex(value)}`;
    lines.push("    .align 2", `.L_${hex(literal)}:`, `    .word ${word}`);
  }
  for (const [table, targets] of [...jumpTables].sort((a, b) => a[0] - b[0])) {
    lines.push("    .align 2", `.L_${hex(table)}:`, ...targets.map((target) => `    .word .L_${hex(target)}`));
  }
  writeFileSync(output, `${lines.join("\n")}\n`);
}
export const emit_discovery = emitDiscovery;

export function emit(rom: Uint8Array, address: number, output: string): void {
  const discovery = new Discovery(rom);
  discovery.run();
  emitDiscovery(rom, discovery, address, output);
}

function usage(): never {
  console.log("usage: emit_function.ts [-h] -o OUTPUT rom address");
  process.exit(0);
}

export function main(argv = process.argv.slice(2)): void {
  if (argv.includes("-h") || argv.includes("--help")) usage();
  let output: string | undefined;
  const positional: string[] = [];
  for (let index = 0; index < argv.length; index++) {
    if (argv[index] === "-o" || argv[index] === "--output") output = argv[++index];
    else positional.push(argv[index]);
  }
  if (positional.length !== 2 || !output) throw new Error("the following arguments are required: rom, address, -o/--output");
  emit(readFileSync(positional[0]), Number.parseInt(positional[1], 0) & ~1, output);
}

if (import.meta.main) main();
