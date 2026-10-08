	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map050
Field_Map050:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map050, .LField_Map050_METATILES
	map_block Field_Map050, .LField_Map050_SHAPES
	map_block Field_Map050, .LField_Map050_GRID
	map_block Field_Map050, .LField_Map050_ANIMATION
	map_block Field_Map050
	map_block Field_Map050, .LField_Map050_MARKERS
.LField_Map050_METATILES:
	.incbin "map/map_050/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map050_SHAPES:
	.incbin "map/map_050/shapes.table.lz"
	.balign 4, 0
.LField_Map050_GRID:
	.incbin "map/map_050/grid.parts.lz"
	.balign 4, 0
.LField_Map050_ANIMATION:
	.incbin "map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map050_MARKERS:
	.incbin "map/map_050/markers.table"
