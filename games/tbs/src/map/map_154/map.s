	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map154
Field_Map154:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map154, .LField_Map154_METATILES
	map_offset Field_Map154, .LField_Map154_SHAPES
	map_offset Field_Map154, .LField_Map154_GRID
	map_offset Field_Map154, .LField_Map154_ANIMATION
	map_offset Field_Map154, .LField_Map154_BLEND
	map_offset Field_Map154, .LField_Map154_MARKERS
.LField_Map154_METATILES:
	.incbin "games/tbs/src/map/map_154/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map154_SHAPES:
	.incbin "games/tbs/src/map/map_154/shapes.table.lz"
	.balign 4, 0
.LField_Map154_GRID:
	.incbin "games/tbs/src/map/map_154/map.grid.lz"
	.balign 4, 0
.LField_Map154_ANIMATION:
	.incbin "games/tbs/src/map/map_154/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map154_BLEND:
	.incbin "games/tbs/src/map/map_154/blend.script.lz"
	.balign 4, 0
.LField_Map154_MARKERS:
	treasure_spot 33, 26, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette42
Tileset_Palette42:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_42.gbapal.lz"
	.byte 0x94, 0x1c	@ PACKER: left in the buffer by Field_Map154+0x1b6
