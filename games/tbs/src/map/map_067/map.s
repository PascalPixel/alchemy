	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map067
Field_Map067:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map067, .LField_Map067_METATILES
	map_block Field_Map067, .LField_Map067_SHAPES
	map_block Field_Map067, .LField_Map067_GRID
	map_block Field_Map067, .LField_Map067_ANIMATION
	map_block Field_Map067, .LField_Map067_BLEND
	map_block Field_Map067, .LField_Map067_MARKERS
.LField_Map067_METATILES:
	.incbin "games/tbs/src/map/map_067/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map067_SHAPES:
	.incbin "games/tbs/src/map/map_067/shapes.table.lz"
	.balign 4, 0
.LField_Map067_GRID:
	.incbin "games/tbs/src/map/map_067/grid.parts.lz"
	.balign 4, 0
.LField_Map067_ANIMATION:
	.incbin "games/tbs/src/map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map067_BLEND:
	.incbin "games/tbs/src/map/map_063/blend.script.lz"
	.balign 4, 0
.LField_Map067_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
