export interface MatchSpan {
  reference_address: string;
  candidate_address: string;
  size: number;
}

export interface SameOffsetSpan {
  address: string;
  size: number;
}

export interface ComparisonReport {
  format: 1;
  reference: string;
  candidate: string;
  reference_size: number;
  candidate_size: number;
  same_offset: {
    compared_bytes: number;
    matching_bytes: number;
    ratio: number;
    run_count: number;
    longest_runs: SameOffsetSpan[];
  };
  relocated: {
    block_size: number;
    step: number;
    span_count: number;
    matched_bytes: number;
    longest_spans: MatchSpan[];
  };
  thumb_relocated?: {
    block_size: number;
    step: number;
    span_count: number;
    matched_bytes: number;
    exact_matched_bytes: number;
    improvement_bytes: number;
    normalization: {
      reference_long_calls: number;
      reference_rom_pointer_literals: number;
      candidate_long_calls: number;
      candidate_rom_pointer_literals: number;
    };
    longest_spans: MatchSpan[];
  };
}

interface RelativeSpan {
  reference: number;
  candidate: number;
  size: number;
}

export interface ThumbNormalization {
  data: Uint8Array;
  long_calls: number;
  rom_pointer_literals: number;
}

function integer(value: string, label: string): number {
  const result = Number(value);
  if (!Number.isSafeInteger(result)) throw new Error(`${label} must be an integer`);
  return result;
}

function basename(path: string): string {
  return path.replaceAll("\\", "/").split("/").at(-1) ?? path;
}

function hexadecimal(value: number, base: number): string {
  return `0x${(value + base).toString(16).padStart(8, "0")}`;
}

function hashBlock(data: Uint8Array, start: number, size: number): number {
  let hash = 0x811c9dc5;
  const end = start + size;
  for (let index = start; index < end; index++) {
    hash = Math.imul(hash ^ data[index], 0x01000193) >>> 0;
  }
  return hash;
}

function readU16(data: Uint8Array, offset: number): number {
  return data[offset] | data[offset + 1] << 8;
}

function readU32(data: Uint8Array, offset: number): number {
  return (data[offset] | data[offset + 1] << 8 | data[offset + 2] << 16 |
    data[offset + 3] << 24) >>> 0;
}

function writeU16(data: Uint8Array, offset: number, value: number): void {
  data[offset] = value;
  data[offset + 1] = value >>> 8;
}

function writeU32(data: Uint8Array, offset: number, value: number): void {
  data[offset] = value;
  data[offset + 1] = value >>> 8;
  data[offset + 2] = value >>> 16;
  data[offset + 3] = value >>> 24;
}

function thumbLongCallTarget(data: Uint8Array, offset: number, base: number): number | null {
  if (offset < 0 || offset + 4 > data.length || (offset & 1) !== 0) return null;
  const first = readU16(data, offset);
  const second = readU16(data, offset + 2);
  if ((first & 0xf800) !== 0xf000 || (second & 0xf800) !== 0xf800) return null;
  let high = first & 0x07ff;
  if ((high & 0x0400) !== 0) high -= 0x0800;
  return base + offset + 4 + high * 0x1000 + (second & 0x07ff) * 2;
}

function romPointer(value: number, base: number, size: number): boolean {
  const address = value - (value & 1);
  return address >= base && address < base + size;
}

export function normalizeThumbRelocations(
  source: Uint8Array,
  base = 0x08000000,
): ThumbNormalization {
  if (!Number.isSafeInteger(base) || base < 0 || base > 0xffffffff) {
    throw new Error("base must be a u32 address");
  }
  const data = Uint8Array.from(source);
  const literalOffsets = new Set<number>();
  let longCalls = 0;
  for (let offset = 0; offset + 4 <= source.length; offset += 2) {
    const target = thumbLongCallTarget(source, offset, base);
    if (target !== null && target >= base && target < base + source.length && (target & 1) === 0) {
      writeU16(data, offset, 0xf000);
      writeU16(data, offset + 2, 0xf800);
      longCalls++;
    }
    const instruction = readU16(source, offset);
    if ((instruction & 0xf800) === 0x4800) {
      const literal = ((offset + 4) & ~3) + (instruction & 0xff) * 4;
      if (literal + 4 <= source.length && (literal & 3) === 0 &&
          romPointer(readU32(source, literal), base, source.length)) {
        literalOffsets.add(literal);
      }
    }
  }
  for (const offset of literalOffsets) {
    const value = readU32(source, offset);
    writeU32(data, offset, (base | (value & 1)) >>> 0);
  }
  return {
    data,
    long_calls: longCalls,
    rom_pointer_literals: literalOffsets.size,
  };
}

function equalAt(
  left: Uint8Array,
  leftStart: number,
  right: Uint8Array,
  rightStart: number,
  size: number,
): boolean {
  for (let index = 0; index < size; index++) {
    if (left[leftStart + index] !== right[rightStart + index]) return false;
  }
  return true;
}

export function sameOffsetRuns(
  reference: Uint8Array,
  candidate: Uint8Array,
  minimum: number,
): { compared: number; matching: number; runs: RelativeSpan[] } {
  if (!Number.isSafeInteger(minimum) || minimum <= 0) throw new Error("minimum run must be positive");
  const compared = Math.min(reference.length, candidate.length);
  const runs: RelativeSpan[] = [];
  let matching = 0;
  let start = -1;
  for (let index = 0; index < compared; index++) {
    if (reference[index] === candidate[index]) {
      matching++;
      if (start < 0) start = index;
    } else if (start >= 0) {
      const size = index - start;
      if (size >= minimum) runs.push({ reference: start, candidate: start, size });
      start = -1;
    }
  }
  if (start >= 0 && compared - start >= minimum) {
    runs.push({ reference: start, candidate: start, size: compared - start });
  }
  return { compared, matching, runs };
}

export function relocatedMatches(
  reference: Uint8Array,
  candidate: Uint8Array,
  blockSize: number,
  step: number,
  maximumOccurrences = 16,
): RelativeSpan[] {
  if (!Number.isSafeInteger(blockSize) || blockSize <= 0) throw new Error("block size must be positive");
  if (!Number.isSafeInteger(step) || step <= 0 || step > blockSize) throw new Error("step must fit block size");
  if (!Number.isSafeInteger(maximumOccurrences) || maximumOccurrences <= 0) {
    throw new Error("maximum occurrences must be positive");
  }
  const index = new Map<number, number[] | null>();
  for (let start = 0; start + blockSize <= candidate.length; start += step) {
    const hash = hashBlock(candidate, start, blockSize);
    const positions = index.get(hash);
    if (positions === null) continue;
    if (positions === undefined) {
      index.set(hash, [start]);
    } else if (positions.length >= maximumOccurrences) {
      index.set(hash, null);
    } else {
      positions.push(start);
    }
  }
  const spans: RelativeSpan[] = [];
  for (let start = 0; start + blockSize <= reference.length; start += step) {
    const positions = index.get(hashBlock(reference, start, blockSize));
    if (!positions) continue;
    const previous = spans.at(-1);
    const continuation = previous && previous.reference + previous.size - blockSize + step === start
      ? previous.candidate + previous.size - blockSize + step
      : -1;
    let chosen = -1;
    if (continuation >= 0 && positions.includes(continuation) &&
        equalAt(reference, start, candidate, continuation, blockSize)) {
      chosen = continuation;
    } else {
      for (const position of positions) {
        if (equalAt(reference, start, candidate, position, blockSize)) {
          chosen = position;
          break;
        }
      }
    }
    if (chosen < 0) continue;
    if (previous && start === previous.reference + previous.size - blockSize + step &&
        chosen === previous.candidate + previous.size - blockSize + step) {
      previous.size += step;
    } else {
      spans.push({ reference: start, candidate: chosen, size: blockSize });
    }
  }
  return spans;
}

export function compare(
  referenceName: string,
  reference: Uint8Array,
  candidateName: string,
  candidate: Uint8Array,
  minimumRun: number,
  blockSize: number,
  step: number,
  limit: number,
  base = 0x08000000,
  thumbRelocations = false,
): ComparisonReport {
  if (!Number.isSafeInteger(limit) || limit <= 0) throw new Error("limit must be positive");
  const same = sameOffsetRuns(reference, candidate, minimumRun);
  const relocated = relocatedMatches(reference, candidate, blockSize, step);
  const longestSame = [...same.runs].sort((left, right) => right.size - left.size).slice(0, limit);
  const longestRelocated = [...relocated].sort((left, right) => right.size - left.size).slice(0, limit);
  const report: ComparisonReport = {
    format: 1,
    reference: basename(referenceName),
    candidate: basename(candidateName),
    reference_size: reference.length,
    candidate_size: candidate.length,
    same_offset: {
      compared_bytes: same.compared,
      matching_bytes: same.matching,
      ratio: same.compared === 0 ? 0 : same.matching / same.compared,
      run_count: same.runs.length,
      longest_runs: longestSame.map((span) => ({
        address: hexadecimal(span.reference, base),
        size: span.size,
      })),
    },
    relocated: {
      block_size: blockSize,
      step,
      span_count: relocated.length,
      matched_bytes: relocated.reduce((sum, span) => sum + span.size, 0),
      longest_spans: longestRelocated.map((span) => ({
        reference_address: hexadecimal(span.reference, base),
        candidate_address: hexadecimal(span.candidate, base),
        size: span.size,
      })),
    },
  };
  if (thumbRelocations) {
    const normalizedReference = normalizeThumbRelocations(reference, base);
    const normalizedCandidate = normalizeThumbRelocations(candidate, base);
    const normalized = relocatedMatches(
      normalizedReference.data, normalizedCandidate.data, blockSize, step,
    );
    const longestNormalized = [...normalized]
      .sort((left, right) => right.size - left.size)
      .slice(0, limit);
    const matchedBytes = normalized.reduce((sum, span) => sum + span.size, 0);
    const exactMatchedBytes = report.relocated.matched_bytes;
    report.thumb_relocated = {
      block_size: blockSize,
      step,
      span_count: normalized.length,
      matched_bytes: matchedBytes,
      exact_matched_bytes: exactMatchedBytes,
      improvement_bytes: matchedBytes - exactMatchedBytes,
      normalization: {
        reference_long_calls: normalizedReference.long_calls,
        reference_rom_pointer_literals: normalizedReference.rom_pointer_literals,
        candidate_long_calls: normalizedCandidate.long_calls,
        candidate_rom_pointer_literals: normalizedCandidate.rom_pointer_literals,
      },
      longest_spans: longestNormalized.map((span) => ({
        reference_address: hexadecimal(span.reference, base),
        candidate_address: hexadecimal(span.candidate, base),
        size: span.size,
      })),
    };
  }
  return report;
}

function synthetic(seed: number, size: number): Uint8Array {
  const data = new Uint8Array(size);
  let state = seed >>> 0;
  for (let index = 0; index < size; index++) {
    state ^= state << 13;
    state ^= state >>> 17;
    state ^= state << 5;
    data[index] = state;
  }
  return data;
}

function encodeThumbLongCall(data: Uint8Array, offset: number, target: number, base: number): void {
  const displacement = target - (base + offset + 4);
  if ((displacement & 1) !== 0 || displacement < -0x400000 || displacement > 0x3ffffe) {
    throw new Error("synthetic long call is outside Thumb range");
  }
  writeU16(data, offset, 0xf000 | displacement >> 12 & 0x07ff);
  writeU16(data, offset + 2, 0xf800 | displacement >> 1 & 0x07ff);
}

export function selfTest(): void {
  const reference = synthetic(1, 1024);
  const candidate = synthetic(2, 1280);
  candidate.set(reference.subarray(128, 640), 400);
  const moved = relocatedMatches(reference, candidate, 64, 16);
  if (!moved.some((span) => span.reference === 128 && span.candidate === 400 && span.size === 512)) {
    throw new Error("relocated comparison self-test failed");
  }
  const sameCandidate = reference.slice();
  sameCandidate.fill(0, 96, 128);
  const same = sameOffsetRuns(reference, sameCandidate, 32);
  if (same.matching !== 992 || same.runs.length !== 2 || same.runs[0].size !== 96) {
    throw new Error("same-offset comparison self-test failed");
  }
  const base = 0x08000000;
  const instructions = new Uint8Array(64);
  writeU16(instructions, 0, 0x4803);
  encodeThumbLongCall(instructions, 4, base + 48, base);
  writeU16(instructions, 8, 0xe001);
  writeU32(instructions, 16, base + 41);
  writeU32(instructions, 20, base + 45);
  const normalizedInstructions = normalizeThumbRelocations(instructions, base);
  if (normalizedInstructions.long_calls !== 1 || normalizedInstructions.rom_pointer_literals !== 1 ||
      readU16(normalizedInstructions.data, 4) !== 0xf000 ||
      readU16(normalizedInstructions.data, 6) !== 0xf800 ||
      readU16(normalizedInstructions.data, 8) !== 0xe001 ||
      readU32(normalizedInstructions.data, 16) !== base + 1 ||
      readU32(normalizedInstructions.data, 20) !== base + 45) {
    throw new Error("Thumb relocation normalization self-test failed");
  }
  const relocatedReference = synthetic(3, 1024);
  const relocatedCandidate = synthetic(4, 1280);
  relocatedCandidate.set(relocatedReference.subarray(128, 256), 400);
  writeU16(relocatedReference, 128, 0x4807);
  writeU16(relocatedCandidate, 400, 0x4807);
  encodeThumbLongCall(relocatedReference, 136, base + 800, base);
  encodeThumbLongCall(relocatedCandidate, 408, base + 1000, base);
  writeU32(relocatedReference, 160, base + 701);
  writeU32(relocatedCandidate, 432, base + 901);
  const exactMoved = relocatedMatches(relocatedReference, relocatedCandidate, 64, 16);
  const normalizedMoved = relocatedMatches(
    normalizeThumbRelocations(relocatedReference, base).data,
    normalizeThumbRelocations(relocatedCandidate, base).data,
    64,
    16,
  );
  if (exactMoved.some((span) => span.reference === 128 && span.candidate === 400 && span.size >= 128) ||
      !normalizedMoved.some((span) => span.reference === 128 && span.candidate === 400 && span.size >= 128)) {
    throw new Error("Thumb-aware relocated comparison self-test failed");
  }
  console.log("self-test=ok");
}

function option(args: string[], name: string, fallback?: string): string {
  const index = args.indexOf(name);
  if (index < 0) {
    if (fallback !== undefined) return fallback;
    throw new Error(`${name} is required`);
  }
  if (index + 1 >= args.length) throw new Error(`${name} requires a value`);
  return args[index + 1];
}

function privateOutput(path: string): boolean {
  const normalized = path.replaceAll("\\", "/");
  return normalized.startsWith("out/") || normalized.includes("/out/") ||
    normalized.startsWith("/tmp/") || normalized.startsWith("/private/tmp/");
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    selfTest();
    return;
  }
  const positional = args.filter((argument, index) =>
    !argument.startsWith("-") && (index === 0 || !args[index - 1].startsWith("-")));
  if (positional.length < 2) {
    console.log("usage: compare_roms.ts REFERENCE CANDIDATE -o OUT [--min-run N] [--block-size N] [--step N] [--limit N] [--thumb-relocations]");
    return;
  }
  const output = option(args, "-o");
  if (!privateOutput(output)) throw new Error("comparison reports must stay under out/ or a temporary directory");
  const referencePath = positional[0];
  const candidatePath = positional[1];
  const reference = new Uint8Array(await Bun.file(referencePath).arrayBuffer());
  const candidate = new Uint8Array(await Bun.file(candidatePath).arrayBuffer());
  const report = compare(
    referencePath,
    reference,
    candidatePath,
    candidate,
    integer(option(args, "--min-run", "64"), "--min-run"),
    integer(option(args, "--block-size", "256"), "--block-size"),
    integer(option(args, "--step", "64"), "--step"),
    integer(option(args, "--limit", "128"), "--limit"),
    0x08000000,
    args.includes("--thumb-relocations"),
  );
  await Bun.write(output, JSON.stringify(report, null, 2) + "\n");
  console.log(`same_offset=${report.same_offset.matching_bytes}/${report.same_offset.compared_bytes} ` +
    `same_runs=${report.same_offset.run_count} relocated_spans=${report.relocated.span_count} ` +
    `relocated_bytes=${report.relocated.matched_bytes}` +
    (report.thumb_relocated === undefined ? "" :
      ` thumb_relocated_spans=${report.thumb_relocated.span_count} ` +
      `thumb_relocated_bytes=${report.thumb_relocated.matched_bytes} ` +
      `thumb_improvement=${report.thumb_relocated.improvement_bytes}`));
}

if (import.meta.main) await main(Bun.argv.slice(2));
