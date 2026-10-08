	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map083
Field_Map083:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map083, .LField_Map083_METATILES
	map_block Field_Map083, .LField_Map083_SHAPES
	map_block Field_Map083, .LField_Map083_GRID
	map_block Field_Map083, .LField_Map083_ANIMATION
	map_block Field_Map083, .LField_Map083_BLEND
	map_block Field_Map083, .LField_Map083_MARKERS
.LField_Map083_METATILES:
	.incbin "map/map_083/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map083_SHAPES:
	.incbin "map/map_083/shapes.table.lz"
	.balign 4, 0
.LField_Map083_GRID:
	.incbin "map/map_083/grid.parts.lz"
	.balign 4, 0
.LField_Map083_ANIMATION:
	.incbin "map/map_080/animation.script.lz"
	.balign 4, 0
.LField_Map083_BLEND:
	.incbin "map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map083_MARKERS:
	.incbin "map/map_083/markers.table"
	.balign 4, 0
