	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map079
Field_Map079:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map079, .LField_Map079_METATILES
	map_block Field_Map079, .LField_Map079_SHAPES
	map_block Field_Map079, .LField_Map079_GRID
	map_block Field_Map079, .LField_Map079_SHAPE_GRID
	map_block Field_Map079, .LField_Map079_ANIMATION
	map_block Field_Map079, .LField_Map079_BLEND
	map_block Field_Map079, .LField_Map079_SCENE
.LField_Map079_METATILES:
	.incbin "games/tla/src/map/map_079/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map079_SHAPES:
	.incbin "games/tla/src/map/map_079/shapes.table.lz"
	.balign 4, 0
.LField_Map079_GRID:
	.incbin "games/tla/src/map/map_079/grid.parts.lz"
	.balign 4, 0
.LField_Map079_SHAPE_GRID:
	.incbin "games/tla/src/map/map_079/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map079_ANIMATION:
	.incbin "games/tla/src/map/map_078/animation.script.lz"
	.balign 4, 0
.LField_Map079_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map079_SCENE:
	.incbin "games/tla/src/map/map_079/scene.parts.lz"
