	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout153
Field_Layout153:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout153, .LField_Layout153_METATILES
	layout_offset Field_Layout153, .LField_Layout153_SHAPES
	layout_offset Field_Layout153, .LField_Layout153_GRID
	layout_offset Field_Layout153, .LField_Layout153_ANIMATION
	layout_offset Field_Layout153, .LField_Layout153_BLEND
	layout_offset Field_Layout153, .LField_Layout153_MARKERS
.LField_Layout153_METATILES:
	.incbin "games/tbs/src/layout/layout_153/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout153_SHAPES:
	.incbin "games/tbs/src/layout/layout_153/shapes.table.lz"
	.balign 4, 0
.LField_Layout153_GRID:
	.incbin "games/tbs/src/layout/layout_153/map.grid.lz"
	.balign 4, 0
.LField_Layout153_ANIMATION:
	.incbin "games/tbs/src/layout/layout_153/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout153_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout153_MARKERS:
	.incbin "games/tbs/src/layout/layout_153/treasure_spots.table"
	.balign 4, 0
