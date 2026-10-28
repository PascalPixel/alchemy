	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map109
Field_Map109:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map109, .LField_Map109_CELL_PICTURES
	map_offset Field_Map109, .LField_Map109_SHAPES
	map_offset Field_Map109, .LField_Map109_GRID
	map_offset Field_Map109, .LField_Map109_ANIMATION
	map_offset Field_Map109, .LField_Map109_BLEND
	map_offset Field_Map109, .LField_Map109_MARKERS
.LField_Map109_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_109/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map109_SHAPES:
	.incbin "games/tbs/src/map/map_009/shapes.table.lz"
	.balign 4, 0
.LField_Map109_GRID:
	.incbin "games/tbs/src/map/map_109/map.grid.lz"
	.balign 4, 0
.LField_Map109_ANIMATION:
	.incbin "games/tbs/src/map/map_054/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map109_BLEND:
	.incbin "games/tbs/src/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map109_MARKERS:
	treasure_spot 19, 8, 100
	treasure_spot 7, 13, 101
	treasure_spot 18, 13, 102
	treasure_spot 12, 17, 103
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette14
Tileset_Palette14:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_14.gbapal.lz"
	.byte 0xf8, 0xe4	@ PACKER: left in the buffer by Field_Map109+0x17e
