	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout055
Field_Layout055:
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout055, .LField_Layout055_METATILES
	layout_offset Field_Layout055, .LField_Layout055_SHAPES
	layout_offset Field_Layout055, .LField_Layout055_GRID
	layout_offset Field_Layout055, .LField_Layout055_ANIMATION
	layout_offset Field_Layout055, .LField_Layout055_BLEND
	layout_offset Field_Layout055, .LField_Layout055_MARKERS
.LField_Layout055_METATILES:
	.incbin "games/tbs/src/layout/layout_055/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout055_SHAPES:
	.incbin "games/tbs/src/layout/layout_055/shapes.table.lz"
	.balign 4, 0
.LField_Layout055_GRID:
	.incbin "games/tbs/src/layout/layout_055/map.grid.lz"
	.balign 4, 0
.LField_Layout055_ANIMATION:
	.incbin "games/tbs/src/layout/layout_054/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout055_BLEND:
	.incbin "games/tbs/src/layout/layout_054/blend.script.lz"
	.balign 4, 0
.LField_Layout055_MARKERS:
	.incbin "games/tbs/src/layout/layout_055/treasure_spots.table"
	.balign 4, 0
