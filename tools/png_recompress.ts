#!/usr/bin/env bun
// Re-canonicalize tracked PNG IDAT streams under the repository's own
// deterministic deflate (tools/zlib.ts).
//
// The 2026-08-05 linux recompress fixed the tree for the host that ran it and
// left darwin unable to reproduce a single canonical byte, because the
// node:zlib shim's output differs across Bun's platform builds. The encoder
// is now implemented in-repo and host-independent; this tool pays the
// one-time re-canonicalization and stays available so any supported host can
// re-run or audit it (`--check` writes nothing).
//
// Safety: a file is rewritten only if the new IDAT inflates back to the
// exact original scanline bytes; every non-IDAT chunk is preserved
// byte-for-byte. Multiple IDAT chunks are merged into one, which is the
// repository's canonical container shape.
import { readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { crc32, deflateSync, inflateSync } from "./zlib.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const SIGNATURE = Buffer.from([0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a]);

interface Chunk {
  type: string;
  data: Buffer;
}

export function readChunks(png: Buffer): Chunk[] {
  if (!png.subarray(0, 8).equals(SIGNATURE)) throw new Error("not a PNG");
  const chunks: Chunk[] = [];
  let offset = 8;
  while (offset < png.length) {
    const length = png.readUInt32BE(offset);
    const type = png.subarray(offset + 4, offset + 8).toString("latin1");
    chunks.push({ type, data: png.subarray(offset + 8, offset + 8 + length) });
    offset += 12 + length;
  }
  return chunks;
}

export function writeChunks(chunks: Chunk[]): Buffer {
  const parts: Buffer[] = [SIGNATURE];
  for (const { type, data } of chunks) {
    const header = Buffer.alloc(8);
    header.writeUInt32BE(data.length, 0);
    header.write(type, 4, "latin1");
    const body = Buffer.concat([Buffer.from(type, "latin1"), data]);
    const trailer = Buffer.alloc(4);
    trailer.writeUInt32BE(crc32(body), 0);
    parts.push(header, data, trailer);
  }
  return Buffer.concat(parts);
}

export function recompress(png: Buffer): { output: Buffer; changed: boolean } {
  const chunks = readChunks(png);
  const scanlines = inflateSync(Buffer.concat(
    chunks.filter((chunk) => chunk.type === "IDAT").map((chunk) => chunk.data),
  ));
  const canonical = deflateSync(scanlines);
  if (!inflateSync(canonical).equals(scanlines)) {
    // The refusal gate: never trade pixels for container canonicality.
    throw new Error("recompressed IDAT does not inflate back to the original scanlines");
  }
  const firstIdat = chunks.findIndex((chunk) => chunk.type === "IDAT");
  const rewritten = [
    ...chunks.slice(0, firstIdat),
    { type: "IDAT", data: canonical },
    ...chunks.slice(firstIdat).filter((chunk) => chunk.type !== "IDAT"),
  ];
  const output = writeChunks(rewritten);
  return { output, changed: !output.equals(png) };
}

function selfTest(): void {
  // A tiny 2x2 indexed PNG built with the repository encoder path: header,
  // palette, one IDAT, end. Round-trip must be canonical and stable.
  const scanlines = Uint8Array.from([0, 0, 1, 0, 1, 0]);
  const ihdr = Buffer.alloc(13);
  ihdr.writeUInt32BE(2, 0);
  ihdr.writeUInt32BE(2, 4);
  ihdr[8] = 8; ihdr[9] = 3;
  const png = writeChunks([
    { type: "IHDR", data: ihdr },
    { type: "PLTE", data: Buffer.from([255, 255, 255, 0, 0, 0]) },
    { type: "IDAT", data: deflateSync(scanlines) },
    { type: "IEND", data: Buffer.alloc(0) },
  ]);
  const first = recompress(png);
  if (first.changed) throw new Error("self-built canonical PNG reported as non-canonical");
  // A foreign-compressed IDAT must be rewritten and become stable.
  const foreign = writeChunks([
    { type: "IHDR", data: ihdr },
    { type: "PLTE", data: Buffer.from([255, 255, 255, 0, 0, 0]) },
    { type: "IDAT", data: Buffer.from(require("node:zlib").deflateSync(scanlines, { level: 9 })) },
    { type: "IEND", data: Buffer.alloc(0) },
  ]);
  const second = recompress(foreign);
  if (!second.changed) throw new Error("foreign-compressed IDAT was not rewritten");
  if (recompress(second.output).changed) throw new Error("recompression is not idempotent");
  if (!inflateSync(readChunks(second.output).find((c) => c.type === "IDAT")!.data).equals(Buffer.from(scanlines))) {
    throw new Error("scanlines not preserved");
  }
  console.log("self-test=ok tool=png_recompress");
}

async function main(): Promise<void> {
  if (Bun.argv.includes("--self-test")) return selfTest();
  const checkOnly = Bun.argv.includes("--check");
  const listing = Bun.spawnSync(["git", "ls-files", "*.png"], { cwd: ROOT, stdout: "pipe" });
  const files = listing.stdout.toString().trim().split("\n").filter(Boolean);
  let changed = 0;
  let canonical = 0;
  const failures: string[] = [];
  for (const file of files) {
    const path = join(ROOT, file);
    let result: { output: Buffer; changed: boolean };
    try {
      result = recompress(readFileSync(path));
    } catch (cause) {
      failures.push(`${file}: ${cause instanceof Error ? cause.message : String(cause)}`);
      continue;
    }
    if (!result.changed) { canonical++; continue; }
    changed++;
    if (!checkOnly) writeFileSync(path, result.output);
  }
  console.log(`files=${files.length} canonical=${canonical} ${checkOnly ? "non_canonical" : "rewritten"}=${changed} failed=${failures.length}`);
  for (const failure of failures) console.error(failure);
  if (failures.length > 0 || (checkOnly && changed > 0)) process.exit(1);
}

if (import.meta.main) await main();
