	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout166
Field_Layout166:
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout166, .LField_Layout166_METATILES
	layout_offset Field_Layout166, .LField_Layout166_SHAPES
	layout_offset Field_Layout166, .LField_Layout166_GRID
	layout_offset Field_Layout166, .LField_Layout166_ANIMATION
	layout_offset Field_Layout166, .LField_Layout166_BLEND
	layout_offset Field_Layout166, .LField_Layout166_MARKERS
.LField_Layout166_METATILES:
	.incbin "games/tbs/src/layout/layout_166/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout166_SHAPES:
	.incbin "games/tbs/src/layout/layout_166/shapes.table.lz"
	.balign 4, 0
.LField_Layout166_GRID:
	.incbin "games/tbs/src/layout/layout_166/grid.parts.lz"
	.balign 4, 0
.LField_Layout166_ANIMATION:
	.incbin "games/tbs/src/layout/layout_164/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout166_BLEND:
	.incbin "games/tbs/src/layout/layout_164/blend.script.lz"
	.balign 4, 0
.LField_Layout166_MARKERS:
	.incbin "games/tbs/src/layout/layout_166/treasure_spots.table"
	.balign 4, 0
