	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map114
Field_Map114:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map114, .LField_Map114_CELL_PICTURES
	map_offset Field_Map114, .LField_Map114_SHAPES
	map_offset Field_Map114, .LField_Map114_GRID
	map_offset Field_Map114, .LField_Map114_ANIMATION
	map_offset Field_Map114, .LField_Map114_BLEND
	map_offset Field_Map114, .LField_Map114_MARKERS
.LField_Map114_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_114/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map114_SHAPES:
	.incbin "games/tbs/src/map/map_114/shapes.table.lz"
	.balign 4, 0
.LField_Map114_GRID:
	.incbin "games/tbs/src/map/map_114/map.grid.lz"
	.balign 4, 0
.LField_Map114_ANIMATION:
	.incbin "games/tbs/src/map/map_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map114_BLEND:
	.incbin "games/tbs/src/map/map_114/blend.script.lz"
	.balign 4, 0
.LField_Map114_MARKERS:
	treasure_spot 4, 12, 101
	treasure_spot 14, 13, 100
	treasure_spot 14, 14, 102
	treasure_spot 25, 16, 103
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette20
Tileset_Palette20:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_20.gbapal.lz"
