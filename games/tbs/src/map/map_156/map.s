	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map156
Field_Map156:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map156, .LField_Map156_METATILES
	map_offset Field_Map156, .LField_Map156_SHAPES
	map_offset Field_Map156, .LField_Map156_GRID
	map_offset Field_Map156, .LField_Map156_ANIMATION
	map_offset Field_Map156, .LField_Map156_BLEND
	map_offset Field_Map156, .LField_Map156_MARKERS
.LField_Map156_METATILES:
	.incbin "games/tbs/src/map/map_156/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map156_SHAPES:
	.incbin "games/tbs/src/map/map_156/shapes.table.lz"
	.balign 4, 0
.LField_Map156_GRID:
	.incbin "games/tbs/src/map/map_156/map.grid.lz"
	.balign 4, 0
.LField_Map156_ANIMATION:
	.incbin "games/tbs/src/map/map_054/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map156_BLEND:
	.incbin "games/tbs/src/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map156_MARKERS:
	treasure_spot 49, 43, 101
	treasure_spot 8, 49, 100
	treasure_spots_end
	.balign 4, 0
