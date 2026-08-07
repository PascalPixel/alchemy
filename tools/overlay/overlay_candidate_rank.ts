#!/usr/bin/env bun
// Compile semantic overlay owners without touching their canonical overlays,
// then rank the candidates by likely exactification effort.  This turns the
// exact-reading queue into a measured queue: twins still multiply a solution,
// while zero-size-delta candidates with only a few differing halfwords rise to
// the top instead of losing to a large but low-probability owner.
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
import { readingList, type Pairing } from "../lib/exact_reading_list.ts";
import {
  assembleOverlay,
  compileOverlayCandidate,
  OVERLAY_BASE,
} from "../lib/overlay_disasm.ts";

const ROOT = dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))));

interface Measurement {
  id: string;
  overlay: string;
  address: string;
  span: number;
  candidateBytes?: number;
  sizeDelta?: number;
  differingHalfwords?: number;
  semanticSource: string;
  error?: string;
}

interface WorkerInput {
  work: string;
  rows: Pairing[];
}

function countDifferingHalfwords(actual: Uint8Array, expected: Uint8Array): number {
  let differing = 0;
  const length = Math.max(actual.length, expected.length);
  for (let offset = 0; offset < length; offset += 2) {
    if (actual[offset] !== expected[offset] || actual[offset + 1] !== expected[offset + 1]) differing++;
  }
  return differing;
}

export function effortTier(row: Measurement): number {
  if (row.error !== undefined || row.sizeDelta === undefined || row.differingHalfwords === undefined) return 5;
  if (row.sizeDelta !== 0) return 4;
  if (row.differingHalfwords === 0) return 0;
  if (row.differingHalfwords <= 8) return 1;
  if (row.differingHalfwords <= 20) return 2;
  return 3;
}

function compareMeasurements(left: Measurement, right: Measurement): number {
  const tier = effortTier(left) - effortTier(right);
  if (tier !== 0) return tier;
  const leftDiff = left.differingHalfwords ?? Number.MAX_SAFE_INTEGER;
  const rightDiff = right.differingHalfwords ?? Number.MAX_SAFE_INTEGER;
  if (leftDiff !== rightDiff) return leftDiff - rightDiff;
  return right.span - left.span;
}

function measureWorker(inputPath: string, outputPath: string): void {
  const input = JSON.parse(readFileSync(inputPath, "utf8")) as WorkerInput;
  const images = new Map<string, Buffer>();
  const measurements: Measurement[] = [];
  for (const row of input.rows) {
    const id = `${row.overlay}:${row.address.slice(-4)}`;
    try {
      let image = images.get(row.overlay);
      if (image === undefined) {
        image = assembleOverlay(join(ROOT, "assets/code", `${row.overlay}_overlay.s`), OVERLAY_BASE);
        images.set(row.overlay, image);
      }
      const address = Number.parseInt(row.address, 16);
      const expected = image.subarray(address - OVERLAY_BASE, address - OVERLAY_BASE + row.bytes);
      const source = resolve(ROOT, row.semanticSource);
      const routingSource = join(ROOT, "exact", `${row.overlay}_c_${row.address.slice(2)}.c`);
      const ownerWork = join(input.work, `${row.overlay}-${row.address.slice(2)}`);
      mkdirSync(ownerWork, { recursive: true });
      const compiled = compileOverlayCandidate(source, ownerWork, row.overlay, routingSource);
      measurements.push({
        id,
        overlay: row.overlay,
        address: row.address,
        span: row.bytes,
        candidateBytes: compiled.data.length,
        sizeDelta: compiled.data.length - row.bytes,
        differingHalfwords: countDifferingHalfwords(compiled.data, expected),
        semanticSource: row.semanticSource,
      });
    } catch (cause) {
      measurements.push({
        id,
        overlay: row.overlay,
        address: row.address,
        span: row.bytes,
        semanticSource: row.semanticSource,
        error: cause instanceof Error ? cause.message : String(cause),
      });
    }
  }
  writeFileSync(outputPath, JSON.stringify(measurements));
}

function selfTest(): void {
  const base: Measurement = {
    id: "resource_000:0000",
    overlay: "resource_000",
    address: "0x02000000",
    span: 16,
    semanticSource: "semantic/example.c",
  };
  if (countDifferingHalfwords(Uint8Array.of(0, 1), Uint8Array.of(0, 1)) !== 0) throw new Error("equal");
  if (countDifferingHalfwords(Uint8Array.of(0, 1), Uint8Array.of(0, 2)) !== 1) throw new Error("diff");
  if (effortTier({ ...base, sizeDelta: 0, differingHalfwords: 0 }) !== 0) throw new Error("exact tier");
  if (effortTier({ ...base, sizeDelta: 0, differingHalfwords: 8 }) !== 1) throw new Error("near tier");
  if (effortTier({ ...base, sizeDelta: 0, differingHalfwords: 20 }) !== 2) throw new Error("shape tier");
  if (effortTier({ ...base, sizeDelta: 2, differingHalfwords: 1 }) !== 4) throw new Error("size tier");
  console.log("self-test=ok tool=overlay-candidate-rank");
}

async function main(): Promise<void> {
  const args = Bun.argv.slice(2);
  if (args[0] === "--worker") return measureWorker(args[1], args[2]);
  if (args.includes("--self-test")) return selfTest();
  const valueAfter = (flag: string): string | undefined => {
    const index = args.indexOf(flag);
    return index < 0 ? undefined : args[index + 1];
  };
  const onlyOverlay = valueAfter("--overlay");
  const jobs = Number(valueAfter("--jobs") ?? Math.max(1, Math.min(8, navigator.hardwareConcurrency - 2)));
  const top = Number(valueAfter("--top") ?? 40);
  const limit = Number(valueAfter("--max") ?? Number.MAX_SAFE_INTEGER);
  if (!Number.isInteger(jobs) || jobs < 1 || !Number.isInteger(top) || top < 1 || !Number.isInteger(limit) || limit < 1) {
    throw new Error("--jobs, --top and --max must be positive integers");
  }
  let rows = readingList().filter((row) => !row.blocked);
  if (onlyOverlay !== undefined) rows = rows.filter((row) => row.overlay === onlyOverlay);
  rows = rows.slice(0, limit);
  const work = resolve(valueAfter("--work") ?? join(ROOT, "out/overlay-candidate-rank"));
  mkdirSync(work, { recursive: true });
  const buckets = Array.from({ length: Math.min(jobs, Math.max(1, rows.length)) }, () => [] as Pairing[]);
  rows.forEach((row, index) => buckets[index % buckets.length].push(row));
  const children = buckets.map((bucket, index) => {
    const inputPath = join(work, `worker-${index}.input.json`);
    const outputPath = join(work, `worker-${index}.output.json`);
    writeFileSync(inputPath, JSON.stringify({ work: join(work, `worker-${index}`), rows: bucket }));
    return {
      outputPath,
      child: Bun.spawn([process.execPath, import.meta.path, "--worker", inputPath, outputPath], {
        cwd: ROOT,
        stdout: "pipe",
        stderr: "pipe",
      }),
    };
  });
  for (const { child } of children) {
    const status = await child.exited;
    if (status !== 0) throw new Error((await new Response(child.stderr).text()).trim() || `worker failed: ${status}`);
  }
  const measured = children.flatMap(({ outputPath }) =>
    existsSync(outputPath) ? JSON.parse(readFileSync(outputPath, "utf8")) as Measurement[] : [],
  ).sort(compareMeasurements);
  writeFileSync(join(work, "report.json"), JSON.stringify({ measured }, null, 2) + "\n");
  console.log("tier  owner                 span  bytes  delta  diff_hw  semantic source");
  for (const row of measured.slice(0, top)) {
    const delta = row.sizeDelta === undefined ? "error" : `${row.sizeDelta >= 0 ? "+" : ""}${row.sizeDelta}`;
    console.log(
      `${String(effortTier(row)).padStart(4)}  ${row.id.padEnd(20)}  ${String(row.span).padStart(4)}  ` +
      `${String(row.candidateBytes ?? "-").padStart(5)}  ${delta.padStart(5)}  ` +
      `${String(row.differingHalfwords ?? "-").padStart(7)}  ${row.semanticSource}`,
    );
  }
  const counts = Array.from({ length: 6 }, (_, tier) => measured.filter((row) => effortTier(row) === tier).length);
  console.log(`measured=${measured.length} exact=${counts[0]} near8=${counts[1]} near20=${counts[2]} same_size_far=${counts[3]} size_mismatch=${counts[4]} errors=${counts[5]}`);
  console.log(`report=${join(work, "report.json")}`);
}

if (import.meta.main) await main();
