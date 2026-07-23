// Tool role: library; imported by tools/build_assets.ts, tools/compose_scene.ts.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname } from "node:path";
import { canonicalJson } from "./canonical_json.ts";

export const ROM_BASE = 0x08000000;
export const TABLE_ADDRESS = 0x08013784;
export const RESOURCE_BASE = 0x128;
export const RECORDS = 186;
export const FIELDS = [
  "container", "palette", "vram_charblock1", "vram_charblock2",
  "vram_charblock3", "animation_source",
] as const;

type LoadRow = { map_index: number } & Record<(typeof FIELDS)[number], string>;
interface LoadDocument {
  format: number;
  resource_base: string;
  records: LoadRow[];
}

export function export_table(rom: Uint8Array, source: string): number {
  const start = TABLE_ADDRESS - ROM_BASE;
  const view = new DataView(rom.buffer, rom.byteOffset, rom.byteLength);
  const rows: LoadRow[] = [];
  for (let mapIndex = 0; mapIndex < RECORDS; mapIndex++) {
    const row = { map_index: mapIndex } as LoadRow;
    FIELDS.forEach((field, index) => {
      row[field] = (view.getUint16(start + mapIndex * 12 + index * 2, true) + RESOURCE_BASE).toString(16);
    });
    rows.push(row);
  }
  const document: LoadDocument = { format: 1, resource_base: `0x${RESOURCE_BASE.toString(16)}`, records: rows };
  mkdirSync(dirname(source), { recursive: true });
  writeFileSync(source, `${canonicalJson(document)}\n`);
  const rebuilt = build_table(source);
  if (!rebuilt.equals(Buffer.from(rom.subarray(start, start + RECORDS * 12))))
    throw new Error("map graphics load table does not round-trip");
  return rows.length;
}

export function build_table(source: string): Buffer {
  const document = JSON.parse(readFileSync(source, "utf8")) as LoadDocument;
  if (document.format !== 1) throw new Error("unsupported map graphics load table");
  if (Number(document.resource_base) !== RESOURCE_BASE)
    throw new Error("map graphics load table has the wrong resource base");
  if (document.records.length !== RECORDS)
    throw new Error(`map graphics load table requires ${RECORDS} records`);
  const encoded = Buffer.alloc(RECORDS * 12);
  document.records.forEach((row, mapIndex) => {
    if (Number(row.map_index ?? -1) !== mapIndex)
      throw new Error("map graphics load table indices must be sequential");
    FIELDS.forEach((field, index) => {
      const value = Number.parseInt(row[field], 16) - RESOURCE_BASE;
      if (!Number.isInteger(value) || value < 0 || value > 0xffff)
        throw new Error("map graphics resource offset is outside u16");
      encoded.writeUInt16LE(value, mapIndex * 12 + index * 2);
    });
  });
  return encoded;
}

export function records_by_container(source: string): Record<string, LoadRow[]> {
  const document = JSON.parse(readFileSync(source, "utf8")) as LoadDocument;
  const result: Record<string, LoadRow[]> = {};
  for (const row of document.records) (result[row.container.toLowerCase()] ??= []).push(row);
  return result;
}

function option(args: string[], long: string, short?: string): string | undefined {
  const index = Math.max(args.indexOf(long), short ? args.indexOf(short) : -1);
  return index >= 0 ? args[index + 1] : undefined;
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: map_load_table.ts ROM [-o FILE]");
    return;
  }
  const rom = args.find((arg, index) => !arg.startsWith("-") && (index === 0 || !args[index - 1].startsWith("-")));
  if (!rom) throw new Error("rom is required");
  const output = option(args, "--output", "-o") ?? "assets/maps/map_load_table.json";
  const rows = export_table(readFileSync(rom), output);
  console.log(`map_load_records=${rows} bytes=0x${(rows * 12).toString(16)}`);
}

if (import.meta.main) main(process.argv.slice(2));
