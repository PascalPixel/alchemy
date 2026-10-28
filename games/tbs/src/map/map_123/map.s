	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map123
Field_Map123:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map123, .LField_Map123_CELL_PICTURES
	map_offset Field_Map123, .LField_Map123_SHAPES
	map_offset Field_Map123, .LField_Map123_GRID
	map_offset Field_Map123, .LField_Map123_ANIMATION
	map_offset Field_Map123, .LField_Map123_BLEND
	map_offset Field_Map123, .LField_Map123_MARKERS
.LField_Map123_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_123/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map123_SHAPES:
	.incbin "games/tbs/src/map/map_123/shapes.table.lz"
	.balign 4, 0
.LField_Map123_GRID:
	.incbin "games/tbs/src/map/map_123/map.grid.lz"
	.balign 4, 0
.LField_Map123_ANIMATION:
	.incbin "games/tbs/src/map/map_123/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map123_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map123_MARKERS:
	treasure_spot 6, 3, 203
	treasure_spot 7, 3, 203
	treasure_spot 24, 3, 200
	treasure_spot 25, 3, 200
	treasure_spot 9, 18, 201
	treasure_spot 10, 18, 201
	treasure_spot 12, 18, 204
	treasure_spot 13, 18, 204
	treasure_spot 55, 18, 202
	treasure_spot 56, 18, 202
	treasure_spot 58, 18, 205
	treasure_spot 59, 18, 205
	treasure_spot 58, 33, 206
	treasure_spot 59, 33, 206
	treasure_spot 34, 61, 100
	treasure_spots_end
	.global Tileset_Palette28
Tileset_Palette28:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_28.gbapal.lz"
	.byte 0x15, 0x30	@ PACKER: left in the buffer by Field_Map123+0x186
