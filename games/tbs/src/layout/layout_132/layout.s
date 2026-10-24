	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout132
Field_Layout132:
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout132, .LField_Layout132_METATILES
	layout_offset Field_Layout132, .LField_Layout132_SHAPES
	layout_offset Field_Layout132, .LField_Layout132_GRID
	layout_offset Field_Layout132, .LField_Layout132_ANIMATION
	layout_offset Field_Layout132, .LField_Layout132_BLEND
	layout_offset Field_Layout132, .LField_Layout132_MARKERS
.LField_Layout132_METATILES:
	.incbin "games/tbs/src/layout/layout_132/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout132_SHAPES:
	.incbin "games/tbs/src/layout/layout_130/shapes.table.lz"
	.balign 4, 0
.LField_Layout132_GRID:
	.incbin "games/tbs/src/layout/layout_132/map.grid.lz"
	.balign 4, 0
.LField_Layout132_ANIMATION:
	.incbin "games/tbs/src/layout/layout_131/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout132_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout132_MARKERS:
	treasure_spot 25, 13, 100
	treasure_spots_end
	.balign 4, 0
