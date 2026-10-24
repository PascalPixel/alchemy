	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout069
Field_Layout069:
	layout_header 0, 0, 106, 80, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout069, .LField_Layout069_METATILES
	layout_offset Field_Layout069, .LField_Layout069_SHAPES
	layout_offset Field_Layout069, .LField_Layout069_GRID
	layout_offset Field_Layout069, .LField_Layout069_ANIMATION
	layout_offset Field_Layout069
	layout_offset Field_Layout069, .LField_Layout069_MARKERS
.LField_Layout069_METATILES:
	.incbin "games/tbs/src/layout/layout_069/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout069_SHAPES:
	.incbin "games/tbs/src/layout/layout_069/shapes.table.lz"
	.balign 4, 0
.LField_Layout069_GRID:
	.incbin "games/tbs/src/layout/layout_069/map.grid.lz"
	.balign 4, 0
.LField_Layout069_ANIMATION:
	.incbin "games/tbs/src/layout/layout_068/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout069_MARKERS:
	treasure_spot 8, 20, 100
	treasure_spots_end
	.balign 4, 0
