	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout088
Field_Layout088:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout088, .LField_Layout088_METATILES
	layout_offset Field_Layout088, .LField_Layout088_SHAPES
	layout_offset Field_Layout088, .LField_Layout088_GRID
	layout_offset Field_Layout088, .LField_Layout088_ANIMATION
	layout_offset Field_Layout088, .LField_Layout088_BLEND
	layout_offset Field_Layout088, .LField_Layout088_MARKERS
.LField_Layout088_METATILES:
	.incbin "games/tbs/src/layout/layout_088/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout088_SHAPES:
	.incbin "games/tbs/src/layout/layout_088/shapes.table.lz"
	.balign 4, 0
.LField_Layout088_GRID:
	.incbin "games/tbs/src/layout/layout_088/map.grid.lz"
	.balign 4, 0
.LField_Layout088_ANIMATION:
	.incbin "games/tbs/src/layout/layout_080/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout088_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout088_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette07
Tileset_Palette07:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_07.gbapal.lz"
	.byte 0x14, 0x26	@ PACKER: left in the buffer by Field_Layout088+0x162
