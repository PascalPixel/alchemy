	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout139
Field_Layout139:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout139, .LField_Layout139_METATILES
	layout_offset Field_Layout139, .LField_Layout139_SHAPES
	layout_offset Field_Layout139, .LField_Layout139_GRID
	layout_offset Field_Layout139, .LField_Layout139_ANIMATION
	layout_offset Field_Layout139, .LField_Layout139_BLEND
	layout_offset Field_Layout139, .LField_Layout139_MARKERS
.LField_Layout139_METATILES:
	.incbin "games/tbs/src/layout/layout_139/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout139_SHAPES:
	.incbin "games/tbs/src/layout/layout_139/shapes.table.lz"
	.balign 4, 0
.LField_Layout139_GRID:
	.incbin "games/tbs/src/layout/layout_139/map.grid.lz"
	.balign 4, 0
.LField_Layout139_ANIMATION:
	.incbin "games/tbs/src/layout/layout_029/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout139_BLEND:
	.incbin "games/tbs/src/layout/layout_139/blend.script.lz"
	.balign 4, 0
.LField_Layout139_MARKERS:
	treasure_spot 29, 24, 104
	treasure_spot 47, 52, 100
	treasure_spot 49, 53, 101
	treasure_spot 46, 54, 103
	treasure_spot 48, 55, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette36
Tileset_Palette36:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_36.gbapal.lz"
