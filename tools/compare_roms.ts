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
}

interface RelativeSpan {
  reference: number;
  candidate: number;
  size: number;
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
): ComparisonReport {
  if (!Number.isSafeInteger(limit) || limit <= 0) throw new Error("limit must be positive");
  const same = sameOffsetRuns(reference, candidate, minimumRun);
  const relocated = relocatedMatches(reference, candidate, blockSize, step);
  const longestSame = [...same.runs].sort((left, right) => right.size - left.size).slice(0, limit);
  const longestRelocated = [...relocated].sort((left, right) => right.size - left.size).slice(0, limit);
  return {
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
    console.log("usage: compare_roms.ts REFERENCE CANDIDATE -o OUT [--min-run N] [--block-size N] [--step N] [--limit N]");
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
  );
  await Bun.write(output, JSON.stringify(report, null, 2) + "\n");
  console.log(`same_offset=${report.same_offset.matching_bytes}/${report.same_offset.compared_bytes} ` +
    `same_runs=${report.same_offset.run_count} relocated_spans=${report.relocated.span_count} ` +
    `relocated_bytes=${report.relocated.matched_bytes}`);
}

if (import.meta.main) await main(Bun.argv.slice(2));
