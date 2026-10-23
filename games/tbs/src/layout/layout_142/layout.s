	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout142
Field_Layout142:
	layout_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout142, .LField_Layout142_METATILES
	layout_offset Field_Layout142, .LField_Layout142_SHAPES
	layout_offset Field_Layout142, .LField_Layout142_GRID
	layout_offset Field_Layout142, .LField_Layout142_ANIMATION
	layout_offset Field_Layout142, .LField_Layout142_BLEND
	layout_offset Field_Layout142, .LField_Layout142_MARKERS
.LField_Layout142_METATILES:
	.incbin "games/tbs/src/layout/layout_142/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout142_SHAPES:
	.incbin "games/tbs/src/layout/layout_062/shapes.table.lz"
	.balign 4, 0
.LField_Layout142_GRID:
	.incbin "games/tbs/src/layout/layout_142/map.grid.lz"
	.balign 4, 0
.LField_Layout142_ANIMATION:
	.incbin "games/tbs/src/layout/layout_029/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout142_BLEND:
	.incbin "games/tbs/src/layout/layout_139/blend.script.lz"
	.balign 4, 0
.LField_Layout142_MARKERS:
	.incbin "games/tbs/src/layout/layout_142/treasure_spots.table"
	.balign 4, 0
