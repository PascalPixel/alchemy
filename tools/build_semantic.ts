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
const SOURCE_NAME = /^resource_[0-9a-f]+_c_([0-9a-f]{8})\.c$/i;

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

function validateSource(source: string): void {
  const name = basename(source);
  const match = SOURCE_NAME.exec(name);
  if (match === null) {
    throw new Error(`semantic overlay source must end in an address: ${relative(ROOT, source)}`);
  }
  const text = readFileSync(source, "utf8");
  const symbol = `Func_${match[1].toLowerCase()}`;
  if (!new RegExp(`\\b${symbol}\\s*\\([^;{}]*\\)\\s*\\{`).test(text)) {
    throw new Error(`${relative(ROOT, source)} does not define ${symbol}`);
  }
  if (/(^|[^A-Za-z0-9_])(__asm__|asm)\s*[(\u007b]/m.test(text)) {
    throw new Error(`${relative(ROOT, source)} contains inline assembly`);
  }
}

export function buildSemantic(directory = SEMANTIC): {
  sources: number;
  sourceBytes: number;
  semanticBytes: number;
} {
  const sources = sourcesBelow(directory);
  const inventoryPath = join(ROOT, "out", "decomp", "overlays.json");
  if (sources.length !== 0 && !existsSync(inventoryPath)) {
    throw new Error("semantic build requires out/decomp/overlays.json; run the overlay inventory first");
  }
  const inventory = sources.length === 0
    ? []
    : (JSON.parse(readFileSync(inventoryPath, "utf8")) as { functions: OverlayFunction[] }).functions;
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
    for (const [index, source] of sources.entries()) {
      validateSource(source);
      sourceBytes += readFileSync(source).byteLength;
      const name = basename(source);
      const address = Number.parseInt(SOURCE_NAME.exec(name)![1], 16);
      SOURCE_NAME.lastIndex = 0;
      const overlay = name.slice(0, name.indexOf("_c_"));
      const inventoried = inventory.find((item) => item.overlay === overlay && item.entry === address);
      const reviewed = manual.find((item) =>
        item.overlay === overlay && Number.parseInt(item.entry, 16) === address
      );
      const spanBytes = inventoried?.span_bytes ?? reviewed?.span_bytes;
      if (spanBytes === undefined) {
        throw new Error(`${relative(ROOT, source)} has no ordinary overlay inventory owner`);
      }
      if (reviewed !== undefined &&
          (!Number.isSafeInteger(reviewed.span_bytes) || reviewed.span_bytes <= 0 ||
           reviewed.evidence.trim() === "")) {
        throw new Error(`${relative(ROOT, manualPath)} contains an invalid reviewed boundary`);
      }
      semanticBytes += spanBytes;
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
    return { sources: sources.length, sourceBytes, semanticBytes };
  } finally {
    rmSync(work, { recursive: true, force: true });
  }
}

function selfTest(): void {
  if (!SOURCE_NAME.test("resource_379_c_020000dc.c")) throw new Error("valid source name rejected");
  SOURCE_NAME.lastIndex = 0;
  if (SOURCE_NAME.test("resource_379.c")) throw new Error("addressless source name accepted");
  console.log("self-test=ok");
}

if (import.meta.main) {
  if (Bun.argv[2] === "--self-test") selfTest();
  else {
    const report = buildSemantic();
    console.log(
      `semantic_sources=${report.sources} semantic_bytes=${report.semanticBytes} ` +
      `source_bytes=${report.sourceBytes} compile=ok`,
    );
  }
}
