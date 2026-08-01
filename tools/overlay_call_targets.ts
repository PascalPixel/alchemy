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
//   bun tools/overlay_show.ts resource_39f 1078 -n 180 | \
//     bun tools/overlay_call_targets.ts resource_39f --annotate   # fix the bl names
//   bun tools/overlay_call_targets.ts --self-test
import { existsSync, readFileSync, readdirSync } from "node:fs";
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
export function overlayImage(overlay: string): Uint8Array {
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
  // The inventory's prologue set is not complete — genuine callees it never
  // flagged were being reported as `unknown`, which reads as "suspicious" when
  // the target is an ordinary function. Recognise the prologue shape directly:
  // `push {..., lr}` is 0xb5xx, a bare `push {regs}` is 0xb4xx.
  if (target + 1 < image.length) {
    const opening = image[target] | (image[target + 1] << 8);
    if ((opening & 0xfe00) === 0xb400) return { kind: "prologue" };
  }
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

/**
 * True when a whole-overlay run resolved nothing and must therefore FAIL.
 *
 * Extracted as a pure predicate so the self-test can pin BOTH directions on
 * synthetic input. Naming real overlays as fixtures was the defect in the
 * first attempt: `resource_37b` was chosen as the resolving fixture, a lane
 * banked a row byte-exact, 37b moved into the refusing set, and the test went
 * red because the PROJECT PROGRESSED. That is HANDOVER's own rule -- self-tests
 * go on synthetic input -- violated by the person who wrote it down.
 *
 * A run with explicit bounds is exempt: the caller stated a span, and an empty
 * result there is a real answer about that span.
 */
export function resolvesNothing(siteCount: number, boundCount: number): boolean {
  return siteCount === 0 && boundCount === 0;
}

export function resolveOverlay(overlay: string, owner?: number, ownerEnd?: number): CallSite[] {
  const image = overlayImage(overlay);
  const rows = inventory().filter((row) => row.overlay === overlay);
  const prologues = new Set(rows.filter((row) => row.starts_with_prologue).map((row) => row.offset));
  let spans = owner === undefined
    ? rows.filter((row) => (row.contained_by ?? []).length === 0)
    : rows.filter((row) => row.offset === owner);
  // An owner that is already banked byte-exact is not in the discovery
  // inventory, so the filter above returns nothing and the tool prints an empty
  // listing — which reads as "this function makes no calls" rather than "this
  // function is not in the queue". That is exactly backwards for the most
  // valuable use of a banked sibling: inverting its printed (wrong) `bl` names
  // to recover real import identities. Synthesise the span from the next known
  // row instead, and say so.
  if (owner !== undefined && spans.length === 0) {
    if (owner >= image.length) throw new Error(`owner 0x${owner.toString(16)} is past the image end`);
    const offsets = [...new Set(rows.map((row) => row.offset))].sort((a, b) => a - b);
    const next = offsets.find((offset) => offset > owner);
    const end = ownerEnd ?? next ?? image.length;
    if (ownerEnd === undefined) {
      // The next *unconverted* row can be far past the owner's real end, with
      // other banked functions in between, so this bound over-reads by default.
      // Say so rather than returning a clean-looking listing.
      console.error(
        `note: 0x${owner.toString(16)} is not an unconverted inventory row (already banked?).\n` +
          `      Walking 0x${owner.toString(16)}..0x${end.toString(16)}, bounded by the next ` +
          `unconverted row — this MAY INCLUDE neighbouring banked functions.\n` +
          `      Pass an explicit end offset as a third argument to bound it exactly.`,
      );
    }
    spans = [{ overlay, offset: owner, span_bytes: end - owner } as (typeof rows)[number]];
  }
  const sites: CallSite[] = [];
  for (const span of spans) {
    // An explicit end always wins: it is the caller stating a boundary they have
    // established, against an inventory span that may be wrong or absent.
    const claimed = ownerEnd !== undefined && owner !== undefined ? ownerEnd : span.offset + span.span_bytes;
    const end = Math.min(claimed, image.length - 3);
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

/**
 * Pick the owner/end bounds out of the command line.
 *
 * This used to be an inline `/^[0-9a-f]{1,4}$/` filter, which silently dropped
 * any bound written in the `0x` form every other tool here accepts. The failure
 * was not an error: with both bounds dropped the tool falls back to walking the
 * WHOLE overlay's top-level rows and prints a clean, plausible listing that
 * reads exactly like a per-owner one. A lane spent a round drawing conclusions
 * from a right-looking listing of the wrong function, so the parser now takes
 * both spellings and is covered by the self-test.
 *
 * The overlay name cannot collide: `resource_3af` is longer than four hex
 * digits and carries a prefix, and the flags all start with `--`.
 *
 * SECOND FAILURE OF THE SAME SHAPE, fixed 2026-08-01. A filter drops what it
 * does not recognise, so anything that is neither the overlay name, a known
 * flag, nor a bound vanished — and the tool fell back to whole-overlay mode
 * and printed `sites=0`, which is indistinguishable from a row that genuinely
 * has no calls. The way to be handed such an argument is not exotic: zsh does
 * NOT word-split an unquoted expansion, so
 *
 *     for r in "3660 36d0"; do bun ... resource_3c9 $r; done
 *
 * passes the single argument `"3660 36d0"`. That returned `sites=0` for six
 * spans in a row and was only caught because the reader already knew one of
 * the six was 3. `overlay_show` was given the same treatment earlier tonight;
 * this is its sibling and it had the same hole.
 *
 * So `parseBounds` now CONSUMES rather than filters: every argument must be
 * accounted for, and an unrecognised or surplus one throws with the offending
 * text quoted. A tool that accepts arguments it does not use will eventually
 * be handed one that mattered.
 */
const KNOWN_FLAGS = new Set(["--self-test", "--json", "--annotate"]);

export function parseBounds(args: string[], overlay?: string): number[] {
  const bounds: number[] = [];
  for (const argument of args) {
    if (KNOWN_FLAGS.has(argument)) continue;
    if (argument === overlay) continue;
    if (overlay === undefined && /^resource_[0-9a-f]+$/.test(argument)) continue;
    if (/^(0x)?[0-9a-f]{1,4}$/i.test(argument)) {
      bounds.push(Number.parseInt(argument.replace(/^0x/i, ""), 16));
      continue;
    }
    throw new Error(
      `overlay_call_targets: unrecognised argument ${JSON.stringify(argument)}.\n` +
        "Bounds are two SEPARATE arguments in either spelling (`1c14 1d0c` or " +
        "`0x1c14 0x1d0c`).\n" +
        "If this looks like two bounds in one string, a shell passed them " +
        "unsplit — zsh does not word-split an unquoted expansion.",
    );
  }
  if (bounds.length > 2) {
    throw new Error(
      `overlay_call_targets: ${bounds.length} bounds given, at most two are used ` +
        "(owner start and end). Refusing rather than silently ignoring the rest.",
    );
  }
  return bounds;
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
  // The annotator must replace the listing's wrong callee, and leave other lines alone.
  const listing = " 2001082:\tf002 fedd \tbl\t0x2003e40\n 2001086:\tmovs r0, #1\n";
  const annotated = annotate(listing, new Map([[0x1082, "Func_0808a010"]]));
  if (!annotated.includes("bl Func_0808a010")) throw new Error("annotation did not apply");
  if (annotated.includes("0x2003e40")) throw new Error("wrong target survived");
  if (!annotated.includes("movs r0, #1")) throw new Error("a non-call line was altered");
  const image = new Uint8Array(16);
  // Trailing word 0x0808a011 — the Thumb bit must be masked off.
  image.set([0x00, 0x4c, 0x20, 0x47, 0x11, 0xa0, 0x08, 0x08], 0);
  const veneer = classify(image, 0, new Set());
  if (veneer.kind !== "veneer" || veneer.imported !== 0x0808a010)
    throw new Error("veneer classification is wrong");
  if (classify(image, 0, new Set([0])).kind !== "prologue")
    throw new Error("a known prologue must win over the veneer shape");
  // A `push {r4, lr}` opening is a prologue even when the inventory missed it.
  const unlisted = new Uint8Array([0x10, 0xb5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]);
  if (classify(unlisted, 0, new Set()).kind !== "prologue")
    throw new Error("an unlisted push prologue must be recognised");
  // Bounds must parse in BOTH spellings. The `0x` form silently produced a
  // whole-overlay listing before this was fixed, which is worse than an error
  // because the output looks correct.
  const bare = parseBounds(["resource_3af", "1c14", "1d0c"]);
  if (bare.length !== 2 || bare[0] !== 0x1c14 || bare[1] !== 0x1d0c)
    throw new Error("bare hex bounds must parse");
  const prefixed = parseBounds(["resource_3af", "0x1c14", "0x1d0c"]);
  if (prefixed.length !== 2 || prefixed[0] !== 0x1c14 || prefixed[1] !== 0x1d0c)
    throw new Error("0x-prefixed bounds must parse");
  const mixed = parseBounds(["resource_3af", "0X1C14", "1d0c", "--annotate", "--json"]);
  if (mixed.length !== 2 || mixed[0] !== 0x1c14 || mixed[1] !== 0x1d0c)
    throw new Error("mixed case and flags must not disturb bounds");
  if (parseBounds(["resource_3af", "--json"]).length !== 0)
    throw new Error("neither the overlay name nor a flag is a bound");
  // An UNRECOGNISED argument must throw rather than vanish. Filtering made a
  // shell mistake indistinguishable from a row with no calls; these five are
  // the ways of being wrong that must not print a plausible answer.
  const rejects: string[][] = [
    // The founding case: zsh handing both bounds through as ONE argument.
    ["resource_3c9", "3660 36d0"],
    // A decimal bound, or any typo, must not be silently dropped.
    ["resource_3c9", "14000"],
    ["resource_3c9", "1c14", "1d0g"],
    // An unknown flag is not a bound and is not ignorable either.
    ["resource_3c9", "--anotate"],
    // More bounds than the tool consumes: refuse rather than drop the rest.
    ["resource_3c9", "1c14", "1d0c", "1e00"],
  ];
  for (const argv of rejects) {
    let threw = false;
    try {
      parseBounds(argv, "resource_3c9");
    } catch {
      threw = true;
    }
    if (!threw) throw new Error(`bad arguments must throw: ${JSON.stringify(argv)}`);
  }
  // ...and the good spellings must still pass with the overlay name consumed.
  const named = parseBounds(["resource_3c9", "0x1c14", "1d0c", "--annotate"], "resource_3c9");
  if (named.length !== 2 || named[0] !== 0x1c14 || named[1] !== 0x1d0c)
    throw new Error("valid bounds must survive the stricter parser");
  // Partial annotation must be DETECTABLE. This is the check that would have
  // caught the no-bounds bug: the resolver covering none of the listing's calls
  // is just the extreme case of covering some of them.
  const twoCalls =
    " 2000388:\tb520      \tpush\t{r5, lr}\n" +
    " 20003b4:\tf004 f9dd \tbl\t0x2004772\n" +
    " 20003ee:\tf004 f9f5 \tbl\t0x20047dc\n";
  if (unannotatedCallSites(twoCalls, new Map()).length !== 2)
    throw new Error("an unresolved listing must report every bl site");
  const half = unannotatedCallSites(twoCalls, new Map([[0x3b4, "Func_080770c0"]]));
  if (half.length !== 1 || half[0] !== 0x3ee)
    throw new Error("partial coverage must report exactly the uncovered site");
  const full = new Map([
    [0x3b4, "Func_080770c0"],
    [0x3ee, "Func_0808a038"],
  ]);
  if (unannotatedCallSites(twoCalls, full).length !== 0)
    throw new Error("full coverage must report nothing");
  if (!annotate(twoCalls, full).includes("bl Func_0808a038"))
    throw new Error("a fully covered listing must still annotate");
  // Conditional branches are not calls. `bls`/`blt` share the first two letters
  // and must not be demanded of the resolver.
  const conditional = " 20012f4:\td90a      \tbls.n\t0x200130c\n";
  if (unannotatedCallSites(conditional, new Map()).length !== 0)
    throw new Error("bls is a branch, not a call site");
  // Pool-word footer lines carry no colon and must never be read as sites.
  if (unannotatedCallSites("  0x20003fc = 0x02000240\n", new Map()).length !== 0)
    throw new Error("a pool footer line is not a call site");
  // The refusal decision, both directions, on SYNTHETIC input. No overlay is
  // named, so no lane's progress can turn this red.
  if (!resolvesNothing(0, 0)) throw new Error("self-test: an empty whole-overlay run must refuse");
  if (resolvesNothing(1, 0)) throw new Error("self-test: a run with sites must not refuse");
  if (resolvesNothing(0, 2)) throw new Error("self-test: an explicitly bounded run must not refuse");

  // And the wiring, as a tree-INDEPENDENT invariant rather than a fixture:
  // whatever the tree currently looks like, the exit code must agree with what
  // was printed. exit 1 if and only if `sites=0`. This holds for every overlay
  // in every state, so it cannot rot; a fixture naming one overlay could, and
  // did. The overlay walked is simply the first one present.
  const self = Bun.fileURLToPath(import.meta.url);
  const first = readdirSync(join(ROOT, "assets", "code"))
    .filter((name) => /^resource_[0-9a-f]+_overlay\.s$/.test(name))
    .sort()[0]
    ?.replace("_overlay.s", "");
  if (first !== undefined) {
    const probe = Bun.spawnSync(["bun", self, first], { stdout: "pipe", stderr: "pipe" });
    const printedNothing = probe.stdout.toString().includes("sites=0 ");
    if (printedNothing !== (probe.exitCode !== 0)) {
      throw new Error(
        `self-test: ${first} printed sites=0=${printedNothing} but exited ${probe.exitCode} — ` +
          "the exit code must agree with the result",
      );
    }
  }
  // An unknown overlay must fail loudly, and that is tree-independent too.
  if (Bun.spawnSync(["bun", self, "resource_ffffff"], { stdout: "pipe", stderr: "pipe" }).exitCode === 0)
    throw new Error("self-test: an unknown overlay must NOT exit 0");

  console.log("self-test=ok (including empty whole-overlay refusal)");
}

/**
 * Rewrite an `overlay_show` listing so each `bl` names its REAL callee.
 *
 * Three separate lanes wrote this by hand before it was promoted here, which is
 * the argument for it existing: the listing's own `bl` annotations are wrong for
 * every overlay, and hand-pairing the summary histogram against call shapes has
 * already produced one exactly-backwards mapping. Annotating in place removes
 * that whole class of error.
 */
export function annotate(listing: string, sites: Map<number, string>): string {
  return listing
    .split("\n")
    .map((line) => {
      const at = /^\s*([0-9a-f]+):/.exec(line);
      if (at === null) return line;
      const site = Number.parseInt(at[1], 16) - OVERLAY_BASE;
      const name = sites.get(site);
      if (name === undefined) return line;
      return line.replace(/\bbl\s+\S+/, `bl ${name}`);
    })
    .join("\n");
}

/**
 * Every `bl` line in the listing whose site the resolver did NOT cover.
 *
 * This exists because partial annotation is indistinguishable from complete
 * annotation by eye. Run without bounds, `--annotate` resolves only
 * *unconverted inventory rows*, so a listing for a banked or published-population
 * row came back with every `bl` untouched — and an untouched `bl` in an overlay
 * listing carries objdump's pc-relative target, which is wrong. Measured on
 * `resource_39e` 0x02000388: 0 of 2 sites annotated with no bounds, 2 of 2 with
 * `388 414`. No error, no warning, and the output looks exactly like a listing
 * that had nothing worth renaming.
 *
 * Requiring bounds alone would not be enough: bounds that merely *disagree* with
 * the `overlay_show` half annotate the overlap and leave the rest, equally
 * quietly. So the invariant is checked on the listing itself — every call site
 * present must have been resolved — which catches both spellings of the mistake.
 *
 * Matches `bl ` with a trailing space so that `bls`, `blt` and friends, which are
 * conditional branches and not calls, are not counted.
 */
export function unannotatedCallSites(listing: string, sites: Map<number, string>): number[] {
  const missed: number[] = [];
  for (const line of listing.split("\n")) {
    const at = /^\s*([0-9a-f]+):/.exec(line);
    if (at === null) continue;
    if (!/\bbl\s/.test(line)) continue;
    const site = Number.parseInt(at[1], 16) - OVERLAY_BASE;
    if (!sites.has(site)) missed.push(site);
  }
  return missed;
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const overlay = args.find((argument) => /^resource_[0-9a-f]+$/.test(argument));
  if (overlay === undefined) {
    console.log(
      "usage: overlay_call_targets.ts <resource_NNN> [ownerHex [endHex]] [--json|--annotate]\n" +
        "       bounds take either spelling: `1c14 1d0c` or `0x1c14 0x1d0c`",
    );
    process.exitCode = 1;
    return;
  }
  const bounds = parseBounds(args, overlay);
  const sites = resolveOverlay(overlay, bounds[0], bounds[1]);
  if (args.includes("--json")) {
    console.log(JSON.stringify(sites, null, 2));
    return;
  }
  if (args.includes("--annotate")) {
    // Read an overlay_show listing on stdin and rewrite its bl names in place.
    if (bounds.length === 0) {
      throw new Error(
        "--annotate needs the SAME bounds as the overlay_show that produced the listing:\n" +
          "  overlay_show <ov> A B | overlay_call_targets <ov> A B --annotate\n" +
          "Without bounds only unconverted inventory rows resolve, so a banked or " +
          "published-population row's listing keeps objdump's pc-relative — and therefore " +
          "wrong — bl targets, with no error and nothing to see.",
      );
    }
    const image = overlayImage(overlay);
    const prologues = new Set(
      inventory().filter((row) => row.overlay === overlay && row.starts_with_prologue).map((row) => row.offset),
    );
    const names = new Map<number, string>();
    for (const site of sites) {
      const detail = classify(image, site.target, prologues);
      names.set(
        site.site,
        detail.imported !== undefined
          ? `Func_${detail.imported.toString(16).padStart(8, "0")}`
          : `Func_${(OVERLAY_BASE + site.target).toString(16)}`,
      );
    }
    const listing = readFileSync(0, "utf8");
    const missed = unannotatedCallSites(listing, names);
    if (missed.length > 0) {
      const shown = missed
        .slice(0, 5)
        .map((site) => `0x${(OVERLAY_BASE + site).toString(16)}`)
        .join(", ");
      throw new Error(
        `${missed.length} bl site(s) in the listing were not resolved, starting at ${shown}.\n` +
          "The bounds passed here do not cover the listing on stdin. Pass the SAME bounds to " +
          "both halves of the pipe — a partially annotated listing is indistinguishable by eye " +
          "from a fully annotated one, and every unresolved bl still carries objdump's wrong " +
          "pc-relative target.",
      );
    }
    console.log(annotate(listing, names));
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
  // RESOLVING NOTHING IS NOT A RESULT. The whole-overlay path walks only
  // UNCONVERTED inventory rows, so on a well-advanced overlay it can print
  // `sites=0 distinct_targets=0` at exit 0 -- which reads as "this overlay
  // makes no calls". Thirty of ninety-six overlays are in that state, and they
  // are the well-advanced ones: resource_380, 39e, 3a4 and 3c9 among them. The
  // failure gets worse as the work gets better, which is the worst direction
  // for a tool a certification leans on.
  //
  // MEASURED CAUSE, because the first draft of this message GUESSED one and was
  // wrong. It said "every row is already banked byte-exact"; the refusing
  // overlays do still carry top-level inventory rows. What they carry is rows
  // lying PAST THE LAST RECORDED OWNER -- veneer-bank stubs and data-tail
  // fragments, which hold no call sites by construction. Across the 30, the
  // walked rows are ~100% past the last owner (resource_382 3,324B, 396 4,534B,
  // 38c 3,644B, all entirely so), against a median 8,012 bytes of real body
  // walked in the 66 that resolve. resource_3c9 refuses while having known
  // undrafted rows, and that is CORRECT: those rows were found by sweeps, not
  // by the inventory, so the inventory has nothing left to walk.
  //
  // So the message states what is measured -- nothing was resolved -- and does
  // not assert why.
  //
  // The single-owner path was already fixed for exactly this -- it synthesises
  // a span from the next known row and prints a `note:`. The sibling
  // whole-overlay branch was not. A fix applied to one branch and not its twin
  // is the same blind spot in a new place: it lives in what the tool ACCEPTS
  // as a completed run, not in what it scans.
  if (resolvesNothing(sites.length, bounds.length)) {
    console.log(
      "NOTHING RESOLVED — this is a FAILURE, not a pass.\n" +
        `  The whole-overlay path walks only UNCONVERTED inventory rows, and for ${overlay}\n` +
        "  those rows yielded no call site. Typically what remains are veneer-bank and\n" +
        "  data-tail fragments, but this tool has NOT established that, and this is in no\n" +
        "  case evidence that the overlay makes no calls.\n" +
        "  Pass explicit owner bounds — `overlay_call_targets.ts <overlay> START END` —\n" +
        "  which synthesises the span and resolves it properly.",
    );
    process.exitCode = 1;
  }
}

if (import.meta.main) main();
