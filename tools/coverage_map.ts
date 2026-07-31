#!/usr/bin/env bun
// ROM coverage map: a treemap of the audited GS1-EN image, drawn as a dark
// product-dashboard card for the README.
//
// The map answers one question visually: of every byte in the English ROM,
// which bytes are already expressed as byte-exact C (Mercury Lighthouse),
// which are expressed as reviewed semantic C (Venus Lighthouse), and which are
// still assembly or non-code asset data.
//
// Every number here is derived from tracked evidence only. There is no ROM
// read, no toolchain, and no build output in the derivation, so either
// lighthouse can regenerate the picture from a fresh clone:
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

export type Lane = "exact_c" | "semantic_c" | "assembly" | "asset_data";

// Lane order is also the stacking order inside a tile: exact at the bottom.
// `ink` is the label colour a tile takes when that lane fills most of it.
const LANE_STYLE: Record<Lane, { fill: string; ink: string; label: string }> = {
  exact_c: { fill: "#0072f5", ink: "#eaf2ff", label: "byte-exact C" },
  semantic_c: { fill: "#50e3c2", ink: "#04241d", label: "semantic C" },
  assembly: { fill: "#333333", ink: "#a1a1a1", label: "assembly" },
  asset_data: { fill: "#ff0080", ink: "#2b0016", label: "assets & data" },
};
const LANE_ORDER: Lane[] = ["exact_c", "semantic_c", "assembly", "asset_data"];

export interface Span {
  start: number;
  end: number;
}

/** One drawable leaf: a byte run of the image with its lane composition. */
export interface Tile {
  label: string;
  bytes: number;
  lanes: Partial<Record<Lane, number>>;
}

export interface Area {
  id: string;
  label: string;
  bytes: number;
  lanes: Partial<Record<Lane, number>>;
  tiles: Tile[];
}

export interface CoverageMap {
  format: 1;
  kind: "golden-sun-rom-coverage-map";
  target: DecompTargetId;
  derivation: "tracked-evidence-v1";
  rom_bytes: number;
  executable_bytes: number;
  lanes: Record<Lane, { bytes: number; percent_of_executable: number }>;
  main: { executable_bytes: number; exact_c_bytes: number; semantic_c_bytes: number };
  overlays: { executable_bytes: number; exact_c_bytes: number; semantic_c_bytes: number };
  provenance: {
    exact_lane: string;
    semantic_lane: string;
    semantic_sources: number;
    semantic_superseded_bytes: number;
    semantic_unresolved: string[];
  };
  rom_areas: Area[];
  executable_areas: Area[];
}

/**
 * The tracked document is a summary: lane totals and one row per area. Tile
 * geometry is a drawing detail that the SVG already carries, and keeping it
 * out of the tree keeps a per-cycle regeneration to a few changed numbers.
 */
export function trackedDocument(map: CoverageMap): unknown {
  const rows = (areas: readonly Area[]) =>
    areas.map((item) => ({
      id: item.id,
      label: item.label,
      bytes: item.bytes,
      lanes: item.lanes,
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
export function overlayPlaceholderSpans(source: string): Span[] {
  const spans: Span[] = [];
  let address: number | undefined;
  let inPlaceholder = false;
  for (const line of source.split(/\r?\n/)) {
    const label = /^\s*AlchemyC_([0-9a-f]{8}):\s*$/i.exec(line);
    if (label) {
      address = Number.parseInt(label[1], 16);
      inPlaceholder = true;
      continue;
    }
    if (inPlaceholder && (/^\s*$/.test(line) || /^\s*\.L_[0-9a-z_.$]+:\s*$/i.test(line))) continue;
    const reservation = /^\s*\.space\s+(0x[0-9a-f]+|\d+)\s*$/i.exec(line);
    if (reservation && inPlaceholder && address !== undefined) {
      const size = Number.parseInt(reservation[1], 0);
      spans.push({ start: address, end: address + size });
      address += size;
      continue;
    }
    if (line.trim()) inPlaceholder = false;
  }
  return normalize(spans);
}

export function exactOverlaySpans(tree: SourceTree): Map<string, Span[]> {
  const owned = new Map<string, Span[]>();
  for (const name of tree.list("assets/code")) {
    const match = OVERLAY_ASSEMBLY.exec(name);
    if (!match) continue;
    const source = tree.read(`assets/code/${name}`);
    if (source === undefined) continue;
    const spans = overlayPlaceholderSpans(source);
    if (spans.length) owned.set(match[1], spans);
  }
  return owned;
}

export interface SemanticLane {
  main: Map<number, Span[]>;
  overlays: Map<string, Span[]>;
  sources: number;
  unresolved: string[];
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
): SemanticLane {
  const limit = executable.at(-1)?.end ?? ROM_BASE;
  const main = new Map<number, Span[]>();
  const overlays = new Map<string, Span[]>();
  const unresolved: string[] = [];
  let sources = 0;

  const ownerDocument = tree.read("semantic/main-regions.json");
  const owners = new Map<number, Span[]>();
  if (ownerDocument !== undefined) {
    for (const owner of JSON.parse(ownerDocument).main_owners ?? []) {
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
  // lane. That is one evidence-bearing entry instead of one per owner, which is
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
    overlays.set(overlay, [...(overlays.get(overlay) ?? []), { start: address, end: address + span }]);
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
  return { main, overlays, sources, unresolved };
}

// --------------------------------------------------------------- ROM layout

interface RomRange {
  start: number;
  end: number;
  kind: string;
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
  const push = (start: number | undefined, size: number | undefined, kind: string): void => {
    if (start === undefined || size === undefined || size <= 0) return;
    if (start < ROM_BASE || start + size > ROM_BASE + romSize) return;
    ranges.push({ start, end: start + size, kind });
  };
  const visit = (node: unknown, kind: string): void => {
    if (Array.isArray(node)) {
      const [, address, size] = node as unknown[];
      if (node.length >= 3 && hexValue(address) !== undefined && hexValue(size) !== undefined) {
        push(hexValue(address), hexValue(size), kind);
        for (const child of node.slice(3)) visit(child, kind);
        return;
      }
      for (const child of node) visit(child, kind);
      return;
    }
    if (node === null || typeof node !== "object") return;
    const record = node as Record<string, unknown>;
    const local = typeof record.kind === "string" ? record.kind : kind;
    push(hexValue(record.address), hexValue(record.size), local);
    for (const [key, value] of Object.entries(record)) {
      // Components describe decoded payloads, not ROM ranges.
      if (key !== "components") visit(value, local);
    }
    for (const reference of [record.index, record.source, record.plan]) {
      if (typeof reference !== "string" || !reference.endsWith(".json")) continue;
      if (visited.has(reference)) continue;
      visited.add(reference);
      const text = tree.read(reference);
      if (text === undefined) continue;
      try {
        visit(JSON.parse(text), local);
      } catch {
        // A package index that cannot be parsed simply contributes no label.
      }
    }
  };
  visit(readJson(tree, "assets/manifest.json"), "asset");
  return ranges;
}

/** Compressed ROM footprint and decoded size of every Thumb code overlay. */
export function overlayStreams(tree: SourceTree): Map<string, { start: number; romBytes: number; decodedBytes: number }> {
  const streams = new Map<string, { start: number; romBytes: number; decodedBytes: number }>();
  for (const series of readJson(tree, "assets/manifest.json").series ?? []) {
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

// ------------------------------------------------------------- map assembly

function laneTotal(tiles: readonly Tile[], lane: Lane): number {
  return tiles.reduce((sum, tile) => sum + (tile.lanes[lane] ?? 0), 0);
}

function area(id: string, label: string, tiles: Tile[]): Area {
  const lanes: Partial<Record<Lane, number>> = {};
  for (const lane of ["exact_c", "semantic_c", "assembly", "asset_data"] as Lane[]) {
    const bytes = laneTotal(tiles, lane);
    if (bytes) lanes[lane] = bytes;
  }
  return {
    id,
    label,
    bytes: tiles.reduce((sum, tile) => sum + tile.bytes, 0),
    lanes,
    tiles: tiles.filter((tile) => tile.bytes > 0),
  };
}

function hex8(address: number): string {
  return address.toString(16).padStart(8, "0");
}

/**
 * Group audited main-image regions into contiguous address bands of roughly
 * `target` executable bytes so the drawing stays legible. Bands never split a
 * region, so every tile is a real run of the image.
 */
function mainBands(
  executable: readonly Span[],
  exact: readonly Span[],
  semantic: readonly Span[],
  target: number,
): Tile[] {
  const tiles: Tile[] = [];
  let current: { start: number; spans: Span[] } | undefined;
  const flush = (): void => {
    if (!current || !current.spans.length) return;
    const bytes = spanBytes(current.spans);
    const exactBytes = spanBytes(intersect(current.spans, exact));
    const semanticBytes = spanBytes(intersect(current.spans, semantic));
    tiles.push({
      label: hex8(current.start).slice(0, 6),
      bytes,
      lanes: {
        exact_c: exactBytes,
        semantic_c: semanticBytes,
        assembly: bytes - exactBytes - semanticBytes,
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
 * Merge neighbouring tiles until each carries at least `target` bytes. The
 * ROM view holds the whole cartridge, so its code areas are drawn at a coarser
 * grain than the executable view where the same regions get the full mosaic.
 */
export function groupTiles(tiles: readonly Tile[], target: number): Tile[] {
  const grouped: Tile[] = [];
  let current: Tile | undefined;
  let first = "";
  let last = "";
  for (const tile of tiles) {
    if (!current) {
      current = { label: tile.label, bytes: 0, lanes: {} };
      first = tile.label;
    }
    current.bytes += tile.bytes;
    last = tile.label;
    for (const lane of LANE_ORDER) {
      const bytes = tile.lanes[lane] ?? 0;
      if (bytes) current.lanes[lane] = (current.lanes[lane] ?? 0) + bytes;
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
}

export function buildCoverageMap(options: BuildOptions): CoverageMap {
  const registry = decompTarget(options.target);
  const inventory = readJson(
    options.exact,
    `metrics/${options.target}-executable.json`,
  ) as ExecutableInventory;
  const tracked = readJson(
    options.exact,
    `metrics/${options.target}-progress.json`,
  ) as ProgressReport;
  if (inventory.audit !== "complete") {
    throw new Error(`${options.target} executable audit is incomplete; coverage map withheld`);
  }

  const mainExecutable = unionIntervals(inventory.main.intervals);
  const overlayExecutable = new Map<string, Span[]>(
    inventory.overlays.map((overlay) => [overlay.id, unionIntervals(overlay.intervals)]),
  );

  const exactMain = [...exactMainSpans(options.exact, mainExecutable).values()].flat();
  const exactOverlayByResource = exactOverlaySpans(options.exact);
  for (const [overlay, spans] of exactOverlayByResource) {
    exactOverlayByResource.set(overlay, intersect(spans, overlayExecutable.get(overlay) ?? []));
  }

  const boundaries = mainBoundaries(options.exact);
  const semanticLane = options.semantic
    ? semanticSpans(options.semantic, boundaries, mainExecutable, overlayExecutable)
    : { main: new Map(), overlays: new Map(), sources: 0, unresolved: [] as string[] };

  // Exact always wins over semantic: the semantic lane only shows the ground
  // the exact lane has not already taken.
  const exactMainUnion = normalize(exactMain);
  const semanticMain = subtract([...semanticLane.main.values()].flat(), exactMainUnion);
  const semanticOverlayByResource = new Map<string, Span[]>();
  let semanticSuperseded = spanBytes([...semanticLane.main.values()].flat()) - spanBytes(semanticMain);
  for (const [overlay, spans] of semanticLane.overlays) {
    const executable = overlayExecutable.get(overlay) ?? [];
    const owned = subtract(intersect(spans, executable), exactOverlayByResource.get(overlay) ?? []);
    semanticSuperseded += spanBytes(spans) - spanBytes(owned);
    if (owned.length) semanticOverlayByResource.set(overlay, owned);
  }

  const exactMainBytes = spanBytes(exactMainUnion);
  const exactOverlayBytes = [...exactOverlayByResource.values()].reduce(
    (sum, spans) => sum + spanBytes(spans),
    0,
  );
  if (exactMainBytes !== tracked.main.full_c_bytes || exactOverlayBytes !== tracked.overlays.full_c_bytes) {
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
  const executableAreas: Area[] = [
    area("main", "Main image", mainBands(mainExecutable, exactMainUnion, semanticMain, 10240)),
  ];
  const overlayTiles: Tile[] = [];
  for (const overlay of inventory.overlays) {
    const executable = overlayExecutable.get(overlay.id) ?? [];
    const exactBytesHere = spanBytes(exactOverlayByResource.get(overlay.id) ?? []);
    const semanticBytesHere = spanBytes(semanticOverlayByResource.get(overlay.id) ?? []);
    overlayTiles.push({
      label: overlay.id.replace(/^resource_/, ""),
      bytes: spanBytes(executable),
      lanes: {
        exact_c: exactBytesHere,
        semantic_c: semanticBytesHere,
        assembly: spanBytes(executable) - exactBytesHere - semanticBytesHere,
      },
    });
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
      mainBands(mainExecutable, exactMainUnion, semanticMain, 65536),
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
    // of its decoded executable bytes each lane owns.
    const exactPart = Math.round(stream.romBytes * exactShare);
    const semanticPart = Math.round(stream.romBytes * semanticShare);
    streamTiles.push({
      label: overlay.replace(/^resource_/, ""),
      bytes: stream.romBytes,
      lanes: {
        exact_c: exactPart,
        semantic_c: semanticPart,
        assembly: stream.romBytes - exactPart - semanticPart,
      },
    });
  }
  romAreas.push(area("rom-overlay-streams", "Compressed code overlays", groupTiles(streamTiles, 49152)));

  const labels = manifestRanges(options.exact, romSize);
  const buckets = new Map<string, { label: string; bytes: number }>();
  let claimed: Span[] = [];
  for (const range of labels.sort((left, right) => left.start - right.start || left.end - right.end)) {
    const fresh = subtract(intersect([range], dataSpans), claimed);
    const bytes = spanBytes(fresh);
    if (!bytes) continue;
    claimed = normalize([...claimed, ...fresh]);
    const bucket = assetBucket(range.kind);
    const entry = buckets.get(bucket.id) ?? { label: bucket.label, bytes: 0 };
    entry.bytes += bytes;
    buckets.set(bucket.id, entry);
  }
  const unlabelled = spanBytes(dataSpans) - spanBytes(claimed);
  if (unlabelled > 0) {
    const entry = buckets.get("other") ?? { label: "Other data", bytes: 0 };
    entry.bytes += unlabelled;
    buckets.set("other", entry);
  }
  romAreas.push(area(
    "rom-data",
    "Assets & data",
    [...buckets].sort((left, right) => right[1].bytes - left[1].bytes)
      .map(([, entry]) => ({ label: entry.label, bytes: entry.bytes, lanes: { asset_data: entry.bytes } })),
  ));

  const romBytesCheck = romAreas.reduce((sum, item) => sum + item.bytes, 0);
  if (romBytesCheck !== romSize) {
    throw new Error(`ROM areas cover ${romBytesCheck} of ${romSize} bytes`);
  }
  const executableCheck = executableAreas.reduce((sum, item) => sum + item.bytes, 0);
  if (executableCheck !== executableBytes) {
    throw new Error(`executable areas cover ${executableCheck} of ${executableBytes} bytes`);
  }

  return {
    format: 1,
    kind: "golden-sun-rom-coverage-map",
    target: options.target,
    derivation: "tracked-evidence-v1",
    rom_bytes: romSize,
    executable_bytes: executableBytes,
    lanes: {
      exact_c: { bytes: exactBytes, percent_of_executable: roundHalfUpPercent(exactBytes, executableBytes) },
      semantic_c: {
        bytes: semanticBytes,
        percent_of_executable: roundHalfUpPercent(semanticBytes, executableBytes),
      },
      assembly: {
        bytes: executableBytes - exactBytes - semanticBytes,
        percent_of_executable: roundHalfUpPercent(
          executableBytes - exactBytes - semanticBytes,
          executableBytes,
        ),
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
      exact_lane: options.exact.id,
      semantic_lane: options.semantic?.id ?? "absent",
      semantic_sources: semanticLane.sources,
      semantic_superseded_bytes: semanticSuperseded,
      semantic_unresolved: semanticLane.unresolved.sort(),
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
 * Squarified treemap placement (Bruls, Huizing, van Wijk). Items are laid out
 * in the order given; the caller sorts them so the result is deterministic.
 */
export function squarify<T>(items: readonly T[], value: (item: T) => number, rect: Rect): Array<Placed<T>> {
  const placed: Array<Placed<T>> = [];
  const queue = items.filter((item) => value(item) > 0);
  const totalValue = queue.reduce((sum, item) => sum + value(item), 0);
  if (!queue.length || totalValue <= 0 || rect.width <= 0 || rect.height <= 0) return placed;
  let free = { ...rect };
  let scale = (rect.width * rect.height) / totalValue;
  let index = 0;
  while (index < queue.length) {
    const short = Math.min(free.width, free.height);
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
    const thickness = rowValue / short;
    let offset = 0;
    for (const item of row) {
      const side = (value(item) * scale) / thickness;
      placed.push({
        item,
        rect: free.width >= free.height
          ? { x: free.x, y: free.y + offset, width: thickness, height: side }
          : { x: free.x + offset, y: free.y, width: side, height: thickness },
      });
      offset += side;
    }
    // The row consumed exactly its own area, so the value-to-area scale still
    // holds for the rectangle that is left.
    free = free.width >= free.height
      ? { x: free.x + thickness, y: free.y, width: free.width - thickness, height: free.height }
      : { x: free.x, y: free.y + thickness, width: free.width, height: free.height - thickness };
    if (free.width <= 0.01 || free.height <= 0.01) break;
  }
  return placed;
}

// ---------------------------------------------------------------- rendering
//
// The drawing is a dark product-dashboard card: a black surface, hairline
// borders, one accent per lane, and monospaced addresses. It reads the same on
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
  return value.replace(/[&<>]/g, (character) =>
    character === "&" ? "&amp;" : character === "<" ? "&lt;" : "&gt;");
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
  let midpointLane: Lane = "assembly";
  for (const lane of LANE_ORDER) {
    const share = tile.lanes[lane] ?? 0;
    if (share <= 0) continue;
    // Lanes stack from the bottom of the tile, so a converting region reads as
    // a filling gauge rather than as a different region.
    const height = body.height * (share / tile.bytes);
    lines.push(rect(
      { x: body.x, y: body.y + body.height - offset - height, width: body.width, height },
      LANE_STYLE[lane].fill,
    ));
    // The label sits on the tile's midline, so it takes its contrast from the
    // lane that actually lies under it rather than from the largest one.
    if (offset <= body.height / 2 && body.height / 2 < offset + height) midpointLane = lane;
    offset += height;
  }
  const text = body.height >= 12 ? fitText(tile.label, body.width - 6, 9) : undefined;
  if (text === tile.label) {
    lines.push(label(body.x + body.width / 2, body.y + body.height / 2 + 3.2, text, {
      size: 9,
      anchor: "middle",
      // Addresses and resource ids are monospaced; prose bucket names are not.
      mono: /^[0-9a-f]+(–[0-9a-f]+)?$/i.test(tile.label),
      fill: LANE_STYLE[midpointLane].ink,
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

/** A stacked, rounded lane bar across the whole executable denominator. */
function laneBar(map: CoverageMap, frame: Rect, lines: string[]): void {
  const clip = "lane-bar-clip";
  lines.push(
    `<defs><clipPath id="${clip}"><rect x="${frame.x}" y="${frame.y}" width="${frame.width}" ` +
    `height="${frame.height}" rx="${frame.height / 2}"/></clipPath></defs>`,
    `<g clip-path="url(#${clip})">`,
    rect(frame, HAIRLINE),
  );
  let cursor = frame.x;
  for (const lane of ["exact_c", "semantic_c", "assembly"] as Lane[]) {
    const laneWidth = frame.width * (map.lanes[lane].bytes / map.executable_bytes);
    lines.push(rect({ ...frame, x: cursor, width: laneWidth }, LANE_STYLE[lane].fill));
    cursor += laneWidth;
  }
  lines.push("</g>");
}

export function renderSvg(map: CoverageMap): string {
  const width = 1200;
  // Tall enough that the ROM card stays portrait: its two code areas are an
  // eighth of the cartridge, and only a portrait card gives them a readable
  // band instead of a sliver.
  const height = 660;
  const margin = 32;
  const lines: string[] = [];
  const exact = map.lanes.exact_c;
  const semantic = map.lanes.semantic_c;
  const combined = exact.bytes + semantic.bytes;

  lines.push(
    // Intrinsic width and height as well as the viewBox: an <img> that is only
    // given a viewBox falls back to a 2:1 default ratio in some engines and
    // letterboxes the drawing.
    `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 ${width} ${height}" ` +
    `width="${width}" height="${height}" font-family="${SANS}" role="img" ` +
    `aria-label="Coverage treemap of the English Golden Sun ROM: ` +
    `${roundHalfUpPercent(combined, map.executable_bytes)}% of the ` +
    `${commas(map.executable_bytes)} audited executable bytes are expressed as C">`,
    rect({ x: 0, y: 0, width, height }, SURFACE),
    label(margin, 46, "Alchemy — Golden Sun", { size: 15, weight: 600, tracking: -0.2 }),
    label(margin, 64, `${map.target} · ${megabytes(map.rom_bytes)} cartridge`, {
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
    // metric, which is the byte-exact lane alone.
    label(width - margin, 64, "EXACT + SEMANTIC C", {
      size: 9.5,
      fill: MUTED,
      anchor: "end",
      tracking: 0.9,
    }),
  );

  // The metric row doubles as the legend: one dot, one lane, one number.
  const metrics: Array<{ lane: Lane; value: string; note?: string }> = [
    { lane: "exact_c", value: `${exact.percent_of_executable}%`, note: "FULL-C BYTE SHARE" },
    { lane: "semantic_c", value: `${semantic.percent_of_executable}%` },
    { lane: "assembly", value: `${map.lanes.assembly.percent_of_executable}%` },
    { lane: "asset_data", value: megabytes(map.lanes.asset_data.bytes), note: "NOT CODE" },
  ];
  const columnWidth = (width - margin * 2) / metrics.length;
  metrics.forEach((metric, index) => {
    const x = margin + index * columnWidth;
    const name = LANE_STYLE[metric.lane].label.toUpperCase() +
      (metric.note ? ` · ${metric.note}` : "");
    lines.push(
      rect({ x, y: 96, width: 8, height: 8 }, LANE_STYLE[metric.lane].fill, { radius: 2 }),
      label(x + 14, 104, name, { size: 9.5, fill: MUTED, tracking: 0.8 }),
      label(x, 128, metric.value, { size: 21, weight: 600, tracking: -0.4 }),
    );
  });

  laneBar(map, { x: margin, y: 146, width: width - margin * 2, height: 6 }, lines);

  const cardTop = 176;
  const cardHeight = height - cardTop - 44;
  card(
    "ROM image",
    `${megabytes(map.rom_bytes)} on the cartridge`,
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
    label(margin, height - 20, "Tiles are main-image address bands and one per overlay; compressed " +
      "overlay tiles are sized by ROM bytes and shaded by the decoded share each lane owns.", {
      size: 9.5,
      fill: MUTED,
    }),
    label(width - margin, height - 20,
      `exact C: ${map.provenance.exact_lane} · semantic C: ${map.provenance.semantic_lane}`, {
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
export function svgCacheVersion(svg: string): string {
  return createHash("sha1").update(svg).digest("hex").slice(0, 8);
}

/** Rewrite the README's embed to carry `version`, replacing any it already has. */
export function readmeWithCacheBuster(
  readme: string,
  target: DecompTargetId,
  version: string,
): string {
  const file = `assets/readme/${target}-coverage.svg`;
  const escaped = file.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
  return readme.replace(
    new RegExp(`\\(${escaped}(?:\\?v=[0-9a-f]*)?\\)`, "g"),
    `(${file}?v=${version})`,
  );
}

function readmePath(): string {
  return join(ROOT, "README.md");
}

function svgPath(target: DecompTargetId): string {
  return join(ROOT, "assets", "readme", `${target}-coverage.svg`);
}

/**
 * The semantic lane lives on the Venus branch. A tree that carries semantic
 * sources describes itself; otherwise the tracked Venus ref is used when it is
 * available locally, and the lane is reported as absent when it is not.
 */
export function resolveSemanticTree(
  exact: SourceTree,
  requested?: string,
  recorded?: string,
): SourceTree | undefined {
  if (requested === "none") return undefined;
  // The recorded lane wins over the describes-itself heuristic below. Once the
  // ring carries semantic C to every branch, "this tree has semantic sources"
  // stops identifying the lane owner: the exact tree has them too, and the
  // heuristic would silently draw Venus's lane from whichever tree it was handed
  // — reporting a smaller, older semantic figure as though it were current.
  const wanted = requested ??
    (recorded && !["worktree", "absent", "none"].includes(recorded) ? recorded : undefined);
  if (wanted) {
    const tree = wanted === "worktree" ? workTree() : refTree(wanted);
    if (!tree) throw new Error(`cannot resolve semantic lane ref ${wanted}`);
    return tree;
  }
  if (exact.list("semantic/main").some((name) => MAIN_SOURCE.test(name))) return exact;
  for (const ref of ["venus", "origin/venus"]) {
    const tree = refTree(ref);
    if (tree && tree.list("semantic/main").some((name) => MAIN_SOURCE.test(name))) return tree;
  }
  return undefined;
}

/**
 * The exact lane belongs to Mercury. A tree that advances the lane itself
 * describes itself, so Mercury and Venus keep drawing from their own worktree
 * and are unaffected by this. `main` is the exception: Mercury pulls from main
 * and never pushes back, so main's own `src/` never receives those conversions
 * and its picture would freeze at whatever exact C main happens to carry while
 * the project moved on. Main therefore records `origin/mercury` as its exact
 * lane in the map's provenance and keeps drawing from there — the same way the
 * semantic lane is read from venus. An explicit `--exact-ref` wins; pass
 * `worktree` to force this tree's own exact C.
 *
 * An unavailable recorded ref is an error, never a quiet fall back to the
 * worktree: falling back would silently republish a smaller exact lane.
 */
export function resolveExactTree(requested?: string, recorded?: string): SourceTree {
  const wanted = requested ?? (recorded && recorded !== "worktree" ? recorded : undefined);
  if (wanted === undefined || wanted === "worktree") return workTree();
  const tree = refTree(wanted);
  if (!tree) {
    throw new Error(
      `exact lane ref ${wanted} is not available here; run: ` +
      `git fetch origin ${wanted.replace(/^origin\//, "")} ` +
      `(or --exact-ref worktree to draw this tree's own exact C)`,
    );
  }
  return tree;
}

/**
 * A tree that cannot see the semantic lane must not publish it as zero. The
 * lane lives on venus, but Mercury redraws this map from its bank cycle and
 * has no reason to hold a venus ref; without this guard the first such bank
 * silently erases Venus's half of the published picture. Returns the refusal
 * message, or undefined when writing is safe: the lane resolved, it was
 * dropped on purpose with `--semantic-ref none`, or the tracked map has no
 * semantic lane to lose.
 */
export function semanticEraseRefusal(
  resolved: boolean,
  requested: string | undefined,
  tracked: CoverageMap | undefined,
): string | undefined {
  if (resolved || requested === "none") return undefined;
  const bytes = tracked?.lanes.semantic_c.bytes ?? 0;
  if (bytes <= 0) return undefined;
  return `refusing to erase the semantic lane: the tracked map records ${bytes} ` +
    `semantic bytes from ${tracked?.provenance.semantic_lane}, which is not available ` +
    `here; run: git fetch origin venus (or --semantic-ref none to publish without it)`;
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
  const combined = map.lanes.exact_c.bytes + map.lanes.semantic_c.bytes;
  return [
    `target=${map.target}`,
    `rom=${commas(map.rom_bytes)}`,
    `executable=${commas(map.executable_bytes)}`,
    `exact=${commas(map.lanes.exact_c.bytes)} (${map.lanes.exact_c.percent_of_executable}%)`,
    `semantic=${commas(map.lanes.semantic_c.bytes)} (${map.lanes.semantic_c.percent_of_executable}%)`,
    `combined=${commas(combined)} (${roundHalfUpPercent(combined, map.executable_bytes)}%)`,
    `semantic_lane=${map.provenance.semantic_lane}`,
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
  const laneTree = (regions: unknown, overlaySources: string[]): SourceTree => ({
    id: "test",
    list: (directory) => (directory === "semantic/overlays" ? overlaySources : []),
    read: (path) => (path === "semantic/regions.json" ? JSON.stringify(regions) : undefined),
  });
  const extent = new Map<string, Span[]>([["resource_375", [{ start: 0x02000000, end: 0x02000100 }]]]);
  const claimed = semanticSpans(
    laneTree({ full_overlays: [{ overlay: "resource_375", evidence: "converted in full" }] },
      ["resource_375_c_02000030.c"]),
    [], [], extent,
  );
  if (spanBytes(claimed.overlays.get("resource_375") ?? []) !== 256) {
    throw new Error("a whole-overlay claim did not take the overlay's executable extent");
  }
  if (claimed.unresolved.length !== 0 || claimed.sources !== 1) {
    throw new Error("a claimed overlay's owners were still reported unresolved");
  }
  const unbacked = semanticSpans(
    laneTree({ full_overlays: [{ overlay: "resource_375", evidence: "no sources" }] }, []),
    [], [], extent,
  );
  if ((unbacked.overlays.get("resource_375") ?? []).length !== 0) {
    throw new Error("a whole-overlay claim with no semantic source credited bytes");
  }
  const noExtent = semanticSpans(
    laneTree({ full_overlays: [{ overlay: "resource_999", evidence: "not audited" }] },
      ["resource_999_c_02000030.c"]),
    [], [], extent,
  );
  if ((noExtent.overlays.get("resource_999") ?? []).length !== 0) {
    throw new Error("a whole-overlay claim without an audited extent credited bytes");
  }
  const unlisted = semanticSpans(laneTree({}, ["resource_375_c_02000030.c"]), [], [], extent);
  if (unlisted.unresolved.length !== 1) {
    throw new Error("an unlisted overlay owner is no longer reported");
  }
  const perOwner = semanticSpans(
    laneTree({ manual_regions: [{ overlay: "resource_375", entry: "0x02000030", span_bytes: 64 }] },
      ["resource_375_c_02000030.c"]),
    [], [], extent,
  );
  if (spanBytes(perOwner.overlays.get("resource_375") ?? []) !== 64) {
    throw new Error("per-owner manual_regions sizing regressed");
  }

  // A recorded semantic lane must beat the describes-itself heuristic: once every
  // branch carries semantic C, having semantic sources no longer identifies the
  // lane owner, and the heuristic would draw Venus's lane from the exact tree.
  const semanticBearing: SourceTree = {
    id: "exact-with-semantic",
    list: (directory) => (directory === "semantic/main" ? ["08000000.c"] : []),
    read: () => undefined,
  };
  expectReject(
    () => resolveSemanticTree(semanticBearing, undefined, "origin/no-such-lane"),
    "an unresolvable recorded semantic lane",
  );
  if (resolveSemanticTree(semanticBearing, undefined, undefined).id !== "exact-with-semantic") {
    throw new Error("the describes-itself heuristic stopped applying with no recorded lane");
  }
  if (resolveSemanticTree(semanticBearing, undefined, "worktree").id !== "exact-with-semantic") {
    throw new Error("a recorded worktree lane did not fall through to the heuristic");
  }
  if (resolveSemanticTree(semanticBearing, "none", "origin/venus") !== undefined) {
    throw new Error("an explicit --semantic-ref none was overridden by the record");
  }

  // The README cache-buster: derived from the SVG, idempotent, and replacing any
  // version already present rather than accumulating them.
  const embed = "![alt](assets/readme/gs1-en-coverage.svg)";
  const once = readmeWithCacheBuster(embed, "gs1-en", "abcd1234");
  if (once !== "![alt](assets/readme/gs1-en-coverage.svg?v=abcd1234)") {
    throw new Error("the cache-buster was not applied to a bare embed");
  }
  if (readmeWithCacheBuster(once, "gs1-en", "abcd1234") !== once) {
    throw new Error("the cache-buster is not idempotent");
  }
  if (readmeWithCacheBuster(once, "gs1-en", "99887766")
      !== "![alt](assets/readme/gs1-en-coverage.svg?v=99887766)") {
    throw new Error("an existing cache-buster was not replaced");
  }
  if (svgCacheVersion("<svg/>") === svgCacheVersion("<svg />")) {
    throw new Error("the cache version does not track the SVG bytes");
  }
  if (svgCacheVersion("<svg/>") !== svgCacheVersion("<svg/>")) {
    throw new Error("the cache version is not deterministic");
  }

  // A ref tree must list subdirectories, not only files. mainBoundaries walks
  // `asm/` recursively; if a directory reports no children the walk stops at
  // the first level, boundaries go missing and every main-image region is
  // measured too long. Asserted against HEAD rather than the worktree so the
  // check holds mid-bank, when the worktree carries uncommitted sources.
  const headTree = refTree("HEAD");
  if (headTree === undefined) throw new Error("HEAD did not resolve to a tree");
  if (!headTree.list("asm").some((name) => !name.includes("."))) {
    throw new Error("ref tree listed no asm/ subdirectories; a tree walk would stop at the first level");
  }

  if (resolveExactTree(undefined, undefined).id !== "worktree") {
    throw new Error("the exact lane did not default to the worktree");
  }
  if (resolveExactTree(undefined, "worktree").id !== "worktree") {
    throw new Error("a recorded worktree exact lane was not honoured");
  }
  if (resolveExactTree("worktree", "origin/mercury").id !== "worktree") {
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

  const withLane = {
    lanes: { semantic_c: { bytes: 391428 } },
    provenance: { semantic_lane: "origin/venus" },
  } as CoverageMap;
  if (semanticEraseRefusal(false, undefined, withLane) === undefined) {
    throw new Error("an unresolved semantic lane was allowed to erase a tracked one");
  }
  if (semanticEraseRefusal(true, undefined, withLane) !== undefined) {
    throw new Error("a resolved semantic lane was refused");
  }
  if (semanticEraseRefusal(false, "none", withLane) !== undefined) {
    throw new Error("an explicit --semantic-ref none was refused");
  }
  if (semanticEraseRefusal(false, undefined, undefined) !== undefined) {
    throw new Error("a first write with no tracked map was refused");
  }
  if (semanticEraseRefusal(false, undefined,
      { lanes: { semantic_c: { bytes: 0 } }, provenance: { semantic_lane: "none" } } as CoverageMap) !== undefined) {
    throw new Error("a tracked map with no semantic lane was refused");
  }

  const tiles: Tile[] = [
    { label: "a", bytes: 60, lanes: { exact_c: 30, assembly: 30 } },
    { label: "b", bytes: 40, lanes: { semantic_c: 40 } },
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

  const bands = mainBands(
    [{ start: 0x08000000, end: 0x08000100 }],
    [{ start: 0x08000000, end: 0x08000040 }],
    [{ start: 0x08000040, end: 0x08000080 }],
    128,
  );
  if (bands.length !== 2 || bands[0].bytes !== 128 || bands[0].lanes.exact_c !== 64 ||
      bands[0].lanes.semantic_c !== 64 || bands[1].lanes.assembly !== 128) {
    throw new Error("main band composition failed");
  }

  const map: CoverageMap = {
    format: 1,
    kind: "golden-sun-rom-coverage-map",
    target: "gs1-en",
    derivation: "tracked-evidence-v1",
    rom_bytes: 1024,
    executable_bytes: 200,
    lanes: {
      exact_c: { bytes: 60, percent_of_executable: 30 },
      semantic_c: { bytes: 40, percent_of_executable: 20 },
      assembly: { bytes: 100, percent_of_executable: 50 },
      asset_data: { bytes: 824, percent_of_executable: 0 },
    },
    main: { executable_bytes: 120, exact_c_bytes: 60, semantic_c_bytes: 20 },
    overlays: { executable_bytes: 80, exact_c_bytes: 0, semantic_c_bytes: 20 },
    provenance: {
      exact_lane: "worktree",
      semantic_lane: "origin/venus",
      semantic_sources: 2,
      semantic_superseded_bytes: 0,
      semantic_unresolved: [],
    },
    rom_areas: [area("rom-data", "Assets & data", [
      { label: "Other data", bytes: 824, lanes: { asset_data: 824 } },
    ])],
    executable_areas: [area("main", "Main image", tiles)],
  };
  const svg = renderSvg(map);
  if (!svg.startsWith("<svg ") || !svg.trimEnd().endsWith("</svg>")) throw new Error("SVG shape failed");
  if (svg.includes("undefined") || svg.includes("NaN")) throw new Error("SVG contains unresolved values");
  for (const lane of LANE_ORDER) {
    if (!svg.toUpperCase().includes(escapeText(LANE_STYLE[lane].label).toUpperCase())) {
      throw new Error(`SVG does not name the ${lane} lane`);
    }
    if (!svg.includes(LANE_STYLE[lane].fill)) throw new Error(`SVG does not draw the ${lane} lane`);
  }
  const openTags = (svg.match(/<rect /g) ?? []).length;
  if (openTags < 4) throw new Error("SVG drew no tiles");

  expectReject(() => optionsOf(["--unknown"]), "unknown argument");
  if (optionsOf(["--semantic-ref", "none"]).semantic !== "none") throw new Error("option parsing failed");

  console.log("self-test=ok tool=coverage_map");
}

async function main(argv: string[]): Promise<void> {
  const options = optionsOf(argv);
  if (options.selfTest) return selfTest();
  // The tracked map records where each lane was drawn from, so a redraw keeps
  // drawing it the same way unless told otherwise. Mercury and Venus record
  // "worktree" and are unaffected; main records origin/mercury.
  const trackedDocumentOnDisk = existsSync(mapPath(options.target))
    ? (JSON.parse(readFileSync(mapPath(options.target), "utf8")) as CoverageMap)
    : undefined;
  const exact = resolveExactTree(options.exact, trackedDocumentOnDisk?.provenance.exact_lane);
  const semantic = resolveSemanticTree(
    exact,
    options.semantic,
    trackedDocumentOnDisk?.provenance.semantic_lane,
  );
  const map = buildCoverageMap({ target: options.target, exact, semantic });
  const svg = renderSvg(map);
  const json = canonicalJson(trackedDocument(map));

  if (options.check) {
    const trackedMap = readFileSync(mapPath(options.target), "utf8");
    const trackedSvg = readFileSync(svgPath(options.target), "utf8");
    const stale: string[] = [];
    // Only the lanes this tree owns can be enforced here: a picture generated
    // with another branch's semantic sources is refreshed by regeneration, not
    // by failing this branch's verification.
    const trackedJson = JSON.parse(trackedMap) as CoverageMap;
    if (trackedJson.lanes.exact_c.bytes !== map.lanes.exact_c.bytes ||
        trackedJson.main.exact_c_bytes !== map.main.exact_c_bytes ||
        trackedJson.overlays.exact_c_bytes !== map.overlays.exact_c_bytes) {
      stale.push("exact lane");
    }
    if (semantic && semantic.id === "worktree" &&
        trackedJson.lanes.semantic_c.bytes !== map.lanes.semantic_c.bytes) {
      stale.push("semantic lane");
    }
    if (trackedJson.executable_bytes !== map.executable_bytes) stale.push("executable denominator");
    if (semantic?.id === trackedJson.provenance.semantic_lane &&
        (trackedMap !== json || trackedSvg !== svg) && !stale.length) {
      stale.push("rendered map");
    }
    const expected = readmeWithCacheBuster(
      readFileSync(readmePath(), "utf8"),
      options.target,
      svgCacheVersion(trackedSvg),
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
    writeFileSync(svgPath(options.target), svg);
    // Keep the README's cache-buster in step with the picture it busts. Doing it
    // here rather than by hand means the two cannot drift.
    const readme = readFileSync(readmePath(), "utf8");
    const busted = readmeWithCacheBuster(readme, options.target, svgCacheVersion(svg));
    if (busted !== readme) writeFileSync(readmePath(), busted);
    console.log(
      `map=${mapPath(options.target).slice(ROOT.length + 1)} ` +
      `svg=${svgPath(options.target).slice(ROOT.length + 1)} ${summarize(map)}`,
    );
    return;
  }
  console.log(summarize(map));
}

if (import.meta.main) await main(Bun.argv.slice(2));
