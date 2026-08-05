#!/usr/bin/env bun
// alchemist: a diagnosis-driven source permuter. Reads candidate_explain.ts's
// scheduler tier diagnosis and applies ONLY the source transformations that
// diagnosis licenses, instead of permuting blindly.
//
// The link between diagnosis and move set is the whole point:
//   - A differing row decided by the "original-order" tier means the pair
//     tied through priority/class/depend-count and gcc broke the tie by insn
//     creation order -- which for independent statements IS source statement
//     order.  Those rows license the statement-order move family: sink or
//     hoist a statement within its block, and the split-store play (hoist a
//     store's memory-read RHS into a temp declared at block top -- keeping
//     the load's early creation slot -- then sink only the store), which is
//     what closed resource_378:26f0.
//   - Differing rows decided by "priority" or flagged "?? model expects"
//     (an unmodeled tier: register pressure, cost==1, dest-order hook) are
//     NOT fixable by statement order; if no differing row licenses a move,
//     this tool says so and stops rather than burning compiles.  That
//     refusal is a result, not a failure: it routes the owner to the
//     documented dead-end classes instead of a guessing loop.
//
// Search is greedy hill-climb on differing-byte count with a compile budget.
// Call statements are pinned relative to each other (reference call order is
// ground truth per the project's craft rules), so the move space stays small.
// Only a byte-exact variant is ever written back (--write); improvements
// short of exact are printed for a human or agent to consider, because a
// lower differing count alone does not prove the reordering is the
// reference's true shape.
import { existsSync, mkdirSync, readdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join, resolve } from "node:path";
import { assembleOverlay, compileOverlayCandidate, OVERLAY_BASE } from "./overlay_disasm.ts";
import { parseInsns } from "./rtl_insn.ts";
import { parseInstruction, type Instruction } from "./thumb_disasm.ts";
import { align } from "./rtl_align.ts";
import { diagnose, parseDependenceTable } from "./rtl_schedule.ts";
import { disassembleInstructions } from "./candidate_explain.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const EXPLAIN_FLAGS = ["-dR", "-fsched-verbose=9"] as const;

// ---------------------------------------------------------------------------
// Statement model: a function body parsed into nested scopes of units, where
// a unit is one whole statement (single line, or a brace-balanced block).
// The project's semantic C is one-statement-per-line, which this relies on.

export interface Unit {
  kind: "statement" | "block" | "declaration";
  lines: string[];       // verbatim source lines
  pinned: boolean;       // contains a call -- never reordered across another pinned unit
  scope: Unit[] | null;  // for blocks: the units inside (excluding the brace lines)
}

const DECLARATION = /^\s*(?:extern\s+)?(?:u8|u16|u32|s8|s16|s32|int|char|void|short|long|unsigned|signed)\b[^=()]*;\s*$/;
const CALLISH = /\b[A-Za-z_][A-Za-z0-9_]*\s*\(/;

// A control-flow header with no `{` governs whatever statement follows it, so
// the two are one indivisible construct even though they are separate lines.
// A header that DOES open a brace is parsed as a block instead and never
// reaches this test. Reordering either half independently silently rebinds
// which statement the condition controls: on resource_3ba:31c0 an
// `if (...)` was hoisted above two assignments, making the first conditional
// and the intended body unconditional, and on resource_3bd:bc8 a header sank
// below its own body and captured the NEXT `if` instead. Both still reduced
// the differing-byte count, so only a structural rule can reject them.
const BRACELESS_CONTROL_HEADER = /^\s*(?:\}\s*)?(?:else\s+if\s*\(|if\s*\(|for\s*\(|while\s*\(|switch\s*\(|else\b|do\b)[^{]*$/;

// A `goto` target, and the jump statements that decide what is reachable.
// These anchor control flow exactly as a braceless header does: hoisting a
// `return` makes the statements it passed dead, sinking it makes statements
// live that previously were not, and moving anything across a label changes
// whether the fallthrough path or only the `goto` path reaches it.
// Matched ANYWHERE in the line, not just at its start: a one-line guard such
// as `if (renderB != 9) continue;` is a self-contained statement the header
// rule above deliberately does not catch, yet moving it past an assignment
// still changes what executes on the jump path. `\b` keeps this off
// identifiers that merely begin with a keyword (`returnValue`).
const LABEL = /^\s*[A-Za-z_][A-Za-z0-9_]*\s*:\s*$/;
const JUMP = /\b(?:return|goto|break|continue)\b/;

function isControlAnchor(unit: Unit): boolean {
  if (unit.kind !== "statement" || unit.lines.length !== 1) return false;
  const line = unit.lines[0];
  if (LABEL.test(line)) return true;
  if (JUMP.test(line)) return true;
  return BRACELESS_CONTROL_HEADER.test(line) && !/;\s*$/.test(line);
}

function parseScope(lines: string[], start: number, end: number): Unit[] {
  const units: Unit[] = [];
  let index = start;
  while (index < end) {
    const line = lines[index];
    const trimmed = line.trim();
    if (trimmed === "") {
      units.push({ kind: "declaration", lines: [line], pinned: true, scope: null });
      index++;
      continue;
    }
    if (trimmed.startsWith("/*") || trimmed.startsWith("//") || /^\*(\s|\/|$)/.test(trimmed)) {
      // Comments attach to nothing; treat each run as an immovable unit so
      // rewrites keep them in place.
      units.push({ kind: "declaration", lines: [line], pinned: true, scope: null });
      index++;
      continue;
    }
    const opensBlock = /[{]\s*$/.test(trimmed);
    if (opensBlock) {
      let depth = 0;
      let scan = index;
      for (; scan < end; scan++) {
        for (const ch of lines[scan]) {
          if (ch === "{") depth++;
          else if (ch === "}") depth--;
        }
        if (depth === 0) break;
      }
      const inner = parseScope(lines, index + 1, scan);
      units.push({
        kind: "block",
        lines: lines.slice(index, scan + 1),
        pinned: true, // blocks stay put; their inner statements permute
        scope: inner,
      });
      index = scan + 1;
      continue;
    }
    if (DECLARATION.test(line)) {
      units.push({ kind: "declaration", lines: [line], pinned: true, scope: null });
      index++;
      continue;
    }
    units.push({
      kind: "statement",
      lines: [line],
      pinned: CALLISH.test(trimmed.replace(/\*\([^)]*\)/g, "")),
      scope: null,
    });
    index++;
  }
  return units;
}

export interface ParsedFunction {
  before: string[]; // everything above the function body's first inner line
  body: Unit[];
  after: string[];  // closing brace onward
}

export function parseFunction(source: string, functionName: string): ParsedFunction | null {
  const lines = source.split("\n");
  const definition = lines.findIndex(
    (line) => line.includes(functionName) && line.includes("(") && !line.trimStart().startsWith("extern") && !/;\s*$/.test(line),
  );
  if (definition === -1) return null;
  let open = definition;
  while (open < lines.length && !lines[open].includes("{")) open++;
  if (open === lines.length) return null;
  let depth = 0;
  let close = open;
  for (; close < lines.length; close++) {
    for (const ch of lines[close]) {
      if (ch === "{") depth++;
      else if (ch === "}") depth--;
    }
    if (depth === 0) break;
  }
  return {
    before: lines.slice(0, open + 1),
    body: parseScope(lines, open + 1, close),
    after: lines.slice(close),
  };
}

function renderScope(units: Unit[]): string[] {
  return units.flatMap((unit) => {
    if (unit.kind !== "block" || unit.scope === null) return unit.lines;
    // A one-line block (its own opening AND closing brace on the same source
    // line -- either a genuinely empty `if (x) { }`, or a cascading
    // `} else if (...) {` line, which closes the PREVIOUS sibling block and
    // opens this one in a single line) has unit.lines[0] === the last
    // element too. Emitting the open/scope/close triple unconditionally
    // duplicated that one line, which on a chain of textually-identical
    // `} else if (SAME_COND) {` lines silently multiplied the source (see
    // resource_371:28e8, byte-diff-neutral since the duplicated branches
    // were empty, so the compile gate never caught it).
    if (unit.lines.length === 1) return unit.lines;
    return [unit.lines[0], ...renderScope(unit.scope), unit.lines[unit.lines.length - 1]];
  });
}

export function renderFunction(parsed: ParsedFunction): string {
  return [...parsed.before, ...renderScope(parsed.body), ...parsed.after].join("\n");
}

// ---------------------------------------------------------------------------
// Move generation.  Each move is a pure function from ParsedFunction to a new
// ParsedFunction (deep-copied), with a human-readable description an agent
// can replay by hand.

export interface Move {
  description: string;
  apply(parsed: ParsedFunction): ParsedFunction;
}

function cloneUnits(units: Unit[]): Unit[] {
  return units.map((unit) => ({
    ...unit,
    lines: [...unit.lines],
    scope: unit.scope === null ? null : cloneUnits(unit.scope),
  }));
}

function cloneParsed(parsed: ParsedFunction): ParsedFunction {
  return { before: [...parsed.before], body: cloneUnits(parsed.body), after: [...parsed.after] };
}

// Walks every scope (function body and nested blocks); the callback receives
// a scope's unit array plus a path label for descriptions.
function forEachScope(units: Unit[], label: string, callback: (scope: Unit[], label: string) => void): void {
  callback(units, label);
  units.forEach((unit, index) => {
    if (unit.scope !== null) forEachScope(unit.scope, `${label}.${index}`, callback);
  });
}

function scopeAt(parsed: ParsedFunction, label: string): Unit[] {
  let units = parsed.body;
  for (const part of label.split(".").slice(1)) {
    const inner = units[Number(part)].scope;
    if (inner === null) throw new Error(`no scope at ${label}`);
    units = inner;
  }
  return units;
}

const STORE_SPLIT = /^(\s*)(\*\(\s*[a-z0-9_]+\s*\*+\s*\)\s*\([^;]*\)|[A-Za-z_][A-Za-z0-9_[\]>.\s-]*)\s*=\s*(.+);\s*$/;

// What a statement does to plain local NAMES. `writes` is set only for a
// simple `name = ...` (or compound-assign) form: a store through a pointer,
// `*(s32 *)(p + 8) = v`, targets memory rather than a local, so it defines no
// name here. `reads` is every other identifier mentioned, which harmlessly
// includes callee and type names -- those are never local definitions, so
// they can never produce a false conflict.
function statementEffects(line: string): { writes: string | null; reads: Set<string> } {
  const trimmed = line.trim();
  const assign = /^([A-Za-z_][A-Za-z0-9_]*)\s*(=(?!=)|\+=|-=|\*=|\/=|\|=|&=|\^=|<<=|>>=)/.exec(trimmed);
  const writes = assign === null ? null : assign[1];
  const rest = assign === null ? trimmed : trimmed.slice(assign[0].length);
  const reads = new Set<string>();
  for (const match of rest.matchAll(/\b([A-Za-z_][A-Za-z0-9_]*)\b/g)) reads.add(match[1]);
  if (assign !== null && assign[2] !== "=") reads.add(assign[1]); // `x += 1` reads x too
  return { writes, reads };
}

// Reordering is only legal when it preserves data flow. Without this, the
// generator happily proposed moves that reuse a stale value or read a local
// before its only write -- three separate instances were found downstream
// (resource_3c5:28a0, resource_375:be0, resource_375:1760), each one
// byte-diff-neutral by coincidence (undefined behavior that happened to
// compile to the same bytes), so the compile gate could never catch them.
// Classic dependence test: the moved statement's definition must not cross a
// read or redefinition of that name (true/output dependence), and its own
// inputs must not cross a redefinition of those names (anti-dependence).
// The split temp must carry the captured value's own type. It was always
// declared `s32`, which silently narrows a pointer into an integer -- a
// constraint violation gcc only warns about, and one this target's 32-bit
// pointers hide (seen on resource_3ae:e40, where `u8 *sub` became
// `s32 permuted_191`). Recover the type from the assignment's destination,
// and return null when it cannot be established so the caller can decline
// to offer the move rather than guess.
function splitTempType(parsed: ParsedFunction, lhs: string): string | null {
  const target = lhs.trim();
  // `*(u8 **)(p + 0x50) = v` stores a `u8 *`: the cast names the pointer to
  // the destination, so the value is one indirection less.
  const cast = /^\*\(\s*(?:const\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*(\*+)\s*\)/.exec(target);
  if (cast !== null) {
    const stars = cast[2].length - 1;
    return stars === 0 ? cast[1] : `${cast[1]} ${"*".repeat(stars)}`;
  }
  const name = /^([A-Za-z_][A-Za-z0-9_]*)\s*(\[)?/.exec(target);
  if (name === null) return null;
  const isElement = name[2] === "[";
  const declaration = new RegExp(
    `^\\s*(?:const\\s+)?((?:unsigned\\s+|signed\\s+)?[A-Za-z_][A-Za-z0-9_]*(?:\\s*\\*+)?)\\s*${name[1]}\\s*(\\[[^\\]]*\\])?\\s*;`,
  );
  for (const line of [...parsed.before, ...parsed.body.flatMap((unit) => unit.lines)]) {
    const found = declaration.exec(line);
    if (found === null) continue;
    // An element of `s32 place[3]` is an `s32`; the array itself is not a
    // value this move can capture, so a non-element match on an array
    // declaration is not usable.
    if (isElement !== (found[2] !== undefined)) return null;
    return found[1].replace(/\s+/g, " ").trim();
  }
  return null;
}

function preservesDataflow(unit: Unit, crossed: Unit[]): boolean {
  const moving = statementEffects(unit.lines[0]);
  for (const other of crossed) {
    for (const line of other.lines) {
      const effects = statementEffects(line);
      if (moving.writes !== null && (effects.reads.has(moving.writes) || effects.writes === moving.writes)) return false;
      if (effects.writes !== null && moving.reads.has(effects.writes)) return false;
    }
  }
  return true;
}

export function generateMoves(parsed: ParsedFunction): Move[] {
  const moves: Move[] = [];
  forEachScope(parsed.body, "body", (scope, label) => {
    for (let index = 0; index < scope.length; index++) {
      const unit = scope[index];
      if (unit.kind !== "statement") continue;

      // Sink/hoist by 1..3 positions, never moving a pinned (call) statement
      // and never crossing another pinned unit with a pinned one -- calls
      // keep their relative order (it is reference ground truth).
      for (const distance of [1, 2, 3]) {
        for (const direction of [1, -1]) {
          const target = index + distance * direction;
          if (target < 0 || target >= scope.length) continue;
          const span = scope.slice(Math.min(index, target), Math.max(index, target) + 1);
          if (unit.pinned && span.some((other) => other !== unit && other.pinned)) continue;
          if (span.some((other) => other.kind === "declaration")) continue;
          // Never move a braceless control header, never move the statement
          // it governs away from it, and never cross one -- all three rebind
          // what the condition controls.
          if (isControlAnchor(unit)) continue;
          if (index > 0 && isControlAnchor(scope[index - 1])) continue;
          if (span.some(isControlAnchor)) continue;
          if (!preservesDataflow(unit, span.filter((other) => other !== unit))) continue;
          moves.push({
            description: `${direction === 1 ? "sink" : "hoist"} [${label}#${index}] "${unit.lines[0].trim().slice(0, 60)}" by ${distance}`,
            apply: (input) => {
              const output = cloneParsed(input);
              const editable = scopeAt(output, label);
              const [moved] = editable.splice(index, 1);
              editable.splice(target, 0, moved);
              return output;
            },
          });
        }
      }

      // Split-store: `LHS = RHS;` where RHS reads memory -> hoist the read
      // into a temp assigned at the statement's original slot (preserving the
      // load's creation order), sink only the store.  This is the exact play
      // that closed resource_378:26f0.
      const match = STORE_SPLIT.exec(unit.lines[0]);
      if (match !== null && (match[3].includes("*(") || /\[[^\]]+\]/.test(match[3]))) {
        const [, indent, lhs, rhs] = match;
        for (const distance of [1, 2, 3]) {
          if (index + distance >= scope.length) continue;
          const crossed = scope.slice(index + 1, index + distance + 1);
          if (crossed.some((other) => other.kind === "declaration")) continue;
          // Same braceless-header rule as sink/hoist: the sunk store must not
          // cross a header, and a statement a header governs cannot be split
          // out from under it.
          if (crossed.some(isControlAnchor)) continue;
          if (index > 0 && isControlAnchor(scope[index - 1])) continue;
          // Same dependence rule as sink/hoist above, applied to the halves
          // this move actually creates: the sunk `lhs = temp;` store must not
          // cross a read or redefinition of lhs, and the temp's own inputs
          // (the RHS it captures) must not cross a redefinition.
          const halves: Unit = {
            kind: "statement",
            lines: [`${lhs} = ${rhs};`],
            pinned: false,
            scope: null,
          };
          if (!preservesDataflow(halves, crossed)) continue;
          const tempType = splitTempType(parsed, lhs);
          if (tempType === null) continue;
          moves.push({
            description: `split [${label}#${index}] "${unit.lines[0].trim().slice(0, 60)}": temp at slot, store sunk by ${distance}`,
            apply: (input) => {
              const output = cloneParsed(input);
              const editable = scopeAt(output, label);
              const temp = `permuted_${index}`;
              editable[index] = { kind: "statement", lines: [`${indent}${temp} = ${rhs};`], pinned: false, scope: null };
              editable.splice(index + distance + 1, 0, {
                kind: "statement", lines: [`${indent}${lhs} = ${temp};`], pinned: false, scope: null,
              });
              // C89: the declaration goes at the top of the function body,
              // after any existing declarations.
              let insertAt = 0;
              while (insertAt < output.body.length && output.body[insertAt].kind === "declaration") insertAt++;
              const declIndent = /^\s*/.exec(output.body[0]?.lines[0] ?? "    ")?.[0] ?? "    ";
              output.body.splice(insertAt, 0, {
                kind: "declaration",
                lines: [`${declIndent}${tempType}${tempType.endsWith("*") ? "" : " "}${temp};`],
                pinned: true,
                scope: null,
              });
              return output;
            },
          });
        }
      }
    }
  });
  return moves;
}

// ---------------------------------------------------------------------------
// Diagnosis gate: compile once with scheduler dumps, align, and check whether
// any DIFFERING row's decision was made by the original-order tier.

interface Gate {
  differingBytes: number;
  licensed: boolean;
  tiersSeen: Set<string>;
}

function differingByteCount(actual: Uint8Array, expected: Uint8Array): number {
  let count = 0;
  const longest = Math.max(actual.length, expected.length);
  for (let index = 0; index < longest; index++) {
    if (actual[index] !== expected[index]) count++;
  }
  return count;
}

function gateDiagnosis(
  sourcePath: string, work: string, overlay: string, routingSource: string, expected: Uint8Array,
): Gate {
  const compiled = compileOverlayCandidate(sourcePath, work, overlay, routingSource, [...EXPLAIN_FLAGS]);
  const differingBytes = differingByteCount(compiled.data, expected);
  const gate: Gate = { differingBytes, licensed: false, tiersSeen: new Set() };
  if (differingBytes === 0) return gate;

  // gcc names the dump after the preprocessed input's own basename; glob for
  // it rather than assuming the pass number.
  const sourceStem = basename(sourcePath);
  const dumpName = readdirSync(work).find((name) => name.startsWith(`${sourceStem}.`) && name.endsWith(".sched2"))
    ?? readdirSync(work).find((name) => name.startsWith(`${sourceStem}.`) && name.endsWith(".sched"));
  if (dumpName === undefined) return gate;
  const dumpPath = join(work, dumpName);
  const dumpText = readFileSync(dumpPath, "utf8");
  const insns = parseInsns(dumpText);
  const table = parseDependenceTable(dumpText);
  const insnsByUid = new Map(insns.map((insn) => [insn.uid, insn]));

  const binPath = join(work, "permute-gate.bin");
  writeFileSync(binPath, compiled.data);
  const instructions: Instruction[] = disassembleInstructions(binPath);
  const alignment = align(insns, instructions);
  const byOffset = new Map(alignment.pairs.map((pair) => [pair.instruction.offset, pair.insn]));

  const lines = dumpText.split("\n");
  const pickedMarker = /--> scheduling insn <<<(\d+)>>>/;
  for (const instruction of instructions) {
    const offset = instruction.offset;
    const halfword = offset & ~1;
    if (compiled.data[halfword] === expected[halfword] && compiled.data[halfword + 1] === expected[halfword + 1]) continue;
    const insn = byOffset.get(offset);
    if (insn === undefined) { gate.tiersSeen.add("unaligned"); continue; }
    const ownIndex = lines.findIndex((line) => line.includes(`--> scheduling insn <<<${insn.uid}>>>`));
    if (ownIndex === -1) { gate.tiersSeen.add("no-decision"); continue; }
    let readyUids: number[] = [];
    for (let back = ownIndex - 1; back >= 0; back--) {
      const ready = /Ready list \(t = *\d+\): *(.*)$/.exec(lines[back]);
      if (ready !== null) { readyUids = ready[1].trim().split(/\s+/).filter(Boolean).map(Number); break; }
      if (pickedMarker.test(lines[back])) break;
    }
    let lastScheduledUid: number | null = null;
    for (let back = ownIndex - 1; back >= 0; back--) {
      const picked = pickedMarker.exec(lines[back]);
      if (picked !== null) { lastScheduledUid = Number(picked[1]); break; }
    }
    const result = diagnose(readyUids, insn.uid, { table, lastScheduledUid, insnsByUid });
    for (const pair of result.perRival) {
      if (pair.winner !== insn.uid) { gate.tiersSeen.add("model-divergence"); continue; }
      gate.tiersSeen.add(pair.tier);
      if (pair.tier === "original-order") gate.licensed = true;
    }
  }
  return gate;
}

// ---------------------------------------------------------------------------

interface Options {
  id: string;
  overlay: string;
  offset: number;
  source: string;
  routingSource: string;
  work: string;
  span?: number;
  budget: number;
  write: boolean;
  force: boolean;
  json: boolean;
}

// Owners routinely miss the overlays.json inventory (address-only owners);
// semantic/regions.json's manual_regions is the project's span registry for
// those, so agents never have to pass --span by hand.
export function resolveSpan(overlay: string, offset: number): number | undefined {
  const inventoryPath = join(ROOT, "out/decomp/overlays.json");
  if (existsSync(inventoryPath)) {
    const inventory = JSON.parse(readFileSync(inventoryPath, "utf8")) as { functions: Array<{ id: string; span_bytes: number }> };
    const row = inventory.functions.find((item) => item.id === `${overlay}:${offset.toString(16)}`);
    if (row !== undefined) return row.span_bytes;
  }
  const regionsPath = join(ROOT, "semantic/regions.json");
  if (existsSync(regionsPath)) {
    const regions = JSON.parse(readFileSync(regionsPath, "utf8")) as { manual_regions: Array<{ overlay: string; entry: string; span_bytes: number }> };
    const entry = `0x${(OVERLAY_BASE + offset).toString(16).padStart(8, "0")}`;
    const row = regions.manual_regions.find((item) => item.overlay === overlay && item.entry === entry);
    if (row !== undefined) return row.span_bytes;
  }
  return undefined;
}

function optionsOf(argv: string[]): Options {
  let id = "";
  let source = "";
  let routingSource = "";
  let work = "";
  let span: number | undefined;
  let budget = 60;
  let write = false;
  let force = false;
  let json = false;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--source") source = argv[++index] ?? "";
    else if (argument === "--routing-source") routingSource = argv[++index] ?? "";
    else if (argument === "--work") work = argv[++index] ?? "";
    else if (argument === "--span") span = Number(argv[++index]);
    else if (argument === "--budget") budget = Number(argv[++index]);
    else if (argument === "--write") write = true;
    else if (argument === "--force") force = true;
    else if (argument === "--json") json = true;
    else if (argument === "-h" || argument === "--help") {
      console.log(
        "usage: alchemist.ts OVERLAY:OFFSET [--source FILE] [--span BYTES] [--budget N] [--write] [--force] [--json]\n" +
        "Reads the scheduler tier diagnosis and applies only the licensed source moves\n" +
        "(statement sink/hoist, split-store).  --source defaults to the owner's\n" +
        "semantic/overlays file; --span resolves from the inventory or\n" +
        "semantic/regions.json.  --write saves the source ONLY when a variant is\n" +
        "byte-exact.  --force runs the search even when no differing row was decided\n" +
        "by the original-order tier (normally a proven dead end).  --json prints one\n" +
        "machine-readable verdict object with an explicit `next` action.",
      );
      process.exit(0);
    } else if (id === "") id = argument;
    else throw new Error(`unrecognised argument: ${argument}`);
  }
  const match = /^(resource_[0-9a-f]+):(?:0x)?([0-9a-f]+)$/i.exec(id);
  if (match === null) throw new Error("usage: alchemist.ts OVERLAY:OFFSET [--source FILE] [--span BYTES]");
  const overlay = match[1];
  const offset = Number.parseInt(match[2], 16);
  const stem = (OVERLAY_BASE + offset).toString(16).padStart(8, "0");
  if (source === "") source = join(ROOT, "semantic", `${overlay}_c_${stem}.c`);
  if (routingSource === "") routingSource = join(ROOT, "exact", `${overlay}_c_${stem}.c`);
  return {
    id, overlay, offset,
    source: resolve(source), routingSource: resolve(routingSource),
    // Per-owner work directory: parallel lanes running different owners never
    // share compile intermediates or scheduler dumps.
    work: resolve(work || join(ROOT, "work/alchemist", `${overlay}_${offset.toString(16)}`)),
    span, budget, write, force, json,
  };
}

// The one-line contract a low-context agent acts on without interpretation.
interface Verdict {
  id: string;
  verdict: "exact" | "improved" | "refused" | "exhausted" | "unnecessary";
  baseline_differing_bytes: number;
  differing_bytes: number;
  tiers: string[];
  compiles: number;
  moves: string[];
  source: string;
  saved: string | null;
  next: string;
}

function emitVerdict(verdict: Verdict, json: boolean): void {
  if (json) {
    console.log(JSON.stringify(verdict));
    return;
  }
  console.log(
    `alchemist=${verdict.verdict} differing_bytes=${verdict.differing_bytes} ` +
    `(baseline ${verdict.baseline_differing_bytes}) compiles=${verdict.compiles} tiers=${verdict.tiers.join(",") || "none"}`,
  );
  if (verdict.moves.length > 0) console.log(`moves:\n  ${verdict.moves.join("\n  ")}`);
  console.log(`next: ${verdict.next}`);
}

function selfTest(): void {
  const fixture = [
    "typedef signed int s32;",
    "",
    "void Func_02000100(void)",
    "{",
    "    s32 along;",
    "    s32 across;",
    "",
    "    along = *(s32 *)(0x2000000) + Func_02000200();",
    "    *(s32 *)(0x2000008) = along;",
    "    across = 4;",
    "    *(s32 *)(0x2000038) = *(s32 *)(0x2000008);",
    "    *(s32 *)(0x2000010) = across;",
    "    *(s32 *)(0x2000040) = across;",
    "}",
    "",
  ].join("\n");
  const parsed = parseFunction(fixture, "Func_02000100");
  if (parsed === null) throw new Error("failed to parse fixture function");
  if (renderFunction(parsed) !== fixture) throw new Error("render is not a faithful round-trip");
  const statements = parsed.body.filter((unit) => unit.kind === "statement");
  if (statements.length !== 6) throw new Error(`expected 6 statements, got ${statements.length}`);
  // The `along = ... Func_...()` statement contains a call and must be pinned.
  if (!statements[0].pinned) throw new Error("call statement was not pinned");
  if (statements[1].pinned) throw new Error("plain store was wrongly pinned");

  const moves = generateMoves(parsed);
  if (moves.length === 0) throw new Error("no moves generated");
  const split = moves.find((move) => move.description.includes("split") && move.description.includes("0x2000038") && move.description.includes("by 2"));
  if (split === undefined) throw new Error("expected a split-store move sinking the 0x2000038 store by 2");
  const applied = renderFunction(split.apply(parsed));
  if (!applied.includes("s32 permuted_")) throw new Error(`split move did not declare its temp:\n${applied}`);
  const tempAssign = applied.indexOf("= *(s32 *)(0x2000008);");
  const store = applied.indexOf("*(s32 *)(0x2000038) = permuted_");
  const acrossStore = applied.indexOf("*(s32 *)(0x2000040)");
  if (tempAssign === -1 || store === -1 || store < acrossStore) {
    throw new Error(`split move produced the wrong shape:\n${applied}`);
  }
  // A hoist over the pinned call statement must not be offered for another
  // pinned unit, but a plain store may hoist across it.
  if (moves.some((move) => move.description.includes("hoist") && move.description.includes("along = ") )) {
    throw new Error("pinned call statement should not hoist across other pinned units at distance crossing them");
  }

  // Regression: a chain of `} else if (SAME_COND) {` lines each closes the
  // previous sibling block and opens the next on a SINGLE source line. That
  // line becomes a one-line "block" unit whose first and last element are
  // the same array entry; renderScope() used to always emit
  // [open, ...scope, close], which for a one-line block silently duplicated
  // it. Confirmed on resource_371:28e8: byte-diff-neutral (the branches were
  // empty) so nothing else catches it -- only an exact round-trip check does.
  const cascadeFixture = [
    "void Func_02000200(void)",
    "{",
    "    if (x > 1) {",
    "    } else if (x > 2) {",
    "    } else if (x > 2) {",
    "    } else if (x > 2) {",
    "        y = 1;",
    "    }",
    "}",
    "",
  ].join("\n");
  const cascadeParsed = parseFunction(cascadeFixture, "Func_02000200");
  if (cascadeParsed === null) throw new Error("failed to parse cascading else-if fixture");
  const cascadeRendered = renderFunction(cascadeParsed);
  if (cascadeRendered !== cascadeFixture) {
    throw new Error(`cascading else-if chain did not round-trip faithfully:\n${cascadeRendered}`);
  }

  // Regression: a move must never cross a statement that reads the name it
  // assigns. Sinking `line = 0xe85;` here would put it below the call that
  // reads `line`, which then reads the variable before its only write --
  // undefined behavior that nonetheless byte-matched on three separate real
  // owners (resource_3c5:28a0, resource_375:be0, resource_375:1760), so the
  // compile gate cannot catch it and only this dependence rule can.
  const dataflowFixture = [
    "void Func_02000300(void)",
    "{",
    "    s32 line;",
    "    s32 other;",
    "",
    "    line = 0xe85;",
    "    Func_0808a170(line);",
    "    other = 1;",
    "    Func_0808a188(other);",
    "}",
    "",
  ].join("\n");
  const dataflowParsed = parseFunction(dataflowFixture, "Func_02000300");
  if (dataflowParsed === null) throw new Error("failed to parse dataflow fixture");
  const dataflowMoves = generateMoves(dataflowParsed);
  const crossesItsOwnRead = dataflowMoves.find((move) =>
    move.description.includes("sink") && move.description.includes("line = 0xe85"),
  );
  if (crossesItsOwnRead !== undefined) {
    throw new Error(`generated a move sinking an assignment past a read of it: ${crossesItsOwnRead.description}`);
  }
  // The rule must not over-refuse: an independent statement pair still moves.
  if (!dataflowMoves.some((move) => move.description.includes("other = 1"))) {
    throw new Error("dataflow rule wrongly blocked an independent statement's move");
  }

  // Regression: a split temp must be declared with the captured value's own
  // type. It was hardcoded `s32`, which narrows a pointer to an integer --
  // legal-looking only because this target's pointers are 32-bit (seen on
  // resource_3ae:e40, where `u8 *sub` was captured into an `s32` temp).
  const pointerSplitFixture = [
    "void Func_02000400(void)",
    "{",
    "    u8 *object;",
    "    u8 *sub;",
    "",
    "    object = Func_0808a080(10);",
    "    sub = *(u8 **)(object + 0x50);",
    "    Func_0808a170(1);",
    "    Func_0808a188(2);",
    "}",
    "",
  ].join("\n");
  const pointerParsed = parseFunction(pointerSplitFixture, "Func_02000400");
  if (pointerParsed === null) throw new Error("failed to parse pointer-split fixture");
  const pointerSplit = generateMoves(pointerParsed).find((move) => move.description.includes("split"));
  if (pointerSplit !== undefined) {
    const rendered = renderFunction(pointerSplit.apply(pointerParsed));
    if (/\bs32\s+permuted_/.test(rendered)) {
      throw new Error(`split captured a pointer into an s32 temp:\n${rendered}`);
    }
    if (!/\bu8\s*\*\s*permuted_/.test(rendered)) {
      throw new Error(`split temp did not take the pointer type of its destination:\n${rendered}`);
    }
  }

  // Regression: control-flow anchors are indivisible. A braceless `if (...)`
  // governs the next statement, a label is a `goto` target, and a jump
  // decides what is reachable -- moving any of them, or moving a statement
  // across one, silently rewrites control flow while still reducing the
  // differing-byte count (resource_3ba:31c0, resource_3bd:bc8,
  // resource_3b3:274c all produced such a variant before this rule).
  const anchorFixture = [
    "void Func_02000500(void)",
    "{",
    "    s32 a;",
    "    s32 b;",
    "",
    "    if (a != 0)",
    "        b = 1;",
    "    a = 2;",
    "    if (b != 9) return;",
    "    b = 3;",
    "}",
    "",
  ].join("\n");
  const anchorParsed = parseFunction(anchorFixture, "Func_02000500");
  if (anchorParsed === null) throw new Error("failed to parse control-anchor fixture");
  for (const move of generateMoves(anchorParsed)) {
    if (/"\s*(?:if|else|for|while)\b/.test(move.description)) {
      throw new Error(`generated a move of a control-flow header: ${move.description}`);
    }
    if (/\b(?:return|goto|break|continue)\b/.test(move.description)) {
      throw new Error(`generated a move of a jump statement: ${move.description}`);
    }
    if (/"\s*b = 1;/.test(move.description)) {
      throw new Error(`moved the statement a braceless header governs: ${move.description}`);
    }
  }
  console.log("self-test=ok tool=alchemist");
}

export async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const options = optionsOf(Bun.argv.slice(2));
  mkdirSync(options.work, { recursive: true });

  const span = options.span ?? resolveSpan(options.overlay, options.offset);
  if (span === undefined) throw new Error(`no inventory or regions.json row for ${options.id}; pass --span BYTES`);
  if (!existsSync(options.source)) throw new Error(`candidate does not exist: ${options.source}`);

  const overlayPath = join(ROOT, "assets/code", `${options.overlay}_overlay.s`);
  const image = assembleOverlay(overlayPath, OVERLAY_BASE);
  const expected = image.subarray(options.offset, options.offset + span);
  const adoptCommand = `bun tools/overlay_adopt.ts ${options.id} --source ${options.source} --span ${span}`;

  const gate = gateDiagnosis(options.source, options.work, options.overlay, options.routingSource, expected);
  const tiers = [...gate.tiersSeen].sort();
  if (gate.differingBytes === 0) {
    emitVerdict({
      id: options.id, verdict: "unnecessary",
      baseline_differing_bytes: 0, differing_bytes: 0, tiers, compiles: 1, moves: [],
      source: options.source, saved: null,
      next: `already byte-exact: ${adoptCommand} --apply, then delete the semantic source`,
    }, options.json);
    return;
  }
  if (!options.json) console.log(`baseline differing_bytes=${gate.differingBytes} differing_row_tiers=${tiers.join(",") || "none"}`);
  if (!gate.licensed && !options.force) {
    emitVerdict({
      id: options.id, verdict: "refused",
      baseline_differing_bytes: gate.differingBytes, differing_bytes: gate.differingBytes,
      tiers, compiles: 1, moves: [], source: options.source, saved: null,
      next: `note STILL-OPEN in the source header citing tiers [${tiers.join(",") || "none"}]; ` +
        "statement order provably cannot fix this residual -- do not hand-permute or flag-sweep it",
    }, options.json);
    return;
  }
  if (!options.json) {
    console.log(gate.licensed
      ? "diagnosis licenses statement-order moves (original-order tier decided a differing row)"
      : "searching under --force despite an unlicensed diagnosis");
  }

  const functionName = `Func_${(OVERLAY_BASE + options.offset).toString(16).padStart(8, "0")}`;
  const originalText = readFileSync(options.source, "utf8");
  let current = parseFunction(originalText, functionName);
  if (current === null) throw new Error(`cannot find ${functionName} in ${options.source}`);

  let best = { parsed: current, score: gate.differingBytes, trail: [] as string[] };
  let compiles = 0;
  // The overlay compile path derives the entry address from the last 8 hex
  // digits of the filename, so the variant file must carry the same stem.
  const variantPath = join(options.work, `variant_${(OVERLAY_BASE + options.offset).toString(16).padStart(8, "0")}.c`);

  for (let round = 1; best.score > 0 && compiles < options.budget; round++) {
    const moves = generateMoves(best.parsed);
    let roundBest: { parsed: ParsedFunction; score: number; description: string } | null = null;
    for (const move of moves) {
      if (compiles >= options.budget) break;
      const candidate = move.apply(best.parsed);
      writeFileSync(variantPath, renderFunction(candidate));
      compiles++;
      let score = Number.POSITIVE_INFINITY;
      try {
        const compiled = compileOverlayCandidate(variantPath, options.work, options.overlay, options.routingSource, []);
        score = differingByteCount(compiled.data, expected);
      } catch {
        // A variant that fails to compile scores as unusable, nothing more.
      }
      if (roundBest === null || score < roundBest.score) {
        roundBest = { parsed: candidate, score, description: move.description };
      }
      if (score === 0) break;
    }
    if (roundBest === null || roundBest.score >= best.score) {
      if (!options.json) console.log(`round ${round}: no improving move (${compiles} compiles used)`);
      break;
    }
    best = { parsed: roundBest.parsed, score: roundBest.score, trail: [...best.trail, roundBest.description] };
    if (!options.json) console.log(`round ${round}: ${roundBest.description} -> differing_bytes=${roundBest.score}`);
  }

  if (best.score === 0) {
    const finalText = renderFunction(best.parsed);
    let saved: string;
    if (options.write) {
      writeFileSync(options.source, finalText);
      saved = options.source;
    } else {
      saved = join(options.work, "permute-exact.c");
      writeFileSync(saved, finalText);
    }
    emitVerdict({
      id: options.id, verdict: "exact",
      baseline_differing_bytes: gate.differingBytes, differing_bytes: 0,
      tiers, compiles: compiles + 1, moves: best.trail, source: options.source, saved,
      next: options.write
        ? `rename permuted_N temps to meaningful names (cannot change bytes), then ${adoptCommand} --apply and delete the semantic source`
        : `re-run with --write to save, or copy ${saved} over the source; then ${adoptCommand} --apply`,
    }, options.json);
  } else if (best.trail.length > 0) {
    const saved = join(options.work, "permute-best.c");
    writeFileSync(saved, renderFunction(best.parsed));
    emitVerdict({
      id: options.id, verdict: "improved",
      baseline_differing_bytes: gate.differingBytes, differing_bytes: best.score,
      tiers, compiles: compiles + 1, moves: best.trail, source: options.source, saved,
      next: "do NOT install: an inexact improvement is not proof of the reference's true shape. " +
        "A capable agent may inspect the saved variant and the moves for a lead; otherwise note STILL-OPEN",
    }, options.json);
  } else {
    emitVerdict({
      id: options.id, verdict: "exhausted",
      baseline_differing_bytes: gate.differingBytes, differing_bytes: gate.differingBytes,
      tiers, compiles: compiles + 1, moves: [], source: options.source, saved: null,
      next: `note STILL-OPEN in the source header citing tiers [${tiers.join(",") || "none"}] and that ` +
        "licensed statement moves are exhausted; do not hand-permute",
    }, options.json);
  }
}

if (import.meta.main) await main();
