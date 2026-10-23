	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout067
Field_Layout067:
	layout_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout067, .LField_Layout067_METATILES
	layout_offset Field_Layout067, .LField_Layout067_SHAPES
	layout_offset Field_Layout067, .LField_Layout067_GRID
	layout_offset Field_Layout067, .LField_Layout067_ANIMATION
	layout_offset Field_Layout067, .LField_Layout067_BLEND
	layout_offset Field_Layout067, .LField_Layout067_MARKERS
.LField_Layout067_METATILES:
	.incbin "games/tbs/src/layout/layout_067/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout067_SHAPES:
	.incbin "games/tbs/src/layout/layout_067/shapes.table.lz"
	.balign 4, 0
.LField_Layout067_GRID:
	.incbin "games/tbs/src/layout/layout_067/grid.parts.lz"
	.balign 4, 0
.LField_Layout067_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout067_BLEND:
	.incbin "games/tbs/src/layout/layout_063/blend.script.lz"
	.balign 4, 0
.LField_Layout067_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
