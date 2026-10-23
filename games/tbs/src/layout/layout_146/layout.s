	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout146
Field_Layout146:
	layout_header 0, 0, 128, 128, 3, 3, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout146, .LField_Layout146_METATILES
	layout_offset Field_Layout146, .LField_Layout146_SHAPES
	layout_offset Field_Layout146, .LField_Layout146_GRID
	layout_offset Field_Layout146, .LField_Layout146_ANIMATION
	layout_offset Field_Layout146, .LField_Layout146_BLEND
	layout_offset Field_Layout146, .LField_Layout146_MARKERS
.LField_Layout146_METATILES:
	.incbin "games/tbs/src/layout/layout_146/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout146_SHAPES:
	.incbin "games/tbs/src/layout/layout_146/shapes.table.lz"
	.balign 4, 0
.LField_Layout146_GRID:
	.incbin "games/tbs/src/layout/layout_146/map.grid.lz"
	.balign 4, 0
.LField_Layout146_ANIMATION:
	.incbin "games/tbs/src/layout/layout_145/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout146_BLEND:
	.incbin "games/tbs/src/layout/layout_009/blend.script.lz"
	.balign 4, 0
.LField_Layout146_MARKERS:
	.incbin "games/tbs/src/layout/layout_146/treasure_spots.table"
