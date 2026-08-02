#!/usr/bin/env bun
/**
 * Compare semantic overlay sources with the resolved order of their Thumb BLs.
 *
 * This is deliberately stronger than overlay_multiset_check.ts.  A multiset
 * proves that the same callees occur, but it cannot see two calls that were
 * swapped.  The source walk below uses postorder evaluation: calls nested in
 * an argument are emitted before the call that consumes their result, which is
 * the dependency order the compiler must preserve. Sibling argument calls are
 * kept in source order as a conservative review signal: C does not specify
 * their relative order, so a mismatch means the source should be inspected
 * and, where effects matter, split into explicitly sequenced statements.
 *
 * Examples:
 *
 *   bun tools/overlay_call_order_check.ts --self-test
 *   bun tools/overlay_call_order_check.ts resource_39d 0b24 1af0
 *   bun tools/overlay_call_order_check.ts resource_3bd
 *   bun tools/overlay_call_order_check.ts --all
 *
 * Bounds are overlay-relative hexadecimal offsets, matching
 * overlay_call_targets.ts.  If an end is omitted, the admitted semantic row's
 * measured span is used.  With no overlay, `--all` is required so an accidental
 * typo cannot silently turn a whole-project audit into an empty run.
 */

import { existsSync, readFileSync, readdirSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { Discovery } from "./discover.ts";
import { OVERLAY_BASE, classify, overlayImage, resolveOverlay, targetOffset } from "./overlay_call_targets.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const SOURCE_DIRECTORY = join(ROOT, "semantic", "overlays");
const REGIONS_PATH = join(ROOT, "semantic", "regions.json");
const INVENTORY_PATH = join(ROOT, "out", "decomp", "overlays.json");
const CALL_VIA = "(call_via)";
const USAGE = "usage: overlay_call_order_check.ts <resource_NNN> [startHex [endHex]] [--json] | --all [--json] | --self-test";

export interface SemanticOwner {
  overlay: string;
  offset: number;
  spanBytes: number;
  source: string;
  symbol: string;
}

export interface MachineCall {
  site: number;
  target: number;
  kind: "veneer" | "prologue" | "call_via" | "leaf" | "unknown";
  name: string;
  /** A call-through target loaded from a nearby literal, when provable. */
  resolvedCallViaTarget?: number;
}

export interface OrderMismatch {
  index: number;
  source?: string;
  machine?: string;
  site?: number;
  detail?: string;
}

export interface OrderResult {
  owner: SemanticOwner;
  sourceCalls: string[];
  machineCalls: MachineCall[];
  mismatches: OrderMismatch[];
  unresolvedCallVia: number;
}

interface ManualRegion {
  overlay: string;
  entry: string;
  span_bytes: number;
}

interface InventoryRow {
  overlay: string;
  entry: number;
  offset: number;
  span_bytes: number;
  starts_with_prologue?: boolean;
}

interface InventoryFile {
  functions: InventoryRow[];
}

function hex(value: number, width = 8): string {
  return (value >>> 0).toString(16).padStart(width, "0");
}

function readManualRegions(): Map<string, number> {
  if (!existsSync(REGIONS_PATH)) throw new Error(`missing ${REGIONS_PATH}`);
  const parsed = JSON.parse(readFileSync(REGIONS_PATH, "utf8")) as { manual_regions?: ManualRegion[] };
  const result = new Map<string, number>();
  for (const row of parsed.manual_regions ?? []) {
    const offset = Number.parseInt(row.entry, 16) - OVERLAY_BASE;
    if (!Number.isInteger(offset) || offset < 0 || !Number.isInteger(row.span_bytes) || row.span_bytes <= 0) {
      throw new Error(`invalid semantic owner boundary: ${JSON.stringify(row)}`);
    }
    const key = `${row.overlay}:${offset.toString(16)}`;
    if (result.has(key)) throw new Error(`duplicate semantic owner boundary: ${key}`);
    result.set(key, row.span_bytes);
  }
  return result;
}

function readInventory(): InventoryFile {
  if (!existsSync(INVENTORY_PATH)) throw new Error(`missing ${INVENTORY_PATH}; run the overlay inventory first`);
  return JSON.parse(readFileSync(INVENTORY_PATH, "utf8")) as InventoryFile;
}

interface OwnerSelection {
  overlay?: string;
  start?: number;
}

function sourceOwnerFiles(selection: OwnerSelection = {}): string[] {
  return readdirSync(SOURCE_DIRECTORY)
    .filter((name) => /^resource_[0-9a-f]+_c_[0-9a-f]{8}\.c$/i.test(name))
    // Apply a requested scope before resolving boundaries.  A project-wide
    // source directory can contain a draft owner without an admitted row;
    // that must not prevent a targeted audit of an already-admitted owner in
    // another overlay from running.
    .filter((name) => {
      const match = /^(resource_[0-9a-f]+)_c_([0-9a-f]{8})\.c$/i.exec(name);
      if (match === null) return false;
      if (selection.overlay !== undefined && match[1].toLowerCase() !== selection.overlay) return false;
      if (selection.start === undefined) return true;
      return Number.parseInt(match[2], 16) - OVERLAY_BASE === selection.start;
    })
    .sort()
    .map((name) => join(SOURCE_DIRECTORY, name));
}

function ownerFromSource(path: string, manual: Map<string, number>, inventory: InventoryFile): SemanticOwner {
  const name = basename(path);
  const match = /^(resource_[0-9a-f]+)_c_([0-9a-f]{8})\.c$/i.exec(name);
  if (match === null) throw new Error(`not a semantic overlay owner source: ${path}`);
  const overlay = match[1].toLowerCase();
  const absolute = Number.parseInt(match[2], 16);
  const offset = absolute - OVERLAY_BASE;
  if (offset < 0) throw new Error(`owner address is below overlay base: ${path}`);
  const key = `${overlay}:${offset.toString(16)}`;
  const manualSpan = manual.get(key);
  const inventoryRow = inventory.functions.find((row) => row.overlay === overlay && row.offset === offset);
  const spanBytes = manualSpan ?? inventoryRow?.span_bytes;
  if (spanBytes === undefined) {
    throw new Error(`no admitted boundary for ${overlay}:0x${offset.toString(16)} (${path})`);
  }
  const source = readFileSync(path, "utf8");
  const symbol = `Func_${match[2].toLowerCase()}`;
  if (!new RegExp(`\\b${symbol}\\s*\\(`).test(source)) {
    throw new Error(`${path} does not contain its owner symbol ${symbol}`);
  }
  return { overlay, offset, spanBytes, source: path, symbol };
}

function loadOwners(selection: OwnerSelection = {}): SemanticOwner[] {
  const manual = readManualRegions();
  const inventory = readInventory();
  return sourceOwnerFiles(selection).map((path) => ownerFromSource(path, manual, inventory));
}

function stripCommentsAndStrings(source: string): string {
  return source
    .replace(/\/\*[\s\S]*?\*\//g, " ")
    .replace(/\/\/[^\n]*/g, " ")
    .replace(/"(?:\\.|[^"\\])*"/g, "\"\"")
    .replace(/'(?:\\.|[^'\\])*'/g, "''");
}

function matchingDelimiter(text: string, open: number, left: string, right: string): number {
  let depth = 0;
  for (let index = open; index < text.length; index++) {
    if (text[index] === left) depth++;
    else if (text[index] === right && --depth === 0) return index;
  }
  throw new Error(`unbalanced ${left}${right} pair near byte ${open}`);
}

function ownerBody(source: string, symbol: string): string {
  const clean = stripCommentsAndStrings(source);
  const pattern = new RegExp(`\\b${symbol}\\s*\\(`, "gi");
  while (true) {
    const found = pattern.exec(clean);
    if (found === null) break;
    const open = clean.indexOf("(", found.index);
    const close = matchingDelimiter(clean, open, "(", ")");
    let after = close + 1;
    while (/\s/.test(clean[after] ?? "")) after++;
    if (clean[after] !== "{") continue; // prototype/declaration
    const bodyEnd = matchingDelimiter(clean, after, "{", "}");
    return clean.slice(after + 1, bodyEnd);
  }
  throw new Error(`${symbol} has no function definition in its source`);
}

// The recursive helper needs to parse arbitrary argument text without
// looking for a function definition. Keeping this separate avoids treating a
// nested call as a declaration while retaining the same postorder rule.
function callsInBody(body: string): string[] {
  const calls: string[] = [];
  let cursor = 0;
  // `callback()` is the source-level form of a BL into an overlay's bare
  // `bx rN` trampoline. Keep it in the sequence as the same explicit marker
  // used by machineCallFromTarget rather than silently shifting every later
  // direct call. Semantic owners use `callback` for this deliberately narrow
  // convention; arbitrary non-Func helper calls remain outside the ABI audit.
  const pattern = /\b(Func_[0-9a-f]{8}|callback)\s*\(/gi;
  while (true) {
    pattern.lastIndex = cursor;
    const found = pattern.exec(body);
    if (found === null) break;
    const open = body.indexOf("(", found.index);
    const close = matchingDelimiter(body, open, "(", ")");
    calls.push(...callsInBody(body.slice(open + 1, close)));
    calls.push(found[1].toLowerCase() === "callback"
      ? CALL_VIA
      : `Func_${found[1].slice(5).toLowerCase()}`);
    cursor = close + 1;
  }
  return calls;
}

// Keep the public function small and make its recursion impossible to confuse
// with owner-definition discovery.
export function sourceCalls(source: string, symbol: string): string[] {
  return callsInBody(ownerBody(source, symbol));
}

function readU16(image: Uint8Array, offset: number): number {
  return image[offset] | (image[offset + 1] << 8);
}

function readU32(image: Uint8Array, offset: number): number {
  return (
    image[offset] |
    (image[offset + 1] << 8) |
    (image[offset + 2] << 16) |
    (image[offset + 3] << 24)
  ) >>> 0;
}

/**
 * Recover a call-through target only for the strong, local pattern used by the
 * historical overlay output: a literal LDR into the slot's BX register in the
 * preceding 40 bytes.  A global pointer or a computed target stays unresolved;
 * guessing in that case would turn an audit into a source-specific oracle.
 */
export function callViaLiteralTarget(image: Uint8Array, slotOffset: number, siteOffset: number): number | undefined {
  if (slotOffset < 0 || slotOffset + 2 > image.length) return undefined;
  const slot = readU16(image, slotOffset);
  if ((slot & 0xff87) !== 0x4700) return undefined;
  const register = (slot >>> 3) & 15;
  for (let at = siteOffset - 2; at >= Math.max(0, siteOffset - 40); at -= 2) {
    const half = readU16(image, at);
    if ((half & 0xf800) !== 0x4800) continue;
    const loadedRegister = (half >>> 8) & 7;
    if (loadedRegister !== register) continue;
    const literal = (((OVERLAY_BASE + at + 4) & ~3) + ((half & 0xff) << 2)) - OVERLAY_BASE;
    if (literal < 0 || literal + 4 > image.length) return undefined;
    const value = readU32(image, literal);
    // IWRAM call-through functions are the only established external targets
    // in this family. A Thumb bit, if present, is metadata rather than part of
    // the name written in the C source.
    if ((value & 0xff000000) !== 0x03000000) return undefined;
    return value & ~1;
  }
  return undefined;
}

function inventoryPrologues(inventory: InventoryFile, overlay: string): Set<number> {
  return new Set(
    inventory.functions
      .filter((row) => row.overlay === overlay && row.starts_with_prologue)
      .map((row) => row.offset),
  );
}

function machineCallFromTarget(
  image: Uint8Array,
  owner: SemanticOwner,
  prologues: Set<number>,
  siteOffset: number,
  target: number,
): MachineCall | null {
  // BLs into the owner's own span are compiler-generated internal transfers,
  // not calls represented by a separate source function.
  if (target > owner.offset && target < owner.offset + owner.spanBytes) return null;
  const detail = classify(image, target, prologues);
  if (detail.kind === "veneer" && detail.imported !== undefined) {
    return { site: siteOffset, target, kind: detail.kind, name: `Func_${hex(detail.imported)}` };
  }
  if (detail.kind === "call_via") {
    // A bare `bx lr` is a genuine local leaf, not an indirect-call slot. The
    // shared resolver intentionally reports this shape as call_via; mirror
    // overlay_multiset_check's explicit exception so established no-op leaf
    // owners remain comparable by their local address.
    if (readU16(image, target) === 0x4770) {
      return {
        site: siteOffset,
        target,
        kind: "leaf",
        name: `Func_${hex(OVERLAY_BASE + target)}`,
      };
    }
    const resolved = callViaLiteralTarget(image, target, siteOffset);
    return {
      site: siteOffset,
      target,
      kind: detail.kind,
      name: CALL_VIA,
      ...(resolved === undefined ? {} : { resolvedCallViaTarget: resolved }),
    };
  }
  return {
    site: siteOffset,
    target,
    kind: detail.kind,
    name: detail.kind === "unknown" ? `UNKNOWN_${hex(target)}` : `Func_${hex(OVERLAY_BASE + target)}`,
  };
}

function machineCalls(owner: SemanticOwner, image: Uint8Array, prologues: Set<number>): MachineCall[] {
  const start = OVERLAY_BASE + owner.offset;
  const end = Math.min(start + owner.spanBytes, OVERLAY_BASE + image.length);
  const discovery = new Discovery(Buffer.from(image), OVERLAY_BASE);
  discovery.add_seed(start, "thumb", "ordered-call-audit");
  discovery.walk_function(start);
  const functionInfo = discovery.functions.get(start);
  const reachable = functionInfo?.instructions ?? new Set<number>();
  if (reachable.size === 0) throw new Error(`${owner.overlay}:0x${owner.offset.toString(16)} has no reachable instructions`);

  // Discovery deliberately stops at an unresolved `mov pc, rN`, because it
  // cannot prove the table arms without overlay-specific link semantics. The
  // resolver has the complementary evidence: the overlay's stored BL rule
  // (`target - 2`) and the complete explicit owner bound. When the walk records
  // a jump-table dispatch, use that bounded resolver listing so the order audit
  // covers the arms after the table instead of returning only the pre-dispatch
  // prefix. Unknown flat-disassembly hits remain visible as UNKNOWN_*; a pool
  // masquerading as BL is therefore a review signal, not silently discarded.
  const hasJumpTable = [...(functionInfo?.unresolved ?? [])].some((address) => {
    const offset = address - OVERLAY_BASE;
    return offset >= owner.offset && offset + 2 <= owner.offset + owner.spanBytes &&
      (readU16(image, offset) & 0xff87) === 0x4687;
  });
  if (hasJumpTable) {
    const calls: MachineCall[] = [];
    for (const site of resolveOverlay(owner.overlay, owner.offset, owner.offset + owner.spanBytes)) {
      const call = machineCallFromTarget(image, owner, prologues, site.site, site.target);
      if (call !== null) calls.push(call);
    }
    return calls;
  }

  const calls: MachineCall[] = [];
  for (let address = start; address + 3 < end; address += 2) {
    if (!reachable.has(address)) continue;
    const siteOffset = address - OVERLAY_BASE;
    const target = targetOffset(readU16(image, siteOffset), readU16(image, siteOffset + 2));
    if (target === null || target < 0 || target >= image.length) continue;
    const call = machineCallFromTarget(image, owner, prologues, siteOffset, target);
    if (call !== null) calls.push(call);
  }
  return calls;
}

function isIramName(name: string): boolean {
  return /^Func_03[0-9a-f]{6}$/i.test(name);
}

export function compareOrder(owner: SemanticOwner, sourceSequence: string[], machineSequence: MachineCall[]): OrderResult {
  const mismatches: OrderMismatch[] = [];
  let unresolvedCallVia = 0;
  const count = Math.max(sourceSequence.length, machineSequence.length);
  for (let index = 0; index < count; index++) {
    const source = sourceSequence[index];
    const machine = machineSequence[index];
    if (machine === undefined) {
      mismatches.push({ index, source, detail: "source call has no resolved BL site" });
      continue;
    }
    if (source === undefined) {
      mismatches.push({ index, machine: machine.name, site: machine.site, detail: "resolved BL site has no source call" });
      continue;
    }
    if (machine.kind === "call_via") {
      if (source === CALL_VIA) {
        if (machine.resolvedCallViaTarget === undefined) unresolvedCallVia++;
      } else if (machine.resolvedCallViaTarget !== undefined) {
        const actual = `Func_${hex(machine.resolvedCallViaTarget)}`;
        if (actual !== source) {
          mismatches.push({ index, source, machine: actual, site: machine.site, detail: "resolved call_via target differs" });
        }
      } else if (isIramName(source)) {
        unresolvedCallVia++;
      } else {
        mismatches.push({ index, source, machine: CALL_VIA, site: machine.site, detail: "unresolved call_via is not an IWRAM source call" });
      }
      continue;
    }
    if (machine.name !== source) {
      mismatches.push({ index, source, machine: machine.name, site: machine.site, detail: `machine kind=${machine.kind}` });
    }
  }
  return { owner, sourceCalls: sourceSequence, machineCalls: machineSequence, mismatches, unresolvedCallVia };
}

function formatOwner(owner: SemanticOwner): string {
  return `${owner.overlay}:0x${owner.offset.toString(16).padStart(4, "0")}`;
}

function auditOwners(owners: SemanticOwner[], json = false): OrderResult[] {
  const inventory = readInventory();
  const images = new Map<string, Uint8Array>();
  const prologueSets = new Map<string, Set<number>>();
  const results: OrderResult[] = [];
  for (const owner of owners) {
    let image = images.get(owner.overlay);
    if (image === undefined) {
      image = overlayImage(owner.overlay);
      images.set(owner.overlay, image);
      prologueSets.set(owner.overlay, inventoryPrologues(inventory, owner.overlay));
    }
    const source = readFileSync(owner.source, "utf8");
    const sourceSequence = sourceCalls(source, owner.symbol);
    const machineSequence = machineCalls(owner, image, prologueSets.get(owner.overlay)!);
    results.push(compareOrder(owner, sourceSequence, machineSequence));
  }
  if (json) {
    console.log(JSON.stringify(results, null, 2));
  } else {
    let failures = 0;
    let unresolved = 0;
    for (const result of results) {
      unresolved += result.unresolvedCallVia;
      if (result.mismatches.length === 0) continue;
      failures++;
      console.log(`FAIL ${formatOwner(result.owner)} source=${result.sourceCalls.length} machine=${result.machineCalls.length}`);
      for (const mismatch of result.mismatches.slice(0, 8)) {
        const source = mismatch.source ?? "<none>";
        const machine = mismatch.machine ?? "<none>";
        const site = mismatch.site === undefined ? "" : ` site=0x${(OVERLAY_BASE + mismatch.site).toString(16)}`;
        console.log(`  [${mismatch.index}] source=${source} machine=${machine}${site} (${mismatch.detail ?? "mismatch"})`);
      }
      if (result.mismatches.length > 8) console.log(`  ... ${result.mismatches.length - 8} more mismatch(es)`);
    }
    console.log(`owners=${results.length} passed=${results.length - failures} failed=${failures} unresolved_call_via=${unresolved}`);
  }
  if (results.some((result) => result.mismatches.length > 0)) process.exitCode = 1;
  return results;
}

function parseArgs(args: string[]): { overlay?: string; start?: number; end?: number; all: boolean; json: boolean } {
  let overlay: string | undefined;
  const bounds: number[] = [];
  let all = false;
  let json = false;
  for (const argument of args) {
    if (argument === "--all") { all = true; continue; }
    if (argument === "--json") { json = true; continue; }
    if (/^resource_[0-9a-f]+$/i.test(argument)) {
      if (overlay !== undefined) throw new Error("only one resource overlay may be selected");
      overlay = argument.toLowerCase();
      continue;
    }
    if (/^(?:0x)?[0-9a-f]{1,4}$/i.test(argument)) {
      bounds.push(Number.parseInt(argument.replace(/^0x/i, ""), 16));
      continue;
    }
    throw new Error(`unrecognised argument ${JSON.stringify(argument)}`);
  }
  if (bounds.length > 2) throw new Error("at most two owner bounds are accepted");
  if (overlay === undefined && !all) throw new Error("select an overlay or pass --all");
  if (overlay === undefined && bounds.length > 0) throw new Error("owner bounds require an overlay");
  if (all && overlay !== undefined) throw new Error("--all and an overlay are mutually exclusive");
  return { overlay, start: bounds[0], end: bounds[1], all, json };
}

function selfTest(): void {
  const source = `void Func_02000000(void) { Func_08000002(Func_08000001(1), Func_02000004()); }`;
  const expected = ["Func_08000001", "Func_02000004", "Func_08000002"];
  const actual = sourceCalls(source, "Func_02000000");
  if (JSON.stringify(actual) !== JSON.stringify(expected)) throw new Error("postorder source-call self-test failed");
  const callbackSource = `void Func_02000000(void (*callback)(void)) { if (callback) callback(); Func_08000002(); }`;
  const callbackCalls = sourceCalls(callbackSource, "Func_02000000");
  if (JSON.stringify(callbackCalls) !== JSON.stringify([CALL_VIA, "Func_08000002"]))
    throw new Error("source callback must remain visible as call_via");

  const image = new Uint8Array(0x40);
  // A call-through slot at offset 0x20: bx r3, with a literal target loaded by
  // `ldr r3,[pc,#0]` at the call site immediately before it.
  image[0x20] = 0x18; image[0x21] = 0x47;
  image[0x00] = 0x01; image[0x01] = 0x48; // ldr r0,[pc,#4] (not slot r3)
  image[0x02] = 0x03; image[0x03] = 0x48; // ldr r0,[pc,#12] (not used)
  // For a simple fixture, callViaLiteralTarget should refuse unrelated loads.
  if (callViaLiteralTarget(image, 0x20, 0x08) !== undefined) throw new Error("call_via register self-test failed");
  image[0x08] = 0x01; image[0x09] = 0x4b; // ldr r3,[pc,#4], pool at 0x10
  image[0x10] = 0xd9; image[0x11] = 0x01; image[0x12] = 0x00; image[0x13] = 0x03;
  if (callViaLiteralTarget(image, 0x20, 0x0a) !== 0x030001d8) throw new Error("call_via literal-target self-test failed");

  const owner: SemanticOwner = { overlay: "resource_test", offset: 0, spanBytes: 2, source: "self-test", symbol: "Func_02000000" };
  const via: MachineCall = { site: 0, target: 0x20, kind: "call_via", name: CALL_VIA };
  const accepted = compareOrder(owner, ["Func_030001d8"], [via]);
  if (accepted.mismatches.length !== 0 || accepted.unresolvedCallVia !== 1) throw new Error("unresolved call_via self-test failed");
  const explicit = compareOrder(owner, [CALL_VIA], [via]);
  if (explicit.mismatches.length !== 0 || explicit.unresolvedCallVia !== 1) throw new Error("explicit callback call_via self-test failed");
  const rejected = compareOrder(owner, ["Func_08000000"], [via]);
  if (rejected.mismatches.length !== 1) throw new Error("call_via source mismatch self-test failed");
  console.log("self-test=ok");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--help") || args.includes("-h")) { console.log(USAGE); return; }
  if (args.includes("--self-test")) { selfTest(); return; }
  try {
    const parsed = parseArgs(args);
    let owners = loadOwners(parsed);
    if (parsed.overlay !== undefined) owners = owners.filter((owner) => owner.overlay === parsed.overlay);
    if (parsed.start !== undefined) owners = owners.filter((owner) => owner.offset === parsed.start);
    if (parsed.end !== undefined) {
      owners = owners.filter((owner) => owner.offset + owner.spanBytes === parsed.end);
    }
    if (owners.length === 0) throw new Error("no semantic owners match the requested selection");
    if (parsed.end !== undefined && parsed.start !== undefined) {
      for (const owner of owners) {
        if (owner.offset + owner.spanBytes !== parsed.end) {
          throw new Error(`${formatOwner(owner)} has measured end 0x${(owner.offset + owner.spanBytes).toString(16)}, not 0x${parsed.end.toString(16)}`);
        }
      }
    }
    auditOwners(owners, parsed.json);
  } catch (error) {
    console.error(error instanceof Error ? error.message : String(error));
    process.exitCode = 1;
  }
}

if (import.meta.main) main();
