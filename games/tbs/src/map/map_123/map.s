	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map123
Field_Map123:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map123, .LField_Map123_METATILES
	map_block Field_Map123, .LField_Map123_SHAPES
	map_block Field_Map123, .LField_Map123_GRID
	map_block Field_Map123, .LField_Map123_ANIMATION
	map_block Field_Map123, .LField_Map123_BLEND
	map_block Field_Map123, .LField_Map123_MARKERS
.LField_Map123_METATILES:
	.incbin "games/tbs/src/map/map_123/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map123_SHAPES:
	.incbin "games/tbs/src/map/map_123/shapes.table.lz"
	.balign 4, 0
.LField_Map123_GRID:
	.incbin "games/tbs/src/map/map_123/grid.parts.lz"
	.balign 4, 0
.LField_Map123_ANIMATION:
	.incbin "games/tbs/src/map/map_123/animation.script.lz"
	.balign 4, 0
.LField_Map123_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map123_MARKERS:
	.incbin "games/tbs/src/map/map_123/markers.table"
	.global Tileset_Palette28
Tileset_Palette28:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_28.gbapal.lz"
	.byte 0x15, 0x30	@ PACKER: left in the buffer by Field_Map123+0x186
