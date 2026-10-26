	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map092
Field_Map092:
	map_header 0, 0, 192, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map092, .LField_Map092_METATILES
	map_offset Field_Map092, .LField_Map092_SHAPES
	map_offset Field_Map092, .LField_Map092_GRID
	map_offset Field_Map092, .LField_Map092_ANIMATION
	map_offset Field_Map092
	map_offset Field_Map092, .LField_Map092_MARKERS
.LField_Map092_METATILES:
	.incbin "games/tbs/src/map/map_092/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map092_SHAPES:
	.incbin "games/tbs/src/map/map_092/shapes.table.lz"
	.balign 4, 0
.LField_Map092_GRID:
	.incbin "games/tbs/src/map/map_092/map.grid.lz"
	.balign 4, 0
.LField_Map092_ANIMATION:
	.incbin "games/tbs/src/map/map_091/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map092_MARKERS:
	treasure_spot 10, 15, 107
	treasure_spots_end
	.balign 4, 0
