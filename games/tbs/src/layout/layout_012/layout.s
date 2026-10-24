	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout012
Field_Layout012:
	layout_header 0, 0, 104, 80, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 104, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout012, .LField_Layout012_METATILES
	layout_offset Field_Layout012, .LField_Layout012_SHAPES
	layout_offset Field_Layout012, .LField_Layout012_GRID
	layout_offset Field_Layout012, .LField_Layout012_ANIMATION
	layout_offset Field_Layout012, .LField_Layout012_BLEND
	layout_offset Field_Layout012, .LField_Layout012_MARKERS
.LField_Layout012_METATILES:
	.incbin "games/tbs/src/layout/layout_012/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout012_SHAPES:
	.incbin "games/tbs/src/layout/layout_001/shapes.table.lz"
	.balign 4, 0
.LField_Layout012_GRID:
	.incbin "games/tbs/src/layout/layout_012/map.grid.lz"
	.balign 4, 0
.LField_Layout012_ANIMATION:
	.incbin "games/tbs/src/layout/layout_012/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout012_BLEND:
	.incbin "games/tbs/src/layout/layout_012/blend.script.lz"
	.balign 4, 0
.LField_Layout012_MARKERS:
	treasure_spot 15, 10, 100
	treasure_spots_end
	.balign 4, 0
