#!/usr/bin/env bun
// ROM coverage map: a SpaceMonger-style treemap of the audited GS1-EN image.
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
const LANE_STYLE: Record<Lane, { fill: string; edge: string; label: string }> = {
  exact_c: { fill: "#3f9c53", edge: "#2b6f3a", label: "byte-exact C" },
  semantic_c: { fill: "#e8c22e", edge: "#a98a12", label: "semantic C" },
  assembly: { fill: "#c9573f", edge: "#8f3a27", label: "assembly" },
  asset_data: { fill: "#6f8fb0", edge: "#4a6580", label: "assets & data" },
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
  note?: string;
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
  const directories = new Map<string, string[]>();
  for (const path of paths) {
    const directory = path.includes("/") ? path.slice(0, path.lastIndexOf("/")) : "";
    if (!directories.has(directory)) directories.set(directory, []);
    directories.get(directory)!.push(basename(path));
  }
  return {
    id: ref,
    list: (directory) => [...(directories.get(directory.replace(/\/$/, "")) ?? [])].sort(),
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
  for (const name of tree.list("semantic/overlays")) {
    const match = SEMANTIC_OVERLAY_SOURCE.exec(name);
    if (!match) continue;
    sources++;
    const overlay = match[1];
    const address = Number.parseInt(match[2], 16);
    const span = reviewed.get(`${overlay}@${address}`);
    if (span === undefined) {
      // The decoded-region inventory that sizes ordinary overlay owners is a
      // build diagnostic, so an unlisted owner is reported, not estimated.
      unresolved.push(`semantic/overlays/${name}`);
      continue;
    }
    overlays.set(overlay, [...(overlays.get(overlay) ?? []), { start: address, end: address + span }]);
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
      current = { label: tile.label, bytes: 0, lanes: {}, note: tile.note };
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
    ? semanticSpans(options.semantic, boundaries, mainExecutable)
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
    area("main", "Main image", mainBands(mainExecutable, exactMainUnion, semanticMain, 6144)),
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
      note: "shaded by decoded coverage",
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

function tileRects(tile: Tile, rect: Rect, lines: string[]): void {
  const bytes = tile.bytes;
  let offset = 0;
  for (const lane of LANE_ORDER) {
    const share = tile.lanes[lane] ?? 0;
    if (share <= 0) continue;
    const height = rect.height * (share / bytes);
    const style = LANE_STYLE[lane];
    // Lanes stack from the bottom of the tile so a filling tile reads as
    // progress rather than as a different region.
    const y = rect.y + rect.height - offset - height;
    lines.push(
      `<rect x="${round(rect.x)}" y="${round(y)}" width="${round(rect.width)}" ` +
      `height="${round(height)}" fill="${style.fill}"/>`,
    );
    offset += height;
  }
  lines.push(
    `<rect x="${round(rect.x)}" y="${round(rect.y)}" width="${round(rect.width)}" ` +
    `height="${round(rect.height)}" fill="none" stroke="#20262c" stroke-width="0.6"/>`,
  );
  // Tiles keep their label only when it fits whole, the way SpaceMonger drops
  // a name rather than showing a stub.
  const label = rect.height >= 11 ? fitText(tile.label, rect.width - 4, 9) : undefined;
  if (label === tile.label) {
    lines.push(
      `<text x="${round(rect.x + rect.width / 2)}" y="${round(rect.y + rect.height / 2 + 3.4)}" ` +
      `text-anchor="middle" font-size="9" fill="#0d1114" opacity="0.75">${escapeText(label)}</text>`,
    );
  }
}

/** Longest prefix of `text` that fits `width` at the given font size. */
function fitText(text: string, width: number, fontSize: number): string | undefined {
  const perCharacter = fontSize * 0.58;
  const room = Math.floor(width / perCharacter);
  if (room < 3) return undefined;
  return text.length <= room ? text : `${text.slice(0, Math.max(room - 1, 1))}…`;
}

function panel(
  title: string,
  subtitle: string,
  areas: readonly Area[],
  rect: Rect,
  lines: string[],
): void {
  lines.push(
    `<rect x="${rect.x}" y="${rect.y}" width="${rect.width}" height="${rect.height}" ` +
    `fill="#c9ccd1" stroke="#5a6068" stroke-width="1"/>`,
    `<rect x="${rect.x}" y="${rect.y}" width="${rect.width}" height="19" fill="#3a4a5c"/>`,
    `<text x="${rect.x + 7}" y="${rect.y + 13}" font-size="11" font-weight="bold" fill="#f2f4f6">` +
    `${escapeText(title)}</text>`,
    `<text x="${rect.x + rect.width - 7}" y="${rect.y + 13}" font-size="10" text-anchor="end" ` +
    `fill="#c3ccd6">${escapeText(subtitle)}</text>`,
  );
  const body: Rect = {
    x: rect.x + 4,
    y: rect.y + 23,
    width: rect.width - 8,
    height: rect.height - 27,
  };
  const placedAreas = squarify(
    [...areas].sort((left, right) => right.bytes - left.bytes || left.id.localeCompare(right.id)),
    (item) => item.bytes,
    body,
  );
  for (const { item, rect: areaRect } of placedAreas) {
    lines.push(
      `<rect x="${round(areaRect.x)}" y="${round(areaRect.y)}" width="${round(areaRect.width)}" ` +
      `height="${round(areaRect.height)}" fill="#eceef1" stroke="#41474e" stroke-width="1"/>`,
    );
    const size = `${commas(item.bytes)} B`;
    const sizeFits = areaRect.width >= 210;
    const label = fitText(item.label, areaRect.width - (sizeFits ? size.length * 5.4 + 24 : 10), 10.5);
    if (label) {
      lines.push(
        `<text x="${round(areaRect.x + 5)}" y="${round(areaRect.y + 12)}" font-size="10.5" ` +
        `font-weight="bold" fill="#1d2329">${escapeText(label)}</text>`,
      );
    }
    if (sizeFits) {
      lines.push(
        `<text x="${round(areaRect.x + areaRect.width - 5)}" y="${round(areaRect.y + 12)}" font-size="9" ` +
        `text-anchor="end" fill="#4a525a">${size}</text>`,
      );
    }
    const inner: Rect = {
      x: areaRect.x + 3,
      y: areaRect.y + 16,
      width: areaRect.width - 6,
      height: areaRect.height - 19,
    };
    for (const placed of squarify(item.tiles, (tile) => tile.bytes, inner)) {
      tileRects(placed.item, placed.rect, lines);
    }
  }
}

export function renderSvg(map: CoverageMap): string {
  const width = 1180;
  const height = 660;
  const lines: string[] = [];
  const exact = map.lanes.exact_c;
  const semantic = map.lanes.semantic_c;
  const combined = exact.bytes + semantic.bytes;
  const combinedPercent = roundHalfUpPercent(combined, map.executable_bytes);

  lines.push(
    `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 ${width} ${height}" width="${width}" ` +
    `height="${height}" font-family="Verdana, DejaVu Sans, sans-serif" role="img" ` +
    `aria-label="Golden Sun English ROM coverage treemap">`,
    `<rect width="${width}" height="${height}" fill="#b7bcc3"/>`,
    `<rect x="6" y="6" width="${width - 12}" height="26" fill="#2b3a49"/>`,
    `<text x="16" y="24" font-size="13" font-weight="bold" fill="#f4f6f8">` +
    `Alchemy — Golden Sun (${map.target}) — ${commas(map.rom_bytes)} bytes</text>`,
    `<text x="${width - 16}" y="24" font-size="11" text-anchor="end" fill="#bcc7d2">` +
    `C-expressed ${commas(combined)} / ${commas(map.executable_bytes)} executable bytes (${combinedPercent}%)</text>`,
  );

  // Toolbar-style lane legend with the measured share of each lane.
  const legend: Array<[Lane, string]> = [
    ["exact_c", `byte-exact C — ${commas(exact.bytes)} B (${exact.percent_of_executable}%)`],
    ["semantic_c", `semantic C — ${commas(semantic.bytes)} B (${semantic.percent_of_executable}%)`],
    ["assembly", `assembly — ${commas(map.lanes.assembly.bytes)} B (${map.lanes.assembly.percent_of_executable}%)`],
    ["asset_data", `assets & data — ${commas(map.lanes.asset_data.bytes)} B`],
  ];
  lines.push(`<rect x="6" y="36" width="${width - 12}" height="24" fill="#d7dade" stroke="#8b9199"/>`);
  let cursor = 14;
  for (const [lane, text] of legend) {
    const style = LANE_STYLE[lane];
    lines.push(
      `<rect x="${cursor}" y="42" width="11" height="11" fill="${style.fill}" stroke="${style.edge}"/>`,
      `<text x="${cursor + 16}" y="51" font-size="10" fill="#20262c">${escapeText(text)}</text>`,
    );
    cursor += 22 + text.length * 5.55;
  }

  // One stacked bar across the executable denominator, so the headline is
  // readable before any tile is inspected.
  const barX = 6;
  const barWidth = width - 12;
  let barCursor = barX;
  lines.push(`<rect x="${barX}" y="64" width="${barWidth}" height="16" fill="#8f959c"/>`);
  for (const lane of ["exact_c", "semantic_c", "assembly"] as Lane[]) {
    const share = map.lanes[lane].bytes / map.executable_bytes;
    const laneWidth = barWidth * share;
    const percent = map.lanes[lane].percent_of_executable;
    lines.push(
      `<rect x="${round(barCursor)}" y="64" width="${round(laneWidth)}" height="16" ` +
      `fill="${LANE_STYLE[lane].fill}" stroke="${LANE_STYLE[lane].edge}" stroke-width="0.5"/>`,
    );
    if (laneWidth > 44) {
      lines.push(
        `<text x="${round(barCursor + laneWidth / 2)}" y="76" font-size="10" text-anchor="middle" ` +
        `fill="#141a1f">${percent}%</text>`,
      );
    }
    barCursor += laneWidth;
  }

  panel(
    "ROM image",
    `${commas(map.rom_bytes)} bytes on the cartridge`,
    map.rom_areas,
    { x: 6, y: 84, width: 436, height: 540 },
    lines,
  );
  panel(
    "Audited executable universe",
    `${commas(map.executable_bytes)} bytes of code`,
    map.executable_areas,
    { x: 448, y: 84, width: width - 454, height: 540 },
    lines,
  );

  lines.push(
    `<text x="12" y="640" font-size="9.5" fill="#2c333a">` +
    `Left: the whole cartridge — every byte already rebuilt from tracked sources, coloured by how it is expressed. ` +
    `Right: the audited executable denominator behind Full-C Byte Share.</text>`,
    `<text x="12" y="652" font-size="9.5" fill="#2c333a">` +
    `Main-image tiles are address bands; overlay tiles are one resource each. Compressed overlay tiles are sized by ROM ` +
    `bytes and shaded by the decoded share each lane owns. Exact C: ${escapeText(map.provenance.exact_lane)} · ` +
    `semantic C: ${escapeText(map.provenance.semantic_lane)}.</text>`,
    "</svg>",
  );
  return lines.join("\n") + "\n";
}

// ------------------------------------------------------------------- driver

function mapPath(target: DecompTargetId): string {
  return join(ROOT, "metrics", `${target}-coverage-map.json`);
}

function svgPath(target: DecompTargetId): string {
  return join(ROOT, "assets", "readme", `${target}-coverage.svg`);
}

/**
 * The semantic lane lives on the Venus branch. A tree that carries semantic
 * sources describes itself; otherwise the tracked Venus ref is used when it is
 * available locally, and the lane is reported as absent when it is not.
 */
export function resolveSemanticTree(exact: SourceTree, requested?: string): SourceTree | undefined {
  if (requested === "none") return undefined;
  if (requested) {
    const tree = requested === "worktree" ? workTree() : refTree(requested);
    if (!tree) throw new Error(`cannot resolve semantic lane ref ${requested}`);
    return tree;
  }
  if (exact.list("semantic/main").some((name) => MAIN_SOURCE.test(name))) return exact;
  for (const ref of ["venus", "origin/venus"]) {
    const tree = refTree(ref);
    if (tree && tree.list("semantic/main").some((name) => MAIN_SOURCE.test(name))) return tree;
  }
  return undefined;
}

interface Options {
  target: DecompTargetId;
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
    else if (argument === "--semantic-ref") options.semantic = argv[++index];
    else if (argument === "--write") options.write = true;
    else if (argument === "--check") options.check = true;
    else if (argument === "--self-test") options.selfTest = true;
    else if (argument === "-h" || argument === "--help") {
      console.log(
        "usage: coverage_map.ts [--target gs1-en|gs2-en] [--semantic-ref <ref>|worktree|none] " +
        "[--write|--check|--self-test]",
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
  if (!svg.includes("byte-exact C") || !svg.includes("semantic C")) throw new Error("SVG legend failed");
  const openTags = (svg.match(/<rect /g) ?? []).length;
  if (openTags < 4) throw new Error("SVG drew no tiles");

  expectReject(() => optionsOf(["--unknown"]), "unknown argument");
  if (optionsOf(["--semantic-ref", "none"]).semantic !== "none") throw new Error("option parsing failed");

  console.log("self-test=ok tool=coverage_map");
}

async function main(argv: string[]): Promise<void> {
  const options = optionsOf(argv);
  if (options.selfTest) return selfTest();
  const exact = workTree();
  const semantic = resolveSemanticTree(exact, options.semantic);
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
    if (stale.length) {
      throw new Error(
        `tracked coverage map is stale (${stale.join(", ")}); run: bun run coverage`,
      );
    }
    console.log(`coverage-map=current ${summarize(map)}`);
    return;
  }

  if (options.write) {
    writeFileSync(mapPath(options.target), json);
    writeFileSync(svgPath(options.target), svg);
    console.log(
      `map=${mapPath(options.target).slice(ROOT.length + 1)} ` +
      `svg=${svgPath(options.target).slice(ROOT.length + 1)} ${summarize(map)}`,
    );
    return;
  }
  console.log(summarize(map));
}

if (import.meta.main) await main(Bun.argv.slice(2));
