	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map107
Field_Map107:
	map_header 0, 0, 64, 78, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 76, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 152, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map107, .LField_Map107_METATILES
	map_block Field_Map107, .LField_Map107_SHAPES
	map_block Field_Map107, .LField_Map107_GRID
	map_block Field_Map107, .LField_Map107_ANIMATION
	map_block Field_Map107
	map_block Field_Map107, .LField_Map107_MARKERS
.LField_Map107_METATILES:
	.incbin "games/tbs/src/layout/layout_107/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map107_SHAPES:
	.incbin "games/tbs/src/layout/layout_107/shapes.table.lz"
	.balign 4, 0
.LField_Map107_GRID:
	.incbin "games/tbs/src/layout/layout_107/grid.parts.lz"
	.balign 4, 0
.LField_Map107_ANIMATION:
	.incbin "games/tbs/src/layout/layout_107/animation.script.lz"
	.balign 4, 0
.LField_Map107_MARKERS:
	.incbin "games/tbs/src/layout/layout_107/markers.table"
	.global Tileset_Palette13
Tileset_Palette13:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_13.gbapal.lz"
	.byte 0x94, 0x4b, 0xc1	@ PACKER: left in the buffer by Field_Map107+0x191
