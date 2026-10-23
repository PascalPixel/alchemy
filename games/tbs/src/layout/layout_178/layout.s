	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout178
Field_Layout178:
	layout_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout178, .LField_Layout178_METATILES
	layout_offset Field_Layout178, .LField_Layout178_SHAPES
	layout_offset Field_Layout178, .LField_Layout178_GRID
	layout_offset Field_Layout178
	layout_offset Field_Layout178
	layout_offset Field_Layout178, .LField_Layout178_MARKERS
.LField_Layout178_METATILES:
	.incbin "games/tbs/src/layout/layout_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout178_SHAPES:
	.incbin "games/tbs/src/layout/layout_001/shapes.table.lz"
	.balign 4, 0
.LField_Layout178_GRID:
	.incbin "games/tbs/src/layout/layout_001/grid.parts.lz"
	.balign 4, 0
.LField_Layout178_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set74Palette
Tileset_Set74Palette:
	.incbin "games/tbs/src/graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Layout178+0xb
	.global Tileset_Set74TilesA
Tileset_Set74TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Layout178+0x14e
	.global Tileset_Set74TilesB
Tileset_Set74TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set74TilesA+0x13f
	.global Tileset_Set74TilesC
Tileset_Set74TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set74TilesA+0x13f
	.global Tileset_Set74TilesD
Tileset_Set74TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set74TilesA+0x13f
