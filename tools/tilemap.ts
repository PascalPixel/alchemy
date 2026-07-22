// Tool role: both; imported by tools/build_assets.ts, tools/chiiki_map_resources.ts, tools/map_container_components.ts (+1 more); invoked by package.json.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname } from "node:path";

const WORD = /^[0-9a-fA-F]{4}$/;

export function export_tilemap(data: Uint8Array, width: number): string {
  if (width <= 0 || data.length % 2 !== 0)
    throw new Error("tilemap must contain whole 16-bit entries");
  const count = data.length / 2;
  if (count === 0 || count % width !== 0)
    throw new Error("width must divide the tilemap entry count");
  const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
  let result = "";
  for (let start = 0; start < count; start += width) {
    const row: string[] = [];
    for (let index = start; index < start + width; index++)
      row.push(view.getUint16(index * 2, true).toString(16).padStart(4, "0"));
    result += `${row.join(" ")}\n`;
  }
  return result;
}

export function import_tilemap(text: string): Buffer {
  const tokens = text.split(/\s+/).filter(Boolean);
  if (tokens.length === 0 || tokens.some((token) => !WORD.test(token)))
    throw new Error("tilemap text must contain four-digit hex words");
  const result = Buffer.alloc(tokens.length * 2);
  tokens.forEach((token, index) => result.writeUInt16LE(Number.parseInt(token, 16), index * 2));
  return result;
}

export function self_test(): void {
  const raw = Buffer.alloc(128);
  for (let index = 0; index < 64; index++) raw.writeUInt16LE(index * 257, index * 2);
  if (!import_tilemap(export_tilemap(raw, 8)).equals(raw))
    throw new Error("tilemap round-trip failed");
  console.log("self-test=ok");
}

function value(args: string[], name: string): string {
  const index = args.indexOf(name);
  if (index < 0 || index + 1 >= args.length) throw new Error(`${name} is required`);
  return args[index + 1];
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: tilemap.ts [--self-test] {export,import} ...");
    return;
  }
  if (args.includes("--self-test")) {
    self_test();
    args = args.filter((arg) => arg !== "--self-test");
    if (args.length === 0) return;
  }
  const command = args[0];
  const input = args[1];
  const output = value(args, args.includes("-o") ? "-o" : "--output");
  mkdirSync(dirname(output), { recursive: true });
  if (command === "export") {
    const result = export_tilemap(readFileSync(input), Number.parseInt(value(args, "--width"), 10));
    writeFileSync(output, result);
    console.log(`entries=${result.trim().split(/\s+/).length}`);
  } else if (command === "import") {
    const result = import_tilemap(readFileSync(input, "utf8"));
    writeFileSync(output, result);
    console.log(`bytes=${result.length}`);
  } else throw new Error("a tilemap command is required");
}

if (import.meta.main) main(process.argv.slice(2));
