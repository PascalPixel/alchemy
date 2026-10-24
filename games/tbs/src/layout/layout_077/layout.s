	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout077
Field_Layout077:
	layout_header 0, 0, 64, 68, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout077, .LField_Layout077_METATILES
	layout_offset Field_Layout077, .LField_Layout077_SHAPES
	layout_offset Field_Layout077, .LField_Layout077_GRID
	layout_offset Field_Layout077, .LField_Layout077_ANIMATION
	layout_offset Field_Layout077, .LField_Layout077_BLEND
	layout_offset Field_Layout077, .LField_Layout077_MARKERS
.LField_Layout077_METATILES:
	.incbin "games/tbs/src/layout/layout_077/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout077_SHAPES:
	.incbin "games/tbs/src/layout/layout_077/shapes.table.lz"
	.balign 4, 0
.LField_Layout077_GRID:
	.incbin "games/tbs/src/layout/layout_077/map.grid.lz"
	.balign 4, 0
.LField_Layout077_ANIMATION:
	.incbin "games/tbs/src/layout/layout_077/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout077_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout077_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette05
Tileset_Palette05:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_05.gbapal.lz"
	.byte 0x5d, 0x02, 0x06	@ PACKER: left in the buffer by Field_Layout077+0x1b1
