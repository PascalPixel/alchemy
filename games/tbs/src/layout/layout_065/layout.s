	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout065
Field_Layout065:
	layout_header 0, 0, 72, 106, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 106, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout065, .LField_Layout065_METATILES
	layout_offset Field_Layout065, .LField_Layout065_SHAPES
	layout_offset Field_Layout065, .LField_Layout065_GRID
	layout_offset Field_Layout065, .LField_Layout065_ANIMATION
	layout_offset Field_Layout065, .LField_Layout065_BLEND
	layout_offset Field_Layout065, .LField_Layout065_MARKERS
.LField_Layout065_METATILES:
	.incbin "games/tbs/src/layout/layout_065/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout065_SHAPES:
	.incbin "games/tbs/src/layout/layout_062/shapes.table.lz"
	.balign 4, 0
.LField_Layout065_GRID:
	.incbin "games/tbs/src/layout/layout_065/map.grid.lz"
	.balign 4, 0
.LField_Layout065_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout065_BLEND:
	.incbin "games/tbs/src/layout/layout_063/blend.script.lz"
	.balign 4, 0
.LField_Layout065_MARKERS:
	treasure_spot 11, 8, 100
	treasure_spots_end
	.balign 4, 0
