#!/usr/bin/env bun
// Compile semantic reconstructions without placing their output
// in the byte-exact ROM.  This makes "understood and expressed as C" a useful
// intermediate state while the existing exact build remains authoritative.
import {
  existsSync,
  mkdtempSync,
  readdirSync,
  readFileSync,
  rmSync,
} from "node:fs";
import { tmpdir } from "node:os";
import { basename, dirname, extname, join, relative } from "node:path";
import { sourceToAssemblyPlan } from "./alchemy_gcc.ts";
import {
  canonicalCSource,
  unionIntervals,
  type ExecutableInventory,
} from "./full_c_progress.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const SEMANTIC = join(ROOT, "semantic");
const OVERLAY_SOURCE_NAME = /^resource_[0-9a-f]+_c_([0-9a-f]{8})\.c$/i;
const MAIN_SOURCE_NAME = /^(08[0-9a-f]{6})\.c$/i;

interface OverlayFunction {
  id: string;
  overlay: string;
  entry: number;
  span_bytes: number;
}

interface ManualRegion {
  overlay: string;
  entry: string;
  span_bytes: number;
  evidence: string;
}

interface MainManifestRegion {
  address: number;
  size: number;
  retention: string;
}

interface MainClaimedRegion {
  address: number;
  size: number;
  source: string;
}

interface MainExecutableRange {
  address: string;
  size: number;
}

interface MainSemanticOwner {
  entry: string;
  evidence: string;
  executable_ranges: MainExecutableRange[];
}

interface ValidatedMainSemanticOwner {
  entry: number;
  evidence: string;
  executableRanges: Array<{ address: number; size: number }>;
}

function parseAddress(value: string, field: string): number {
  if (!/^0x[0-9a-f]+$/i.test(value)) {
    throw new Error(`${field} must be a hexadecimal address`);
  }
  const address = Number.parseInt(value, 16);
  if (!Number.isSafeInteger(address)) throw new Error(`${field} is not a safe integer`);
  return address;
}

export function validateMainSemanticOwners(
  owners: MainSemanticOwner[],
  manifestRegions: MainManifestRegion[],
  claimedRegions: MainClaimedRegion[] = [],
  source = "semantic/main-regions.json",
): ValidatedMainSemanticOwner[] {
  const validated: ValidatedMainSemanticOwner[] = [];
  const entries = new Set<number>();
  for (const [ownerIndex, owner] of owners.entries()) {
    const ownerField = `${source} owner ${ownerIndex}`;
    const entry = parseAddress(owner.entry, `${ownerField} entry`);
    if (entries.has(entry)) throw new Error(`${source} repeats entry ${owner.entry}`);
    entries.add(entry);
    if (typeof owner.evidence !== "string" || owner.evidence.trim() === "") {
      throw new Error(`${ownerField} has empty evidence`);
    }
    if (!Array.isArray(owner.executable_ranges) || owner.executable_ranges.length === 0) {
      throw new Error(`${ownerField} has no executable ranges`);
    }
    const executableRanges = owner.executable_ranges.map((range, rangeIndex) => {
      const rangeField = `${ownerField} range ${rangeIndex}`;
      const address = parseAddress(range.address, `${rangeField} address`);
      if (!Number.isSafeInteger(range.size) || range.size <= 0) {
        throw new Error(`${rangeField} size must be a positive safe integer`);
      }
      if (!Number.isSafeInteger(address + range.size)) {
        throw new Error(`${rangeField} end is not a safe integer`);
      }
      const manifestRegion = manifestRegions.find((item) =>
        item.address <= address &&
        address + range.size <= item.address + item.size
      );
      // A few historically byte-matched main sources are deliberately
      // noncanonical (register pins or inline asm).  They are omitted from
      // the ordinary-assembly manifest, but a reviewed semantic owner may
      // replace their coverage without promoting that source to exact C.
      const noncanonicalExactSource = join(ROOT, "src", `${owner.entry.slice(2)}.c`);
      const hasNoncanonicalExactSource = existsSync(noncanonicalExactSource) &&
        !canonicalCSource(readFileSync(noncanonicalExactSource, "utf8"));
      const expectedClaimSource = `src/${owner.entry.slice(2).toLowerCase()}.c`;
      const noncanonicalClaim = hasNoncanonicalExactSource
        ? claimedRegions.find((item) =>
          item.source.toLowerCase() === expectedClaimSource &&
          item.address <= address &&
          address + range.size <= item.address + item.size
        )
        : undefined;
      if (manifestRegion === undefined && noncanonicalClaim === undefined) {
        throw new Error(
          `${rangeField} is not fully contained in an assembly-manifest row ` +
          `or its same-entry noncanonical claimed-C row`,
        );
      }
      return { address, size: range.size };
    }).sort((left, right) => left.address - right.address);
    if (!executableRanges.some((range) =>
      range.address <= entry && entry < range.address + range.size
    )) {
      throw new Error(`${ownerField} executable ranges do not include its entry`);
    }
    for (let index = 1; index < executableRanges.length; index++) {
      const prior = executableRanges[index - 1];
      const current = executableRanges[index];
      if (current.address < prior.address + prior.size) {
        throw new Error(`${ownerField} has overlapping executable ranges`);
      }
    }
    for (const priorOwner of validated) {
      for (const range of executableRanges) {
        for (const priorRange of priorOwner.executableRanges) {
          if (range.address < priorRange.address + priorRange.size &&
              priorRange.address < range.address + range.size) {
            throw new Error(`${ownerField} overlaps another tracked main owner`);
          }
        }
      }
    }
    validated.push({ entry, evidence: owner.evidence, executableRanges });
  }
  return validated;
}

function sourcesBelow(directory: string): string[] {
  if (!existsSync(directory)) return [];
  const result: string[] = [];
  for (const entry of readdirSync(directory, { withFileTypes: true })) {
    const path = join(directory, entry.name);
    if (entry.isDirectory()) result.push(...sourcesBelow(path));
    else if (entry.isFile() && extname(entry.name) === ".c") result.push(path);
  }
  return result.sort();
}

function checked(command: readonly string[], cwd: string): void {
  const process = Bun.spawnSync([...command], { cwd, stdout: "pipe", stderr: "pipe" });
  if (process.exitCode !== 0) {
    const detail = (process.stderr.toString() || process.stdout.toString()).trim();
    throw new Error(`${basename(command[0])} failed${detail ? `: ${detail}` : ""}`);
  }
}

function validateSource(source: string): {
  kind: "main" | "overlay";
  address: number;
  owner: string;
  symbol: string;
} {
  const name = basename(source);
  const overlayMatch = OVERLAY_SOURCE_NAME.exec(name);
  const mainMatch = MAIN_SOURCE_NAME.exec(name);
  if (overlayMatch === null && mainMatch === null) {
    throw new Error(`semantic C source must use its eight-digit address: ${relative(ROOT, source)}`);
  }
  const kind = overlayMatch === null ? "main" : "overlay";
  const addressText = (overlayMatch ?? mainMatch)![1].toLowerCase();
  const owner = kind === "overlay" ? name.slice(0, name.indexOf("_c_")) : "main";
  const symbol = `Func_${addressText}`;
  const text = readFileSync(source, "utf8");
  if (!new RegExp(`\\b${symbol}\\s*\\([^;{}]*\\)\\s*\\{`).test(text)) {
    throw new Error(`${relative(ROOT, source)} does not define ${symbol}`);
  }
  if (/(^|[^A-Za-z0-9_])(__asm__|asm)\s*[(\u007b]/m.test(text)) {
    throw new Error(`${relative(ROOT, source)} contains inline assembly`);
  }
  if (/\b(M2C_ERROR|M2C_UNK|GLOBAL_ASM)\b/.test(text)) {
    throw new Error(`${relative(ROOT, source)} contains an unresolved decompiler construct`);
  }
  return { kind, address: Number.parseInt(addressText, 16), owner, symbol };
}

export function buildSemantic(directory = SEMANTIC): {
  sources: number;
  sourceBytes: number;
  reviewedBytes: number;
  outsideExecutableBytes: number;
  semanticBytes: number;
  mainSemanticBytes: number;
  overlaySemanticBytes: number;
  expressedBytes: number;
  executableBytes: number;
} {
  const sources = sourcesBelow(directory);
  const inventoryPath = join(ROOT, "out", "decomp", "overlays.json");
  if (sources.length !== 0 && !existsSync(inventoryPath)) {
    throw new Error("semantic build requires out/decomp/overlays.json; run the code-overlay inventory first");
  }
  const inventory = sources.length === 0
    ? []
    : (JSON.parse(readFileSync(inventoryPath, "utf8")) as { functions: OverlayFunction[] }).functions;
  const mainManifestPath = join(ROOT, "out", "full", "asm", "manifest.json");
  const mainManifestRegions = existsSync(mainManifestPath)
    ? (JSON.parse(readFileSync(mainManifestPath, "utf8")) as {
      regions: MainManifestRegion[];
    }).regions
    : [];
  const mainClaimedPath = join(ROOT, "out", "full", "claimed", "manifest.json");
  const mainClaimedRegions = existsSync(mainClaimedPath)
    ? (JSON.parse(readFileSync(mainClaimedPath, "utf8")) as {
      regions: MainClaimedRegion[];
    }).regions
    : [];
  const mainOwnerPath = join(SEMANTIC, "main-regions.json");
  const mainOwnerDocument = existsSync(mainOwnerPath)
    ? (JSON.parse(readFileSync(mainOwnerPath, "utf8")) as {
      format: number;
      main_owners: MainSemanticOwner[];
    })
    : { format: 1, main_owners: [] };
  if (mainOwnerDocument.format !== 1 || !Array.isArray(mainOwnerDocument.main_owners)) {
    throw new Error(`${relative(ROOT, mainOwnerPath)} has an unsupported schema`);
  }
  const mainOwners = validateMainSemanticOwners(
    mainOwnerDocument.main_owners,
    mainManifestRegions,
    mainClaimedRegions,
    relative(ROOT, mainOwnerPath),
  );
  const manualPath = join(SEMANTIC, "regions.json");
  const manual = existsSync(manualPath)
    ? (JSON.parse(readFileSync(manualPath, "utf8")) as {
      format: number;
      manual_regions: ManualRegion[];
    }).manual_regions
    : [];
  const work = mkdtempSync(join(tmpdir(), "alchemy-semantic-"));
  try {
    let sourceBytes = 0;
    let reviewedBytes = 0;
    const admitted: Array<{
      overlay: string;
      ranges: Array<{ address: number; size: number }>;
      source: string;
    }> = [];
    for (const [index, source] of sources.entries()) {
      const identity = validateSource(source);
      sourceBytes += readFileSync(source).byteLength;
      const name = basename(source);
      const address = identity.address;
      const overlay = identity.owner;
      const exactSource = identity.kind === "overlay"
        ? join(ROOT, "assets", "code", name)
        : join(ROOT, "src", name);
      if (existsSync(exactSource) && canonicalCSource(readFileSync(exactSource, "utf8"))) {
        throw new Error(`${relative(ROOT, source)} duplicates exact source ${relative(ROOT, exactSource)}`);
      }
      const inventoried = identity.kind === "overlay"
        ? inventory.find((item) => item.overlay === overlay && item.entry === address)
        : undefined;
      const reviewed = identity.kind === "overlay"
        ? manual.find((item) =>
          item.overlay === overlay && Number.parseInt(item.entry, 16) === address
        )
        : undefined;
      const mainRegion = identity.kind === "main"
        ? mainManifestRegions.find((item) => item.address === address && [
          "c_candidate", "split_first", "merge_with_continuations", "merge_with_owner",
        ].includes(item.retention))
        : undefined;
      const trackedMainOwner = identity.kind === "main"
        ? mainOwners.find((item) => item.entry === address)
        : undefined;
      const ranges = trackedMainOwner?.executableRanges ??
        (reviewed !== undefined
          // A reviewed manual boundary exists specifically to correct or
          // extend the structural inventory (for example through a complete
          // trailing literal pool). It must therefore win when both exist.
          ? [{ address, size: reviewed.span_bytes }]
          : inventoried !== undefined
            ? [{ address, size: inventoried.span_bytes }]
            : mainRegion !== undefined
              ? [{ address, size: mainRegion.size }]
              : undefined);
      if (ranges === undefined) {
        throw new Error(`${relative(ROOT, source)} has no admitted semantic owner`);
      }
      if (reviewed !== undefined &&
          (!Number.isSafeInteger(reviewed.span_bytes) || reviewed.span_bytes <= 0 ||
           reviewed.evidence.trim() === "")) {
        throw new Error(`${relative(ROOT, manualPath)} contains an invalid reviewed boundary`);
      }
      const spanBytes = ranges.reduce((sum, range) => sum + range.size, 0);
      reviewedBytes += spanBytes;
      for (const prior of admitted) {
        if (prior.overlay !== overlay) continue;
        for (const range of ranges) {
          for (const priorRange of prior.ranges) {
            if (range.address < priorRange.address + priorRange.size &&
                priorRange.address < range.address + range.size) {
              throw new Error(
                `${relative(ROOT, source)} overlaps ${relative(ROOT, prior.source)} in ${overlay}`,
              );
            }
          }
        }
      }
      admitted.push({ overlay, ranges, source });
      const stem = `${index.toString().padStart(4, "0")}-${basename(source, ".c")}`;
      const plan = sourceToAssemblyPlan({
        target: "gs1",
        routingSource: source,
        input: source,
        output: join(work, `${stem}.s`),
        preprocessedOutput: join(work, `${stem}.i`),
      });
      for (const step of plan.steps) checked(step.command, work);
    }
    const exact = JSON.parse(readFileSync(
      join(ROOT, "metrics", "gs1-en-progress.json"),
      "utf8",
    )) as { full_c_bytes: number; executable_bytes: number };
    const executable = JSON.parse(readFileSync(
      join(ROOT, "metrics", "gs1-en-executable.json"),
      "utf8",
    )) as ExecutableInventory;
    const coveredBytes = (namespace: string, intervals: Array<{ start: number; end: number }>): number => {
      const ranges = admitted
        .filter((item) => item.overlay === namespace)
        .flatMap((item) => item.ranges.map((range) => ({
          start: range.address,
          end: range.address + range.size,
        })))
        .sort((left, right) => left.start - right.start || left.end - right.end);
      let total = 0;
      for (const range of ranges) {
        for (const interval of intervals) {
          const start = Math.max(range.start, interval.start);
          const end = Math.min(range.end, interval.end);
          if (end > start) total += end - start;
        }
      }
      return total;
    };
    const mainSemanticBytes = coveredBytes("main", unionIntervals(executable.main.intervals));
    const overlaySemanticBytes = executable.overlays.reduce(
      (sum, overlay) => sum + coveredBytes(overlay.id, unionIntervals(overlay.intervals)),
      0,
    );
    const semanticBytes = mainSemanticBytes + overlaySemanticBytes;
    return {
      sources: sources.length,
      sourceBytes,
      reviewedBytes,
      outsideExecutableBytes: reviewedBytes - semanticBytes,
      semanticBytes,
      mainSemanticBytes,
      overlaySemanticBytes,
      expressedBytes: exact.full_c_bytes + semanticBytes,
      executableBytes: exact.executable_bytes,
    };
  } finally {
    rmSync(work, { recursive: true, force: true });
  }
}

function selfTest(): void {
  if (!OVERLAY_SOURCE_NAME.test("resource_379_c_020000dc.c")) throw new Error("valid overlay name rejected");
  if (!MAIN_SOURCE_NAME.test("0809a294.c")) throw new Error("valid main name rejected");
  if (OVERLAY_SOURCE_NAME.test("resource_379.c")) throw new Error("addressless source name accepted");
  const manifest = [
    { address: 0x08001000, size: 12, retention: "split_first" },
    { address: 0x08001008, size: 4, retention: "merge_with_function_owner" },
    { address: 0x08001020, size: 8, retention: "merge_with_function_owner" },
  ];
  const valid = validateMainSemanticOwners([{
    entry: "0x08001000",
    evidence: "test owner",
    executable_ranges: [
      { address: "0x08001000", size: 12 },
      { address: "0x08001020", size: 8 },
    ],
  }], manifest, [], "self-test");
  if (valid[0].executableRanges.reduce((sum, range) => sum + range.size, 0) !== 20) {
    throw new Error("noncontiguous owner byte sum rejected");
  }
  const contained = validateMainSemanticOwners([{
    entry: "0x08001000",
    evidence: "contained split",
    executable_ranges: [
      { address: "0x08001000", size: 4 },
      { address: "0x08001008", size: 4 },
    ],
  }], manifest, [], "self-test");
  if (contained[0].executableRanges.reduce((sum, range) => sum + range.size, 0) !== 8) {
    throw new Error("contained main-row split rejected");
  }
  const rejects = (owner: MainSemanticOwner, message: string): void => {
    try {
      validateMainSemanticOwners([owner], manifest, [], "self-test");
    } catch {
      return;
    }
    throw new Error(message);
  };
  rejects({
    entry: "0x08001000",
    evidence: "bad size",
    executable_ranges: [{ address: "0x08001000", size: 0 }],
  }, "non-positive main range accepted");
  rejects({
    entry: "0x08001000",
    evidence: "missing row",
    executable_ranges: [{ address: "0x08000ffc", size: 8 }],
  }, "main range crossing a manifest-row boundary accepted");
  rejects({
    entry: "0x08001030",
    evidence: "outside rows",
    executable_ranges: [{ address: "0x08001030", size: 4 }],
  }, "main range outside manifest rows accepted");
  rejects({
    entry: "0x08001000",
    evidence: "overlap",
    executable_ranges: [
      { address: "0x08001000", size: 12 },
      { address: "0x08001008", size: 4 },
    ],
  }, "overlapping main ranges accepted");
  console.log("self-test=ok");
}

if (import.meta.main) {
  if (Bun.argv[2] === "--self-test") selfTest();
  else {
    const report = buildSemantic();
    console.log(
      `semantic_sources=${report.sources} semantic_bytes=${report.semanticBytes} ` +
      `main_semantic=${report.mainSemanticBytes} overlay_semantic=${report.overlaySemanticBytes} ` +
      `reviewed_bytes=${report.reviewedBytes} outside_executable=${report.outsideExecutableBytes} ` +
      `c_expressed=${report.expressedBytes}/${report.executableBytes} ` +
      `remaining=${report.executableBytes - report.expressedBytes} ` +
      `source_bytes=${report.sourceBytes} compile=ok`,
    );
  }
}
