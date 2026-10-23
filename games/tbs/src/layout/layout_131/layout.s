	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout131
Field_Layout131:
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout131, .LField_Layout131_METATILES
	layout_offset Field_Layout131, .LField_Layout131_SHAPES
	layout_offset Field_Layout131, .LField_Layout131_GRID
	layout_offset Field_Layout131, .LField_Layout131_ANIMATION
	layout_offset Field_Layout131, .LField_Layout131_BLEND
	layout_offset Field_Layout131, .LField_Layout131_MARKERS
.LField_Layout131_METATILES:
	.incbin "games/tbs/src/layout/layout_131/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout131_SHAPES:
	.incbin "games/tbs/src/layout/layout_062/shapes.table.lz"
	.balign 4, 0
.LField_Layout131_GRID:
	.incbin "games/tbs/src/layout/layout_131/map.grid.lz"
	.balign 4, 0
.LField_Layout131_ANIMATION:
	.incbin "games/tbs/src/layout/layout_131/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout131_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout131_MARKERS:
	.incbin "games/tbs/src/layout/layout_131/treasure_spots.table"
	.balign 4, 0
