	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout082
Field_Layout082:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout082, .LField_Layout082_METATILES
	layout_offset Field_Layout082, .LField_Layout082_SHAPES
	layout_offset Field_Layout082, .LField_Layout082_GRID
	layout_offset Field_Layout082, .LField_Layout082_ANIMATION
	layout_offset Field_Layout082, .LField_Layout082_BLEND
	layout_offset Field_Layout082, .LField_Layout082_MARKERS
.LField_Layout082_METATILES:
	.incbin "games/tbs/src/layout/layout_082/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout082_SHAPES:
	.incbin "games/tbs/src/layout/layout_082/shapes.table.lz"
	.balign 4, 0
.LField_Layout082_GRID:
	.incbin "games/tbs/src/layout/layout_082/grid.parts.lz"
	.balign 4, 0
.LField_Layout082_ANIMATION:
	.incbin "games/tbs/src/layout/layout_078/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout082_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout082_MARKERS:
	.incbin "games/tbs/src/layout/layout_082/treasure_spots.table"
	.balign 4, 0
