	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map028
Field_Map028:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map028, .LField_Map028_METATILES
	map_block Field_Map028, .LField_Map028_SHAPES
	map_block Field_Map028, .LField_Map028_GRID
	map_block Field_Map028, .LField_Map028_SHAPE_GRID
	map_block Field_Map028, .LField_Map028_ANIMATION
	map_block Field_Map028, .LField_Map028_BLEND
	map_block Field_Map028, .LField_Map028_SCENE
.LField_Map028_METATILES:
	.incbin "games/tla/src/map/map_028/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map028_SHAPES:
	.incbin "games/tla/src/map/map_028/shapes.table.lz"
	.balign 4, 0
.LField_Map028_GRID:
	.incbin "games/tla/src/map/map_028/grid.parts.lz"
	.balign 4, 0
.LField_Map028_SHAPE_GRID:
	.incbin "games/tla/src/map/map_028/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map028_ANIMATION:
	.incbin "games/tla/src/map/map_028/animation.script.lz"
	.balign 4, 0
.LField_Map028_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map028_SCENE:
	.incbin "games/tla/src/map/map_028/scene.parts.lz"
