	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout072
Field_Layout072:
	layout_header 10, 4, 67, 67, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout072, .LField_Layout072_METATILES
	layout_offset Field_Layout072, .LField_Layout072_SHAPES
	layout_offset Field_Layout072, .LField_Layout072_GRID
	layout_offset Field_Layout072, .LField_Layout072_ANIMATION
	layout_offset Field_Layout072, .LField_Layout072_BLEND
	layout_offset Field_Layout072, .LField_Layout072_MARKERS
.LField_Layout072_METATILES:
	.incbin "games/tbs/src/layout/layout_072/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout072_SHAPES:
	.incbin "games/tbs/src/layout/layout_072/shapes.table.lz"
	.balign 4, 0
.LField_Layout072_GRID:
	.incbin "games/tbs/src/layout/layout_072/map.grid.lz"
	.balign 4, 0
.LField_Layout072_ANIMATION:
	.incbin "games/tbs/src/layout/layout_028/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout072_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout072_MARKERS:
	.incbin "games/tbs/src/layout/layout_072/treasure_spots.table"
	.balign 4, 0
