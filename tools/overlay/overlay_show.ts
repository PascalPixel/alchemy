#!/usr/bin/env bun
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
import { assembleOverlay, OVERLAY_BASE } from "../lib/overlay_disasm.ts";
import { annotate, resolvedCallNames, unannotatedCallSites } from "../lib/overlay_call_targets.ts";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));
const PC_LOAD_MASK = 0xf800;
const PC_LOAD = 0x4800;
const RETURN_MASK = 0xff87;
const RETURN = 0x4700;

interface Options { overlay: string; offset: number; length: number; annotate: boolean }

const USAGE = "usage: overlay_show.ts <overlay> <offsetHex> [endHex | -n BYTES] [--annotate]";

/** Accept either an image offset or its ordinary 0x02000000 RAM spelling. */
function imageOffset(value: number, label: string): number {
  const offset = value >= OVERLAY_BASE ? value - OVERLAY_BASE : value;
  if (!Number.isInteger(offset) || offset < 0) throw new Error(`${label} must be hexadecimal`);
  return offset;
}

/**
 * Parse the command line.
 *
 * An END bound is accepted as a second positional so this tool takes the same
 * `<overlay> <startHex> <endHex>` spelling as `overlay_call_targets.ts`. It
 * used to IGNORE a third argument and fall back to `extentOf`, which stops at
 * the first return-shaped halfword — so `overlay_show <ov> 1cd4 23e0` silently
 * printed 60% of an 1804-byte row, complete with the pool-word footer that
 * makes a truncated listing look like a finished function. Two resource_3b9
 * rows lost ~40% each that way, and one of them hid both its inner gates and
 * its epilogue. Anything unparseable now throws rather than being dropped: a
 * silently ignored bound is the entire bug.
 */
export function optionsOf(argv: string[]): Options {
  const rest: string[] = [];
  const options: Options = { overlay: "", offset: -1, length: 0, annotate: false };
  let explicitLength = false;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--length" || argument === "-n") {
      options.length = Number(argv[++index]);
      explicitLength = true;
    } else if (argument === "--annotate") {
      options.annotate = true;
    } else if (argument === "-h" || argument === "--help") {
      console.log(USAGE);
      process.exit(0);
    } else rest.push(argument);
  }
  const [overlay, offsetText, endText, ...extra] = rest;
  if (overlay === undefined || offsetText === undefined) throw new Error(USAGE);
  if (extra.length > 0) throw new Error(`${USAGE}\nunexpected argument: ${extra[0]}`);
  options.overlay = overlay;
  options.offset = imageOffset(Number.parseInt(offsetText, 16), "offset");
  if (endText !== undefined) {
    if (explicitLength) throw new Error("pass an end bound OR -n BYTES, not both");
    if (!/^(0x)?[0-9a-f]+$/i.test(endText)) throw new Error(`end bound must be hexadecimal: ${endText}`);
    const end = imageOffset(Number.parseInt(endText.replace(/^0x/i, ""), 16), "end bound");
    if (end <= options.offset) throw new Error("end bound must be greater than the start offset");
    options.length = end - options.offset;
  }
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

  // A second positional is an END bound, not something to drop. This is the
  // regression that matters: `resource_3b9 1cd4 23e0` used to ignore `23e0`,
  // fall back to extentOf, and print 60% of the row under a pool-word footer
  // that reads exactly like the end of a function.
  const bounded = optionsOf(["resource_3b9", "1cd4", "23e0"]);
  if (bounded.offset !== 0x1cd4) throw new Error("overlay show self-test: start bound");
  if (bounded.length !== 0x23e0 - 0x1cd4) throw new Error("overlay show self-test: end bound ignored");
  const prefixed = optionsOf(["resource_3b9", "1cd4", "0x23e0"]);
  if (prefixed.length !== 0x23e0 - 0x1cd4) throw new Error("overlay show self-test: 0x end bound");
  const absolute = optionsOf(["resource_3b9", "0x02001cd4", "0x020023e0", "--annotate"]);
  if (absolute.offset !== 0x1cd4 || absolute.length !== 0x23e0 - 0x1cd4 || !absolute.annotate)
    throw new Error("overlay show self-test: absolute address normalization or annotation flag");
  if (optionsOf(["resource_3b9", "1cd4", "-n", "1804"]).length !== 1804)
    throw new Error("overlay show self-test: -n must still work");
  if (optionsOf(["resource_3b9", "1cd4"]).length !== 0)
    throw new Error("overlay show self-test: no bound must defer to extentOf");

  // Every way of being ambiguous or wrong must THROW rather than be dropped.
  const rejects = (argv: string[], why: string): void => {
    let threw = false;
    try { optionsOf(argv); } catch { threw = true; }
    if (!threw) throw new Error(`overlay show self-test: ${why}`);
  };
  rejects(["resource_3b9", "1cd4", "23e0", "-n", "1804"], "end bound and -n together must be rejected");
  rejects(["resource_3b9", "1cd4", "1cd4"], "an end bound at the start must be rejected");
  rejects(["resource_3b9", "1cd4", "11ac"], "an end bound below the start must be rejected");
  rejects(["resource_3b9", "1cd4", "zzz"], "a non-hex end bound must be rejected");
  rejects(["resource_3b9", "1cd4", "23e0", "2400"], "a fourth positional must be rejected");

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
  let listing = dumped.stdout.toString().split("\n")
    .filter((line) => /^\s+[0-9a-f]+:/.test(line))
    .map((line) => line.replace(/\s+$/, ""))
    .join("\n");
  if (options.annotate) {
    const names = resolvedCallNames(options.overlay, options.offset, options.offset + length);
    const missed = unannotatedCallSites(listing, names);
    if (missed.length > 0) {
      throw new Error(
        `overlay show annotation missed ${missed.length} call site(s); explicit bounds do not cover the listing`,
      );
    }
    listing = annotate(listing, names);
  }
  for (const line of listing.split("\n")) {
    if (line.length > 0) console.log(line);
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
