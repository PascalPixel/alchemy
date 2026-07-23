#!/usr/bin/env bun
// Tool role: entrypoint; invoked by PLAYBOOK.md.
// Authoritative C-debt queue and structural-family report. This consumes only
// repository assembly claims and local compiler diagnostics; generated reports
// remain below ignored out/.
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";
import { canonicalJson } from "./canonical_json.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const DEFAULT_MANIFEST = join(ROOT, "out/full/asm/manifest.json");
const DEFAULT_QUEUE = join(ROOT, "out/decomp/queue.json");
const DEFAULT_OUTPUT = join(ROOT, "out/decomp/families.json");

interface Region {
  address: number;
  size: number;
  source: string;
  retention: string;
  confidence?: string;
}

interface Diagnosis {
  byte_mismatches: number;
  actual_size: number;
  expected_size: number;
  dominant: string;
}

interface QueueDiagnosis { stem: string; diagnosis: Diagnosis; expected_value?: number }

interface DebtItem {
  stem: string;
  address: number;
  size: number;
  source: string;
  exact_family: string;
  structural_family: string;
  exact_family_size: number;
  structural_family_size: number;
  branches: number;
  calls: number;
  executable_function: boolean;
  complexity: number;
  candidate: string | null;
  diagnosis: Diagnosis | null;
  signature_confidence: "candidate" | "unknown";
  lane: "reuse" | "family" | "research";
  estimated_cpu_hours: number;
  expected_conversions_per_cpu_hour: number;
  score: number;
}

function argumentsOf(argv: string[]): { top: number; output: string; selfTest: boolean } {
  let top = 10, output = DEFAULT_OUTPUT, selfTest = false;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--top") top = Number(argv[++index]);
    else if (argument === "--output" || argument === "-o") output = argv[++index];
    else if (argument === "--self-test") selfTest = true;
    else if (argument === "-h" || argument === "--help") {
      console.log("usage: decomp_families.ts [--top N] [-o OUTPUT] [--self-test]");
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  if (!Number.isInteger(top) || top < 1) throw new Error("top must be a positive integer");
  return { top, output, selfTest };
}

function hash(value: string): string {
  return Bun.hash(value).toString(36);
}

function instructions(source: string): string[] {
  const result: string[] = [];
  for (const raw of source.split(/\r?\n/)) {
    const line = raw.replace(/@.*$/, "").trim();
    if (!line || line.startsWith(".") || line.endsWith(":")) continue;
    const match = /^([a-z][a-z0-9.]*)\s*(.*)$/i.exec(line);
    if (match) result.push(`${match[1].toLowerCase()} ${match[2].trim()}`.trim());
  }
  return result;
}

function fingerprint(lines: string[], structural: boolean): string {
  const registers = new Map<string, string>();
  let next = 0;
  return lines.map((line) => line
    .replace(/\b(?:r(?:1[0-2]|[0-9])|sl|fp|ip)\b/gi, (register) => {
      const key = register.toLowerCase();
      if (!registers.has(key)) registers.set(key, `v${next++}`);
      return registers.get(key)!;
    })
    .replace(/\.L[\w.$]+/g, "LABEL")
    .replace(/Func_[0-9a-f]+/gi, structural ? "FUNC" : "FUNC")
    .replace(/Data_[0-9a-f]+/gi, structural ? "DATA" : "DATA")
    .replace(/#(?:-?0x[0-9a-f]+|-?\d+)/gi, structural ? "#IMM" : (value) => value.toLowerCase())
    .replace(/\b0x[0-9a-f]+\b/gi, structural ? "CONST" : (value) => value.toLowerCase())
    .replace(/\s+/g, " ")
  ).join("\n");
}

function diagnosisMap(): Map<string, QueueDiagnosis> {
  if (!existsSync(DEFAULT_QUEUE)) return new Map();
  const report = JSON.parse(readFileSync(DEFAULT_QUEUE, "utf8")) as { items?: QueueDiagnosis[] };
  return new Map((report.items ?? []).map((item) => [item.stem, item]));
}

function candidateFor(stem: string): string | null {
  const options = [
    join(ROOT, "out/decomp/candidates", `${stem}.c`),
    join(ROOT, "out/brute/best", `${stem}.c`),
    join(ROOT, "work/hand", stem, `${stem}.c`),
  ];
  return options.find(existsSync)?.slice(ROOT.length + 1) ?? null;
}

function report(): { format: 1; generated_at: string; totals: Record<string, number>; families: object[]; queue: DebtItem[] } {
  if (!existsSync(DEFAULT_MANIFEST)) throw new Error("run bun tools/build_full.ts before building the debt queue");
  const manifest = JSON.parse(readFileSync(DEFAULT_MANIFEST, "utf8")) as { regions: Region[] };
  const regions = manifest.regions.filter((region) => region.retention === "c_candidate");
  const parsed = regions.map((region) => {
    const path = join(ROOT, region.source);
    const stem = basename(region.source, ".s");
    const code = instructions(readFileSync(path, "utf8"));
    const executableFunction = code.length > 0 && !region.source.includes("/executable_gaps/");
    return {
      region, stem, code,
      executableFunction,
      exact: executableFunction ? hash(fingerprint(code, false)) : `fragment:${stem}`,
      structural: executableFunction ? hash(fingerprint(code, true)) : `fragment:${stem}`,
      branches: code.filter((line) => /^(?:b(?:eq|ne|lt|gt|le|ge|hi|ls|cc|cs|mi|pl|vs|vc)?)(?:\.n)?\b/.test(line)).length,
      calls: code.filter((line) => /^bl\b/.test(line)).length,
    };
  });
  const exactCounts = new Map<string, number>();
  const structuralCounts = new Map<string, number>();
  for (const item of parsed) {
    exactCounts.set(item.exact, (exactCounts.get(item.exact) ?? 0) + 1);
    structuralCounts.set(item.structural, (structuralCounts.get(item.structural) ?? 0) + 1);
  }
  const diagnoses = diagnosisMap();
  const queue: DebtItem[] = parsed.map((item) => {
    const exactFamilySize = exactCounts.get(item.exact)!;
    const structuralFamilySize = structuralCounts.get(item.structural)!;
    const diagnostic = diagnoses.get(item.stem)?.diagnosis ?? null;
    const candidate = candidateFor(item.stem);
    const mismatchRatio = diagnostic ? diagnostic.byte_mismatches / Math.max(1, item.region.size) : 1;
    const complexity = 1 + item.branches * 1.8 + item.calls * 1.3 + item.region.size / 48;
    const reusable = item.executableFunction && candidate !== null && diagnostic !== null &&
      (diagnostic.byte_mismatches <= 12 || mismatchRatio <= 0.08);
    const lane: DebtItem["lane"] = reusable ? "reuse" : item.executableFunction && structuralFamilySize > 1 ? "family" : "research";
    const familyLeverage = Math.max(exactFamilySize, structuralFamilySize * 0.75);
    const estimatedCpuHours = Math.max(0.03, complexity * (0.035 + mismatchRatio * 0.16) / Math.sqrt(familyLeverage));
    const throughput = familyLeverage / estimatedCpuHours;
    const closeness = diagnostic ? 160 / (1 + diagnostic.byte_mismatches) : 0;
    const score = throughput * 12 + closeness + exactFamilySize * 45 + structuralFamilySize * 18 - complexity * 3;
    return {
      stem: item.stem, address: item.region.address, size: item.region.size, source: item.region.source,
      exact_family: item.exact, structural_family: item.structural,
      exact_family_size: exactFamilySize, structural_family_size: structuralFamilySize,
      branches: item.branches, calls: item.calls, executable_function: item.executableFunction, complexity,
      candidate, diagnosis: diagnostic, signature_confidence: candidate ? "candidate" : "unknown",
      lane, estimated_cpu_hours: estimatedCpuHours,
      expected_conversions_per_cpu_hour: throughput, score,
    };
  }).sort((left, right) => right.score - left.score || left.address - right.address);
  const grouped = new Map<string, DebtItem[]>();
  for (const item of queue) {
    if (!grouped.has(item.structural_family)) grouped.set(item.structural_family, []);
    grouped.get(item.structural_family)!.push(item);
  }
  const families = [...grouped.entries()].map(([id, members]) => ({
    id, members: members.map((item) => item.stem), count: members.length,
    bytes: members.reduce((sum, item) => sum + item.size, 0),
    exact_subfamilies: new Set(members.map((item) => item.exact_family)).size,
    lane: members.some((item) => item.lane === "reuse") ? "reuse" : members.length > 1 ? "family" : "research",
    score: members.reduce((sum, item) => sum + item.score, 0),
    expected_conversions_per_cpu_hour: members.reduce((sum, item) => sum + item.expected_conversions_per_cpu_hour, 0),
  })).sort((left, right) => right.score - left.score);
  return {
    format: 1, generated_at: new Date().toISOString(),
    totals: {
      regions: queue.length, bytes: queue.reduce((sum, item) => sum + item.size, 0),
      exact_duplicate_regions: queue.filter((item) => item.exact_family_size > 1).length,
      structural_family_regions: queue.filter((item) => item.structural_family_size > 1).length,
      reuse: queue.filter((item) => item.lane === "reuse").length,
      family: queue.filter((item) => item.lane === "family").length,
      research: queue.filter((item) => item.lane === "research").length,
    },
    families, queue,
  };
}

function selfTest(): void {
  const a = instructions(".thumb\nfoo:\n adds r3, r1, #4 @ note\n bne.n .L1\n.L1:\n bx lr\n");
  const b = instructions(".thumb\nbar:\n adds r7, r2, #8\n bne.n .L9\n.L9:\n bx lr\n");
  if (fingerprint(a, true) !== fingerprint(b, true)) throw new Error("structural normalization self-test failed");
  if (fingerprint(a, false) === fingerprint(b, false)) throw new Error("exact normalization self-test failed");
  console.log("decomp_families self-test passed");
}

const options = argumentsOf(Bun.argv.slice(2));
if (options.selfTest) selfTest();
else {
  const value = report();
  mkdirSync(dirname(options.output), { recursive: true });
  writeFileSync(options.output, canonicalJson(value) + "\n");
  console.log(`regions=${value.totals.regions} bytes=${value.totals.bytes} reuse=${value.totals.reuse} family=${value.totals.family} research=${value.totals.research}`);
  for (const family of value.families.slice(0, options.top) as Array<{ id: string; members: string[]; count: number; bytes: number; lane: string; expected_conversions_per_cpu_hour: number }>) {
    console.log(`${family.id}\tlane=${family.lane}\tcount=${family.count}\tbytes=${family.bytes}\tyield=${family.expected_conversions_per_cpu_hour.toFixed(1)}\t${family.members.join(",")}`);
  }
  console.log(`report=${options.output}`);
}
