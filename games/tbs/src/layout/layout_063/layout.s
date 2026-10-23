	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout063
Field_Layout063:
	layout_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout063, .LField_Layout063_METATILES
	layout_offset Field_Layout063, .LField_Layout063_SHAPES
	layout_offset Field_Layout063, .LField_Layout063_GRID
	layout_offset Field_Layout063, .LField_Layout063_ANIMATION
	layout_offset Field_Layout063, .LField_Layout063_BLEND
	layout_offset Field_Layout063, .LField_Layout063_MARKERS
.LField_Layout063_METATILES:
	.incbin "games/tbs/src/layout/layout_063/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout063_SHAPES:
	.incbin "games/tbs/src/layout/layout_063/shapes.table.lz"
	.balign 4, 0
.LField_Layout063_GRID:
	.incbin "games/tbs/src/layout/layout_063/grid.parts.lz"
	.balign 4, 0
.LField_Layout063_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout063_BLEND:
	.incbin "games/tbs/src/layout/layout_063/blend.script.lz"
	.balign 4, 0
.LField_Layout063_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
