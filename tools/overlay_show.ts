#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md.
// Print one overlay function's reconstruction disassembly together with the
// pool words its pc-relative loads resolve to, which is the pair of facts a
// semantics-first draft needs and which otherwise takes two tools and a manual
// address computation per constant.
//
// Resolving the pool is the point. A Thumb `ldr rN,[pc,#k]` names its word by a
// rounded, biased offset, so reading a candidate's constants by hand is where
// the per-function time went during the 2026-07-25 overlay session; the laws in
// LAWS.md then say how to spell each one (symbols for pool words, immediates
// for values the reference materialises with `movs`).
//
// Prints decoded instructions and resolved constants, never a raw ROM span.
import { mkdirSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { assembleOverlay, OVERLAY_BASE } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const PC_LOAD_MASK = 0xf800;
const PC_LOAD = 0x4800;
const RETURN_MASK = 0xff87;
const RETURN = 0x4700;

interface Options { overlay: string; offset: number; length: number }

function optionsOf(argv: string[]): Options {
  const rest: string[] = [];
  const options: Options = { overlay: "", offset: -1, length: 0 };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--length" || argument === "-n") options.length = Number(argv[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: overlay_show.ts <overlay> <offsetHex> [-n BYTES]");
      process.exit(0);
    } else rest.push(argument);
  }
  const [overlay, offsetText] = rest;
  if (overlay === undefined || offsetText === undefined) {
    throw new Error("usage: overlay_show.ts <overlay> <offsetHex> [-n BYTES]");
  }
  options.overlay = overlay;
  options.offset = Number.parseInt(offsetText, 16);
  if (!Number.isInteger(options.offset) || options.offset < 0) throw new Error("offset must be hexadecimal");
  return options;
}

// pc相対ロードの参照先。Thumbでは (PC+4) を4境界へ丸めた上に変位×4。
export function poolTarget(insnOffset: number, half: number): number {
  return ((insnOffset + 4) & ~3) + (half & 0xff) * 4;
}

// 戻りまでを走査して関数の広がりを推し量る。プール語はその後ろに続くので、
// 参照された最遠のプール語までを既定の表示長にする。
function extentOf(data: Buffer, offset: number): number {
  let end = offset + 2;
  let furthest = offset;
  for (let at = offset; at + 2 <= Math.min(offset + 512, data.length); at += 2) {
    const half = data.readUInt16LE(at);
    if ((half & PC_LOAD_MASK) === PC_LOAD) furthest = Math.max(furthest, poolTarget(at, half) + 4);
    end = at + 2;
    if ((half & RETURN_MASK) === RETURN) break;
  }
  return Math.max(end, furthest) - offset;
}

function selfTest(): void {
  // `ldr r0,[pc,#0]` at offset 0 resolves to offset 4; at offset 2 it rounds
  // the biased pc down to 4 and so resolves to 4 as well.
  if (poolTarget(0, 0x4800) !== 4) throw new Error("overlay show self-test: pool target at 0");
  if (poolTarget(2, 0x4800) !== 4) throw new Error("overlay show self-test: pool target rounding");
  if (poolTarget(0, 0x4801) !== 8) throw new Error("overlay show self-test: pool target displacement");
  console.log("overlay show self-test passed");
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) {
    selfTest();
    return;
  }
  const options = optionsOf(argv);
  const data = assembleOverlay(join(ROOT, "assets/code", `${options.overlay}_overlay.s`), OVERLAY_BASE);
  const length = options.length > 0 ? options.length : extentOf(data, options.offset);
  const work = join(ROOT, "work/overlay-show");
  mkdirSync(work, { recursive: true });
  const binary = join(work, `${options.overlay}_${options.offset.toString(16)}.bin`);
  writeFileSync(binary, data.subarray(options.offset, options.offset + length));

  const dumped = Bun.spawnSync([
    "arm-none-eabi-objdump", "-D", "-b", "binary", "-m", "arm", "-M", "force-thumb",
    `--adjust-vma=0x${(OVERLAY_BASE + options.offset).toString(16)}`, binary,
  ], { stdout: "pipe", stderr: "pipe" });
  if (dumped.exitCode !== 0) throw new Error(`objdump failed: ${dumped.stderr.toString().trim()}`);

  const pools = new Map<number, number>();
  for (const line of dumped.stdout.toString().split("\n")) {
    if (/^\s+[0-9a-f]+:/.test(line)) console.log(line.replace(/\s+$/, ""));
  }
  for (let at = options.offset; at + 2 <= options.offset + length; at += 2) {
    const half = data.readUInt16LE(at);
    if ((half & PC_LOAD_MASK) !== PC_LOAD) continue;
    const target = poolTarget(at, half);
    if (target + 4 <= data.length) pools.set(target, data.readUInt32LE(target));
  }
  if (pools.size > 0) {
    console.log("--- pool words referenced:");
    for (const [at, value] of [...pools].sort((left, right) => left[0] - right[0])) {
      console.log(`  0x${(OVERLAY_BASE + at).toString(16)} = 0x${value.toString(16).padStart(8, "0")}`);
    }
  }
}

if (import.meta.main) main();
