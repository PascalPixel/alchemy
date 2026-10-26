	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map117
Field_Map117:
	map_header 0, 0, 45, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map117, .LField_Map117_METATILES
	map_offset Field_Map117, .LField_Map117_SHAPES
	map_offset Field_Map117, .LField_Map117_GRID
	map_offset Field_Map117, .LField_Map117_ANIMATION
	map_offset Field_Map117
	map_offset Field_Map117, .LField_Map117_MARKERS
.LField_Map117_METATILES:
	.incbin "games/tbs/src/map/map_117/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map117_SHAPES:
	.incbin "games/tbs/src/map/map_117/shapes.table.lz"
	.balign 4, 0
.LField_Map117_GRID:
	.incbin "games/tbs/src/map/map_117/map.grid.lz"
	.balign 4, 0
.LField_Map117_ANIMATION:
	.incbin "games/tbs/src/map/map_103/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map117_MARKERS:
	treasure_spot 0, 1, 100
	treasure_spot 0, 3, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette23
Tileset_Palette23:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_23.gbapal.lz"
