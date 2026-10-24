	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout113
Field_Layout113:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout113, .LField_Layout113_METATILES
	layout_offset Field_Layout113, .LField_Layout113_SHAPES
	layout_offset Field_Layout113, .LField_Layout113_GRID
	layout_offset Field_Layout113, .LField_Layout113_ANIMATION
	layout_offset Field_Layout113, .LField_Layout113_BLEND
	layout_offset Field_Layout113, .LField_Layout113_MARKERS
.LField_Layout113_METATILES:
	.incbin "games/tbs/src/layout/layout_113/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout113_SHAPES:
	.incbin "games/tbs/src/layout/layout_113/shapes.table.lz"
	.balign 4, 0
.LField_Layout113_GRID:
	.incbin "games/tbs/src/layout/layout_113/map.grid.lz"
	.balign 4, 0
.LField_Layout113_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout113_BLEND:
	.incbin "games/tbs/src/layout/layout_113/blend.script.lz"
	.balign 4, 0
.LField_Layout113_MARKERS:
	treasure_spot 38, 9, 102
	treasure_spot 50, 11, 101
	treasure_spot 40, 16, 103
	treasure_spot 42, 31, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette19
Tileset_Palette19:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_19.gbapal.lz"
	.byte 0xb3, 0x90, 0x80	@ PACKER: left in the buffer by Field_Layout113+0x185
