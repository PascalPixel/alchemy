	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map062
Field_Map062:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map062, .LField_Map062_METATILES
	map_block Field_Map062, .LField_Map062_SHAPES
	map_block Field_Map062, .LField_Map062_GRID
	map_block Field_Map062, .LField_Map062_ANIMATION
	map_block Field_Map062, .LField_Map062_BLEND
	map_block Field_Map062, .LField_Map062_MARKERS
.LField_Map062_METATILES:
	.incbin "games/tbs/src/map/map_062/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map062_SHAPES:
	.incbin "games/tbs/src/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map062_GRID:
	.incbin "games/tbs/src/map/map_062/grid.parts.lz"
	.balign 4, 0
.LField_Map062_ANIMATION:
	.incbin "games/tbs/src/map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map062_BLEND:
	.incbin "games/tbs/src/map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map062_MARKERS:
	.incbin "games/tbs/src/map/map_062/markers.table"
	.balign 4, 0
	.global Tileset_Palette03
Tileset_Palette03:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_03.gbapal.lz"
	.byte 0x7e, 0x5e	@ PACKER: left in the buffer by Field_Map062+0x156
