import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { decode_palette_trace, encode_palette, type PaletteGroup, type PaletteOperation } from "./extract_resource.ts";
import { tile_png } from "./export_asset.ts";
import { gba_graphics, type Rgb } from "./import_asset.ts";

const ROM_BASE = 0x08000000;
const RESOURCE_TABLE = 0x08320000;

export interface TitleComponent {
  source: string;
  offset: number;
  size: number;
  bpp: 4 | 8;
  width: number;
  height: number;
  role: string;
}

export interface TitlePlan {
  format: 1;
  codec: "golden-sun-title-lz";
  resource_id: number;
  address: string;
  container_size: number;
  source_size: number;
  decoded_size: number;
  palette_entries: number;
  palette_upload_entries: number;
  tail: { size: number; policy: "zero" | "fallback" };
  components: TitleComponent[];
  consumer: Record<string, unknown>;
  groups: CompactGroup[];
}

export type CompactGroup = 0 | number[];

interface FixedComponent extends Omit<TitleComponent, "size"> {
  size?: number;
}

interface FixedResource {
  id: number;
  decoded: number;
  paletteUpload: number;
  components: FixedComponent[];
  consumer: Record<string, unknown>;
}

const RESOURCES: FixedResource[] = [
  {
    id: 0x15,
    decoded: 0x1e00,
    paletteUpload: 256,
    components: [
      { source: "objects/hikari_64.8bpp.png", offset: 0x0000, bpp: 8, width: 64, height: 64, role: "hikari" },
      { source: "objects/taiyou_32.8bpp.png", offset: 0x1000, bpp: 8, width: 32, height: 32, role: "taiyou" },
      { source: "objects/niji_32.8bpp.png", offset: 0x1400, bpp: 8, width: 32, height: 32, role: "niji" },
      { source: "objects/niji_16_a.8bpp.png", offset: 0x1800, bpp: 8, width: 16, height: 16, role: "niji_a" },
      { source: "objects/niji_16_b.8bpp.png", offset: 0x1900, bpp: 8, width: 16, height: 16, role: "niji_b" },
      { source: "objects/niji_16_c.8bpp.png", offset: 0x1a00, bpp: 8, width: 16, height: 16, role: "niji_c" },
      { source: "objects/niji_16_d.8bpp.png", offset: 0x1b00, bpp: 8, width: 16, height: 16, role: "niji_d" },
      { source: "objects/niji_16_e.8bpp.png", offset: 0x1c00, bpp: 8, width: 16, height: 16, role: "niji_e" },
      { source: "objects/niji_16_f.8bpp.png", offset: 0x1d00, bpp: 8, width: 16, height: 16, role: "niji_f" },
    ],
    consumer: {
      palette: "obj", vram: "obj", mapping: "1d", tile_depth: 8,
      objects: [
        { role: "hikari", tile: 0, width: 64, height: 64 },
        { role: "taiyou", tile: 64, width: 32, height: 32 },
        { role: "niji", tile: 80, width: 32, height: 32 },
        { role: "niji_a", tile: 96, width: 16, height: 16 },
        { role: "niji_b", tile: 100, width: 16, height: 16 },
        { role: "niji_c", tile: 104, width: 16, height: 16 },
        { role: "niji_d", tile: 108, width: 16, height: 16 },
        { role: "niji_e", tile: 112, width: 16, height: 16 },
        { role: "niji_f", tile: 116, width: 16, height: 16 },
      ],
    },
  },
  {
    id: 0x16,
    decoded: 0x9600,
    paletteUpload: 240,
    components: [
      { source: "title.8bpp.png", offset: 0, bpp: 8, width: 240, height: 160, role: "title" },
    ],
    consumer: {
      palette: "bg", vram_address: "0x06004000", tile_depth: 8,
      tilemap: { width: 30, height: 20, screen_base: 6, first_tile: 256 },
    },
  },
  {
    id: 0x17,
    decoded: 0x14dc0,
    paletteUpload: 256,
    components: [
      { source: "sora.4bpp.png", offset: 0, bpp: 4, width: 240, height: 344, role: "sora" },
      { source: "iwa.8bpp.png", offset: 0xa140, bpp: 8, width: 240, height: 184, role: "iwa" },
    ],
    consumer: {
      palette: "bg", map_width: 30, map_height: 21, circular_rows: 32,
      uploads: [
        { role: "sora", source_row: 11, rows: 21, vram_address: "0x06000000", tile_depth: 4 },
        { role: "iwa", source_row: 0, rows: 21, vram_address: "0x06004ec0", tile_depth: 8 },
      ],
    },
  },
];

function resourcePointer(rom: Buffer, id: number): number {
  const offset = RESOURCE_TABLE - ROM_BASE + id * 4;
  if (offset < 0 || offset + 4 > rom.length) throw new Error("resource pointer lies outside the ROM");
  const address = rom.readUInt32LE(offset);
  if (address < ROM_BASE || address > ROM_BASE + rom.length) throw new Error("resource pointer lies outside the ROM");
  return address - ROM_BASE;
}

function colors(raw: Uint8Array): Rgb[] {
  if (raw.length !== 0x200) throw new Error("title palette must contain 256 colors");
  const data = new DataView(raw.buffer, raw.byteOffset, raw.byteLength);
  return Array.from({ length: 256 }, (_, index) => {
    const value = data.getUint16(index * 2, true);
    if (value & 0x8000) throw new Error("title palette contains a high-bit color");
    return [(value & 31) << 3, (value >>> 5 & 31) << 3, (value >>> 10 & 31) << 3];
  });
}

function componentSize(component: FixedComponent): number {
  const result = component.width * component.height * component.bpp / 8;
  if (!Number.isInteger(result)) throw new Error("component size is fractional");
  return result;
}

function validateComponents(components: readonly TitleComponent[], decodedSize: number): void {
  let cursor = 0;
  for (const component of components) {
    if (component.offset !== cursor || component.size !== component.width * component.height * component.bpp / 8)
      throw new Error("title components are not a contiguous native layout");
    if (component.width % 8 || component.height % 8) throw new Error("title component dimensions are not tile aligned");
    cursor += component.size;
  }
  if (cursor !== decodedSize) throw new Error("title components do not cover decoded output");
}

export function compactGroups(groups: readonly PaletteGroup[]): CompactGroup[] {
  return groups.map((group) => {
    if (group[0] === "z") return 0;
    let flags = 0;
    const result: number[] = [0];
    group[1].forEach((operation, index) => {
      if (operation[0] === "l") return;
      flags |= 1 << (7 - index);
      if (operation[0] === "e") result.push(0, 0);
      else result.push(operation[1], operation[2]);
    });
    if (!flags) throw new Error("nonzero title group has no copy operation");
    result[0] = flags;
    return result;
  });
}

export function expandGroups(groups: readonly CompactGroup[]): PaletteGroup[] {
  let ended = false;
  const result: PaletteGroup[] = [];
  for (const group of groups) {
    if (ended) throw new Error("title groups contain data after terminator");
    if (group === 0) { result.push(["z"]); continue; }
    if (!Array.isArray(group) || group.length < 3 || group.some((value) => !Number.isInteger(value)))
      throw new Error("invalid compact title group");
    const flags = group[0];
    if (flags < 1 || flags > 255) throw new Error("compact title flags are invalid");
    const operations: PaletteOperation[] = [];
    let cursor = 1;
    for (let bit = 7; bit >= 0; bit--) {
      if (!(flags & 1 << bit)) { operations.push(["l"]); continue; }
      if (cursor + 1 >= group.length) throw new Error("compact title copy is truncated");
      const length = group[cursor++], distance = group[cursor++];
      if (length === 0 && distance === 0) { operations.push(["e"]); ended = true; break; }
      operations.push(["c", length, distance]);
    }
    if (cursor !== group.length) throw new Error("compact title group has unused values");
    result.push(["g", operations]);
  }
  if (!ended) throw new Error("compact title groups lack a terminator");
  return result;
}

function sourceGraphics(component: TitleComponent): [Buffer, Buffer] {
  const [tiles, colors, report] = gba_graphics(readFileSync(component.source), component.bpp);
  if (report.width !== component.width || report.height !== component.height || tiles.length !== component.size)
    throw new Error(`title image dimensions differ: ${component.source}`);
  return [tiles, colors];
}

export function build_title_resource(planPath: string): Buffer {
  const plan = JSON.parse(readFileSync(planPath, "utf8")) as TitlePlan;
  if (plan.format !== 1 || plan.codec !== "golden-sun-title-lz") throw new Error("unsupported title-resource plan");
  const components = plan.components.map((component) => ({ ...component, source: join(dirname(planPath), component.source) }));
  validateComponents(components, plan.decoded_size);
  const decoded = Buffer.alloc(plan.decoded_size);
  const graphics = components.map((component) => sourceGraphics(component));
  const palette = graphics.find(([, colors]) => colors.length === plan.palette_entries * 2)?.[1];
  if (!palette) throw new Error("full title palette is missing");
  for (const [index, component] of components.entries()) {
    const [tiles, componentPalette] = graphics[index];
    if (!palette.subarray(0, componentPalette.length).equals(componentPalette))
      throw new Error(`title image palette differs: ${component.source}`);
    tiles.copy(decoded, component.offset);
  }
  if (palette.length !== plan.palette_entries * 2) throw new Error("title palette has the wrong size");
  const encoded = Buffer.concat([palette, encode_palette(decoded, expandGroups(plan.groups))]);
  if (encoded.length !== plan.source_size) throw new Error("title source size differs from its plan");
  if (plan.tail.size !== plan.container_size - plan.source_size || plan.tail.size < 0)
    throw new Error("title tail size differs from its plan");
  if (plan.tail.policy === "zero") return Buffer.concat([encoded, Buffer.alloc(plan.tail.size)]);
  if (plan.tail.policy !== "fallback") throw new Error("unsupported title tail policy");
  return encoded;
}

function exportResource(rom: Buffer, root: string, fixed: FixedResource): [number, number] {
  const start = resourcePointer(rom, fixed.id), end = resourcePointer(rom, fixed.id + 1);
  if (end <= start + 0x200) throw new Error("title resource is empty");
  const palette = rom.subarray(start, start + 0x200);
  const [decoded, cursor, tokens] = decode_palette_trace(rom, start + 0x200, end, fixed.decoded);
  if (decoded.length !== fixed.decoded) throw new Error("title decoded size differs from consumer bounds");
  const replay = Buffer.concat([palette, encode_palette(decoded, tokens)]);
  if (!replay.equals(rom.subarray(start, cursor))) throw new Error("title codec replay differs");
  const tail = rom.subarray(cursor, end);
  const tailPolicy = tail.every((value) => value === 0) ? "zero" : "fallback";
  const directory = join(root, `resource_${fixed.id.toString(16)}`);
  const components: TitleComponent[] = fixed.components.map((component) => ({
    ...component, size: componentSize(component),
  }));
  validateComponents(components, decoded.length);
  const paletteColors = colors(palette);
  for (const component of components) {
    const image = tile_png(
      decoded.subarray(component.offset, component.offset + component.size),
      component.bpp, component.width / 8,
      component.bpp === 4 ? paletteColors.slice(0, 16) : paletteColors,
    )[0];
    const output = join(directory, component.source);
    mkdirSync(dirname(output), { recursive: true });
    writeFileSync(output, image);
  }
  const plan: TitlePlan = {
    format: 1,
    codec: "golden-sun-title-lz",
    resource_id: fixed.id,
    address: `0x${(ROM_BASE + start).toString(16).padStart(8, "0")}`,
    container_size: end - start,
    source_size: cursor - start,
    decoded_size: decoded.length,
    palette_entries: 256,
    palette_upload_entries: fixed.paletteUpload,
    tail: { size: tail.length, policy: tailPolicy },
    components,
    consumer: fixed.consumer,
    groups: compactGroups(tokens),
  };
  const planPath = join(directory, "container.json");
  mkdirSync(directory, { recursive: true });
  writeFileSync(planPath, `${JSON.stringify(plan)}\n`);
  const built = build_title_resource(planPath);
  if (!built.equals(rom.subarray(start, start + built.length))) throw new Error("title canonical source replay differs");
  return [built.length, end - start - built.length];
}

export function export_title_resources(romPath: string, root: string): [number, number] {
  const rom = readFileSync(romPath);
  let source = 0, fallback = 0;
  for (const fixed of RESOURCES) {
    const result = exportResource(rom, root, fixed);
    source += result[0]; fallback += result[1];
  }
  return [source, fallback];
}

function selfTest(): void {
  for (const resource of RESOURCES) {
    const components = resource.components.map((component) => ({ ...component, size: componentSize(component) }));
    validateComponents(components, resource.decoded);
  }
  const groups: PaletteGroup[] = [["z"], ["g", [["l"], ["c", 2, 1], ["e"]]]];
  if (JSON.stringify(expandGroups(compactGroups(groups))) !== JSON.stringify(groups))
    throw new Error("compact title group round trip differs");
  let rejected = false;
  try { expandGroups([[0x80, 2]]); } catch { rejected = true; }
  if (!rejected) throw new Error("truncated compact title group was accepted");
  console.log("self-test=ok");
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || index + 1 >= args.length) throw new Error(`${name} is required`);
  return args[index + 1];
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: title_resources.ts [--self-test] [export ROM --directory DIR]");
    return;
  }
  if (args.includes("--self-test")) {
    selfTest();
    args = args.filter((argument) => argument !== "--self-test");
    if (!args.length) return;
  }
  if (args[0] !== "export" || !args[1]) throw new Error("export ROM is required");
  const [source, fallback] = export_title_resources(args[1], option(args, "--directory"));
  console.log(`resources=${RESOURCES.length} source=${source} fallback=${fallback}`);
}

if (import.meta.main) main(process.argv.slice(2));
