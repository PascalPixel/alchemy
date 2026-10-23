	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout009
Field_Layout009:
	layout_header 0, 0, 100, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout009, .LField_Layout009_METATILES
	layout_offset Field_Layout009, .LField_Layout009_SHAPES
	layout_offset Field_Layout009, .LField_Layout009_GRID
	layout_offset Field_Layout009, .LField_Layout009_ANIMATION
	layout_offset Field_Layout009, .LField_Layout009_BLEND
	layout_offset Field_Layout009, .LField_Layout009_MARKERS
.LField_Layout009_METATILES:
	.incbin "games/tbs/src/layout/layout_009/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout009_SHAPES:
	.incbin "games/tbs/src/layout/layout_009/shapes.table.lz"
	.balign 4, 0
.LField_Layout009_GRID:
	.incbin "games/tbs/src/layout/layout_009/grid.parts.lz"
	.balign 4, 0
.LField_Layout009_ANIMATION:
	.incbin "games/tbs/src/layout/layout_009/animation.script.lz"
	.balign 4, 0
.LField_Layout009_BLEND:
	.incbin "games/tbs/src/layout/layout_009/blend.script.lz"
	.balign 4, 0
.LField_Layout009_MARKERS:
	.incbin "games/tbs/src/layout/layout_009/markers.table"
	.balign 4, 0
	.global Tileset_Set08Palette
Tileset_Set08Palette:
	.incbin "games/tbs/src/graphics/tileset/set_08/palette.gbapal.lz"
	.byte 0xcf, 0x83, 0x07	@ PACKER: left in the buffer by Field_Layout009+0x159
	.global Tileset_Set08TilesA
Tileset_Set08TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_08/tiles_a.4bpp.mtf"
	.global Tileset_Set08TilesB
Tileset_Set08TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_08/tiles_b.4bpp.mtf"
	.byte 0x18	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x1a4b
	.global Tileset_Set08TilesC
Tileset_Set08TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_08/tiles_c.4bpp.mtf"
	.global Tileset_Set08TilesD
Tileset_Set08TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_08/tiles_d.4bpp.mtf"
	.byte 0xe1, 0x71	@ PACKER: left in the buffer by Tileset_Set08TilesC+0x842
