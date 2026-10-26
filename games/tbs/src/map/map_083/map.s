	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map083
Field_Map083:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map083, .LField_Map083_METATILES
	map_offset Field_Map083, .LField_Map083_SHAPES
	map_offset Field_Map083, .LField_Map083_GRID
	map_offset Field_Map083, .LField_Map083_ANIMATION
	map_offset Field_Map083, .LField_Map083_BLEND
	map_offset Field_Map083, .LField_Map083_MARKERS
.LField_Map083_METATILES:
	.incbin "games/tbs/src/map/map_083/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map083_SHAPES:
	.incbin "games/tbs/src/map/map_083/shapes.table.lz"
	.balign 4, 0
.LField_Map083_GRID:
	.incbin "games/tbs/src/map/map_083/map.grid.lz"
	.balign 4, 0
.LField_Map083_ANIMATION:
	.incbin "games/tbs/src/map/map_080/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map083_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map083_MARKERS:
	treasure_spot 3, 24, 100
	treasure_spot 16, 49, 101
	treasure_spots_end
	.balign 4, 0
