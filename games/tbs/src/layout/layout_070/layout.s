	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout070
Field_Layout070:
	layout_header 0, 0, 106, 80, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout070, .LField_Layout070_METATILES
	layout_offset Field_Layout070, .LField_Layout070_SHAPES
	layout_offset Field_Layout070, .LField_Layout070_GRID
	layout_offset Field_Layout070, .LField_Layout070_ANIMATION
	layout_offset Field_Layout070
	layout_offset Field_Layout070, .LField_Layout070_MARKERS
.LField_Layout070_METATILES:
	.incbin "games/tbs/src/layout/layout_070/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout070_SHAPES:
	.incbin "games/tbs/src/layout/layout_070/shapes.table.lz"
	.balign 4, 0
.LField_Layout070_GRID:
	.incbin "games/tbs/src/layout/layout_070/map.grid.lz"
	.balign 4, 0
.LField_Layout070_ANIMATION:
	.incbin "games/tbs/src/layout/layout_068/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout070_MARKERS:
	.incbin "games/tbs/src/layout/layout_070/treasure_spots.table"
	.balign 4, 0
