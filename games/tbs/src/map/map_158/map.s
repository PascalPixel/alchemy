	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map158
Field_Map158:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map158, .LField_Map158_METATILES
	map_offset Field_Map158, .LField_Map158_SHAPES
	map_offset Field_Map158, .LField_Map158_GRID
	map_offset Field_Map158, .LField_Map158_ANIMATION
	map_offset Field_Map158
	map_offset Field_Map158, .LField_Map158_MARKERS
.LField_Map158_METATILES:
	.incbin "games/tbs/src/map/map_158/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map158_SHAPES:
	.incbin "games/tbs/src/map/map_158/shapes.table.lz"
	.balign 4, 0
.LField_Map158_GRID:
	.incbin "games/tbs/src/map/map_158/map.grid.lz"
	.balign 4, 0
.LField_Map158_ANIMATION:
	.incbin "games/tbs/src/map/map_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map158_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette46
Tileset_Palette46:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_46.gbapal.lz"
	.byte 0x90, 0x27, 0xd4	@ PACKER: left in the buffer by Field_Map158+0x1a5
