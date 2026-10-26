	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map168
Field_Map168:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map168, .LField_Map168_METATILES
	map_offset Field_Map168, .LField_Map168_SHAPES
	map_offset Field_Map168, .LField_Map168_GRID
	map_offset Field_Map168, .LField_Map168_ANIMATION
	map_offset Field_Map168
	map_offset Field_Map168, .LField_Map168_MARKERS
.LField_Map168_METATILES:
	.incbin "games/tbs/src/map/map_168/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map168_SHAPES:
	.incbin "games/tbs/src/map/map_168/shapes.table.lz"
	.balign 4, 0
.LField_Map168_GRID:
	.incbin "games/tbs/src/map/map_168/map.grid.lz"
	.balign 4, 0
.LField_Map168_ANIMATION:
	.incbin "games/tbs/src/map/map_054/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map168_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette52
Tileset_Palette52:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_52.gbapal.lz"
	.byte 0x06	@ PACKER: left in the buffer by Field_Map168+0x103
