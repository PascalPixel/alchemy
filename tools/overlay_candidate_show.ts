#!/usr/bin/env bun
// Compile one prospective code-overlay C owner without touching the overlay,
// then print its Thumb disassembly beside the reference owner.  This is the
// read-only overlay counterpart to candidate_show.ts.
//
// `overlay_adopt.ts` must rehearse in place so installed-path compiler routing
// remains authoritative.  That makes it the final oracle, but it also means
// two exploratory dry runs against one overlay can race.  Source iteration
// belongs here; adoption happens once, after this view reaches zero diffs.
import { existsSync, mkdirSync, readFileSync } from "node:fs";
import { basename, dirname, join, resolve } from "node:path";
import {
  assembleOverlay,
  compileOverlayCandidate,
  OVERLAY_BASE,
} from "./overlay_disasm.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface FunctionRow {
  id: string;
  offset: number;
  span_bytes: number;
}

interface Options {
  id: string;
  overlay: string;
  offset: number;
  source: string;
  routingSource: string;
  work: string;
  span?: number;
}

function optionsOf(argv: string[]): Options {
  let id = "";
  let source = "";
  let routingSource = "";
  let work = join(ROOT, "work/overlay-candidate-show");
  let span: number | undefined;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--source") source = argv[++index] ?? "";
    else if (argument === "--routing-source") routingSource = argv[++index] ?? "";
    else if (argument === "--work") work = argv[++index] ?? "";
    else if (argument === "--span") span = Number(argv[++index]);
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: overlay_candidate_show.ts OVERLAY:OFFSET --source FILE [--routing-source FILE] [--span BYTES] [--work DIR]");
      process.exit(0);
    } else if (id === "") id = argument;
    else throw new Error(`unrecognised argument: ${argument}`);
  }
  const matched = /^(resource_[0-9a-f]+):(?:0x)?([0-9a-f]+)$/i.exec(id);
  if (matched === null || source === "") {
    throw new Error("usage: overlay_candidate_show.ts OVERLAY:OFFSET --source FILE [--routing-source FILE] [--span BYTES] [--work DIR]");
  }
  if (span !== undefined && (!Number.isInteger(span) || span <= 0)) {
    throw new Error("--span must be a positive integer");
  }
  if (routingSource === "") {
    const stem = (OVERLAY_BASE + Number.parseInt(matched[2], 16)).toString(16).padStart(8, "0");
    routingSource = join(ROOT, "assets/code", `${matched[1]}_c_${stem}.c`);
  }
  return {
    id,
    overlay: matched[1],
    offset: Number.parseInt(matched[2], 16),
    source: resolve(source),
    routingSource: resolve(routingSource),
    work: resolve(work),
    span,
  };
}

export function differingHalfwords(actual: Uint8Array, expected: Uint8Array): Set<number> {
  const found = new Set<number>();
  const shared = Math.min(actual.length, expected.length);
  for (let offset = 0; offset + 2 <= shared; offset += 2) {
    if (actual[offset] !== expected[offset] || actual[offset + 1] !== expected[offset + 1]) {
      found.add(offset);
    }
  }
  for (let offset = shared & ~1; offset < Math.max(actual.length, expected.length); offset += 2) {
    found.add(offset);
  }
  return found;
}

function disassemble(path: string): Map<number, string> {
  const result = Bun.spawnSync([
    "arm-none-eabi-objdump", "-D", "-b", "binary", "-m", "arm", "-M", "force-thumb", path,
  ], { stdout: "pipe", stderr: "pipe" });
  if (result.exitCode !== 0) throw new Error(result.stderr.toString().trim());
  const rows = new Map<number, string>();
  for (const line of result.stdout.toString().split("\n")) {
    const matched = /^\s+([0-9a-f]+):\t[0-9a-f ]+\t(.*)$/.exec(line);
    if (matched !== null) rows.set(Number.parseInt(matched[1], 16), matched[2].trimEnd());
  }
  return rows;
}

function selfTest(): void {
  const same = Uint8Array.from([0x00, 0xb5, 0x01, 0x20]);
  if (differingHalfwords(same, same).size !== 0) throw new Error("equal buffers differ");
  const changed = Uint8Array.from([0x00, 0xb5, 0x02, 0x20]);
  const found = differingHalfwords(same, changed);
  if (found.size !== 1 || !found.has(2)) throw new Error("wrong differing halfword");
  if (!differingHalfwords(same, same.subarray(0, 2)).has(2)) throw new Error("missing length tail");
  console.log("self-test=ok tool=overlay-candidate-show");
}

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const options = optionsOf(Bun.argv.slice(2));
  const inventoryPath = join(ROOT, "out/decomp/overlays.json");
  const inventory = JSON.parse(readFileSync(inventoryPath, "utf8")) as { functions: FunctionRow[] };
  const row = inventory.functions.find((item) => item.id === options.id);
  const span = options.span ?? row?.span_bytes;
  if (span === undefined) throw new Error(`no inventory row for ${options.id}; pass --span BYTES`);
  if (!existsSync(options.source)) throw new Error(`candidate does not exist: ${options.source}`);

  mkdirSync(options.work, { recursive: true });
  const overlayPath = join(ROOT, "assets/code", `${options.overlay}_overlay.s`);
  const image = assembleOverlay(overlayPath, OVERLAY_BASE);
  const expected = image.subarray(options.offset, options.offset + span);
  const compiled = compileOverlayCandidate(
    options.source,
    options.work,
    options.overlay,
    options.routingSource,
  );
  if (compiled.address !== OVERLAY_BASE + options.offset) {
    throw new Error(`candidate entry 0x${compiled.address.toString(16)} does not match ${options.id}`);
  }
  const actual = compiled.data;
  const actualPath = join(options.work, "candidate.bin");
  const expectedPath = join(options.work, "reference.bin");
  await Bun.write(actualPath, actual);
  await Bun.write(expectedPath, expected);
  const left = disassemble(actualPath);
  const right = disassemble(expectedPath);
  const differing = differingHalfwords(actual, expected);
  const offsets = [...new Set([...left.keys(), ...right.keys()])].sort((a, b) => a - b);

  console.log(
    `candidate=${actual.length} reference=${expected.length} ` +
    `differing_halfwords=${differing.size} routing=${basename(options.routingSource)}`,
  );
  console.log("      offset  candidate                      reference");
  for (const offset of offsets) {
    const mark = differing.has(offset) ? "!" : " ";
    const candidate = (left.get(offset) ?? "").padEnd(30).slice(0, 30);
    console.log(`  ${mark} ${offset.toString(16).padStart(4, "0")}  ${candidate} ${right.get(offset) ?? ""}`);
  }
}

if (import.meta.main) await main();
