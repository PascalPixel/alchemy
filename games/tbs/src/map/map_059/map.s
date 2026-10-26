	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map059
Field_Map059:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map059, .LField_Map059_METATILES
	map_offset Field_Map059, .LField_Map059_SHAPES
	map_offset Field_Map059, .LField_Map059_GRID
	map_offset Field_Map059, .LField_Map059_ANIMATION
	map_offset Field_Map059
	map_offset Field_Map059, .LField_Map059_MARKERS
.LField_Map059_METATILES:
	.incbin "games/tbs/src/map/map_059/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map059_SHAPES:
	.incbin "games/tbs/src/map/map_059/shapes.table.lz"
	.balign 4, 0
.LField_Map059_GRID:
	.incbin "games/tbs/src/map/map_059/map.grid.lz"
	.balign 4, 0
.LField_Map059_ANIMATION:
	.incbin "games/tbs/src/map/map_054/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map059_MARKERS:
	treasure_spot 31, 8, 100
	treasure_spots_end
	.balign 4, 0
