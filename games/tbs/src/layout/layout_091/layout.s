	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout091
Field_Layout091:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0x7f, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout091, .LField_Layout091_METATILES
	layout_offset Field_Layout091, .LField_Layout091_SHAPES
	layout_offset Field_Layout091, .LField_Layout091_GRID
	layout_offset Field_Layout091, .LField_Layout091_ANIMATION
	layout_offset Field_Layout091
	layout_offset Field_Layout091, .LField_Layout091_MARKERS
.LField_Layout091_METATILES:
	.incbin "games/tbs/src/layout/layout_091/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout091_SHAPES:
	.incbin "games/tbs/src/layout/layout_091/shapes.table.lz"
	.balign 4, 0
.LField_Layout091_GRID:
	.incbin "games/tbs/src/layout/layout_091/map.grid.lz"
	.balign 4, 0
.LField_Layout091_ANIMATION:
	.incbin "games/tbs/src/layout/layout_091/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout091_MARKERS:
	treasure_spot 15, 12, 102
	treasure_spot 58, 19, 103
	treasure_spot 40, 25, 104
	treasure_spot 18, 36, 105
	treasure_spot 45, 56, 106
	treasure_spots_end
	.balign 4, 0
