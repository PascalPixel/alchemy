#!/usr/bin/env bun
// Sweep E: rule what sweep D reports, and count returns PER OWNER.
//
// RECONCILED WITH SWEEP D, 2026-08-01. This tool and `overlay_gaps.ts` were
// built from the same argument -- a Thumb function cannot avoid returning --
// and initially both implemented the
// head, the tail and the veneer/call_via masks. Two instruments answering one
// question diverge, and the day they disagree nobody knows which is
// authoritative. So the questions are divided by evidence, not by tidiness:
//
//   SWEEP D OWNS the head, the interior gaps, the tail scan, and the bank
//   masks. Its `maskBanks` is better than the one that stood here: it pins the
//   veneer by REGISTER AGREEMENT -- a veneer branches to the register it just
//   loaded -- where this file accepted any `bx rM` after any `ldr rN`, which
//   masks `ldr r0,=table / bx lr`, a real published getter stub, as structure.
//   Its `call_via` pad test is strict (`0x46c0` only) where this file also
//   accepted `0x0000`, and strict is right: a leaf followed by a zero pad word
//   would have been masked away as a bank entry by the looser rule. Both
//   differences were measured before adopting, not assumed.
//
//   SWEEP E OWNS the two questions sweep D does not ask:
//
//   1. RULING sweep D's leftover tail returns. `ruleTail` reports RETURN-SUSPECT
//      and stops -- correctly, it is a scanner. This resolves each reported
//      return against the ROM. resource_3b8's tail carries 24 and not one is
//      code: every one is the low half of a 4-aligned Thumb pointer inside a
//      12-byte descriptor record (`0x0200bd41` reads as `pop {r0, r6, pc}`).
//      This class is **a published pointer word whose low halfword wears a
//      return shape**. Keeping one structural name avoids two tools describing
//      the same case differently.
//   2. THE RETURN CENSUS PER OWNER, which nothing else asks at all. A recorded
//      span inflated to its neighbour's start SWALLOWS whatever sits between
//      the real end and that start: sweep D subtracts against the recorded
//      number and sees no gap, sweep C sees no prologue because a leaf has
//      none. The header can carry every other fact correctly and still record
//      the wrong number, and the number is the only part any sweep reads. But
//      a swallowed function still returns, so a span holding TWO returns is the
//      tell, and a span holding NONE does not hold a whole function.
//
// Plus the two corroborations from the resource_380 certification, kept because
// they fail differently from everything above:
//   - every `bl` inside a recorded owner, resolved through `targetOffset` and
//     NEVER hand-rolled, lands in the code region or in a masked bank -- none
//     in tail data;
//   - every image word pointing into tail data is EVEN. A published function
//     entry must carry the Thumb bit; a plain data pointer does not.
//
// What this file deliberately no longer asks: whether the head is export-thunk
// shaped. Sweep D rules the head by return shape with the banks masked, and
// sweep B already resolves every published pointer word in the image including
// the head's. The shape test fired on 45 overlays whose heads legitimately hold
// pointer tables -- 233 of 397 findings, and every one of them noise from a
// question two other sweeps already answer better.
//
//   bun tools/overlay/overlay_certify.ts resource_3b8
//   bun tools/overlay/overlay_certify.ts            # every overlay
//   bun tools/overlay/overlay_certify.ts --json
//   bun tools/overlay/overlay_certify.ts --self-test
import { overlayImage, targetOffset, BASE_SHIFT, OVERLAY_BASE } from "../lib/overlay_call_targets.ts";
import { isReturnShape, maskBanks, ownerSpans, overlayNames, ruleTail } from "./overlay_gaps.ts";

/** Offset an in-image pointer word designates, or null if it points elsewhere. */
export function pointerOffset(word: number, imageLength: number): number | null {
  const offset = (word & ~1) - OVERLAY_BASE - BASE_SHIFT;
  return offset >= 0 && offset < imageLength ? offset : null;
}

export interface Finding {
  /** Image offset the complaint is about. */
  offset: number;
  /** TAIL, OWNER, BL or POINTER. */
  region: string;
  note: string;
}

export interface Certification {
  overlay: string;
  owners: number;
  imageBytes: number;
  tailStart: number;
  tailBytes: number;
  tailVeneers: number;
  tailCallVia: number;
  /** Tail returns sweep D reported, before this tool rules them. */
  tailReturns: number;
  /** Of those, the low halves of published owner pointers. */
  tailPointerHalves: number;
  /** Of those, `bx rN` slots proven to be dispatched by a real `bl`. */
  tailDispatched: number;
  blSites: number;
  blToBank: number;
  blToCode: number;
  tailPointers: number;
  multiReturnOwners: number[];
  findings: Finding[];
}

export function certify(overlay: string): Certification {
  const { spans } = ownerSpans(overlay);
  return certifyImage(overlay, overlayImage(overlay), spans);
}

/**
 * The pure core: an image plus the spans claimed to own it.
 *
 * Separated from `certify` so the self-test runs on SYNTHETIC input. Pinning a
 * self-test to a real overlay is this project's own rule broken -- see the
 * `resolvesNothing` note in `overlay_call_targets.ts`, where `resource_37b` was
 * used as a fixture and the test went red because the PROJECT PROGRESSED.
 */
export function certifyImage(
  overlay: string,
  image: Uint8Array,
  ownerList: { start: number; end: number }[],
): Certification {
  const view = new DataView(image.buffer, image.byteOffset, image.byteLength);
  const halfword = (at: number): number => view.getUint16(at, true);
  const word = (at: number): number => view.getUint32(at, true);

  // Fresh objects: the clamp below must not reach back into the caller's rows.
  const spans = ownerList.map((span) => ({ start: span.start, end: span.end }));
  spans.sort((a, b) => a.start - b.start);
  const starts = new Set(spans.map((span) => span.start));
  const findings: Finding[] = [];

  const empty: Certification = {
    overlay,
    owners: 0,
    imageBytes: image.length,
    tailStart: 0,
    tailBytes: 0,
    tailVeneers: 0,
    tailCallVia: 0,
    tailReturns: 0,
    tailPointerHalves: 0,
    tailDispatched: 0,
    blSites: 0,
    blToBank: 0,
    blToCode: 0,
    tailPointers: 0,
    multiReturnOwners: [],
    findings: [{ offset: 0, region: "OWNER", note: "no recorded owners -- nothing to certify" }],
  };
  if (spans.length === 0) return empty;

  // A recorded span that runs past the end of the image is a span error, not an
  // owner. Say so and clamp, rather than throwing halfway through the sweep and
  // reporting nothing about the other twenty-five owners.
  for (const span of spans)
    if (span.end > image.length || span.start < 0)
      findings.push({
        offset: span.start,
        region: "OWNER",
        note: `recorded span ends at 0x${span.end.toString(16)}, past the ${image.length}-byte image`,
      });
  for (const span of spans) span.end = Math.min(span.end, image.length);

  const tailStart = spans[spans.length - 1].end;
  const tail = ruleTail(image, tailStart);
  const { covered } = maskBanks(image, tailStart, image.length);
  const inBank = (offset: number): boolean =>
    offset >= tailStart && offset < image.length && covered[offset - tailStart] === 1;

  // ---- the `bl` corroboration, computed first because the tail ruling uses it
  let blSites = 0;
  let blToBank = 0;
  let blToCode = 0;
  const dispatched = new Set<number>();
  const strayCalls: { at: number; target: number }[] = [];
  for (const span of spans) {
    for (let at = span.start; at + 4 <= span.end; at += 2) {
      const high = halfword(at);
      const low = halfword(at + 2);
      if ((high & 0xf800) !== 0xf000 || (low & 0xf800) !== 0xf800) continue;
      const target = targetOffset(high, low);
      if (target === null) continue;
      // A target OUTSIDE the image is proof the site is not a call at all:
      // stepping by halfwords walks inline literal pools, and a pool word whose
      // halves read f000/f800 decodes as a `bl` to nowhere. Discard those --
      // they are the instrument's noise floor. resource_370's two "sites" at
      // 0xd34 and 0xda4 resolve to 0x1d36 and 0x1da6 in a 0x171a-byte image,
      // which is how they were caught.
      if (target < 0 || target >= image.length) continue;
      blSites += 1;
      if (inBank(target)) blToBank += 1;
      else if (target < tailStart) blToCode += 1;
      else {
        dispatched.add(target);
        strayCalls.push({ at, target });
      }
    }
  }

  // ---- rule every tail return sweep D reported
  let tailPointerHalves = 0;
  let tailDispatched = 0;
  const ruledDispatch = new Set<number>();
  for (const at of tail.returns) {
    // Class one: the low half of a 4-aligned Thumb pointer onto a recorded
    // owner -- a published pointer word wearing a return shape.
    if (at % 4 === 0) {
      const value = word(at);
      const target = pointerOffset(value, image.length);
      if ((value & 1) === 1 && target !== null && starts.has(target)) {
        tailPointerHalves += 1;
        continue;
      }
    }
    // Class two: a `bx rN` slot the mask missed because its pad is not 0x46c0,
    // proven to be a dispatch bank entry BY EVIDENCE rather than by loosening
    // the pad test -- a real `bl` in a real body targets it. resource_398's
    // `bx lr` at 0x0904 is padded with 0x0000 and carries fifteen call sites.
    if ((halfword(at) & 0xff87) === 0x4700 && dispatched.has(at)) {
      tailDispatched += 1;
      ruledDispatch.add(at);
      continue;
    }
    findings.push({
      offset: at,
      region: "TAIL",
      note: `return shape 0x${halfword(at).toString(16)} outside the banks, not a published-pointer half, not dispatched`,
    });
  }

  // A call into the tail that did NOT land on a ruled dispatch slot is a call
  // into data, which is the claim the 380 certification turns on.
  for (const stray of strayCalls)
    if (!ruledDispatch.has(stray.target))
      findings.push({
        offset: stray.at,
        region: "BL",
        note: `resolves to 0x${stray.target.toString(16)}, inside the tail and outside every bank`,
      });

  // ---- the return census per owner: the swallowed leaf, and nothing else asks
  const multiReturnOwners: number[] = [];
  for (const span of spans) {
    let returns = 0;
    for (let at = span.start; at + 1 < span.end; at += 2) if (isReturnShape(halfword(at))) returns += 1;
    if (returns === 0)
      findings.push({ offset: span.start, region: "OWNER", note: "recorded span contains NO return shape" });
    else if (returns > 1) multiReturnOwners.push(span.start);
  }

  // ---- corroboration two: nothing carries the Thumb bit into tail data
  let tailPointers = 0;
  for (let at = 0; at + 4 <= image.length; at += 4) {
    const value = word(at);
    const target = pointerOffset(value, image.length);
    if (target === null || target < tailStart) continue;
    if (inBank(target)) continue;
    tailPointers += 1;
    if ((value & 1) === 1)
      findings.push({
        offset: at,
        region: "POINTER",
        note: `word 0x${value.toString(16)} carries the Thumb bit into tail data at 0x${target.toString(16)}`,
      });
  }

  return {
    overlay,
    owners: spans.length,
    imageBytes: image.length,
    tailStart,
    tailBytes: tail.bytes,
    tailVeneers: tail.veneers,
    tailCallVia: tail.callVia,
    tailReturns: tail.returns.length,
    tailPointerHalves,
    tailDispatched,
    blSites,
    blToBank,
    blToCode,
    tailPointers,
    multiReturnOwners,
    findings,
  };
}

function selfTest(): void {
  const checks: [string, boolean][] = [];

  checks.push([
    "pointerOffset applies the 0x8000 link bias",
    pointerOffset(0x0200c0b5, 0x10000) === 0x40b4 && pointerOffset(0x0200c0b4, 0x10000) === 0x40b4,
  ]);
  checks.push(["pointerOffset rejects out-of-image", pointerOffset(0x03001ebc, 0x10000) === null]);
  checks.push(["pointerOffset rejects a word below the bias", pointerOffset(0x02000004, 0x10000) === null]);

  // ---- synthetic images ----------------------------------------------------
  // Layout: an owner at 0x00 that pushes and returns, then a tail. The head is
  // deliberately absent -- sweep D owns the head and this tool must not grow a
  // second opinion about it.
  const build = (owner: number[], tail: number[]): Uint8Array => {
    const halfwords = [...owner, ...tail];
    const bytes = new Uint8Array(halfwords.length * 2);
    const out = new DataView(bytes.buffer);
    halfwords.forEach((h, i) => out.setUint16(i * 2, h, true));
    return bytes;
  };
  const ONE_OWNER = [{ start: 0, end: 4 }];

  const base = certifyImage("synthetic", build([0xb500, 0xbd00], [0x0000, 0x0000]), ONE_OWNER);
  checks.push(["a clean synthetic image certifies", base.findings.length === 0]);

  // A LEAF IN THE TAIL -- the case a prologue test cannot see, because it saves
  // no register. It must still return, and sweep E must not rule it away.
  const leaf = certifyImage("synthetic", build([0xb500, 0xbd00], [0x2000, 0x4770]), ONE_OWNER);
  checks.push(["a leaf hidden in the tail survives every ruling", leaf.findings.some((f) => f.region === "TAIL")]);

  // The same halfword inside an import veneer is masked by sweep D's maskBanks,
  // so it never reaches this tool. `ldr r0,[pc,#0] / bx r0 / .word`.
  const veneered = certifyImage("synthetic", build([0xb500, 0xbd00], [0x4800, 0x4700, 0x0100, 0x0300]), ONE_OWNER);
  checks.push(["an import veneer is masked upstream", veneered.tailVeneers === 1 && veneered.findings.length === 0]);

  // REGISTER AGREEMENT, adopted from sweep D: `ldr r0,=table / bx lr` is a real
  // published getter stub, not a veneer, and must NOT be masked.
  const stub = certifyImage("synthetic", build([0xb500, 0xbd00], [0x4800, 0x4770, 0x0100, 0x0300]), ONE_OWNER);
  checks.push(["a getter stub is not mistaken for a veneer", stub.tailVeneers === 0 && stub.findings.length > 0]);

  // Class one: a published pointer word whose low halfword wears a return
  // shape. The 0x8000 link bias makes this concrete rather than convenient:
  // the word 0x0200bd41 designates offset 0x3d40, so the owner has to BE at
  // 0x3d40 for the ruling to apply -- which is the whole point of resolving the
  // pointer instead of eyeballing the halfword.
  const wide = (owner: number, ptrAt: number, ptrWord: number): Uint8Array => {
    const bytes = new Uint8Array(ptrAt + 8);
    const out = new DataView(bytes.buffer);
    out.setUint16(owner, 0xb500, true);
    out.setUint16(owner + 2, 0xbd00, true);
    out.setUint32(ptrAt, ptrWord, true);
    return bytes;
  };
  const OWNER_3D40 = [{ start: 0x3d40, end: 0x3d44 }];
  const pointerHalf = certifyImage("synthetic", wide(0x3d40, 0x3d48, 0x0200bd41), OWNER_3D40);
  checks.push([
    "a published-pointer half is ruled, not accused",
    pointerHalf.tailPointerHalves === 1 && pointerHalf.findings.length === 0,
  ]);
  // ...and the same halfword NOT backed by a pointer to an owner still fires.
  const notAPointer = certifyImage("synthetic", wide(0x3d40, 0x3d48, 0x0300bd41), OWNER_3D40);
  checks.push(["a lookalike that points nowhere still fires", notAPointer.findings.some((f) => f.region === "TAIL")]);

  // Class two: a `bx rN` slot padded with 0x0000 rather than 0x46c0, ruled by
  // EVIDENCE -- a real `bl` in the body targets it -- rather than by loosening
  // the pad test. `f000 f803` at 0x0002 resolves to 0x0008 under the overlay
  // `bl`'s stored-displacement rule; taken from `targetOffset`, not hand-rolled.
  const dispatchImage = build([0xb500, 0xf000, 0xf803, 0xbd00], [0x4770, 0x0000]);
  const dispatch = certifyImage("synthetic", dispatchImage, [{ start: 0, end: 8 }]);
  checks.push([
    "a dispatch slot the pad test missed is ruled by its call sites",
    dispatch.tailDispatched === 1 && dispatch.findings.length === 0,
  ]);

  // A SWALLOWED LEAF: one recorded span holding two returns.
  const swallowed = certifyImage("synthetic", build([0xb500, 0xbd00, 0x2000, 0x4770], [0x0000]), [
    { start: 0, end: 8 },
  ]);
  checks.push(["a span with two returns is reported", swallowed.multiReturnOwners.length === 1]);

  // A span with NO return does not hold a whole function.
  const noReturn = certifyImage("synthetic", build([0xb500, 0x2000], [0x0000, 0x0000]), ONE_OWNER);
  checks.push(["a span with no return at all is reported", noReturn.findings.some((f) => f.region === "OWNER")]);

  // A span running past the image end is a span error, and must not throw.
  const overrun = certifyImage("synthetic", build([0xb500, 0xbd00], [0x0000]), [{ start: 0, end: 999 }]);
  checks.push([
    "a span past the image end is reported, not thrown",
    overrun.findings.some((f) => f.region === "OWNER" && f.note.includes("past the")),
  ]);

  // The Thumb bit into tail data. 0x0200800b -> tail offset 0x000b... use an
  // owner ending at 4 so the tail begins at 4, and point at 0x0004 odd.
  const thumbIntoData = certifyImage("synthetic", build([0xb500, 0xbd00], [0x8005, 0x0200, 0x0000, 0x0000]), ONE_OWNER);
  checks.push([
    "a Thumb-bit word into tail data is reported",
    thumbIntoData.findings.some((f) => f.region === "POINTER"),
  ]);

  let failed = 0;
  for (const [name, ok] of checks) {
    if (!ok) {
      failed += 1;
      console.log(`FAIL ${name}`);
    }
  }
  if (failed > 0) {
    console.log(`sweep E self-test: ${failed} of ${checks.length} failed`);
    process.exit(1);
  }
  console.log(`sweep E self-test passed (${checks.length} checks, tail ruling, owner returns, bl, pointers)`);
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  // A NAME IS REJECTED BY EXISTENCE, NOT BY SHAPE.
  const known = new Set(overlayNames());
  const requested = args.filter((argument) => !argument.startsWith("--"));
  for (const name of requested)
    if (!known.has(name)) {
      console.log(`NOTHING SWEPT — no overlay named ${name}. This is a FAILURE, not a pass.`);
      process.exit(1);
    }
  const overlays = requested.length > 0 ? requested : overlayNames();
  if (overlays.length === 0) {
    console.log("NOTHING SWEPT — this is a FAILURE, not a pass.");
    process.exit(1);
  }
  const results = overlays.map(certify);
  if (args.includes("--json")) {
    console.log(JSON.stringify(results, null, 2));
    return;
  }
  let findings = 0;
  for (const result of results) {
    findings += result.findings.length;
    if (result.findings.length === 0 && result.multiReturnOwners.length === 0 && overlays.length > 1) continue;
    console.log(
      `${result.overlay}  owners=${result.owners} tail=${result.tailBytes}B from 0x${result.tailStart.toString(16)} ` +
        `(${result.tailVeneers} veneers, ${result.tailCallVia} call_via) ` +
        `returns=${result.tailReturns} ruled=${result.tailPointerHalves} pointer-halves + ${result.tailDispatched} dispatched ` +
        `bl=${result.blSites} (${result.blToBank} bank, ${result.blToCode} code)`,
    );
    for (const owner of result.multiReturnOwners)
      console.log(
        `  OWNER  0x${(OVERLAY_BASE + owner).toString(16)}  more than one return shape — read for a swallowed leaf`,
      );
    for (const finding of result.findings)
      console.log(`  ${finding.region.padEnd(7)}0x${(OVERLAY_BASE + finding.offset).toString(16)}  ${finding.note}`);
  }
  console.log(`\noverlays=${results.length} sweep_e_findings=${findings}`);
  if (findings > 0) process.exit(1);
}

if (import.meta.main) main();
