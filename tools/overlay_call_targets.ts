#!/usr/bin/env bun
// Resolve an overlay's `bl` targets correctly.
//
// An overlay `bl` does NOT store a pc-relative displacement the way a linked
// main-image `bl` does. It stores **the target's image offset minus 2**:
//
//     true_target_offset = stored_displacement + 2
//
// Every disassembler — including `tools/overlay_show.ts` — adds the branch's own
// pc, which produces addresses that drift with the call site. The symptoms that
// follow are the ones the project chased for a long time: targets past the end
// of the image, targets landing inside the caller's own body, and two call sites
// with *bit-identical* encodings printing different callees. In
// `resource_39f:1078` the branches at 0x020010c4 and 0x02001114 both print
// `bl 0x02003ec2` yet are different functions.
//
// Measured on `resource_39f`: 723 call sites decode pc-relatively to 689
// distinct addresses, 453 of them beyond the image end; the *stored*
// displacements are only 74 distinct values, and `+2` resolves all 74 onto the
// import veneer table, the overlay's own `call_via` slot, or a real prologue.
// The same collapse holds on 371/372/373/38f/3b8/3bf/3c4/3c8.
//
//   bun tools/overlay_call_targets.ts resource_39f            # whole overlay
//   bun tools/overlay_call_targets.ts resource_39f 1078       # one owner
//   bun tools/overlay_call_targets.ts resource_39f --json
//   bun tools/overlay_call_targets.ts --self-test
import { existsSync, readFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { assembleOverlay } from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
/** Overlays are linked here, so an in-image address is `pool_word - BASE_SHIFT`. */
export const BASE_SHIFT = 0x8000;
export const OVERLAY_BASE = 0x02000000;

/**
 * Decode a Thumb BL pair into the displacement the instruction stores.
 * Returns null when the halfwords are not a BL prefix/suffix pair.
 */
export function storedDisplacement(high: number, low: number): number | null {
  if ((high & 0xf800) !== 0xf000) return null;
  if ((low & 0xf800) !== 0xf800) return null;
  const upper = high & 0x07ff;
  const lower = low & 0x07ff;
  // The prefix carries a signed 11-bit field; sign-extend before combining.
  const signed = upper >= 0x0400 ? upper - 0x0800 : upper;
  return (signed << 12) | (lower << 1);
}

/** The rule: the stored displacement is the target offset minus two. */
export function targetOffset(high: number, low: number): number | null {
  const displacement = storedDisplacement(high, low);
  return displacement === null ? null : displacement + 2;
}

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

function inventory(): OverlayFunction[] {
  const path = join(ROOT, "out", "decomp", "overlays.json");
  if (!existsSync(path)) throw new Error(`missing ${path}; run the overlay inventory first`);
  return (JSON.parse(readFileSync(path, "utf8")) as { functions: OverlayFunction[] }).functions;
}

/**
 * Overlay image bytes, as the reconstruction assembles them. This is the same
 * path `overlay_show` reads, so the offsets here line up with its listing.
 */
function overlayImage(overlay: string): Uint8Array {
  const source = join(ROOT, "assets", "code", `${overlay}_overlay.s`);
  if (!existsSync(source)) throw new Error(`no reconstruction assembly for ${overlay}`);
  return assembleOverlay(source);
}

export interface CallSite {
  /** Offset of the BL prefix halfword within the overlay image. */
  site: number;
  /** Resolved target offset within the overlay image. */
  target: number;
  /** What the target lands on, once classified. */
  kind: "veneer" | "prologue" | "call_via" | "unknown";
}

/**
 * Classify a resolved target. A veneer entry is the 8-byte
 * `ldr r4,[pc,#0] / bx r4 / .word T` shape, so the import's real identity is the
 * main-image address in its trailing word.
 */
export function classify(
  image: Uint8Array,
  target: number,
  prologues: Set<number>,
): { kind: CallSite["kind"]; imported?: number } {
  if (prologues.has(target)) return { kind: "prologue" };
  if (target + 8 <= image.length) {
    const first = image[target] | (image[target + 1] << 8);
    const second = image[target + 2] | (image[target + 3] << 8);
    // ldr r4,[pc,#0] == 0x4c00, bx r4 == 0x4720
    if (first === 0x4c00 && second === 0x4720) {
      const word =
        image[target + 4] |
        (image[target + 5] << 8) |
        (image[target + 6] << 16) |
        (image[target + 7] << 24);
      // The stored word carries the Thumb bit; the import's address is even.
      return { kind: "veneer", imported: word & ~1 };
    }
    // A bare `bx rN` slot is the overlay's own call_via bank.
    if ((first & 0xff87) === 0x4700) return { kind: "call_via" };
  }
  return { kind: "unknown" };
}

export function resolveOverlay(overlay: string, owner?: number): CallSite[] {
  const image = overlayImage(overlay);
  const rows = inventory().filter((row) => row.overlay === overlay);
  const prologues = new Set(rows.filter((row) => row.starts_with_prologue).map((row) => row.offset));
  const spans = owner === undefined
    ? rows.filter((row) => (row.contained_by ?? []).length === 0)
    : rows.filter((row) => row.offset === owner);
  const sites: CallSite[] = [];
  for (const span of spans) {
    const end = Math.min(span.offset + span.span_bytes, image.length - 3);
    for (let at = span.offset; at < end; at += 2) {
      const high = image[at] | (image[at + 1] << 8);
      const low = image[at + 2] | (image[at + 3] << 8);
      const target = targetOffset(high, low);
      if (target === null || target < 0 || target >= image.length) continue;
      sites.push({ site: at, target, kind: classify(image, target, prologues).kind });
    }
  }
  return sites;
}

function selfTest(): void {
  // `f000 fe67` stores 0xcce, so the callee is at 0xcd0 — a real
  // `resource_39f` owner. Measured from the live disassembly.
  if (storedDisplacement(0xf000, 0xfe67) !== 0xcce) throw new Error("displacement decode is wrong");
  if (targetOffset(0xf000, 0xfe67) !== 0xcd0) throw new Error("target rule is wrong");
  // `f002 fe75` appears at two sites in resource_39f:1078 and must resolve to
  // ONE callee from both, which is the whole point of the rule.
  if (targetOffset(0xf002, 0xfe75) !== 0x2cec) throw new Error("veneer target is wrong");
  if (storedDisplacement(0x4770, 0x0000) !== null) throw new Error("non-BL must not decode");
  if (storedDisplacement(0xf000, 0x4770) !== null) throw new Error("BL suffix must be checked");
  // A negative prefix must sign-extend rather than wrap.
  if (storedDisplacement(0xf7ff, 0xf800) !== -0x1000) throw new Error("sign extension is wrong");
  const image = new Uint8Array(16);
  // Trailing word 0x0808a011 — the Thumb bit must be masked off.
  image.set([0x00, 0x4c, 0x20, 0x47, 0x11, 0xa0, 0x08, 0x08], 0);
  const veneer = classify(image, 0, new Set());
  if (veneer.kind !== "veneer" || veneer.imported !== 0x0808a010)
    throw new Error("veneer classification is wrong");
  if (classify(image, 0, new Set([0])).kind !== "prologue")
    throw new Error("a known prologue must win over the veneer shape");
  console.log("self-test=ok");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const overlay = args.find((argument) => /^resource_[0-9a-f]+$/.test(argument));
  if (overlay === undefined) {
    console.log("usage: overlay_call_targets.ts <resource_NNN> [ownerHex] [--json]");
    process.exitCode = 1;
    return;
  }
  const ownerText = args.find((argument) => /^[0-9a-f]{1,4}$/i.test(argument));
  const sites = resolveOverlay(overlay, ownerText === undefined ? undefined : parseInt(ownerText, 16));
  if (args.includes("--json")) {
    console.log(JSON.stringify(sites, null, 2));
    return;
  }
  const image = overlayImage(overlay);
  const prologues = new Set(
    inventory().filter((row) => row.overlay === overlay && row.starts_with_prologue).map((row) => row.offset),
  );
  const distinct = new Map<number, number>();
  for (const site of sites) distinct.set(site.target, (distinct.get(site.target) ?? 0) + 1);
  for (const [target, count] of [...distinct.entries()].sort((a, b) => a[0] - b[0])) {
    const detail = classify(image, target, prologues);
    const named = detail.imported === undefined
      ? ""
      : `  -> Func_${detail.imported.toString(16).padStart(8, "0")}`;
    console.log(
      `  ${(OVERLAY_BASE + target).toString(16)}  x${String(count).padStart(3)}  ${detail.kind}${named}`,
    );
  }
  const kinds: Record<string, number> = {};
  for (const site of sites) kinds[site.kind] = (kinds[site.kind] ?? 0) + 1;
  console.log(
    `\nsites=${sites.length} distinct_targets=${distinct.size} ` +
      Object.entries(kinds).map(([k, v]) => `${k}=${v}`).join(" "),
  );
}

if (import.meta.main) main();
