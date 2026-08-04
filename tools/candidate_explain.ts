#!/usr/bin/env bun
// Compile a candidate with gcc-2.96's own -fsched-verbose=9 scheduler trace
// enabled, correlate the final RTL insn order with the actual disassembled
// instructions by CONTENT (instruction class + destination register, via
// rtl_align.ts), and print the compiler's own ready-list trace beside each
// differing row -- so a stuck "why did the scheduler put this instruction
// here" question is answered by reading the compiler's own decision log
// instead of by reading haifa-sched.c source and guessing flags.
//
// This does not explain every residual: only RTL-level pass decisions are
// dumpable this way (scheduling, combine, loop, gcse, register allocation).
// Front-end tree transforms (constant folding, truncation pushdown, operand
// canonicalization) happen before any RTL exists and are invisible here.
import { existsSync, mkdirSync, readdirSync, readFileSync } from "node:fs";
import { basename, dirname, join, resolve } from "node:path";
import { assembleOverlay, compileOverlayCandidate, OVERLAY_BASE } from "./overlay_disasm.ts";
import { verifyCandidate, ROM_BASE } from "./match_m2c.ts";
import { type RtlInsn, calleeSymbol, parseInsns } from "./rtl_insn.ts";
import { type Instruction, parseInstruction } from "./thumb_disasm.ts";
import { align } from "./rtl_align.ts";
import { type DependenceRow, type PairComparison, type ScheduleContext, diagnose, parseDependenceTable } from "./rtl_schedule.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

// gcc-2.96's per-pass RTL dumps: -dR is the second (post-reload) Thumb
// scheduling pass, the one that actually decides final instruction order at
// -O2. -dS is the pre-reload pass (usually inert for this target, harmless
// to also request). -fsched-verbose=9 is the highest trace level: it prints
// the ready list and every "--> scheduling insn <<<N>>>" decision.
const EXPLAIN_FLAGS = ["-dS", "-dR", "-fsched-verbose=9"] as const;

// gcc names dump files from its -dumpbase argument (the routing source's own
// basename, see alchemy_gcc.ts's sourceToAssemblyPlan), not from the actual
// preprocessed input filename -- so the prefix cannot be predicted from the
// candidate's own stem, but IS predictable from the routing source's own
// stem, which every caller here already knows. Work directories are reused
// across repeated invocations (not cleaned between runs), so matching on
// suffix alone picks up stale dump files left behind by a PREVIOUS,
// unrelated candidate compiled into the same directory -- silently
// producing a plausible-looking but entirely wrong scheduler trace. Matching
// on the routing stem prefix as well avoids that.
function findDumpFile(work: string, routingStem: string, suffix: string): string | null {
  const match = readdirSync(work).find((name) => name.startsWith(`${routingStem}.`) && name.endsWith(`.${suffix}`));
  return match === undefined ? null : join(work, match);
}

export function disassembleInstructions(path: string, base = 0): Instruction[] {
  const result = Bun.spawnSync([
    "arm-none-eabi-objdump", "-D", "-b", "binary", "-m", "arm", "-M", "force-thumb",
    ...(base ? [`--adjust-vma=0x${base.toString(16)}`] : []), path,
  ], { stdout: "pipe", stderr: "pipe" });
  if (result.exitCode !== 0) throw new Error(result.stderr.toString().trim());
  const rows: Instruction[] = [];
  for (const line of result.stdout.toString().split("\n")) {
    const matched = /^\s+([0-9a-f]+):\t[0-9a-f ]+\t(.*)$/.exec(line);
    if (matched === null) continue;
    rows.push(parseInstruction(Number.parseInt(matched[1], 16) - base, matched[2].trimEnd()));
  }
  return rows;
}

function differingHalfwords(actual: Uint8Array, expected: Uint8Array): Set<number> {
  const found = new Set<number>();
  const shared = Math.min(actual.length, expected.length);
  for (let offset = 0; offset + 2 <= shared; offset += 2) {
    if (actual[offset] !== expected[offset] || actual[offset + 1] !== expected[offset + 1]) found.add(offset);
  }
  for (let offset = shared & ~1; offset < Math.max(actual.length, expected.length); offset += 2) found.add(offset);
  return found;
}

// The scheduling trace at -fsched-verbose=9 prints "Ready list (t = N):
// UID UID ..." immediately before each "--> scheduling insn <<<UID>>>"
// pick, both as `;;`-prefixed lines in file order. Finding the decision for
// a given UID means finding its own "scheduling insn" line, then walking
// backward for the nearest preceding "Ready list" line (its own ready set)
// and, separately, the nearest preceding *other* "scheduling insn" line
// (the last-scheduled insn rank_for_schedule's class tier compares against
// -- null if this is the first decision in the dump).
interface Decision { readyUids: number[]; lastScheduledUid: number | null }

function decisionForInsn(dumpText: string, uid: number): Decision | null {
  const lines = dumpText.split("\n");
  const pickedMarker = new RegExp(`--> scheduling insn <<<(\\d+)>>>`);
  const ownIndex = lines.findIndex((line) => line.includes(`--> scheduling insn <<<${uid}>>>`));
  if (ownIndex === -1) return null;

  let readyUids: number[] = [];
  for (let back = ownIndex - 1; back >= 0; back--) {
    const match = /Ready list \(t = *\d+\): *(.*)$/.exec(lines[back]);
    if (match !== null) {
      readyUids = match[1].trim().split(/\s+/).filter(Boolean).map(Number);
      break;
    }
    // A "scheduling insn" line encountered before any "Ready list" line
    // means we've walked past this decision's own ready-list snapshot
    // (queue/dequeue trace lines between them don't reset the search).
    if (pickedMarker.test(lines[back])) break;
  }

  let lastScheduledUid: number | null = null;
  for (let back = ownIndex - 1; back >= 0; back--) {
    const match = pickedMarker.exec(lines[back]);
    if (match !== null) { lastScheduledUid = Number(match[1]); break; }
  }
  return { readyUids, lastScheduledUid };
}

function describeInsn(insn: RtlInsn): string {
  const callee = calleeSymbol(insn);
  if (callee !== null) return `call ${callee}`;
  return `insn ${insn.uid}`;
}

// `pickedUid` is the insn the real trace shows scheduled -- the row this
// pair is being printed under. comparePair is a pure tier-by-tier
// comparator, so when our model is accurate it agrees pickedUid won; when
// it doesn't, that mismatch IS the finding (an unmodeled tier -- register
// pressure, the cost==1 shortcut, or the project's own dest-order hook --
// was decisive), and must be shown as a divergence, not silently reported
// as if pickedUid had won anyway.
function formatPair(pickedUid: number, pair: PairComparison): string {
  if (pair.winner === pickedUid) return `beats ${pair.loser} (${pair.tier}: ${pair.detail})`;
  return `?? model expects ${pair.winner} to beat ${pickedUid} here (${pair.tier}: ${pair.detail}) -- ` +
    `actual pick suggests an unmodeled tier (register pressure / cost==1 / dest-order hook)`;
}

// Combines the raw decision (this insn's ready list + the last-scheduled
// insn at that point) with rank_for_schedule's own tier-by-tier comparator
// to explain, for each other insn that was ready at the same cycle, exactly
// which tier picked this insn over it. Falls back to a bare description
// when there's no scheduling decision to read (e.g. the insn was the only
// one ready, or it predates any "Ready list" line in the dump).
function traceForInsn(dumpText: string, insn: RtlInsn, context: ScheduleContext): string {
  const description = describeInsn(insn);
  const decision = decisionForInsn(dumpText, insn.uid);
  if (decision === null) return description;
  const diagnosis = diagnose(decision.readyUids, insn.uid, { ...context, lastScheduledUid: decision.lastScheduledUid });
  if (diagnosis.perRival.length === 0) return description;
  return `${description} -- ${diagnosis.perRival.map((pair) => formatPair(insn.uid, pair)).join("; ")}`;
}

function report(actual: Buffer, expected: Buffer, work: string, actualPath: string, routingStem: string): void {
  const instructions = disassembleInstructions(actualPath);
  const differing = differingHalfwords(actual, expected);

  const dumpPath = findDumpFile(work, routingStem, "sched2") ?? findDumpFile(work, routingStem, "sched");
  const dumpText = dumpPath === null ? "" : readFileSync(dumpPath, "utf8");
  const insns = dumpPath === null ? [] : parseInsns(dumpText);
  const alignment = align(insns, instructions);
  const byOffset = new Map(alignment.pairs.map((pair) => [pair.instruction.offset, pair.insn]));
  const table = dumpPath === null ? new Map<number, DependenceRow>() : parseDependenceTable(dumpText);
  const insnsByUid = new Map(insns.map((insn) => [insn.uid, insn]));

  console.log(`candidate=${actual.length} reference=${expected.length} differing_halfwords=${differing.size}`);
  if (dumpPath === null) {
    console.log("(no scheduler dump found -- family/route may not support -dR, or nothing was scheduled)");
  } else {
    console.log(
      `(scheduler trace: ${insns.length} RTL insns, ${instructions.length} real instructions, ` +
      `${alignment.pairs.length} aligned by class+destination-register; ` +
      `${alignment.unmatchedInstructions.length} real instruction(s) had no RTL match -- ` +
      `typically prologue/epilogue multi-insn expansion, shown without a trace column below)`,
    );
  }

  console.log("      offset  candidate                      insn  scheduler trace");
  for (const instruction of instructions) {
    const offset = instruction.offset;
    const mark = differing.has(offset) ? "!" : " ";
    const candidateColumn = instruction.raw.replace(/\t/g, " ").padEnd(30).slice(0, 30);
    const insn = byOffset.get(offset);
    const uidLabel = insn === undefined ? "" : String(insn.uid).padStart(4);
    const trace = insn === undefined || dumpPath === null ? "" : traceForInsn(dumpText, insn, { table, lastScheduledUid: null, insnsByUid });
    console.log(`  ${mark} ${offset.toString(16).padStart(4, "0")}  ${candidateColumn} ${uidLabel}  ${trace}`);
  }
  if (dumpPath !== null) console.log(`\nfull dump: ${dumpPath}`);
}

interface OverlayArgs {
  mode: "overlay";
  id: string;
  overlay: string;
  offset: number;
  source: string;
  routingSource: string;
  work: string;
  span?: number;
}

interface MainImageArgs {
  mode: "main";
  source: string;
  work: string;
}

function optionsOf(argv: string[]): OverlayArgs | MainImageArgs {
  let id = "";
  let source = "";
  let routingSource = "";
  let work = "";
  let span: number | undefined;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--source") source = argv[++index] ?? "";
    else if (argument === "--routing-source") routingSource = argv[++index] ?? "";
    else if (argument === "--work") work = argv[++index] ?? "";
    else if (argument === "--span") span = Number(argv[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log(
        "usage: candidate_explain.ts OVERLAY:OFFSET --source FILE [--routing-source FILE] [--span BYTES] [--work DIR]\n" +
        "       candidate_explain.ts semantic/main/08xxxxxx.c [--work DIR]",
      );
      process.exit(0);
    } else if (id === "") id = argument;
    else throw new Error(`unrecognised argument: ${argument}`);
  }
  const overlayMatch = /^(resource_[0-9a-f]+):(?:0x)?([0-9a-f]+)$/i.exec(id);
  if (overlayMatch !== null) {
    if (source === "") throw new Error("overlay mode requires --source FILE");
    const overlay = overlayMatch[1];
    const offset = Number.parseInt(overlayMatch[2], 16);
    if (routingSource === "") {
      const stem = (OVERLAY_BASE + offset).toString(16).padStart(8, "0");
      routingSource = join(ROOT, "exact", `${overlay}_c_${stem}.c`);
    }
    return {
      mode: "overlay", id, overlay, offset,
      source: resolve(source), routingSource: resolve(routingSource),
      work: resolve(work || join(ROOT, "work/candidate-explain-overlay")), span,
    };
  }
  if (id === "") throw new Error("usage: candidate_explain.ts OVERLAY:OFFSET --source FILE | candidate_explain.ts FILE.c");
  return { mode: "main", source: resolve(id), work: resolve(work || join(ROOT, "work/candidate-explain-main")) };
}

function selfTest(): void {
  const instructions = [
    parseInstruction(0, "adds\tr3, r3, r2"),
    parseInstruction(2, "pop\t{r5}"),
    parseInstruction(4, "bl\t0x11f4"),
  ];
  const insns = parseInsns(
    "(insn 15 10 17 (set (reg:SI 3 r3)\n" +
    "        (plus:SI (reg/v:SI 3 r3)\n" +
    "            (reg/v:SI 2 r2))) 5 {*thumb_addsi3} (nil))\n" +
    '(call_insn 19 17 21 (parallel [\n' +
    "            (set (reg:SI 0 r0)\n" +
    '                (call (mem:SI (symbol_ref:SI ("Func_02001508")) 0)\n' +
    "                    (const_int 0 [0x0])))\n" +
    "        ] ) 245 {*call_value_insn} (nil))\n",
  );
  if (insns.length !== 2) throw new Error(`expected 2 insns from fixture, got ${insns.length}`);
  const alignment = align(insns, instructions);
  if (alignment.pairs.length !== 2) throw new Error(`expected 2 aligned pairs, got ${alignment.pairs.length}`);
  if (alignment.pairs[0].instruction.offset !== 0 || alignment.pairs[0].insn.uid !== 15) {
    throw new Error(`wrong first pair: ${JSON.stringify(alignment.pairs[0])}`);
  }
  // The unmatched `pop` (offset 2) must not have derailed the alignment: the
  // call at offset 4 must still find its call_insn.
  if (alignment.pairs[1].instruction.offset !== 4 || alignment.pairs[1].insn.uid !== 19) {
    throw new Error(`gap tolerance failed: ${JSON.stringify(alignment.pairs[1])}`);
  }
  if (alignment.unmatchedInstructions.length !== 1 || alignment.unmatchedInstructions[0].offset !== 2) {
    throw new Error(`expected the pop to be reported unmatched, got ${JSON.stringify(alignment.unmatchedInstructions)}`);
  }
  console.log("self-test=ok tool=candidate-explain");
}

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const options = optionsOf(Bun.argv.slice(2));
  mkdirSync(options.work, { recursive: true });

  if (options.mode === "overlay") {
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
    const expected = Buffer.from(image.subarray(options.offset, options.offset + span));
    const compiled = compileOverlayCandidate(
      options.source, options.work, options.overlay, options.routingSource, [...EXPLAIN_FLAGS],
    );
    if (compiled.address !== OVERLAY_BASE + options.offset) {
      throw new Error(`candidate entry 0x${compiled.address.toString(16)} does not match ${options.id}`);
    }
    const actualPath = join(options.work, "candidate.bin");
    await Bun.write(actualPath, compiled.data);
    report(compiled.data, expected, options.work, actualPath, basename(options.routingSource, ".c"));
  } else {
    const rom = readFileSync(join(ROOT, "roms/gs1-en.gba"));
    const verification = await verifyCandidate(
      options.source, rom, options.work, [...EXPLAIN_FLAGS], ROM_BASE, "gs1", { family: "routed", removeFlags: [] },
    );
    const stem = basename(options.source, ".c");
    const actualPath = join(options.work, `${stem}.bin`);
    report(Buffer.from(verification.actual), Buffer.from(verification.expected), options.work, actualPath, stem);
  }
}

if (import.meta.main) await main();
