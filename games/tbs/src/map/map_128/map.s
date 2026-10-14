	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map128
Field_Map128:
	map_header 0, 0, 166, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map128, .LField_Map128_METATILES
	map_block Field_Map128, .LField_Map128_SHAPES
	map_block Field_Map128, .LField_Map128_GRID
	map_block Field_Map128, .LField_Map128_ANIMATION
	map_block Field_Map128, .LField_Map128_BLEND
	map_block Field_Map128, .LField_Map128_MARKERS
.LField_Map128_METATILES:
	.incbin "games/tbs/src/map/map_128/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map128_SHAPES:
	.incbin "games/tbs/src/map/map_128/shapes.table.lz"
	.balign 4, 0
.LField_Map128_GRID:
	.incbin "games/tbs/src/map/map_128/grid.parts.lz"
	.balign 4, 0
.LField_Map128_ANIMATION:
	.incbin "games/tbs/src/map/map_128/animation.script.lz"
	.balign 4, 0
.LField_Map128_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map128_MARKERS:
	.incbin "games/tbs/src/map/map_128/markers.table"
	.balign 4, 0
	.global Tileset_Set61Palette
Tileset_Set61Palette:
	.incbin "games/tbs/src/graphics/tileset/set_61/palette.gbapal.lz"
	.global Tileset_Set61TilesA
Tileset_Set61TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_61/tiles_a.4bpp.mtf"
	.byte 0x91	@ PACKER: left in the buffer by Tileset_Set60TilesA+0x2907
	.global Tileset_Set61TilesB
Tileset_Set61TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_61/tiles_b.4bpp.mtf"
	.byte 0x6d	@ PACKER: left in the buffer by Tileset_Set61TilesA+0x1c4b
	.global Tileset_Set61TilesC
Tileset_Set61TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_61/tiles_c.4bpp.mtf"
	.byte 0x06, 0x21, 0x87	@ PACKER: left in the buffer by Tileset_Set61TilesB+0x1445
	.global Tileset_Set61TilesD
Tileset_Set61TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_61/tiles_d.4bpp.mtf"
	.byte 0x7f, 0x3d, 0x47	@ PACKER: left in the buffer by Tileset_Set61TilesA+0x1e7d
