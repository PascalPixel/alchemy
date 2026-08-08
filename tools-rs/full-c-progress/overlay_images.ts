#!/usr/bin/env bun
// Sidecar for the one thing the native Full-C progress implementation
// cannot do natively: `assembleOverlay`.
//
// WHY: `assembleOverlay` is not an assembler wrapper. It runs the whole
// alchemy-gcc compile plan for every exact-C row in an overlay -- plan stamps,
// compiler bundle signatures, the content-addressed cache under
// `out/cache/overlay-c` -- and patches the results into the placeholder image.
// Reimplementing that in this crate would mean porting several thousand more
// lines of TypeScript that nothing else in this port needs. Instead the binary
// shells out here ONCE, with every overlay that has a C sibling, and reads the
// images back.
//
// Protocol: argv[2] is a file of newline-separated absolute `.s` paths, argv[3]
// is the output path. The output is, for each input path in order, an 8-byte
// little-endian length followed by that many image bytes. Any failure prints
// the message on stderr and exits 1.

import { readFileSync, writeFileSync } from "node:fs";
import { assembleOverlay } from "../../tools/lib/overlay_disasm.ts";

function main(argv: string[]): void {
  const [listPath, outputPath] = argv;
  if (!listPath || !outputPath) throw new Error("usage: overlay_images.ts <list> <output>");
  const sources = readFileSync(listPath, "utf8").split("\n").filter((line) => line.length > 0);
  const chunks: Buffer[] = [];
  for (const source of sources) {
    const image = assembleOverlay(source);
    const header = Buffer.alloc(8);
    header.writeBigUInt64LE(BigInt(image.length));
    chunks.push(header, image);
  }
  writeFileSync(outputPath, Buffer.concat(chunks));
}

if (import.meta.main) {
  try {
    main(Bun.argv.slice(2));
  } catch (cause) {
    console.error(cause instanceof Error ? cause.message : String(cause));
    process.exit(1);
  }
}
