	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout078
Field_Layout078:
	layout_header 0, 0, 64, 64, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout078, .LField_Layout078_METATILES
	layout_offset Field_Layout078, .LField_Layout078_SHAPES
	layout_offset Field_Layout078, .LField_Layout078_GRID
	layout_offset Field_Layout078, .LField_Layout078_ANIMATION
	layout_offset Field_Layout078, .LField_Layout078_BLEND
	layout_offset Field_Layout078, .LField_Layout078_MARKERS
.LField_Layout078_METATILES:
	.incbin "games/tbs/src/layout/layout_078/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout078_SHAPES:
	.incbin "games/tbs/src/layout/layout_078/shapes.table.lz"
	.balign 4, 0
.LField_Layout078_GRID:
	.incbin "games/tbs/src/layout/layout_078/map.grid.lz"
	.balign 4, 0
.LField_Layout078_ANIMATION:
	.incbin "games/tbs/src/layout/layout_078/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout078_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout078_MARKERS:
	treasure_spots_end
	.balign 4, 0
