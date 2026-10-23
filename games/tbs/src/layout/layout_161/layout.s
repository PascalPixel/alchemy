	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout161
Field_Layout161:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout161, .LField_Layout161_METATILES
	layout_offset Field_Layout161, .LField_Layout161_SHAPES
	layout_offset Field_Layout161, .LField_Layout161_GRID
	layout_offset Field_Layout161, .LField_Layout161_ANIMATION
	layout_offset Field_Layout161, .LField_Layout161_BLEND
	layout_offset Field_Layout161, .LField_Layout161_MARKERS
.LField_Layout161_METATILES:
	.incbin "games/tbs/src/layout/layout_161/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout161_SHAPES:
	.incbin "games/tbs/src/layout/layout_161/shapes.table.lz"
	.balign 4, 0
.LField_Layout161_GRID:
	.incbin "games/tbs/src/layout/layout_161/map.grid.lz"
	.balign 4, 0
.LField_Layout161_ANIMATION:
	.incbin "games/tbs/src/layout/layout_161/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout161_BLEND:
	.incbin "games/tbs/src/layout/layout_018/blend.script.lz"
	.balign 4, 0
.LField_Layout161_MARKERS:
	.incbin "games/tbs/src/layout/layout_161/treasure_spots.table"
	.balign 4, 0
