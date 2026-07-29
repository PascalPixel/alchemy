#!/usr/bin/env bun
// Compile the speed-lane semantic reconstructions without placing their output
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
  semanticBytes: number;
  mainSemanticBytes: number;
  overlaySemanticBytes: number;
  expressedBytes: number;
  executableBytes: number;
} {
  const sources = sourcesBelow(directory);
  const inventoryPath = join(ROOT, "out", "decomp", "overlays.json");
  if (sources.length !== 0 && !existsSync(inventoryPath)) {
    throw new Error("semantic build requires out/decomp/overlays.json; run the overlay inventory first");
  }
  const inventory = sources.length === 0
    ? []
    : (JSON.parse(readFileSync(inventoryPath, "utf8")) as { functions: OverlayFunction[] }).functions;
  const mainManifestPath = join(ROOT, "out", "full", "asm", "manifest.json");
  const mainRegions = sources.length === 0
    ? []
    : (JSON.parse(readFileSync(mainManifestPath, "utf8")) as {
      regions: Array<{ address: number; size: number; retention: string }>;
    }).regions;
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
    let semanticBytes = 0;
    let mainSemanticBytes = 0;
    let overlaySemanticBytes = 0;
    const admitted: Array<{ overlay: string; address: number; span: number; source: string }> = [];
    for (const [index, source] of sources.entries()) {
      const identity = validateSource(source);
      sourceBytes += readFileSync(source).byteLength;
      const name = basename(source);
      const address = identity.address;
      const overlay = identity.owner;
      const exactSource = identity.kind === "overlay"
        ? join(ROOT, "assets", "code", name)
        : join(ROOT, "src", name);
      if (existsSync(exactSource)) {
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
        ? mainRegions.find((item) => item.address === address && [
          "c_candidate", "split_first", "merge_with_continuations", "merge_with_owner",
        ].includes(item.retention))
        : undefined;
      const spanBytes = inventoried?.span_bytes ?? reviewed?.span_bytes ?? mainRegion?.size;
      if (spanBytes === undefined) {
        throw new Error(`${relative(ROOT, source)} has no ordinary overlay inventory owner`);
      }
      if (reviewed !== undefined &&
          (!Number.isSafeInteger(reviewed.span_bytes) || reviewed.span_bytes <= 0 ||
           reviewed.evidence.trim() === "")) {
        throw new Error(`${relative(ROOT, manualPath)} contains an invalid reviewed boundary`);
      }
      semanticBytes += spanBytes;
      if (identity.kind === "main") mainSemanticBytes += spanBytes;
      else overlaySemanticBytes += spanBytes;
      for (const prior of admitted) {
        if (prior.overlay !== overlay) continue;
        if (address < prior.address + prior.span && prior.address < address + spanBytes) {
          throw new Error(
            `${relative(ROOT, source)} overlaps ${relative(ROOT, prior.source)} in ${overlay}`,
          );
        }
      }
      admitted.push({ overlay, address, span: spanBytes, source });
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
    return {
      sources: sources.length,
      sourceBytes,
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
  console.log("self-test=ok");
}

if (import.meta.main) {
  if (Bun.argv[2] === "--self-test") selfTest();
  else {
    const report = buildSemantic();
    console.log(
      `semantic_sources=${report.sources} semantic_bytes=${report.semanticBytes} ` +
      `main_semantic=${report.mainSemanticBytes} overlay_semantic=${report.overlaySemanticBytes} ` +
      `c_expressed=${report.expressedBytes}/${report.executableBytes} ` +
      `remaining=${report.executableBytes - report.expressedBytes} ` +
      `source_bytes=${report.sourceBytes} compile=ok`,
    );
  }
}
