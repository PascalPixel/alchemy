// Tool role: library; imported by tools/build_assets.ts.
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname } from "node:path";

export function export_words(data: Uint8Array): string {
  if (data.length % 2 !== 0) throw new Error("word stream has an odd byte count");
  const view = new DataView(data.buffer, data.byteOffset, data.byteLength);
  let result = "";
  for (let offset = 0; offset < data.length; offset += 2)
    result += `0x${view.getUint16(offset, true).toString(16).padStart(4, "0")}\n`;
  return result;
}

export function import_words(text: string): Buffer {
  const words: number[] = [];
  for (const [index, line] of text.split(/\r?\n/).entries()) {
    const value = line.split("#", 1)[0].trim();
    if (!value) continue;
    const word = Number(value);
    if (!/^[-+]?(?:0[xX][0-9a-fA-F]+|0[bB][01]+|0[oO][0-7]+|[0-9]+)$/.test(value) || !Number.isInteger(word))
      throw new Error(`line ${index + 1}: invalid word`);
    if (word < 0 || word > 0xffff) throw new Error(`line ${index + 1}: word is out of range`);
    words.push(word);
  }
  const result = Buffer.alloc(words.length * 2);
  words.forEach((word, index) => result.writeUInt16LE(word, index * 2));
  return result;
}

function main(args: string[]): void {
  if (args.includes("-h") || args.includes("--help")) {
    console.log("usage: wordstream.ts {export,build} INPUT OUTPUT");
    return;
  }
  const [command, input, output] = args;
  if (!command || !input || !output || !["export", "build"].includes(command))
    throw new Error("a word-stream command is required");
  mkdirSync(dirname(output), { recursive: true });
  if (command === "export") writeFileSync(output, export_words(readFileSync(input)));
  else writeFileSync(output, import_words(readFileSync(input, "utf8")));
}

if (import.meta.main) main(process.argv.slice(2));
