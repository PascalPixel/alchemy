	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout136
Field_Layout136:
	layout_header 0, 0, 96, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout136, .LField_Layout136_METATILES
	layout_offset Field_Layout136, .LField_Layout136_SHAPES
	layout_offset Field_Layout136, .LField_Layout136_GRID
	layout_offset Field_Layout136, .LField_Layout136_ANIMATION
	layout_offset Field_Layout136, .LField_Layout136_BLEND
	layout_offset Field_Layout136, .LField_Layout136_MARKERS
.LField_Layout136_METATILES:
	.incbin "games/tbs/src/layout/layout_136/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout136_SHAPES:
	.incbin "games/tbs/src/layout/layout_136/shapes.table.lz"
	.balign 4, 0
.LField_Layout136_GRID:
	.incbin "games/tbs/src/layout/layout_136/map.grid.lz"
	.balign 4, 0
.LField_Layout136_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout136_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Layout136_MARKERS:
	treasure_spot 27, 6, 100
	treasure_spot 24, 13, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette34
Tileset_Palette34:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_34.gbapal.lz"
