	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map241
Field_Map241:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map241, .LField_Map241_METATILES
	map_block Field_Map241, .LField_Map241_SHAPES
	map_block Field_Map241, .LField_Map241_GRID
	map_block Field_Map241, .LField_Map241_SHAPE_GRID
	map_block Field_Map241, .LField_Map241_ANIMATION
	map_block Field_Map241
	map_block Field_Map241, .LField_Map241_SCENE
.LField_Map241_METATILES:
	.incbin "games/tla/src/map/map_241/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map241_SHAPES:
	.incbin "games/tla/src/map/map_241/shapes.table.lz"
	.balign 4, 0
.LField_Map241_GRID:
	.incbin "games/tla/src/map/map_241/grid.parts.lz"
	.balign 4, 0
.LField_Map241_SHAPE_GRID:
	.incbin "games/tla/src/map/map_241/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map241_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map241_SCENE:
	.incbin "games/tla/src/map/map_241/scene.parts.lz"
	.balign 4, 0
