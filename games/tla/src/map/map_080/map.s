	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map080
Field_Map080:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map080, .LField_Map080_METATILES
	map_block Field_Map080, .LField_Map080_SHAPES
	map_block Field_Map080, .LField_Map080_GRID
	map_block Field_Map080, .LField_Map080_SHAPE_GRID
	map_block Field_Map080, .LField_Map080_ANIMATION
	map_block Field_Map080, .LField_Map080_BLEND
	map_block Field_Map080, .LField_Map080_SCENE
.LField_Map080_METATILES:
	.incbin "games/tla/src/map/map_080/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map080_SHAPES:
	.incbin "games/tla/src/map/map_080/shapes.table.lz"
	.balign 4, 0
.LField_Map080_GRID:
	.incbin "games/tla/src/map/map_080/grid.parts.lz"
	.balign 4, 0
.LField_Map080_SHAPE_GRID:
	.incbin "games/tla/src/map/map_080/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map080_ANIMATION:
	.incbin "games/tla/src/map/map_078/animation.script.lz"
	.balign 4, 0
.LField_Map080_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map080_SCENE:
	.incbin "games/tla/src/map/map_080/scene.parts.lz"
