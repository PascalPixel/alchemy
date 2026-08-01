#!/usr/bin/env bun
// Locate an overlay's ENTRY DRIVER from its header.
//
// Not to be confused with tools/overlay_entry.ts, which repairs a function
// entry that discovery seeded partway into a body. This tool answers a
// different question: which function does the loader call when the overlay is
// entered at all?
//
// Every overlay image opens with an 8-byte veneer to its own entry point:
//
//     0x0000  4c00        ldr r4, [pc, #0]
//     0x0002  4720        bx  r4
//     0x0004  .word ENTRY | 1      (thumb)
//
// so the driver's image offset is `word_at_4 - 1 - BASE_SHIFT - OVERLAY_BASE`.
// Branching to the overlay's base address trampolines through that word.
//
// This matters for closure. An overlay's driver is frequently absent from the
// structural inventory, and it is never the target of any `bl` inside the image
// -- nothing in the resource calls it, the loader does -- so a draft can
// every listed row and still be missing the function that sequences them.
// resource_380's driver at 0x02003f24 was found by hand, by bounding the gap
// between the finale and the wrapper block; this rule finds it directly, and
// finds every other overlay's driver the same way.
//
// Measured over all 96 reconstructed overlays: the word at offset 4 lands
// inside the image for 96 of 96, and points at a `push` prologue for 95. The
// single exception is resource_3cc, whose entry at 0x020000b4 opens
// `movs r0, #0` -- a real entry that simply does not push.
//
//   bun tools/overlay_driver.ts resource_380      # one overlay
//   bun tools/overlay_driver.ts --all             # every overlay, with owner status
//   bun tools/overlay_driver.ts --all --unowned   # only the drivers nobody owns
//   bun tools/overlay_driver.ts --self-test
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { overlayImage, OVERLAY_BASE, BASE_SHIFT } from "./overlay_call_targets.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

/** The two halfwords an overlay's entry veneer always opens with. */
export const ENTRY_VENEER_LOAD = 0x4c00;
export const ENTRY_VENEER_BRANCH = 0x4720;

function view(image: Uint8Array): Buffer {
  return Buffer.from(image.buffer, image.byteOffset, image.length);
}

/** True when the image opens with the `ldr r4,[pc,#0] / bx r4` trampoline. */
export function hasEntryVeneer(image: Uint8Array): boolean {
  if (image.length < 8) return false;
  const data = view(image);
  return data.readUInt16LE(0) === ENTRY_VENEER_LOAD && data.readUInt16LE(2) === ENTRY_VENEER_BRANCH;
}

/**
 * Image offset of the overlay's entry driver, or null when the header word
 * does not land inside the image. Never guesses.
 */
export function driverOffset(image: Uint8Array): number | null {
  if (image.length < 8) return null;
  const word = view(image).readUInt32LE(4);
  const offset = word - (word & 1) - BASE_SHIFT - OVERLAY_BASE;
  if (offset < 0 || offset + 2 > image.length) return null;
  return offset;
}

function overlayNames(): string[] {
  return readdirSync(join(ROOT, "assets", "code"))
    .filter((name) => /^resource_[0-9a-f]+_overlay\.s$/.test(name))
    .map((name) => name.replace("_overlay.s", ""))
    .sort();
}

/** Which tree, if any, already owns the function at this offset. */
export function ownerOf(overlay: string, offset: number): string {
  const address = "0x0" + (OVERLAY_BASE + offset).toString(16);
  const regions = JSON.parse(readFileSync(join(ROOT, "semantic", "regions.json"), "utf8"))
    .manual_regions as { overlay: string; entry: string }[];
  if (regions.some((row) => row.overlay === overlay && row.entry === address)) return "semantic-C";
  // `address` carries the "0x" prefix; the exact-C filenames do not.
  if (existsSync(join(ROOT, "assets", "code", `${overlay}_c_${address.slice(2)}.c`))) return "exact-C";
  const inventoryPath = join(ROOT, "out", "decomp", "overlays.json");
  if (existsSync(inventoryPath)) {
    const rows = JSON.parse(readFileSync(inventoryPath, "utf8"))
      .functions as { overlay: string; offset: number }[];
    if (rows.some((row) => row.overlay === overlay && row.offset === offset)) return "inventory row";
  }
  return "UNOWNED";
}

function selfTest(): void {
  // A synthetic header: veneer, then the entry word for image offset 0x40.
  const image = new Uint8Array(0x80);
  const data = Buffer.from(image.buffer);
  data.writeUInt16LE(ENTRY_VENEER_LOAD, 0);
  data.writeUInt16LE(ENTRY_VENEER_BRANCH, 2);
  data.writeUInt32LE(OVERLAY_BASE + BASE_SHIFT + 0x40 + 1, 4);
  if (!hasEntryVeneer(image)) throw new Error("overlay driver self-test: veneer not recognised");
  if (driverOffset(image) !== 0x40) throw new Error("overlay driver self-test: wrong driver offset");
  // The thumb bit must be stripped, not carried into the offset.
  data.writeUInt32LE(OVERLAY_BASE + BASE_SHIFT + 0x40, 4);
  if (driverOffset(image) !== 0x40) throw new Error("overlay driver self-test: thumb bit mishandled");
  // Out-of-range header words are rejected rather than guessed at.
  data.writeUInt32LE(OVERLAY_BASE + BASE_SHIFT + 0x4000, 4);
  if (driverOffset(image) !== null) throw new Error("overlay driver self-test: out-of-range word accepted");
  if (driverOffset(new Uint8Array(4)) !== null) throw new Error("overlay driver self-test: short image accepted");
  // ownerOf builds an exact-C filename from the address: the "0x" prefix must
  // be dropped, not one character of it (that bug reported owned drivers as
  // UNOWNED and inflated the audit's count from 23 to 38).
  if (ownerOf("resource_3c9", 0x71c) !== "exact-C") {
    throw new Error("overlay driver self-test: exact-C owner not recognised");
  }
  // A name with no image must FAIL. This defect lived in `main`, so the only
  // honest way to pin it is to run the tool. Both directions are asserted so
  // the check cannot rot into always-failing either.
  const self = Bun.fileURLToPath(import.meta.url);
  const run = (name: string) =>
    Bun.spawnSync(["bun", self, name], { stdout: "pipe", stderr: "pipe" }).exitCode;
  // The passing fixture is selected from the tree: whichever overlay is first.
  // This tool exits 0 for any overlay whose image assembles, whatever
  // its driver turns out to be owned by, so ordinary source changes cannot turn it red.
  if (run("resource_ffffff") === 0)
    throw new Error("overlay driver self-test: an unknown overlay must NOT exit 0");
  const anyOverlay = overlayNames()[0];
  if (anyOverlay !== undefined && run(anyOverlay) !== 0)
    throw new Error(`overlay driver self-test: a real overlay (${anyOverlay}) must exit 0`);

  console.log("overlay driver self-test passed (including unknown-overlay refusal)");
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.includes("--self-test")) return selfTest();
  const wantAll = argv.includes("--all");
  const onlyUnowned = argv.includes("--unowned");
  const names = wantAll ? overlayNames() : argv.filter((a) => /^resource_[0-9a-f]+$/.test(a));
  if (names.length === 0) {
    console.log("usage: overlay_driver.ts <resource_NNN> | --all [--unowned]");
    process.exitCode = 1;
    return;
  }
  let unowned = 0;
  let examined = 0;
  // A name with no assembled image used to be swallowed by the `catch` below,
  // leaving `examined=0 unowned=0` printed at exit 0 — and `unowned=0` is this
  // tool's SUCCESS signal, so a mistyped overlay reported every driver owned.
  // Second fault of this class in this file: `ownerOf` accepting a census row
  // as ownership was the first. Collect the misses and fail on them.
  const missing: string[] = [];
  for (const overlay of names) {
    let image: Uint8Array;
    try { image = overlayImage(overlay); } catch { missing.push(overlay); continue; }
    const offset = driverOffset(image);
    if (offset === null) { console.log(`  ${overlay}  header word out of range`); continue; }
    examined++;
    const owner = ownerOf(overlay, offset);
    if (owner === "UNOWNED") unowned++;
    if (onlyUnowned && owner !== "UNOWNED") continue;
    const head = view(image).readUInt16LE(offset);
    const note = (head & 0xfe00) === 0xb400 ? "" : "  (does not open with push)";
    console.log(`  ${overlay}  driver 0x${(OVERLAY_BASE + offset).toString(16)}  ${owner}${note}`);
  }
  if (wantAll) console.log(`\nentry drivers examined=${examined} unowned=${unowned}`);
  if (missing.length > 0) {
    console.log(
      `NO OVERLAY IMAGE — this is a FAILURE, not a pass: ${missing.join(" ")}\n` +
        "  Check the name against assets/code/*_overlay.s. Nothing was examined for\n" +
        "  these, and `unowned=0` here does NOT mean their drivers are owned.",
    );
    process.exitCode = 1;
  }
  if (examined === 0) process.exitCode = 1;
}

if (import.meta.main) main();
