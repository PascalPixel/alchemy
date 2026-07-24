// Tool role: both; imported by tools/compose_scene.ts, tools/export_asset.ts, tools/f0_archive.ts (+5 more); invoked by AGENTS.md.
//
// Cross-platform note: this deliberately uses node:zlib (classic zlib, the
// portable RFC 1950/1951 reference implementation Node.js links against)
// instead of Bun.deflateSync/Bun.inflateSync/Bun.hash. Bun's own APIs are
// backed by libdeflate, a from-scratch compressor with build-configuration-
// dependent output: it reproduced neither its own upstream source build nor
// itself across hosts. Every DEFLATE-compressed byte this repo tracks (PNG
// IDAT chunks) must be reproducible from any host running the pinned Bun
// version (see CONVENTIONS.md); classic zlib is deterministic for a given
// input, level, and strategy regardless of CPU architecture, which is why it
// is the standard choice for reproducible builds generally.
import { crc32 as nodeCrc32, deflateSync as nodeDeflateSync, inflateSync as nodeInflateSync } from "node:zlib";

interface DeflateOptions {
  level?: 6 | 9;
}

export function crc32(data: Uint8Array): number {
  return nodeCrc32(data) >>> 0;
}

export function deflateSync(data: Uint8Array, options: DeflateOptions = {}): Buffer {
  return nodeDeflateSync(data, { level: options.level ?? 6 });
}

export function inflateSync(data: Uint8Array): Buffer {
  return nodeInflateSync(data);
}
