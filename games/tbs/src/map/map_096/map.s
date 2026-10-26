	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map096
Field_Map096:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map096, .LField_Map096_METATILES
	map_offset Field_Map096, .LField_Map096_SHAPES
	map_offset Field_Map096, .LField_Map096_GRID
	map_offset Field_Map096, .LField_Map096_ANIMATION
	map_offset Field_Map096, .LField_Map096_BLEND
	map_offset Field_Map096, .LField_Map096_MARKERS
.LField_Map096_METATILES:
	.incbin "games/tbs/src/map/map_096/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map096_SHAPES:
	.incbin "games/tbs/src/map/map_096/shapes.table.lz"
	.balign 4, 0
.LField_Map096_GRID:
	.incbin "games/tbs/src/map/map_096/map.grid.lz"
	.balign 4, 0
.LField_Map096_ANIMATION:
	.incbin "games/tbs/src/map/map_093/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map096_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map096_MARKERS:
	treasure_spots_end
	.balign 4, 0
