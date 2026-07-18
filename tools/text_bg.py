#!/usr/bin/env python3
"""GBA regular/text-background tile-entry fields and flip transforms."""


TILE_INDEX_MASK = 0x03FF
HFLIP = 0x0400
VFLIP = 0x0800
PALETTE_SHIFT = 12
PALETTE_MASK = 0x0F


def decode_entry(entry):
    """Return ``(tile, palette, hflip, vflip)`` for one u16 entry."""
    if not 0 <= entry <= 0xFFFF:
        raise ValueError("text-background entry is outside u16")
    return (
        entry & TILE_INDEX_MASK,
        entry >> PALETTE_SHIFT & PALETTE_MASK,
        bool(entry & HFLIP),
        bool(entry & VFLIP),
    )


def encode_entry(tile, palette=0, hflip=False, vflip=False):
    """Pack one regular/text-background entry."""
    if not 0 <= tile <= TILE_INDEX_MASK:
        raise ValueError("text-background tile index is outside 0..1023")
    if not 0 <= palette <= PALETTE_MASK:
        raise ValueError("text-background palette bank is outside 0..15")
    return (tile | int(bool(hflip)) * HFLIP | int(bool(vflip)) * VFLIP |
            palette << PALETTE_SHIFT)


def flip_tile(pixels, hflip=False, vflip=False, size=8):
    """Apply an entry's display flips to one flat square tile.

    The transform is its own inverse, so the same helper also recovers the
    canonical stored tile from a displayed object tile.
    """
    if len(pixels) != size * size:
        raise ValueError("tile pixel count differs from its square dimensions")
    return [
        pixels[(size - 1 - y if vflip else y) * size +
               (size - 1 - x if hflip else x)]
        for y in range(size) for x in range(size)
    ]


def self_test():
    entry = encode_entry(0x2A5, 13, True, True)
    if decode_entry(entry) != (0x2A5, 13, True, True):
        raise AssertionError("text-background entry round-trip failed")
    tile = list(range(64))
    if flip_tile(flip_tile(tile, True, False), True, False) != tile:
        raise AssertionError("horizontal tile flip is not reversible")
    if flip_tile(flip_tile(tile, False, True), False, True) != tile:
        raise AssertionError("vertical tile flip is not reversible")
    print("self-test=ok")


if __name__ == "__main__":
    self_test()
