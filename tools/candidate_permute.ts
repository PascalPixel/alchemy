#!/usr/bin/env bun
// A diagnosis-driven source permuter: reads candidate_explain.ts's scheduler
// tier diagnosis and applies ONLY the source transformations that diagnosis
// licenses, instead of permuting blindly.
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
  return units.flatMap((unit) =>
    unit.kind === "block" && unit.scope !== null
      ? [unit.lines[0], ...renderScope(unit.scope), unit.lines[unit.lines.length - 1]]
      : unit.lines,
  );
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
          if (scope.slice(index + 1, index + distance + 1).some((other) => other.kind === "declaration")) continue;
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
                kind: "declaration", lines: [`${declIndent}s32 ${temp};`], pinned: true, scope: null,
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
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--source") source = argv[++index] ?? "";
    else if (argument === "--routing-source") routingSource = argv[++index] ?? "";
    else if (argument === "--work") work = argv[++index] ?? "";
    else if (argument === "--span") span = Number(argv[++index]);
    else if (argument === "--budget") budget = Number(argv[++index]);
    else if (argument === "--write") write = true;
    else if (argument === "--force") force = true;
    else if (argument === "-h" || argument === "--help") {
      console.log(
        "usage: candidate_permute.ts OVERLAY:OFFSET --source FILE [--span BYTES] [--budget N] [--write] [--force]\n" +
        "Reads the scheduler tier diagnosis and applies only the licensed source moves\n" +
        "(statement sink/hoist, split-store).  --write saves the source ONLY when a\n" +
        "variant is byte-exact.  --force runs the search even when no differing row\n" +
        "was decided by the original-order tier (normally a proven dead end).",
      );
      process.exit(0);
    } else if (id === "") id = argument;
    else throw new Error(`unrecognised argument: ${argument}`);
  }
  const match = /^(resource_[0-9a-f]+):(?:0x)?([0-9a-f]+)$/i.exec(id);
  if (match === null) throw new Error("usage: candidate_permute.ts OVERLAY:OFFSET --source FILE [--span BYTES]");
  if (source === "") throw new Error("--source FILE is required");
  const overlay = match[1];
  const offset = Number.parseInt(match[2], 16);
  if (routingSource === "") {
    const stem = (OVERLAY_BASE + offset).toString(16).padStart(8, "0");
    routingSource = join(ROOT, "assets/code", `${overlay}_c_${stem}.c`);
  }
  return {
    id, overlay, offset,
    source: resolve(source), routingSource: resolve(routingSource),
    work: resolve(work || join(ROOT, "work/candidate-permute")), span, budget, write, force,
  };
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
  console.log("self-test=ok tool=candidate-permute");
}

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const options = optionsOf(Bun.argv.slice(2));
  mkdirSync(options.work, { recursive: true });

  const inventoryPath = join(ROOT, "out/decomp/overlays.json");
  let span = options.span;
  if (span === undefined && existsSync(inventoryPath)) {
    const inventory = JSON.parse(readFileSync(inventoryPath, "utf8")) as { functions: Array<{ id: string; span_bytes: number }> };
    span = inventory.functions.find((item) => item.id === options.id)?.span_bytes;
  }
  if (span === undefined) throw new Error(`no inventory row for ${options.id}; pass --span BYTES`);
  if (!existsSync(options.source)) throw new Error(`candidate does not exist: ${options.source}`);

  const overlayPath = join(ROOT, "assets/code", `${options.overlay}_overlay.s`);
  const image = assembleOverlay(overlayPath, OVERLAY_BASE);
  const expected = image.subarray(options.offset, options.offset + span);

  const gate = gateDiagnosis(options.source, options.work, options.overlay, options.routingSource, expected);
  if (gate.differingBytes === 0) {
    console.log("permute=unnecessary differing_bytes=0 (candidate is already byte-exact -- run overlay_adopt)");
    return;
  }
  const tiers = [...gate.tiersSeen].sort().join(",") || "none";
  console.log(`baseline differing_bytes=${gate.differingBytes} differing_row_tiers=${tiers}`);
  if (!gate.licensed && !options.force) {
    console.log(
      "permute=refused (no differing row was decided by the original-order tier, so statement\n" +
      "order provably cannot fix this residual -- see the tier list above for what did decide\n" +
      "it: priority/class/depend-count differences need a different lever, and model-divergence\n" +
      "marks an unmodeled tier.  Pass --force to search anyway.)",
    );
    return;
  }
  if (gate.licensed) {
    console.log("diagnosis licenses statement-order moves (original-order tier decided a differing row)");
  } else {
    console.log("searching under --force despite an unlicensed diagnosis");
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
      console.log(`round ${round}: no improving move (${compiles} compiles used)`);
      break;
    }
    best = { parsed: roundBest.parsed, score: roundBest.score, trail: [...best.trail, roundBest.description] };
    console.log(`round ${round}: ${roundBest.description} -> differing_bytes=${roundBest.score}`);
  }

  if (best.score === 0) {
    const finalText = renderFunction(best.parsed);
    if (options.write) {
      writeFileSync(options.source, finalText);
      console.log(`permute=exact compiles=${compiles} written=${options.source}`);
    } else {
      const outPath = join(options.work, "permute-exact.c");
      writeFileSync(outPath, finalText);
      console.log(`permute=exact compiles=${compiles} saved=${outPath} (re-run with --write to update the source)`);
    }
    console.log(`moves applied:\n  ${best.trail.join("\n  ")}`);
    console.log("note: rename any permuted_N temps to meaningful names -- renaming locals cannot change bytes");
  } else if (best.trail.length > 0) {
    console.log(`permute=improved differing_bytes=${best.score} (from ${gate.differingBytes}) compiles=${compiles}`);
    console.log(`moves (NOT written -- an inexact improvement is not proof of the reference's true shape):\n  ${best.trail.join("\n  ")}`);
  } else {
    console.log(`permute=exhausted differing_bytes=${gate.differingBytes} compiles=${compiles} (no licensed move improved the candidate)`);
  }
}

if (import.meta.main) await main();
