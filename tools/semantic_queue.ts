#!/usr/bin/env bun
// Rank the remaining main-image semantic work by reconstruction cost.
//
// Historical m2c output is useful evidence, but it is not admission-ready C.
// This queue puts compact, ordinary-ABI owners first and makes the expensive
// failure modes visible before a human spends time cleaning a draft.
import {
  existsSync,
  readdirSync,
  readFileSync,
} from "node:fs";
import { basename, dirname, join, relative } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ORDINARY_RETENTION = new Set([
  "c_candidate",
  "split_first",
  "merge_with_continuations",
  "merge_with_owner",
]);

interface Region {
  address: number;
  size: number;
  source: string;
  retention: string;
}

interface Candidate {
  stem: string;
  bytes: number;
  calls: number;
  sourceLines: number;
  unsetRegisters: number;
  internalExits: number;
  unknownTypes: number;
  highRegisterCallSetups: number;
  runtimeThunkCalls: number;
  boundaryShape: string;
  blockedLane?: string;
  blockedReason?: string;
  draft: string;
  score: number;
}

function sourceStems(directory: string): Set<string> {
  if (!existsSync(directory)) return new Set();
  return new Set(
    readdirSync(directory)
      .filter((name) => /^08[0-9a-f]{6}\.c$/i.test(name))
      .map((name) => basename(name, ".c").toLowerCase()),
  );
}

function occurrences(text: string, expression: RegExp): number {
  return [...text.matchAll(expression)].length;
}

export function analyzeCandidate(
  region: Region,
  draftPath: string,
): Candidate {
  const draft = readFileSync(draftPath, "utf8");
  const assembly = readFileSync(
    region.source.startsWith("/") ? region.source : join(ROOT, region.source),
    "utf8",
  );
  const calls = occurrences(assembly, /^\s*bl\s+\S+/gm);
  const unsetRegisters = occurrences(draft, /\bM2C_ERROR\s*\(/g);
  const internalExits = occurrences(
    assembly,
    /^\s*b(?:\.[a-z]+)?\s+Func_08[0-9a-f]{6}\b/gim,
  );
  const unknownTypes = occurrences(draft, /\bM2C_UNK\b/g);
  const runtimeThunkCalls = occurrences(
    assembly,
    /^\s*bl\s+Func_08007(?:2e4|2e8|2ec|2f0|2f4|2f8|2fc|300|304|308|30c|310|314|318)\b/gim,
  );
  const sourceLines = draft.split("\n").length;
  const assemblyLines = assembly.split("\n");
  let highRegisterCallSetups = 0;
  for (let index = 0; index < assemblyLines.length; index++) {
    if (!/^\s*bl\s+\S+/.test(assemblyLines[index])) continue;
    const setup = assemblyLines.slice(Math.max(0, index - 5), index).join("\n");
    if (/\bmov\s+(?:r8|r9|sl|fp)\s*,/.test(setup))
      highRegisterCallSetups++;
  }
  const boundaryPenalty = region.retention === "c_candidate" ? 0 : 500;

  // Calls and unresolved register values dominate review time. Source length
  // breaks ties between otherwise similar owners; byte size is deliberately
  // not a penalty because every admitted byte advances the semantic goal.
  const score =
    calls * 20 +
    unsetRegisters * 200 +
    internalExits * 250 +
    unknownTypes * 8 +
    highRegisterCallSetups * 100 +
    runtimeThunkCalls * 250 +
    boundaryPenalty +
    Math.ceil(sourceLines / 10);
  return {
    stem: region.address.toString(16).padStart(8, "0"),
    bytes: region.size,
    calls,
    sourceLines,
    unsetRegisters,
    internalExits,
    unknownTypes,
    highRegisterCallSetups,
    runtimeThunkCalls,
    boundaryShape: region.retention,
    draft: relative(ROOT, draftPath),
    score,
  };
}

export function semanticQueue(): Candidate[] {
  const manifest = JSON.parse(
    readFileSync(join(ROOT, "out", "full", "asm", "manifest.json"), "utf8"),
  ) as { regions: Region[] };
  const admitted = new Set([
    ...sourceStems(join(ROOT, "src")),
    ...sourceStems(join(ROOT, "semantic", "main")),
  ]);
  const blockersPath = join(ROOT, "semantic", "ordinary-blockers.json");
  const blockers = existsSync(blockersPath)
    ? (JSON.parse(readFileSync(blockersPath, "utf8")) as {
      owners: Record<string, { lane: string; reason: string }>;
    }).owners
    : {};
  const candidates: Candidate[] = [];

  for (const region of manifest.regions) {
    if (!ORDINARY_RETENTION.has(region.retention)) continue;
    const stem = region.address.toString(16).padStart(8, "0");
    if (admitted.has(stem)) continue;
    const paths = [
      join(ROOT, "work", "candidates", `${stem}.c`),
      join(ROOT, "work", "m2c-ctx", `${stem}.c`),
      join(ROOT, "work", `${stem}.c`),
    ];
    const draft = paths.find((path) => existsSync(path));
    if (draft !== undefined) {
      const candidate = analyzeCandidate(region, draft);
      const blocker = blockers[stem];
      if (blocker !== undefined) {
        candidate.score += 10_000;
        candidate.blockedLane = blocker.lane;
        candidate.blockedReason = blocker.reason;
      }
      candidates.push(candidate);
    }
  }
  return candidates.sort(
    (left, right) =>
      left.score - right.score ||
      right.bytes - left.bytes ||
      left.stem.localeCompare(right.stem),
  );
}

function selfTest(): void {
  const draft = "M2C_UNK Func_08001234(void);\nM2C_ERROR(/* r0 */);\n";
  if (occurrences(draft, /\bM2C_UNK\b/g) !== 1)
    throw new Error("unknown-type count is wrong");
  if (occurrences(draft, /\bM2C_ERROR\s*\(/g) !== 1)
    throw new Error("unset-register count is wrong");
  const assembly = "  bl Func_08001234\n  b.n Func_08005678\n";
  if (occurrences(assembly, /^\s*bl\s+\S+/gm) !== 1)
    throw new Error("call count is wrong");
  if (occurrences(
      assembly,
      /^\s*b(?:\.[a-z]+)?\s+Func_08[0-9a-f]{6}\b/gim,
    ) !== 1)
    throw new Error("internal-exit count is wrong");
  console.log("self-test=ok");
}

if (import.meta.main) {
  if (Bun.argv.includes("--self-test")) {
    selfTest();
  } else {
    const queue = semanticQueue();
    if (Bun.argv.includes("--json")) {
      console.log(JSON.stringify(queue, null, 2));
    } else {
      const limitArgument = Bun.argv.find((argument) =>
        argument.startsWith("--limit=")
      );
      const limit = limitArgument === undefined
        ? 30
        : Number.parseInt(limitArgument.slice("--limit=".length), 10);
      for (const item of queue.slice(0, limit)) {
        console.log(
          `${item.stem} ${item.bytes.toString().padStart(5)}B ` +
          `score=${item.score.toString().padStart(4)} ` +
          `calls=${item.calls.toString().padStart(3)} ` +
          `unset=${item.unsetRegisters} exits=${item.internalExits} ` +
          `higharg=${item.highRegisterCallSetups} ` +
          `thunks=${item.runtimeThunkCalls} ` +
          `unk=${item.unknownTypes.toString().padStart(2)} ` +
          `shape=${item.boundaryShape} ` +
          `${item.blockedLane === undefined ? "" : `blocked=${item.blockedLane} `}` +
          `${item.draft}`,
        );
      }
      console.log(
        `queued=${queue.length} bytes=${queue.reduce(
          (sum, item) => sum + item.bytes,
          0,
        )}`,
      );
    }
  }
}
