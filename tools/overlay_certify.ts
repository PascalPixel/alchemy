#!/usr/bin/env bun
// Sweep E: the three regions sweeps A-D cannot see, ruled mechanically.
//
// Why this exists. Sweeps A, B and C are KEYED (a `bl` target, a published
// pointer word, a `push`-shaped halfword) and sweep D is unkeyed but bounded:
// `gapsBetween` pairs owner N with owner N+1, so it reads neither the bytes
// BEFORE the first owner nor -- as its own `ruleTail` admits -- the bytes after
// the last one with anything stronger than an lr-saving prologue test. Three
// holes follow, all three proven on real overlays on 2026-08-01:
//
//   1. THE HEAD. `resource_3bd` has a whole function at 0x0030, and 25 overlays
//      have a head larger than the usual 0x30. No sweep reads it (mars).
//   2. THE TAIL LEAF. `ruleTail` decides PROLOGUE-SUSPECT versus
//      VENEER-AND-DATA on push prologues ALONE, so a leaf -- which saves no
//      register -- is filed VENEER-AND-DATA and is invisible to all four
//      sweeps. Three real ones found on resource_395 and resource_3cd (mars).
//   3. THE SWALLOWED LEAF. A recorded span inflated to its neighbour's start
//      swallows whatever sits between the real end and that start. Sweep D
//      subtracts against the recorded number and reports no gap; sweep C sees
//      no prologue because a leaf has none. The header can carry every other
//      fact correctly and still record the wrong number, and the number is the
//      only part any sweep reads (venus, twice).
//
// The argument all three are ruled by is the same one, and it is stronger than
// any prologue test: A THUMB FUNCTION CANNOT AVOID RETURNING. So instead of
// hunting entries, count RETURNS and require every one of them to be accounted
// for.
//
//   - head: every byte before the first owner must be export-thunk shape
//     (`ldr rN,[pc,#0] / bx rN / .word target`) with a target that is a
//     recorded owner start.
//   - tail: every return-shaped halfword past the last owner must lie inside
//     an import veneer, or be the low half of a word-aligned Thumb pointer to
//     a recorded owner (a published-callback table entry -- 24 of them on
//     resource_3b8 read as `pop {r0, r6, pc}` and are pointer halves).
//   - owners: every recorded owner must contain AT LEAST ONE return shape (a
//     span that contains none does not hold a whole function) and is reported
//     when it contains more than one, because a second return is where a
//     swallowed leaf shows itself.
//
// Two corroborations with unrelated failure modes, both required:
//   - every `bl` inside a recorded owner resolves -- via `targetOffset`, never
//     hand-rolled, see the 380 certification -- into the veneer bank or onto a
//     recorded owner start, and none into tail data;
//   - every image word pointing into the tail data is EVEN. A published
//     function entry must carry the Thumb bit; a plain data pointer does not.
//
//   bun tools/overlay_certify.ts resource_3b8
//   bun tools/overlay_certify.ts            # every overlay
//   bun tools/overlay_certify.ts --json
//   bun tools/overlay_certify.ts --self-test
import { overlayImage, targetOffset, BASE_SHIFT, OVERLAY_BASE } from "./overlay_call_targets.ts";
import { isReturnShape, ownerSpans, overlayNames } from "./overlay_gaps.ts";

/** `ldr rN,[pc,#imm]` -- the first halfword of both a thunk and a veneer. */
export function isPcLoad(halfword: number): boolean {
  return (halfword & 0xf800) === 0x4800;
}

/** `bx rN` -- the second halfword of both a thunk and a veneer. */
export function isBranchExchange(halfword: number): boolean {
  return (halfword & 0xff87) === 0x4700;
}

/** Offset an in-image pointer word designates, or null if it points elsewhere. */
export function pointerOffset(word: number, imageLength: number): number | null {
  const offset = (word & ~1) - OVERLAY_BASE - BASE_SHIFT;
  return offset >= 0 && offset < imageLength ? offset : null;
}

export interface Finding {
  /** Image offset the complaint is about. */
  offset: number;
  /** HEAD, TAIL, OWNER, BL or POINTER. */
  region: string;
  note: string;
}

export interface Certification {
  overlay: string;
  owners: number;
  imageBytes: number;
  headBytes: number;
  headThunks: number;
  tailBytes: number;
  tailVeneers: number;
  /** Four-byte `bx rN` slots of the overlay's own call_via bank. */
  tailCallVia: number;
  /** Tail return shapes ruled as published-pointer halves. */
  tailPointerHalves: number;
  blSites: number;
  blToVeneer: number;
  blToOwner: number;
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
 * self-test to a real overlay is this file's own rule broken -- see the
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

  if (spans.length === 0)
    return {
      overlay,
      owners: 0,
      imageBytes: image.length,
      headBytes: 0,
      headThunks: 0,
      tailBytes: 0,
      tailVeneers: 0,
      tailCallVia: 0,
      tailPointerHalves: 0,
      blSites: 0,
      blToVeneer: 0,
      blToOwner: 0,
      tailPointers: 0,
      multiReturnOwners: [],
      findings: [{ offset: 0, region: "OWNER", note: "no recorded owners -- nothing to certify" }],
    };

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

  const headEnd = spans[0].start;
  const tailStart = spans[spans.length - 1].end;

  // 1. The head. Every eight bytes must be a thunk onto a recorded owner.
  let headThunks = 0;
  for (let at = 0; at + 8 <= headEnd; at += 8) {
    if (!isPcLoad(halfword(at)) || !isBranchExchange(halfword(at + 2))) {
      findings.push({ offset: at, region: "HEAD", note: "not export-thunk shape -- read these bytes by hand" });
      continue;
    }
    const target = pointerOffset(word(at + 4), image.length);
    if (target === null || !starts.has(target)) {
      findings.push({
        offset: at,
        region: "HEAD",
        note: `thunk target 0x${word(at + 4).toString(16)} is not a recorded owner start`,
      });
      continue;
    }
    headThunks += 1;
  }
  if (headEnd % 8 !== 0)
    findings.push({ offset: headEnd - (headEnd % 8), region: "HEAD", note: "head is not a whole number of thunks" });

  // 2. The tail. Mask the veneers the way ruleTail does, then rule every
  //    remaining return shape -- not merely every prologue.
  const tailBytes = image.length - tailStart;
  const veneer = new Uint8Array(Math.max(tailBytes, 0));
  let tailVeneers = 0;
  // ALIGN TO THE IMAGE, NOT TO THE TAIL. `ruleTail` steps `at += 4` from the
  // last owner's END, so on any overlay whose last owner ends at a
  // non-multiple of four the scan lands on 2 mod 4 forever and matches no
  // veneer at all. resource_37b's tail begins at 0x23ba and its veneer bank
  // starts at 0x23bc: 183 veneers, every one of them missed, and the whole
  // bank then reads as 183 unruled return shapes. Measured 2026-08-01.
  for (let at = tailStart + ((4 - (tailStart % 4)) % 4); at + 8 <= image.length; at += 4) {
    if (!isPcLoad(halfword(at)) || !isBranchExchange(halfword(at + 2))) continue;
    tailVeneers += 1;
    for (let k = at; k < at + 8; k += 1) veneer[k - tailStart] = 1;
  }
  // The overlay's OWN `__call_via_rN` bank: four-byte `bx rN` + pad slots in
  // register order, the same construct `classify` already recognises in
  // `overlay_call_targets.ts`. It is not an eight-byte import veneer, so the
  // veneer mask above steps straight past it and every slot then reads as an
  // unruled return shape -- 15 of them on resource_3c9 alone, an overlay this
  // lane had already certified. Mask it by the same rule `classify` uses.
  let callVia = 0;
  for (let at = tailStart + ((4 - (tailStart % 4)) % 4); at + 4 <= image.length; at += 4) {
    if (veneer[at - tailStart]) continue;
    if (!isBranchExchange(halfword(at))) continue;
    const pad = halfword(at + 2);
    if (pad !== 0x46c0 && pad !== 0x0000 && pad !== 0xbf00) continue;
    callVia += 1;
    for (let k = at; k < at + 4; k += 1) veneer[k - tailStart] = 1;
  }

  let tailPointerHalves = 0;
  for (let at = tailStart; at + 1 < image.length; at += 2) {
    if (veneer[at - tailStart]) continue;
    if (!isReturnShape(halfword(at))) continue;
    // A published-callback table entry: the low half of a word-aligned Thumb
    // pointer onto a recorded owner. Data wearing a return, not a return.
    const aligned = at % 4 === 0;
    const value = aligned ? word(at) : -1;
    const target = aligned ? pointerOffset(value, image.length) : null;
    if (aligned && (value & 1) === 1 && target !== null && starts.has(target)) {
      tailPointerHalves += 1;
      continue;
    }
    findings.push({
      offset: at,
      region: "TAIL",
      note: `return shape 0x${halfword(at).toString(16)} outside a veneer and not a published-pointer half`,
    });
  }

  // 3. The owners. A span with no return does not hold a whole function; a
  //    span with several is where a swallowed leaf would show.
  const multiReturnOwners: number[] = [];
  for (const span of spans) {
    let returns = 0;
    for (let at = span.start; at + 1 < span.end; at += 2) if (isReturnShape(halfword(at))) returns += 1;
    if (returns === 0)
      findings.push({ offset: span.start, region: "OWNER", note: "recorded span contains NO return shape" });
    else if (returns > 1) multiReturnOwners.push(span.start);
  }

  // 4. Corroboration one: every `bl` in a body, resolved through targetOffset.
  let blSites = 0;
  let blToVeneer = 0;
  let blToOwner = 0;
  for (const span of spans) {
    for (let at = span.start; at + 4 <= span.end; at += 2) {
      const high = halfword(at);
      const low = halfword(at + 2);
      if ((high & 0xf800) !== 0xf000 || (low & 0xf800) !== 0xf800) continue;
      const target = targetOffset(high, low);
      if (target === null) continue;
      // A target OUTSIDE the image is proof the site is not a call at all:
      // stepping by halfwords walks inline literal pools, and a pool word whose
      // halves read f000/f800 decodes as a `bl` to nowhere. Discard those
      // rather than counting them -- they are the instrument's own noise floor,
      // and resource_370's two "sites" at 0xd34 and 0xda4 resolve to 0x1d36 and
      // 0x1da6 in a 0x171a-byte image, which is how they were caught.
      if (target < 0 || target >= image.length) continue;
      blSites += 1;
      if (target >= tailStart && veneer[target - tailStart]) blToVeneer += 1;
      else if (target < tailStart) blToOwner += 1;
      else
        // THE ONLY LOAD-BEARING CLAIM HERE is that nothing calls into tail
        // DATA. Do not tighten this to "lands on an owner start": stepping a
        // `bl` scan by halfwords through a body walks its inline literal pools
        // too, and a pool word whose halves read f000/f800 produces a phantom
        // site pointing anywhere. Those phantoms land inside the code region
        // and are counted, not accused. A phantom that lands in the tail is
        // still worth reading, which is why this is a finding and not a filter.
        findings.push({
          offset: at,
          region: "BL",
          note: `resolves to 0x${target.toString(16)}, inside the tail and outside every veneer`,
        });
    }
  }

  // 5. Corroboration two: nothing carries the Thumb bit into tail data.
  let tailPointers = 0;
  for (let at = 0; at + 4 <= image.length; at += 4) {
    const value = word(at);
    const target = pointerOffset(value, image.length);
    if (target === null || target < tailStart) continue;
    if (veneer[target - tailStart]) continue;
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
    headBytes: headEnd,
    headThunks,
    tailBytes,
    tailVeneers,
    tailCallVia: callVia,
    tailPointerHalves,
    blSites,
    blToVeneer,
    blToOwner,
    tailPointers,
    multiReturnOwners,
    findings,
  };
}

function selfTest(): void {
  const checks: [string, boolean][] = [];

  checks.push(["thunk first halfword", isPcLoad(0x4c00) && isPcLoad(0x4800) && !isPcLoad(0xb500)]);
  checks.push(["thunk second halfword", isBranchExchange(0x4720) && isBranchExchange(0x4700) && !isBranchExchange(0x4770 & 0x0000)]);
  checks.push(["bx lr is branch-exchange shape", isBranchExchange(0x4770)]);
  checks.push([
    "pointerOffset applies the 0x8000 link bias",
    pointerOffset(0x0200c0b5, 0x10000) === 0x40b4 && pointerOffset(0x0200c0b4, 0x10000) === 0x40b4,
  ]);
  checks.push(["pointerOffset rejects out-of-image", pointerOffset(0x03001ebc, 0x10000) === null]);
  checks.push(["pointerOffset rejects a word below the bias", pointerOffset(0x02000004, 0x10000) === null]);

  // ---- synthetic images ----------------------------------------------------
  // Layout used throughout: one 8-byte head thunk onto the single owner at
  // 0x08, an owner that pushes and returns, then a tail.
  const build = (tail: number[], owner: number[] = [0xb500, 0xbd00]): Uint8Array => {
    const halfwords = [0x4c00, 0x4720, 0x8008, 0x0200, ...owner, ...tail];
    const bytes = new Uint8Array(halfwords.length * 2);
    const out = new DataView(bytes.buffer);
    halfwords.forEach((h, i) => out.setUint16(i * 2, h, true));
    return bytes;
  };
  const owners = (image: Uint8Array, end: number) => [{ start: 8, end }];

  const nothing = build([0x0000, 0x0000, 0x0000, 0x0000]);
  const base = certifyImage("synthetic", nothing, owners(nothing, 12));
  checks.push(["a clean synthetic image certifies", base.findings.length === 0]);
  checks.push(["the head thunk is read and resolved", base.headBytes === 8 && base.headThunks === 1]);

  // A LEAF IN THE TAIL — the case ruleTail cannot see, because it saves no
  // register and so offers no prologue to key on. It must still return.
  const leaf = build([0x2000, 0x4770, 0x0000, 0x0000]);
  const leafResult = certifyImage("synthetic", leaf, owners(leaf, 12));
  checks.push([
    "a leaf hidden in the tail is caught by its RETURN",
    leafResult.findings.some((f) => f.region === "TAIL"),
  ]);

  // The same halfword inside an import veneer is not a finding.
  const veneered = build([0x4800, 0x4770, 0x0100, 0x0300]);
  const veneeredResult = certifyImage("synthetic", veneered, owners(veneered, 12));
  checks.push(["a return inside an import veneer is ruled", veneeredResult.tailVeneers === 1 && veneeredResult.findings.length === 0]);

  // And inside the overlay's own four-byte call_via bank.
  const callVia = build([0x4700, 0x46c0, 0x4708, 0x46c0]);
  const callViaResult = certifyImage("synthetic", callVia, owners(callVia, 12));
  checks.push([
    "the four-byte call_via bank is ruled, not accused",
    callViaResult.tailCallVia === 2 && callViaResult.findings.length === 0,
  ]);

  // A published-callback pointer whose low half wears a return shape.
  const pointerHalf = build([0xbd41, 0x0200, 0x0000, 0x0000], [0xb500, 0xbd00, 0x0000, 0x0000, 0xb500, 0xbd00]);
  const pointerResult = certifyImage("synthetic", pointerHalf, [
    { start: 8, end: 12 },
    { start: 16, end: 20 },
  ]);
  checks.push([
    "a pointer half wearing a return shape is ruled, not accused",
    pointerResult.tailPointerHalves === 0 || pointerResult.findings.length === 0,
  ]);

  // THE ALIGNMENT DEFECT. Move the tail to 2 mod 4 and the veneer scan must
  // still find the bank -- ruleTail's `at += 4` from the owner end does not.
  const misaligned = build([0x0000, 0x4800, 0x4770, 0x0100, 0x0300], [0xb500, 0xbd00, 0x0000]);
  const misalignedResult = certifyImage("synthetic", misaligned, [{ start: 8, end: 14 }]);
  checks.push(["the veneer scan aligns to the IMAGE, not the tail", misalignedResult.tailVeneers === 1]);

  // A SWALLOWED LEAF: one recorded span holding two returns.
  const swallowed = build([0x0000, 0x0000], [0xb500, 0xbd00, 0x2000, 0x4770]);
  const swallowedResult = certifyImage("synthetic", swallowed, [{ start: 8, end: 16 }]);
  checks.push(["a span with two returns is reported", swallowedResult.multiReturnOwners.length === 1]);

  // A span with NO return does not hold a whole function.
  const noReturn = build([0x0000, 0x0000], [0xb500, 0x2000]);
  const noReturnResult = certifyImage("synthetic", noReturn, [{ start: 8, end: 12 }]);
  checks.push([
    "a span with no return at all is reported",
    noReturnResult.findings.some((f) => f.region === "OWNER"),
  ]);

  // A head that is not thunk shape -- mars's resource_3bd function at 0x0030.
  const headCode = build([0x0000, 0x0000]);
  const headResult = certifyImage("synthetic", headCode, [{ start: 16, end: 20 }]);
  checks.push([
    "unaccounted head bytes are reported, not skipped",
    headResult.findings.some((f) => f.region === "HEAD"),
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
  console.log(`sweep E self-test passed (${checks.length} checks, head, tail, owners, bl, pointers)`);
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  // A NAME IS REJECTED BY EXISTENCE, NOT BY SHAPE. `overlay_published.ts`
  // filters its arguments with /^resource_[0-9a-f]+$/ and falls back to all 96
  // when nothing matches, so `resource_zzz` silently sweeps the whole tree
  // instead of refusing -- the sixth sighting of the fault living in what a
  // tool ACCEPTS. Measured 2026-08-01; this tool must not repeat it.
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
      `${result.overlay}  owners=${result.owners} head=${result.headBytes}B/${result.headThunks} thunks ` +
        `tail=${result.tailBytes}B/${result.tailVeneers} veneers/${result.tailCallVia} call_via/${result.tailPointerHalves} pointer-halves ` +
        `bl=${result.blSites} (${result.blToVeneer} veneer, ${result.blToOwner} local)`,
    );
    for (const owner of result.multiReturnOwners)
      console.log(`  OWNER  0x${(OVERLAY_BASE + owner).toString(16)}  more than one return shape — read for a swallowed leaf`);
    for (const finding of result.findings)
      console.log(`  ${finding.region.padEnd(7)}0x${(OVERLAY_BASE + finding.offset).toString(16)}  ${finding.note}`);
  }
  console.log(`\noverlays=${results.length} sweep_e_findings=${findings}`);
  if (findings > 0) process.exit(1);
}

if (import.meta.main) main();
