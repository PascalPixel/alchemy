	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map197
Field_Map197:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map197, .LField_Map197_METATILES
	map_block Field_Map197, .LField_Map197_SHAPES
	map_block Field_Map197, .LField_Map197_GRID
	map_block Field_Map197, .LField_Map197_SHAPE_GRID
	map_block Field_Map197, .LField_Map197_ANIMATION
	map_block Field_Map197, .LField_Map197_BLEND
	map_block Field_Map197, .LField_Map197_SCENE
.LField_Map197_METATILES:
	.incbin "games/tla/src/map/map_197/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map197_SHAPES:
	.incbin "games/tla/src/map/map_197/shapes.table.lz"
	.balign 4, 0
.LField_Map197_GRID:
	.incbin "games/tla/src/map/map_197/grid.parts.lz"
	.balign 4, 0
.LField_Map197_SHAPE_GRID:
	.incbin "games/tla/src/map/map_197/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map197_ANIMATION:
	.incbin "games/tla/src/map/map_195/animation.script.lz"
	.balign 4, 0
.LField_Map197_BLEND:
	.incbin "games/tla/src/map/map_196/blend.script.lz"
	.balign 4, 0
.LField_Map197_SCENE:
	.incbin "games/tla/src/map/map_197/scene.parts.lz"
