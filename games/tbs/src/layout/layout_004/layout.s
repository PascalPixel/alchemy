	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout004
Field_Layout004:
	layout_header 0, 0, 60, 70, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout004, .LField_Layout004_METATILES
	layout_offset Field_Layout004, .LField_Layout004_SHAPES
	layout_offset Field_Layout004, .LField_Layout004_GRID
	layout_offset Field_Layout004
	layout_offset Field_Layout004
	layout_offset Field_Layout004, .LField_Layout004_MARKERS
.LField_Layout004_METATILES:
	.incbin "games/tbs/src/layout/layout_004/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout004_SHAPES:
	.incbin "games/tbs/src/layout/layout_004/shapes.table.lz"
	.balign 4, 0
.LField_Layout004_GRID:
	.incbin "games/tbs/src/layout/layout_004/grid.parts.lz"
	.balign 4, 0
.LField_Layout004_MARKERS:
	.incbin "games/tbs/src/layout/layout_004/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set03Palette
Tileset_Set03Palette:
	.incbin "games/tbs/src/graphics/tileset/set_03/palette.gbapal.lz"
	.byte 0xc1, 0xac, 0xce	@ PACKER: left in the buffer by Field_Layout004+0x191
	.global Tileset_Set03TilesA
Tileset_Set03TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_03/tiles_a.4bpp.mtf"
	.byte 0x1e	@ PACKER: left in the buffer by Field_Layout003+0x283f
	.global Tileset_Set03TilesB
Tileset_Set03TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_03/tiles_b.4bpp.mtf"
	.byte 0xcb, 0x42, 0xda	@ PACKER: left in the buffer by Tileset_Set03TilesA+0x1e15
	.global Tileset_Set03TilesC
Tileset_Set03TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_03/tiles_c.4bpp.mtf"
	.byte 0xeb, 0x91	@ PACKER: left in the buffer by Tileset_Set03TilesA+0x26de
	.global Tileset_Set03TilesD
Tileset_Set03TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_03/tiles_d.4bpp.mtf"
	.byte 0xe1, 0xf0, 0xf8	@ PACKER: left in the buffer by Tileset_Set03TilesC+0x1145
