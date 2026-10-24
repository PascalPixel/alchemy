	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout057
Field_Layout057:
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout057, .LField_Layout057_METATILES
	layout_offset Field_Layout057, .LField_Layout057_SHAPES
	layout_offset Field_Layout057, .LField_Layout057_GRID
	layout_offset Field_Layout057, .LField_Layout057_ANIMATION
	layout_offset Field_Layout057, .LField_Layout057_BLEND
	layout_offset Field_Layout057, .LField_Layout057_MARKERS
.LField_Layout057_METATILES:
	.incbin "games/tbs/src/layout/layout_057/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout057_SHAPES:
	.incbin "games/tbs/src/layout/layout_057/shapes.table.lz"
	.balign 4, 0
.LField_Layout057_GRID:
	.incbin "games/tbs/src/layout/layout_057/map.grid.lz"
	.balign 4, 0
.LField_Layout057_ANIMATION:
	.incbin "games/tbs/src/layout/layout_054/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout057_BLEND:
	.incbin "games/tbs/src/layout/layout_054/blend.script.lz"
	.balign 4, 0
.LField_Layout057_MARKERS:
	treasure_spots_end
	.balign 4, 0
