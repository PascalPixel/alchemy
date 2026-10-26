	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map120
Field_Map120:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map120, .LField_Map120_METATILES
	map_offset Field_Map120, .LField_Map120_SHAPES
	map_offset Field_Map120, .LField_Map120_GRID
	map_offset Field_Map120, .LField_Map120_ANIMATION
	map_offset Field_Map120, .LField_Map120_BLEND
	map_offset Field_Map120, .LField_Map120_MARKERS
.LField_Map120_METATILES:
	.incbin "games/tbs/src/map/map_120/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map120_SHAPES:
	.incbin "games/tbs/src/map/map_120/shapes.table.lz"
	.balign 4, 0
.LField_Map120_GRID:
	.incbin "games/tbs/src/map/map_120/map.grid.lz"
	.balign 4, 0
.LField_Map120_ANIMATION:
	.incbin "games/tbs/src/map/map_094/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map120_BLEND:
	.incbin "games/tbs/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map120_MARKERS:
	treasure_spot 20, 5, 200
	treasure_spot 21, 5, 200
	treasure_spot 19, 7, 100
	treasure_spot 6, 20, 101
	treasure_spot 36, 20, 101
	treasure_spot 37, 20, 201
	treasure_spot 38, 20, 201
	treasure_spot 14, 35, 202
	treasure_spot 15, 35, 202
	treasure_spot 35, 52, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette26
Tileset_Palette26:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_26.gbapal.lz"
	.byte 0x9c, 0x00	@ PACKER: left in the buffer by Field_Map120+0x1de
