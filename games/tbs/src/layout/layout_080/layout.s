	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout080
Field_Layout080:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout080, .LField_Layout080_METATILES
	layout_offset Field_Layout080, .LField_Layout080_SHAPES
	layout_offset Field_Layout080, .LField_Layout080_GRID
	layout_offset Field_Layout080, .LField_Layout080_ANIMATION
	layout_offset Field_Layout080, .LField_Layout080_BLEND
	layout_offset Field_Layout080, .LField_Layout080_MARKERS
.LField_Layout080_METATILES:
	.incbin "games/tbs/src/layout/layout_080/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout080_SHAPES:
	.incbin "games/tbs/src/layout/layout_080/shapes.table.lz"
	.balign 4, 0
.LField_Layout080_GRID:
	.incbin "games/tbs/src/layout/layout_080/map.grid.lz"
	.balign 4, 0
.LField_Layout080_ANIMATION:
	.incbin "games/tbs/src/layout/layout_080/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout080_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout080_MARKERS:
	treasure_spots_end
	.balign 4, 0
