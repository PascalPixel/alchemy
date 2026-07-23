// Tool role: both; imported by tools/archive_asset.ts, tools/battle_effect_data.ts, tools/build_assets.ts (+27 more); invoked by CONVENTIONS.md, package.json.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname } from "node:path";
import { crc32, deflateSync, inflateSync } from "./zlib.ts";
import { canonicalJson } from "./canonical_json.ts";

export type Rgb = [number, number, number];
export type Report = Record<string, number>;

export function* chunks(data: Uint8Array, signature: Uint8Array): Generator<[Buffer, Buffer]> {
  const source = Buffer.from(data.buffer, data.byteOffset, data.byteLength);
  const marker = Buffer.from(signature.buffer, signature.byteOffset, signature.byteLength);
  if (!source.subarray(0, marker.length).equals(marker)) throw new Error("invalid file signature");
  let cursor = marker.length;
  while (cursor < source.length) {
    if (cursor + 12 > source.length) throw new Error("truncated chunk");
    const size = source.readUInt32BE(cursor);
    const kind = source.subarray(cursor + 4, cursor + 8);
    const end = cursor + 12 + size;
    if (end > source.length) throw new Error("truncated chunk payload");
    const payload = source.subarray(cursor + 8, cursor + 8 + size);
    const expected = source.readUInt32BE(cursor + 8 + size);
    if (crc32(Buffer.concat([kind, payload])) !== expected) throw new Error("chunk CRC mismatch");
    yield [kind, payload];
    cursor = end;
  }
}

export function paeth(left: number, above: number, upper_left: number): number {
  const prediction = left + above - upper_left;
  const a = Math.abs(prediction - left);
  const b = Math.abs(prediction - above);
  const c = Math.abs(prediction - upper_left);
  return a <= b && a <= c ? left : b <= c ? above : upper_left;
}

function unfilter(raw: Uint8Array, height: number, row_size: number, stride: number): Buffer[] {
  if (raw.length !== height * (row_size + 1)) throw new Error("unexpected decompressed PNG size");
  const rows: Buffer[] = [];
  let cursor = 0;
  let previous = Buffer.alloc(row_size);
  for (let y = 0; y < height; y++) {
    const method = raw[cursor];
    const source = raw.subarray(cursor + 1, cursor + 1 + row_size);
    cursor += row_size + 1;
    if (method > 4) throw new Error("invalid PNG filter");
    const row = Buffer.alloc(row_size);
    for (let index = 0; index < row_size; index++) {
      const left = index >= stride ? row[index - stride] : 0;
      const above = previous[index];
      const upperLeft = index >= stride ? previous[index - stride] : 0;
      const predictor = method === 0 ? 0 : method === 1 ? left : method === 2 ? above :
        method === 3 ? Math.floor((left + above) / 2) : paeth(left, above, upperLeft);
      row[index] = (source[index] + predictor) & 255;
    }
    rows.push(row);
    previous = row;
  }
  return rows;
}

export function indexed_png(data: Uint8Array): [number, number, number[], Rgb[]] {
  let width: number | undefined;
  let height: number | undefined;
  let depth: number | undefined;
  let palette: Rgb[] | undefined;
  let transparency: Buffer | undefined;
  const compressed: Buffer[] = [];
  for (const [kind, payload] of chunks(data, Buffer.from("89504e470d0a1a0a", "hex"))) {
    const name = kind.toString("ascii");
    if (name === "IHDR") {
      if (payload.length !== 13) throw new Error("invalid IHDR");
      width = payload.readUInt32BE(0);
      height = payload.readUInt32BE(4);
      depth = payload[8];
      const color = payload[9];
      if (color !== 3 || ![1, 2, 4, 8].includes(depth))
        throw new Error("PNG must use an indexed 1/2/4/8-bit palette");
      if (payload[10] || payload[11] || payload[12]) throw new Error("unsupported PNG encoding");
    } else if (name === "PLTE") {
      if (payload.length % 3 !== 0 || payload.length < 3 || payload.length > 768) throw new Error("invalid PLTE");
      palette = [];
      for (let index = 0; index < payload.length; index += 3)
        palette.push([payload[index], payload[index + 1], payload[index + 2]]);
    } else if (name === "tRNS") transparency = Buffer.from(payload);
    else if (name === "IDAT") compressed.push(payload);
    else if (name === "IEND") break;
  }
  if (width === undefined || height === undefined || depth === undefined || palette === undefined)
    throw new Error("PNG lacks IHDR or PLTE");
  if (width === 0 || height === 0 || width % 8 !== 0 || height % 8 !== 0)
    throw new Error("PNG dimensions must be nonzero multiples of eight");
  if (palette.length > 1 << depth) throw new Error("palette exceeds indexed bit depth");
  if (transparency && (transparency.length > palette.length || Array.from(transparency).some((alpha) => alpha !== 0 && alpha !== 255)))
    throw new Error("transparency must contain only binary alpha");
  const rowSize = Math.floor((width * depth + 7) / 8);
  const rows = unfilter(inflateSync(Buffer.concat(compressed)), height, rowSize, 1);
  const pixels: number[] = [];
  const mask = (1 << depth) - 1;
  for (const row of rows) for (let x = 0; x < width; x++) {
    const shift = 8 - depth - (x * depth % 8);
    const index = row[Math.floor(x * depth / 8)] >>> shift & mask;
    if (index >= palette.length) throw new Error("pixel references a missing palette entry");
    pixels.push(index);
  }
  return [width, height, pixels, palette];
}

export function rgba_png(data: Uint8Array): [number, number, Buffer] {
  let width: number | undefined;
  let height: number | undefined;
  const compressed: Buffer[] = [];
  for (const [kind, payload] of chunks(data, Buffer.from("89504e470d0a1a0a", "hex"))) {
    const name = kind.toString("ascii");
    if (name === "IHDR") {
      if (payload.length !== 13) throw new Error("invalid IHDR");
      width = payload.readUInt32BE(0);
      height = payload.readUInt32BE(4);
      if (payload[9] !== 6 || payload[8] !== 8) throw new Error("PNG must use 8-bit RGBA pixels");
      if (payload[10] || payload[11] || payload[12]) throw new Error("unsupported PNG encoding");
    } else if (name === "IDAT") compressed.push(payload);
    else if (name === "IEND") break;
  }
  if (width === undefined || height === undefined || width === 0 || height === 0)
    throw new Error("PNG lacks a nonempty IHDR");
  const rows = unfilter(inflateSync(Buffer.concat(compressed)), height, width * 4, 4);
  return [width, height, Buffer.concat(rows)];
}

export function gba_palette_rgba(data: Uint8Array): [Buffer, Report] {
  const [width, height, pixels] = rgba_png(data);
  const palette = Buffer.alloc(pixels.length / 2);
  for (let offset = 0; offset < pixels.length; offset += 4) {
    const [red, green, blue, alpha] = pixels.subarray(offset, offset + 4);
    if ((red & 7) || (green & 7) || (blue & 7)) throw new Error("RGBA palette colors must be multiples of eight");
    if (alpha !== 254 && alpha !== 255) throw new Error("RGBA palette alpha must be 254 or 255");
    palette.writeUInt16LE((red >>> 3) | (green >>> 3) << 5 | (blue >>> 3) << 10 | (255 - alpha) << 15, offset / 2);
  }
  return [palette, { width, height, palette_entries: palette.length / 2 }];
}

export function gba_graphics(data: Uint8Array, bpp: number): [Buffer, Buffer, Report] {
  const [width, height, pixels, palette] = indexed_png(data);
  const limit = bpp === 4 ? 16 : 256;
  if (palette.length > limit || pixels.some((pixel) => pixel >= limit)) throw new Error(`image does not fit ${bpp}bpp`);
  const colors = Buffer.alloc(palette.length * 2);
  palette.forEach(([red, green, blue], index) => {
    if ((red & 7) || (green & 7) || (blue & 7))
      throw new Error("palette channels must be exact five-bit values (multiples of 8)");
    colors.writeUInt16LE((red >>> 3) | (green >>> 3) << 5 | (blue >>> 3) << 10, index * 2);
  });
  const tiles: number[] = [];
  for (let top = 0; top < height; top += 8) for (let left = 0; left < width; left += 8) {
    const tile: number[] = [];
    for (let y = 0; y < 8; y++) for (let x = 0; x < 8; x++) tile.push(pixels[(top + y) * width + left + x]);
    if (bpp === 4) for (let index = 0; index < 64; index += 2) tiles.push(tile[index] | tile[index + 1] << 4);
    else tiles.push(...tile);
  }
  return [Buffer.from(tiles), colors, { width, height, bpp, tiles: width / 8 * (height / 8), palette_entries: palette.length }];
}

export function vlq(data: Uint8Array, cursor: number): [number, number] {
  let value = 0;
  for (let index = 0; index < 4; index++) {
    if (cursor >= data.length) throw new Error("truncated variable-length quantity");
    const byte = data[cursor++];
    value = value << 7 | byte & 0x7f;
    if (byte < 0x80) return [value, cursor];
  }
  throw new Error("overlong variable-length quantity");
}

export interface MidiEvent { tick: number; track: number; order: number; type?: string; meta?: number; status?: number; data?: string | number[] }
export interface MidiReport { format: number; tracks: number; ticks_per_quarter: number; events: MidiEvent[] }

export function midi_events(data: Uint8Array): MidiReport {
  const parsed: [string, Buffer][] = [];
  let cursor = 0;
  while (cursor < data.length) {
    if (cursor + 8 > data.length) throw new Error("truncated MIDI chunk");
    const kind = Buffer.from(data.subarray(cursor, cursor + 4)).toString("ascii");
    const size = new DataView(data.buffer, data.byteOffset + cursor + 4, 4).getUint32(0, false);
    cursor += 8;
    const payload = Buffer.from(data.subarray(cursor, cursor + size));
    cursor += size;
    if (payload.length !== size) throw new Error("truncated MIDI chunk payload");
    parsed.push([kind, payload]);
  }
  if (!parsed.length || parsed[0][0] !== "MThd" || parsed[0][1].length !== 6) throw new Error("invalid MIDI header");
  const form = parsed[0][1].readUInt16BE(0);
  const tracks = parsed[0][1].readUInt16BE(2);
  const division = parsed[0][1].readUInt16BE(4);
  if (![0, 1].includes(form) || tracks === 0 || division & 0x8000) throw new Error("only format 0/1 PPQN MIDI is supported");
  const trackChunks = parsed.slice(1).filter(([kind]) => kind === "MTrk").map(([, payload]) => payload);
  if (trackChunks.length !== tracks) throw new Error("MIDI track count mismatch");
  const result: MidiEvent[] = [];
  for (const [track, payload] of trackChunks.entries()) {
    let cursor = 0, tick = 0, order = 0;
    let running: number | undefined;
    while (cursor < payload.length) {
      let delta: number;
      [delta, cursor] = vlq(payload, cursor);
      tick += delta;
      if (cursor >= payload.length) throw new Error("truncated MIDI event");
      let status = payload[cursor];
      if (status < 0x80) {
        if (running === undefined) throw new Error("running status without channel status");
        status = running;
      } else cursor++;
      const event: MidiEvent = { tick, track, order: order++ };
      if (status === 0xff) {
        if (cursor >= payload.length) throw new Error("truncated meta event");
        const meta = payload[cursor];
        let size: number;
        [size, cursor] = vlq(payload, cursor + 1);
        const value = payload.subarray(cursor, cursor + size);
        cursor += size;
        if (value.length !== size) throw new Error("truncated meta payload");
        Object.assign(event, { type: "meta", meta, data: value.toString("hex") });
        running = undefined;
      } else if (status === 0xf0 || status === 0xf7) {
        let size: number;
        [size, cursor] = vlq(payload, cursor);
        const value = payload.subarray(cursor, cursor + size);
        cursor += size;
        if (value.length !== size) throw new Error("truncated system-exclusive payload");
        Object.assign(event, { type: "sysex", status, data: value.toString("hex") });
        running = undefined;
      } else if (status >= 0x80 && status <= 0xef) {
        running = status;
        const size = [0xc0, 0xd0].includes(status & 0xf0) ? 1 : 2;
        const value = Array.from(payload.subarray(cursor, cursor + size));
        cursor += size;
        if (value.length !== size || value.some((byte) => byte & 0x80)) throw new Error("invalid channel event");
        Object.assign(event, { type: "channel", status, data: value });
      } else throw new Error("unsupported MIDI system event");
      result.push(event);
    }
  }
  result.sort((a, b) => a.tick - b.tick || a.track - b.track || a.order - b.order);
  return { format: form, tracks, ticks_per_quarter: division, events: result };
}

export function make_chunk(kind: Uint8Array, payload: Uint8Array): Buffer {
  const body = Buffer.concat([Buffer.from(kind), Buffer.from(payload)]);
  const head = Buffer.alloc(4), tail = Buffer.alloc(4);
  head.writeUInt32BE(payload.length); tail.writeUInt32BE(crc32(body));
  return Buffer.concat([head, body, tail]);
}

export function make_midi_chunk(kind: Uint8Array, payload: Uint8Array): Buffer {
  const size = Buffer.alloc(4); size.writeUInt32BE(payload.length);
  return Buffer.concat([Buffer.from(kind), size, Buffer.from(payload)]);
}

export async function self_test(): Promise<void> {
  const palette = Buffer.from([0, 0, 0, 248, 0, 0]);
  const rows = Buffer.concat(Array.from({ length: 8 }, () => Buffer.from([0, 0x55])));
  const ihdr = Buffer.alloc(13); ihdr.writeUInt32BE(8, 0); ihdr.writeUInt32BE(8, 4); ihdr.set([1, 3, 0, 0, 0], 8);
  const png = Buffer.concat([Buffer.from("89504e470d0a1a0a", "hex"), make_chunk(Buffer.from("IHDR"), ihdr),
    make_chunk(Buffer.from("PLTE"), palette), make_chunk(Buffer.from("IDAT"), deflateSync(rows)), make_chunk(Buffer.from("IEND"), Buffer.alloc(0))]);
  const [tiles, colors, report] = gba_graphics(png, 4);
  if (!tiles.equals(Buffer.alloc(32, 0x10)) || !colors.equals(Buffer.from([0, 0, 0x1f, 0])) || report.tiles !== 1)
    throw new Error("graphics self-test failed");
  const track = Buffer.from("00c00500903c4060803c0000ff2f00", "hex");
  const midi = Buffer.concat([make_midi_chunk(Buffer.from("MThd"), Buffer.from("000000010060", "hex")), make_midi_chunk(Buffer.from("MTrk"), track)]);
  const midiReport = midi_events(midi);
  if (midiReport.ticks_per_quarter !== 96 || midiReport.events.map((event) => event.tick).join() !== "0,0,96,96")
    throw new Error("MIDI self-test failed");
  const { rgba_image, palette_rgba_image } = await import("./export_asset.ts");
  const rgba = Buffer.from(Array.from({ length: 7 * 5 * 4 }, (_, index) => (index * 29 + 3) & 255));
  const [image] = rgba_image(rgba, 7);
  const unpacked = rgba_png(image);
  if (unpacked[0] !== 7 || unpacked[1] !== 5 || !unpacked[2].equals(rgba)) throw new Error("RGBA self-test failed");
  const rawPalette = Buffer.alloc(256);
  for (let index = 0; index < 128; index++) rawPalette.writeUInt16LE(index * 109 & 0xffff, index * 2);
  const [paletteImage] = palette_rgba_image(rawPalette, 16);
  if (!gba_palette_rgba(paletteImage)[0].equals(rawPalette)) throw new Error("RGBA palette self-test failed");
  console.log("self-test=ok");
}

function option(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || index + 1 >= args.length) throw new Error(`${name} is required`);
  return args[index + 1];
}

function sortedJson(value: Record<string, unknown>): string {
  const sorted = Object.fromEntries(Object.keys(value).sort().map((key) => [key, value[key]]));
  return JSON.stringify(sorted).replace(/:/g, ": ").replace(/,/g, ", ");
}

async function main(args: string[]): Promise<void> {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: import_asset.ts [--self-test] {png,indexed,midi} ...");
    return;
  }
  if (args.includes("--self-test")) {
    await self_test();
    args = args.filter((arg) => arg !== "--self-test");
    if (args.length === 0) return;
  }
  const command = args[0];
  const input = args[1];
  if (!input) throw new Error("an asset command is required");
  if (command === "png") {
    const [tiles, palette, report] = gba_graphics(readFileSync(input), Number(option(args, "--bpp")));
    const tilePath = option(args, "--tiles"), palettePath = option(args, "--palette");
    mkdirSync(dirname(tilePath), { recursive: true }); mkdirSync(dirname(palettePath), { recursive: true });
    writeFileSync(tilePath, tiles); writeFileSync(palettePath, palette); console.log(sortedJson(report));
  } else if (command === "indexed") {
    const output = option(args, args.includes("-o") ? "-o" : "--output");
    const [width, height, pixels, palette] = indexed_png(readFileSync(input));
    mkdirSync(dirname(output), { recursive: true }); writeFileSync(output, Buffer.from(pixels));
    console.log(sortedJson({ width, height, pixels: pixels.length, palette_entries: palette.length }));
  } else if (command === "midi") {
    const output = option(args, args.includes("-o") ? "-o" : "--output");
    const report = midi_events(readFileSync(input));
    mkdirSync(dirname(output), { recursive: true }); writeFileSync(output, canonicalJson(report) + "\n");
    console.log(`tracks=${report.tracks} events=${report.events.length}`);
  } else throw new Error("an asset command is required");
}

if (import.meta.main) await main(process.argv.slice(2));
