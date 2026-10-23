	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout162
Field_Layout162:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout162, .LField_Layout162_METATILES
	layout_offset Field_Layout162, .LField_Layout162_SHAPES
	layout_offset Field_Layout162, .LField_Layout162_GRID
	layout_offset Field_Layout162, .LField_Layout162_ANIMATION
	layout_offset Field_Layout162, .LField_Layout162_BLEND
	layout_offset Field_Layout162, .LField_Layout162_MARKERS
.LField_Layout162_METATILES:
	.incbin "games/tbs/src/layout/layout_162/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout162_SHAPES:
	.incbin "games/tbs/src/layout/layout_162/shapes.table.lz"
	.balign 4, 0
.LField_Layout162_GRID:
	.incbin "games/tbs/src/layout/layout_162/map.grid.lz"
	.balign 4, 0
.LField_Layout162_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout162_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout162_MARKERS:
	.incbin "games/tbs/src/layout/layout_162/treasure_spots.table"
	.balign 4, 0
