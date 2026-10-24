	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout084
Field_Layout084:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout084, .LField_Layout084_METATILES
	layout_offset Field_Layout084, .LField_Layout084_SHAPES
	layout_offset Field_Layout084, .LField_Layout084_GRID
	layout_offset Field_Layout084, .LField_Layout084_ANIMATION
	layout_offset Field_Layout084, .LField_Layout084_BLEND
	layout_offset Field_Layout084, .LField_Layout084_MARKERS
.LField_Layout084_METATILES:
	.incbin "games/tbs/src/layout/layout_084/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout084_SHAPES:
	.incbin "games/tbs/src/layout/layout_084/shapes.table.lz"
	.balign 4, 0
.LField_Layout084_GRID:
	.incbin "games/tbs/src/layout/layout_084/map.grid.lz"
	.balign 4, 0
.LField_Layout084_ANIMATION:
	.incbin "games/tbs/src/layout/layout_080/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout084_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout084_MARKERS:
	treasure_spot 6, 27, 100
	treasure_spots_end
	.balign 4, 0
