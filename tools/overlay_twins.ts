#!/usr/bin/env bun
// Find owners that are the SAME routine shared verbatim across overlays.
//
// Overlays reuse whole routines. `resource_3c4`'s owners at 0x00c4, 0x02a8,
// 0x0474 and 0x0ae8 are halfword-identical to `resource_39f`'s owners at the
// same offsets, differing only in `bl` displacements (each overlay's veneer
// table sits at a different offset) and in literal-pool words. That is 1,424
// bytes convertible by transposing an existing source instead of reading
// assembly — and the same trick spans the whole overlay set.
//
// The comparison therefore masks the two things that legitimately differ:
//   * both halfwords of every BL pair, and
//   * any halfword inside the span that is not reached as an instruction
//     (literal pools), approximated by masking words that decode as pool-like.
// What remains is the instruction skeleton. Two owners with the same masked
// digest are the same routine.
//
//   bun tools/overlay_twins.ts                    # every twin group
//   bun tools/overlay_twins.ts resource_383       # groups touching one overlay
//   bun tools/overlay_twins.ts --unconverted      # only groups with work left
//   bun tools/overlay_twins.ts --semantic --unconverted
//                                                # reviewed-C templates too
//   bun tools/overlay_twins.ts --leads            # families with no member solved
//   bun tools/overlay_twins.ts --self-test
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { assembleOverlay, overlayCSpans } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const USAGE = "usage: overlay_twins.ts [resource_NNN] [--unconverted] [--semantic] [--self-test]";

interface OverlayFunction {
  overlay: string;
  offset: number;
  span_bytes: number;
  starts_with_prologue: boolean;
  returns: number;
  structural_veneer: boolean;
  data_walk: boolean;
  contained_by: unknown[];
}

/**
 * Digest an owner's instruction skeleton, masking what differs legitimately
 * between two copies of the same routine in different overlays.
 *
 * A BL pair is masked because the displacement encodes the callee's offset in
 * *this* overlay's image (see overlay_call_targets.ts), and the veneer table
 * sits at a different offset in each overlay. Masking the pair rather than the
 * whole instruction keeps the call's *position* significant, which is what makes
 * the match meaningful.
 */
export function skeletonDigest(image: Uint8Array, offset: number, size: number): string {
  const hasher = new Bun.CryptoHasher("sha256");
  const end = Math.min(offset + size, image.length);
  for (let at = offset; at + 1 < end; at += 2) {
    const halfword = image[at] | (image[at + 1] << 8);
    const isBlPrefix = (halfword & 0xf800) === 0xf000;
    if (isBlPrefix && at + 3 < end && (((image[at + 2] | (image[at + 3] << 8)) & 0xf800) === 0xf800)) {
      // Mask both halves of the pair, but record that a call happened here.
      hasher.update("CALL");
      at += 2;
      continue;
    }
    // A pointer-shaped word is a literal-pool entry naming an address that
    // differs per overlay — the same data table lives at a different in-image
    // offset in each. Masking it was necessary, not cosmetic: without it the
    // known resource_3c4/resource_39f twins differ by exactly ONE halfword out
    // of 192 and are missed. This can also mask a real instruction pair whose
    // upper half happens to look like 0x0200-0x09ff; that costs a little
    // discrimination and is why size is part of the group key.
    if ((at & 3) === 0 && at + 3 < end) {
      const word = halfword | ((image[at + 2] | (image[at + 3] << 8)) << 16);
      const region = (word >>> 24) & 0xff;
      if (region === 0x02 || region === 0x03 || region === 0x08 || region === 0x09) {
        hasher.update("PTR");
        at += 2;
        continue;
      }
    }
    hasher.update(new Uint8Array([halfword & 0xff, (halfword >> 8) & 0xff]));
  }
  return hasher.digest("hex").slice(0, 16);
}

function inventory(): OverlayFunction[] {
  const path = join(ROOT, "out", "decomp", "overlays.json");
  if (!existsSync(path)) throw new Error(`missing ${path}`);
  return (JSON.parse(readFileSync(path, "utf8")) as { functions: OverlayFunction[] }).functions;
}

/** Exact-C owners, optionally joined by reviewed semantic-C owners. */
function convertedKeys(includeSemantic = false): Set<string> {
  const keys = new Set<string>();
  const path = join(ROOT, "exact");
  if (existsSync(path)) {
    for (const name of readdirSync(path)) {
      const match = /^(resource_[0-9a-f]+)_c_0200([0-9a-f]{4})\.c$/.exec(name);
      if (match !== null) keys.add(`${match[1]}:${match[2]}`);
    }
  }
  if (includeSemantic) {
    const semantic = join(ROOT, "semantic");
    if (existsSync(semantic)) {
      for (const name of readdirSync(semantic)) {
        const match = /^(resource_[0-9a-f]+)_c_0200([0-9a-f]{4})\.c$/.exec(name);
        if (match !== null) keys.add(`${match[1]}:${match[2]}`);
      }
    }
  }
  return keys;
}

/**
 * Exact-C owners disappear from `out/decomp/overlays.json`: the inventory is
 * deliberately a discovery queue, not a complete symbol table.  That made the
 * twin finder blind to the most valuable member of a family — the verified C
 * template.  Recover those rows from the compiler-produced extents that the
 * overlay builder already exposes, then compare their rebuilt bytes alongside
 * the remaining assembly owners.
 */
function exactRows(): OverlayFunction[] {
  const code = join(ROOT, "assets", "code");
  if (!existsSync(code)) return [];
  const rows: OverlayFunction[] = [];
  for (const name of readdirSync(code).filter((entry) => /^resource_[0-9a-f]+_overlay\.s$/.test(entry)).sort()) {
    const overlay = name.replace(/_overlay\.s$/, "");
    for (const span of overlayCSpans(join(code, name))) {
      rows.push({
        overlay,
        offset: span.start,
        span_bytes: span.end - span.start,
        starts_with_prologue: true,
        returns: 1,
        structural_veneer: false,
        data_walk: false,
        contained_by: [],
      });
    }
  }
  return rows;
}

export interface Twin {
  overlay: string;
  offset: number;
  bytes: number;
  converted: boolean;
}

export function twinGroups(includeSemantic = false): Twin[][] {
  const discovered = inventory();
  const known = new Set(discovered.map((row) => `${row.overlay}:${row.offset}`));
  const rows = [...discovered, ...exactRows().filter((row) => !known.has(`${row.overlay}:${row.offset}`))].filter(
    (row) =>
      row.starts_with_prologue &&
      row.returns > 0 &&
      !row.structural_veneer &&
      !row.data_walk &&
      (row.contained_by ?? []).length === 0,
  );
  const converted = convertedKeys(includeSemantic);
  const images = new Map<string, Uint8Array>();
  const groups = new Map<string, Twin[]>();
  for (const row of rows) {
    if (!images.has(row.overlay)) {
      const source = join(ROOT, "assets", "code", `${row.overlay}_overlay.s`);
      if (!existsSync(source)) continue;
      images.set(row.overlay, assembleOverlay(source));
    }
    const image = images.get(row.overlay);
    if (image === undefined || row.offset + row.span_bytes > image.length) continue;
    // Size is part of the key: two routines of different length are not twins.
    const key = `${row.span_bytes}:${skeletonDigest(image, row.offset, row.span_bytes)}`;
    const twin: Twin = {
      overlay: row.overlay,
      offset: row.offset,
      bytes: row.span_bytes,
      converted: converted.has(`${row.overlay}:${row.offset.toString(16).padStart(4, "0")}`),
    };
    groups.set(key, [...(groups.get(key) ?? []), twin]);
  }
  return [...groups.values()]
    .filter((group) => group.length > 1)
    .sort((left, right) => {
      const leftGain = left.filter((t) => !t.converted).reduce((sum, t) => sum + t.bytes, 0);
      const rightGain = right.filter((t) => !t.converted).reduce((sum, t) => sum + t.bytes, 0);
      return rightGain - leftGain;
    });
}

function selfTest(): void {
  // Two images identical except for the BL displacement must digest the same.
  const left = new Uint8Array([0x10, 0xb5, 0x00, 0xf0, 0x02, 0xf8, 0x10, 0xbd]);
  const right = new Uint8Array([0x10, 0xb5, 0x02, 0xf0, 0x75, 0xfe, 0x10, 0xbd]);
  if (skeletonDigest(left, 0, 8) !== skeletonDigest(right, 0, 8))
    throw new Error("BL displacement must be masked");
  // A different instruction must NOT digest the same.
  const other = new Uint8Array([0x10, 0xb5, 0x00, 0xf0, 0x02, 0xf8, 0x00, 0xbd]);
  if (skeletonDigest(left, 0, 8) === skeletonDigest(other, 0, 8))
    throw new Error("a real instruction difference must survive");
  // A call's position is significant: moving it changes the digest.
  const moved = new Uint8Array([0x00, 0xf0, 0x02, 0xf8, 0x10, 0xb5, 0x10, 0xbd]);
  if (skeletonDigest(left, 0, 8) === skeletonDigest(moved, 0, 8))
    throw new Error("call position must stay significant");
  // Pointer-shaped pool words differing per overlay must be masked.
  const poolA = new Uint8Array([0x10, 0xb5, 0x00, 0x00, 0xf8, 0xae, 0x00, 0x02]);
  const poolB = new Uint8Array([0x10, 0xb5, 0x00, 0x00, 0xf0, 0xb1, 0x00, 0x02]);
  if (skeletonDigest(poolA, 0, 8) !== skeletonDigest(poolB, 0, 8))
    throw new Error("pointer pool words must be masked");
  console.log("self-test=ok");
}

/**
 * Families where nothing is converted yet.
 *
 * `--unconverted` answers "what can I transpose today" and therefore requires a
 * solved member. That deliberately hides the larger prize: a family of N with no
 * solved member costs one reading and then pays out the other N-1 by
 * transposition. Ranking by that payout rather than by family size puts the
 * reading effort where it multiplies most.
 */
function reportLeads(groups: Twin[][]): void {
  const leads = groups
    .filter((group) => group.every((twin) => !twin.converted))
    .map((group) => ({ group, unlock: (group.length - 1) * group[0].bytes }))
    .sort((left, right) => right.unlock - left.unlock);
  for (const { group, unlock } of leads) {
    const first = group[0];
    console.log(
      `${String(unlock).padStart(6)}B unlocked  ${group.length}x${first.bytes}B  ` +
        `read ${first.overlay}:${first.offset.toString(16).padStart(4, "0")} ` +
        `-> transpose ${group.length - 1}`,
    );
  }
  const total = leads.reduce((sum, lead) => sum + lead.unlock, 0);
  const reading = leads.reduce((sum, lead) => sum + lead.group[0].bytes, 0);
  console.log(`\nleads=${leads.length} unlock_bytes=${total} reading_bytes=${reading}`);
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--help") || args.includes("-h")) {
    console.log(USAGE);
    return;
  }
  if (args.includes("--self-test")) return selfTest();
  const unknown = args.find((argument) =>
    !/^resource_[0-9a-f]+$/.test(argument) &&
    !["--unconverted", "--semantic", "--leads"].includes(argument));
  if (unknown !== undefined) throw new Error(`${USAGE}\nunrecognised argument: ${unknown}`);
  if (args.filter((argument) => /^resource_[0-9a-f]+$/.test(argument)).length > 1) {
    throw new Error(`${USAGE}\nselect at most one overlay`);
  }
  const only = args.find((argument) => /^resource_[0-9a-f]+$/.test(argument));
  const unconvertedOnly = args.includes("--unconverted");
  const includeSemantic = args.includes("--semantic");
  let groups = twinGroups(includeSemantic);
  if (only !== undefined) groups = groups.filter((g) => g.some((t) => t.overlay === only));
  if (args.includes("--leads")) return reportLeads(groups);
  if (unconvertedOnly) {
    // A reusable family needs BOTH a source and an unconverted sibling. Groups
    // with no source are discovery leads, not recoverable bytes; excluding
    // them here keeps the printed count equal to the rows the user can act on.
    groups = groups.filter((group) =>
      group.some((twin) => twin.converted) && group.some((twin) => !twin.converted));
  }
  let recoverable = 0;
  for (const group of groups) {
    const done = group.filter((t) => t.converted);
    const todo = group.filter((t) => !t.converted);
    recoverable += todo.reduce((sum, t) => sum + t.bytes, 0);
    console.log(
      `${group[0].bytes} bytes x${group.length}` +
        (done.length > 0 ? `  source: ${done[0].overlay}:${done[0].offset.toString(16)}` : "  (none converted yet)"),
    );
    for (const twin of todo) {
      console.log(`    TODO ${twin.overlay}:${twin.offset.toString(16).padStart(4, "0")}`);
    }
  }
  console.log(`\ngroups=${groups.length} recoverable_bytes=${recoverable} mode=${includeSemantic ? "semantic" : "exact"}`);
}

if (import.meta.main) main();
