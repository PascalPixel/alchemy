#!/usr/bin/env bun
// Who references this main-image address?
//
// Scans the whole main image for every way an address can be reached:
//
//   * a `bl` to it (a call),
//   * an unconditional or conditional `b` to it (a branch, i.e. it is interior
//     to some function),
//   * an aligned pool word equal to it (a data reference), and
//   * an aligned pool word equal to **address + 1** — the Thumb bit set, which
//     is how an *indirectly published callback* is stored.
//
// What this tool CANNOT decide: a region with no inbound reference at all. A
// genuine continuation is normally reached by falling through from the
// preceding instruction, which leaves no reference behind, so a literal pool
// and a fall-through continuation are indistinguishable here. Measured on the
// 32 open main-image continuation regions, 31 came back with no inbound
// reference — which narrows them to "pool or fall-through" and no further.
// Settling those needs a reachability walk from the owner's entry.
//
// That last case is the one that earns this tool. It is the only cheap way to
// tell an unreferenced tail from an independently callable function: a lane
// proved `0808f498` is a real function, not a continuation, from five pool
// words holding `0x0808f499` and no branch into it at all. Five witnesses, one
// command, no disassembly.
//
// Read the output like this:
//
//   calls > 0                     an ordinary function
//   thumb_pointers > 0            an indirectly published callback — a function
//   branches > 0, calls == 0      interior to another owner, not an entry
//   word_refs only, no branches   a literal pool or data table, NOT code
//   nothing at all                INCONCLUSIVE — see below
//
//   bun tools/main_xref.ts 0808f498
//   bun tools/main_xref.ts 080e53f4 080f474a 080bea9c    # several at once
//   bun tools/main_xref.ts --self-test
import { readFileSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
/** The main image is linked at this base. */
export const IMAGE_BASE = 0x08000000;

export interface Xref {
  address: number;
  calls: number[];
  branches: number[];
  wordRefs: number[];
  thumbPointers: number[];
}

/**
 * Decode a Thumb BL pair into its target. The main image is LINKED, so this is
 * ordinary `pc + 4 + displacement` — the overlay `stored + 2` rule is an
 * artefact of unlinked overlay images and must NOT be applied here. Confirmed
 * by resolving `.set sub_*` symbols across four regions against the tree.
 */
export function blTarget(site: number, high: number, low: number): number | null {
  if ((high & 0xf800) !== 0xf000) return null;
  if ((low & 0xf800) !== 0xf800) return null;
  const upper = high & 0x07ff;
  const lower = low & 0x07ff;
  const signed = upper >= 0x0400 ? upper - 0x0800 : upper;
  return site + 4 + ((signed << 12) | (lower << 1));
}

/** `b <label>` — the unconditional 11-bit form. */
export function branchTarget(site: number, halfword: number): number | null {
  if ((halfword & 0xf800) !== 0xe000) return null;
  const offset = halfword & 0x07ff;
  const signed = offset >= 0x0400 ? offset - 0x0800 : offset;
  return site + 4 + (signed << 1);
}

/** `b<cond> <label>` — the conditional 8-bit form. 0xdf is SWI, not a branch. */
export function conditionalTarget(site: number, halfword: number): number | null {
  if ((halfword & 0xf000) !== 0xd000) return null;
  const condition = (halfword >> 8) & 0xf;
  if (condition === 0xe || condition === 0xf) return null;
  const offset = halfword & 0xff;
  const signed = offset >= 0x80 ? offset - 0x100 : offset;
  return site + 4 + (signed << 1);
}

export function scan(image: Uint8Array, wanted: Set<number>): Map<number, Xref> {
  const found = new Map<number, Xref>();
  for (const address of wanted) {
    found.set(address, { address, calls: [], branches: [], wordRefs: [], thumbPointers: [] });
  }
  for (let at = 0; at + 1 < image.length; at += 2) {
    const site = IMAGE_BASE + at;
    const halfword = image[at] | (image[at + 1] << 8);

    if (at + 3 < image.length) {
      const low = image[at + 2] | (image[at + 3] << 8);
      const call = blTarget(site, halfword, low);
      if (call !== null && wanted.has(call)) found.get(call)!.calls.push(site);
    }
    const branch = branchTarget(site, halfword) ?? conditionalTarget(site, halfword);
    if (branch !== null && wanted.has(branch)) found.get(branch)!.branches.push(site);

    // Pool words are word-aligned; an unaligned match is a coincidence of two
    // adjacent halfwords, not a reference.
    if ((at & 3) === 0 && at + 3 < image.length) {
      const word =
        (image[at] | (image[at + 1] << 8) | (image[at + 2] << 16) | (image[at + 3] << 24)) >>> 0;
      if (wanted.has(word)) found.get(word)!.wordRefs.push(site);
      if (wanted.has(word - 1)) found.get(word - 1)!.thumbPointers.push(site);
    }
  }
  return found;
}

export function verdict(xref: Xref): string {
  if (xref.calls.length > 0) return "FUNCTION (called directly)";
  if (xref.thumbPointers.length > 0) return "FUNCTION (indirectly published callback)";
  if (xref.branches.length > 0) return "INTERIOR to another owner — not an entry";
  // Measured: every row reaching this verdict on the main image was ARM-mode.
  // The word reference is the SOURCE ADDRESS OF A RUNTIME COPY — a relocated
  // ARM helper DMA'd or memcpy'd into IWRAM or a stack frame — not a pointer to
  // data. Treat it exactly like an ARM row: skip, no C form.
  if (xref.wordRefs.length > 0)
    return "DATA / RELOCATED-ARM SOURCE — referenced as a word, never branched to; skip";
  // NOT a proof of anything on its own. A genuine continuation is normally
  // reached by FALLING THROUGH from the preceding instruction, which leaves no
  // reference for this tool to find — so a literal pool and a fall-through
  // continuation look identical here. Deciding between them needs a
  // reachability walk from the owner's entry.
  return "NO INBOUND REFERENCE — pool or fall-through continuation; a walk decides";
}

function selfTest(): void {
  // `f000 f802` at 0x08000000 targets 0x08000008.
  if (blTarget(0x08000000, 0xf000, 0xf802) !== 0x08000008) throw new Error("bl decode");
  // `e002` at 0x08000000 branches to 0x08000008.
  if (branchTarget(0x08000000, 0xe002) !== 0x08000008) throw new Error("b decode");
  // Backwards branch: `e7fe` is the classic self-loop, site + 4 - 4.
  if (branchTarget(0x08000000, 0xe7fe) !== 0x08000000) throw new Error("backwards b decode");
  // `d002` (beq) at 0x08000000 targets 0x08000008; 0xdf00 is SWI, not a branch.
  if (conditionalTarget(0x08000000, 0xd002) !== 0x08000008) throw new Error("beq decode");
  if (conditionalTarget(0x08000000, 0xdf00) !== null) throw new Error("SWI is not a branch");

  // A word holding `target + 1` is a published Thumb callback; the same word
  // without the bit is a plain data reference. Distinguishing them is the
  // whole point of the tool.
  const image = new Uint8Array(16);
  const write = (at: number, word: number) => {
    image[at] = word & 0xff;
    image[at + 1] = (word >> 8) & 0xff;
    image[at + 2] = (word >> 16) & 0xff;
    image[at + 3] = (word >>> 24) & 0xff;
  };
  write(0, 0x08001235);
  write(4, 0x08001234);
  const found = scan(image, new Set([0x08001234]));
  const xref = found.get(0x08001234)!;
  if (xref.thumbPointers.length !== 1) throw new Error("the +1 word must be a thumb pointer");
  if (xref.wordRefs.length !== 1) throw new Error("the exact word must be a data reference");
  if (verdict(xref) !== "FUNCTION (indirectly published callback)")
    throw new Error("a published callback outranks a plain data reference");

  const bare = scan(new Uint8Array(8), new Set([0x08001234])).get(0x08001234)!;
  if (!verdict(bare).startsWith("NO INBOUND REFERENCE"))
    throw new Error("no references must read as inconclusive, not as proof of a pool");
  console.log("self-test=ok");
}

function mainImage(): Uint8Array {
  for (const candidate of ["roms/gs1-en.gba", "roms/gs1.gba"]) {
    try {
      return new Uint8Array(readFileSync(join(ROOT, candidate)));
    } catch {
      continue;
    }
  }
  throw new Error("no main-image ROM found under roms/");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const wanted = new Set(
    args
      .filter((argument) => /^(0x)?[0-9a-f]{6,8}$/i.test(argument))
      .map((argument) => {
        const value = parseInt(argument.replace(/^0x/i, ""), 16);
        return value < IMAGE_BASE ? value + IMAGE_BASE : value;
      }),
  );
  if (wanted.size === 0) {
    console.log("usage: main_xref.ts <address>... [--json]");
    process.exitCode = 1;
    return;
  }
  const found = scan(mainImage(), wanted);
  if (args.includes("--json")) {
    console.log(JSON.stringify([...found.values()], null, 2));
    return;
  }
  for (const xref of found.values()) {
    const hex = (value: number) => `0x${value.toString(16).padStart(8, "0")}`;
    console.log(`${hex(xref.address)}  ${verdict(xref)}`);
    console.log(
      `    calls=${xref.calls.length} branches=${xref.branches.length} ` +
        `word_refs=${xref.wordRefs.length} thumb_pointers=${xref.thumbPointers.length}`,
    );
    for (const [label, sites] of [
      ["called from", xref.calls],
      ["branched from", xref.branches],
      ["published at", xref.thumbPointers],
      ["word ref at", xref.wordRefs],
    ] as const) {
      for (const site of sites.slice(0, 6)) console.log(`      ${label} ${hex(site)}`);
      if (sites.length > 6) console.log(`      ... and ${sites.length - 6} more`);
    }
  }
}

if (import.meta.main) main();
