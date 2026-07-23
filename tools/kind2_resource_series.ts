#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { canonicalJson, isCanonicalJsonText } from "./canonical_json.ts";
import {
  existsSync,
  mkdirSync,
  mkdtempSync,
  readFileSync,
  realpathSync,
  renameSync,
  rmSync,
  writeFileSync,
} from "fs";
import { tmpdir } from "os";
import { basename, dirname, isAbsolute, join, relative, resolve } from "path";
import { byte_png, palette_rgba_image, tile_png } from "./export_asset.ts";
import { gba_graphics, gba_palette_rgba, indexed_png, type Rgb } from "./import_asset.ts";
import {
  decode_kind2,
  encode_kind2,
  type Kind2Plan,
  type Kind2Token,
} from "./kind2_resource.ts";

const ROM_BASE = 0x08000000;
const RESOURCE_TABLE = 0x08320000;
const MAX_RESOURCE_ID = 999;
const MAX_LOOKAHEAD = 3;

type Presentation = "koma-4bpp" | "haikei-8bpp" | "naiyou";
type PresentationStatus = "audited" | "provisional-neutral-byte-canvas";

interface ImageSpec {
  encoding: Presentation;
  columns: number;
  status: PresentationStatus;
}

interface CatalogEntry {
  id: string;
  address: string;
  size: string;
  prefix_palette_size: string;
  image: ImageSpec;
}

interface CatalogDocument {
  format: 1;
  kind: "golden-sun-kind2-resource-catalog";
  resources: CatalogEntry[];
}

interface ProbeCandidate {
  prefix_palette_size: string;
  image: ImageSpec;
}

interface ProbeEntry {
  id: string;
  address: string;
  size: string;
  selected_prefix_palette_size: null;
  candidates: ProbeCandidate[];
}

interface ProbeDocument {
  format: 1;
  kind: "golden-sun-kind2-resource-probe";
  resources: ProbeEntry[];
}

interface IndexEntry {
  id: string;
  address: string;
  size: string;
  prefix_palette_size: string;
  presentation_status: PresentationStatus;
  source: string;
}

interface IndexDocument {
  format: 1;
  kind: "golden-sun-kind2-resource-series";
  resources: IndexEntry[];
}

interface ResourcePlan {
  format: 1;
  kind: "golden-sun-kind2-resource";
  resource_id: string;
  address: string;
  size: string;
  prefix_palette: null | {
    size: string;
    source: "iro.rgba.png";
  };
  stream: {
    format: 1;
    codec: "golden-sun-kind2-lz";
    encoded_size: string;
    decoded_size: string;
    tokens: Kind2Token[];
    lookahead: string;
  };
  image: {
    encoding: Presentation;
    source: "koma.4bpp.png" | "haikei.8bpp.png" | "naiyou.png";
    canvas_size: string;
    columns: number;
    status: PresentationStatus;
  };
}

export interface BuiltKind2Resource {
  id: number;
  address: number;
  data: Buffer;
  prefixPaletteSize: number;
  presentationStatus: PresentationStatus;
  sources: string[];
}

function exactKeys(value: Record<string, unknown>, expected: readonly string[], label: string): void {
  const actual = Object.keys(value).sort();
  const keys = [...expected].sort();
  if (actual.length !== keys.length || actual.some((key, index) => key !== keys[index])) {
    throw new Error(`${label} has unexpected fields`);
  }
}

function integer(value: unknown, label: string): number {
  const result = typeof value === "string" ? Number(value) : value;
  if (!Number.isSafeInteger(result)) throw new Error(`${label} must be an integer`);
  return result as number;
}

function hexadecimal(value: number, width = 8): string {
  return `0x${value.toString(16).padStart(width, "0")}`;
}

function idText(value: number): string {
  return hexadecimal(value, 3);
}

function parseId(value: unknown): number {
  if (typeof value !== "string" || !/^0x[0-9a-f]{3}$/.test(value)) {
    throw new Error("kind-2 resource ID is not canonical");
  }
  const result = Number(value);
  if (result < 0 || result > MAX_RESOURCE_ID) throw new Error("kind-2 resource ID is outside the directory");
  return result;
}

function parseHex(value: unknown, width: number, label: string): number {
  if (typeof value !== "string" || !new RegExp(`^0x[0-9a-f]{${width}}$`).test(value)) {
    throw new Error(`${label} is not canonical`);
  }
  return Number(value);
}

function readCanonicalJson(path: string, pretty: boolean, label: string): unknown {
  const text = readFileSync(path, "utf8");
  const value: unknown = JSON.parse(text);
  if (!isCanonicalJsonText(text, value)) throw new Error(`${label} is not canonical JSON`);
  return value;
}

function contained(root: string, path: string): boolean {
  const position = relative(root, path);
  return position === "" || (position !== ".." && !position.startsWith("../") &&
    !position.startsWith("..\\") && !isAbsolute(position));
}

function child(root: string, name: string): string {
  if (name.includes("/") || name.includes("\\") || name === "." || name === "..") {
    throw new Error("kind-2 source name must be a direct child");
  }
  const result = resolve(root, name);
  if (!contained(resolve(root), result)) throw new Error("kind-2 source escaped its directory");
  return result;
}

function indexedChild(root: string, name: string, id: number): string {
  const expected = `resource_${id.toString(16).padStart(3, "0")}/stream.json`;
  if (name !== expected) throw new Error("kind-2 index source name is not canonical");
  const result = resolve(root, name);
  if (!contained(resolve(root), result)) throw new Error("kind-2 index source escaped its series");
  return result;
}

function imageName(presentation: Presentation): ResourcePlan["image"]["source"] {
  if (presentation === "koma-4bpp") return "koma.4bpp.png";
  if (presentation === "haikei-8bpp") return "haikei.8bpp.png";
  return "naiyou.png";
}

function rowBytes(presentation: Presentation, columns: number): number {
  if (!Number.isSafeInteger(columns) || columns <= 0 || columns > 256) {
    throw new Error("kind-2 image columns are outside 1..256");
  }
  if (presentation === "koma-4bpp") return columns * 32;
  if (presentation === "haikei-8bpp") return columns * 64;
  if (presentation === "naiyou") return columns;
  throw new Error("unsupported kind-2 presentation");
}

function canvasSize(decodedSize: number, presentation: Presentation, columns: number): number {
  const row = rowBytes(presentation, columns);
  const block = presentation === "naiyou" ? row * 8 : row;
  return Math.ceil(decodedSize / block) * block;
}

function exactBytes(actual: Uint8Array, expected: Uint8Array, label: string): void {
  if (!Buffer.from(actual).equals(Buffer.from(expected))) throw new Error(`${label} is not canonical`);
}

function paletteColors(data: Uint8Array): Rgb[] {
  if (data.length % 2) throw new Error("kind-2 palette has an odd byte count");
  const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
  return Array.from({ length: data.length / 2 }, (_, index) => {
    const value = view.getUint16(index * 2, true);
    return [(value & 31) << 3, (value >>> 5 & 31) << 3, (value >>> 10 & 31) << 3];
  });
}

function parseImage(value: unknown, label: string): ImageSpec {
  if (typeof value !== "object" || value === null || Array.isArray(value)) {
    throw new Error(`${label} must be an object`);
  }
  const image = value as ImageSpec;
  exactKeys(image as unknown as Record<string, unknown>, ["encoding", "columns", "status"], label);
  rowBytes(image.encoding, image.columns);
  if (image.status !== "audited" && image.status !== "provisional-neutral-byte-canvas") {
    throw new Error(`${label} status is unsupported`);
  }
  return image;
}

function validateExtents(resources: Array<{ id: string; address: string; size: string }>, label: string): void {
  let previousEnd = ROM_BASE;
  const ids = new Set<number>();
  resources.forEach((entry, position) => {
    const id = parseId(entry.id);
    const address = parseHex(entry.address, 8, `${label} address`);
    const size = parseHex(entry.size, entry.size.length - 2, `${label} size`);
    if (ids.has(id)) throw new Error(`${label} repeats a resource ID`);
    if (size <= 0 || address < previousEnd) throw new Error(`${label} resources are not sorted nonoverlapping extents`);
    if (address < ROM_BASE || address + size > 0x0a000000) throw new Error(`${label} resource is outside the GBA ROM window`);
    ids.add(id);
    previousEnd = address + size;
    if (position > MAX_RESOURCE_ID) throw new Error(`${label} contains too many resources`);
  });
}

function parseCatalog(value: unknown): CatalogDocument {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error("kind-2 catalog must be an object");
  const catalog = value as CatalogDocument;
  exactKeys(catalog as unknown as Record<string, unknown>, ["format", "kind", "resources"], "kind-2 catalog");
  if (catalog.format !== 1 || catalog.kind !== "golden-sun-kind2-resource-catalog" || !Array.isArray(catalog.resources)) {
    throw new Error("unsupported kind-2 catalog");
  }
  validateExtents(catalog.resources, "kind-2 catalog");
  catalog.resources.forEach((entry) => {
    if (typeof entry !== "object" || entry === null || Array.isArray(entry)) throw new Error("kind-2 catalog entry must be an object");
    exactKeys(entry as unknown as Record<string, unknown>, ["id", "address", "size", "prefix_palette_size", "image"], "kind-2 catalog entry");
    const size = Number(entry.size);
    const prefix = parseHex(entry.prefix_palette_size, entry.prefix_palette_size.length - 2, "kind-2 palette size");
    if (prefix < 0 || prefix >= size || prefix % 2) throw new Error("kind-2 palette prefix is outside its resource");
    parseImage(entry.image, "kind-2 catalog image");
  });
  return catalog;
}

function parseProbe(value: unknown): ProbeDocument {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error("kind-2 probe must be an object");
  const probe = value as ProbeDocument;
  exactKeys(probe as unknown as Record<string, unknown>, ["format", "kind", "resources"], "kind-2 probe");
  if (probe.format !== 1 || probe.kind !== "golden-sun-kind2-resource-probe" || !Array.isArray(probe.resources)) {
    throw new Error("unsupported kind-2 probe");
  }
  validateExtents(probe.resources, "kind-2 probe");
  probe.resources.forEach((entry) => {
    if (typeof entry !== "object" || entry === null || Array.isArray(entry)) throw new Error("kind-2 probe entry must be an object");
    exactKeys(entry as unknown as Record<string, unknown>, [
      "id", "address", "size", "selected_prefix_palette_size", "candidates",
    ], "kind-2 probe entry");
    if (entry.selected_prefix_palette_size !== null || !Array.isArray(entry.candidates) || entry.candidates.length < 2) {
      throw new Error("kind-2 probe must remain unresolved with multiple candidates");
    }
    const size = Number(entry.size);
    const prefixes = new Set<number>();
    entry.candidates.forEach((candidate) => {
      if (typeof candidate !== "object" || candidate === null || Array.isArray(candidate)) {
        throw new Error("kind-2 probe candidate must be an object");
      }
      exactKeys(candidate as unknown as Record<string, unknown>, ["prefix_palette_size", "image"], "kind-2 probe candidate");
      const prefix = parseHex(
        candidate.prefix_palette_size,
        candidate.prefix_palette_size.length - 2,
        "kind-2 probe palette size",
      );
      if (prefix < 0 || prefix >= size || prefix % 2 || prefixes.has(prefix)) {
        throw new Error("kind-2 probe palette candidate is invalid");
      }
      prefixes.add(prefix);
      const image = parseImage(candidate.image, "kind-2 probe image");
      if (image.status !== "provisional-neutral-byte-canvas") {
        throw new Error("kind-2 unresolved probe presentation must remain provisional");
      }
    });
  });
  return probe;
}

export function validate_kind2_catalog(catalogFile: string, probeFile?: string): void {
  const catalog = parseCatalog(readCanonicalJson(catalogFile, true, "kind-2 catalog"));
  let probes = 0;
  if (probeFile !== undefined) {
    const probe = parseProbe(readCanonicalJson(probeFile, true, "kind-2 probe"));
    const catalogIds = new Set(catalog.resources.map((entry) => Number(entry.id)));
    if (probe.resources.some((entry) => catalogIds.has(Number(entry.id)))) {
      throw new Error("kind-2 unresolved probe overlaps the extraction catalog");
    }
    probes = probe.resources.length;
  }
  console.log(`catalog=ok resources=${catalog.resources.length} unresolved_probes=${probes}`);
}

function parseIndex(value: unknown): IndexDocument {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error("kind-2 index must be an object");
  const index = value as IndexDocument;
  exactKeys(index as unknown as Record<string, unknown>, ["format", "kind", "resources"], "kind-2 index");
  if (index.format !== 1 || index.kind !== "golden-sun-kind2-resource-series" || !Array.isArray(index.resources)) {
    throw new Error("unsupported kind-2 series index");
  }
  validateExtents(index.resources, "kind-2 index");
  index.resources.forEach((entry) => {
    if (typeof entry !== "object" || entry === null || Array.isArray(entry)) throw new Error("kind-2 index entry must be an object");
    exactKeys(entry as unknown as Record<string, unknown>, [
      "id", "address", "size", "prefix_palette_size", "presentation_status", "source",
    ], "kind-2 index entry");
    const size = Number(entry.size);
    const prefix = parseHex(entry.prefix_palette_size, entry.prefix_palette_size.length - 2, "kind-2 index palette size");
    if (prefix < 0 || prefix >= size || prefix % 2 ||
        (entry.presentation_status !== "audited" && entry.presentation_status !== "provisional-neutral-byte-canvas")) {
      throw new Error("kind-2 index provenance is invalid");
    }
    indexedChild(".", entry.source, parseId(entry.id));
  });
  return index;
}

function parsePlan(value: unknown): ResourcePlan {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error("kind-2 plan must be an object");
  const plan = value as ResourcePlan;
  exactKeys(plan as unknown as Record<string, unknown>, [
    "format", "kind", "resource_id", "address", "size", "prefix_palette", "stream", "image",
  ], "kind-2 plan");
  if (plan.format !== 1 || plan.kind !== "golden-sun-kind2-resource") throw new Error("unsupported kind-2 resource plan");
  parseId(plan.resource_id);
  parseHex(plan.address, 8, "kind-2 plan address");
  const size = parseHex(plan.size, plan.size.length - 2, "kind-2 plan size");
  let prefixSize = 0;
  if (plan.prefix_palette !== null) {
    if (typeof plan.prefix_palette !== "object" || Array.isArray(plan.prefix_palette)) throw new Error("kind-2 palette plan must be an object");
    exactKeys(plan.prefix_palette as unknown as Record<string, unknown>, ["size", "source"], "kind-2 palette plan");
    prefixSize = parseHex(plan.prefix_palette.size, plan.prefix_palette.size.length - 2, "kind-2 palette plan size");
    if (plan.prefix_palette.source !== "iro.rgba.png" || prefixSize <= 0 || prefixSize >= size || prefixSize % 2) {
      throw new Error("kind-2 palette plan is invalid");
    }
  }
  if (typeof plan.stream !== "object" || plan.stream === null || Array.isArray(plan.stream)) throw new Error("kind-2 stream plan must be an object");
  exactKeys(plan.stream as unknown as Record<string, unknown>, [
    "format", "codec", "encoded_size", "decoded_size", "tokens", "lookahead",
  ], "kind-2 stream plan");
  const encodedSize = parseHex(plan.stream.encoded_size, plan.stream.encoded_size.length - 2, "kind-2 encoded size");
  const decodedSize = parseHex(plan.stream.decoded_size, plan.stream.decoded_size.length - 2, "kind-2 decoded size");
  if (plan.stream.format !== 1 || plan.stream.codec !== "golden-sun-kind2-lz" ||
      encodedSize !== size - prefixSize || decodedSize <= 0 || !Array.isArray(plan.stream.tokens) ||
      !/^(?:[0-9a-f]{2}){0,3}$/.test(plan.stream.lookahead)) {
    throw new Error("kind-2 stream plan is invalid");
  }
  if (typeof plan.image !== "object" || plan.image === null || Array.isArray(plan.image)) throw new Error("kind-2 image plan must be an object");
  exactKeys(plan.image as unknown as Record<string, unknown>, [
    "encoding", "source", "canvas_size", "columns", "status",
  ], "kind-2 image plan");
  const expectedImage = imageName(plan.image.encoding);
  const expectedCanvas = canvasSize(decodedSize, plan.image.encoding, plan.image.columns);
  if (plan.image.source !== expectedImage || parseHex(plan.image.canvas_size, plan.image.canvas_size.length - 2, "kind-2 canvas size") !== expectedCanvas) {
    throw new Error("kind-2 image plan is invalid");
  }
  if (plan.image.status !== "audited" && plan.image.status !== "provisional-neutral-byte-canvas") {
    throw new Error("kind-2 image plan status is unsupported");
  }
  return plan;
}

function buildDecoded(plan: ResourcePlan, directory: string): Buffer {
  const source = child(directory, plan.image.source);
  const image = readFileSync(source);
  let data: Buffer;
  if (plan.image.encoding === "naiyou") {
    const [width, , pixels] = indexed_png(image);
    if (width !== plan.image.columns) throw new Error("kind-2 data image has the wrong width");
    data = Buffer.from(pixels);
    exactBytes(image, byte_png(data, plan.image.columns)[0], "kind-2 data image");
  } else {
    const bpp = plan.image.encoding === "koma-4bpp" ? 4 : 8;
    const [pixels, , report] = gba_graphics(image, bpp);
    if (report.width !== plan.image.columns * 8) throw new Error("kind-2 graphics image has the wrong width");
    data = pixels;
    exactBytes(image, tile_png(data, bpp, plan.image.columns, indexed_png(image)[3])[0], "kind-2 graphics image");
  }
  const decodedSize = Number(plan.stream.decoded_size);
  const canvas = Number(plan.image.canvas_size);
  if (data.length !== canvas || data.subarray(decodedSize).some((value) => value !== 0)) {
    throw new Error("kind-2 image has nonzero data outside its decoded stream");
  }
  return data.subarray(0, decodedSize);
}

export function build_kind2_resource(planFile: string): BuiltKind2Resource {
  const plan = parsePlan(readCanonicalJson(planFile, false, "kind-2 plan"));
  const directory = dirname(planFile);
  const sources = [planFile, child(directory, plan.image.source)];
  const decoded = buildDecoded(plan, directory);
  let prefix = Buffer.alloc(0);
  if (plan.prefix_palette !== null) {
    const paletteFile = child(directory, plan.prefix_palette.source);
    const image = readFileSync(paletteFile);
    prefix = gba_palette_rgba(image)[0];
    if (prefix.length !== Number(plan.prefix_palette.size)) throw new Error("kind-2 palette source has the wrong size");
    exactBytes(image, palette_rgba_image(prefix, 16)[0], "kind-2 palette source");
    sources.push(paletteFile);
  }
  const encoding: Kind2Plan = {
    format: 1,
    codec: "golden-sun-kind2-lz",
    decoded_size: decoded.length,
    encoded_size: Number(plan.stream.encoded_size),
    tokens: plan.stream.tokens,
    lookahead: plan.stream.lookahead,
  };
  const stream = encode_kind2(decoded, encoding);
  if (stream.length !== Number(plan.stream.encoded_size)) throw new Error("kind-2 stream has the wrong encoded size");
  const data = Buffer.concat([prefix, stream]);
  if (data.length !== Number(plan.size)) throw new Error("kind-2 resource has the wrong size");
  return {
    id: Number(plan.resource_id),
    address: Number(plan.address),
    data,
    prefixPaletteSize: prefix.length,
    presentationStatus: plan.image.status,
    sources,
  };
}

export function build_kind2_series(indexFile: string): BuiltKind2Resource[] {
  const index = parseIndex(readCanonicalJson(indexFile, true, "kind-2 index"));
  const directory = dirname(indexFile);
  return index.resources.map((entry) => {
    const built = build_kind2_resource(indexedChild(directory, entry.source, Number(entry.id)));
    if (built.id !== Number(entry.id) || built.address !== Number(entry.address) || built.data.length !== Number(entry.size)) {
      throw new Error("kind-2 resource differs from its series index");
    }
    if (built.prefixPaletteSize !== Number(entry.prefix_palette_size) ||
        built.presentationStatus !== entry.presentation_status) {
      throw new Error("kind-2 resource provenance differs from its series index");
    }
    return { ...built, sources: [indexFile, ...built.sources] };
  });
}

function resourcePointer(rom: Buffer, id: number): number {
  const offset = RESOURCE_TABLE - ROM_BASE + id * 4;
  if (offset < 0 || offset + 4 > rom.length) throw new Error("resource directory is outside the ROM");
  return rom.readUInt32LE(offset);
}

function nextResourcePointer(rom: Buffer, id: number): number {
  const current = resourcePointer(rom, id);
  for (let next = id + 1; next <= MAX_RESOURCE_ID; next++) {
    const pointer = resourcePointer(rom, next);
    if (pointer > current) return pointer;
  }
  throw new Error("kind-2 resource lacks a following directory pointer");
}

function checkedRange(rom: Buffer, entry: Pick<CatalogEntry, "id" | "address" | "size">): Buffer {
  const id = Number(entry.id);
  const address = Number(entry.address);
  const size = Number(entry.size);
  if (resourcePointer(rom, id) !== address || nextResourcePointer(rom, id) !== address + size) {
    throw new Error(`kind-2 resource ${entry.id} differs from its directory extent`);
  }
  const start = address - ROM_BASE;
  const result = rom.subarray(start, start + size);
  if (result.length !== size) throw new Error("kind-2 resource is outside the ROM");
  return result;
}

function validProbeCandidate(original: Buffer, candidate: ProbeCandidate): boolean {
  const prefixSize = Number(candidate.prefix_palette_size);
  const encoded = original.subarray(prefixSize);
  try {
    const [decoded, used, , tokens] = decode_kind2(encoded);
    const lookahead = encoded.subarray(used);
    if (decoded.length === 0 || lookahead.length > MAX_LOOKAHEAD) return false;
    const rebuilt = encode_kind2(decoded, {
      format: 1,
      codec: "golden-sun-kind2-lz",
      decoded_size: decoded.length,
      encoded_size: encoded.length,
      tokens,
      lookahead: lookahead.toString("hex"),
    });
    return rebuilt.equals(encoded);
  } catch {
    return false;
  }
}

function uniqueProbeCandidate(id: string, candidates: ProbeCandidate[]): ProbeCandidate {
  if (candidates.length !== 1) {
    throw new Error(`kind-2 resource ${id} has ${candidates.length} valid prefix candidates`);
  }
  return candidates[0];
}

function resolveProbeEntries(rom: Buffer, probe: ProbeDocument): CatalogEntry[] {
  return probe.resources.map((entry) => {
    const original = checkedRange(rom, entry);
    const selected = uniqueProbeCandidate(
      entry.id,
      entry.candidates.filter((candidate) => validProbeCandidate(original, candidate)),
    );
    return {
      id: entry.id,
      address: entry.address,
      size: entry.size,
      prefix_palette_size: selected.prefix_palette_size,
      image: selected.image,
    };
  });
}

function combinedCatalog(catalog: CatalogDocument, resolved: CatalogEntry[]): CatalogDocument {
  const resources = [...catalog.resources, ...resolved]
    .sort((left, right) => Number(left.address) - Number(right.address));
  validateExtents(resources, "resolved kind-2 catalog");
  if (new Set(resources.map((entry) => Number(entry.id))).size !== resources.length) {
    throw new Error("resolved kind-2 catalog repeats a resource ID");
  }
  return { format: 1, kind: "golden-sun-kind2-resource-catalog", resources };
}

function writeImage(directory: string, spec: ImageSpec, decoded: Buffer, palette: Buffer): [string, number] {
  const canvas = Buffer.alloc(canvasSize(decoded.length, spec.encoding, spec.columns));
  decoded.copy(canvas);
  const source = imageName(spec.encoding);
  let image: Buffer;
  if (spec.encoding === "naiyou") image = byte_png(canvas, spec.columns)[0];
  else {
    const bpp = spec.encoding === "koma-4bpp" ? 4 : 8;
    const colors = palette.length ? paletteColors(palette).slice(0, bpp === 4 ? 16 : 256) : undefined;
    image = tile_png(canvas, bpp, spec.columns, colors)[0];
  }
  writeFileSync(join(directory, source), image);
  return [source, canvas.length];
}

function exportOne(rom: Buffer, root: string, entry: CatalogEntry): IndexEntry {
  const original = checkedRange(rom, entry);
  const prefixSize = Number(entry.prefix_palette_size);
  const prefix = original.subarray(0, prefixSize);
  const encoded = original.subarray(prefixSize);
  let decoded: Buffer, used: number, tokens: Kind2Token[];
  try { [decoded, used, , tokens] = decode_kind2(encoded); }
  catch (error) { throw new Error(`kind-2 resource ${entry.id} cannot decode: ${String(error)}`); }
  const lookahead = encoded.subarray(used);
  if (lookahead.length > MAX_LOOKAHEAD) throw new Error("kind-2 stream leaves an opaque suffix");
  const id = Number(entry.id);
  const directory = join(root, `resource_${id.toString(16).padStart(3, "0")}`);
  mkdirSync(directory, { recursive: true });
  if (prefixSize) writeFileSync(join(directory, "iro.rgba.png"), palette_rgba_image(prefix, 16)[0]);
  const [source, canvas] = writeImage(directory, entry.image, decoded, prefix);
  const plan: ResourcePlan = {
    format: 1,
    kind: "golden-sun-kind2-resource",
    resource_id: entry.id,
    address: entry.address,
    size: entry.size,
    prefix_palette: prefixSize ? { size: entry.prefix_palette_size, source: "iro.rgba.png" } : null,
    stream: {
      format: 1,
      codec: "golden-sun-kind2-lz",
      encoded_size: hexadecimal(encoded.length, Math.max(1, encoded.length.toString(16).length)),
      decoded_size: hexadecimal(decoded.length, Math.max(1, decoded.length.toString(16).length)),
      tokens,
      lookahead: lookahead.toString("hex"),
    },
    image: {
      encoding: entry.image.encoding,
      source,
      canvas_size: hexadecimal(canvas, Math.max(1, canvas.toString(16).length)),
      columns: entry.image.columns,
      status: entry.image.status,
    },
  };
  const planFile = join(directory, "stream.json");
  writeFileSync(planFile, `${JSON.stringify(plan)}\n`);
  const built = build_kind2_resource(planFile);
  if (!built.data.equals(original)) throw new Error("kind-2 resource export does not round-trip");
  return {
    id: entry.id,
    address: entry.address,
    size: entry.size,
    prefix_palette_size: entry.prefix_palette_size,
    presentation_status: entry.image.status,
    source: `resource_${id.toString(16).padStart(3, "0")}/stream.json`,
  };
}

function writeKind2Series(rom: Buffer, catalog: CatalogDocument, directory: string): string {
  mkdirSync(directory, { recursive: true });
  const index: IndexDocument = {
    format: 1,
    kind: "golden-sun-kind2-resource-series",
    resources: catalog.resources.map((entry) => exportOne(rom, directory, entry)),
  };
  const indexFile = join(directory, "index.json");
  writeFileSync(indexFile, `${canonicalJson(index)}\n`);
  const built = build_kind2_series(indexFile);
  if (built.length !== catalog.resources.length) throw new Error("kind-2 series export is incomplete");
  return indexFile;
}

export function export_kind2_series(romFile: string, catalogFile: string, directory: string): void {
  const catalog = parseCatalog(readCanonicalJson(catalogFile, true, "kind-2 catalog"));
  const rom = readFileSync(romFile);
  writeKind2Series(rom, catalog, directory);
}

function verifyBuiltResources(rom: Buffer, built: BuiltKind2Resource[]): number {
  let bytes = 0;
  for (const resource of built) {
    const start = resource.address - ROM_BASE;
    const expected = rom.subarray(start, start + resource.data.length);
    if (expected.length !== resource.data.length || !resource.data.equals(expected)) {
      throw new Error(`kind-2 resource ${idText(resource.id)} differs from ROM`);
    }
    bytes += resource.data.length;
  }
  return bytes;
}

export function probe_export_kind2_series(
  romFile: string,
  catalogFile: string,
  probeFile: string,
  directory: string,
  expectedBytes?: number,
): void {
  const catalog = parseCatalog(readCanonicalJson(catalogFile, true, "kind-2 catalog"));
  const probe = parseProbe(readCanonicalJson(probeFile, true, "kind-2 probe"));
  const catalogIds = new Set(catalog.resources.map((entry) => Number(entry.id)));
  if (probe.resources.some((entry) => catalogIds.has(Number(entry.id)))) {
    throw new Error("kind-2 unresolved probe overlaps the extraction catalog");
  }
  const rom = readFileSync(romFile);
  const resolved = combinedCatalog(catalog, resolveProbeEntries(rom, probe));
  const declaredBytes = resolved.resources.reduce((sum, entry) => sum + Number(entry.size), 0);
  if (expectedBytes !== undefined && declaredBytes !== expectedBytes) {
    throw new Error(`resolved kind-2 catalog covers ${declaredBytes} bytes instead of ${expectedBytes}`);
  }
  const destination = resolve(directory);
  if (existsSync(destination)) throw new Error("refusing to replace an existing kind-2 source directory");
  const parent = dirname(destination);
  const leaf = basename(destination);
  if (!leaf) throw new Error("kind-2 source directory has no basename");
  mkdirSync(parent, { recursive: true });
  const temporary = mkdtempSync(join(parent, `.${leaf}-`));
  let installed = false;
  try {
    const indexFile = writeKind2Series(rom, resolved, temporary);
    const built = build_kind2_series(indexFile);
    const bytes = verifyBuiltResources(rom, built);
    if (bytes !== declaredBytes || built.length !== resolved.resources.length) {
      throw new Error("resolved kind-2 series verification is incomplete");
    }
    renameSync(temporary, destination);
    installed = true;
    console.log(`identical=true resources=${built.length} source_bytes=${bytes} installed=${destination}`);
  } finally {
    if (!installed) rmSync(temporary, { recursive: true, force: true });
  }
}

export function verify_kind2_series(romFile: string, indexFile: string): void {
  const rom = readFileSync(romFile);
  const built = build_kind2_series(indexFile);
  const bytes = verifyBuiltResources(rom, built);
  console.log(`identical=true resources=${built.length} source_bytes=${bytes}`);
}

function rejects(action: () => void): boolean {
  try { action(); return false; } catch { return true; }
}

export function self_test(): void {
  const temporary = mkdtempSync(join(tmpdir(), "alchemy-kind2-series-"));
  try {
    const firstDecoded = Buffer.from(Array.from({ length: 64 }, (_, index) => index * 37 & 255));
    const secondDecoded = Buffer.from(Array.from({ length: 96 }, (_, index) => index * 19 + 7 & 255));
    const firstPrefix = Buffer.alloc(0x20);
    for (let index = 0; index < firstPrefix.length / 2; index++) firstPrefix.writeUInt16LE(index * 997 & 0x7fff, index * 2);
    const firstStream = encode_kind2(firstDecoded, {
      format: 1, codec: "golden-sun-kind2-lz", tokens: Array(64).fill(4), lookahead: "0000",
    });
    const secondStream = encode_kind2(secondDecoded, {
      format: 1, codec: "golden-sun-kind2-lz", tokens: Array(96).fill(4), lookahead: "00",
    });
    const firstAddress = 0x08321000;
    const secondAddress = firstAddress + firstPrefix.length + firstStream.length;
    const end = secondAddress + secondStream.length;
    const rom = Buffer.alloc(end - ROM_BASE);
    rom.writeUInt32LE(firstAddress, RESOURCE_TABLE - ROM_BASE + 0x10 * 4);
    rom.writeUInt32LE(secondAddress, RESOURCE_TABLE - ROM_BASE + 0x11 * 4);
    rom.writeUInt32LE(end, RESOURCE_TABLE - ROM_BASE + 0x12 * 4);
    Buffer.concat([firstPrefix, firstStream]).copy(rom, firstAddress - ROM_BASE);
    secondStream.copy(rom, secondAddress - ROM_BASE);
    const romFile = join(temporary, "synthetic.gba");
    const catalogFile = join(temporary, "catalog.json");
    const output = join(temporary, "source");
    writeFileSync(romFile, rom);
    const catalog: CatalogDocument = {
      format: 1,
      kind: "golden-sun-kind2-resource-catalog",
      resources: [
        {
          id: "0x010",
          address: hexadecimal(firstAddress),
          size: hexadecimal(firstPrefix.length + firstStream.length, 3),
          prefix_palette_size: "0x20",
          image: { encoding: "koma-4bpp", columns: 2, status: "audited" },
        },
        {
          id: "0x011",
          address: hexadecimal(secondAddress),
          size: hexadecimal(secondStream.length, 3),
          prefix_palette_size: "0x0",
          image: { encoding: "naiyou", columns: 32, status: "audited" },
        },
      ],
    };
    writeFileSync(catalogFile, `${canonicalJson(catalog)}\n`);
    export_kind2_series(romFile, catalogFile, output);
    const built = build_kind2_series(join(output, "index.json"));
    if (built.length !== 2 || !built[0].data.equals(Buffer.concat([firstPrefix, firstStream])) ||
        !built[1].data.equals(secondStream)) {
      throw new Error("kind-2 series round-trip self-test failed");
    }
    const planFile = join(output, "resource_010/stream.json");
    const plan = JSON.parse(readFileSync(planFile, "utf8")) as ResourcePlan;
    plan.stream.lookahead = "00000000";
    writeFileSync(planFile, `${JSON.stringify(plan)}\n`);
    if (!rejects(() => build_kind2_series(join(output, "index.json"))) ||
        !rejects(() => indexedChild(output, "../private/stream.json", 0x10)) ||
        !rejects(() => parseCatalog({ format: 1, kind: "golden-sun-kind2-resource-catalog", resources: [] , extra: true })) ||
        !rejects(() => parseProbe({
          format: 1,
          kind: "golden-sun-kind2-resource-probe",
          resources: [{
            id: "0x012",
            address: hexadecimal(end),
            size: "0x100",
            selected_prefix_palette_size: "0x0",
            candidates: [],
          }],
        }))) {
      throw new Error("kind-2 series adversarial self-test failed");
    }
  } finally {
    rmSync(temporary, { recursive: true, force: true });
  }
  console.log("self-test=ok");
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || args[index + 1] === undefined) throw new Error(`${name} is required`);
  return args[index + 1];
}

function optionalOption(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index < 0 ? undefined : args[index + 1];
}

function same(left: string, right: string): boolean {
  try { return realpathSync(left) === realpathSync(right); }
  catch { return resolve(left) === resolve(right); }
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: kind2_resource_series.ts [--self-test] {catalog --catalog FILE [--probe FILE] | probe-export ROM --catalog FILE --probe FILE --directory DIR [--expected-bytes N] | export ROM --catalog FILE --directory DIR | verify ROM --index FILE}");
    return;
  }
  if (args.includes("--self-test")) {
    self_test();
    args = args.filter((argument) => argument !== "--self-test");
    if (!args.length) return;
  }
  const command = args[0];
  if (command === "catalog") {
    validate_kind2_catalog(option(args, "--catalog"), optionalOption(args, "--probe"));
    return;
  }
  const romFile = args[1];
  if (!romFile || (command !== "probe-export" && command !== "export" && command !== "verify")) {
    throw new Error("probe-export, export, or verify and a ROM are required");
  }
  if (command === "probe-export") {
    const catalog = option(args, "--catalog");
    const probe = option(args, "--probe");
    const directory = option(args, "--directory");
    const expected = optionalOption(args, "--expected-bytes");
    if (same(romFile, directory) || same(romFile, catalog) || same(romFile, probe)) {
      throw new Error("refusing to overwrite the input ROM");
    }
    probe_export_kind2_series(
      romFile,
      catalog,
      probe,
      directory,
      expected === undefined ? undefined : integer(expected, "expected kind-2 byte count"),
    );
  } else if (command === "export") {
    const catalog = option(args, "--catalog");
    const directory = option(args, "--directory");
    if (same(romFile, directory) || same(romFile, catalog)) throw new Error("refusing to overwrite the input ROM");
    export_kind2_series(romFile, catalog, directory);
    verify_kind2_series(romFile, join(directory, "index.json"));
  } else verify_kind2_series(romFile, option(args, "--index"));
}

if (import.meta.main) main(Bun.argv.slice(2));
