#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md, package.json.
// Rank remaining assembly C-debt regions by structural similarity to the
// already byte-verified src/*.c corpus, so the next hand-matching batch starts
// from "an installed sibling can probably be adapted" rather than from size
// order. Registers and immediates are normalized away before comparison, so a
// close hit means the opcode skeleton matches; the semantics may belong to a
// completely different subsystem, and that is fine.
//
// Publication note: this reads ROM bytes to disassemble them, but emits only
// region addresses, sizes, opcode counts and edit distances. No ROM bytes, no
// disassembly text and no candidate C ever leave the ignored scratch tree.
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { canonicalJson } from "./canonical_json.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const SCRATCH = join(ROOT, "out", "sibrank");
const ROM_BASE = 0x08000000;

/* Absolute cap, and a proportional one so that a 12-instruction leaf is not
   declared a sibling of anything at distance 15. The proportional cap does the
   real work; the absolute one only stops long regions from pairing off with
   siblings too distant to adapt. Raised from 10 to 15 after batches 5-7 showed
   distance-11..15 siblings still transferring their C shape usefully -- at 10
   the candidate pool was 18 regions, at 15 it is ~98. */
const DISTANCE_CAP = 15;
/* A region whose opcode skeleton is this much longer or shorter than the
   candidate cannot come in under the distance cap, so it is skipped before the
   quadratic comparison runs at all. Every length difference costs at least one
   edit, so this must not be tighter than the cap or real siblings are dropped. */
const LENGTH_SLACK = DISTANCE_CAP;
const DISTANCE_FRACTION = 0.34;
/* Below this many instructions every region looks like every other region. */
const MINIMUM_OPCODES = 4;

interface Region {
  address: number;
  size: number;
  source: string;
  kind?: string;
}

interface Shape extends Region {
  opcodes: string[];
}

export interface SiblingRanking {
  stem: string;
  size: number;
  opcodes: number;
  distance: number;
  sibling: string;
}

function regionsOf(manifest: string): Region[] {
  const path = join(ROOT, manifest);
  if (!existsSync(path)) throw new Error(`missing manifest ${manifest}; run tools/build_full.ts first`);
  const document = JSON.parse(readFileSync(path, "utf8")) as { regions?: Region[] };
  return document.regions ?? [];
}

/* Normalize one disassembled instruction to an opcode token: mnemonic plus
   operand structure, with every register collapsed to R and every immediate or
   address to K. Two regions that differ only in which registers the allocator
   picked, or in which constants they carry, produce identical token streams. */
function opcodeToken(mnemonic: string, operands: string): string {
  const bare = mnemonic.toLowerCase().replace(/\.(?:n|w)$/, "");
  const shape = operands
    .replace(/\s*[;@].*$/, "")
    .replace(/\b(?:r(?:1[0-2]|[0-9])|sp|lr|pc|ip|fp|sl)\b/gi, "R")
    .replace(/#-?\d+|0x[0-9a-f]+|\b\d+\b/gi, "K")
    .replace(/\s+/g, "");
  return `${bare} ${shape}`;
}

async function shapeOf(rom: Buffer, region: Region): Promise<string[]> {
  const path = join(SCRATCH, `${region.address.toString(16)}.bin`);
  const start = region.address - ROM_BASE;
  writeFileSync(path, rom.subarray(start, start + region.size));
  const child = Bun.spawn([
    "arm-none-eabi-objdump", "-D", "-b", "binary", "-m", "arm", "-M", "force-thumb",
    `--adjust-vma=0x${region.address.toString(16)}`, path,
  ], { stdout: "pipe", stderr: "pipe" });
  const listing = await new Response(child.stdout).text();
  await child.exited;
  const opcodes: string[] = [];
  for (const row of listing.split(/\r?\n/)) {
    const parsed = row.match(/^\s*([0-9a-f]+):\s+([0-9a-f ]+?)\s+\t(\S+)\s*(.*)$/i);
    if (parsed) opcodes.push(opcodeToken(parsed[3], parsed[4]));
  }
  return opcodes;
}

async function shapesOf(rom: Buffer, regions: Region[], jobs: number): Promise<Shape[]> {
  const shapes: Shape[] = [];
  let next = 0;
  await Promise.all(Array.from({ length: jobs }, async () => {
    for (;;) {
      const index = next++;
      if (index >= regions.length) return;
      const region = regions[index];
      shapes.push({ ...region, opcodes: await shapeOf(rom, region) });
    }
  }));
  return shapes;
}

/* Levenshtein over opcode tokens, abandoning a pair as soon as every cell in a
   row exceeds the cap. Without the early exit this is minutes rather than
   seconds across a 1200-by-800 comparison. */
export function tokenDistance(left: string[], right: string[], cap: number): number {
  if (Math.abs(left.length - right.length) > cap) return cap + 1;
  let previous = Array.from({ length: right.length + 1 }, (_, index) => index);
  for (let row = 1; row <= left.length; row++) {
    const current = [row];
    let best = row;
    for (let column = 1; column <= right.length; column++) {
      const value = Math.min(
        previous[column] + 1,
        current[column - 1] + 1,
        previous[column - 1] + (left[row - 1] === right[column - 1] ? 0 : 1),
      );
      current.push(value);
      if (value < best) best = value;
    }
    if (best > cap) return cap + 1;
    previous = current;
  }
  return previous[right.length];
}

export function rankAgainstCorpus(candidates: Shape[], corpus: Shape[]): SiblingRanking[] {
  const ranked: SiblingRanking[] = [];
  for (const candidate of candidates) {
    if (candidate.opcodes.length < MINIMUM_OPCODES) continue;
    const cap = Math.min(DISTANCE_CAP, Math.ceil(candidate.opcodes.length * DISTANCE_FRACTION));
    let best = { distance: Number.POSITIVE_INFINITY, sibling: "" };
    for (const installed of corpus) {
      if (Math.abs(installed.opcodes.length - candidate.opcodes.length) > LENGTH_SLACK) continue;
      const distance = tokenDistance(candidate.opcodes, installed.opcodes, cap);
      if (distance < best.distance) best = { distance, sibling: installed.source };
      if (distance === 0) break;
    }
    if (best.distance > DISTANCE_CAP) continue;
    ranked.push({
      stem: basename(candidate.source, ".s"),
      size: candidate.size,
      opcodes: candidate.opcodes.length,
      distance: best.distance,
      sibling: best.sibling,
    });
  }
  ranked.sort((a, b) => a.distance - b.distance || a.opcodes - b.opcodes || a.stem.localeCompare(b.stem));
  return ranked;
}

function parseArguments(argv: string[]): { rom: string; limit: number; maximumSize: number; selfTest: boolean } {
  let rom = "", limit = 40, maximumSize = 1400, selfTest = false;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--self-test") selfTest = true;
    else if (argument === "--limit") limit = Number(argv[++index]);
    else if (argument === "--max-size") maximumSize = Number(argv[++index]);
    else if (!argument.startsWith("--")) rom = argument;
  }
  return { rom, limit, maximumSize, selfTest };
}

function selfTest(): void {
  if (opcodeToken("adds", "r5, r3, #0") !== opcodeToken("adds", "r2, r7, #12")) {
    throw new Error("register and immediate normalization self-test failed");
  }
  if (opcodeToken("bl", "0x80a17c4") === opcodeToken("blx", "0x80a17c4")) {
    throw new Error("mnemonic distinctness self-test failed");
  }
  if (tokenDistance(["a", "b", "c"], ["a", "b", "c"], 4) !== 0) throw new Error("identity distance self-test failed");
  if (tokenDistance(["a", "b", "c"], ["a", "x", "c"], 4) !== 1) throw new Error("substitution distance self-test failed");
  if (tokenDistance(["a", "b"], ["a", "b", "c"], 4) !== 1) throw new Error("insertion distance self-test failed");
  if (tokenDistance(["a", "b", "c", "d"], ["w", "x", "y", "z"], 2) <= 2) {
    throw new Error("distance cap early-exit self-test failed");
  }
  const corpus: Shape[] = [
    { address: 0x08000000, size: 8, source: "src/08000000.c", opcodes: ["push R", "movs RK", "pop R", "bx R"] },
    { address: 0x08000100, size: 8, source: "src/08000100.c", opcodes: ["push R", "ldr RK", "str RK", "pop R"] },
  ];
  const candidates: Shape[] = [
    { address: 0x08001000, size: 8, source: "asm/08001000.s", opcodes: ["push R", "movs RK", "pop R", "bx R"] },
    { address: 0x08002000, size: 4, source: "asm/08002000.s", opcodes: ["bx R"] },
  ];
  const ranked = rankAgainstCorpus(candidates, corpus);
  if (ranked.length !== 1) throw new Error("minimum-opcode filter self-test failed");
  if (ranked[0].stem !== "08001000" || ranked[0].distance !== 0 || ranked[0].sibling !== "src/08000000.c") {
    throw new Error("ranking self-test failed");
  }
  console.log("self-test=ok");
}

async function main(): Promise<void> {
  const options = parseArguments(Bun.argv.slice(2));
  if (options.selfTest) return selfTest();
  if (!options.rom) throw new Error("the following arguments are required: rom");
  mkdirSync(SCRATCH, { recursive: true });
  const rom = readFileSync(options.rom);

  const corpusRegions = regionsOf("out/full/claimed/manifest.json").filter((r) => r.source.startsWith("src/"));
  const candidateRegions = regionsOf("out/full/asm/manifest.json").filter((region) =>
    region.kind === "compiler_output" &&
    region.size <= options.maximumSize &&
    /^[0-9a-f]{8}$/.test(basename(region.source, ".s")));

  const jobs = Math.max(1, Math.min(16, (navigator.hardwareConcurrency || 8) - 2));
  const [corpus, candidates] = await Promise.all([
    shapesOf(rom, corpusRegions, jobs),
    shapesOf(rom, candidateRegions, jobs),
  ]);
  const ranked = rankAgainstCorpus(candidates, corpus);

  const report = { format: 1, corpus: corpus.length, candidates: candidates.length, ranked: ranked.slice(0, options.limit) };
  console.log(canonicalJson(report));
  console.error(`corpus=${corpus.length} candidates=${candidates.length} ranked=${ranked.length}`);
}

if (import.meta.main) await main();
