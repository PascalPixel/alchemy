import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname } from "node:path";

export function export_pairs(data: Uint8Array): string {
  if (data.length === 0 || data.length % 4 !== 0)
    throw new Error("pair table must contain whole nonempty u16 pairs");
  const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
  let result = "";
  for (let offset = 0; offset < data.length; offset += 4)
    result += `0x${view.getUint16(offset, true).toString(16).padStart(4, "0")} 0x${view.getUint16(offset + 2, true).toString(16).padStart(4, "0")}\n`;
  return result;
}

export function import_pairs(text: string): Buffer {
  const values: number[] = [];
  for (const [index, line] of text.split(/\r?\n/).entries()) {
    const tokens = line.split("#", 1)[0].trim().split(/\s+/).filter(Boolean);
    if (tokens.length === 0) continue;
    if (tokens.length !== 2) throw new Error(`line ${index + 1}: expected exactly two values`);
    for (const token of tokens) {
      if (!/^[-+]?(?:0[xX][0-9a-fA-F]+|0[bB][01]+|0[oO][0-7]+|[0-9]+)$/.test(token))
        throw new Error(`line ${index + 1}: invalid pair value`);
      const number = Number(token);
      if (!Number.isInteger(number)) throw new Error(`line ${index + 1}: invalid pair value`);
      if (number < 0 || number > 0xffff) throw new Error(`line ${index + 1}: pair value is out of range`);
      values.push(number);
    }
  }
  if (values.length === 0) throw new Error("pair table is empty");
  const result = Buffer.alloc(values.length * 2);
  values.forEach((number, index) => result.writeUInt16LE(number, index * 2));
  return result;
}

export function self_test(): void {
  const raw = Buffer.alloc(12);
  [0, 1, 0x1234, 0xabcd, 0xffff, 2].forEach((value, index) => raw.writeUInt16LE(value, index * 2));
  if (!import_pairs(export_pairs(raw)).equals(raw)) throw new Error("pair-table round-trip failed");
  console.log("self-test=ok");
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: pairtable.ts [--self-test] {export,build} INPUT OUTPUT");
    return;
  }
  if (args.includes("--self-test")) {
    self_test();
    args = args.filter((arg) => arg !== "--self-test");
    if (args.length === 0) return;
  }
  const [command, input, output] = args;
  if (!command || !input || !output || !["export", "build"].includes(command))
    throw new Error("a pair-table command is required");
  mkdirSync(dirname(output), { recursive: true });
  if (command === "export") writeFileSync(output, export_pairs(readFileSync(input)));
  else writeFileSync(output, import_pairs(readFileSync(input, "utf8")));
}

if (import.meta.main) main(process.argv.slice(2));
