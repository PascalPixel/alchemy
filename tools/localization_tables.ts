#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join } from "node:path";

export const ROM_BASE = 0x08000000;

type ElementKind = "u8" | "s8" | "u16" | "u32" | "ascii-fixed";

interface NumericSegment {
  address: string;
  end: string;
  element: Exclude<ElementKind, "ascii-fixed">;
  stride: number;
  consumers: string[];
  values: number[];
}

interface TextSegment {
  address: string;
  end: string;
  element: "ascii-fixed";
  stride: 1;
  consumers: string[];
  text: string;
}

type TableSegment = NumericSegment | TextSegment;

export interface LocalizationTableSource {
  format: 1;
  kind: "golden-sun-localization-tables";
  address: string;
  size: string;
  segments: TableSegment[];
}

interface SegmentLayout {
  start: number;
  end: number;
  element: ElementKind;
  stride: number;
  consumers: string[];
}

interface RegionLayout {
  start: number;
  end: number;
  segments: SegmentLayout[];
}

const S = (start: number, end: number, element: ElementKind, stride: number, ...consumers: string[]): SegmentLayout =>
  ({ start, end, element, stride, consumers });

const REGIONS: RegionLayout[] = [
  {
    start: 0x08033e60,
    end: 0x08033ef8,
    segments: [
      S(0x08033e60, 0x08033eb0, "u8", 1, "Func_080191cc"),
      S(0x08033eb0, 0x08033ed8, "s8", 2, "Func_080191cc"),
      S(0x08033ed8, 0x08033ee8, "u8", 1),
      S(0x08033ee8, 0x08033ef8, "s8", 1, "Func_080191cc"),
    ],
  },
  {
    start: 0x080366f8,
    end: 0x080368d4,
    segments: [
      S(0x080366f8, 0x08036738, "u16", 2, "Func_0801908c", "Func_08021e6c", "Func_08028194"),
      S(0x08036738, 0x08036740, "s8", 1, "Func_0801908c"),
      S(0x08036740, 0x08036750, "s8", 1, "Func_0801a98c"),
      S(0x08036750, 0x080367b0, "u8", 1, "Func_0801ca1c", "Func_0801ccc0"),
      S(0x080367b0, 0x080367c9, "u8", 1),
      S(0x080367c9, 0x080367cc, "s8", 1, "Func_0801d108", "Func_0801d4cc"),
      S(0x080367cc, 0x080367ce, "s8", 1, "Func_0801d108", "Func_0801d4cc"),
      S(0x080367ce, 0x080367d0, "s8", 1, "Func_0801d108", "Func_0801d4cc"),
      S(0x080367d0, 0x080367d6, "s8", 1, "Func_0801d9d4"),
      S(0x080367d6, 0x080367dc, "s8", 1, "Func_0801d9d4"),
      S(0x080367dc, 0x080367e4, "s8", 1, "Func_0801d9d4"),
      S(0x080367e4, 0x0803680c, "u16", 2, "Func_08019d2c", "Func_0802977c"),
      S(0x0803680c, 0x080368d4, "u16", 4, "Func_08019d2c", "Func_0802977c"),
    ],
  },
  {
    start: 0x080370d4,
    end: 0x08037464,
    segments: [
      S(0x080370d4, 0x080371b4, "u8", 1, "Func_0801de5c"),
      S(0x080371b4, 0x080371c4, "u8", 1, "Func_0801de5c"),
      S(0x080371c4, 0x080371e0, "u8", 1, "Func_0801ef68"),
      S(0x080371e0, 0x080371f6, "u8", 1, "Func_08020198"),
      S(0x080371f6, 0x080371fe, "s8", 1, "Func_08020bd8"),
      S(0x080371fe, 0x08037206, "u16", 2, "Func_08021228"),
      S(0x08037206, 0x08037216, "u16", 2, "Func_08021360"),
      S(0x08037216, 0x08037226, "u16", 2, "Func_08021360"),
      S(0x08037226, 0x08037230, "s8", 1, "Func_080216b4"),
      S(0x08037230, 0x08037250, "u32", 4, "Func_080217a4"),
      S(0x08037250, 0x08037280, "u8", 8, "Func_08021848"),
      S(0x08037280, 0x080372c0, "u8", 4, "Func_080219c8"),
      S(0x080372c0, 0x08037300, "u16", 2, "Func_08021a18"),
      S(0x08037300, 0x08037308, "ascii-fixed", 1, "Func_08021c34"),
      S(0x08037308, 0x08037328, "u8", 1, "Func_08023e70"),
      S(0x08037328, 0x080373a8, "u8", 8, "Func_08023178"),
      S(0x080373a8, 0x080373b8, "u8", 4, "Func_08023178"),
      S(0x080373b8, 0x080373d8, "u8", 4, "Func_08023178"),
      S(0x080373d8, 0x080373dc, "ascii-fixed", 1, "Func_08023178"),
      S(0x080373dc, 0x080373e0, "ascii-fixed", 1, "Func_08023178"),
      S(0x080373e0, 0x080373e4, "ascii-fixed", 1, "Func_08023178"),
      S(0x080373e4, 0x080373e7, "ascii-fixed", 1, "Func_08023178"),
      S(0x080373e7, 0x080373eb, "u8", 2, "Func_08023e70"),
      S(0x080373eb, 0x080373ef, "u8", 2, "Func_08023e70"),
      S(0x080373ef, 0x080373f7, "u8", 1, "Func_080286a0"),
      S(0x080373f7, 0x08037403, "u8", 1, "Func_08028920"),
      S(0x08037403, 0x0803740f, "u8", 1, "Func_08028920"),
      S(0x0803740f, 0x08037428, "u8", 1, "Func_080289e8"),
      S(0x08037428, 0x0803742c, "ascii-fixed", 1, "Func_08028ef0", "Func_080292c4"),
      S(0x0803742c, 0x08037440, "ascii-fixed", 1, "Func_080292c4"),
      S(0x08037440, 0x08037448, "ascii-fixed", 1, "Func_08029554"),
      S(0x08037448, 0x08037450, "ascii-fixed", 1, "Func_08029554"),
      S(0x08037450, 0x08037458, "ascii-fixed", 1, "Func_08029554"),
      S(0x08037458, 0x08037460, "ascii-fixed", 1, "Func_08029554"),
      S(0x08037460, 0x08037464, "ascii-fixed", 1, "Func_08029554"),
    ],
  },
];

function hex(value: number): string {
  return `0x${value.toString(16).padStart(8, "0")}`;
}

function integer(value: unknown, label: string): number {
  if (!Number.isSafeInteger(value)) throw new Error(`${label} must be an integer`);
  return value as number;
}

function exactKeys(value: object, keys: string[], label: string): void {
  if (Object.keys(value).sort().join(",") !== [...keys].sort().join(",")) {
    throw new Error(`${label} has unknown fields`);
  }
}

function numeric(source: NumericSegment, size: number): Buffer {
  const widths = { u8: 1, s8: 1, u16: 2, u32: 4 } as const;
  const width = widths[source.element];
  if (source.stride < width || source.stride % width !== 0 || size % source.stride !== 0 || source.values.length !== size / width)
    throw new Error("numeric table layout differs");
  const output = Buffer.alloc(size);
  source.values.forEach((raw, index) => {
    const value = integer(raw, "table value");
    if (source.element === "u8") {
      if (value < 0 || value > 0xff) throw new Error("u8 table value is outside its range");
      output[index] = value;
    } else if (source.element === "s8") {
      if (value < -0x80 || value > 0x7f) throw new Error("s8 table value is outside its range");
      output.writeInt8(value, index);
    } else if (source.element === "u16") {
      if (value < 0 || value > 0xffff) throw new Error("u16 table value is outside its range");
      output.writeUInt16LE(value, index * 2);
    } else {
      if (value < 0 || value > 0xffffffff) throw new Error("u32 table value is outside its range");
      output.writeUInt32LE(value, index * 4);
    }
  });
  return output;
}

function text(source: TextSegment, size: number): Buffer {
  if (source.stride !== 1 || !/^[\x20-\x7e]*$/.test(source.text)) throw new Error("fixed text differs");
  const encoded = Buffer.from(source.text, "ascii");
  if (encoded.length >= size) throw new Error("fixed text lacks room for its terminator");
  const output = Buffer.alloc(size);
  encoded.copy(output);
  return output;
}

function layoutFor(source: LocalizationTableSource): RegionLayout {
  const region = REGIONS.find((item) => source.address === hex(item.start));
  if (region === undefined || source.size !== hex(region.end - region.start)) throw new Error("localization-table region differs");
  return region;
}

export function build_localization_tables(value: unknown): Buffer {
  if (typeof value !== "object" || value === null) throw new Error("localization-table source must be an object");
  const source = value as LocalizationTableSource;
  exactKeys(source, ["format", "kind", "address", "size", "segments"], "localization-table source");
  if (source.format !== 1 || source.kind !== "golden-sun-localization-tables" || !Array.isArray(source.segments))
    throw new Error("localization-table source differs");
  const region = layoutFor(source);
  if (source.segments.length !== region.segments.length) throw new Error("localization-table segment count differs");
  const parts = source.segments.map((segment, index) => {
    const expected = region.segments[index];
    exactKeys(segment, segment.element === "ascii-fixed"
      ? ["address", "end", "element", "stride", "consumers", "text"]
      : ["address", "end", "element", "stride", "consumers", "values"], `localization-table segment ${index}`);
    if (segment.address !== hex(expected.start) || segment.end !== hex(expected.end) ||
        segment.element !== expected.element || segment.stride !== expected.stride ||
        JSON.stringify(segment.consumers) !== JSON.stringify(expected.consumers))
      throw new Error(`localization-table segment ${index} differs`);
    const size = expected.end - expected.start;
    return segment.element === "ascii-fixed" ? text(segment as TextSegment, size) : numeric(segment as NumericSegment, size);
  });
  const result = Buffer.concat(parts);
  if (result.length !== region.end - region.start) throw new Error("localization-table extent differs");
  return result;
}

function range(rom: Uint8Array, start: number, end: number): Buffer {
  const begin = start - ROM_BASE;
  const finish = end - ROM_BASE;
  if (begin < 0 || finish > rom.length || begin > finish) throw new Error("ROM range differs");
  return Buffer.from(rom.subarray(begin, finish));
}

function exportSegment(rom: Uint8Array, layout: SegmentLayout): TableSegment {
  const source = range(rom, layout.start, layout.end);
  const common = {
    address: hex(layout.start), end: hex(layout.end), stride: layout.stride, consumers: layout.consumers,
  };
  if (layout.element === "ascii-fixed") {
    const zero = source.indexOf(0);
    if (zero < 0 || source.subarray(zero).some((value) => value !== 0)) throw new Error("fixed text padding differs");
    const value = source.subarray(0, zero).toString("ascii");
    if (!/^[\x20-\x7e]*$/.test(value)) throw new Error("fixed text is not printable ASCII");
    return { ...common, element: "ascii-fixed", stride: 1, text: value };
  }
  const values: number[] = [];
  const data = new DataView(source.buffer, source.byteOffset, source.byteLength);
  if (layout.element === "u8") values.push(...source);
  else if (layout.element === "s8") for (let offset = 0; offset < source.length; offset++) values.push(data.getInt8(offset));
  else if (layout.element === "u16") for (let offset = 0; offset < source.length; offset += 2) values.push(data.getUint16(offset, true));
  else for (let offset = 0; offset < source.length; offset += 4) values.push(data.getUint32(offset, true));
  return { ...common, element: layout.element, values } as NumericSegment;
}

export function export_localization_tables(rom: Uint8Array): LocalizationTableSource[] {
  return REGIONS.map((region) => {
    const source: LocalizationTableSource = {
      format: 1,
      kind: "golden-sun-localization-tables",
      address: hex(region.start),
      size: hex(region.end - region.start),
      segments: region.segments.map((segment) => exportSegment(rom, segment)),
    };
    if (!build_localization_tables(source).equals(range(rom, region.start, region.end)))
      throw new Error(`exported table region ${hex(region.start)} differs`);
    return source;
  });
}

export function self_test(): void {
  for (const region of REGIONS) {
    const source: LocalizationTableSource = {
      format: 1,
      kind: "golden-sun-localization-tables",
      address: hex(region.start),
      size: hex(region.end - region.start),
      segments: region.segments.map((segment) => {
        const common = {
          address: hex(segment.start), end: hex(segment.end), stride: segment.stride, consumers: segment.consumers,
        };
        if (segment.element === "ascii-fixed")
          return { ...common, element: "ascii-fixed" as const, stride: 1 as const, text: "" };
        const width = segment.element === "u16" ? 2 : segment.element === "u32" ? 4 : 1;
        return {
          ...common, element: segment.element, values: Array((segment.end - segment.start) / width).fill(0),
        } as NumericSegment;
      }),
    };
    if (build_localization_tables(source).length !== region.end - region.start)
      throw new Error("localization-table self-test failed");
  }
  console.log("self-test=ok");
}

function option(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : undefined;
}

function sourceName(source: LocalizationTableSource): string {
  return `table_${source.address.slice(2)}.json`;
}

function main(args: string[]): void {
  if (args.includes("--self-test")) {
    self_test();
    args = args.filter((item) => item !== "--self-test");
    if (args.length === 0) return;
  }
  const command = args[0];
  if (command === "export") {
    const romPath = args[1];
    const directory = option(args, "--directory");
    if (!romPath || !directory) throw new Error("usage: localization_tables.ts export ROM --directory DIR");
    const sources = export_localization_tables(readFileSync(romPath));
    mkdirSync(directory, { recursive: true });
    sources.forEach((source) => writeFileSync(join(directory, sourceName(source)), `${JSON.stringify(source, null, 2)}\n`));
    console.log(`identical=true regions=${sources.length} bytes=${sources.reduce((sum, source) => sum + Number(source.size), 0)}`);
    return;
  }
  if (command === "build") {
    const input = args[1];
    const output = option(args, "--output");
    if (!input || !output) throw new Error("usage: localization_tables.ts build SOURCE --output FILE");
    const built = build_localization_tables(JSON.parse(readFileSync(input, "utf8")));
    mkdirSync(dirname(output), { recursive: true });
    writeFileSync(output, built);
    console.log(`bytes=${built.length}`);
    return;
  }
  if (command === "verify") {
    const romPath = args[1];
    const input = args[2];
    if (!romPath || !input) throw new Error("usage: localization_tables.ts verify ROM SOURCE");
    const source = JSON.parse(readFileSync(input, "utf8")) as LocalizationTableSource;
    const built = build_localization_tables(source);
    const start = Number(source.address);
    if (!built.equals(range(readFileSync(romPath), start, start + built.length)))
      throw new Error(`${basename(input)} differs from ROM`);
    console.log(`identical=true bytes=${built.length}`);
    return;
  }
  throw new Error("usage: localization_tables.ts {export ROM --directory DIR|build SOURCE --output FILE|verify ROM SOURCE}");
}

if (import.meta.main) main(process.argv.slice(2));
