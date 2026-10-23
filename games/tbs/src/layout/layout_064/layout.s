	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout064
Field_Layout064:
	layout_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout064, .LField_Layout064_METATILES
	layout_offset Field_Layout064, .LField_Layout064_SHAPES
	layout_offset Field_Layout064, .LField_Layout064_GRID
	layout_offset Field_Layout064, .LField_Layout064_ANIMATION
	layout_offset Field_Layout064, .LField_Layout064_BLEND
	layout_offset Field_Layout064, .LField_Layout064_MARKERS
.LField_Layout064_METATILES:
	.incbin "games/tbs/src/layout/layout_064/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout064_SHAPES:
	.incbin "games/tbs/src/layout/layout_064/shapes.table.lz"
	.balign 4, 0
.LField_Layout064_GRID:
	.incbin "games/tbs/src/layout/layout_064/map.grid.lz"
	.balign 4, 0
.LField_Layout064_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout064_BLEND:
	.incbin "games/tbs/src/layout/layout_063/blend.script.lz"
	.balign 4, 0
.LField_Layout064_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
