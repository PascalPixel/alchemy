	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout066
Field_Layout066:
	layout_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout066, .LField_Layout066_METATILES
	layout_offset Field_Layout066, .LField_Layout066_SHAPES
	layout_offset Field_Layout066, .LField_Layout066_GRID
	layout_offset Field_Layout066, .LField_Layout066_ANIMATION
	layout_offset Field_Layout066, .LField_Layout066_BLEND
	layout_offset Field_Layout066, .LField_Layout066_MARKERS
.LField_Layout066_METATILES:
	.incbin "games/tbs/src/layout/layout_066/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout066_SHAPES:
	.incbin "games/tbs/src/layout/layout_066/shapes.table.lz"
	.balign 4, 0
.LField_Layout066_GRID:
	.incbin "games/tbs/src/layout/layout_066/map.grid.lz"
	.balign 4, 0
.LField_Layout066_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout066_BLEND:
	.incbin "games/tbs/src/layout/layout_063/blend.script.lz"
	.balign 4, 0
.LField_Layout066_MARKERS:
	treasure_spot 17, 5, 100
	treasure_spots_end
	.balign 4, 0
