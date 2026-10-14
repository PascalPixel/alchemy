	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map031
Field_Map031:
	map_header 0, 0, 128, 128, 2, 2, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 12, 12, 0, 0, 0xff, 0xff
	map_block Field_Map031, .LField_Map031_METATILES
	map_block Field_Map031, .LField_Map031_SHAPES
	map_block Field_Map031, .LField_Map031_GRID
	map_block Field_Map031, .LField_Map031_SHAPE_GRID
	map_block Field_Map031, .LField_Map031_ANIMATION
	map_block Field_Map031
	map_block Field_Map031, .LField_Map031_SCENE
.LField_Map031_METATILES:
	.incbin "games/tla/src/map/map_031/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map031_SHAPES:
	.incbin "games/tla/src/map/map_031/shapes.table.lz"
	.balign 4, 0
.LField_Map031_GRID:
	.incbin "games/tla/src/map/map_031/grid.parts.lz"
	.balign 4, 0
.LField_Map031_SHAPE_GRID:
	.incbin "games/tla/src/map/map_031/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map031_ANIMATION:
	.incbin "games/tla/src/map/map_031/animation.script.lz"
	.balign 4, 0
.LField_Map031_SCENE:
	.incbin "games/tla/src/map/map_031/scene.parts.lz"
