	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map116
Field_Map116:
	map_header 0, 0, 34, 40, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map116, .LField_Map116_METATILES
	map_block Field_Map116, .LField_Map116_SHAPES
	map_block Field_Map116, .LField_Map116_GRID
	map_block Field_Map116, .LField_Map116_ANIMATION
	map_block Field_Map116
	map_block Field_Map116, .LField_Map116_MARKERS
.LField_Map116_METATILES:
	.incbin "games/tbs/src/layout/layout_116/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map116_SHAPES:
	.incbin "games/tbs/src/layout/layout_116/shapes.table.lz"
	.balign 4, 0
.LField_Map116_GRID:
	.incbin "games/tbs/src/layout/layout_116/grid.parts.lz"
	.balign 4, 0
.LField_Map116_ANIMATION:
	.incbin "games/tbs/src/layout/layout_101/animation.script.lz"
	.balign 4, 0
.LField_Map116_MARKERS:
	.incbin "games/tbs/src/layout/layout_101/markers.table"
	.balign 4, 0
	.global Tileset_Palette22
Tileset_Palette22:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_22.gbapal.lz"
	.byte 0x50, 0x42, 0x7d	@ PACKER: left in the buffer by Field_Map116+0x171
