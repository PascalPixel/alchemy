	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout060
Field_Layout060:
	layout_header 0, 0, 55, 68, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout060, .LField_Layout060_METATILES
	layout_offset Field_Layout060, .LField_Layout060_SHAPES
	layout_offset Field_Layout060, .LField_Layout060_GRID
	layout_offset Field_Layout060, .LField_Layout060_ANIMATION
	layout_offset Field_Layout060, .LField_Layout060_BLEND
	layout_offset Field_Layout060, .LField_Layout060_MARKERS
.LField_Layout060_METATILES:
	.incbin "games/tbs/src/layout/layout_060/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout060_SHAPES:
	.incbin "games/tbs/src/layout/layout_060/shapes.table.lz"
	.balign 4, 0
.LField_Layout060_GRID:
	.incbin "games/tbs/src/layout/layout_060/grid.parts.lz"
	.balign 4, 0
.LField_Layout060_ANIMATION:
	.incbin "games/tbs/src/layout/layout_028/animation.script.lz"
	.balign 4, 0
.LField_Layout060_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout060_MARKERS:
	.incbin "games/tbs/src/layout/layout_060/markers.table"
	.balign 4, 0
	.global Tileset_Set44Palette
Tileset_Set44Palette:
	.incbin "games/tbs/src/graphics/tileset/set_44/palette.gbapal.lz"
	.byte 0x32, 0xc8	@ PACKER: left in the buffer by Field_Layout060+0x1be
	.global Tileset_Set44TilesA
Tileset_Set44TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_44/tiles_a.4bpp.mtf"
