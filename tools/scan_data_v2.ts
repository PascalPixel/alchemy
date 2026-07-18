#!/usr/bin/env bun
import type { ComparisonReport } from "./compare_roms.ts";
import { compareRegions } from "./compare_regions.ts";
import { splitManifestAtPointers } from "./scan_decomp.ts";

interface Region {
  address: number | string;
  size: number;
  source: string;
  kind?: string;
}

interface Manifest {
  regions: Region[];
}

interface DeltaDiagnosis {
  streams: number;
  consumed_bytes: number;
  consumed_ratio: number;
  decoded_bytes: number;
  maximum_distance: number;
  prefill_references: number;
  average_compressed_bytes: number;
  streams_per_kibibyte: number;
  archive_confidence: "none" | "medium" | "high";
}

type Category =
  "stateful_delta_archive" |
  "cross_title_shared" |
  "same_game_shared" |
  "regional_variant_shared" |
  "localization_or_unique_archive" |
  "unique_data";

function numeric(value: number | string): number {
  const result = typeof value === "number" ? value : Number(value);
  if (!Number.isSafeInteger(result)) throw new Error("invalid numeric value");
  return result;
}

function hexadecimal(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function leaf(path: string): string {
  return path.replaceAll("\\", "/").split("/").at(-1) ?? path;
}

function entropy(data: Uint8Array): [number, number] {
  const counts = new Uint32Array(256);
  for (const value of data) counts[value]++;
  let result = 0;
  let dominant = 0;
  for (const count of counts) {
    if (count === 0) continue;
    const ratio = count / data.length;
    result -= ratio * Math.log2(ratio);
    dominant = Math.max(dominant, ratio);
  }
  return [result, dominant];
}

function scanDeltaStream(data: Uint8Array, start: number, end: number): {
  end: number;
  decoded: number;
  maximumDistance: number;
  prefillReferences: number;
} {
  let cursor = start;
  let decoded = 0;
  let maximumDistance = 0;
  let prefillReferences = 0;
  const take = (): number => {
    if (cursor >= end) throw new Error("delta stream reached its bound");
    return data[cursor++];
  };
  while (true) {
    const flags = take();
    if (flags === 0) {
      if (cursor + 8 > end) throw new Error("delta literal group is truncated");
      cursor += 8;
      decoded += 8;
      continue;
    }
    for (let bit = 7; bit >= 0; bit--) {
      if (!(flags & 1 << bit)) {
        take();
        decoded++;
        continue;
      }
      const first = take();
      const second = take();
      const distance = (first & 0xf0) << 4 | second;
      let length = first & 0x0f;
      if (length === 0) {
        if (distance === 0) return { end: cursor, decoded, maximumDistance, prefillReferences };
        length = take() + 17;
      } else {
        length++;
      }
      maximumDistance = Math.max(maximumDistance, distance);
      if (distance > decoded) prefillReferences++;
      decoded += length;
    }
  }
}

export function diagnoseDeltaArchive(data: Uint8Array): DeltaDiagnosis {
  let cursor = 0;
  let streams = 0;
  let decoded = 0;
  let maximumDistance = 0;
  let prefillReferences = 0;
  try {
    while (cursor < data.length) {
      const result = scanDeltaStream(data, cursor, data.length);
      if (result.end <= cursor) throw new Error("delta stream did not advance");
      cursor = result.end;
      streams++;
      decoded += result.decoded;
      maximumDistance = Math.max(maximumDistance, result.maximumDistance);
      prefillReferences += result.prefillReferences;
    }
  } catch {
  }
  const consumedRatio = data.length === 0 ? 0 : cursor / data.length;
  const averageCompressed = streams === 0 ? 0 : cursor / streams;
  const density = data.length === 0 ? 0 : streams * 1024 / data.length;
  const archiveConfidence = consumedRatio >= 0.7 && streams >= 8 && averageCompressed <= 4096
    ? "high"
    : consumedRatio >= 0.7 && streams >= 4 && averageCompressed <= 8192
      ? "medium"
      : "none";
  return {
    streams,
    consumed_bytes: cursor,
    consumed_ratio: consumedRatio,
    decoded_bytes: decoded,
    maximum_distance: maximumDistance,
    prefill_references: prefillReferences,
    average_compressed_bytes: averageCompressed,
    streams_per_kibibyte: density,
    archive_confidence: archiveConfidence,
  };
}

function privateOutput(path: string): boolean {
  const normalized = path.replaceAll("\\", "/");
  return normalized.startsWith("out/") || normalized.includes("/out/") ||
    normalized.startsWith("/tmp/") || normalized.startsWith("/private/tmp/");
}

function option(args: string[], name: string, fallback?: string): string {
  const index = args.indexOf(name);
  if (index < 0) {
    if (fallback !== undefined) return fallback;
    throw new Error(`${name} is required`);
  }
  if (args[index + 1] === undefined) throw new Error(`${name} requires a value`);
  return args[index + 1];
}

function positional(args: string[]): string[] {
  const valued = new Set(["-o", "--resource-table", "--resource-count", "--code-end"]);
  const result: string[] = [];
  for (let index = 0; index < args.length; index++) {
    if (valued.has(args[index])) index++;
    else if (!args[index].startsWith("-")) result.push(args[index]);
  }
  return result;
}

export function selfTest(): void {
  const stream = Uint8Array.from([0, 1, 2, 3, 4, 5, 6, 7, 8, 0x80, 0, 0]);
  const diagnosis = diagnoseDeltaArchive(Uint8Array.from([...stream, ...stream]));
  if (diagnosis.streams !== 2 || diagnosis.consumed_ratio !== 1 ||
      diagnosis.decoded_bytes !== 16 || diagnosis.maximum_distance !== 0 ||
      diagnosis.archive_confidence !== "none") {
    throw new Error("data scanner delta diagnosis self-test failed");
  }
  const archive = diagnoseDeltaArchive(Uint8Array.from(Array(8).fill([...stream]).flat()));
  if (archive.streams !== 8 || archive.archive_confidence !== "high" ||
      archive.average_compressed_bytes !== stream.length) {
    throw new Error("data scanner archive-confidence self-test failed");
  }
  console.log("self-test=ok");
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    selfTest();
    return;
  }
  const inputs = positional(args);
  if (inputs.length < 3) {
    console.log("usage: scan_data_v2.ts REFERENCE FALLBACK COMPARISON... -o OUT --resource-table ADDRESS --resource-count COUNT [--code-end ADDRESS]");
    return;
  }
  const output = option(args, "-o");
  if (!privateOutput(output)) throw new Error("data scan reports must stay under out/ or a temporary directory");
  const reference = new Uint8Array(await Bun.file(inputs[0]).arrayBuffer());
  const original = await Bun.file(inputs[1]).json() as Manifest;
  const table = Number(option(args, "--resource-table"));
  const count = Number(option(args, "--resource-count"));
  const codeEnd = Number(option(args, "--code-end", "0x08100000"));
  const manifest = splitManifestAtPointers(original, reference, table, count);
  manifest.regions = manifest.regions.filter((region) => numeric(region.address) >= codeEnd);
  const reports = await Promise.all(inputs.slice(2).map(async (path) =>
    await Bun.file(path).json() as ComparisonReport
  ));
  const matches = new Map<string, Record<string, { ratio: number; address: string; confidence: string }>>();
  for (const report of reports) {
    const candidate = leaf(report.candidate);
    const comparison = compareRegions(report, manifest, 0, true);
    for (const match of comparison.regions) {
      const evidence = matches.get(match.source) ?? {};
      evidence[candidate] = {
        ratio: match.ratio,
        address: match.candidate_address,
        confidence: match.confidence ?? "high",
      };
      matches.set(match.source, evidence);
    }
  }
  const pointerIds = new Map<number, number[]>();
  const view = new DataView(reference.buffer, reference.byteOffset, reference.byteLength);
  for (let id = 0; id < count; id++) {
    const pointer = view.getUint32(table - 0x08000000 + id * 4, true);
    if (pointer < 0x08000000 || pointer > 0x08000000 + reference.length) continue;
    pointerIds.set(pointer, [...(pointerIds.get(pointer) ?? []), id]);
  }
  const items = manifest.regions.map((region) => {
    const address = numeric(region.address);
    const bytes = reference.subarray(address - 0x08000000, address - 0x08000000 + region.size);
    const [information, dominant] = entropy(bytes);
    const evidence = matches.get(region.source) ?? {};
    const gs1 = Object.entries(evidence).filter(([name]) => name.startsWith("gs1-"));
    const gs2 = Object.entries(evidence).filter(([name]) => name.startsWith("gs2-"));
    const gs1Full = gs1.filter(([, item]) => item.ratio >= 0.99).length;
    const gs2Full = gs2.filter(([, item]) => item.ratio >= 0.99).length;
    const ids = pointerIds.get(address) ?? [];
    const delta = diagnoseDeltaArchive(bytes);
    const stateful = ids.length > 0 && delta.archive_confidence !== "none" &&
      delta.maximum_distance <= 0xfff && delta.prefill_references > 0;
    let category: Category;
    if (stateful) category = "stateful_delta_archive";
    else if (gs2Full > 0) category = "cross_title_shared";
    else if (gs1Full >= 3) category = "same_game_shared";
    else if (gs1Full > 0) category = "regional_variant_shared";
    else if (region.size >= 4096 && information >= 5) category = "localization_or_unique_archive";
    else category = "unique_data";
    const score = region.size * (category === "stateful_delta_archive" ? 1.5 :
      category === "cross_title_shared" ? 1.35 : category === "same_game_shared" ? 1.2 : 1);
    return {
      category,
      score: Math.round(score),
      address: hexadecimal(address),
      size: region.size,
      resource_ids: ids.map((id) => `0x${id.toString(16)}`),
      entropy: Number(information.toFixed(4)),
      dominant_byte_ratio: Number(dominant.toFixed(6)),
      gs1_full_supporters: gs1Full,
      gs2_full_supporters: gs2Full,
      best_gs1_ratio: Math.max(0, ...gs1.map(([, item]) => item.ratio)),
      best_gs2_ratio: Math.max(0, ...gs2.map(([, item]) => item.ratio)),
      delta,
      evidence,
    };
  }).sort((left, right) => right.score - left.score || right.size - left.size ||
    left.address.localeCompare(right.address));
  const categories: Record<string, { regions: number; bytes: number }> = {};
  for (const item of items) {
    categories[item.category] ??= { regions: 0, bytes: 0 };
    categories[item.category].regions++;
    categories[item.category].bytes += item.size;
  }
  const result = {
    format: 2,
    kind: "multi-rom-data-scan",
    reference: leaf(inputs[0]),
    reports: reports.map((report) => leaf(report.candidate)),
    resource_table: hexadecimal(table),
    resource_count: count,
    code_end: hexadecimal(codeEnd),
    regions: items.length,
    categories,
    items,
  };
  await Bun.write(output, JSON.stringify(result, null, 2) + "\n");
  console.log(`regions=${items.length} ` + Object.entries(categories)
    .map(([name, value]) => `${name}=${value.regions}/${value.bytes}`).join(" "));
}

if (import.meta.main) await main(Bun.argv.slice(2));
