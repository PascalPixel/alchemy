	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout027
Field_Layout027:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout027, .LField_Layout027_METATILES
	layout_offset Field_Layout027, .LField_Layout027_SHAPES
	layout_offset Field_Layout027, .LField_Layout027_GRID
	layout_offset Field_Layout027, .LField_Layout027_ANIMATION
	layout_offset Field_Layout027, .LField_Layout027_BLEND
	layout_offset Field_Layout027, .LField_Layout027_MARKERS
.LField_Layout027_METATILES:
	.incbin "games/tbs/src/layout/layout_027/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout027_SHAPES:
	.incbin "games/tbs/src/layout/layout_027/shapes.table.lz"
	.balign 4, 0
.LField_Layout027_GRID:
	.incbin "games/tbs/src/layout/layout_027/map.grid.lz"
	.balign 4, 0
.LField_Layout027_ANIMATION:
	.incbin "games/tbs/src/layout/layout_025/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout027_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout027_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
