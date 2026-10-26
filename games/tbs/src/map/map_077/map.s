	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map077
Field_Map077:
	map_header 0, 0, 64, 68, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map077, .LField_Map077_METATILES
	map_offset Field_Map077, .LField_Map077_SHAPES
	map_offset Field_Map077, .LField_Map077_GRID
	map_offset Field_Map077, .LField_Map077_ANIMATION
	map_offset Field_Map077, .LField_Map077_BLEND
	map_offset Field_Map077, .LField_Map077_MARKERS
.LField_Map077_METATILES:
	.incbin "games/tbs/src/map/map_077/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map077_SHAPES:
	.incbin "games/tbs/src/map/map_077/shapes.table.lz"
	.balign 4, 0
.LField_Map077_GRID:
	.incbin "games/tbs/src/map/map_077/map.grid.lz"
	.balign 4, 0
.LField_Map077_ANIMATION:
	.incbin "games/tbs/src/map/map_077/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map077_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map077_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette05
Tileset_Palette05:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_05.gbapal.lz"
	.byte 0x5d, 0x02, 0x06	@ PACKER: left in the buffer by Field_Map077+0x1b1
