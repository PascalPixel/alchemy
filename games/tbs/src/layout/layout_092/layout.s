	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout092
Field_Layout092:
	layout_header 0, 0, 192, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0x7f, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout092, .LField_Layout092_METATILES
	layout_offset Field_Layout092, .LField_Layout092_SHAPES
	layout_offset Field_Layout092, .LField_Layout092_GRID
	layout_offset Field_Layout092, .LField_Layout092_ANIMATION
	layout_offset Field_Layout092
	layout_offset Field_Layout092, .LField_Layout092_MARKERS
.LField_Layout092_METATILES:
	.incbin "games/tbs/src/layout/layout_092/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout092_SHAPES:
	.incbin "games/tbs/src/layout/layout_092/shapes.table.lz"
	.balign 4, 0
.LField_Layout092_GRID:
	.incbin "games/tbs/src/layout/layout_092/map.grid.lz"
	.balign 4, 0
.LField_Layout092_ANIMATION:
	.incbin "games/tbs/src/layout/layout_091/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout092_MARKERS:
	treasure_spot 10, 15, 107
	treasure_spots_end
	.balign 4, 0
