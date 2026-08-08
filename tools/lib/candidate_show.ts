#!/usr/bin/env bun
// Compile one main-image candidate and print its disassembly beside the
// reference's, aligned by offset, marking every halfword that differs.
//
// Why this exists: promote_candidate.ts reports a single score, which says a
// draft got worse but never says where. Hand-drafting against a score alone is
// guessing -- two attempts on 08006408 and one on 08004760 each scored worse
// than the stored baseline with no indication of which statement caused it.
// tools/overlay/overlay_show.ts already gives the reference side for overlays; this
// gives both sides for the main image, which is what a semantics-first draft
// needs in order to converge instead of oscillate.
//
// Prints decoded instructions, never a raw ROM span.
//
// A candidate may define more than one function: some c_candidate regions hold
// a short leaf after the first function's constant pool, and the build accepts
// those because tools/lib/integrate_matches.ts measures the region across every
// compiled Func_ symbol. verifyCandidate stops at the entry symbol, so the
// side-by-side view spans the whole assembly region instead -- otherwise the
// trailing function is invisible here while it still decides the gate.
import { existsSync, mkdirSync, readFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import {
  verifyCandidate,
  ROM_BASE,
  type CandidateCompilerConfiguration,
  type CandidateCompilerFamily,
} from "./match_m2c.ts";
import { linkedFunctionExtent } from "./integrate_matches.ts";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));

// The assembly builder already fixed each region's extent; the entry symbol's own size
// is only the head of it when a trailing function shares the region.
export function regionSize(stem: string): number | null {
  const path = [join(ROOT, "out/full/asm/manifest.json"), join(ROOT, "out/asm/manifest.json")].find(existsSync);
  if (path === undefined) return null;
  const document = JSON.parse(readFileSync(path, "utf8")) as { regions?: Array<{ source: string; size: number }> };
  for (const region of document.regions ?? []) {
    if (basename(region.source, ".s") === stem) return region.size;
  }
  return null;
}

interface Options {
  source: string;
  rom: string;
  work: string;
  flags: string[];
  configuration: CandidateCompilerConfiguration;
}

function optionsOf(argv: string[]): Options {
  const options: Options = {
    source: "",
    rom: join(ROOT, "roms/gs1-en.gba"),
    work: join(ROOT, "work/candidate-show"),
    flags: [],
    configuration: { family: "routed", removeFlags: [] },
  };
  const rest: string[] = [];
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--rom") options.rom = argv[++index];
    else if (argument === "--work") options.work = argv[++index];
    // A candidate mode has to be visible before it is worth routing: the
    // allowlists in alchemy_gcc.ts are shared, so probing one here keeps a
    // trial flag out of every other region's build.
    else if (argument === "--flags") options.flags = argv[++index].split(",").filter(Boolean);
    else if (argument === "--remove-flags") {
      options.configuration.removeFlags = argv[++index].split(",").filter(Boolean);
    } else if (argument === "--family") {
      const family = argv[++index] as CandidateCompilerFamily;
      if (!["routed", "gcc296", "old-agbcc", "pret-early-thumb", "gcc2951", "gcc3"].includes(family)) {
        throw new Error("--family must be routed, gcc296, old-agbcc, pret-early-thumb, gcc2951, or gcc3");
      }
      options.configuration.family = family;
    }
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: candidate_show.ts <candidate.c> [--rom FILE] [--work DIR] [--family routed|gcc296|old-agbcc|pret-early-thumb|gcc2951|gcc3] [--flags -fa,-fb] [--remove-flags -fa,-fb]");
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
  const verification = await verifyCandidate(
    options.source, rom, options.work, options.flags, ROM_BASE, "gs1", options.configuration,
  );

  const stem = basename(options.source, ".c");
  const address = Number.parseInt(stem, 16);
  const linked = readFileSync(join(options.work, `${stem}.bin`));
  const symbols = Bun.spawnSync(["arm-none-eabi-nm", "-S", "--defined-only", join(options.work, `${stem}.elf`)],
    { stdout: "pipe", stderr: "pipe" });
  const extent = symbols.exitCode === 0 && options.configuration.family !== "gcc2951"
    ? linkedFunctionExtent(symbols.stdout.toString(), `Func_${stem}`, address, linked.length)
    : verification.actual.length;
  // Promoted sources may own a complete multi-function span after their
  // reference `.s` row has been removed. In that case the linked extent is
  // the authoritative candidate boundary; the verification object only knows
  // the entry symbol's head size.
  const expectedSize = regionSize(stem) ?? linked.length;
  const actual = linked.subarray(0, extent);
  const expected = Buffer.from(rom).subarray(address - ROM_BASE, address - ROM_BASE + expectedSize);

  const candidate = join(options.work, "candidate.bin");
  const reference = join(options.work, "reference.bin");
  await Bun.write(candidate, actual);
  await Bun.write(reference, expected);

  const left = disassemble(candidate, 0);
  const right = disassemble(reference, 0);
  const differing = differingOffsets(actual, expected);
  const offsets = [...new Set([...left.keys(), ...right.keys()])].sort((a, b) => a - b);

  console.log(`candidate=${actual.length} reference=${expected.length} differing_halfwords=${differing.size}`);
  console.log("      offset  candidate                      reference");
  for (const offset of offsets) {
    const mark = differing.has(offset) ? "!" : " ";
    const a = (left.get(offset) ?? "").padEnd(30).slice(0, 30);
    const b = right.get(offset) ?? "";
    console.log(`  ${mark} ${offset.toString(16).padStart(4, "0")}  ${a} ${b}`);
  }
}

if (import.meta.main) await main();
