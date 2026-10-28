	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map144
Field_Map144:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map144, .LField_Map144_CELL_PICTURES
	map_offset Field_Map144, .LField_Map144_SHAPES
	map_offset Field_Map144, .LField_Map144_GRID
	map_offset Field_Map144, .LField_Map144_ANIMATION
	map_offset Field_Map144, .LField_Map144_BLEND
	map_offset Field_Map144, .LField_Map144_MARKERS
.LField_Map144_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_144/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map144_SHAPES:
	.incbin "games/tbs/src/map/map_144/shapes.table.lz"
	.balign 4, 0
.LField_Map144_GRID:
	.incbin "games/tbs/src/map/map_144/map.grid.lz"
	.balign 4, 0
.LField_Map144_ANIMATION:
	.incbin "games/tbs/src/map/map_144/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map144_BLEND:
	.incbin "games/tbs/src/map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map144_MARKERS:
	treasure_spot 24, 19, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette38
Tileset_Palette38:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_38.gbapal.lz"
