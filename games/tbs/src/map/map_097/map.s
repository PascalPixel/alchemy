	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map097
Field_Map097:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map097, .LField_Map097_METATILES
	map_block Field_Map097, .LField_Map097_SHAPES
	map_block Field_Map097, .LField_Map097_GRID
	map_block Field_Map097, .LField_Map097_ANIMATION
	map_block Field_Map097, .LField_Map097_BLEND
	map_block Field_Map097, .LField_Map097_MARKERS
.LField_Map097_METATILES:
	.incbin "map/map_097/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map097_SHAPES:
	.incbin "map/map_097/shapes.table.lz"
	.balign 4, 0
.LField_Map097_GRID:
	.incbin "map/map_097/grid.parts.lz"
	.balign 4, 0
.LField_Map097_ANIMATION:
	.incbin "map/map_097/animation.script.lz"
	.balign 4, 0
.LField_Map097_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map097_MARKERS:
	.incbin "map/map_097/markers.table"
	.balign 4, 0
	.global Tileset_Palette08
Tileset_Palette08:
	.incbin "graphics/tileset/palettes/pal_08.gbapal.lz"
