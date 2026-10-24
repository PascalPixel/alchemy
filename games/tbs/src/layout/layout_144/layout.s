	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout144
Field_Layout144:
	layout_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout144, .LField_Layout144_METATILES
	layout_offset Field_Layout144, .LField_Layout144_SHAPES
	layout_offset Field_Layout144, .LField_Layout144_GRID
	layout_offset Field_Layout144, .LField_Layout144_ANIMATION
	layout_offset Field_Layout144, .LField_Layout144_BLEND
	layout_offset Field_Layout144, .LField_Layout144_MARKERS
.LField_Layout144_METATILES:
	.incbin "games/tbs/src/layout/layout_144/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout144_SHAPES:
	.incbin "games/tbs/src/layout/layout_144/shapes.table.lz"
	.balign 4, 0
.LField_Layout144_GRID:
	.incbin "games/tbs/src/layout/layout_144/map.grid.lz"
	.balign 4, 0
.LField_Layout144_ANIMATION:
	.incbin "games/tbs/src/layout/layout_144/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout144_BLEND:
	.incbin "games/tbs/src/layout/layout_033/blend.script.lz"
	.balign 4, 0
.LField_Layout144_MARKERS:
	treasure_spot 24, 19, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette38
Tileset_Palette38:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_38.gbapal.lz"
