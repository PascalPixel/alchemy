#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md.
// Compile one main-image candidate and print its disassembly beside the
// reference's, aligned by offset, marking every halfword that differs.
//
// Why this exists: promote_candidate.ts reports a single score, which says a
// draft got worse but never says where. Hand-drafting against a score alone is
// guessing -- two attempts on 08006408 and one on 08004760 each scored worse
// than the stored baseline with no indication of which statement caused it.
// tools/overlay_show.ts already gives the reference side for overlays; this
// gives both sides for the main image, which is what a semantics-first draft
// needs in order to converge instead of oscillate.
//
// Prints decoded instructions, never a raw ROM span.
import { mkdirSync, readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { verifyCandidate, ROM_BASE } from "./match_m2c.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface Options { source: string; rom: string; work: string }

function optionsOf(argv: string[]): Options {
  const options: Options = {
    source: "",
    rom: join(ROOT, "roms/gs1-en.gba"),
    work: join(ROOT, "work/candidate-show"),
  };
  const rest: string[] = [];
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--rom") options.rom = argv[++index];
    else if (argument === "--work") options.work = argv[++index];
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: candidate_show.ts <candidate.c> [--rom FILE]");
      process.exit(0);
    } else rest.push(argument);
  }
  if (rest.length !== 1) throw new Error("usage: candidate_show.ts <candidate.c> [--rom FILE]");
  options.source = rest[0];
  return options;
}

// 逆アセンブルを「番地 -> 表記」の対にほどく。並べる際の照合鍵は番地ではなく
// 先頭からの変位にする。候補と参照は同じ番地に置かれるとは限らないため。
function disassemble(binary: string, base: number): Map<number, string> {
  const dumped = Bun.spawnSync([
    "arm-none-eabi-objdump", "-D", "-b", "binary", "-m", "arm", "-M", "force-thumb",
    `--adjust-vma=0x${base.toString(16)}`, binary,
  ], { stdout: "pipe", stderr: "pipe" });
  if (dumped.exitCode !== 0) throw new Error(`objdump failed: ${dumped.stderr.toString().trim()}`);
  const rows = new Map<number, string>();
  for (const line of dumped.stdout.toString().split("\n")) {
    const match = /^\s+([0-9a-f]+):\t([0-9a-f ]+)\t(.*)$/.exec(line);
    if (match === null) continue;
    rows.set(Number.parseInt(match[1], 16) - base, match[3].replace(/\s+$/, ""));
  }
  return rows;
}

export function differingOffsets(actual: Buffer, expected: Buffer): Set<number> {
  const differing = new Set<number>();
  const shared = Math.min(actual.length, expected.length);
  for (let at = 0; at + 2 <= shared; at += 2) {
    if (actual.readUInt16LE(at) !== expected.readUInt16LE(at)) differing.add(at);
  }
  for (let at = shared & ~1; at < Math.max(actual.length, expected.length); at += 2) {
    differing.add(at);
  }
  return differing;
}

function selfTest(): void {
  const same = Buffer.from([0x00, 0xb5, 0x01, 0x20]);
  if (differingOffsets(same, Buffer.from(same)).size !== 0) {
    throw new Error("candidate show self-test: identical buffers must not differ");
  }
  const other = Buffer.from([0x00, 0xb5, 0x02, 0x20]);
  const found = differingOffsets(same, other);
  if (found.size !== 1 || !found.has(2)) {
    throw new Error("candidate show self-test: wrong differing offset");
  }
  // 長さが違うときは、短い側を越えた分をすべて差分として数える。
  if (differingOffsets(same, Buffer.from([0x00, 0xb5])).size !== 1) {
    throw new Error("candidate show self-test: trailing bytes must count as differing");
  }
  console.log("candidate show self-test passed");
}

async function main(): Promise<void> {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) {
    selfTest();
    return;
  }
  const options = optionsOf(argv);
  const rom = readFileSync(options.rom);
  mkdirSync(options.work, { recursive: true });
  const verification = await verifyCandidate(options.source, rom, options.work, [], ROM_BASE);

  const mine = join(options.work, "candidate.bin");
  const theirs = join(options.work, "reference.bin");
  await Bun.write(mine, verification.actual);
  await Bun.write(theirs, verification.expected);

  const left = disassemble(mine, 0);
  const right = disassemble(theirs, 0);
  const differing = differingOffsets(verification.actual, verification.expected);
  const offsets = [...new Set([...left.keys(), ...right.keys()])].sort((a, b) => a - b);

  console.log(`candidate=${verification.actual.length} reference=${verification.expected.length} differing_halfwords=${differing.size}`);
  console.log("      offset  candidate                      reference");
  for (const offset of offsets) {
    const mark = differing.has(offset) ? "!" : " ";
    const a = (left.get(offset) ?? "").padEnd(30).slice(0, 30);
    const b = right.get(offset) ?? "";
    console.log(`  ${mark} ${offset.toString(16).padStart(4, "0")}  ${a} ${b}`);
  }
}

if (import.meta.main) await main();
