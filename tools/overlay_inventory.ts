#!/usr/bin/env bun
// Discover ordinary Thumb functions inside Alchemy's canonical overlay sources
// and group relocation-normalized duplicates. Generated reports remain ignored
// below out/ and contain no copied executable bytes.
import { mkdirSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { Discovery, sx } from "./discover.ts";
import { assembleOverlay, overlayCAddresses, OVERLAY_BASE } from "./overlay_disasm.ts";
import { canonicalJson } from "./canonical_json.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface Options { assets: string; output: string; top: number }
interface FunctionRow {
  id: string;
  overlay: string;
  entry: number;
  offset: number;
  instruction_count: number;
  instruction_offsets: number[];
  code_bytes: number;
  span_bytes: number;
  calls: number;
  returns: number;
  starts_with_prologue: boolean;
  unresolved: number;
  unresolved_sites: number[];
  jump_tables: number;
  fingerprint: string;
  seed_sources: string[];
  contained_by: string[];
  structural_veneer: boolean;
  data_walk: boolean;
}

function optionsOf(argv: string[]): Options {
  const options: Options = {
    assets: join(ROOT, "assets/code"),
    output: join(ROOT, "out/decomp/overlays.json"),
    top: 20,
  };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--assets") options.assets = argv[++index];
    else if (argument === "--output" || argument === "-o") options.output = argv[++index];
    else if (argument === "--top") options.top = Number(argv[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: overlay_inventory.ts [--assets DIR] [-o OUTPUT] [--top N] [--self-test]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  if (!Number.isInteger(options.top) || options.top < 1) throw new Error("--top must be a positive integer");
  return options;
}

// How far the adjacency scan may step over the gap between one function's
// return and the next function's prologue.
//
// Filler keeps the budget the scan has always had: four halfwords. Only two are
// ever needed (one `.align` in front of a pool, one behind it), so this is
// already slack.
//
// Pools are capped by word count rather than by bytes because that is the unit
// the evidence arrives in. The longest run of consecutive recorded pool words
// across the 96 overlays is 40 (resource_373:39e8), so 64 clears every real
// layout while still bounding the scan, so a pathological run of recorded slots
// cannot walk the whole overlay.
const FILLER_HALFWORD_BUDGET = 4;
const POOL_WORD_BUDGET = 64;

// Every shape that may end the scan, i.e. every halfword the scan is willing to
// call the *entry* of the next function.
//
// `0xb5xx` (`push {..., lr}`) alone was the gate, and it is what a non-leaf gcc
// function opens with -- but a leaf opens with whatever its first statement
// needs, and stopping the chain there left everything behind such a function
// dark. Three more shapes are admitted, and they are not guesses: they are the
// same shapes `Discovery.discover_global_pointer_tables` already trusts as
// "plausible" when it decides a tagged word points at a function, plus the
// pc-relative load a constant-returning leaf opens with.
//
//   * `0xb5xx`  push {..., lr}       -- an ordinary prologue;
//   * `0x4770`  bx lr               -- a stub that returns immediately;
//   * `0x20xx-0x27xx`  movs rN,#imm -- a leaf whose first statement is a constant;
//   * `0x48xx-0x4fxx`  ldr rN,[pc]  -- a leaf that opens on its own pool word.
//
// The three additions let the chain step past a leaf and reach the ordinary
// functions laid out behind it, which is where the recall was lost.
//
// CORRECTED 2026-08-01. This used to read "the three additions can only ever
// act as *relays*: the conversion queue still filters on
// `starts_with_prologue`, so a widened entry never becomes a queue row itself."
// **That is no longer true.** The conversion queue is return-based now
// (`isConvertibleRow` in `semantic_regions_sync`, which `exact_reading_list`'s
// `isStrictRow` delegates to), and a leaf entry CAN become a queue row — 204 of
// them did.
//
// The old sentence is quoted rather than deleted because it was a door written
// in prose: it told a reader that leaves were structurally unconvertible, which
// is reason enough never to look. When a rule is removed from the code, the
// comments and self-tests asserting it are where it survives.
function isEntryShape(half: number): boolean {
  return (half & 0xff00) === 0xb500 || half === 0x4770 ||
    (half & 0xf800) === 0x2000 || (half & 0xf800) === 0x4800;
}

// Find the next function entry after a verified function's return.
//
// GCC-Thumb puts a function's literal pool immediately after its return, so the
// bytes between one function's last instruction and the next function's
// prologue are pool words and alignment filler, not code. The scan used to give
// up at the first halfword that was neither `0x0000` nor `0x46c0`, which is the
// first pool word at essentially every real function boundary -- so it recovered
// only the handful of functions that happen to have no constants at all.
//
// Discovery already records the target of every `ldr rN,[pc,#k]` it decodes in
// `literal_slots`, so the pool can be stepped over from evidence instead of by
// guessing which words look like data. Each advance this scan makes is licensed
// by exactly one fact, and it stops at the first halfword that none of them
// explain:
//
//   * `literal_slots` says this word is read as a constant by some pc-relative
//     load, so it is data -- skip the whole word;
//   * `0x0000` and `0x46c0` (`mov r8,r8`) are the assembler's own filler;
//   * a 2-byte gap sitting directly in front of a recorded pool word is the
//     `.align` that pool needed to reach its 4-byte boundary.
//
// Checking `literal_slots` before the entry-shape test matters for precision as
// well as reach: a pool word whose low halfword happens to look like `0xb5xx`
// -- or, now, like `movs r4,#33` -- is data, and testing the slot first refuses
// to seed it. The same reasoning rejects a candidate that some already-walked
// function decoded as an instruction -- reaching one means the scan has run into
// a body, not a boundary.
//
// Chaining is deliberately still done only from a function's *return*, and not
// from a tail branch or an unresolved indirect branch. That extension was built
// and measured, and both halves of it failed:
//
//   * an unconditional `b` never ends a walked function in practice. The walk
//     follows the branch, so a tail call's callee is absorbed into the caller
//     and the caller's highest-address instruction is the callee's return --
//     already a return. Admitting `branch` seeded 0 functions across the 96
//     overlays;
//   * an unresolved `indirect` (`bx rN`, `add pc,rN`) does end a function, but
//     what follows it is nearly always the jump table or dispatch data it reads.
//     Admitting it seeded 3,574 functions, of which 3,571 decoded three
//     instructions or fewer, only 2 opened with a prologue, and -- against 926
//     converted functions whose entries are known exactly -- *none* landed on a
//     real entry, where the return chain hits 539.
function nextEntryAfterReturn(discovery: Discovery, last: number): number | null {
  let candidate = last + discovery.instructions.get(last)!.size;
  let fillers = 0;
  let pools = 0;
  while (discovery.inside(candidate, 2)) {
    if (discovery.literal_slots.has(candidate)) {
      if (++pools > POOL_WORD_BUDGET) return null;
      candidate += 4;
      continue;
    }
    const half = discovery.u16(candidate);
    if (isEntryShape(half)) {
      return discovery.instructions.has(candidate) ? null : candidate;
    }
    const alignsPool = (candidate & 3) === 2 && discovery.literal_slots.has(candidate + 2);
    if (half === 0 || half === 0x46c0 || alignsPool) {
      if (++fillers > FILLER_HALFWORD_BUDGET) return null;
      candidate += 2;
      continue;
    }
    return null;
  }
  return null;
}

export function discoverOverlay(data: Buffer): Discovery {
  const discovery = new Discovery(data, OVERLAY_BASE);
  // Overlay headers are fixed veneers, not ordinary C functions. The first
  // compiler prologue after that header is the reliable control-flow root.
  for (let offset = 0; offset + 2 <= Math.min(data.length, 0x400); offset += 2) {
    if ((data.readUInt16LE(offset) & 0xff00) !== 0xb500) continue;
    discovery.add_seed(OVERLAY_BASE + offset, "thumb", "first-prologue");
    break;
  }
  for (let offset = 0; offset + 4 <= data.length; offset += 4) {
    const value = data.readUInt32LE(offset);
    const target = value & ~1;
    if ((value & 1) && discovery.inside(target, 2)) {
      const first = discovery.u16(target);
      if ((first & 0xff00) === 0xb500 || first === 0x4770 || (first & 0xf800) === 0x2000) {
        discovery.add_seed(target, "thumb", "local-function-pointer");
      }
    }
  }
  const walked = new Set<number>();
  while (true) {
    const pending = [...discovery.functions.keys()].filter((entry) => !walked.has(entry)).sort((a, b) => a - b);
    if (pending.length === 0) {
      // Recover adjacent, uncalled functions only when a verified function
      // ends in a return and the pool rule reaches a plausible function entry.
      let added = false;
      for (const fn of discovery.functions.values()) {
        const addresses = [...fn.instructions].sort((a, b) => a - b);
        if (addresses.length === 0) continue;
        const last = addresses.at(-1)!;
        if (discovery.instructions.get(last)?.kind !== "return") continue;
        const candidate = nextEntryAfterReturn(discovery, last);
        if (candidate !== null) {
          added = discovery.add_seed(candidate, "thumb", `after-return:${last.toString(16)}`) || added;
        }
      }
      if (!added) break;
      continue;
    }
    for (const entry of pending) {
      try { discovery.walk_function(entry); } catch {}
      walked.add(entry);
    }
  }
  for (const site of discovery.jump_table_sites.keys()) discovery.unresolved.delete(site);
  for (const fn of discovery.functions.values()) {
    for (const site of discovery.jump_table_sites.keys()) fn.unresolved.delete(site);
  }
  return discovery;
}

// Synthetic overlay images for the self-test: `h` writes a halfword of code and
// `w` a 32-bit word of data, so each case reads as the layout it is testing.
// Every image is 0x40 bytes, and the trailing zeros double as a check that a
// scan running off the end of the code never invents a trailing function.
function syntheticOverlay(items: Array<[offset: number, width: "h" | "w", value: number]>): Buffer {
  const data = Buffer.alloc(0x40);
  for (const [offset, width, value] of items) {
    if (width === "h") data.writeUInt16LE(value, offset);
    else data.writeUInt32LE(value, offset);
  }
  return data;
}

// Cover the pool-skip in the adjacency scan. Each image opens with `push {lr}`
// at offset 0 so the first-prologue seeder has its one root, and every later
// function must be reached by the scan rather than by any other seeder -- which
// the `after-return:` source assertion checks explicitly.
function selfTest(): void {
  const base = OVERLAY_BASE;
  const check = (label: string, ok: boolean): void => {
    if (!ok) throw new Error(`code-overlay inventory self-test: ${label}`);
  };
  const fromScan = (discovery: Discovery, entry: number): boolean => {
    const fn = discovery.functions.get(entry);
    return fn !== undefined && [...fn.sources].some((source) => source.startsWith("after-return:"));
  };

  // A return followed by the function's own two pool words, then a real
  // prologue. Both words are recorded in `literal_slots` by the `ldr rN,[pc,#4]`
  // loads at 0x02 and 0x04, so the scan steps over them and reaches 0x10. The
  // old scan stopped dead at 0x08 on the first pool word's low halfword.
  const pooled = discoverOverlay(syntheticOverlay([
    [0x00, "h", 0xb500], [0x02, "h", 0x4801], [0x04, "h", 0x4901], [0x06, "h", 0xbd00],
    [0x08, "w", 0x0a0b0c0d], [0x0c, "w", 0x12345678],
    [0x10, "h", 0xb510], [0x12, "h", 0x2000], [0x14, "h", 0xbd10],
  ]));
  check("pool words must be stepped over", fromScan(pooled, base + 0x10));
  check("pool words are not function entries", !pooled.functions.has(base + 0x08) && !pooled.functions.has(base + 0x0c));

  // Genuine data after the return, with no pool word recorded anywhere. The
  // scan must stop at the first unexplained halfword (0xbeef) and must not
  // reach the `0xb500` halfword sitting inside the data word at 0x08.
  const opaque = discoverOverlay(syntheticOverlay([
    [0x00, "h", 0xb500], [0x02, "h", 0xbd00],
    [0x04, "w", 0xdeadbeef], [0x08, "w", 0x0002b500],
  ]));
  check("unexplained data must stop the scan", !opaque.functions.has(base + 0x08));
  check("data must not yield extra functions", opaque.functions.size === 1);

  // One recorded pool word followed by an *unrecorded* word, then a real-looking
  // prologue. The scan advances only over evidence, so the unrecorded word at
  // 0x0c stops it: reaching 0x10 would mean guessing how long the pool is.
  const unrecorded = discoverOverlay(syntheticOverlay([
    [0x00, "h", 0xb500], [0x02, "h", 0x4801], [0x04, "h", 0xbd00], [0x06, "h", 0x0000],
    [0x08, "w", 0x0a0b0c0d], [0x0c, "w", 0x11223344],
    [0x10, "h", 0xb510], [0x12, "h", 0xbd10],
  ]));
  check("unrecorded words must stop the scan", !unrecorded.functions.has(base + 0x10));

  // The filler cases the scan has always handled must survive the rewrite: a
  // `0x0000` alignment halfword in front of the pool, and two `0x46c0`
  // (`mov r8,r8`) padding halfwords behind it.
  const padded = discoverOverlay(syntheticOverlay([
    [0x00, "h", 0xb500], [0x02, "h", 0x4801], [0x04, "h", 0xbd00], [0x06, "h", 0x0000],
    [0x08, "w", 0x0a0b0c0d], [0x0c, "h", 0x46c0], [0x0e, "h", 0x46c0],
    [0x10, "h", 0xb510], [0x12, "h", 0xbd10],
  ]));
  check("alignment and nop padding must be stepped over", fromScan(padded, base + 0x10));

  // A pool word whose own low halfword looks like `push {r4,lr}`. Testing
  // `literal_slots` before the prologue shape refuses it as data and carries on
  // to the real function behind it; the old scan seeded the pool word itself.
  const disguised = discoverOverlay(syntheticOverlay([
    [0x00, "h", 0xb500], [0x02, "h", 0x4801], [0x04, "h", 0xbd00], [0x06, "h", 0x0000],
    [0x08, "w", 0x0001b510],
    [0x0c, "h", 0xb510], [0x0e, "h", 0xbd10],
  ]));
  check("a prologue-shaped pool word is data", !disguised.functions.has(base + 0x08));
  check("the function behind a disguised pool word is found", fromScan(disguised, base + 0x0c));

  // Each widened entry shape, behind the pool word that used to hide it. The
  // leaf at 0x0c opens with `bx lr` / `movs r0,#1` / `ldr r0,[pc,#0]` instead of
  // a prologue, and the scan that only accepted `0xb5xx` stopped at it and left
  // the *ordinary* function at 0x14 behind it dark. Both must now be reached,
  // and the second only via the first.
  const prefix: Array<[number, "h" | "w", number]> = [
    [0x00, "h", 0xb500], [0x02, "h", 0x4801], [0x04, "h", 0xbd00], [0x06, "h", 0x0000],
    [0x08, "w", 0x0a0b0c0d],
  ];
  const tail: Array<[number, "h" | "w", number]> = [[0x14, "h", 0xb510], [0x16, "h", 0xbd10]];
  const leaves: Array<[string, Array<[number, "h" | "w", number]>]> = [
    // `bx lr` is itself the leaf's return; 0x10-0x13 stay zero filler.
    ["bx lr", [[0x0c, "h", 0x4770]]],
    ["movs", [[0x0c, "h", 0x2001], [0x0e, "h", 0x4770]]],
    // The leaf's own `ldr r0,[pc,#0]` records 0x10 as a pool word, so the chain
    // out of the leaf steps over it exactly as it does after a prologue.
    ["pc-relative load", [[0x0c, "h", 0x4800], [0x0e, "h", 0x4770], [0x10, "w", 0x0a0b0c0d]]],
  ];
  for (const [label, leaf] of leaves) {
    const widened = discoverOverlay(syntheticOverlay([...prefix, ...leaf, ...tail]));
    check(`a leaf opening with ${label} is an entry`, fromScan(widened, base + 0x0c));
    check(`the function behind a ${label} leaf is found`, fromScan(widened, base + 0x14));
  }

  // The same precision guard as the prologue-shaped pool word, for a widened
  // shape: `literal_slots` is tested first, so a pool word whose low halfword
  // reads as `movs r4,#33` stays data. This is the one shape that produced a
  // mid-function seed when the widening was measured against the converted
  // spans, so it is pinned here.
  const disguisedLeaf = discoverOverlay(syntheticOverlay([
    [0x00, "h", 0xb500], [0x02, "h", 0x4801], [0x04, "h", 0xbd00], [0x06, "h", 0x0000],
    [0x08, "w", 0x00002421],
    [0x0c, "h", 0xb510], [0x0e, "h", 0xbd10],
  ]));
  check("a movs-shaped pool word is data", !disguisedLeaf.functions.has(base + 0x08));
  check("the function behind a movs-shaped pool word is found", fromScan(disguisedLeaf, base + 0x0c));

  // Chaining stays anchored on returns. A function ending in an unresolved
  // `bx r3` is followed by the dispatch data it reads, not by a function, and
  // seeding there measured as pure noise -- so nothing may be seeded past it
  // even though the halfword behind it is a perfectly good prologue.
  const indirect = discoverOverlay(syntheticOverlay([
    [0x00, "h", 0xb500], [0x02, "h", 0x4718],
    [0x04, "h", 0xb510], [0x06, "h", 0xbd10],
  ]));
  check("an unresolved indirect branch seeds no successor", !indirect.functions.has(base + 0x04));

  console.log("code-overlay inventory self-test passed");
}

function targetClass(discovery: Discovery, value: number): string {
  const address = value & ~1;
  if (discovery.inside(address, 2)) return "overlay";
  const bank = value >>> 24;
  if (bank === 0x08 || bank === 0x09) return "rom";
  if (bank === 0x02 || bank === 0x03) return "ram";
  if (bank === 0x04) return "io";
  return `constant:${value <= 0xffff ? value.toString(16) : "wide"}`;
}

// A long-branch veneer is a literal word followed by `ldr rN,[pc,#0]` / `bx rN`,
// laid out so each veneer's pointer sits in front of the next veneer's code.
// Discovery seeds on the tagged pointer, so the entry it reports is the word,
// not the branch, and the "function" it walks is the following veneer plus
// whatever trails it. These are linker output, not compiler output: there is no
// C source to recover, so they are excluded from the conversion queue.
function structuralVeneer(discovery: Discovery, entry: number): boolean {
  if (!discovery.inside(entry, 8)) return false;
  const pointer = discovery.u32(entry);
  if ((pointer & 1) === 0) return false;
  const bank = pointer >>> 24;
  if (bank !== 0x08 && bank !== 0x09) return false;
  const load = discovery.u16(entry + 4);
  const branch = discovery.u16(entry + 6);
  if ((load & 0xf8ff) !== 0x4800) return false;
  if ((branch & 0xff87) !== 0x4700) return false;
  return ((load >>> 8) & 7) === ((branch >>> 3) & 0xf);
}

// ARMv4T reserves 0xe800-0xffff entirely for the two halves of the 4-byte
// BL/BLX pair, so no standalone 2-byte Thumb instruction can encode a halfword
// in that range. A walk that decodes one as a 2-byte instruction is therefore
// not reading code: the seed landed in a data table, or mid-BL, and everything
// after it is a misalignment. Discovery seeds aggressively on tagged words
// whose first halfword merely looks like `movs rN,#imm`, which matches a great
// deal of ordinary data, so these walks are common and can run for thousands of
// bytes. They have no C source to recover and are excluded from the queue.
//
// This is an encoding fact rather than a heuristic, and it measures that way:
// across the 2,477 functions the main-ROM discovery finds -- 1,167 of which
// already have byte-exact C -- it flags none.
function dataWalk(discovery: Discovery, addresses: number[]): boolean {
  for (const address of addresses) {
    if (discovery.instructions.get(address)?.size === 2 && discovery.u16(address) >= 0xe800) return true;
  }
  return false;
}

function fingerprint(discovery: Discovery, addresses: number[]): string {
  const tokens: string[] = [];
  for (const address of addresses) {
    const instruction = discovery.instructions.get(address)!;
    const half = discovery.u16(address);
    if (instruction.size === 4 && (half & 0xf800) === 0xf000) {
      const low = discovery.u16(address + 2);
      const offset = sx(((half & 0x7ff) << 12) | ((low & 0x7ff) << 1), 23);
      tokens.push(`bl:${targetClass(discovery, address + 4 + offset)}`);
    } else if ((half & 0xf800) === 0xe000) {
      tokens.push(`b:${half >>> 11}`);
    } else if ((half & 0xf000) === 0xd000 && ((half >>> 8) & 0xf) < 0xe) {
      tokens.push(`bc:${(half >>> 8) & 0xf}`);
    } else if ((half & 0xf800) === 0x4800) {
      const literal = ((address + 4) & ~3) + ((half & 0xff) << 2);
      const kind = discovery.inside(literal, 4) ? targetClass(discovery, discovery.u32(literal)) : "external";
      tokens.push(`ldr-lit:r${(half >>> 8) & 7}:${kind}`);
    } else if ((half & 0xf800) === 0xa000) {
      tokens.push(`adr:r${(half >>> 8) & 7}`);
    } else {
      tokens.push(half.toString(16).padStart(4, "0"));
    }
  }
  return Bun.hash(tokens.join(" ")).toString(36);
}

function convertedPlaceholders(source: string): Map<number, number> {
  const placeholders = new Map<number, number>();
  const text = readFileSync(source, "utf8");
  const pattern = /^AlchemyC_([0-9a-f]{8}):[ \t]*$/gim;
  for (const match of text.matchAll(pattern)) {
    const address = Number.parseInt(match[1], 16);
    if (placeholders.has(address)) throw new Error(`duplicate overlay C placeholder: ${source}:0x${address.toString(16)}`);
    let size = 0;
    const tail = text.slice(match.index! + match[0].length);
    for (const raw of tail.split(/\r?\n/).slice(1)) {
      const line = raw.trim();
      if (line === "" || /^\.L_[0-9a-z_.$]+:$/i.test(line)) continue;
      const space = /^\.space\s+(0x[0-9a-f]+|[0-9]+)\b/i.exec(line);
      if (space === null) break;
      size += Number.parseInt(space[1], 0);
    }
    if (size === 0) throw new Error(`overlay C placeholder has no space: ${source}:0x${address.toString(16)}`);
    placeholders.set(address, size);
  }
  return placeholders;
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) {
    selfTest();
    return;
  }
  const options = optionsOf(argv);
  // Flat layout, matching the asset builder and the overlay disassembler: the
  // overlay is assets/code/<name>_overlay.s, not <name>/overlay.s. The old
  // nested spelling matched nothing and reported overlays=0 without erroring.
  const suffix = "_overlay.s";
  const overlays = readdirSync(options.assets)
    .filter((name) => name.startsWith("resource_") && name.endsWith(suffix))
    .sort()
    .map((name) => ({ name: name.slice(0, -suffix.length), source: join(options.assets, name) }))
    .filter((item) => Bun.file(item.source).size > 0);
  if (overlays.length === 0) throw new Error(`no overlay sources under ${options.assets}`);
  const functions: FunctionRow[] = [];
  let decodedBytes = 0, instructionBytes = 0, unresolved = 0, jumpTables = 0;
  let convertedFunctions = 0, convertedInstructionBytes = 0, convertedSpanBytes = 0;
  let convertedInternalEntries = 0, undiscoveredConvertedFunctions = 0;
  for (const overlay of overlays) {
    const data = assembleOverlay(overlay.source, OVERLAY_BASE);
    const converted = overlayCAddresses(overlay.source);
    const placeholders = convertedPlaceholders(overlay.source);
    const missing = [...converted].filter((address) => !placeholders.has(address));
    const orphaned = [...placeholders.keys()].filter((address) => !converted.has(address));
    if (missing.length > 0 || orphaned.length > 0) {
      const detail = [
        missing.length > 0 ? `missing placeholders=${missing.map((address) => `0x${address.toString(16)}`).join(",")}` : "",
        orphaned.length > 0 ? `orphaned placeholders=${orphaned.map((address) => `0x${address.toString(16)}`).join(",")}` : "",
      ].filter(Boolean).join(" ");
      throw new Error(`overlay C/source mismatch: ${overlay.name}: ${detail}`);
    }
    convertedFunctions += converted.size;
    convertedSpanBytes += [...placeholders.values()].reduce((sum, size) => sum + size, 0);
    const convertedSpans = [...placeholders];
    decodedBytes += data.length;
    const discovery = discoverOverlay(data);
    instructionBytes += [...discovery.instructions.values()].reduce((sum, item) => sum + item.size, 0);
    convertedInstructionBytes += [...discovery.instructions].filter(([address]) =>
      convertedSpans.some(([start, size]) => start <= address && address < start + size))
      .reduce((sum, [, item]) => sum + item.size, 0);
    unresolved += discovery.unresolved.size;
    jumpTables += discovery.jump_tables.size;
    const discoveredConverted = new Set<number>();
    for (const [entry, fn] of [...discovery.functions].sort((a, b) => a[0] - b[0])) {
      const addresses = [...fn.instructions].sort((a, b) => a - b);
      if (addresses.length === 0 || addresses[0] !== entry) continue;
      const convertedOwner = convertedSpans.find(([start, size]) => start <= entry && entry < start + size)?.[0];
      if (convertedOwner !== undefined) {
        if (entry === convertedOwner) discoveredConverted.add(entry);
        else convertedInternalEntries++;
        continue;
      }
      let end = Math.max(...addresses.map((address) => address + discovery.instructions.get(address)!.size));
      for (const address of addresses) {
        const half = discovery.u16(address);
        if ((half & 0xf800) !== 0x4800) continue;
        const literal = ((address + 4) & ~3) + ((half & 0xff) << 2);
        if (discovery.inside(literal, 4)) end = Math.max(end, literal + 4);
      }
      const offset = entry - OVERLAY_BASE;
      functions.push({
        id: `${overlay.name}:${offset.toString(16).padStart(4, "0")}`,
        overlay: overlay.name,
        entry,
        offset,
        instruction_count: addresses.length,
        instruction_offsets: addresses.map((address) => address - entry),
        code_bytes: addresses.reduce((sum, address) => sum + discovery.instructions.get(address)!.size, 0),
        span_bytes: end - entry,
        calls: fn.callees.size + fn.external_callees.size,
        returns: addresses.filter((address) => discovery.instructions.get(address)!.kind === "return").length,
        starts_with_prologue: (discovery.u16(entry) & 0xff00) === 0xb500,
        unresolved: fn.unresolved.size,
        unresolved_sites: [...fn.unresolved].sort((a, b) => a - b).map((site) => site - OVERLAY_BASE),
        jump_tables: [...discovery.jump_table_sites.keys()].filter((site) => fn.instructions.has(site)).length,
        fingerprint: fingerprint(discovery, addresses),
        seed_sources: [...fn.sources].sort(),
        contained_by: [],
        structural_veneer: structuralVeneer(discovery, entry),
        data_walk: dataWalk(discovery, addresses),
      });
    }
    undiscoveredConvertedFunctions += [...converted].filter((address) => !discoveredConverted.has(address)).length;
  }
  const instructionOwners = new Map<string, Map<number, string[]>>();
  for (const fn of functions) {
    if (!instructionOwners.has(fn.overlay)) instructionOwners.set(fn.overlay, new Map());
    const owners = instructionOwners.get(fn.overlay)!;
    for (const offset of fn.instruction_offsets) {
      const address = fn.entry + offset;
      if (!owners.has(address)) owners.set(address, []);
      owners.get(address)!.push(fn.id);
    }
  }
  for (const fn of functions) {
    fn.contained_by = (instructionOwners.get(fn.overlay)!.get(fn.entry) ?? [])
      .filter((id) => id !== fn.id)
      .sort();
  }
  const veneers = functions.filter((fn) => fn.structural_veneer);
  const dataWalks = functions.filter((fn) => fn.data_walk);
  const ordinary = functions.filter((fn) => fn.unresolved === 0 && fn.jump_tables === 0 &&
    fn.code_bytes >= 8 && fn.span_bytes - fn.code_bytes <= 64 && !fn.structural_veneer && !fn.data_walk);
  const tiny = functions.filter((fn) => fn.code_bytes <= 6);
  const contained = functions.filter((fn) => fn.contained_by.length > 0);
  const returning = functions.filter((fn) => fn.returns > 0);
  // A walked row that contains no return is the one reliable signal of a
  // mis-derived span: the walk either stopped inside a mid-function pool that a
  // branch jumps over, or cut the trailing pool short. A span audit over 1,337
  // strict rows found exactly three such rows and two were genuinely mis-spanned
  // (resource_399:15b4 advertised 220 against a true 248; resource_3ca:0f80
  // advertised 328 against 340), while every other row ended at exactly one
  // return. Row ends landing on a non-prologue address flag 74 rows and are
  // almost all veneer banks, so they are not worth reporting.
  // KEPT prologue-keyed, on measured grounds and NOT "for the same reason the
  // strict queue does" — the strict queue is return-based now (§5k), and this
  // filter is deliberately not following it. The reason is signal-to-noise in a
  // WARNING, not eligibility for work: without the prologue test the relay seeds
  // that land on data (2-4 byte walks) drown it, 503 rows against the 3 that
  // matter. A diagnostic nobody reads is worse than no diagnostic, and no owner
  // is excluded from anything by this line.
  const spanSuspects = functions.filter((fn) =>
    fn.returns === 0 && fn.starts_with_prologue && !fn.structural_veneer &&
    !fn.data_walk && fn.contained_by.length === 0);
  for (const fn of spanSuspects) {
    console.warn(`warning: ${fn.id} walked ${fn.code_bytes} bytes with no return — verify its span`);
  }
  const ordinaryPrologueReturn = ordinary.filter((fn) =>
    fn.contained_by.length === 0 && fn.starts_with_prologue && fn.returns > 0);
  const groups = new Map<string, FunctionRow[]>();
  for (const fn of ordinary) {
    if (!groups.has(fn.fingerprint)) groups.set(fn.fingerprint, []);
    groups.get(fn.fingerprint)!.push(fn);
  }
  const families = [...groups.entries()].map(([fingerprint, members]) => ({
    fingerprint,
    count: members.length,
    overlays: new Set(members.map((item) => item.overlay)).size,
    code_bytes: members.reduce((sum, item) => sum + item.code_bytes, 0),
    members: members.map((item) => item.id),
  })).sort((left, right) => right.count - left.count || right.code_bytes - left.code_bytes);
  const report = {
    format: 1,
    generated_at: new Date().toISOString(),
    totals: {
      overlays: overlays.length,
      decoded_bytes: decodedBytes,
      converted_functions: convertedFunctions,
      unconverted_discoveries: functions.length,
      tiny_unconverted_discoveries: tiny.length,
      contained_unconverted_discoveries: contained.length,
      returning_unconverted_discoveries: returning.length,
      ordinary_prologue_return_discoveries: ordinaryPrologueReturn.length,
      structural_veneer_discoveries: veneers.length,
      data_walk_discoveries: dataWalks.length,
      converted_internal_entries: convertedInternalEntries,
      undiscovered_converted_functions: undiscoveredConvertedFunctions,
      functions: functions.length,
      ordinary_unconverted_discoveries: ordinary.length,
      ordinary_functions: ordinary.length,
      instruction_bytes: instructionBytes,
      converted_instruction_bytes: convertedInstructionBytes,
      converted_span_bytes: convertedSpanBytes,
      unresolved,
      jump_tables: jumpTables,
      duplicate_families: families.filter((family) => family.count > 1).length,
      duplicate_functions: families.filter((family) => family.count > 1).reduce((sum, family) => sum + family.count, 0),
    },
    families,
    functions,
  };
  mkdirSync(dirname(options.output), { recursive: true });
  writeFileSync(options.output, canonicalJson(report) + "\n");
  console.log(`overlays=${report.totals.overlays} converted_functions=${convertedFunctions} unconverted_discoveries=${functions.length} ordinary_unconverted_discoveries=${ordinary.length} tiny_unconverted_discoveries=${tiny.length} contained_unconverted_discoveries=${contained.length} returning_unconverted_discoveries=${returning.length} ordinary_prologue_return_discoveries=${ordinaryPrologueReturn.length} structural_veneer_discoveries=${veneers.length} data_walk_discoveries=${dataWalks.length}`);
  console.log(`decoded_bytes=${decodedBytes} instruction_bytes=${instructionBytes} converted_instruction_bytes=${convertedInstructionBytes} converted_span_bytes=${convertedSpanBytes} converted_internal_entries=${convertedInternalEntries} undiscovered_converted_functions=${undiscoveredConvertedFunctions} unresolved=${unresolved} jump_tables=${jumpTables}`);
  console.log(`duplicate_families=${report.totals.duplicate_families} duplicate_functions=${report.totals.duplicate_functions}`);
  for (const family of families.filter((item) => item.count > 1).slice(0, options.top)) {
    const sample = family.members.slice(0, 12).join(",");
    console.log(`${family.fingerprint}\tcount=${family.count}\toverlays=${family.overlays}\tbytes=${family.code_bytes}\t${sample}${family.count > 12 ? ",..." : ""}`);
  }
  console.log(`report=${options.output}`);
}

if (import.meta.main) main();
