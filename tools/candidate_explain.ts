#!/usr/bin/env bun
// Compile a candidate with gcc-2.96's own -fsched-verbose=9 scheduler trace
// enabled, correlate the final RTL insn order with the actual disassembled
// instructions by output position, and print the compiler's own ready-list
// trace beside each differing row -- so a stuck "why did the scheduler put
// this instruction here" question is answered by reading the compiler's own
// decision log instead of by reading haifa-sched.c source and guessing flags.
//
// This does not explain every residual: only RTL-level pass decisions are
// dumpable this way (scheduling, combine, loop, gcse, register allocation).
// Front-end tree transforms (constant folding, truncation pushdown, operand
// canonicalization) happen before any RTL exists and are invisible here.
import { existsSync, mkdirSync, readdirSync, readFileSync } from "node:fs";
import { basename, dirname, join, resolve } from "node:path";
import { assembleOverlay, compileOverlayCandidate, OVERLAY_BASE } from "./overlay_disasm.ts";
import { verifyCandidate, ROM_BASE } from "./match_m2c.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

// gcc-2.96's per-pass RTL dumps: -dR is the second (post-reload) Thumb
// scheduling pass, the one that actually decides final instruction order at
// -O2. -dS is the pre-reload pass (usually inert for this target, harmless
// to also request). -fsched-verbose=9 is the highest trace level: it prints
// the ready list and every "--> scheduling insn <<<N>>>" decision.
const EXPLAIN_FLAGS = ["-dS", "-dR", "-fsched-verbose=9"] as const;

interface InsnNode {
  uid: number;
  kind: "insn" | "call_insn" | "jump_insn";
  summary: string;
}

// Every top-level `(insn N ...)` / `(call_insn N ...)` / `(jump_insn N ...)`
// line in a gcc RTL dump starts at column 0 with that exact literal token
// followed by its UID; nested dependency references (`insn_list N ...`)
// never start a line this way. A -dR dump file contains exactly one such
// listing, in final scheduled order, after the scheduling trace text -- this
// is not documented anywhere, it was confirmed by inspection of a real dump.
export function parseFinalInsnOrder(dumpText: string): InsnNode[] {
  const nodes: InsnNode[] = [];
  const lines = dumpText.split("\n");
  for (let index = 0; index < lines.length; index++) {
    const header = /^\((insn|call_insn|jump_insn) (\d+) /.exec(lines[index]);
    if (header === null) continue;
    const kind = header[1] as InsnNode["kind"];
    const uid = Number.parseInt(header[2], 10);
    // The pattern summary is whatever follows the UID/prev/next triple on
    // this line plus continuation lines up to the closing paren of the SET,
    // truncated hard -- this is a hint for a human reader, not a full parse.
    const rest = lines.slice(index, index + 3).join(" ").replace(/\s+/g, " ").trim();
    const summary = rest.length > 90 ? `${rest.slice(0, 90)}...` : rest;
    nodes.push({ uid, kind, summary });
  }
  return nodes;
}

// The scheduling trace at -fsched-verbose=9 prints "Ready list (t = N): ..."
// immediately before each "--> scheduling insn <<<UID>>>" pick. Pulling the
// pair for a given UID shows exactly which other ready insns it was chosen
// ahead of at that cycle -- the raw fact haifa-sched.c's rank_for_schedule
// decided on, without re-deriving the decision from source.
export function traceForInsn(dumpText: string, uid: number): string | null {
  const lines = dumpText.split("\n");
  const marker = `--> scheduling insn <<<${uid}>>>`;
  for (let index = 0; index < lines.length; index++) {
    if (!lines[index].includes(marker)) continue;
    let readyLine = "";
    for (let back = index - 1; back >= 0 && index - back <= 6; back--) {
      const match = /Ready list \(t = *\d+\): *(.*)$/.exec(lines[back]);
      if (match !== null) { readyLine = match[1].trim(); break; }
    }
    const picked = lines[index].trim();
    return readyLine.length > 0
      ? `ready list: [${readyLine}] -- ${picked}`
      : picked;
  }
  return null;
}

// gcc names dump files from its -dumpbase argument (the routing source's own
// basename, see alchemy_gcc.ts's sourceToAssemblyPlan), not from the actual
// preprocessed input filename -- so the prefix cannot be predicted from the
// stem alone. Each work directory holds one candidate's build at a time, so
// matching on suffix within it is unambiguous.
function findDumpFile(work: string, _stem: string, suffix: string): string | null {
  const match = readdirSync(work).find((name) => name.endsWith(`.${suffix}`));
  return match === undefined ? null : join(work, match);
}

function disassemble(path: string, base = 0): Map<number, string> {
  const result = Bun.spawnSync([
    "arm-none-eabi-objdump", "-D", "-b", "binary", "-m", "arm", "-M", "force-thumb",
    ...(base ? [`--adjust-vma=0x${base.toString(16)}`] : []), path,
  ], { stdout: "pipe", stderr: "pipe" });
  if (result.exitCode !== 0) throw new Error(result.stderr.toString().trim());
  const rows = new Map<number, string>();
  for (const line of result.stdout.toString().split("\n")) {
    const matched = /^\s+([0-9a-f]+):\t[0-9a-f ]+\t(.*)$/.exec(line);
    if (matched !== null) rows.set(Number.parseInt(matched[1], 16) - base, matched[2].trimEnd());
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

function report(
  actual: Buffer, expected: Buffer, work: string, stem: string, actualPath: string,
): void {
  const left = disassemble(actualPath);
  const differing = differingHalfwords(actual, expected);
  const offsets = [...left.keys()].sort((a, b) => a - b);

  const dumpPath = findDumpFile(work, stem, "sched2") ?? findDumpFile(work, stem, "sched");
  const insnOrder = dumpPath === null ? [] : parseFinalInsnOrder(readFileSync(dumpPath, "utf8"))
    .filter((node) => node.kind !== undefined);
  // Real instructions only: notes/barriers/uses emit no bytes and must not
  // consume a disassembly-row slot when zipping by position.
  const realInsns = insnOrder.filter((node) => ["insn", "call_insn", "jump_insn"].includes(node.kind));

  console.log(`candidate=${actual.length} reference=${expected.length} differing_halfwords=${differing.size}`);
  if (dumpPath === null) {
    console.log("(no scheduler dump found -- family/route may not support -dR, or nothing was scheduled)");
  } else if (realInsns.length !== offsets.filter((o) => left.has(o)).length) {
    console.log(
      `(scheduler trace present but insn count (${realInsns.length}) does not match ` +
      `instruction row count (${offsets.length}) -- correlation by position is unreliable here, ` +
      `showing raw disassembly only)`,
    );
  }
  const reliable = dumpPath !== null && realInsns.length === offsets.filter((o) => left.has(o)).length;

  console.log("      offset  candidate                      insn  scheduler trace");
  let insnIndex = 0;
  for (const offset of offsets) {
    const mark = differing.has(offset) ? "!" : " ";
    const candidate = (left.get(offset) ?? "").padEnd(30).slice(0, 30);
    const node = reliable ? realInsns[insnIndex++] : undefined;
    const uidLabel = node === undefined ? "" : String(node.uid).padStart(4);
    const trace = node === undefined || dumpPath === null ? "" : (traceForInsn(readFileSync(dumpPath, "utf8"), node.uid) ?? "");
    console.log(`  ${mark} ${offset.toString(16).padStart(4, "0")}  ${candidate} ${uidLabel}  ${trace}`);
  }
  if (dumpPath !== null) {
    console.log(`\nfull dump: ${dumpPath}`);
  }
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
      routingSource = join(ROOT, "assets/code", `${overlay}_c_${stem}.c`);
    }
    return {
      mode: "overlay", id, overlay, offset,
      source: resolve(source), routingSource: resolve(routingSource),
      work: resolve(work || join(ROOT, "work/candidate-explain-overlay")), span,
    };
  }
  // Main-image mode: the positional argument IS the candidate source file.
  if (id === "") throw new Error("usage: candidate_explain.ts OVERLAY:OFFSET --source FILE | candidate_explain.ts FILE.c");
  return { mode: "main", source: resolve(id), work: resolve(work || join(ROOT, "work/candidate-explain-main")) };
}

function selfTest(): void {
  const same = "(insn 15 10 17 (set (reg:SI 3) (const_int 3)) 174 {*x} (nil))\n" +
    "(note 17 15 19 NOTE_INSN_DELETED 0)\n" +
    "(call_insn 19 17 21 (parallel [(set (reg:SI 0) (call ...))]) 245 {*y} (nil))\n";
  const parsed = parseFinalInsnOrder(same);
  if (parsed.length !== 2) throw new Error(`expected 2 real-instruction nodes (note excluded), got ${parsed.length}`);
  if (parsed[0].uid !== 15 || parsed[0].kind !== "insn") throw new Error("wrong first node");
  if (parsed[1].uid !== 19 || parsed[1].kind !== "call_insn") throw new Error("wrong second node");

  const trace = ";; Ready list (t =  4):    19  28\n;;\t\t--> scheduling insn <<<28>>> on unit core\n";
  const found = traceForInsn(trace, 28);
  if (found === null || !found.includes("19") || !found.includes("28")) {
    throw new Error(`trace lookup failed: ${found}`);
  }
  if (traceForInsn(trace, 999) !== null) throw new Error("expected no trace for unknown uid");
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
    const stem = (OVERLAY_BASE + options.offset).toString(16).padStart(8, "0");
    const actualPath = join(options.work, "candidate.bin");
    await Bun.write(actualPath, compiled.data);
    report(compiled.data, expected, options.work, stem, actualPath);
  } else {
    const rom = readFileSync(join(ROOT, "roms/gs1-en.gba"));
    const verification = await verifyCandidate(
      options.source, rom, options.work, [...EXPLAIN_FLAGS], ROM_BASE, "gs1", { family: "routed", removeFlags: [] },
    );
    const stem = basename(options.source, ".c");
    const actualPath = join(options.work, `${stem}.bin`);
    report(
      Buffer.from(verification.actual), Buffer.from(verification.expected),
      options.work, stem, actualPath,
    );
  }
}

if (import.meta.main) await main();
