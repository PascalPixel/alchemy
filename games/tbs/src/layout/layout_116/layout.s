	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout116
Field_Layout116:
	layout_header 0, 0, 34, 40, 3, 2, 1, 1, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout116, .LField_Layout116_METATILES
	layout_offset Field_Layout116, .LField_Layout116_SHAPES
	layout_offset Field_Layout116, .LField_Layout116_GRID
	layout_offset Field_Layout116, .LField_Layout116_ANIMATION
	layout_offset Field_Layout116
	layout_offset Field_Layout116, .LField_Layout116_MARKERS
.LField_Layout116_METATILES:
	.incbin "games/tbs/src/layout/layout_116/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout116_SHAPES:
	.incbin "games/tbs/src/layout/layout_116/shapes.table.lz"
	.balign 4, 0
.LField_Layout116_GRID:
	.incbin "games/tbs/src/layout/layout_116/grid.parts.lz"
	.balign 4, 0
.LField_Layout116_ANIMATION:
	.incbin "games/tbs/src/layout/layout_101/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout116_MARKERS:
	.incbin "games/tbs/src/layout/layout_101/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette22
Tileset_Palette22:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_22.gbapal.lz"
	.byte 0x50, 0x42, 0x7d	@ PACKER: left in the buffer by Field_Layout116+0x171
