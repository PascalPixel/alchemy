	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map097
Field_Map097:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map097, .LField_Map097_CELL_PICTURES
	map_offset Field_Map097, .LField_Map097_SHAPES
	map_offset Field_Map097, .LField_Map097_GRID
	map_offset Field_Map097, .LField_Map097_ANIMATION
	map_offset Field_Map097, .LField_Map097_BLEND
	map_offset Field_Map097, .LField_Map097_MARKERS
.LField_Map097_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_097/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map097_SHAPES:
	.incbin "games/tbs/src/map/map_097/shapes.table.lz"
	.balign 4, 0
.LField_Map097_GRID:
	.incbin "games/tbs/src/map/map_097/map.grid.lz"
	.balign 4, 0
.LField_Map097_ANIMATION:
	.incbin "games/tbs/src/map/map_097/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map097_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map097_MARKERS:
	treasure_spot 6, 4, 201
	treasure_spot 7, 4, 201
	treasure_spot 11, 4, 200
	treasure_spot 12, 4, 200
	treasure_spot 37, 4, 202
	treasure_spot 38, 4, 202
	treasure_spot 4, 15, 100
	treasure_spot 42, 16, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette08
Tileset_Palette08:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_08.gbapal.lz"
