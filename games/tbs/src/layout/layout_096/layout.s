	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout096
Field_Layout096:
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout096, .LField_Layout096_METATILES
	layout_offset Field_Layout096, .LField_Layout096_SHAPES
	layout_offset Field_Layout096, .LField_Layout096_GRID
	layout_offset Field_Layout096, .LField_Layout096_ANIMATION
	layout_offset Field_Layout096, .LField_Layout096_BLEND
	layout_offset Field_Layout096, .LField_Layout096_MARKERS
.LField_Layout096_METATILES:
	.incbin "games/tbs/src/layout/layout_096/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout096_SHAPES:
	.incbin "games/tbs/src/layout/layout_096/shapes.table.lz"
	.balign 4, 0
.LField_Layout096_GRID:
	.incbin "games/tbs/src/layout/layout_096/map.grid.lz"
	.balign 4, 0
.LField_Layout096_ANIMATION:
	.incbin "games/tbs/src/layout/layout_093/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout096_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout096_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
