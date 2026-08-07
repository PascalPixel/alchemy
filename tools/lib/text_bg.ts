export const TILE_INDEX_MASK = 0x03ff;
export const HFLIP = 0x0400;
export const VFLIP = 0x0800;
export const PALETTE_SHIFT = 12;
export const PALETTE_MASK = 0x0f;

export type TextBgEntry = [number, number, boolean, boolean];

export function decode_entry(entry: number): TextBgEntry {
  if (!Number.isInteger(entry) || entry < 0 || entry > 0xffff)
    throw new Error("text-background entry is outside u16");
  return [entry & TILE_INDEX_MASK, (entry >>> PALETTE_SHIFT) & PALETTE_MASK,
    Boolean(entry & HFLIP), Boolean(entry & VFLIP)];
}

export function encode_entry(tile: number, palette = 0, hflip = false, vflip = false): number {
  if (!Number.isInteger(tile) || tile < 0 || tile > TILE_INDEX_MASK)
    throw new Error("text-background tile index is outside 0..1023");
  if (!Number.isInteger(palette) || palette < 0 || palette > PALETTE_MASK)
    throw new Error("text-background palette bank is outside 0..15");
  return tile | Number(hflip) * HFLIP | Number(vflip) * VFLIP | palette << PALETTE_SHIFT;
}

export function flip_tile<T>(pixels: readonly T[], hflip = false, vflip = false, size = 8): T[] {
  if (pixels.length !== size * size)
    throw new Error("tile pixel count differs from its square dimensions");
  const result: T[] = [];
  for (let y = 0; y < size; y++)
    for (let x = 0; x < size; x++)
      result.push(pixels[(vflip ? size - 1 - y : y) * size + (hflip ? size - 1 - x : x)]);
  return result;
}

export function self_test(): void {
  const entry = encode_entry(0x2a5, 13, true, true);
  if (JSON.stringify(decode_entry(entry)) !== JSON.stringify([0x2a5, 13, true, true]))
    throw new Error("text-background entry round-trip failed");
  const tile = Array.from({ length: 64 }, (_, index) => index);
  if (JSON.stringify(flip_tile(flip_tile(tile, true), true)) !== JSON.stringify(tile))
    throw new Error("horizontal tile flip is not reversible");
  if (JSON.stringify(flip_tile(flip_tile(tile, false, true), false, true)) !== JSON.stringify(tile))
    throw new Error("vertical tile flip is not reversible");
  console.log("self-test=ok");
}

if (import.meta.main) self_test();
