// Build an OpenType font from the localization glyph records in the ROM.
// The 224 records at FONT_ADDRESS (see tools/localization_font.ts) each carry
// a u16 advance and 15 u16 bitmap rows for codepoints 0x20-0xFF; every set
// pixel becomes a 100-unit square (unitsPerEm 1600), so rendering at any
// multiple of 16 px reproduces the ROM pixels exactly.
// Run: bun tools/font_to_otf.ts [--out assets/fonts/weyard.otf]
import { readFileSync, writeFileSync, mkdirSync } from "node:fs";
import { join, dirname } from "node:path";
import opentype from "opentype.js";

const ROOT = join(dirname(Bun.fileURLToPath(import.meta.url)), "..");
const FONT_ADDRESS = 0x08032224;
const ROM_BASE = 0x08000000;
const GLYPHS = 224;
const FIRST_CODE = 0x20;
const PX = 100;                      // font units per ROM pixel
const UNITS_PER_EM = 16 * PX;
const BASELINE_ROW = 12;             // rows 0-11 above baseline, 12-14 below

function glyphPath(rows: number[], baselineRow: number): opentype.Path {
  const path = new opentype.Path();
  for (let y = 0; y < rows.length; y++) {
    const row = rows[y];
    let x = 0;
    while (x < 16) {
      if (!(row >>> (15 - x) & 1)) { x++; continue; }
      let end = x;
      while (end < 16 && (row >>> (15 - end) & 1)) end++;
      const top = (baselineRow - y) * PX;
      const bottom = top - PX;
      path.moveTo(x * PX, bottom);
      path.lineTo(end * PX, bottom);
      path.lineTo(end * PX, top);
      path.lineTo(x * PX, top);
      path.close();
      x = end;
    }
  }
  return path;
}

function main(args: string[]): void {
  const outFlag = args.indexOf("--out");
  const outPath = outFlag >= 0 ? args[outFlag + 1] : join(ROOT, "assets", "fonts", "weyard.otf");
  const rom = readFileSync(join(ROOT, "baserom.gba"));
  const base = FONT_ADDRESS - ROM_BASE;
  const glyphs: opentype.Glyph[] = [
    new opentype.Glyph({ name: ".notdef", advanceWidth: 8 * PX, path: new opentype.Path() }),
  ];
  for (let index = 0; index < GLYPHS; index++) {
    const record = base + index * 32;
    const advance = rom.readUInt16LE(record);
    const rows: number[] = [];
    for (let y = 0; y < 15; y++) rows.push(rom.readUInt16LE(record + 2 + y * 2));
    const code = FIRST_CODE + index;
    glyphs.push(new opentype.Glyph({
      name: `uni${code.toString(16).toUpperCase().padStart(4, "0")}`,
      unicode: code,
      advanceWidth: Math.max(advance, 1) * PX,
      path: glyphPath(rows, BASELINE_ROW),
    }));
  }
  const font = new opentype.Font({
    familyName: "Weyard",
    styleName: "Regular",
    unitsPerEm: UNITS_PER_EM,
    ascender: BASELINE_ROW * PX,
    descender: (BASELINE_ROW - 15) * PX,
    glyphs,
  });
  mkdirSync(dirname(outPath), { recursive: true });
  writeFileSync(outPath, Buffer.from(font.toArrayBuffer()));
  console.log(`wrote ${outPath}: ${GLYPHS} glyphs, em ${UNITS_PER_EM}, baseline row ${BASELINE_ROW}`);
}

main(Bun.argv.slice(2));
