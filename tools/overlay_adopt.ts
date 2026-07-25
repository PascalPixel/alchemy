#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md.
// Adopt a byte-exact overlay C reconstruction: install it as the overlay's
// `_c_<address>.c` sibling and swap the reconstruction assembly for the
// `AlchemyC_<address>: / .space` placeholder the asset builder expects.
//
// The swap is only committed when the rebuilt overlay is byte-identical to the
// overlay as it stands today, so adoption can never change the ROM image. The
// region's instructions are located with the assembler's own listing rather
// than by counting directive widths: Thumb encodings are 2 or 4 bytes and a
// hand-rolled width table would be wrong exactly where it matters.
import { copyFileSync, existsSync, mkdtempSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { basename, dirname, join } from "node:path";
import { assembleOverlay, OVERLAY_BASE } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const CODE = join(ROOT, "assets/code");
// Listing rows are "<line> <offset> <bytes> <source>"; continuation rows of a
// wide directive repeat the line number and omit the offset. GNU as prints the
// offset in lowercase but the byte column in uppercase, so the byte column has
// to be matched case-insensitively: a lowercase-only class silently drops every
// row whose first byte is >= 0xa0, which shortens the region by those rows and
// makes a byte-exact candidate fail the rebuild by the dropped byte count.
const LISTING_ROW = /^\s*(\d+)\s+([0-9a-f]{4,})\s+[0-9A-Fa-f]/;
const LOCAL_LABEL = /^\s*(\.L_[0-9a-f]+):/;

interface Options {
  span?: number;
  id: string;
  source: string;
  apply: boolean;
}

interface FunctionRow {
  id: string;
  overlay: string;
  entry: number;
  offset: number;
  span_bytes: number;
}

function optionsOf(argv: string[]): Options {
  const options: Options = { id: "", source: "", apply: false };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--source" || argument === "-s") options.source = argv[++index];
    else if (argument === "--apply") options.apply = true;
    else if (argument === "--span") {
      options.span = Number(argv[++index]);
      if (!Number.isInteger(options.span) || options.span <= 0) throw new Error("--span must be a positive byte count");
    }
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: overlay_adopt.ts <overlay:offsetHex> --source FILE [--span BYTES] [--apply]");
      process.exit(0);
    } else if (options.id === "") options.id = argument;
    else throw new Error(`unrecognized argument: ${argument}`);
  }
  if (options.id === "" || options.source === "") throw new Error("both an overlay function id and --source are required");
  return options;
}

function hex8(value: number): string {
  return value.toString(16).padStart(8, "0");
}

// Source line number -> section offset, straight from the assembler.
function listingOffsets(assembly: string): Map<number, number> {
  const work = mkdtempSync(join(tmpdir(), "alchemy-adopt-"));
  try {
    const listing = join(work, "listing.txt");
    const result = Bun.spawnSync([
      "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
      `-al=${listing}`, "-o", join(work, "listing.o"), assembly,
    ], { stdout: "pipe", stderr: "pipe" });
    if (result.exitCode !== 0) throw new Error(`as failed: ${result.stderr.toString().trim()}`);
    const offsets = new Map<number, number>();
    for (const row of readFileSync(listing, "utf8").split(/\r?\n/)) {
      const found = LISTING_ROW.exec(row);
      if (found === null) continue;
      const line = Number(found[1]);
      if (!offsets.has(line)) offsets.set(line, Number.parseInt(found[2], 16));
    }
    return offsets;
  } finally {
    rmSync(work, { recursive: true, force: true });
  }
}

// The lines that encode [offset, offset + span), as a half-open line range.
function regionLines(offsets: Map<number, number>, offset: number, span: number): [number, number] {
  const inside = [...offsets].filter(([, at]) => at >= offset && at < offset + span).map(([line]) => line);
  if (inside.length === 0) throw new Error(`no assembly covers offset 0x${offset.toString(16)}`);
  const first = Math.min(...inside), last = Math.max(...inside);
  for (const [line, at] of offsets) {
    if (line >= first && line <= last && (at < offset || at >= offset + span)) {
      throw new Error(`lines ${first}-${last} straddle the region boundary at 0x${at.toString(16)}`);
    }
  }
  if (offsets.get(first) !== offset) {
    throw new Error(`region does not start on an encoded boundary: 0x${offsets.get(first)!.toString(16)}`);
  }
  return [first, last];
}

function main(): void {
  const options = optionsOf(Bun.argv.slice(2));
  const inventory = JSON.parse(readFileSync(join(ROOT, "out/decomp/overlays.json"), "utf8")) as { functions: FunctionRow[] };
  const found = inventory.functions.find((row) => row.id === options.id);
  // Discovery seeds from control flow inside the stream, so a function whose
  // only callers live in the main image or in an external pointer table is
  // never inventoried. `--span` adopts such a function from its id alone. It
  // weakens nothing: the region boundary, straddling-label and rehearse-and-
  // compare checks below are what actually gate the splice, and they read the
  // assembly, not the inventory.
  let fn: FunctionRow;
  if (found !== undefined) {
    fn = found;
  } else if (options.span !== undefined) {
    const [overlay, offsetText] = options.id.split(":");
    const offset = Number.parseInt(offsetText, 16);
    if (!Number.isInteger(offset)) throw new Error(`unparseable overlay id: ${options.id}`);
    fn = { id: options.id, overlay, entry: OVERLAY_BASE + offset, offset, span_bytes: options.span } as FunctionRow;
  } else {
    throw new Error(`no such overlay function: ${options.id} (pass --span BYTES to adopt an undiscovered entry)`);
  }
  const stem = hex8(fn.entry);
  if (fn.entry - OVERLAY_BASE !== fn.offset) throw new Error("inventory entry and offset disagree");

  const assembly = join(CODE, `${fn.overlay}_overlay.s`);
  const baseline = assembleOverlay(assembly, OVERLAY_BASE);
  const lines = readFileSync(assembly, "utf8").split("\n");
  const [first, last] = regionLines(listingOffsets(assembly), fn.offset, fn.span_bytes);

  // Adopting a region twice appends a second `AlchemyC_` label in front of the
  // first one's `.space`. The assembler tolerates the duplicate and the ROM
  // still rebuilds byte-identically, so the full build does not catch it; only
  // the inventory's placeholder walk does, one run later. Refuse up front.
  // The label itself emits no bytes, so it has no listing offset and sits just
  // before the region's first encoded line; look a little further back.
  for (const line of lines.slice(Math.max(0, first - 4), last)) {
    if (line.startsWith("AlchemyC_")) throw new Error(`${options.id} is already adopted as C`);
  }

  // A local label inside the region that something outside still branches to
  // would silently lose its definition when the body becomes `.space`.
  const outside = [...lines.slice(0, first - 1), ...lines.slice(last)].join("\n");
  for (const line of lines.slice(first - 1, last)) {
    const label = LOCAL_LABEL.exec(line);
    if (label !== null && outside.includes(label[1])) throw new Error(`label ${label[1]} is referenced from outside the region`);
  }

  const replaced = [
    ...lines.slice(0, first - 1),
    `AlchemyC_${stem}:`,
    `\t.space 0x${fn.span_bytes.toString(16)}`,
    ...lines.slice(last),
  ].join("\n");

  // Rehearse in place rather than in a scratch copy. Compiler flags are routed
  // by repository-relative path (`sourceKey` in alchemy_gcc.ts), so a candidate
  // compiled from a temp directory silently loses every path-keyed flag -- a
  // dry run under /tmp would reject a correct flag-routed match. Both files are
  // restored unless the rebuild is byte-identical and --apply was given.
  const installed = join(CODE, `${fn.overlay}_c_${stem}.c`);
  // Only remove the installed C file if this run created it. A rehearsal over
  // a region that already has one (a dry run, or a repeat) must leave the
  // existing source alone -- deleting it orphans the placeholder and the next
  // inventory run fails on the mismatch.
  const preexisting = existsSync(installed) ? readFileSync(installed) : null;
  const revert = (): void => {
    if (preexisting === null) rmSync(installed, { force: true });
    else writeFileSync(installed, preexisting);
    writeFileSync(assembly, lines.join("\n"));
  };
  let rebuilt: Buffer;
  try {
    copyFileSync(options.source, installed);
    writeFileSync(assembly, replaced);
    rebuilt = assembleOverlay(assembly, OVERLAY_BASE);
  } catch (error) {
    revert();
    throw error;
  }

  if (rebuilt.length !== baseline.length || !rebuilt.equals(baseline)) {
    let differing = Math.abs(rebuilt.length - baseline.length);
    for (let byte = 0; byte < Math.min(rebuilt.length, baseline.length); byte++) {
      if (rebuilt[byte] !== baseline[byte]) differing++;
    }
    revert();
    console.log(`adopt=rejected ${options.id} differing_bytes=${differing} size=${rebuilt.length}/${baseline.length}`);
    process.exitCode = 1;
    return;
  }
  if (!options.apply) {
    revert();
    console.log(`adopt=ready ${options.id} span=${fn.span_bytes} lines=${first}-${last} source=${basename(options.source)} (pass --apply to install)`);
    return;
  }
  console.log(`adopt=applied ${options.id} span=${fn.span_bytes} c=assets/code/${fn.overlay}_c_${stem}.c`);
}

if (import.meta.main) main();
