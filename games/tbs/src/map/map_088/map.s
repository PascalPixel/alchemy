	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map088
Field_Map088:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map088, .LField_Map088_METATILES
	map_offset Field_Map088, .LField_Map088_SHAPES
	map_offset Field_Map088, .LField_Map088_GRID
	map_offset Field_Map088, .LField_Map088_ANIMATION
	map_offset Field_Map088, .LField_Map088_BLEND
	map_offset Field_Map088, .LField_Map088_MARKERS
.LField_Map088_METATILES:
	.incbin "games/tbs/src/map/map_088/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map088_SHAPES:
	.incbin "games/tbs/src/map/map_088/shapes.table.lz"
	.balign 4, 0
.LField_Map088_GRID:
	.incbin "games/tbs/src/map/map_088/map.grid.lz"
	.balign 4, 0
.LField_Map088_ANIMATION:
	.incbin "games/tbs/src/map/map_080/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map088_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map088_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette07
Tileset_Palette07:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_07.gbapal.lz"
	.byte 0x14, 0x26	@ PACKER: left in the buffer by Field_Map088+0x162
