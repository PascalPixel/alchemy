#!/usr/bin/env bun
/**
 * Main-image retained-assembly guard.
 *
 * The orange part of the main-image map is not a hand-maintained total.  It is
 * exactly the audited executable inventory less byte-identical C and reviewed
 * semantic C.  This guard recomputes that complement and makes the two current
 * full-build manifests account for every one of its bytes.
 *
 *   bun tools/core_retained_audit.ts --check
 *   bun tools/core_retained_audit.ts --check --json
 *   bun tools/core_retained_audit.ts --self-test
 */
import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;

type Span = { start: number; end: number };
type InventoryInterval = Span & { kind: string; evidence: string };
type AsmRegion = Span & {
  source: string; kind: string; retention: string; confidence: string; evidence: string;
};
type ClaimedRegion = Span & { source: string };

export interface CoreRetainedAudit {
  format: 1;
  kind: "core-retained-complement-audit";
  status: "ok" | "failed";
  inputs: { inventory: string; semantic: string; asm_manifest: string; claimed_manifest: string };
  bytes: { executable: number; exact_c: number; semantic_c: number; retained: number };
  retained_by_kind_confidence: Array<{ kind: string; confidence: string; regions: number; bytes: number }>;
  failures: string[];
}

function json(path: string): any {
  if (!existsSync(path)) throw new Error(`missing required input: ${path}`);
  return JSON.parse(readFileSync(path, "utf8"));
}

function number(value: unknown, label: string): number {
  const parsed = typeof value === "string" ? Number(value) : value;
  if (!Number.isSafeInteger(parsed) || (parsed as number) < 0) throw new Error(`${label} must be a non-negative integer`);
  return parsed as number;
}

function span(value: any, label: string): Span {
  const start = number(value.start ?? value.address, `${label}.start`);
  const end = value.end === undefined ? start + number(value.size, `${label}.size`) : number(value.end, `${label}.end`);
  if (end <= start) throw new Error(`${label} is empty or reversed`);
  return { start, end };
}

function normalized(items: readonly Span[], label: string): Span[] {
  const sorted = [...items].sort((a, b) => a.start - b.start || a.end - b.end);
  const result: Span[] = [];
  for (const item of sorted) {
    if (item.start < ROM_BASE || item.end > ROM_BASE + 0x800000) throw new Error(`${label} lies outside the gs1-en ROM`);
    const previous = result.at(-1);
    if (!previous || item.start > previous.end) result.push({ ...item });
    else previous.end = Math.max(previous.end, item.end);
  }
  return result;
}

function bytes(items: readonly Span[]): number {
  return normalized(items, "span").reduce((total, item) => total + item.end - item.start, 0);
}

function mark(mask: Uint8Array, spans: readonly Span[], bit: number): void {
  for (const item of spans) mask.fill(bit, item.start - ROM_BASE, item.end - ROM_BASE);
}

function allCovered(mask: Uint8Array, spans: readonly Span[]): boolean {
  return spans.every((item) => !mask.subarray(item.start - ROM_BASE, item.end - ROM_BASE).some((value) => value === 0));
}

function intersections(mask: Uint8Array, selected: number): Span[] {
  const result: Span[] = [];
  let start = -1;
  for (let offset = 0; offset <= mask.length; offset++) {
    const matches = offset < mask.length && mask[offset] === selected;
    if (matches && start < 0) start = offset;
    if (!matches && start >= 0) {
      result.push({ start: ROM_BASE + start, end: ROM_BASE + offset });
      start = -1;
    }
  }
  return result;
}

function without(left: readonly Span[], right: readonly Span[]): Span[] {
  const mask = new Uint8Array(0x800000);
  mark(mask, left, 1);
  mark(mask, right, 2);
  for (let offset = 0; offset < mask.length; offset++) if (mask[offset] === 2) mask[offset] = 0;
  return intersections(mask, 1);
}

function missingCoverage(mask: Uint8Array, spans: readonly Span[]): Span[] {
  const requested = new Uint8Array(0x800000);
  mark(requested, spans, 1);
  for (let offset = 0; offset < requested.length; offset++) if (requested[offset] && mask[offset]) requested[offset] = 0;
  return intersections(requested, 1);
}

function overlap(a: Span, b: Span): Span | undefined {
  const start = Math.max(a.start, b.start), end = Math.min(a.end, b.end);
  return start < end ? { start, end } : undefined;
}

function hex(value: number): string { return `0x${value.toString(16).padStart(8, "0")}`; }

function mainInventory(path: string): InventoryInterval[] {
  const document = json(path);
  if (document.format !== 1 || document.target !== "gs1-en" || !Array.isArray(document.main?.intervals))
    throw new Error("executable inventory is not the gs1-en main-image executable inventory");
  return document.main.intervals.map((item: any, index: number) => {
    const result = span(item, `inventory.main.intervals[${index}]`);
    if (typeof item.kind !== "string" || typeof item.evidence !== "string" || !item.evidence.trim())
      throw new Error(`inventory.main.intervals[${index}] lacks kind/evidence`);
    return { ...result, kind: item.kind, evidence: item.evidence };
  });
}

function manifests(asmPath: string, claimedPath: string): { asm: AsmRegion[]; claimed: ClaimedRegion[] } {
  const asmDocument = json(asmPath), claimedDocument = json(claimedPath);
  if (!Array.isArray(asmDocument.regions) || !Array.isArray(claimedDocument.regions)) throw new Error("full manifests lack regions");
  const asm = asmDocument.regions.map((item: any, index: number) => {
    const result = span(item, `asm.regions[${index}]`);
    for (const key of ["source", "kind", "retention", "confidence", "evidence"] as const) {
      if (typeof item[key] !== "string" || !item[key].trim()) throw new Error(`asm.regions[${index}] lacks ${key}`);
    }
    return { ...result, source: item.source, kind: item.kind, retention: item.retention, confidence: item.confidence, evidence: item.evidence };
  });
  const claimed = claimedDocument.regions.map((item: any, index: number) => {
    const result = span(item, `claimed.regions[${index}]`);
    if (typeof item.source !== "string" || !item.source.startsWith("src/")) throw new Error(`claimed.regions[${index}] is not a claimed C source`);
    return { ...result, source: item.source };
  });
  return { asm, claimed };
}

function canonicalC(source: string): boolean {
  return ![/\bregister\b[^;\n]*\basm\s*\(/, /\b__asm__\b|\basm\s+volatile\b/, /\.incbin\b/, /\bM2C_ERROR\b/]
    .some((pattern) => pattern.test(source));
}

function sourceNames(directory: string): string[] {
  if (!existsSync(directory)) return [];
  const result: string[] = [];
  for (const name of readdirSync(directory, { withFileTypes: true })) {
    if (name.isDirectory()) result.push(...sourceNames(join(directory, name.name)));
    else result.push(join(directory, name.name));
  }
  return result;
}

function boundaries(): number[] {
  const addresses = new Set<number>();
  for (const path of sourceNames(join(ROOT, "src"))) {
    const match = /\/([0-9a-f]{8})\.c$/i.exec(path);
    if (match) addresses.add(Number.parseInt(match[1], 16));
  }
  for (const path of sourceNames(join(ROOT, "asm"))) {
    const match = /\/([0-9a-f]{8})\.s$/i.exec(path);
    if (match) addresses.add(Number.parseInt(match[1], 16));
  }
  const alignment = json(join(ROOT, "asm/alignment.json"));
  for (const address of alignment.addresses ?? []) addresses.add(Number(address));
  return [...addresses].sort((a, b) => a - b);
}

function regionSpan(address: number, boundaries: readonly number[], limit: number): Span {
  const next = boundaries.find((boundary) => boundary > address) ?? limit;
  return { start: address, end: next };
}

function intersect(items: readonly Span[], executable: readonly Span[]): Span[] {
  return normalized(items.flatMap((item) => executable.map((region) => overlap(item, region)).filter(Boolean) as Span[]), "intersected span");
}

function canonicalSpans(executable: readonly Span[]): {
  exact: Span[]; semantic: Span[]; nonC: Array<Span & { kind: string; evidence: string }>;
  noncanonicalSemanticClaims: ClaimedRegion[];
} {
  const limit = executable.at(-1)?.end ?? ROM_BASE;
  const starts = boundaries();
  const exact: Span[] = [];
  for (const path of sourceNames(join(ROOT, "src"))) {
    const match = /\/([0-9a-f]{8})\.c$/i.exec(path);
    if (match && canonicalC(readFileSync(path, "utf8"))) exact.push(regionSpan(Number.parseInt(match[1], 16), starts, limit));
  }
  const { semantic, nonC, claims } = semanticSpans(join(ROOT, "semantic/main-regions.json"), starts, limit, executable);
  return { exact: intersect(exact, executable), semantic, nonC, noncanonicalSemanticClaims: claims };
}

function semanticSpans(path: string, boundaries: readonly number[], limit: number, executable: readonly Span[]): {
  semantic: Span[]; nonC: Array<Span & { kind: string; evidence: string }>; claims: ClaimedRegion[];
} {
  const document = json(path);
  if (document.format !== 1 || !Array.isArray(document.main_owners)) throw new Error("semantic/main-regions.json lacks main owners");
  const semantic: Span[] = [];
  const claims: ClaimedRegion[] = [];
  const addSemantic = (entry: string, ranges: Span[]): void => {
    semantic.push(...ranges);
    // A source can be compiled into the full claimed manifest while still
    // being deliberately noncanonical (register pins/inline asm). It remains
    // reviewed semantic C, never byte-exact C; name that replacement here so
    // the claimed-manifest cross-check cannot promote it by accident.
    const claimedSource = join(ROOT, "src", `${entry.slice(2).toLowerCase()}.c`);
    if (existsSync(claimedSource) && !canonicalC(readFileSync(claimedSource, "utf8"))) {
      claims.push(...ranges.map((range) => ({ ...range, source: `src/${entry.slice(2).toLowerCase()}.c` })));
    }
  };
  for (const [index, owner] of document.main_owners.entries()) {
    if (typeof owner.entry !== "string" || !/^0x080[0-9a-f]{5}$/i.test(owner.entry)) throw new Error(`semantic owner ${index} has an invalid entry`);
    const source = join(ROOT, "semantic", "main", `${owner.entry.slice(2).toLowerCase()}.c`);
    if (!existsSync(source)) throw new Error(`semantic owner ${owner.entry} has no canonical source (${source})`);
    if (!Array.isArray(owner.executable_ranges) || owner.executable_ranges.length === 0) throw new Error(`semantic owner ${owner.entry} has no executable ranges`);
    addSemantic(owner.entry, owner.executable_ranges.map((range: any, rangeIndex: number) => span(range, `semantic owner ${index} range ${rangeIndex}`)));
  }
  const nonC = (document.non_c_ranges ?? []).map((item: any, index: number) => {
    const result = span(item, `semantic non-C range ${index}`);
    if (!["literal_pool", "alignment_padding", "lookup_table"].includes(item.kind) || typeof item.evidence !== "string" || !item.evidence.trim())
      throw new Error(`semantic non-C range ${index} is not an evidenced literal-pool/alignment/lookup span`);
    return { ...result, kind: item.kind, evidence: item.evidence };
  });
  // A source without a special multi-range registration owns its canonical
  // source boundary span.  Registered owners deliberately override that
  // default (their range can exclude a literal pool or combine fragments).
  const registered = new Set(document.main_owners.map((owner: any) => Number.parseInt(owner.entry, 16)));
  for (const path of sourceNames(join(ROOT, "semantic/main"))) {
    const match = /\/([0-9a-f]{8})\.c$/i.exec(path);
    if (!match) continue;
    const address = Number.parseInt(match[1], 16);
    if (!registered.has(address)) addSemantic(`0x${address.toString(16)}`, [regionSpan(address, boundaries, limit)]);
  }
  return { semantic: intersect(semantic, executable), nonC, claims: claims.flatMap((claim) =>
    intersect([claim], executable).map((range) => ({ ...range, source: claim.source }))), };
}

/** Audit inputs are injectable so the self-test covers byte reconciliation. */
export function auditCoreRetained(input: {
  inventory: InventoryInterval[]; exact: Span[]; semantic: Span[]; nonC: Array<Span & { kind: string; evidence: string }>;
  asm: AsmRegion[]; claimed: ClaimedRegion[]; noncanonicalSemanticClaims?: ClaimedRegion[];
  paths?: CoreRetainedAudit["inputs"];
}): CoreRetainedAudit {
  const failures: string[] = [];
  const executable = normalized(input.inventory, "executable inventory");
  const exact = normalized(input.exact, "canonical exact-C spans");
  const semantic = normalized(input.semantic, "semantic spans");
  const semanticOnly = without(semantic, exact);
  const semanticClaimMask = new Uint8Array(0x800000);
  mark(semanticClaimMask, input.noncanonicalSemanticClaims ?? [], 1);
  const mask = new Uint8Array(0x800000);
  mark(mask, executable, 1);
  // Semantic and exact C must be subsets of the audited denominator.  This is
  // deliberately checked before they are subtracted: a registry typo cannot
  // make a byte disappear from the audit.
  for (const [label, spans] of [["exact C", exact], ["semantic C", semantic]] as const) {
    if (!allCovered(mask, spans)) failures.push(`${label} contains bytes outside the tracked executable inventory`);
  }
  const owned = new Uint8Array(mask);
  mark(owned, exact, 2);
  mark(owned, semantic, 3);
  for (let i = 0; i < mask.length; i++) if (mask[i] === 1 && owned[i] !== 1) owned[i] = 0;
  const retained = intersections(owned, 1);

  const claimedMask = new Uint8Array(mask.length), asmMask = new Uint8Array(mask.length), nonCMask = new Uint8Array(mask.length);
  mark(claimedMask, input.claimed, 1); mark(asmMask, input.asm, 1); mark(nonCMask, input.nonC, 1);
  for (const [label, coverage, spans] of [
    ["canonical exact-C span absent from the current full claimed manifest", claimedMask, exact],
    ["canonical semantic-C span absent from the current full asm/approved noncanonical-claim manifest", (() => {
      const coverage = new Uint8Array(asmMask); for (let i = 0; i < coverage.length; i++) if (semanticClaimMask[i]) coverage[i] = 1; return coverage;
    })(), semanticOnly],
    ["retained complement absent from the current full asm manifest", asmMask, retained],
  ] as const) {
    const missing = missingCoverage(coverage, spans);
    if (missing.length) failures.push(...missing.slice(0, 20).map((part) => `${label} at ${hex(part.start)}..${hex(part.end)}`));
    if (missing.length > 20) failures.push(`${label}: ${missing.length - 20} further spans omitted`);
  }

  // A C fragment in executable space needs a matching canonical exact-C
  // inventory span; compilation alone is not admission to the numerator.
  for (const region of input.claimed) {
    for (const part of executable.map((item) => overlap(region, item)).filter(Boolean) as Span[]) {
      const exactCoverage = allCovered(new Uint8Array(exactMask(exact)), [part]);
      const approvedSemanticReplacement = allCovered(semanticClaimMask, [part]) &&
        (input.noncanonicalSemanticClaims ?? []).some((claim) => claim.source === region.source && overlap(claim, part));
      if (!exactCoverage && !approvedSemanticReplacement)
        failures.push(`unsupported claimed-C fragment ${region.source} at ${hex(part.start)}..${hex(part.end)}`);
    }
  }

  const totals = new Map<string, { kind: string; confidence: string; regions: number; bytes: number }>();
  for (const region of input.asm) {
    for (const part of retained.map((item) => overlap(region, item)).filter(Boolean) as Span[]) {
      const permittedDirective = allCovered(nonCMask, [part]);
      if ((region.retention === "c_candidate" || region.kind === "compiler_output") && !permittedDirective) {
        failures.push(`ordinary ${region.retention}/${region.kind} retained code at ${hex(part.start)}..${hex(part.end)} (${region.source})`);
      }
      const key = `${region.kind}\u0000${region.confidence}`;
      const row = totals.get(key) ?? { kind: region.kind, confidence: region.confidence, regions: 0, bytes: 0 };
      row.regions++; row.bytes += part.end - part.start; totals.set(key, row);
    }
  }
  return {
    format: 1, kind: "core-retained-complement-audit", status: failures.length ? "failed" : "ok",
    inputs: input.paths ?? { inventory: "", semantic: "", asm_manifest: "", claimed_manifest: "" },
    bytes: { executable: bytes(executable), exact_c: bytes(exact), semantic_c: bytes(semanticOnly), retained: bytes(retained) },
    retained_by_kind_confidence: [...totals.values()].sort((a, b) => a.kind.localeCompare(b.kind) || a.confidence.localeCompare(b.confidence)),
    failures: [...new Set(failures)],
  };
}

function exactMask(spans: readonly Span[]): Uint8Array { const result = new Uint8Array(0x800000); mark(result, spans, 1); return result; }

function selfTest(): void {
  const inventory: InventoryInterval[] = [
    { start: ROM_BASE, end: ROM_BASE + 12, kind: "thumb", evidence: "asm/a.s:ordinary" },
    { start: ROM_BASE + 12, end: ROM_BASE + 16, kind: "thumb", evidence: "src/a.c:byte-identical-claimed-C" },
  ];
  const asm: AsmRegion[] = [{ start: ROM_BASE, end: ROM_BASE + 12, source: "asm/a.s", kind: "retained", retention: "keep_asm", confidence: "proven", evidence: "proof" }];
  const claimed: ClaimedRegion[] = [{ start: ROM_BASE + 12, end: ROM_BASE + 16, source: "src/a.c" }];
  const ok = auditCoreRetained({ inventory, exact: [{ start: ROM_BASE + 12, end: ROM_BASE + 16 }], semantic: [], nonC: [], asm, claimed });
  if (ok.status !== "ok" || ok.bytes.retained !== 12 || ok.retained_by_kind_confidence[0]?.bytes !== 12) throw new Error("core retained audit self-test baseline failed");
  const bad = auditCoreRetained({ inventory, exact: [{ start: ROM_BASE + 12, end: ROM_BASE + 16 }], semantic: [], nonC: [], asm: [{ ...asm[0], retention: "c_candidate", kind: "compiler_output" }], claimed });
  if (bad.status !== "failed" || !bad.failures.some((failure) => failure.includes("ordinary c_candidate"))) throw new Error("core retained audit self-test candidate rejection failed");
  const unsupported = auditCoreRetained({ inventory, exact: [{ start: ROM_BASE + 12, end: ROM_BASE + 16 }], semantic: [], nonC: [], asm, claimed: [{ start: ROM_BASE + 8, end: ROM_BASE + 16, source: "src/a.c" }] });
  if (unsupported.status !== "failed" || !unsupported.failures.some((failure) => failure.includes("unsupported claimed-C"))) throw new Error("core retained audit self-test claimed rejection failed");
  const semanticReplacement = auditCoreRetained({
    inventory, exact: [{ start: ROM_BASE + 12, end: ROM_BASE + 16 }], semantic: [{ start: ROM_BASE, end: ROM_BASE + 12 }], nonC: [], asm: [],
    claimed: [
      { start: ROM_BASE, end: ROM_BASE + 12, source: "src/noncanonical.c" },
      { start: ROM_BASE + 12, end: ROM_BASE + 16, source: "src/exact.c" },
    ],
    noncanonicalSemanticClaims: [{ start: ROM_BASE, end: ROM_BASE + 12, source: "src/noncanonical.c" }],
  });
  if (semanticReplacement.status !== "ok") {
    throw new Error("core retained audit self-test noncanonical semantic replacement failed");
  }
  for (const kind of ["literal_pool", "alignment_padding", "lookup_table"]) {
    const permitted = auditCoreRetained({
      inventory, exact: [{ start: ROM_BASE + 12, end: ROM_BASE + 16 }], semantic: [],
      nonC: [{ start: ROM_BASE, end: ROM_BASE + 12, kind, evidence: "explicit directive" }],
      asm: [{ ...asm[0], retention: "c_candidate", kind: "compiler_output" }], claimed,
    });
    if (permitted.status !== "ok") throw new Error(`core retained audit self-test ${kind} exemption failed`);
  }
  console.log("self-test=ok");
}

function main(): void {
  const argv = Bun.argv.slice(2);
  if (argv.length === 1 && argv[0] === "--self-test") return selfTest();
  if (argv.some((argument) => !["--check", "--json"].includes(argument)) || !argv.includes("--check"))
    throw new Error("usage: core_retained_audit.ts --check [--json] | --self-test");
  const paths = {
    inventory: join(ROOT, "metrics/gs1-en-executable.json"), semantic: join(ROOT, "semantic/main-regions.json"),
    asm_manifest: join(ROOT, "out/full/asm/manifest.json"), claimed_manifest: join(ROOT, "out/full/claimed/manifest.json"),
  };
  const inventory = mainInventory(paths.inventory);
  const result = auditCoreRetained({ inventory, ...canonicalSpans(normalized(inventory, "executable inventory")), ...manifests(paths.asm_manifest, paths.claimed_manifest), paths });
  if (argv.includes("--json")) console.log(JSON.stringify(result, null, 2));
  else {
    console.log(`status=${result.status} executable=${result.bytes.executable} exact_c=${result.bytes.exact_c} semantic_c=${result.bytes.semantic_c} retained=${result.bytes.retained}`);
    for (const row of result.retained_by_kind_confidence) console.log(`${row.kind}\t${row.confidence}\tregions=${row.regions}\tbytes=${row.bytes}`);
    for (const failure of result.failures) console.error(`FAIL ${failure}`);
  }
  if (result.status !== "ok") process.exitCode = 1;
}

if (import.meta.main) main();
