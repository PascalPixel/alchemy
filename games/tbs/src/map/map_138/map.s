	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map138
Field_Map138:
	map_header 0, 0, 56, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map138, .LField_Map138_METATILES
	map_offset Field_Map138, .LField_Map138_SHAPES
	map_offset Field_Map138, .LField_Map138_GRID
	map_offset Field_Map138, .LField_Map138_ANIMATION
	map_offset Field_Map138, .LField_Map138_BLEND
	map_offset Field_Map138, .LField_Map138_MARKERS
.LField_Map138_METATILES:
	.incbin "games/tbs/src/map/map_138/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map138_SHAPES:
	.incbin "games/tbs/src/map/map_138/shapes.table.lz"
	.balign 4, 0
.LField_Map138_GRID:
	.incbin "games/tbs/src/map/map_138/map.grid.lz"
	.balign 4, 0
.LField_Map138_ANIMATION:
	.incbin "games/tbs/src/map/map_031/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map138_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map138_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette35
Tileset_Palette35:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_35.gbapal.lz"
