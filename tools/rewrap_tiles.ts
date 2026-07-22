#!/usr/bin/env bun
// Tool role: entrypoint; invoked by assets/README.md.
// 連番タイル銀行PNGの折返し列数を32列へ正規化する。タイル順序と
// パレットは不変で、見た目の折返しだけを著作時の32x16へ直す。
// 取込み側は行優先でタイルを読むため、符号化結果は変わらない。
import { readFileSync, writeFileSync } from "node:fs";
import { gba_graphics, indexed_png } from "./import_asset.ts";
import { tile_png } from "./export_asset.ts";

const COLUMNS = 32;

function main(paths: string[]): void {
  if (paths.length === 0) throw new Error("usage: rewrap_tiles.ts PNG...");
  for (const path of paths) {
    const data = readFileSync(path);
    const [, , , palette] = indexed_png(data);
    const [tiles, , report] = gba_graphics(data, 4);
    if (report.tiles % COLUMNS !== 0) {
      console.log(`skip ${path}: ${report.tiles} tiles do not fill ${COLUMNS} columns`);
      continue;
    }
    if (report.width === COLUMNS * 8) {
      console.log(`ok ${path}: already ${COLUMNS} columns`);
      continue;
    }
    const [rewrapped] = tile_png(tiles, 4, COLUMNS, palette);
    const [verifyTiles] = gba_graphics(rewrapped, 4);
    if (!verifyTiles.equals(tiles)) throw new Error(`${path}: rewrap altered tile data`);
    writeFileSync(path, rewrapped);
    console.log(`rewrapped ${path}: ${report.width}x${report.height} -> ${COLUMNS * 8}x${(report.tiles / COLUMNS) * 8}`);
  }
}

main(Bun.argv.slice(2));
