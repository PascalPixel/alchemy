	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map109
Field_Map109:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map109, .LField_Map109_METATILES
	map_block Field_Map109, .LField_Map109_SHAPES
	map_block Field_Map109, .LField_Map109_GRID
	map_block Field_Map109, .LField_Map109_ANIMATION
	map_block Field_Map109, .LField_Map109_BLEND
	map_block Field_Map109, .LField_Map109_MARKERS
.LField_Map109_METATILES:
	.incbin "map/map_109/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map109_SHAPES:
	.incbin "map/map_009/shapes.table.lz"
	.balign 4, 0
.LField_Map109_GRID:
	.incbin "map/map_109/grid.parts.lz"
	.balign 4, 0
.LField_Map109_ANIMATION:
	.incbin "map/map_054/animation.script.lz"
	.balign 4, 0
.LField_Map109_BLEND:
	.incbin "common/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map109_MARKERS:
	.incbin "map/map_109/markers.table"
	.balign 4, 0
	.global Tileset_Palette14
Tileset_Palette14:
	.incbin "graphics/tileset/palettes/pal_14.gbapal.lz"
	.byte 0xf8, 0xe4	@ PACKER: left in the buffer by Field_Map109+0x17e
