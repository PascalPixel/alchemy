	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout008
Field_Layout008:
	layout_header 0, 0, 30, 45, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 30, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 50, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout008, .LField_Layout008_METATILES
	layout_offset Field_Layout008, .LField_Layout008_SHAPES
	layout_offset Field_Layout008, .LField_Layout008_GRID
	layout_offset Field_Layout008
	layout_offset Field_Layout008
	layout_offset Field_Layout008, .LField_Layout008_MARKERS
.LField_Layout008_METATILES:
	.incbin "games/tbs/src/layout/layout_008/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout008_SHAPES:
	.incbin "games/tbs/src/layout/layout_008/shapes.table.lz"
	.balign 4, 0
.LField_Layout008_GRID:
	.incbin "games/tbs/src/layout/layout_008/grid.parts.lz"
	.balign 4, 0
.LField_Layout008_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Set07Palette
Tileset_Set07Palette:
	.incbin "games/tbs/src/graphics/tileset/set_07/palette.gbapal.lz"
	.byte 0x13, 0x3e	@ PACKER: left in the buffer by Field_Layout008+0xe2
	.global Tileset_Set07TilesA
Tileset_Set07TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_07/tiles_a.4bpp.mtf"
	.byte 0xf0, 0xa4, 0x22	@ PACKER: left in the buffer by Field_Layout008+0x11e9
	.global Tileset_Set07TilesB
Tileset_Set07TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_07/tiles_b.4bpp.mtf"
	.byte 0xce, 0x43, 0x48	@ PACKER: left in the buffer by Field_Layout008+0x1c85
	.global Tileset_Set07TilesC
Tileset_Set07TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_07/tiles_c.4bpp.mtf"
	.byte 0x3e	@ PACKER: left in the buffer by Tileset_Set07TilesB+0x50b
	.global Tileset_Set07TilesD
Tileset_Set07TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_07/tiles_d.4bpp.mtf"
	.byte 0x4e	@ PACKER: left in the buffer by Tileset_Set07TilesC+0x13f
