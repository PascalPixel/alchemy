	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout167
Field_Layout167:
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout167, .LField_Layout167_METATILES
	layout_offset Field_Layout167, .LField_Layout167_SHAPES
	layout_offset Field_Layout167, .LField_Layout167_GRID
	layout_offset Field_Layout167, .LField_Layout167_ANIMATION
	layout_offset Field_Layout167, .LField_Layout167_BLEND
	layout_offset Field_Layout167, .LField_Layout167_MARKERS
.LField_Layout167_METATILES:
	.incbin "games/tbs/src/layout/layout_167/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout167_SHAPES:
	.incbin "games/tbs/src/layout/layout_167/shapes.table.lz"
	.balign 4, 0
.LField_Layout167_GRID:
	.incbin "games/tbs/src/layout/layout_167/map.grid.lz"
	.balign 4, 0
.LField_Layout167_ANIMATION:
	.incbin "games/tbs/src/layout/layout_164/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout167_BLEND:
	.incbin "games/tbs/src/layout/layout_164/blend.script.lz"
	.balign 4, 0
.LField_Layout167_MARKERS:
	.incbin "games/tbs/src/layout/layout_167/treasure_spots.table"
	.balign 4, 0
