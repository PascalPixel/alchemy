	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map074
Field_Map074:
	map_header 0, 0, 72, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 72, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 144, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map074, .LField_Map074_METATILES
	map_offset Field_Map074, .LField_Map074_SHAPES
	map_offset Field_Map074, .LField_Map074_GRID
	map_offset Field_Map074, .LField_Map074_ANIMATION
	map_offset Field_Map074, .LField_Map074_BLEND
	map_offset Field_Map074, .LField_Map074_MARKERS
.LField_Map074_METATILES:
	.incbin "games/tbs/src/map/map_074/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map074_SHAPES:
	.incbin "games/tbs/src/map/map_074/shapes.table.lz"
	.balign 4, 0
.LField_Map074_GRID:
	.incbin "games/tbs/src/map/map_074/map.grid.lz"
	.balign 4, 0
.LField_Map074_ANIMATION:
	.incbin "games/tbs/src/map/map_074/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map074_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map074_MARKERS:
	treasure_spot 27, 24, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette04
Tileset_Palette04:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_04.gbapal.lz"
	.byte 0xdb, 0x0e, 0xea	@ PACKER: left in the buffer by Field_Map074+0x195
