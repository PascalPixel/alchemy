	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout007
Field_Layout007:
	layout_header 0, 0, 48, 33, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout007, .LField_Layout007_METATILES
	layout_offset Field_Layout007, .LField_Layout007_SHAPES
	layout_offset Field_Layout007, .LField_Layout007_GRID
	layout_offset Field_Layout007, .LField_Layout007_ANIMATION
	layout_offset Field_Layout007, .LField_Layout007_BLEND
	layout_offset Field_Layout007, .LField_Layout007_MARKERS
.LField_Layout007_METATILES:
	.incbin "games/tbs/src/layout/layout_007/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout007_SHAPES:
	.incbin "games/tbs/src/layout/layout_007/shapes.table.lz"
	.balign 4, 0
.LField_Layout007_GRID:
	.incbin "games/tbs/src/layout/layout_007/grid.parts.lz"
	.balign 4, 0
.LField_Layout007_ANIMATION:
	.incbin "games/tbs/src/layout/layout_007/animation.script.lz"
	.balign 4, 0
.LField_Layout007_BLEND:
	.incbin "games/tbs/src/layout/layout_005/blend.script.lz"
	.balign 4, 0
.LField_Layout007_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Set06Palette
Tileset_Set06Palette:
	.incbin "games/tbs/src/graphics/tileset/set_06/palette.gbapal.lz"
	.byte 0x88, 0xd7, 0x3c	@ PACKER: left in the buffer by Field_Layout007+0xfd
	.global Tileset_Set06TilesA
Tileset_Set06TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_06/tiles_a.4bpp.mtf"
	.byte 0x8f, 0x85	@ PACKER: left in the buffer by Tileset_Set05TilesD+0x13e2
	.global Tileset_Set06TilesB
Tileset_Set06TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_06/tiles_b.4bpp.mtf"
	.byte 0xe4, 0x05, 0x41	@ PACKER: left in the buffer by Tileset_Set06TilesA+0x1181
	.global Tileset_Set06TilesC
Tileset_Set06TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_06/tiles_c.4bpp.mtf"
	.byte 0x50, 0xa3	@ PACKER: left in the buffer by Tileset_Set06TilesB+0x986
	.global Tileset_Set06TilesD
Tileset_Set06TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_06/tiles_d.4bpp.mtf"
