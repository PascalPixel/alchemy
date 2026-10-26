	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map149
Field_Map149:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map149, .LField_Map149_METATILES
	map_offset Field_Map149, .LField_Map149_SHAPES
	map_offset Field_Map149, .LField_Map149_GRID
	map_offset Field_Map149, .LField_Map149_ANIMATION
	map_offset Field_Map149, .LField_Map149_BLEND
	map_offset Field_Map149, .LField_Map149_MARKERS
.LField_Map149_METATILES:
	.incbin "games/tbs/src/map/map_149/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map149_SHAPES:
	.incbin "games/tbs/src/map/map_130/shapes.table.lz"
	.balign 4, 0
.LField_Map149_GRID:
	.incbin "games/tbs/src/map/map_149/map.grid.lz"
	.balign 4, 0
.LField_Map149_ANIMATION:
	.incbin "games/tbs/src/map/map_080/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map149_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map149_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette40
Tileset_Palette40:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_40.gbapal.lz"
	.byte 0xb5, 0x03	@ PACKER: left in the buffer by Field_Map149+0x172
