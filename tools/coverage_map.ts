#!/usr/bin/env bun
// ROM coverage map: a treemap of the audited GS1-EN image, drawn as a dark
// product-dashboard card for the README.
//
// The map answers one question visually: of every byte in the English ROM,
// which bytes are already expressed as byte-exact C, which are expressed as
// reviewed semantic C, and which are
// still assembly or non-code asset data.
//
// Exact and semantic ownership are derived from tracked evidence without a ROM
// read or compiler. The orange retained-assembly layer additionally consumes
// the latest verified full-build assembly manifest; when that manifest is
// absent, only explicit tracked non-code ranges are orange and the rest stays
// gray. Publish the map after `bun run verify`, never from stale build output:
//
//   * executable classification  metrics/<target>-executable.json
//   * exact main ownership       src/<address>.c against audited region bounds
//   * exact overlay ownership    assets/code/*_overlay.s AlchemyC placeholders
//   * semantic ownership         semantic/ sources and their tracked spans
//   * ROM layout                 assets/manifest.json addresses and sizes
//
// The derived exact totals are reconciled against the tracked Full-C report
// before anything is written: a mismatch is an error, never a rounded picture.
import { existsSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { createHash } from "node:crypto";
import { basename, dirname, join } from "node:path";
import { canonicalJson } from "./canonical_json.ts";
import {
  DEFAULT_TARGET,
  decompTarget,
  parseDecompTarget,
  type DecompTargetId,
} from "./decomp_targets.ts";
import {
  canonicalCSource,
  roundHalfUpPercent,
  unionIntervals,
  type ExecutableInventory,
  type ProgressReport,
} from "./full_c_progress.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;
const MAIN_SOURCE = /^(08[0-9a-f]{6})\.c$/i;
const ASSEMBLY_SOURCE = /^([0-9a-f]{8})\.s$/i;
const SEMANTIC_OVERLAY_SOURCE = /^(resource_[0-9a-f]+)_c_([0-9a-f]{8})\.c$/i;
const OVERLAY_ASSEMBLY = /^(resource_[0-9a-f]+)_overlay\.s$/i;
const OVERLAY_SERIES = "golden-sun-thumb-overlay-series";

export type CoverageCategory = "exact_c" | "semantic_c" | "assembly" | "retained_asm" | "asset_data";
export type AssetMaturityCategory =
  | "asset_objects"
  | "asset_color"
  | "asset_bw"
  | "asset_bytes"
  | "asset_unclassified";
const RETAINED_ASM_FILL = "#ff8a00";

// CoverageCategory order is also the stacking order inside a tile: exact at the bottom.
// `ink` is the label colour a tile takes when that category fills most of it.
const CATEGORY_STYLE: Record<CoverageCategory, { fill: string; ink: string; label: string }> = {
  exact_c: { fill: "#0072f5", ink: "#eaf2ff", label: "byte-exact C" },
  semantic_c: { fill: "#50e3c2", ink: "#04241d", label: "semantic C" },
  assembly: { fill: "#333333", ink: "#a1a1a1", label: "assembly" },
  retained_asm: { fill: RETAINED_ASM_FILL, ink: "#2b1600", label: "permanent asm" },
  asset_data: { fill: "#ff0080", ink: "#2b0016", label: "data / assets" },
};
const CATEGORY_ORDER: CoverageCategory[] = ["exact_c", "semantic_c", "assembly", "retained_asm", "asset_data"];

export interface Span {
  start: number;
  end: number;
}

/** One drawable leaf: a byte run of the image with its category composition. */
export interface Tile {
  label: string;
  bytes: number;
  categories: Partial<Record<CoverageCategory | AssetMaturityCategory, number>>;
  /** Logical folders for the SpaceMonger-style dashboard projection. */
  group?: string;
  subgroup?: string;
  /** Physical ROM/RAM address used to assign honest address-bank owners. */
  address?: number;
}

export interface Area {
  id: string;
  label: string;
  bytes: number;
  categories: Partial<Record<CoverageCategory, number>>;
  tiles: Tile[];
}

export interface CoverageMap {
  format: 1;
  kind: "golden-sun-rom-coverage-map";
  target: DecompTargetId;
  derivation: "tracked-evidence-v1";
  rom_bytes: number;
  executable_bytes: number;
  categories: Record<CoverageCategory, { bytes: number; percent_of_executable: number }>;
  main: { executable_bytes: number; exact_c_bytes: number; semantic_c_bytes: number };
  overlays: { executable_bytes: number; exact_c_bytes: number; semantic_c_bytes: number };
  provenance: {
    exact_source: string;
    semantic_source: string;
    semantic_sources: number;
    semantic_superseded_bytes: number;
    semantic_outside_extent_bytes: number;
    main_semantic_census: "closed" | "open";
    semantic_unresolved: string[];
  };
  rom_areas: Area[];
  executable_areas: Area[];
}

/**
 * The tracked document is a summary: category totals and one row per area. Tile
 * geometry is a drawing detail that the SVG already carries, and keeping it
 * out of the tree keeps a per-cycle regeneration to a few changed numbers.
 */
export function trackedDocument(map: CoverageMap): unknown {
  const rows = (areas: readonly Area[]) =>
    areas.map((item) => ({
      id: item.id,
      label: item.label,
      bytes: item.bytes,
      categories: item.categories,
      tiles: item.tiles.length,
    }));
  return {
    ...map,
    rom_areas: rows(map.rom_areas),
    executable_areas: rows(map.executable_areas),
  };
}

// ---------------------------------------------------------------- intervals

export function spanBytes(spans: readonly Span[]): number {
  return spans.reduce((sum, span) => sum + span.end - span.start, 0);
}

export function normalize(spans: readonly Span[]): Span[] {
  const sorted = [...spans].filter((span) => span.end > span.start)
    .sort((left, right) => left.start - right.start || left.end - right.end);
  const result: Span[] = [];
  for (const span of sorted) {
    const previous = result.at(-1);
    if (previous && span.start <= previous.end) previous.end = Math.max(previous.end, span.end);
    else result.push({ ...span });
  }
  return result;
}

export function intersect(spans: readonly Span[], mask: readonly Span[]): Span[] {
  const result: Span[] = [];
  const left = normalize(spans);
  const right = normalize(mask);
  let index = 0;
  for (const span of left) {
    while (index > 0 && right[index - 1].end > span.start) index--;
    while (index < right.length && right[index].end <= span.start) index++;
    for (let cursor = index; cursor < right.length && right[cursor].start < span.end; cursor++) {
      const start = Math.max(span.start, right[cursor].start);
      const end = Math.min(span.end, right[cursor].end);
      if (end > start) result.push({ start, end });
    }
  }
  return normalize(result);
}

export function subtract(spans: readonly Span[], removed: readonly Span[]): Span[] {
  const result: Span[] = [];
  const cuts = normalize(removed);
  for (const span of normalize(spans)) {
    let start = span.start;
    for (const cut of cuts) {
      if (cut.end <= start) continue;
      if (cut.start >= span.end) break;
      if (cut.start > start) result.push({ start, end: Math.min(cut.start, span.end) });
      start = Math.max(start, cut.end);
    }
    if (start < span.end) result.push({ start, end: span.end });
  }
  return normalize(result);
}

// ------------------------------------------------------------- source trees

/** A read-only view of tracked files, backed by the work tree or a git ref. */
export interface SourceTree {
  readonly id: string;
  list(directory: string): string[];
  read(path: string): string | undefined;
}

function git(args: string[]): { text: string; ok: boolean } {
  const result = Bun.spawnSync(["git", ...args], { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  return { text: result.stdout.toString(), ok: result.exitCode === 0 };
}

export function workTree(root = ROOT): SourceTree {
  return {
    id: "worktree",
    list(directory) {
      const path = join(root, directory);
      return existsSync(path) ? readdirSync(path).sort() : [];
    },
    read(path) {
      const full = join(root, path);
      return existsSync(full) ? readFileSync(full, "utf8") : undefined;
    },
  };
}

export function refTree(ref: string): SourceTree | undefined {
  const resolved = git(["rev-parse", "--verify", "--quiet", `${ref}^{commit}`]);
  if (!resolved.ok) return undefined;
  const listing = git(["ls-tree", "-r", "--name-only", ref]);
  if (!listing.ok) return undefined;
  const paths = listing.text.split("\n").filter(Boolean);
  // Every path segment is registered against its parent, so a directory lists
  // its subdirectories as well as its files. `readdirSync` behaves that way and
  // callers that walk a tree (mainBoundaries over `asm/`) depend on it: listing
  // files only would silently stop the walk at the first level and drop the
  // boundary addresses that size every main-image region.
  const entries = new Map<string, Set<string>>();
  for (const path of paths) {
    const segments = path.split("/");
    for (let index = 0; index < segments.length; index++) {
      const parent = segments.slice(0, index).join("/");
      if (!entries.has(parent)) entries.set(parent, new Set());
      entries.get(parent)!.add(segments[index]);
    }
  }
  return {
    id: ref,
    list: (directory) => [...(entries.get(directory.replace(/\/$/, "")) ?? [])].sort(),
    read(path) {
      const blob = git(["show", `${ref}:${path}`]);
      return blob.ok ? blob.text : undefined;
    },
  };
}

function readJson(tree: SourceTree, path: string): any {
  const text = tree.read(path);
  if (text === undefined) throw new Error(`${tree.id} is missing ${path}`);
  return JSON.parse(text);
}

// -------------------------------------------------------- region boundaries

/**
 * Audited main-image region starts. A region owns bytes from its own address
 * up to the next region start, which is how `docs/full-c-history.json` maps
 * tracked ownership onto the executable classification. Alignment padding is
 * a region of its own, so it never inflates the function in front of it.
 */
export function mainBoundaries(tree: SourceTree): number[] {
  const addresses = new Set<number>();
  for (const name of tree.list("src")) {
    const match = MAIN_SOURCE.exec(name);
    if (match) addresses.add(Number.parseInt(match[1], 16));
  }
  const walk = (directory: string): void => {
    for (const name of tree.list(directory)) {
      const match = ASSEMBLY_SOURCE.exec(name);
      if (match) addresses.add(Number.parseInt(match[1], 16));
      else if (!name.includes(".")) walk(`${directory}/${name}`);
    }
  };
  walk("asm");
  const alignment = tree.read("asm/alignment.json");
  if (alignment !== undefined) {
    for (const address of JSON.parse(alignment).addresses ?? []) {
      addresses.add(Number.parseInt(address, 16));
    }
  }
  return [...addresses].sort((left, right) => left - right);
}

export function regionSpan(address: number, boundaries: readonly number[], limit: number): Span {
  let low = 0;
  let high = boundaries.length;
  while (low < high) {
    const middle = (low + high) >> 1;
    if (boundaries[middle] <= address) low = middle + 1;
    else high = middle;
  }
  return { start: address, end: low < boundaries.length ? boundaries[low] : limit };
}

/** Byte-exact C ownership in the main image, keyed by source address. */
export function exactMainSpans(
  tree: SourceTree,
  executable: readonly Span[],
): Map<number, Span[]> {
  const boundaries = mainBoundaries(tree);
  const limit = executable.at(-1)?.end ?? ROM_BASE;
  const owned = new Map<number, Span[]>();
  for (const name of tree.list("src")) {
    const match = MAIN_SOURCE.exec(name);
    if (!match) continue;
    const source = tree.read(`src/${name}`);
    // Register-pinned, inline-assembly and fakematch C is not a C claim.
    if (source === undefined || !canonicalCSource(source)) continue;
    const address = Number.parseInt(match[1], 16);
    const spans = intersect([regionSpan(address, boundaries, limit)], executable);
    if (spans.length) owned.set(address, spans);
  }
  return owned;
}

/**
 * Verified C placeholders inside an overlay's reconstruction assembly. The
 * shape is the one `tools/full_c_progress.ts` measures: an `AlchemyC_<addr>`
 * label followed by the `.space` reservation that stands in for the compiled
 * C body.
 */
export interface OverlayOwner {
  label: string;
  entry: number;
  spans: Span[];
}

export function overlayPlaceholderOwners(source: string): OverlayOwner[] {
  const owners: OverlayOwner[] = [];
  let owner: OverlayOwner | undefined;
  let cursor = 0;
  const flush = (): void => {
    if (owner !== undefined && owner.spans.length > 0) owners.push(owner);
    owner = undefined;
  };
  for (const line of source.split(/\r?\n/)) {
    const label = /^\s*AlchemyC_([0-9a-f]{8}):\s*$/i.exec(line);
    if (label) {
      flush();
      cursor = Number.parseInt(label[1], 16);
      owner = { label: `AlchemyC_${label[1].toLowerCase()}`, entry: cursor, spans: [] };
      continue;
    }
    if (owner !== undefined && (/^\s*$/.test(line) || /^\s*\.L_[0-9a-z_.$]+:\s*$/i.test(line))) continue;
    const reservation = /^\s*\.space\s+(0x[0-9a-f]+|\d+)\s*$/i.exec(line);
    if (reservation && owner !== undefined) {
      const size = Number.parseInt(reservation[1], 0);
      owner.spans.push({ start: cursor, end: cursor + size });
      cursor += size;
      continue;
    }
    if (line.trim()) flush();
  }
  flush();
  return owners;
}

export function overlayPlaceholderSpans(source: string): Span[] {
  return normalize(overlayPlaceholderOwners(source).flatMap((owner) => owner.spans));
}

export function exactOverlayOwners(tree: SourceTree): Map<string, OverlayOwner[]> {
  const owned = new Map<string, OverlayOwner[]>();
  for (const name of tree.list("assets/code")) {
    const match = OVERLAY_ASSEMBLY.exec(name);
    if (!match) continue;
    const source = tree.read(`assets/code/${name}`);
    if (source === undefined) continue;
    const owners = overlayPlaceholderOwners(source).filter((owner) => {
      const cPath = `assets/code/${match[1]}_c_${hex8(owner.entry)}.c`;
      const cSource = tree.read(cPath);
      return cSource !== undefined && canonicalCSource(cSource);
    });
    if (owners.length) owned.set(match[1], owners);
  }
  return owned;
}

export function exactOverlaySpans(tree: SourceTree): Map<string, Span[]> {
  return new Map([...exactOverlayOwners(tree)].map(([overlay, owners]) => [
    overlay,
    normalize(owners.flatMap((owner) => owner.spans)),
  ]));
}

export interface SemanticCoverage {
  main: Map<number, Span[]>;
  overlays: Map<string, Span[]>;
  /** Natural source-owner spans before adjacent coverage is normalized. */
  overlayOwners: Map<string, Span[]>;
  sources: number;
  unresolved: string[];
  mainCensusClosed: boolean;
}

/**
 * Reviewed semantic-C ownership. Main-image owners either declare their
 * executable ranges in `semantic/main-regions.json` (multi-range owners) or
 * occupy exactly one audited region. Overlay owners carry their reviewed span
 * in `semantic/regions.json`; anything else is reported rather than guessed.
 */
export function semanticSpans(
  tree: SourceTree,
  boundaries: readonly number[],
  executable: readonly Span[],
  overlayExecutable: ReadonlyMap<string, Span[]> = new Map(),
): SemanticCoverage {
  const limit = executable.at(-1)?.end ?? ROM_BASE;
  const main = new Map<number, Span[]>();
  const overlays = new Map<string, Span[]>();
  const overlayOwners = new Map<string, Span[]>();
  const unresolved: string[] = [];
  let sources = 0;
  let mainCensusClosed = false;

  const ownerDocument = tree.read("semantic/main-regions.json");
  const owners = new Map<number, Span[]>();
  if (ownerDocument !== undefined) {
    const document = JSON.parse(ownerDocument);
    mainCensusClosed = document.ordinary_census?.status === "closed";
    for (const owner of document.main_owners ?? []) {
      owners.set(
        Number.parseInt(owner.entry, 16),
        (owner.executable_ranges ?? []).map((range: { address: string; size: number }) => ({
          start: Number.parseInt(range.address, 16),
          end: Number.parseInt(range.address, 16) + range.size,
        })),
      );
    }
  }
  for (const name of tree.list("semantic/main")) {
    const match = MAIN_SOURCE.exec(name);
    if (!match) continue;
    sources++;
    const address = Number.parseInt(match[1], 16);
    const declared = owners.get(address) ?? [regionSpan(address, boundaries, limit)];
    const spans = intersect(declared, executable);
    if (spans.length) main.set(address, spans);
    else unresolved.push(`semantic/main/${name}`);
  }

  const reviewedDocument = tree.read("semantic/regions.json");
  const reviewed = new Map<string, number>();
  if (reviewedDocument !== undefined) {
    for (const region of JSON.parse(reviewedDocument).manual_regions ?? []) {
      reviewed.set(`${region.overlay}@${Number.parseInt(region.entry, 16)}`, region.span_bytes);
    }
  }
  // A whole-overlay claim sizes every owner in one reviewed assertion: if each
  // executable byte of an overlay belongs to some semantic source, the owners
  // need no individual spans and the overlay's audited executable extent is the
  // covered extent. That is one evidence-bearing entry instead of one per owner, which is
  // what an overlay converted in full actually warrants. Exact C is subtracted
  // downstream, so a partially exact overlay stays honest.
  const fullOverlays = new Map<string, string>();
  if (reviewedDocument !== undefined) {
    for (const claim of JSON.parse(reviewedDocument).full_overlays ?? []) {
      fullOverlays.set(claim.overlay, claim.evidence ?? "");
    }
  }
  const claimedSources = new Set<string>();

  for (const name of tree.list("semantic/overlays")) {
    const match = SEMANTIC_OVERLAY_SOURCE.exec(name);
    if (!match) continue;
    sources++;
    const overlay = match[1];
    const address = Number.parseInt(match[2], 16);
    if (fullOverlays.has(overlay)) {
      claimedSources.add(overlay);
      continue;
    }
    const span = reviewed.get(`${overlay}@${address}`);
    if (span === undefined) {
      // The decoded-region inventory that sizes ordinary overlay owners is a
      // build diagnostic, so an unlisted owner is reported, not estimated.
      unresolved.push(`semantic/overlays/${name}`);
      continue;
    }
    const owner = { start: address, end: address + span };
    overlayOwners.set(overlay, [...(overlayOwners.get(overlay) ?? []), owner]);
    overlays.set(overlay, [...(overlays.get(overlay) ?? []), owner]);
  }

  // A claim is only honoured for an overlay that actually carries semantic
  // sources and has an audited executable extent; an empty claim credits
  // nothing rather than crediting the whole overlay on an unbacked assertion.
  for (const overlay of fullOverlays.keys()) {
    const extent = overlayExecutable.get(overlay) ?? [];
    if (!claimedSources.has(overlay) || extent.length === 0) continue;
    overlays.set(overlay, normalize([...(overlays.get(overlay) ?? []), ...extent]));
  }
  for (const [overlay, spans] of overlays) overlays.set(overlay, normalize(spans));
  return { main, overlays, overlayOwners, sources, unresolved, mainCensusClosed };
}

// --------------------------------------------------------------- ROM layout

interface RomRange {
  start: number;
  end: number;
  kind: string;
  family: string;
  label: string;
  sources: string[];
}

function hexValue(value: unknown): number | undefined {
  if (typeof value === "number" && Number.isSafeInteger(value)) return value;
  if (typeof value === "string" && /^0x[0-9a-f]+$/i.test(value)) return Number.parseInt(value, 16);
  return undefined;
}

/**
 * Address ranges named by the tracked asset manifest. Only the labelling of
 * non-code ROM data uses this walk; sizes shown for those areas are computed
 * as the complement of the audited code areas, so an unlabelled package
 * cannot change a byte count, only the bucket a byte is described under.
 */
export function manifestRanges(tree: SourceTree, romSize: number): RomRange[] {
  const ranges: RomRange[] = [];
  const visited = new Set<string>();
  const sourceCache = new Map<string, string[]>();
  const sourcesOf = (node: unknown, chain = new Set<string>()): string[] => {
    const found = new Set<string>();
    const gather = (value: unknown): void => {
      if (Array.isArray(value)) {
        for (const child of value) gather(child);
        return;
      }
      if (value === null || typeof value !== "object") {
        if (typeof value !== "string") return;
        if (/\.json$/i.test(value)) {
          if (chain.has(value)) return;
          const cached = sourceCache.get(value);
          if (cached !== undefined) {
            for (const source of cached) found.add(source);
            return;
          }
          const text = tree.read(value);
          if (text === undefined) return;
          try {
            const nestedChain = new Set(chain).add(value);
            const nested = sourcesOf(JSON.parse(text), nestedChain);
            sourceCache.set(value, nested);
            for (const source of nested) found.add(source);
          } catch {
            // An unreadable nested plan contributes no representation evidence.
          }
          return;
        }
        if (/\.[a-z0-9]{2,5}$/i.test(value)) found.add(value);
        return;
      }
      for (const child of Object.values(value as Record<string, unknown>)) gather(child);
    };
    gather(node);
    return [...found].sort();
  };
  const push = (
    start: number | undefined,
    size: number | undefined,
    kind: string,
    family: string,
    label: string,
    sources: string[],
  ): void => {
    if (start === undefined || size === undefined || size <= 0) return;
    if (start < ROM_BASE || start + size > ROM_BASE + romSize) return;
    ranges.push({ start, end: start + size, kind, family, label, sources });
  };
  const visit = (node: unknown, kind: string, family: string): void => {
    if (Array.isArray(node)) {
      // Manifest families use both [id, address, size, ...] and embedded
      // [address, size, source] tuples; a few family records carry several
      // address/size pairs in one row. Recognize every ROM-address pair rather
      // than teaching the dashboard a display-specific version of each schema.
      for (let index = 0; index + 1 < node.length; index++) {
        const start = hexValue(node[index]);
        const size = hexValue(node[index + 1]);
        if (start === undefined || start < ROM_BASE || size === undefined || size <= 0) continue;
        const identity = index > 0 && typeof node[index - 1] === "string" &&
            hexValue(node[index - 1]) === undefined
          ? node[index - 1]
          : kind;
        push(start, size, kind, family, `${String(identity)} · ${kind}`, sourcesOf(node));
      }
      for (const child of node) visit(child, kind, family);
      return;
    }
    if (node === null || typeof node !== "object") return;
    const record = node as Record<string, unknown>;
    const local = typeof record.kind === "string" ? record.kind : kind;
    const identity = record.id ?? record.name ?? record.source ?? record.index ?? local;
    push(
      hexValue(record.address), hexValue(record.size), local, family,
      `${String(identity)} · ${local}`,
      sourcesOf(record),
    );
    for (const [key, value] of Object.entries(record)) {
      // Components describe decoded payloads, not ROM ranges.
      if (key !== "components") visit(value, local, family);
    }
    for (const reference of [record.index, record.source, record.plan]) {
      if (typeof reference !== "string" || !reference.endsWith(".json")) continue;
      if (visited.has(reference)) continue;
      visited.add(reference);
      const text = tree.read(reference);
      if (text === undefined) continue;
      try {
        visit(JSON.parse(text), local, family);
      } catch {
        // A package index that cannot be parsed simply contributes no label.
      }
    }
  };
  const manifest = readJson(tree, "assets/manifest.json");
  for (const section of [manifest.series, manifest.regions, manifest.closure_packages]) {
    if (!Array.isArray(section)) continue;
    for (const entry of section) {
      if (entry === null || typeof entry !== "object") continue;
      const family = typeof entry.kind === "string" ? entry.kind : "asset";
      visit(entry, family, family);
    }
  }
  return ranges;
}

/** Compressed ROM footprint and decoded size of every Thumb code overlay. */
export function overlayStreams(tree: SourceTree): Map<string, { start: number; romBytes: number; decodedBytes: number }> {
  const streams = new Map<string, { start: number; romBytes: number; decodedBytes: number }>();
  const manifest = readJson(tree, "assets/manifest.json");
  for (const series of manifest.series ?? []) {
    if (series.kind !== OVERLAY_SERIES) continue;
    for (const entry of series.resources ?? []) {
      const [id, address, size, decoded] = entry as [string, string, string, string];
      const start = hexValue(address);
      const romBytes = hexValue(size);
      const decodedBytes = hexValue(decoded);
      if (start === undefined || romBytes === undefined) continue;
      streams.set(`resource_${id}`, { start, romBytes, decodedBytes: decodedBytes ?? 0 });
    }
  }
  // The final-battle module is selected through a dedicated layout rather than
  // the ordinary 3xx series table. It is still compressed code and must not be
  // painted as asset data merely because it has a different manifest schema.
  for (const entry of [...(manifest.series ?? []), ...(manifest.closure_packages ?? [])]) {
    if (entry.kind !== "golden-sun-final-battle-overlay-series" ||
        typeof entry.source !== "string") continue;
    const layoutText = tree.read(entry.source);
    if (layoutText === undefined) continue;
    const layout = JSON.parse(layoutText);
    const start = hexValue(layout.address);
    const romBytes = hexValue(layout.stream_size);
    const decodedBytes = hexValue(layout.decoded_size);
    const resource = hexValue(layout.resource_id);
    if (start === undefined || romBytes === undefined || resource === undefined) continue;
    streams.set(`resource_${resource.toString(16)}`, {
      start, romBytes, decodedBytes: decodedBytes ?? 0,
    });
  }
  return streams;
}

const ASSET_BUCKETS: Array<{ id: string; label: string; match: RegExp }> = [
  { id: "sprites", label: "Sprites & battle art", match: /sprite|character-catalog|mtf4|chr|enemy/i },
  { id: "backgrounds", label: "Backgrounds & stills", match: /still|background|title|tile|palette|canvas|4bpp|8bpp/i },
  { id: "maps", label: "Maps & world", match: /map|grid|charblock|chiiki|tokushu|world/i },
  { id: "audio", label: "Music & sound", match: /sound|audio|wave|pcm|music|midi|sequence/i },
  { id: "text", label: "Text & fonts", match: /message|font|localization|kana|glyph|namae|staff/i },
  { id: "tables", label: "Tables & databases", match: /data|table|database|directory|runtime|encounter|header|resource/i },
];

export function assetBucket(kind: string): { id: string; label: string } {
  for (const bucket of ASSET_BUCKETS) if (bucket.match.test(kind)) return bucket;
  return { id: "other", label: "Other data" };
}

/**
 * Natural ROM-data leaves. Every tile is backed by one addressed manifest
 * range; overlapping parent packages lose the bytes owned by their more
 * specific children, so nested manifests cannot double-count the cartridge.
 * Compressed packages remain one package tile because decoded children have no
 * honest one-to-one ROM-byte footprint. The final remainder is explicit.
 */
export function manifestAssetTiles(
  tree: SourceTree,
  romSize: number,
  dataSpans: readonly Span[],
): Tile[] {
  const byExtent = new Map<string, RomRange>();
  for (const range of manifestRanges(tree, romSize)) {
    const key = `${range.start}:${range.end}`;
    const previous = byExtent.get(key);
    if (previous === undefined) {
      byExtent.set(key, { ...range, sources: [...range.sources] });
      continue;
    }
    previous.sources = [...new Set([...previous.sources, ...range.sources])].sort();
    if (previous.kind === "asset" && range.kind !== "asset") {
      previous.kind = range.kind;
      previous.family = range.family;
      previous.label = range.label;
    }
  }

  // Children first. A parent can then claim only its still-unclaimed ROM
  // footprint, preserving both byte conservation and the finest known unit.
  const ranges = [...byExtent.values()].sort((left, right) =>
    (left.end - left.start) - (right.end - right.start) ||
    left.start - right.start || left.end - right.end || left.label.localeCompare(right.label));
  const tiles: Tile[] = [];
  let claimed: Span[] = [];
  for (const range of ranges) {
    const fresh = subtract(intersect([range], dataSpans), claimed);
    const bytes = spanBytes(fresh);
    if (bytes <= 0) continue;
    claimed = normalize([...claimed, ...fresh]);
    const tier = assetTierOf(range.sources, range.kind);
    const categories: Tile["categories"] = { asset_data: bytes };
    categories[tier] = bytes;
    tiles.push({
      label: `${range.label} · 0x${hex8(range.start)} · ${bytes.toLocaleString("en-US")} bytes`,
      bytes,
      categories,
      group: range.family,
      subgroup: range.family === range.kind ? undefined : range.kind,
      address: range.start,
    });
  }

  const unclassified = spanBytes(dataSpans) - spanBytes(claimed);
  if (unclassified > 0) {
    tiles.push({
      label: "Unclassified ROM-image data",
      bytes: unclassified,
      categories: { asset_data: unclassified, asset_unclassified: unclassified },
      group: "unclassified-rom-data",
    });
  }
  return tiles;
}

interface BuiltAssetRegion {
  address: number;
  size: number;
  kind: string;
  sources?: string[];
}

/** Map every tracked asset source back to the real top-level manifest family
 * that owns it. Nested JSON indexes are followed with their paths resolved
 * relative to the referring index; first-party region/series owners take
 * precedence over the later closure bookkeeping packages. */
function assetFamiliesBySource(tree: SourceTree): Map<string, string> {
  const manifest = readJson(tree, "assets/manifest.json");
  const families = new Map<string, string>();
  const visited = new Set<string>();
  const visit = (value: unknown, family: string, base = ""): void => {
    if (Array.isArray(value)) {
      for (const child of value) visit(child, family, base);
      return;
    }
    if (value !== null && typeof value === "object") {
      for (const child of Object.values(value as Record<string, unknown>)) visit(child, family, base);
      return;
    }
    if (typeof value !== "string" || !/\.[a-z0-9]{2,5}$/i.test(value)) return;
    const path = /^(?:assets|asm|semantic|src)\//.test(value) ? value : join(base, value);
    if (!families.has(path)) families.set(path, family);
    if (!path.endsWith(".json") || visited.has(path)) return;
    visited.add(path);
    const text = tree.read(path);
    if (text === undefined) return;
    try {
      visit(JSON.parse(text), family, dirname(path));
    } catch {
      // A malformed or intentionally non-JSON reference contributes no lineage.
    }
  };
  for (const section of [manifest.series, manifest.regions, manifest.closure_packages]) {
    if (!Array.isArray(section)) continue;
    for (const entry of section) {
      if (entry === null || typeof entry !== "object") continue;
      const family = typeof entry.kind === "string" ? entry.kind : "asset";
      visit(entry, family);
    }
  }
  return families;
}

/**
 * Leaves from the byte-verified asset build. This is the schema-aware expansion
 * of every tracked package and series, so it is the authoritative live
 * dashboard source when available; the generic tracked-manifest walk remains
 * the clone-safe publication fallback.
 */
export function verifiedAssetTiles(
  tree: SourceTree,
  target: DecompTargetId,
): Tile[] | undefined {
  const text = tree.read("out/full/assets/manifest.json");
  if (text === undefined) return undefined;
  const document = JSON.parse(text) as {
    format?: number;
    rom_size?: number;
    verification?: string;
    regions?: BuiltAssetRegion[];
  };
  const registry = decompTarget(target);
  if (document.format !== 1 || document.rom_size !== registry.romSize ||
      !["rom", "source_only"].includes(document.verification ?? "") ||
      !Array.isArray(document.regions)) {
    throw new Error("verified asset manifest has an unsupported or incomplete schema");
  }
  const inventory = readJson(tree, `metrics/${target}-executable.json`) as ExecutableInventory;
  const mainExecutable = unionIntervals(inventory.main.intervals);
  const streams = overlayStreams(tree);
  const codeSpans = normalize([
    ...mainExecutable,
    ...[...streams.values()].map((stream) => ({
      start: stream.start,
      end: stream.start + stream.romBytes,
    })),
  ]);
  const dataSpans = subtract(
    [{ start: ROM_BASE, end: ROM_BASE + registry.romSize }],
    codeSpans,
  );

  const ordered = [...document.regions].sort((left, right) =>
    left.address - right.address || left.size - right.size);
  const lineage = manifestRanges(tree, registry.romSize);
  const sourceFamilies = assetFamiliesBySource(tree);
  const assetManifest = readJson(tree, "assets/manifest.json");
  const topFamilies = new Set<string>(
    [...(assetManifest.series ?? []), ...(assetManifest.regions ?? [])]
      .map((entry) => entry.kind)
      .filter((kind): kind is string => typeof kind === "string"),
  );
  const familyOf = (region: BuiltAssetRegion): string => {
    const directSeries = `${region.kind}-series`;
    if (topFamilies.has(directSeries)) return directSeries;
    for (const source of region.sources ?? []) {
      const family = sourceFamilies.get(source);
      if (family !== undefined) return family;
    }
    const end = region.address + region.size;
    return lineage
      .filter((range) => range.start <= region.address && range.end >= end)
      .sort((left, right) =>
        (left.end - left.start) - (right.end - right.start))[0]?.family ?? region.kind;
  };
  let previousEnd = ROM_BASE;
  const tiles: Tile[] = [];
  let covered: Span[] = [];
  for (const region of ordered) {
    if (!Number.isSafeInteger(region.address) || !Number.isSafeInteger(region.size) || region.size <= 0 ||
        region.address < ROM_BASE || region.address + region.size > ROM_BASE + registry.romSize) {
      throw new Error("verified asset manifest contains an invalid region");
    }
    if (region.address < previousEnd) throw new Error("verified asset manifest regions overlap");
    previousEnd = region.address + region.size;
    const span = { start: region.address, end: region.address + region.size };
    const data = intersect([span], dataSpans);
    const bytes = spanBytes(data);
    if (bytes <= 0) continue;
    if (bytes !== region.size) {
      throw new Error(`asset region 0x${hex8(region.address)} partially overlaps code`);
    }
    covered.push(...data);
    const sources = region.sources ?? [];
    const tier = assetTierOf(sources, region.kind);
    const family = familyOf(region);
    const categories: Tile["categories"] = { asset_data: bytes };
    categories[tier] = bytes;
    const identity = sources.length > 0 ? basename(sources[0]) : region.kind;
    tiles.push({
      label: `${identity} · ${region.kind} · 0x${hex8(region.address)}`,
      bytes,
      categories,
      group: family,
      subgroup: family === region.kind ? undefined : region.kind,
      address: region.address,
    });
  }
  const expected = spanBytes(dataSpans);
  const actual = spanBytes(normalize(covered));
  if (actual !== expected) {
    throw new Error(`verified asset leaves cover ${actual} of ${expected} ROM-data bytes`);
  }
  return tiles;
}

// ------------------------------------------------------------- map assembly

function categoryTotal(tiles: readonly Tile[], category: CoverageCategory): number {
  return tiles.reduce((sum, tile) => sum + (tile.categories[category] ?? 0), 0);
}

function area(id: string, label: string, tiles: Tile[]): Area {
  const categories: Partial<Record<CoverageCategory, number>> = {};
  for (const category of ["exact_c", "semantic_c", "assembly", "retained_asm", "asset_data"] as CoverageCategory[]) {
    const bytes = categoryTotal(tiles, category);
    if (bytes) categories[category] = bytes;
  }
  return {
    id,
    label,
    bytes: tiles.reduce((sum, tile) => sum + tile.bytes, 0),
    categories,
    tiles: tiles.filter((tile) => tile.bytes > 0),
  };
}

function hex8(address: number): string {
  return address.toString(16).padStart(8, "0");
}

// Regions that will never become C by design are rendered orange. keep_asm
// retention, structural runtime/veneer/padding
// kinds, and explicit cannot-express contracts qualify; keep_structured_asm
// alone does not (it is a default, not a contract).
const PERMANENT_KINDS = new Set([
  "linker_veneer", "alignment_padding", "relocated_arm_runtime_module",
  "armv4t_helper_bank", "iwram_runtime_veneer",
]);
export function retainedMainSpans(): Span[] {
  // This is the same full-build assembly evidence checked by
  // core_retained_audit.ts. If it is unavailable, render no orange rather than
  // guessing from a census complement or from a partial build manifest.
  const path = join(dirname(Bun.fileURLToPath(import.meta.url)), "..", "out", "full", "asm", "manifest.json");
  const explicitNonC: Span[] = [];
  const nonCPath = join(ROOT, "semantic", "main-regions.json");
  if (existsSync(nonCPath)) {
    try {
      const document = JSON.parse(readFileSync(nonCPath, "utf8")) as {
        non_c_ranges?: { address?: string | number; size?: number; kind?: string; evidence?: string }[];
      };
      for (const region of document.non_c_ranges ?? []) {
        const start = typeof region.address === "string" ? Number.parseInt(region.address, 16) : region.address;
        if (!Number.isSafeInteger(start) || !Number.isSafeInteger(region.size) || region.size <= 0 ||
            !["literal_pool", "alignment_padding", "lookup_table"].includes(region.kind ?? "") ||
            !region.evidence?.trim()) continue;
        explicitNonC.push({ start, end: start + region.size });
      }
    } catch {
      // An unavailable/invalid registry never becomes orange evidence.
    }
  }
  if (!existsSync(path)) return normalize(explicitNonC);
  try {
    const manifest = JSON.parse(readFileSync(path, "utf8")) as {
      regions?: { address?: number; size?: number; kind?: string; retention?: string; confidence?: string; evidence?: unknown }[];
    };
    const spans: Span[] = [];
    for (const r of manifest.regions ?? []) {
      if (typeof r.address !== "number" || typeof r.size !== "number" || r.size <= 0) continue;
      const permanent = r.retention === "keep_asm" ||
        (r.retention === "keep_structured_asm" && r.confidence === "proven" &&
          typeof r.evidence === "string" && r.evidence.trim().length > 0) ||
        // Audited 2026-07-31: the merge-with-owner family is literal
        // pools, alignment and data the owner registration deliberately
        // excludes — 64 bytes residual across the whole bucket. Permanent.
        r.retention === "merge_with_owner" ||
        r.retention === "merge_with_function_owner" ||
        r.retention === "merge_with_continuations" ||
        (r.kind ?? "").startsWith("deliberate_") ||
        r.retention === "adjacent_section_alignment" ||
        PERMANENT_KINDS.has(r.kind ?? "") ||
        String(r.evidence ?? "").includes("approved_compiler_cannot_express");
      if (permanent) spans.push({ start: r.address, end: r.address + r.size });
    }
    return normalize([...spans, ...explicitNonC]);
  } catch {
    return normalize(explicitNonC);
  }
}

/**
 * Group audited main-image regions into contiguous address bands of roughly
 * `target` executable bytes. This coarser view is used by the ROM dashboard,
 * where the main image shares space with the whole ROM image.
 */
function mainBands(
  executable: readonly Span[],
  exact: readonly Span[],
  semantic: readonly Span[],
  retained: readonly Span[],
  target: number,
): Tile[] {
  const tiles: Tile[] = [];
  let current: { start: number; spans: Span[] } | undefined;
  const flush = (): void => {
    if (!current || !current.spans.length) return;
    const bytes = spanBytes(current.spans);
    const exactBytes = spanBytes(intersect(current.spans, exact));
    const semanticBytes = spanBytes(intersect(current.spans, semantic));
    // Retained spans may only claim bytes no C category already owns.
    const owned = normalize([...exact, ...semantic]);
    const retainedBytes = spanBytes(intersect(subtract(current.spans, owned), retained));
    tiles.push({
      label: hex8(current.start).slice(0, 6),
      bytes,
      categories: {
        exact_c: exactBytes,
        semantic_c: semanticBytes,
        assembly: Math.max(bytes - exactBytes - semanticBytes - retainedBytes, 0),
        retained_asm: retainedBytes,
      },
    });
    current = undefined;
  };
  for (const span of normalize(executable)) {
    let start = span.start;
    while (start < span.end) {
      if (!current) current = { start, spans: [] };
      const room = target - spanBytes(current.spans);
      const end = Math.min(span.end, start + Math.max(room, 1));
      current.spans.push({ start, end });
      start = end;
      if (spanBytes(current.spans) >= target) flush();
    }
  }
  flush();
  return tiles;
}

/**
 * One tile per audited source/owner boundary. Unlike `mainBands`, this never
 * invents a display-sized partition: a tile owns its natural executable bytes
 * between consecutive audited starts (or an audited extent edge).
 */
export function mainOwnerTiles(
  executable: readonly Span[],
  boundaries: readonly number[],
  exact: readonly Span[],
  semantic: readonly Span[],
  retained: readonly Span[],
): Tile[] {
  const tiles: Tile[] = [];
  const owned = normalize([...exact, ...semantic]);
  for (const executableSpan of normalize(executable)) {
    const bankCuts: number[] = [];
    for (let address = Math.floor(executableSpan.start / 0x10000) * 0x10000 + 0x10000;
         address < executableSpan.end; address += 0x10000) {
      bankCuts.push(address);
    }
    const cuts = [...new Set([
      executableSpan.start,
      executableSpan.end,
      ...bankCuts,
      ...boundaries.filter((address) => address > executableSpan.start && address < executableSpan.end),
    ])].sort((left, right) => left - right);
    for (let index = 0; index < cuts.length - 1; index++) {
      const span = { start: cuts[index], end: cuts[index + 1] };
      const bytes = span.end - span.start;
      const exactBytes = spanBytes(intersect([span], exact));
      const semanticBytes = spanBytes(intersect([span], semantic));
      const retainedBytes = spanBytes(intersect(subtract([span], owned), retained));
      tiles.push({
        label: `0x${hex8(span.start)}–0x${hex8(span.end)}`,
        bytes,
        categories: {
          exact_c: exactBytes,
          semantic_c: semanticBytes,
          assembly: Math.max(bytes - exactBytes - semanticBytes - retainedBytes, 0),
          retained_asm: retainedBytes,
        },
        group: `${hex8(Math.floor(span.start / 0x10000) * 0x10000).slice(0, 4)} · 64 KiB bank`,
        address: span.start,
      });
    }
  }
  return tiles;
}

/** One tile per source-backed owner, plus one per contiguous unowned run. */
export function overlayOwnerTiles(
  overlay: string,
  executable: readonly Span[],
  exactOwners: readonly OverlayOwner[],
  semanticOwners: readonly Span[],
): Tile[] {
  const extent = normalize(executable);
  const exact = normalize(exactOwners.flatMap((owner) => owner.spans));
  const tiles: Tile[] = [];

  for (const owner of exactOwners) {
    const bytes = spanBytes(intersect(owner.spans, extent));
    if (bytes <= 0) continue;
    tiles.push({
      label: `${overlay.replace(/^resource_/, "")} · ${owner.label} · byte-exact C`,
      bytes,
      categories: { exact_c: bytes },
      group: overlay.replace(/^resource_/, ""),
      address: owner.entry,
    });
  }

  const semanticCredited: Span[] = [];
  for (const owner of semanticOwners) {
    const credited = subtract(intersect([owner], extent), exact);
    const bytes = spanBytes(credited);
    if (bytes <= 0) continue;
    semanticCredited.push(...credited);
    tiles.push({
      label: `${overlay.replace(/^resource_/, "")} · semantic owner 0x${hex8(owner.start)}`,
      bytes,
      categories: { semantic_c: bytes },
      group: overlay.replace(/^resource_/, ""),
      address: owner.start,
    });
  }

  const owned = normalize([...exact, ...semanticCredited]);
  for (const span of subtract(extent, owned)) {
    const bytes = span.end - span.start;
    tiles.push({
      label: `${overlay.replace(/^resource_/, "")} · unowned assembly 0x${hex8(span.start)}–0x${hex8(span.end)}`,
      bytes,
      categories: { assembly: bytes },
      group: overlay.replace(/^resource_/, ""),
      address: span.start,
    });
  }
  return tiles;
}

/**
 * Merge neighbouring tiles until each carries at least `target` bytes. The
 * ROM view holds the whole ROM image, so its code areas are drawn at a coarser
 * grain than the executable view where the same regions get the full mosaic.
 */
export function groupTiles(tiles: readonly Tile[], target: number): Tile[] {
  const grouped: Tile[] = [];
  let current: Tile | undefined;
  let first = "";
  let last = "";
  for (const tile of tiles) {
    if (!current) {
      current = { label: tile.label, bytes: 0, categories: {} };
      first = tile.label;
    }
    current.bytes += tile.bytes;
    last = tile.label;
    for (const category of CATEGORY_ORDER) {
      const bytes = tile.categories[category] ?? 0;
      if (bytes) current.categories[category] = (current.categories[category] ?? 0) + bytes;
    }
    if (current.bytes >= target) {
      current.label = first === last ? first : `${first}–${last}`;
      grouped.push(current);
      current = undefined;
    }
  }
  if (current) {
    current.label = first === last ? first : `${first}–${last}`;
    grouped.push(current);
  }
  return grouped;
}

export interface BuildOptions {
  target: DecompTargetId;
  exact: SourceTree;
  semantic?: SourceTree;
  /**
   * The publishing path checks the derived exact total against the tracked
   * Full-C report. A live dashboard deliberately reads an uncommitted worktree,
   * where that report may lag by a few seconds or an entire edit, so it can
   * disable only this publication check while retaining every ownership rule.
   */
  validateTrackedProgress?: boolean;
  /** Prefer the schema-expanded, byte-verified build manifest for ROM-data
   * leaves. Publication and the live dashboard enable this after verification;
   * source/ref-only projections retain the tracked-manifest fallback. */
  preferVerifiedAssets?: boolean;
}

export function buildCoverageMap(options: BuildOptions): CoverageMap {
  const registry = decompTarget(options.target);
  const inventory = readJson(
    options.exact,
    `metrics/${options.target}-executable.json`,
  ) as ExecutableInventory;
  const tracked = options.validateTrackedProgress === false
    ? undefined
    : readJson(options.exact, `metrics/${options.target}-progress.json`) as ProgressReport;
  if (inventory.audit !== "complete") {
    throw new Error(`${options.target} executable audit is incomplete; coverage map withheld`);
  }

  const mainExecutable = unionIntervals(inventory.main.intervals);
  const overlayExecutable = new Map<string, Span[]>(
    inventory.overlays.map((overlay) => [overlay.id, unionIntervals(overlay.intervals)]),
  );

  const exactMain = [...exactMainSpans(options.exact, mainExecutable).values()].flat();
  const exactOverlayOwnersByResource = exactOverlayOwners(options.exact);
  const exactOverlayByResource = new Map<string, Span[]>();
  for (const [overlay, owners] of exactOverlayOwnersByResource) {
    exactOverlayByResource.set(
      overlay,
      intersect(owners.flatMap((owner) => owner.spans), overlayExecutable.get(overlay) ?? []),
    );
  }

  const boundaries = mainBoundaries(options.exact);
  const semanticCoverage = options.semantic
    ? semanticSpans(options.semantic, boundaries, mainExecutable, overlayExecutable)
    : {
        main: new Map(), overlays: new Map(), overlayOwners: new Map(),
        sources: 0, unresolved: [] as string[], mainCensusClosed: false,
      };

  // Exact always wins over semantic: semantic coverage only shows executable
  // bytes that are not already byte-exact.
  //
  // Two different things shrink semantic coverage, and they must be counted
  // apart. Supersession means exact C replaced a semantic draft. Outside-extent
  // means a
  // semantic source claims an address the executable audit does not call
  // executable, so the work is real but will never appear on this map. Folding
  // them into one figure invites reading a total as supersession when none of
  // it is, which is a mistake this tool has already caused once.
  const exactMainUnion = normalize(exactMain);
  const semanticMain = subtract([...semanticCoverage.main.values()].flat(), exactMainUnion);
  const semanticOverlayByResource = new Map<string, Span[]>();
  let semanticSuperseded = spanBytes([...semanticCoverage.main.values()].flat()) - spanBytes(semanticMain);
  let semanticOutsideExtent = 0;
  for (const [overlay, spans] of semanticCoverage.overlays) {
    const executable = overlayExecutable.get(overlay) ?? [];
    const inExtent = intersect(spans, executable);
    const owned = subtract(inExtent, exactOverlayByResource.get(overlay) ?? []);
    semanticOutsideExtent += spanBytes(spans) - spanBytes(inExtent);
    semanticSuperseded += spanBytes(inExtent) - spanBytes(owned);
    if (owned.length) semanticOverlayByResource.set(overlay, owned);
  }

  const exactMainBytes = spanBytes(exactMainUnion);
  const exactOverlayBytes = [...exactOverlayByResource.values()].reduce(
    (sum, spans) => sum + spanBytes(spans),
    0,
  );
  if (tracked !== undefined &&
      (exactMainBytes !== tracked.main.full_c_bytes || exactOverlayBytes !== tracked.overlays.full_c_bytes)) {
    throw new Error(
      "derived exact ownership disagrees with the tracked Full-C report " +
      `(main ${exactMainBytes} vs ${tracked.main.full_c_bytes}, ` +
      `overlays ${exactOverlayBytes} vs ${tracked.overlays.full_c_bytes}); ` +
      "regenerate metrics/ before the coverage map",
    );
  }

  const semanticMainBytes = spanBytes(semanticMain);
  const semanticOverlayBytes = [...semanticOverlayByResource.values()].reduce(
    (sum, spans) => sum + spanBytes(spans),
    0,
  );
  const executableBytes = inventory.total_union_bytes;
  const exactBytes = exactMainBytes + exactOverlayBytes;
  const semanticBytes = semanticMainBytes + semanticOverlayBytes;

  // -------------------------------------------------- executable universe
  // Orange is reserved for spans with an explicit retained-assembly contract
  // in the audited manifest. A closed semantic-owner census does not itself
  // prove the remaining bytes permanent: any complement without such evidence
  // stays gray as unresolved assembly.
  const mainRetained = retainedMainSpans();
  const executableAreas: Area[] = [
    area("main", "Main image", mainOwnerTiles(mainExecutable, boundaries, exactMainUnion, semanticMain, mainRetained)),
  ];
  const overlayTiles: Tile[] = [];
  for (const overlay of inventory.overlays) {
    const executable = overlayExecutable.get(overlay.id) ?? [];
    const semanticOwners = semanticCoverage.overlayOwners.get(overlay.id) ?? [];
    overlayTiles.push(...overlayOwnerTiles(
      overlay.id,
      executable,
      exactOverlayOwnersByResource.get(overlay.id) ?? [],
      semanticOwners.length > 0
        ? semanticOwners
        : semanticOverlayByResource.get(overlay.id) ?? [],
    ));
  }
  executableAreas.push(area("overlays", "Decoded code overlays", overlayTiles));

  // ------------------------------------------------------------ ROM image
  const romSize = registry.romSize;
  const streams = overlayStreams(options.exact);
  const streamSpans = [...streams.values()].map((stream) => ({
    start: stream.start,
    end: stream.start + stream.romBytes,
  }));
  const codeSpans = normalize([...mainExecutable, ...streamSpans]);
  const dataSpans = subtract([{ start: ROM_BASE, end: ROM_BASE + romSize }], codeSpans);

  const romAreas: Area[] = [
    area(
      "rom-main-code",
      "Main image code",
      mainBands(mainExecutable, exactMainUnion, semanticMain, mainRetained, 65536),
    ),
  ];
  const streamTiles: Tile[] = [];
  for (const [overlay, stream] of [...streams].sort((left, right) => left[1].start - right[1].start)) {
    const executable = overlayExecutable.get(overlay) ?? [];
    const decoded = spanBytes(executable);
    const exactShare = decoded ? spanBytes(exactOverlayByResource.get(overlay) ?? []) / decoded : 0;
    const semanticShare = decoded ? spanBytes(semanticOverlayByResource.get(overlay) ?? []) / decoded : 0;
    // A compressed stream has no per-byte correspondence with the code it
    // decodes to, so its tile is sized by ROM bytes and shaded by the share
    // of its decoded executable bytes each category owns.
    const exactPart = Math.round(stream.romBytes * exactShare);
    const semanticPart = Math.round(stream.romBytes * semanticShare);
    streamTiles.push({
      label: overlay.replace(/^resource_/, ""),
      bytes: stream.romBytes,
      categories: {
        exact_c: exactPart,
        semantic_c: semanticPart,
        assembly: stream.romBytes - exactPart - semanticPart,
      },
    });
  }
  romAreas.push(area("rom-overlay-streams", "Compressed code overlays", groupTiles(streamTiles, 49152)));

  const assetTiles = options.preferVerifiedAssets
    ? verifiedAssetTiles(options.exact, options.target) ??
      manifestAssetTiles(options.exact, romSize, dataSpans)
    : manifestAssetTiles(options.exact, romSize, dataSpans);
  romAreas.push(area("rom-data", "Assets & data", assetTiles));

  const romBytesCheck = romAreas.reduce((sum, item) => sum + item.bytes, 0);
  if (romBytesCheck !== romSize) {
    throw new Error(`ROM areas cover ${romBytesCheck} of ${romSize} bytes`);
  }
  const executableCheck = executableAreas.reduce((sum, item) => sum + item.bytes, 0);
  if (executableCheck !== executableBytes) {
    throw new Error(`executable areas cover ${executableCheck} of ${executableBytes} bytes`);
  }

  const retainedBytes = executableAreas.reduce(
    (sum, item) => sum + (item.categories.retained_asm ?? 0), 0,
  );
  return {
    format: 1,
    kind: "golden-sun-rom-coverage-map",
    target: options.target,
    derivation: "tracked-evidence-v1",
    rom_bytes: romSize,
    executable_bytes: executableBytes,
    categories: {
      exact_c: { bytes: exactBytes, percent_of_executable: roundHalfUpPercent(exactBytes, executableBytes) },
      semantic_c: {
        bytes: semanticBytes,
        percent_of_executable: roundHalfUpPercent(semanticBytes, executableBytes),
      },
      assembly: {
        bytes: executableBytes - exactBytes - semanticBytes - retainedBytes,
        percent_of_executable: roundHalfUpPercent(
          executableBytes - exactBytes - semanticBytes - retainedBytes,
          executableBytes,
        ),
      },
      retained_asm: {
        bytes: retainedBytes,
        percent_of_executable: roundHalfUpPercent(retainedBytes, executableBytes),
      },
      asset_data: {
        bytes: spanBytes(dataSpans),
        percent_of_executable: 0,
      },
    },
    main: {
      executable_bytes: spanBytes(mainExecutable),
      exact_c_bytes: exactMainBytes,
      semantic_c_bytes: semanticMainBytes,
    },
    overlays: {
      executable_bytes: executableBytes - spanBytes(mainExecutable),
      exact_c_bytes: exactOverlayBytes,
      semantic_c_bytes: semanticOverlayBytes,
    },
    provenance: {
      exact_source: options.exact.id,
      semantic_source: options.semantic?.id ?? "absent",
      semantic_sources: semanticCoverage.sources,
      main_semantic_census: semanticCoverage.mainCensusClosed ? "closed" : "open",
      semantic_superseded_bytes: semanticSuperseded,
      semantic_outside_extent_bytes: semanticOutsideExtent,
      semantic_unresolved: semanticCoverage.unresolved.sort(),
    },
    rom_areas: romAreas,
    executable_areas: executableAreas,
  };
}

// ------------------------------------------------------------------ treemap

export interface Rect {
  x: number;
  y: number;
  width: number;
  height: number;
}

export interface Placed<T> {
  item: T;
  rect: Rect;
}

/**
 * Squarified treemap placement (Bruls, Huizing, van Wijk). The algorithm's
 * aspect-ratio guarantee depends on descending area order; address order turns
 * a dense owner census into barcode strips. Stable sorting here makes every
 * caller correct while retaining deterministic order for equal-sized leaves.
 */
export function squarify<T>(items: readonly T[], value: (item: T) => number, rect: Rect): Array<Placed<T>> {
  const placed: Array<Placed<T>> = [];
  const queue = items
    .map((item, index) => ({ item, index }))
    .filter(({ item }) => value(item) > 0)
    .sort((left, right) => value(right.item) - value(left.item) || left.index - right.index)
    .map(({ item }) => item);
  const totalValue = queue.reduce((sum, item) => sum + value(item), 0);
  if (!queue.length || totalValue <= 0 || rect.width <= 0 || rect.height <= 0) return placed;
  let free = { ...rect };
  let scale = (rect.width * rect.height) / totalValue;
  let index = 0;
  while (index < queue.length) {
    const short = Math.min(free.width, free.height);
    if (short <= 0) throw new Error("treemap exhausted its rectangle before placing every item");
    const row: T[] = [];
    let rowValue = 0;
    let best = Number.POSITIVE_INFINITY;
    while (index < queue.length) {
      const candidate = value(queue[index]) * scale;
      const nextValue = rowValue + candidate;
      const area = nextValue;
      const thickness = area / short;
      const worstNext = row.concat(queue[index]).reduce((worst, item) => {
        const side = (value(item) * scale) / thickness;
        return Math.max(worst, Math.max(thickness / side, side / thickness));
      }, 0);
      if (row.length && worstNext > best) break;
      row.push(queue[index]);
      rowValue = nextValue;
      best = worstNext;
      index++;
    }
    const finalRow = index === queue.length;
    const horizontal = free.width >= free.height;
    const thickness = finalRow ? (horizontal ? free.width : free.height) : rowValue / short;
    const rawRowValue = row.reduce((sum, item) => sum + value(item), 0);
    let offset = 0;
    row.forEach((item, rowIndex) => {
      // Shared final boundaries prevent accumulated floating-point drift from
      // producing hairline gaps. The final row consumes the exact remainder.
      const side = rowIndex === row.length - 1
        ? short - offset
        : finalRow
        ? short * value(item) / rawRowValue
        : (value(item) * scale) / thickness;
      placed.push({
        item,
        rect: horizontal
          ? { x: free.x, y: free.y + offset, width: thickness, height: side }
          : { x: free.x + offset, y: free.y, width: side, height: thickness },
      });
      offset += side;
    });
    // The row consumed exactly its own area, so the value-to-area scale still
    // holds for the rectangle that is left.
    free = horizontal
      ? { x: free.x + thickness, y: free.y, width: free.width - thickness, height: free.height }
      : { x: free.x, y: free.y + thickness, width: free.width, height: free.height - thickness };
    if (finalRow) break;
  }
  if (placed.length !== queue.length) throw new Error("treemap did not place every positive item");
  return placed;
}

// ---------------------------------------------------------------- rendering
//
// The drawing is a dark product-dashboard card: a black surface, hairline
// borders, one accent per category, and monospaced addresses. It reads the same on
// a light or a dark README because it carries its own surface.

const SURFACE = "#000000";
const CARD = "#0a0a0a";
const BORDER = "#262626";
const HAIRLINE = "#1f1f1f";
const TEXT = "#ededed";
const MUTED = "#8f8f8f";
const SANS = "'Geist Sans', Inter, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif";
const MONO = "'Geist Mono', ui-monospace, SFMono-Regular, 'SF Mono', Menlo, Consolas, monospace";

function escapeText(value: string): string {
  return value.replace(/[&<>"']/g, (character) => ({
    "&": "&amp;",
    "<": "&lt;",
    ">": "&gt;",
    '"': "&quot;",
    "'": "&apos;",
  })[character] ?? character);
}

function commas(value: number): string {
  return Math.round(value).toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
}

function round(value: number): number {
  return Math.round(value * 10) / 10;
}

function megabytes(value: number): string {
  return `${(value / (1024 * 1024)).toFixed(2)} MB`;
}

/** Longest prefix of `text` that fits `width` at the given font size. */
function fitText(text: string, width: number, fontSize: number): string | undefined {
  const perCharacter = fontSize * 0.58;
  const room = Math.floor(width / perCharacter);
  if (room < 3) return undefined;
  return text.length <= room ? text : `${text.slice(0, Math.max(room - 1, 1))}…`;
}

function rect(
  area: Rect,
  fill: string,
  options: { stroke?: string; radius?: number; opacity?: number } = {},
): string {
  const radius = options.radius ? ` rx="${options.radius}"` : "";
  const stroke = options.stroke ? ` stroke="${options.stroke}" stroke-width="1"` : "";
  const opacity = options.opacity === undefined ? "" : ` opacity="${options.opacity}"`;
  return `<rect x="${round(area.x)}" y="${round(area.y)}" width="${round(area.width)}" ` +
    `height="${round(area.height)}" fill="${fill}"${stroke}${radius}${opacity}/>`;
}

function label(
  x: number,
  y: number,
  text: string,
  options: {
    size?: number;
    fill?: string;
    anchor?: "start" | "middle" | "end";
    weight?: number;
    tracking?: number;
    mono?: boolean;
    opacity?: number;
  } = {},
): string {
  const parts = [
    `x="${round(x)}"`,
    `y="${round(y)}"`,
    `font-size="${options.size ?? 11}"`,
    `fill="${options.fill ?? TEXT}"`,
  ];
  if (options.anchor && options.anchor !== "start") parts.push(`text-anchor="${options.anchor}"`);
  if (options.weight) parts.push(`font-weight="${options.weight}"`);
  if (options.tracking) parts.push(`letter-spacing="${options.tracking}"`);
  if (options.mono) parts.push(`font-family="${MONO}"`);
  if (options.opacity !== undefined) parts.push(`opacity="${options.opacity}"`);
  return `<text ${parts.join(" ")}>${escapeText(text)}</text>`;
}

function tileRects(tile: Tile, area: Rect, lines: string[]): void {
  // A one-pixel gutter turns the treemap into separate cards instead of a
  // grid of hairlines.
  const body: Rect = {
    x: area.x + 0.5,
    y: area.y + 0.5,
    width: Math.max(area.width - 1, 0.5),
    height: Math.max(area.height - 1, 0.5),
  };
  let offset = 0;
  let midpointCategory: CoverageCategory = "assembly";
  for (const category of CATEGORY_ORDER) {
    const share = tile.categories[category] ?? 0;
    if (share <= 0) continue;
    // Categories stack from the bottom of the tile, so a converting region reads as
    // a filling gauge rather than as a different region.
    const height = body.height * (share / tile.bytes);
    lines.push(rect(
      { x: body.x, y: body.y + body.height - offset - height, width: body.width, height },
      CATEGORY_STYLE[category].fill,
    ));
    // The label sits on the tile's midline, so it takes its contrast from the
    // category that actually lies under it rather than from the largest one.
    if (offset <= body.height / 2 && body.height / 2 < offset + height) midpointCategory = category;
    offset += height;
  }
  const text = body.height >= 12 ? fitText(tile.label, body.width - 6, 9) : undefined;
  if (text === tile.label) {
    lines.push(label(body.x + body.width / 2, body.y + body.height / 2 + 3.2, text, {
      size: 9,
      anchor: "middle",
      // Addresses and resource ids are monospaced; prose bucket names are not.
      mono: /^[0-9a-f]+(–[0-9a-f]+)?$/i.test(tile.label),
      fill: CATEGORY_STYLE[midpointCategory].ink,
      opacity: 0.9,
    }));
  }
}

function card(
  title: string,
  meta: string,
  areas: readonly Area[],
  frame: Rect,
  lines: string[],
): void {
  lines.push(
    rect(frame, CARD, { stroke: BORDER, radius: 8 }),
    label(frame.x + 16, frame.y + 25, title.toUpperCase(), {
      size: 10,
      fill: MUTED,
      tracking: 0.9,
    }),
    label(frame.x + frame.width - 16, frame.y + 25, meta, { size: 10, fill: MUTED, anchor: "end" }),
  );
  const body: Rect = {
    x: frame.x + 12,
    y: frame.y + 40,
    width: frame.width - 24,
    height: frame.height - 52,
  };
  const placedAreas = squarify(
    [...areas].sort((left, right) => right.bytes - left.bytes || left.id.localeCompare(right.id)),
    (item) => item.bytes,
    body,
  );
  for (const { item, rect: areaRect } of placedAreas) {
    // Groups are separated by their gutter and their own label rather than by
    // a frame; one fewer box per group, same reading.
    const inset: Rect = {
      x: areaRect.x + 6,
      y: areaRect.y + 6,
      width: areaRect.width - 12,
      height: areaRect.height - 12,
    };
    const size = `${commas(item.bytes)} B`;
    const sizeFits = inset.width >= 190;
    const heading = fitText(
      item.label.toUpperCase(),
      inset.width - (sizeFits ? size.length * 5.2 + 16 : 4),
      9.5,
    );
    if (heading) {
      lines.push(label(inset.x, inset.y + 8, heading, { size: 9.5, fill: MUTED, tracking: 0.8 }));
    }
    if (sizeFits) {
      lines.push(label(inset.x + inset.width, inset.y + 8, size, {
        size: 9.5,
        fill: MUTED,
        anchor: "end",
        mono: true,
      }));
    }
    const tiles: Rect = {
      x: inset.x,
      y: inset.y + 16,
      width: inset.width,
      height: inset.height - 16,
    };
    for (const placed of squarify(item.tiles, (tile) => tile.bytes, tiles)) {
      tileRects(placed.item, placed.rect, lines);
    }
  }
}

/** A stacked, rounded category bar across the whole executable denominator. */
function categoryBar(map: CoverageMap, frame: Rect, lines: string[]): void {
  const clip = "category-bar-clip";
  lines.push(
    `<defs><clipPath id="${clip}"><rect x="${frame.x}" y="${frame.y}" width="${frame.width}" ` +
    `height="${frame.height}" rx="${frame.height / 2}"/></clipPath></defs>`,
    `<g clip-path="url(#${clip})">`,
    rect(frame, HAIRLINE),
  );
  let cursor = frame.x;
  for (const category of ["exact_c", "semantic_c", "assembly", "retained_asm"] as CoverageCategory[]) {
    const categoryWidth = frame.width * (map.categories[category].bytes / map.executable_bytes);
    lines.push(rect({ ...frame, x: cursor, width: categoryWidth }, CATEGORY_STYLE[category].fill));
    cursor += categoryWidth;
  }
  lines.push("</g>");
}

export function renderSvg(map: CoverageMap): string {
  const width = 1200;
  // Tall enough that the ROM card stays portrait: its two code areas are an
  // eighth of the ROM image, and only a portrait card gives them a readable
  // band instead of a sliver.
  const height = 660;
  const margin = 32;
  const lines: string[] = [];
  const exact = map.categories.exact_c;
  const semantic = map.categories.semantic_c;
  const combined = exact.bytes + semantic.bytes;

  lines.push(
    // Intrinsic width and height as well as the viewBox: an <img> that is only
    // given a viewBox falls back to a 2:1 default ratio in some engines and
    // letterboxes the drawing.
    `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 ${width} ${height}" ` +
    `width="${width}" height="${height}" font-family="${SANS}" role="img" ` +
    `aria-label="Coverage treemap of the English Golden Sun ROM image: ` +
    `${roundHalfUpPercent(combined, map.executable_bytes)}% of the ` +
    `${commas(map.executable_bytes)} audited executable bytes are expressed as C">`,
    rect({ x: 0, y: 0, width, height }, SURFACE),
    label(margin, 46, "Alchemy — Golden Sun", { size: 15, weight: 600, tracking: -0.2 }),
    label(margin, 64, `${map.target} · ${megabytes(map.rom_bytes)} ROM image`, {
      size: 10,
      fill: MUTED,
    }),
    label(width - margin, 46, `${roundHalfUpPercent(combined, map.executable_bytes)}%`, {
      size: 26,
      anchor: "end",
      weight: 600,
      tracking: -0.6,
    }),
    // Naming the hero number as a sum keeps it from reading as the headline
    // metric, which is byte-exact C alone.
    label(width - margin, 64, "EXACT + SEMANTIC C", {
      size: 9.5,
      fill: MUTED,
      anchor: "end",
      tracking: 0.9,
    }),
  );

  // The metric row doubles as the legend: one dot, one category, one number.
  const metrics: Array<{ category: CoverageCategory; value: string; note?: string }> = [
    { category: "exact_c", value: `${exact.percent_of_executable}%`, note: "FULL-C BYTE SHARE" },
    { category: "semantic_c", value: `${semantic.percent_of_executable}%` },
    { category: "assembly", value: `${map.categories.assembly.percent_of_executable}%` },
    { category: "retained_asm", value: `${map.categories.retained_asm.percent_of_executable}%`, note: "BY DESIGN" },
    { category: "asset_data", value: megabytes(map.categories.asset_data.bytes), note: "NOT CODE" },
  ];
  const columnWidth = (width - margin * 2) / metrics.length;
  metrics.forEach((metric, index) => {
    const x = margin + index * columnWidth;
    const name = CATEGORY_STYLE[metric.category].label.toUpperCase() +
      (metric.note ? ` · ${metric.note}` : "");
    lines.push(
      rect({ x, y: 96, width: 8, height: 8 }, CATEGORY_STYLE[metric.category].fill, { radius: 2 }),
      label(x + 14, 104, name, { size: 9.5, fill: MUTED, tracking: 0.8 }),
      label(x, 128, metric.value, { size: 21, weight: 600, tracking: -0.4 }),
    );
  });

  categoryBar(map, { x: margin, y: 146, width: width - margin * 2, height: 6 }, lines);

  const cardTop = 176;
  const cardHeight = height - cardTop - 44;
  card(
    "ROM image",
    `${megabytes(map.rom_bytes)} in the ROM image`,
    map.rom_areas,
    { x: margin, y: cardTop, width: 372, height: cardHeight },
    lines,
  );
  card(
    "Audited executable universe",
    `${commas(map.executable_bytes)} bytes of code`,
    map.executable_areas,
    { x: margin + 388, y: cardTop, width: width - margin * 2 - 388, height: cardHeight },
    lines,
  );

  lines.push(
    label(margin, height - 20, "Tiles are main-image address bands and one per code overlay; compressed " +
      "code-overlay tiles are sized by ROM-image bytes and shaded by decoded coverage category.", {
      size: 9.5,
      fill: MUTED,
    }),
    label(width - margin, height - 20,
      `exact C: ${map.provenance.exact_source} · semantic C: ${map.provenance.semantic_source}`, {
        size: 9.5,
        fill: MUTED,
        anchor: "end",
      }),
    "</svg>",
  );
  return lines.join("\n") + "\n";
}

// ------------------------------------------------------------------- driver

function mapPath(target: DecompTargetId): string {
  return join(ROOT, "metrics", `${target}-coverage-map.json`);
}

/**
 * GitHub proxies README images through its own cache, which serves a stale
 * picture long after the SVG changes. A query parameter defeats that, but only
 * if it actually moves — so it is derived from the SVG's own bytes rather than
 * from a timestamp. Identical picture, identical URL: no spurious diffs on a
 * redraw that changed nothing. Different picture, different URL, immediately.
 */
// Per-tree colour bands, perceptually equalized: every tree renders at ONE
// OKLCH lightness (L=0.70), so no hue reads brighter than another, and
// completion is encoded as CHROMA — full completion sits at the hue's
// Display-P3 gamut edge at that lightness, zero completion is neutral gray at
// the same lightness. An hsl() approximation rides the CSS cascade as the
// fallback for engines without oklch().
interface HueBand { hslHue: number; okHue: number; okCmax: number; edge: string }
const CORE_HUE: HueBand = { hslHue: 275, okHue: 295, okCmax: 0.26, edge: "#6d4fc2" };
const OVERLAY_HUE: HueBand = { hslHue: 190, okHue: 200, okCmax: 0.17, edge: "#1f7f93" };
const ASSET_HUE: HueBand = { hslHue: 330, okHue: 355, okCmax: 0.30, edge: "#bb2f77" };
const GROUND = "#ffffff";
const OK_LIGHTNESS = 0.70;
// Completion fractions: code ladder assembly 0 -> semantic 0.5 -> exact 0.75
// -> humanized 1 (empty until real humanization lands). Assets ladder ends at
// individual objects; byte-represented keeps a faint tint floor.
const CODE_FRACTION: Record<string, number> = {
  humanized_c: 1, exact_c: 0.75, semantic_c: 0.5, assembly: 0, retained_asm: 0,
};
export const ASSET_TIERS = ["asset_bytes", "asset_bw", "asset_color", "asset_objects"] as const;
export type AssetTier = (typeof ASSET_TIERS)[number];
const ASSET_FRACTION: Record<string, number> = {
  asset_bytes: 0.08, asset_unclassified: 0.08, asset_bw: 0.34, asset_color: 0.67, asset_objects: 1,
  asset_data: 0.08,
};
const BOX_TREE_LEGEND: Record<string, string> = {
  exact_c: "Exact",
  semantic_c: "Semantic",
  assembly: "Unknown",
  retained_asm: "Assembly",
  asset_objects: "Objects",
  asset_color: "Color images",
  asset_bw: "B&W",
  asset_bytes: "Encoded bytes",
  asset_unclassified: "Unclassified",
  asset_data: "Data / assets",
};

// The 540 px README canvas and the one permitted 16 px pixel font are both
// fixed. These shorter forms leave room for an honest one-decimal percentage
// without shrinking or horizontally scaling Weyard.
const BOX_TREE_COMPACT_LEGEND: Record<string, string> = {
  asset_color: "Color",
  asset_bytes: "Encoded",
  asset_unclassified: "Unknown",
};

let weyardFontBase64: string | undefined;
function embeddedWeyardFont(): string {
  weyardFontBase64 ??= readFileSync(join(ROOT, "assets", "fonts", "weyard.otf")).toString("base64");
  return weyardFontBase64;
}

/** Representation-form tier for one package: package kind first (music,
 * samples and soundfonts are .mid/.wav/.sf2-backed object corpora even when
 * their indexes reference sources by prefix), then source names. */
export function assetTierOf(sources: readonly string[], kind = ""): AssetTier {
  if (/music|sequence|midi|wave|pcm|sound-?font|audio/i.test(kind)) return "asset_objects";
  if (/sprite|character-catalog|chr/i.test(kind) && sources.some((name) => /koma_|frame_/i.test(name))) {
    return "asset_objects";
  }
  let tier: AssetTier = "asset_bytes";
  const rank = (t: AssetTier) => ASSET_TIERS.indexOf(t);
  for (const name of sources) {
    let seen: AssetTier | undefined;
    if (/\.(mid|wav|sf2|pcm)$/i.test(name)) seen = "asset_objects";
    else if (/koma_\d+\.png$|frame_\d+\.png$|object[^/]*\.png$/i.test(name)) seen = "asset_objects";
    else if (/(1bpp|mask|value_low|value_high|grid_sentinels)[^/]*\.png$/i.test(name)) seen = "asset_bw";
    else if (/\.png$/i.test(name)) seen = "asset_color";
    else if (/\.(bin|tilemap)$/i.test(name)) seen = "asset_bytes";
    if (seen !== undefined && rank(seen) > rank(tier)) tier = seen;
  }
  return tier;
}

/** Asset maturity tiles: same buckets as the ROM card, categories keyed by tier. */
export function assetMaturityTiles(tree: SourceTree): Tile[] {
  const manifest = readJson(tree, "assets/manifest.json") as Record<string, unknown>;
  const buckets = new Map<string, Tile>();
  const visited = new Set<string>();
  const record = (kind: string, size: number, sources: readonly string[], rawKind = ""): void => {
    if (size <= 0) return;
    const bucket = assetBucket(kind);
    // Music, samples and soundfonts live as per-object .mid/.wav corpora even
    // where package indexes name them by prefix, so the audio bucket is
    // object-tier by construction.
    const tier = bucket.id === "audio" ? "asset_objects" : assetTierOf(sources, rawKind || kind);
    const tile = buckets.get(bucket.id) ?? { label: bucket.label, bytes: 0, categories: {} };
    tile.bytes += size;
    (tile.categories as Record<string, number>)[tier] =
      ((tile.categories as Record<string, number>)[tier] ?? 0) + size;
    buckets.set(bucket.id, tile);
  };
  const gatherSources = (node: unknown, out: string[]): void => {
    if (Array.isArray(node)) { for (const child of node) gatherSources(child, out); return; }
    if (node === null || typeof node !== "object") {
      if (typeof node !== "string") return;
      if (/\.json$/i.test(node) && !visited.has(node)) {
        // Follow nested plans (bank.json, index.json chains) so per-object
        // sources — koma frames, .mid tracks, .wav samples — are seen even
        // when they sit two or three references deep.
        visited.add(node);
        const text = tree.read(node);
        if (text !== undefined) {
          try { gatherSources(JSON.parse(text), out); } catch { /* no label */ }
        }
        return;
      }
      if (/\.[a-z0-9]{2,4}$/i.test(node)) out.push(node);
      return;
    }
    for (const value of Object.values(node as Record<string, unknown>)) gatherSources(value, out);
  };
  const visit = (node: unknown, kind: string): void => {
    if (Array.isArray(node)) { for (const child of node) visit(child, kind); return; }
    if (node === null || typeof node !== "object") return;
    const item = node as Record<string, unknown>;
    const local = typeof item.kind === "string" ? item.kind : kind;
    const size = hexValue(item.size);
    if (size !== undefined && size > 0) {
      const sources: string[] = [];
      gatherSources(item, sources);
      for (const reference of [item.index, item.source, item.plan]) {
        if (typeof reference !== "string" || !reference.endsWith(".json")) continue;
        if (visited.has(reference)) continue;
        visited.add(reference);
        const text = tree.read(reference);
        if (text !== undefined) {
          try { gatherSources(JSON.parse(text), sources); } catch { /* no label */ }
        }
      }
      record(local, size, sources, local);
    }
    for (const [key, value] of Object.entries(item)) {
      if (key !== "components") visit(value, local);
    }
  };
  visit(manifest, "asset");
  return [...buckets.values()].filter((tile) => tile.bytes > 0);
}

/** Logical folder keys carried by addressed leaves in the 16:9 box trees. */
export type TileFolder = "group" | "subgroup";

type BoxTreeNode =
  | { node: "group"; id: string; label: string; children: BoxTreeNode[] }
  | { node: "leaf"; id: string; tile: Tile };

function boxTreeNodes(
  tiles: readonly Tile[],
  folders: readonly TileFolder[],
  depth = 0,
  path = "root",
): BoxTreeNode[] {
  if (depth >= folders.length) {
    return tiles.map((tile, index) => ({
      node: "leaf",
      id: `${path}/leaf-${index}-${tile.address === undefined ? "na" : hex8(tile.address)}`,
      tile,
    }));
  }
  const key = folders[depth];
  const groups = new Map<string, Tile[]>();
  const direct: Tile[] = [];
  for (const tile of tiles) {
    const label = tile[key];
    if (label === undefined) {
      direct.push(tile);
      continue;
    }
    groups.set(label, [...(groups.get(label) ?? []), tile]);
  }
  return [
    ...boxTreeNodes(direct, folders, folders.length, `${path}/direct`),
    ...[...groups].map(([label, children], index) => ({
    node: "group",
    id: `${path}/${depth}-${index}`,
    label,
    children: boxTreeNodes(children, folders, depth + 1, `${path}/${depth}-${index}`),
    }) as BoxTreeNode),
  ];
}

function boxNodeBytes(node: BoxTreeNode): number {
  return node.node === "leaf"
    ? node.tile.bytes
    : node.children.reduce((sum, child) => sum + boxNodeBytes(child), 0);
}

function folderDisplayName(label: string): string {
  if (/^resource_[0-9a-f]+$/i.test(label)) return label.replace(/^resource_/, "").toUpperCase();
  return label
    .replace(/^golden-sun-/, "")
    .replace(/^gba-/, "GBA ")
    .replaceAll("-", " ")
    .replace(/\b\w/g, (character) => character.toUpperCase());
}

function precise(value: number): string {
  const rounded = Math.round(value * 1_000_000) / 1_000_000;
  return Object.is(rounded, -0) ? "0" : String(rounded);
}

function preciseRect(rectangle: Rect, attributes: string): string {
  const x = Math.round(rectangle.x * 1_000_000) / 1_000_000;
  const y = Math.round(rectangle.y * 1_000_000) / 1_000_000;
  const right = Math.round((rectangle.x + rectangle.width) * 1_000_000) / 1_000_000;
  const bottom = Math.round((rectangle.y + rectangle.height) * 1_000_000) / 1_000_000;
  return `<rect x="${precise(x)}" y="${precise(y)}" width="${precise(right - x)}" ` +
    `height="${precise(bottom - y)}" ${attributes}/>`;
}

function boxTreeCategoryBytes(area: Area, category: string): number {
  return area.tiles.reduce((sum, tile) => sum + (tile.categories[category] ?? 0), 0);
}

function boxTreePercent(bytes: number, total: number): string {
  if (total <= 0) return "0.0%";
  return `${(Math.round(bytes * 1_000 / total) / 10).toFixed(1)}%`;
}

/**
 * A recursive SpaceMonger-style box tree. Parent rectangles are real logical
 * owners and derive their byte size exclusively from their descendants. Leaves
 * have zero gutter and zero minimum size; folder outlines and labels overlay
 * the data field, so hierarchy never steals or invents byte area.
 */
export function renderBoxTree(
  area: Area,
  ariaLabel: string,
  hue: HueBand = CORE_HUE,
  categoryFraction: Record<string, number> = CODE_FRACTION,
  categoryOrder: readonly string[] = ["assembly", "retained_asm", "semantic_c", "exact_c", "humanized_c"],
  folders: readonly TileFolder[] = [],
  title = area.label,
): string {
  // This is also the CSS/native README size. Keeping the viewBox 1:1 is what
  // makes the one permitted 16px Weyard face pixel-exact instead of scaling it
  // down with a large responsive SVG canvas.
  const width = 540;
  const height = 304;
  const plot: Rect = { x: 3, y: 22, width: width - 6, height: height - 46 };
  const lines: string[] = [];
  lines.push(
    `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 ${width} ${height}" ` +
    `width="${width}" height="${height}" shape-rendering="crispEdges" role="img" ` +
    `aria-label="${escapeText(ariaLabel)}">`,
    "<defs><style>" +
      `@font-face{font-family:Weyard;src:url(data:font/otf;base64,${embeddedWeyardFont()}) format('opentype');font-style:italic;}` +
      ".weyard{font-family:Weyard;font-size:16px;font-style:italic;fill:#fff;}" +
    "</style></defs>",
    preciseRect({ x: 0, y: 0, width, height }, `fill="${hue.edge}" rx="8"`),
    preciseRect({ x: 1, y: 1, width: width - 2, height: height - 2 },
      `fill="none" stroke="#1c1c26" stroke-width="2" rx="7"`),
    preciseRect(plot, `fill="${GROUND}" rx="3"`),
  );
  const cellAttributes = (fraction: number): string => {
    // One ramp for every hue: lightness falls 0.93 -> 0.55 and chroma rises to
    // the hue's P3 edge as completion rises, so tiers separate strongly while
    // no hue reads brighter than another at the same tier.
    const light = 0.93 - 0.38 * fraction;
    const hsl = `hsl(${hue.hslHue}, ${Math.round(fraction * 95)}%, ${Math.round(93 - 38 * fraction)}%)`;
    const ok = `oklch(${light.toFixed(3)} ${(hue.okCmax * fraction).toFixed(3)} ${hue.okHue})`;
    return `style="fill:${hsl};fill:${ok}"`;
  };
  const cellRect = (rectangle: Rect, fraction: number): string =>
    preciseRect(rectangle, cellAttributes(fraction));
  const drawLeaf = (tile: Tile, rectangle: Rect): void => {
    const categories = tile.categories as Record<string, number>;
    const populated = categoryOrder.filter((category) => (categories[category] ?? 0) > 0);
    const total = populated.reduce((sum, category) => sum + (categories[category] ?? 0), 0);
    lines.push(`<g data-byte-leaf="true" aria-label="${escapeText(`${tile.label}: ${commas(tile.bytes)} bytes`)}">`,
      `<title>${escapeText(`${tile.label}: ${commas(tile.bytes)} bytes`)}</title>`);
    if (total <= 0) {
      lines.push(cellRect(rectangle, categoryFraction[categoryOrder.at(-1) ?? ""] ?? 0.08), "</g>");
      return;
    }
    let credited = 0;
    populated.forEach((category, index) => {
      const top = rectangle.y + rectangle.height * (credited / total);
      credited += categories[category] ?? 0;
      const bottom = index === populated.length - 1
        ? rectangle.y + rectangle.height
        : rectangle.y + rectangle.height * (credited / total);
      const band = { x: rectangle.x, y: top, width: rectangle.width, height: bottom - top };
      if (category === "retained_asm") {
        lines.push(preciseRect(band, `style="fill:${RETAINED_ASM_FILL}"`));
      } else {
        lines.push(cellRect(band, categoryFraction[category] ?? 0.08));
      }
    });
    lines.push("</g>");
  };
  let folderSequence = 0;
  const drawNodes = (nodes: readonly BoxTreeNode[], rectangle: Rect, depth: number): void => {
    for (const placed of squarify(nodes, boxNodeBytes, rectangle)) {
      const node = placed.item;
      if (node.node === "leaf") {
        drawLeaf(node.tile, placed.rect);
        continue;
      }
      const bytes = boxNodeBytes(node);
      const folderDepth = depth + 1;
      lines.push(`<g data-folder-depth="${folderDepth}" aria-label="${escapeText(`${node.label}: ${commas(bytes)} bytes`)}">`,
        `<title>${escapeText(`${node.label}: ${commas(bytes)} bytes`)}</title>`);
      drawNodes(node.children, placed.rect, folderDepth);
      const stroke = folderDepth === 1 ? 2 : 1;
      lines.push(preciseRect(placed.rect,
        `fill="none" stroke="hsl(${hue.hslHue} 70% 24%)" stroke-width="${stroke}" vector-effect="non-scaling-stroke"`));
      const showLabel = placed.rect.height >= 18 &&
        placed.rect.width >= (folderDepth === 1 ? 50 : 120);
      if (showLabel) {
        const id = `folder-clip-${folderSequence++}`;
        const headerHeight = Math.min(18, placed.rect.height);
        lines.push(
          `<clipPath id="${id}">${preciseRect(placed.rect, "")}</clipPath>`,
          preciseRect({ ...placed.rect, height: headerHeight },
            `fill="hsl(${hue.hslHue} 70% 24%)" fill-opacity="0.9"`),
          `<text class="weyard" x="${precise(Math.round(placed.rect.x) + 3)}" ` +
            `y="${precise(Math.round(placed.rect.y) + 15)}" clip-path="url(#${id})">` +
            `${escapeText(folderDisplayName(node.label))}</text>`,
        );
      }
      lines.push("</g>");
    }
  };
  const nodes = boxTreeNodes(area.tiles, folders);
  if (nodes.reduce((sum, node) => sum + boxNodeBytes(node), 0) !== area.bytes) {
    throw new Error(`${area.id} hierarchy does not conserve its byte total`);
  }
  drawNodes(nodes, plot, 0);
  lines.push(`<text class="weyard" x="6" y="17">${escapeText(title.toUpperCase())}</text>`);
  const completionCategory = categoryOrder.includes("exact_c")
    ? "exact_c"
    : categoryOrder.includes("asset_objects")
    ? "asset_objects"
    : categoryOrder[0];
  if (completionCategory !== undefined) {
    const completionLabel = completionCategory === "exact_c"
      ? "EXACT"
      : (BOX_TREE_LEGEND[completionCategory] ?? completionCategory).toUpperCase();
    // Retained assembly is source-owned and byte-exact too; it stays orange so
    // readers can distinguish its representation, but it belongs in the title
    // bar's exact-completion total rather than in unknown/decompilation debt.
    const completionBytes = boxTreeCategoryBytes(area, completionCategory) +
      (completionCategory === "exact_c" ? boxTreeCategoryBytes(area, "retained_asm") : 0);
    const completion = boxTreePercent(completionBytes, area.bytes);
    lines.push(`<text class="weyard" x="${width - 6}" y="17" text-anchor="end">` +
      `${escapeText(`${completionLabel} ${completion}`)}</text>`);
  }
  let legendX = 6;
  const legendCenterY = height - 11;
  for (const category of categoryOrder) {
    const legendLabel = BOX_TREE_LEGEND[category];
    if (legendLabel === undefined) continue;
    const percentage = boxTreePercent(boxTreeCategoryBytes(area, category), area.bytes);
    const displayLabel = `${BOX_TREE_COMPACT_LEGEND[category] ?? legendLabel} ${percentage}`;
    const swatch = { x: legendX, y: legendCenterY - 5, width: 10, height: 10 };
    lines.push(category === "retained_asm"
      ? preciseRect(swatch, `fill="${RETAINED_ASM_FILL}"`)
      : preciseRect(swatch, cellAttributes(categoryFraction[category] ?? 0.08)));
    lines.push(`<text class="weyard" x="${precise(legendX + 14)}" y="${legendCenterY}" ` +
      `dominant-baseline="middle">${escapeText(displayLabel)}</text>`);
    legendX += 16 + displayLabel.length * 8;
  }
  lines.push("</svg>");
  return lines.join("\n");
}

export const BOX_TREES = ["core", "overlays", "assets"] as const;
export type BoxTreeId = (typeof BOX_TREES)[number];

export function boxTreePath(target: DecompTargetId, tree: BoxTreeId): string {
  return join(ROOT, "assets", "readme", `${target}-${tree}.svg`);
}

export function renderBoxTrees(
  map: CoverageMap,
  tree?: SourceTree,
  preferVerifiedAssets = false,
): Record<BoxTreeId, string> {
  const core = map.executable_areas.find((item) => item.id === "main");
  const overlays = map.executable_areas.find((item) => item.id === "overlays");
  const romData = map.rom_areas.find((item) => item.id === "rom-data");
  if (!core || !overlays || !romData) throw new Error("coverage map is missing a box-tree area");
  const embeddedMaturity = romData.tiles.filter((tile) =>
    ASSET_TIERS.some((tier) => (tile.categories[tier] ?? 0) > 0) ||
    (tile.categories.asset_unclassified ?? 0) > 0);
  const verifiedMaturity = preferVerifiedAssets && tree !== undefined
    ? verifiedAssetTiles(tree, map.target)
    : undefined;
  // New maps carry exact addressed package leaves. The fallback keeps older
  // tracked/synthetic maps renderable, but the live dashboard never collapses
  // those leaves back into the seven historical display buckets.
  const maturity = verifiedMaturity ?? (embeddedMaturity.length > 0
    ? embeddedMaturity
    : tree ? assetMaturityTiles(tree) : []);
  const cataloguedBytes = maturity.reduce(
    (sum, tile) => sum + ((tile.categories.asset_unclassified ?? 0) > 0 ? 0 : tile.bytes),
    0,
  );
  const embeddedUnclassified = maturity.reduce(
    (sum, tile) => sum + (tile.categories.asset_unclassified ?? 0), 0);
  const unclassifiedBytes = verifiedMaturity !== undefined || embeddedMaturity.length > 0
    ? embeddedUnclassified
    : Math.max(romData.bytes - cataloguedBytes, 0);
  // The maturity census is manifest-backed and intentionally does not claim
  // unknown ROM data as decoded. Keep that remainder in the tree at the
  // byte-represented floor, rather than silently drawing a smaller universe or
  // crediting it to a higher maturity tier.
  const maturityWithRemainder = verifiedMaturity !== undefined || embeddedMaturity.length > 0
    ? maturity
    : tree !== undefined && unclassifiedBytes
    ? [...maturity, {
        label: "Unclassified ROM-image data (byte-represented)",
        bytes: unclassifiedBytes,
        categories: { asset_unclassified: unclassifiedBytes },
      }]
    : maturity;
  const detailedAssets = tree !== undefined || embeddedMaturity.length > 0 || verifiedMaturity !== undefined;
  const assetsArea: Area = detailedAssets
    ? { id: "rom-data", label: romData.label, bytes: romData.bytes,
        categories: {}, tiles: maturityWithRemainder }
    : romData;
  return {
    core: renderBoxTree(core,
      "Main-image code coverage box tree, purple band; 64 KiB address banks contain audited source-owner leaves at their natural executable byte size",
      CORE_HUE, CODE_FRACTION,
      ["assembly", "retained_asm", "semantic_c", "exact_c", "humanized_c"], ["group"], "Main image"),
    overlays: renderBoxTree(overlays,
      "Decoded code-overlay coverage box tree, cyan band; each resource contains its exact, semantic, and unowned source regions",
      OVERLAY_HUE, CODE_FRACTION,
      ["assembly", "retained_asm", "semantic_c", "exact_c", "humanized_c"], ["group"], "Code overlays"),
    assets: renderBoxTree(assetsArea,
      detailedAssets
        ? `Asset maturity box tree, pink band; ${commas(cataloguedBytes)} catalogued bytes are tiered by tracked sources and ${commas(unclassifiedBytes)} unclassified ROM-image data bytes remain at the byte-represented floor`
        : "Asset maturity box tree, pink band",
      ASSET_HUE, ASSET_FRACTION,
      detailedAssets ? ["asset_unclassified", "asset_bytes", "asset_bw", "asset_color", "asset_objects"] : ["asset_data"],
      maturityWithRemainder.some((tile) => tile.group !== undefined)
        ? ["group", "subgroup"]
        : [], "Data / assets"),
  };
}

export function svgCacheVersion(svg: string): string {
  return createHash("sha1").update(svg).digest("hex").slice(0, 8);
}

/** Rewrite the README's embeds to carry per-tree versions, replacing any present. */
export function readmeWithCacheBuster(
  readme: string,
  target: DecompTargetId,
  versions: Record<BoxTreeId, string>,
): string {
  let out = readme;
  for (const tree of BOX_TREES) {
    const file = `assets/readme/${target}-${tree}.svg`;
    const escaped = file.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
    out = out.replace(
      new RegExp(`\\(${escaped}(?:\\?v=[0-9a-f]*)?\\)`, "g"),
      `(${file}?v=${versions[tree]})`,
    );
  }
  return out;
}

function readmePath(): string {
  return join(ROOT, "README.md");
}

function svgPath(target: DecompTargetId): string {
  return join(ROOT, "assets", "readme", `${target}-coverage.svg`);
}

/**
 * Resolve the tree containing semantic sources. A requested or recorded ref
 * wins; otherwise a work tree that contains semantic sources describes itself.
 */
export function resolveSemanticTree(
  exact: SourceTree,
  requested?: string,
  recorded?: string,
): SourceTree | undefined {
  if (requested === "none") return undefined;
  // A recorded source wins over the describes-itself heuristic so regenerating
  // the map is stable when an explicit external ref was previously selected.
  const wanted = requested ??
    (recorded && !["worktree", "absent", "none"].includes(recorded) ? recorded : undefined);
  if (wanted) {
    const tree = wanted === "worktree" ? workTree() : refTree(wanted);
    if (!tree) throw new Error(`cannot resolve semantic source ref ${wanted}`);
    return tree;
  }
  if (exact.list("semantic/main").some((name) => MAIN_SOURCE.test(name))) return exact;
  return undefined;
}

/**
 * Resolve the tree containing byte-exact sources. An explicit ref wins; pass
 * `worktree` to force the current tree. An unavailable recorded ref is an
 * error because falling back could silently republish an older measurement.
 */
export function resolveExactTree(requested?: string, recorded?: string): SourceTree {
  const wanted = requested ?? (recorded && recorded !== "worktree" ? recorded : undefined);
  if (wanted === undefined || wanted === "worktree") return workTree();
  const tree = refTree(wanted);
  if (!tree) {
    throw new Error(
      `exact source ref ${wanted} is not available here; run: ` +
      `git fetch origin ${wanted.replace(/^origin\//, "")} ` +
      `(or --exact-ref worktree to draw this tree's own exact C)`,
    );
  }
  return tree;
}

/**
 * A tree that cannot resolve the recorded semantic source must not publish its
 * coverage as zero. Returns a refusal message unless the source resolved, was
 * explicitly disabled, or the tracked map contains no semantic coverage.
 */
export function semanticEraseRefusal(
  resolved: boolean,
  requested: string | undefined,
  tracked: CoverageMap | undefined,
): string | undefined {
  if (resolved || requested === "none") return undefined;
  const bytes = tracked?.categories.semantic_c.bytes ?? 0;
  if (bytes <= 0) return undefined;
  return `refusing to erase semantic coverage: the tracked map records ${bytes} ` +
    `semantic bytes from ${tracked?.provenance.semantic_source}, which is not available ` +
    `here; fetch that ref or pass --semantic-ref none to publish without it`;
}

interface Options {
  target: DecompTargetId;
  exact?: string;
  semantic?: string;
  write: boolean;
  check: boolean;
  selfTest: boolean;
}

function optionsOf(argv: string[]): Options {
  const options: Options = { target: DEFAULT_TARGET, write: false, check: false, selfTest: false };
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "--target") options.target = parseDecompTarget(argv[++index]);
    else if (argument === "--exact-ref") options.exact = argv[++index];
    else if (argument === "--semantic-ref") options.semantic = argv[++index];
    else if (argument === "--write") options.write = true;
    else if (argument === "--check") options.check = true;
    else if (argument === "--self-test") options.selfTest = true;
    else if (argument === "-h" || argument === "--help") {
      console.log(
        "usage: coverage_map.ts [--target gs1-en|gs2-en] [--exact-ref <ref>|worktree] " +
        "[--semantic-ref <ref>|worktree|none] [--write|--check|--self-test]",
      );
      process.exit(0);
    } else throw new Error(`unrecognized argument: ${argument}`);
  }
  return options;
}

function summarize(map: CoverageMap): string {
  const combined = map.categories.exact_c.bytes + map.categories.semantic_c.bytes;
  return [
    `target=${map.target}`,
    `rom=${commas(map.rom_bytes)}`,
    `executable=${commas(map.executable_bytes)}`,
    `exact=${commas(map.categories.exact_c.bytes)} (${map.categories.exact_c.percent_of_executable}%)`,
    `semantic=${commas(map.categories.semantic_c.bytes)} (${map.categories.semantic_c.percent_of_executable}%)`,
    `combined=${commas(combined)} (${roundHalfUpPercent(combined, map.executable_bytes)}%)`,
    `semantic_source=${map.provenance.semantic_source}`,
  ].join(" ");
}

export function selfTest(): void {
  const expectReject = (action: () => unknown, label: string): void => {
    try {
      action();
    } catch {
      return;
    }
    throw new Error(`${label} was accepted`);
  };

  if (spanBytes(normalize([{ start: 0, end: 4 }, { start: 2, end: 8 }])) !== 8) {
    throw new Error("span normalization failed");
  }
  if (spanBytes(intersect([{ start: 0, end: 10 }], [{ start: 4, end: 6 }, { start: 8, end: 20 }])) !== 4) {
    throw new Error("span intersection failed");
  }
  const removed = subtract([{ start: 0, end: 10 }], [{ start: 2, end: 4 }]);
  if (removed.length !== 2 || removed[0].end !== 2 || removed[1].start !== 4) {
    throw new Error("span subtraction failed");
  }

  const boundaries = [0x08000000, 0x08000010, 0x08000030];
  const span = regionSpan(0x08000010, boundaries, 0x08000040);
  if (span.start !== 0x08000010 || span.end !== 0x08000030) throw new Error("region span failed");
  if (regionSpan(0x08000030, boundaries, 0x08000040).end !== 0x08000040) {
    throw new Error("final region span failed");
  }

  const placeholders = overlayPlaceholderSpans(
    "AlchemyC_02000010:\n\t.space 0x10\n.L_02000020:\n\tbx lr\nAlchemyC_02000030:\n\n\t.space 8\n\tpush\t{lr}\n",
  );
  if (placeholders.length !== 2 || spanBytes(placeholders) !== 24) {
    throw new Error("overlay placeholder measurement failed");
  }
  if (overlayPlaceholderSpans("\t.space 0x40\n").length !== 0) {
    throw new Error("assembly reservation counted as C");
  }

  if (assetBucket("golden-sun-pcm-wave-series").id !== "audio") throw new Error("audio bucket failed");
  if (assetBucket("brand-new-package").id !== "other") throw new Error("unknown bucket failed");

  // Whole-overlay semantic claims.
  const sourceTree = (regions: unknown, overlaySources: string[]): SourceTree => ({
    id: "test",
    list: (directory) => (directory === "semantic/overlays" ? overlaySources : []),
    read: (path) => (path === "semantic/regions.json" ? JSON.stringify(regions) : undefined),
  });
  const extent = new Map<string, Span[]>([["resource_375", [{ start: 0x02000000, end: 0x02000100 }]]]);
  const claimed = semanticSpans(
    sourceTree({ full_overlays: [{ overlay: "resource_375", evidence: "converted in full" }] },
      ["resource_375_c_02000030.c"]),
    [], [], extent,
  );
  if (spanBytes(claimed.overlays.get("resource_375") ?? []) !== 256) {
    throw new Error("a whole-code-overlay claim did not take the code overlay's executable extent");
  }
  if (claimed.unresolved.length !== 0 || claimed.sources !== 1) {
    throw new Error("a claimed overlay's owners were still reported unresolved");
  }
  const unbacked = semanticSpans(
    sourceTree({ full_overlays: [{ overlay: "resource_375", evidence: "no sources" }] }, []),
    [], [], extent,
  );
  if ((unbacked.overlays.get("resource_375") ?? []).length !== 0) {
    throw new Error("a whole-code-overlay claim with no semantic source credited bytes");
  }
  const noExtent = semanticSpans(
    sourceTree({ full_overlays: [{ overlay: "resource_999", evidence: "not audited" }] },
      ["resource_999_c_02000030.c"]),
    [], [], extent,
  );
  if ((noExtent.overlays.get("resource_999") ?? []).length !== 0) {
    throw new Error("a whole-code-overlay claim without an audited extent credited bytes");
  }
  const unlisted = semanticSpans(sourceTree({}, ["resource_375_c_02000030.c"]), [], [], extent);
  if (unlisted.unresolved.length !== 1) {
    throw new Error("an unlisted overlay owner is no longer reported");
  }
  const perOwner = semanticSpans(
    sourceTree({ manual_regions: [{ overlay: "resource_375", entry: "0x02000030", span_bytes: 64 }] },
      ["resource_375_c_02000030.c"]),
    [], [], extent,
  );
  if (spanBytes(perOwner.overlays.get("resource_375") ?? []) !== 64) {
    throw new Error("per-owner manual_regions sizing regressed");
  }

  // A recorded semantic source must beat the describes-itself heuristic.
  const semanticBearing: SourceTree = {
    id: "exact-with-semantic",
    list: (directory) => (directory === "semantic/main" ? ["08000000.c"] : []),
    read: () => undefined,
  };
  expectReject(
    () => resolveSemanticTree(semanticBearing, undefined, "refs/heads/no-such-source"),
    "an unresolvable recorded semantic source",
  );
  if (resolveSemanticTree(semanticBearing, undefined, undefined).id !== "exact-with-semantic") {
    throw new Error("the describes-itself heuristic stopped applying with no recorded source");
  }
  if (resolveSemanticTree(semanticBearing, undefined, "worktree").id !== "exact-with-semantic") {
    throw new Error("a recorded worktree source did not fall through to the heuristic");
  }
  if (resolveSemanticTree(semanticBearing, "none", "refs/heads/example") !== undefined) {
    throw new Error("an explicit --semantic-ref none was overridden by the record");
  }

  // The README cache-buster: derived from the SVG, idempotent, and replacing any
  // version already present rather than accumulating them.
  const embed = "![a](assets/readme/gs1-en-core.svg) ![b](assets/readme/gs1-en-overlays.svg) ![c](assets/readme/gs1-en-assets.svg)";
  const sameVersion = { core: "abcd1234", overlays: "abcd1234", assets: "abcd1234" } as const;
  const once = readmeWithCacheBuster(embed, "gs1-en", sameVersion);
  if (once !== "![a](assets/readme/gs1-en-core.svg?v=abcd1234) ![b](assets/readme/gs1-en-overlays.svg?v=abcd1234) ![c](assets/readme/gs1-en-assets.svg?v=abcd1234)") {
    throw new Error("the cache-buster was not applied to a bare embed");
  }
  if (readmeWithCacheBuster(once, "gs1-en", sameVersion) !== once) {
    throw new Error("the cache-buster is not idempotent");
  }
  if (!readmeWithCacheBuster(once, "gs1-en", { core: "99887766", overlays: "abcd1234", assets: "abcd1234" })
      .includes("![a](assets/readme/gs1-en-core.svg?v=99887766)")) {
    throw new Error("an existing cache-buster was not replaced");
  }
  if (svgCacheVersion("<svg/>") === svgCacheVersion("<svg />")) {
    throw new Error("the cache version does not track the SVG bytes");
  }
  if (svgCacheVersion("<svg/>") !== svgCacheVersion("<svg/>")) {
    throw new Error("the cache version is not deterministic");
  }

  // The four recovered Djinn are deliberately shown at their native-looking
  // 64 px presentation size. Keep this as a checked constraint: README cache
  // updates must never silently turn them back into oversized hero artwork.
  const readme = readFileSync(readmePath(), "utf8");
  const djinn = [...readme.matchAll(
    /<img src="assets\/readme\/djinn_10[1-4]_idle\.gif" width="(\d+)" height="(\d+)"/g,
  )];
  if (djinn.length !== 4 || djinn.some((match) => match[1] !== "64" || match[2] !== "64")) {
    throw new Error("README Djinn must remain exactly 64 by 64 pixels");
  }

  // A ref tree must list subdirectories, not only files. mainBoundaries walks
  // `asm/` recursively; if a directory reports no children the walk stops at
  // the first level, boundaries go missing and every main-image region is
  // measured too long. Asserted against HEAD rather than the worktree so the
  // check also holds while the work tree carries uncommitted sources.
  const headTree = refTree("HEAD");
  if (headTree === undefined) throw new Error("HEAD did not resolve to a tree");
  if (!headTree.list("asm").some((name) => !name.includes("."))) {
    throw new Error("ref tree listed no asm/ subdirectories; a tree walk would stop at the first level");
  }

  if (resolveExactTree(undefined, undefined).id !== "worktree") {
    throw new Error("the exact source did not default to the worktree");
  }
  if (resolveExactTree(undefined, "worktree").id !== "worktree") {
    throw new Error("a recorded worktree exact source was not honoured");
  }
  if (resolveExactTree("worktree", "refs/heads/example").id !== "worktree") {
    throw new Error("an explicit --exact-ref worktree did not override the recorded ref");
  }
  expectReject(
    () => resolveExactTree(undefined, "origin/no-such-lighthouse"),
    "an unavailable recorded exact ref",
  );
  expectReject(
    () => resolveExactTree("origin/no-such-lighthouse", undefined),
    "an unavailable requested exact ref",
  );

  const withSemantic = {
    categories: { semantic_c: { bytes: 391428 } },
    provenance: { semantic_source: "refs/heads/example" },
  } as CoverageMap;
  if (semanticEraseRefusal(false, undefined, withSemantic) === undefined) {
    throw new Error("unresolved semantic coverage was allowed to erase a tracked value");
  }
  if (semanticEraseRefusal(true, undefined, withSemantic) !== undefined) {
    throw new Error("resolved semantic coverage was refused");
  }
  if (semanticEraseRefusal(false, "none", withSemantic) !== undefined) {
    throw new Error("an explicit --semantic-ref none was refused");
  }
  if (semanticEraseRefusal(false, undefined, undefined) !== undefined) {
    throw new Error("a first write with no tracked map was refused");
  }
  if (semanticEraseRefusal(false, undefined,
      { categories: { semantic_c: { bytes: 0 } }, provenance: { semantic_source: "none" } } as CoverageMap) !== undefined) {
    throw new Error("a tracked map with no semantic coverage was refused");
  }

  const tiles: Tile[] = [
    { label: "a", bytes: 60, categories: { exact_c: 30, assembly: 30 } },
    { label: "b", bytes: 40, categories: { semantic_c: 40 } },
  ];
  const placed = squarify(tiles, (tile) => tile.bytes, { x: 0, y: 0, width: 100, height: 100 });
  if (placed.length !== 2) throw new Error("treemap placement failed");
  const placedArea = placed.reduce((sum, item) => sum + item.rect.width * item.rect.height, 0);
  if (Math.abs(placedArea - 10000) > 1) throw new Error("treemap does not fill its rectangle");
  for (const item of placed) {
    if (item.rect.x < -0.01 || item.rect.y < -0.01 ||
        item.rect.x + item.rect.width > 100.01 || item.rect.y + item.rect.height > 100.01) {
      throw new Error("treemap escaped its rectangle");
    }
  }
  const subpixel = squarify(
    [{ bytes: 999_999 }, { bytes: 1 }],
    (item) => item.bytes,
    { x: 0, y: 0, width: 1600, height: 900 },
  );
  if (subpixel.length !== 2 ||
      Math.min(subpixel[1].rect.width, subpixel[1].rect.height) <= 0 ||
      Math.min(subpixel[1].rect.width, subpixel[1].rect.height) >= 0.5) {
    throw new Error("treemap enlarged or dropped a sub-0.5-pixel byte leaf");
  }

  const bands = mainBands(
    [{ start: 0x08000000, end: 0x08000100 }],
    [{ start: 0x08000000, end: 0x08000040 }],
    [{ start: 0x08000040, end: 0x08000080 }],
    [{ start: 0x080000c0, end: 0x080000e0 }],
    128,
  );
  if (bands.length !== 2 || bands[0].bytes !== 128 || bands[0].categories.exact_c !== 64 ||
      bands[0].categories.semantic_c !== 64 || bands[1].categories.assembly !== 96 ||
      bands[1].categories.retained_asm !== 32) {
    throw new Error("main band composition failed");
  }
  const ownerTiles = mainOwnerTiles(
    [{ start: 0x08000000, end: 0x08000100 }],
    [0x08000000, 0x08000030, 0x080000a0],
    [{ start: 0x08000000, end: 0x08000040 }],
    [{ start: 0x08000040, end: 0x08000080 }],
    [{ start: 0x080000c0, end: 0x080000e0 }],
  );
  if (ownerTiles.length !== 3 || ownerTiles.map((tile) => tile.bytes).join(",") !== "48,112,96" ||
      ownerTiles[1].categories.semantic_c !== 64 || ownerTiles[2].categories.retained_asm !== 32) {
    throw new Error("main owner tiles did not retain audited boundaries");
  }
  const crossingOwner = mainOwnerTiles(
    [{ start: 0x0800fff0, end: 0x08010010 }],
    [0x0800fff0], [], [], [],
  );
  if (crossingOwner.length !== 2 || crossingOwner.map((tile) => tile.bytes).join(",") !== "16,16" ||
      new Set(crossingOwner.map((tile) => tile.group)).size !== 2) {
    throw new Error("a main-image owner was not split at its real 64 KiB bank edge");
  }
  const retainedTree = renderBoxTree(
    area("retained", "Retained", [{ label: "r", bytes: 32, categories: { retained_asm: 32 } }]),
    "retained colour test",
  );
  if (!retainedTree.includes(`fill:${RETAINED_ASM_FILL}`) || retainedTree.includes("#141414")) {
    throw new Error("retained assembly is not rendered orange in the box trees");
  }
  if (!retainedTree.includes("EXACT 100.0%")) {
    throw new Error("retained assembly was omitted from exact completion");
  }

  const map: CoverageMap = {
    format: 1,
    kind: "golden-sun-rom-coverage-map",
    target: "gs1-en",
    derivation: "tracked-evidence-v1",
    rom_bytes: 1024,
    executable_bytes: 200,
    categories: {
      exact_c: { bytes: 60, percent_of_executable: 30 },
      semantic_c: { bytes: 40, percent_of_executable: 20 },
      assembly: { bytes: 80, percent_of_executable: 40 },
      retained_asm: { bytes: 20, percent_of_executable: 10 },
      asset_data: { bytes: 824, percent_of_executable: 0 },
    },
    main: { executable_bytes: 120, exact_c_bytes: 60, semantic_c_bytes: 20 },
    overlays: { executable_bytes: 80, exact_c_bytes: 0, semantic_c_bytes: 20 },
    provenance: {
      exact_source: "worktree",
      semantic_source: "refs/heads/example",
      semantic_sources: 2,
      semantic_superseded_bytes: 0,
      semantic_outside_extent_bytes: 0,
      main_semantic_census: "open",
      semantic_unresolved: [],
    },
    rom_areas: [area("rom-data", "Assets & data", [
      { label: "Other data", bytes: 824, categories: { asset_data: 824 } },
    ])],
    executable_areas: [area("main", "Main image", tiles)],
  };
  const svg = renderSvg(map);
  if (!svg.startsWith("<svg ") || !svg.trimEnd().endsWith("</svg>")) throw new Error("SVG shape failed");
  const boxTree = renderBoxTree(map.executable_areas[0], "self-test box tree");
  if (!boxTree.includes("font-family:Weyard;font-size:16px") ||
      /font-size:(?!16px)/.test(boxTree)) {
    throw new Error("box-tree chrome does not use only 16px Weyard");
  }
  if (!boxTree.includes("Exact 30.0%") || !boxTree.includes("Semantic 40.0%") ||
      !boxTree.includes("Assembly 0.0%") || !boxTree.includes("Unknown 30.0%")) {
    throw new Error("box-tree title or legend is missing from the reproducible SVG");
  }
  if (!boxTree.includes("EXACT 30.0%") ||
      !boxTree.includes('dominant-baseline="middle"')) {
    throw new Error("box-tree completion or vertically centred legend is missing");
  }
  const legendOrder = ["Unknown 30.0%", "Assembly 0.0%", "Semantic 40.0%", "Exact 30.0%"]
    .map((legend) => boxTree.indexOf(`>${legend}</text>`));
  if (legendOrder.some((index) => index < 0) ||
      legendOrder.some((index, position) => position > 0 && index <= legendOrder[position - 1])) {
    throw new Error("box-tree legend no longer runs from unknown through exact");
  }
  if (!boxTree.includes('viewBox="0 0 540 304"') ||
      !boxTree.includes('width="540" height="304"')) {
    throw new Error("box tree lost its fixed pixel-exact dashboard canvas");
  }
  if (!boxTree.includes('width="540" height="304" fill="#6d4fc2" rx="8"')) {
    throw new Error("box tree background no longer follows its rounded outer border");
  }
  const folderTree = renderBoxTree(area("folders", "Folders", [
    { label: "one", bytes: 60, categories: { exact_c: 60 }, group: "first" },
    { label: "two", bytes: 40, categories: { assembly: 40 }, group: "second" },
  ]), "folder self-test", CORE_HUE, CODE_FRACTION,
    ["exact_c", "assembly"], ["group"]);
  if ((folderTree.match(/data-folder-depth="1"/g) ?? []).length !== 2 ||
      (folderTree.match(/data-byte-leaf="true"/g) ?? []).length !== 2) {
    throw new Error("recursive box-tree hierarchy lost a group or leaf");
  }
  if (svg.includes("undefined") || svg.includes("NaN")) throw new Error("SVG contains unresolved values");
  for (const category of CATEGORY_ORDER) {
    if (!svg.toUpperCase().includes(escapeText(CATEGORY_STYLE[category].label).toUpperCase())) {
      throw new Error(`SVG does not name the ${category} category`);
    }
    if (!svg.includes(CATEGORY_STYLE[category].fill)) throw new Error(`SVG does not draw the ${category} category`);
  }
  const openTags = (svg.match(/<rect /g) ?? []).length;
  if (openTags < 4) throw new Error("SVG drew no tiles");

  const maturityTree: SourceTree = {
    id: "maturity-test",
    list: () => [],
    read: (path) => path === "assets/manifest.json"
      ? JSON.stringify({ kind: "sprite", size: 100, source: "assets/example.bin" })
      : undefined,
  };
  const maturityMap = {
    ...map,
    rom_areas: [area("rom-data", "Assets & data", [
      { label: "Other data", bytes: 200, categories: { asset_data: 200 } },
    ])],
    executable_areas: [
      ...map.executable_areas,
      area("overlays", "Decoded code overlays", [
        { label: "test", bytes: 80, categories: { assembly: 80 } },
      ]),
    ],
  };
  const maturitySvg = renderBoxTrees(maturityMap, maturityTree).assets;
  if (!maturitySvg.includes("100 catalogued bytes") || !maturitySvg.includes("100 unclassified ROM-image data bytes")) {
    throw new Error("asset maturity tree did not include its unclassified remainder");
  }

  expectReject(() => optionsOf(["--unknown"]), "unknown argument");
  if (optionsOf(["--semantic-ref", "none"]).semantic !== "none") throw new Error("option parsing failed");

  console.log("self-test=ok tool=coverage_map");
}

async function main(argv: string[]): Promise<void> {
  const options = optionsOf(argv);
  if (options.selfTest) return selfTest();
  // Reuse recorded source refs so a redraw is stable unless explicitly
  // overridden.
  const trackedDocumentOnDisk = existsSync(mapPath(options.target))
    ? (JSON.parse(readFileSync(mapPath(options.target), "utf8")) as CoverageMap)
    : undefined;
  const exact = resolveExactTree(options.exact, trackedDocumentOnDisk?.provenance.exact_source);
  const semantic = resolveSemanticTree(
    exact,
    options.semantic,
    trackedDocumentOnDisk?.provenance.semantic_source,
  );
  const map = buildCoverageMap({
    target: options.target,
    exact,
    semantic,
    preferVerifiedAssets: true,
  });
  // `bun run coverage` is intentionally run after the verified build. Consume
  // the same schema-expanded asset evidence as the live dashboard so the
  // checked-in SVG panels are the dashboard artifacts, not approximations.
  const trees = renderBoxTrees(map, exact, true);
  const json = canonicalJson(trackedDocument(map));

  if (options.check) {
    const trackedMap = readFileSync(mapPath(options.target), "utf8");
    const trackedTrees = Object.fromEntries(BOX_TREES.map((tree) =>
      [tree, readFileSync(boxTreePath(options.target, tree), "utf8")],
    )) as Record<BoxTreeId, string>;
    const stale: string[] = [];
    // Exact coverage is always checked. Semantic coverage is checked when its
    // selected source is the current work tree.
    const trackedJson = JSON.parse(trackedMap) as CoverageMap;
    if (trackedJson.categories.exact_c.bytes !== map.categories.exact_c.bytes ||
        trackedJson.main.exact_c_bytes !== map.main.exact_c_bytes ||
        trackedJson.overlays.exact_c_bytes !== map.overlays.exact_c_bytes) {
      stale.push("exact coverage");
    }
    if (semantic && semantic.id === "worktree" &&
        trackedJson.categories.semantic_c.bytes !== map.categories.semantic_c.bytes) {
      stale.push("semantic coverage");
    }
    if (trackedJson.executable_bytes !== map.executable_bytes) stale.push("executable denominator");
    if (semantic?.id === trackedJson.provenance.semantic_source &&
        (trackedMap !== json ||
         BOX_TREES.some((tree) => trackedTrees[tree] !== trees[tree])) && !stale.length) {
      stale.push("rendered map");
    }
    const expected = readmeWithCacheBuster(
      readFileSync(readmePath(), "utf8"),
      options.target,
      Object.fromEntries(BOX_TREES.map((tree) =>
        [tree, svgCacheVersion(trackedTrees[tree])],
      )) as Record<BoxTreeId, string>,
    );
    if (expected !== readFileSync(readmePath(), "utf8")) stale.push("README cache-buster");
    if (stale.length) {
      throw new Error(
        `tracked coverage map is stale (${stale.join(", ")}); run: bun run coverage`,
      );
    }
    console.log(`coverage-map=current ${summarize(map)}`);
    return;
  }

  if (options.write) {
    const refusal = semanticEraseRefusal(semantic !== undefined, options.semantic, trackedDocumentOnDisk);
    if (refusal) throw new Error(refusal);
    writeFileSync(mapPath(options.target), json);
    for (const tree of BOX_TREES) writeFileSync(boxTreePath(options.target, tree), trees[tree]);
    // Keep the README's cache-busters in step with the pictures they bust. Doing
    // it here rather than by hand means they cannot drift.
    const readme = readFileSync(readmePath(), "utf8");
    const busted = readmeWithCacheBuster(readme, options.target,
      Object.fromEntries(BOX_TREES.map((tree) =>
        [tree, svgCacheVersion(trees[tree])],
      )) as Record<BoxTreeId, string>);
    if (busted !== readme) writeFileSync(readmePath(), busted);
    console.log(
      `map=${mapPath(options.target).slice(ROOT.length + 1)} ` +
      `trees=${BOX_TREES.map((tree) => boxTreePath(options.target, tree).slice(ROOT.length + 1)).join(",")} ` +
      `${summarize(map)}`,
    );
    return;
  }
  console.log(summarize(map));
}

if (import.meta.main) await main(Bun.argv.slice(2));
