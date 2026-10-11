	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map113
Field_Map113:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map113, .LField_Map113_METATILES
	map_block Field_Map113, .LField_Map113_SHAPES
	map_block Field_Map113, .LField_Map113_GRID
	map_block Field_Map113, .LField_Map113_ANIMATION
	map_block Field_Map113, .LField_Map113_BLEND
	map_block Field_Map113, .LField_Map113_MARKERS
.LField_Map113_METATILES:
	.incbin "map/map_113/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map113_SHAPES:
	.incbin "map/map_113/shapes.table.lz"
	.balign 4, 0
.LField_Map113_GRID:
	.incbin "map/map_113/grid.parts.lz"
	.balign 4, 0
.LField_Map113_ANIMATION:
	.incbin "map/map_037/animation.script.lz"
	.balign 4, 0
.LField_Map113_BLEND:
	.incbin "map/map_113/blend.script.lz"
	.balign 4, 0
.LField_Map113_MARKERS:
	.incbin "map/map_113/markers.table"
	.balign 4, 0
	.global Tileset_Palette19
Tileset_Palette19:
	.incbin "graphics/tileset/palettes/pal_19.gbapal.lz"
	.byte 0xb3, 0x90, 0x80	@ PACKER: left in the buffer by Field_Map113+0x185
