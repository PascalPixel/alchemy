	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map078
Field_Map078:
	map_header 0, 0, 64, 64, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map078, .LField_Map078_METATILES
	map_offset Field_Map078, .LField_Map078_SHAPES
	map_offset Field_Map078, .LField_Map078_GRID
	map_offset Field_Map078, .LField_Map078_ANIMATION
	map_offset Field_Map078, .LField_Map078_BLEND
	map_offset Field_Map078, .LField_Map078_MARKERS
.LField_Map078_METATILES:
	.incbin "games/tbs/src/map/map_078/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map078_SHAPES:
	.incbin "games/tbs/src/map/map_078/shapes.table.lz"
	.balign 4, 0
.LField_Map078_GRID:
	.incbin "games/tbs/src/map/map_078/map.grid.lz"
	.balign 4, 0
.LField_Map078_ANIMATION:
	.incbin "games/tbs/src/map/map_078/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map078_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map078_MARKERS:
	treasure_spots_end
	.balign 4, 0
