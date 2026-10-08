	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map040
Field_Map040:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map040, .LField_Map040_METATILES
	map_block Field_Map040, .LField_Map040_SHAPES
	map_block Field_Map040, .LField_Map040_GRID
	map_block Field_Map040, .LField_Map040_ANIMATION
	map_block Field_Map040, .LField_Map040_BLEND
	map_block Field_Map040, .LField_Map040_MARKERS
.LField_Map040_METATILES:
	.incbin "map/map_040/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map040_SHAPES:
	.incbin "map/map_040/shapes.table.lz"
	.balign 4, 0
.LField_Map040_GRID:
	.incbin "map/map_040/grid.parts.lz"
	.balign 4, 0
.LField_Map040_ANIMATION:
	.incbin "map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map040_BLEND:
	.incbin "map/map_040/blend.script.lz"
	.balign 4, 0
.LField_Map040_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
