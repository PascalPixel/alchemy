#!/usr/bin/env bun
// Prove a semantic overlay source places every call the assembly makes — per
// target, never by total.
//
// This is the completeness proof every overlay lane runs, and until now every
// lane wrote its own throwaway for it. Four independent reports asked for it to
// be promoted into `tools/`, which is the signal to stop retyping it.
//
// Why per target and never a total: a run once showed 137 = 137 while five
// targets were mutually mis-assigned — four calls moved from one import to
// another and the total never moved. A total-count check cannot see that class
// of error at all, which is precisely the class hand-conversion produces.
//
//   bun tools/overlay_multiset_check.ts resource_398 0538
//   bun tools/overlay_multiset_check.ts resource_398          # every converted row
//   bun tools/overlay_multiset_check.ts --self-test
//
// Exit status is 1 when any row fails, so it can gate a lane's own loop.
//
// Two subtleties that cost lanes real time, both handled here:
//   * Declarations and comments must be stripped before counting `Func_x(`, or
//     every forward declaration inflates its target by one.
//   * A `bl` to the owner's OWN epilogue is a `goto`, not a call, and a target
//     inside the owner's span is an intra-overlay call that the C spells as a
//     call. The first is excluded; the second is not.
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { classify, overlayImage, resolveOverlay } from "./overlay_call_targets.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

/**
 * Marker for a `bl` through the overlay's own `call_via` bank. The destination
 * lives in a register at run time, so the assembly cannot name it; the source
 * names the real callee, which is an IWRAM address on every case seen.
 */
export const CALL_VIA = "(call_via)";

export interface Mismatch {
  target: string;
  assembly: number;
  source: number;
}

/**
 * Strip what must not be counted: block and line comments, and declarations.
 * A declaration is a `Func_x(...);` with no body — the trailing `;` directly
 * after the closing parenthesis is the tell, and it is what separates
 * `void Func_0808a018();` from a real call `Func_0808a018();`.
 *
 * Declarations are recognised by a leading type, so a bare `Func_x();` on its
 * own line still counts as the call it is.
 */
export function strippedBody(source: string): string {
  const withoutComments = source
    .replace(/\/\*[\s\S]*?\*\//g, " ")
    .replace(/\/\/[^\n]*/g, " ");
  // A declaration OR the owner's own definition has a RETURN TYPE before the
  // name; a call has either nothing before it or an operator (`=`, `if (`,
  // `return`). Match on that shape rather than on a list of type keywords: the
  // first version listed the scalar typedefs and missed
  // `struct SceneEntity *Func_0808a080();`, which made four freshly-converted
  // overlays look like they had an extra call each.
  //
  // The test is: some identifier/`*` text precedes the name on the line, with
  // no `=` and no `(` in it, and the line is not a control-flow keyword.
  return withoutComments
    .split("\n")
    .filter((line) => {
      const declaration = /^\s*([A-Za-z_][A-Za-z0-9_\s*]*?)\bFunc_[0-9a-f]{8}\s*\(/.exec(line);
      if (declaration === null) return true;
      const before = declaration[1];
      if (before.trim() === "") return true; // a bare `Func_x(...)` call
      if (/[=(]/.test(before)) return true; // `x = Func_x(...)`, `if (Func_x(...))`
      if (/\b(?:if|while|for|return|else|do|switch|case)\b/.test(before)) return true;
      return false; // a return type precedes the name: declaration or definition
    })
    .join("\n");
}

/** How many times each `Func_xxxxxxxx(` is actually invoked in the body. */
export function sourceCounts(source: string): Map<string, number> {
  const counts = new Map<string, number>();
  for (const match of strippedBody(source).matchAll(/\b(Func_[0-9a-f]{8})\s*\(/g)) {
    counts.set(match[1], (counts.get(match[1]) ?? 0) + 1);
  }
  return counts;
}

/**
 * What the assembly calls, per target name, for one owner. Veneer targets are
 * named by the import they forward to; an in-overlay prologue is named by its
 * own overlay address. A `bl` landing on the owner's epilogue is a `goto` and
 * is excluded — five of them in `resource_3c4:259c` alone.
 */
export function assemblyCounts(overlay: string, owner: number, span: number): Map<string, number> {
  const image = overlayImage(overlay);
  const counts = new Map<string, number>();
  for (const site of resolveOverlay(overlay, owner, owner + span)) {
    const detail = classify(image, site.target, new Set());
    let name: string;
    if (detail.kind === "veneer" && detail.imported !== undefined) {
      name = `Func_${detail.imported.toString(16).padStart(8, "0")}`;
    } else if (site.target > owner && site.target < owner + span) {
      continue; // a long `bl` inside our own body is a goto, not a call
    } else if (detail.kind === "unknown") {
      // A LITERAL POOL WORD OF THE FORM `f0xx f8xx` DECODES AS A BL PAIR. The
      // walk here is a flat 2-byte stride with no pool map, so those words are
      // seen as calls to addresses that are not functions — and they surface as
      // `asm=1 src=0`, which reads as a dropped call. Three of four such reports
      // in one audit were pool words (`resource_399:00d8` 0xf896f001,
      // `resource_3b5:007c` 0xf8b6f001, `resource_383:48c8` 0xfd92f005), and a
      // repair lane came close to inventing calls to match them.
      //
      // An unresolvable target cannot be named, so it can never match anything
      // in the C either way; counting it can only manufacture a defect. A
      // genuinely dropped call still shows up, because its target resolves to a
      // veneer or a prologue and reports `src=0` under a real name.
      continue;
    } else if (detail.kind === "call_via") {
      // The slot forwards to whatever the register holds, so the assembly
      // cannot name the destination — but the source can and does, usually an
      // IWRAM `Func_030xxxxx`. Record the site under a marker the comparison
      // reconciles against those names, rather than under the slot's own
      // address, which would produce a phantom pair at identical counts.
      name = CALL_VIA;
    } else {
      name = `Func_${(0x02000000 + site.target).toString(16).padStart(8, "0")}`;
    }
    counts.set(name, (counts.get(name) ?? 0) + 1);
  }
  return counts;
}

export function compare(assembly: Map<string, number>, source: Map<string, number>): Mismatch[] {
  const mismatches: Mismatch[] = [];
  // Reconcile `call_via` sites against the IWRAM callees the source names. Both
  // sides are counted, so a genuine miscount still shows: only the pairing of
  // "N indirect sites" with "N IWRAM calls" is forgiven.
  const viaSites = assembly.get(CALL_VIA) ?? 0;
  if (viaSites > 0) {
    assembly = new Map(assembly);
    source = new Map(source);
    assembly.delete(CALL_VIA);
    let remaining = viaSites;
    for (const [name, count] of [...source]) {
      if (remaining === 0) break;
      if (!/^Func_03[0-9a-f]{6}$/.test(name)) continue;
      const taken = Math.min(count, remaining);
      remaining -= taken;
      if (count === taken) source.delete(name);
      else source.set(name, count - taken);
    }
    if (remaining > 0) mismatches.push({ target: CALL_VIA, assembly: remaining, source: 0 });
  }
  for (const target of new Set([...assembly.keys(), ...source.keys()])) {
    const left = assembly.get(target) ?? 0;
    const right = source.get(target) ?? 0;
    if (left !== right) mismatches.push({ target, assembly: left, source: right });
  }
  return mismatches.sort((a, b) => Math.abs(b.assembly - b.source) - Math.abs(a.assembly - a.source));
}

interface Row {
  overlay: string;
  offset: number;
  span_bytes: number;
}

/**
 * Rows to check for one overlay: the discovery inventory, PLUS the reviewed
 * boundaries in `semantic/regions.json`.
 *
 * The inventory is not complete. Sweeping the two-byte gaps in
 * `metrics/gs1-en-executable.json` for `push {..,lr}` prologues turns up owners
 * the discovery pass never recorded — 36 of them in one sweep, 15 provably
 * called by a `bl` in their own overlay. Those owners have no inventory row, so
 * an inventory-only scan reported `checked=0` for a freshly converted file and
 * looked like a pass. A derived owner is admitted by a `manual_regions` entry,
 * which carries the same `entry`/`span_bytes` this check needs, so read it too
 * and let the inventory win where both describe the same offset.
 */
function rows(overlay: string): Row[] {
  const inventory = JSON.parse(
    readFileSync(join(ROOT, "out", "decomp", "overlays.json"), "utf8"),
  ) as { functions: Row[] };
  const found = inventory.functions.filter((row) => row.overlay === overlay);
  const manualPath = join(ROOT, "semantic", "regions.json");
  if (!existsSync(manualPath)) return found;
  const manual = JSON.parse(readFileSync(manualPath, "utf8")) as {
    manual_regions?: { overlay: string; entry: string; span_bytes: number }[];
  };
  for (const region of manual.manual_regions ?? []) {
    if (region.overlay !== overlay) continue;
    const offset = Number.parseInt(region.entry, 16) - 0x02000000;
    if (found.some((row) => row.offset === offset)) continue;
    found.push({ overlay, offset, span_bytes: region.span_bytes });
  }
  return found.sort((a, b) => a.offset - b.offset);
}

function selfTest(): void {
  const source = `
/* Func_08000000(); in a comment must not count. */
void Func_0808a018();          // a declaration must not count
s32 Func_080770c0();
struct SceneEntity *Func_0808a090();   /* a pointer return type, still a declaration */
extern u32 Func_080770c8();
void Func_0200011c(void)
{
    struct SceneEntity *entity;
    Func_0808a018();
    Func_0808a018();
    entity = Func_0808a090(8);
    if (Func_080770c0(0x809)) return;   // Func_08000000() here is a comment too
    result = Func_080770c8(0x80a);
}
`;
  const counts = sourceCounts(source);
  if (counts.get("Func_0808a018") !== 2) throw new Error("two calls must count as two");
  if (counts.get("Func_0808a090") !== 1) throw new Error("a pointer-returning declaration must not count");
  if (counts.get("Func_080770c8") !== 1) throw new Error("an assigned call must count");
  if (counts.get("Func_080770c0") !== 1) throw new Error("a call inside a test still counts");
  if (counts.has("Func_08000000")) throw new Error("comments must not be counted");

  const assembly = new Map([
    ["Func_0808a018", 2],
    ["Func_080770c0", 1],
    ["Func_0808a090", 1],
    ["Func_080770c8", 1],
  ]);
  if (compare(assembly, counts).length !== 0) throw new Error("an exact match must report nothing");

  // The failure this tool exists for: a total that matches while two targets
  // are mutually mis-assigned.
  const swapped = new Map(assembly);
  swapped.set("Func_0808a018", 1);
  swapped.set("Func_080770c0", 2);
  const found = compare(assembly, swapped);
  if (found.length !== 2) throw new Error("a mutual mis-assignment must report both targets");
  const total = (map: Map<string, number>) => [...map.values()].reduce((sum, n) => sum + n, 0);
  if (total(assembly) !== total(swapped)) throw new Error("the totals must be equal, or this proves nothing");
  console.log("self-test=ok");
}

function main(): void {
  const args = Bun.argv.slice(2);
  if (args.includes("--self-test")) return selfTest();
  const overlay = args.find((argument) => /^resource_[0-9a-f]+$/.test(argument));
  if (overlay === undefined) {
    console.log("usage: overlay_multiset_check.ts <resource_NNN> [ownerHex]");
    process.exitCode = 1;
    return;
  }
  const ownerText = args.find((argument) => /^[0-9a-f]{1,4}$/i.test(argument));
  const wanted = ownerText === undefined ? undefined : parseInt(ownerText, 16);
  let checked = 0;
  let failed = 0;
  for (const row of rows(overlay)) {
    if (wanted !== undefined && row.offset !== wanted) continue;
    const address = (0x02000000 + row.offset).toString(16).padStart(8, "0");
    const path = join(ROOT, "semantic", "overlays", `${overlay}_c_${address}.c`);
    if (!existsSync(path)) continue;
    const mismatches = compare(
      assemblyCounts(overlay, row.offset, row.span_bytes),
      sourceCounts(readFileSync(path, "utf8")),
    );
    checked += 1;
    if (mismatches.length === 0) {
      console.log(`0x${address}  MATCH`);
      continue;
    }
    failed += 1;
    console.log(`0x${address}  MISMATCH on ${mismatches.length} target(s)`);
    for (const mismatch of mismatches) {
      console.log(`    ${mismatch.target}  assembly=${mismatch.assembly}  source=${mismatch.source}`);
    }
  }
  console.log(`\nchecked=${checked} failed=${failed}`);
  if (failed > 0) process.exitCode = 1;
}

if (import.meta.main) main();
