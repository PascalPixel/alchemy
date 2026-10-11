	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map077
Field_Map077:
	map_header 0, 0, 64, 68, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map077, .LField_Map077_METATILES
	map_block Field_Map077, .LField_Map077_SHAPES
	map_block Field_Map077, .LField_Map077_GRID
	map_block Field_Map077, .LField_Map077_ANIMATION
	map_block Field_Map077, .LField_Map077_BLEND
	map_block Field_Map077, .LField_Map077_MARKERS
.LField_Map077_METATILES:
	.incbin "map/map_077/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map077_SHAPES:
	.incbin "map/map_077/shapes.table.lz"
	.balign 4, 0
.LField_Map077_GRID:
	.incbin "map/map_077/grid.parts.lz"
	.balign 4, 0
.LField_Map077_ANIMATION:
	.incbin "map/map_077/animation.script.lz"
	.balign 4, 0
.LField_Map077_BLEND:
	.incbin "map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map077_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette05
Tileset_Palette05:
	.incbin "graphics/tileset/palettes/pal_05.gbapal.lz"
	.byte 0x5d, 0x02, 0x06	@ PACKER: left in the buffer by Field_Map077+0x1b1
