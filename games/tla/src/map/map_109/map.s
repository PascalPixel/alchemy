	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map109
Field_Map109:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map109, .LField_Map109_METATILES
	map_block Field_Map109, .LField_Map109_SHAPES
	map_block Field_Map109, .LField_Map109_GRID
	map_block Field_Map109, .LField_Map109_SHAPE_GRID
	map_block Field_Map109, .LField_Map109_ANIMATION
	map_block Field_Map109, .LField_Map109_BLEND
	map_block Field_Map109, .LField_Map109_SCENE
.LField_Map109_METATILES:
	.incbin "games/tla/src/map/map_109/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map109_SHAPES:
	.incbin "games/tla/src/map/map_109/shapes.table.lz"
	.balign 4, 0
.LField_Map109_GRID:
	.incbin "games/tla/src/map/map_109/grid.parts.lz"
	.balign 4, 0
.LField_Map109_SHAPE_GRID:
	.incbin "games/tla/src/map/map_109/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map109_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map109_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map109_SCENE:
	.incbin "games/tla/src/map/map_109/scene.parts.lz"
