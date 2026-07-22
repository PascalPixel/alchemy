#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.
import { readFileSync } from "fs";

export const BYTE_HENKAN_ADDRESS = 0x080092b8;
export const BYTE_HENKAN_END = 0x08009bb8;
export const BYTE_HENKAN_SIZE = BYTE_HENKAN_END - BYTE_HENKAN_ADDRESS;

type JsonObject = Record<string, unknown>;

interface TableSource {
  name: string;
  kind: "permutation" | "mapping";
  rows: number[][];
}

interface SourceDocument {
  format: 1;
  kind: "golden-sun-byte-henkan-tables";
  address: string;
  end: string;
  table_width: 16;
  source_rows: 14;
  reserved_rows: 2;
  tables: TableSource[];
}

function exactKeys(value: unknown, keys: readonly string[], label: string): asserts value is JsonObject {
  if (typeof value !== "object" || value === null || Array.isArray(value)) {
    throw new Error(`${label} must be an object`);
  }
  const actual = Object.keys(value).sort();
  const expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${label} has unexpected fields`);
  }
}

function integer(value: unknown, minimum: number, maximum: number, label: string): number {
  if (typeof value !== "number" || !Number.isSafeInteger(value) || value < minimum || value > maximum) {
    throw new Error(`${label} is outside its range`);
  }
  return value;
}

function parseDocument(value: unknown): SourceDocument {
  exactKeys(value, [
    "format", "kind", "address", "end", "table_width", "source_rows", "reserved_rows", "tables",
  ], "byte conversion source");
  if (value.format !== 1 || value.kind !== "golden-sun-byte-henkan-tables" ||
      value.address !== "0x080092b8" || value.end !== "0x08009bb8" ||
      value.table_width !== 16 || value.source_rows !== 14 || value.reserved_rows !== 2 ||
      !Array.isArray(value.tables) || value.tables.length !== 9) {
    throw new Error("byte conversion layout differs");
  }
  const tables = value.tables.map((item, tableIndex) => {
    exactKeys(item, ["name", "kind", "rows"], `byte conversion table ${tableIndex}`);
    const expectedName = `hyou_${String(tableIndex).padStart(2, "0")}`;
    const expectedKind = tableIndex < 5 ? "permutation" : "mapping";
    if (item.name !== expectedName || item.kind !== expectedKind ||
        !Array.isArray(item.rows) || item.rows.length !== 14) {
      throw new Error(`byte conversion table ${tableIndex} differs`);
    }
    const rows = item.rows.map((row, rowIndex) => {
      if (!Array.isArray(row) || row.length !== 16) {
        throw new Error(`byte conversion table ${tableIndex} row ${rowIndex} differs`);
      }
      return row.map((entry, columnIndex) => integer(
        entry, 0, 223, `byte conversion table ${tableIndex} row ${rowIndex} column ${columnIndex}`,
      ));
    });
    const values = rows.flat();
    if (expectedKind === "permutation") {
      const ordered = [...values].sort((left, right) => left - right);
      if (ordered.some((entry, index) => entry !== index)) {
        throw new Error(`byte conversion table ${tableIndex} is not a permutation`);
      }
    }
    return { name: expectedName, kind: expectedKind, rows } as TableSource;
  });
  return { ...value, tables } as SourceDocument;
}

export function build_byte_henkan_tables(path: string): Buffer {
  const text = readFileSync(path, "utf8");
  if (text.includes("\r") || !text.endsWith("\n")) throw new Error("byte conversion source is not canonical text");
  const source = parseDocument(JSON.parse(text));
  const parts: Buffer[] = [];
  for (const table of source.tables) {
    parts.push(Buffer.from(table.rows.flat()));
    parts.push(Buffer.alloc(source.reserved_rows * source.table_width));
  }
  const result = Buffer.concat(parts);
  if (result.length !== BYTE_HENKAN_SIZE) throw new Error("byte conversion output size differs");
  return result;
}

function testDocument(): SourceDocument {
  const permutation = Array.from({ length: 14 }, (_, row) =>
    Array.from({ length: 16 }, (_, column) => row * 16 + column));
  const mapping = Array.from({ length: 14 }, () => Array(16).fill(7));
  return {
    format: 1,
    kind: "golden-sun-byte-henkan-tables",
    address: "0x080092b8",
    end: "0x08009bb8",
    table_width: 16,
    source_rows: 14,
    reserved_rows: 2,
    tables: Array.from({ length: 9 }, (_, index) => ({
      name: `hyou_${String(index).padStart(2, "0")}`,
      kind: index < 5 ? "permutation" : "mapping",
      rows: (index < 5 ? permutation : mapping).map((row) => [...row]),
    })) as TableSource[],
  };
}

export function selfTest(): void {
  const source = testDocument();
  const parsed = parseDocument(source);
  const parts = parsed.tables.flatMap((table) => [Buffer.from(table.rows.flat()), Buffer.alloc(32)]);
  const result = Buffer.concat(parts);
  if (result.length !== BYTE_HENKAN_SIZE || result[223] !== 223 || result[224] !== 0 ||
      result[5 * 256] !== 7 || result.at(-1) !== 0) {
    throw new Error("byte conversion build self-test failed");
  }
  const duplicate = testDocument();
  duplicate.tables[0].rows[0][0] = 1;
  let rejected = false;
  try {
    parseDocument(duplicate);
  } catch {
    rejected = true;
  }
  if (!rejected) throw new Error("invalid byte conversion permutation was accepted");
  console.log("self-test=ok");
}

if (import.meta.main) {
  if (Bun.argv.includes("--self-test")) selfTest();
  else {
    const source = Bun.argv[2] ?? "assets/data/byte_henkan/hyou.json";
    console.log(`bytes=${build_byte_henkan_tables(source).length}`);
  }
}
