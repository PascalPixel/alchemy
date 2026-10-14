	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map147
Field_Map147:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map147, .LField_Map147_METATILES
	map_block Field_Map147, .LField_Map147_SHAPES
	map_block Field_Map147, .LField_Map147_GRID
	map_block Field_Map147, .LField_Map147_SHAPE_GRID
	map_block Field_Map147, .LField_Map147_ANIMATION
	map_block Field_Map147, .LField_Map147_BLEND
	map_block Field_Map147, .LField_Map147_SCENE
.LField_Map147_METATILES:
	.incbin "games/tla/src/map/map_147/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map147_SHAPES:
	.incbin "games/tla/src/map/map_147/shapes.table.lz"
	.balign 4, 0
.LField_Map147_GRID:
	.incbin "games/tla/src/map/map_147/grid.parts.lz"
	.balign 4, 0
.LField_Map147_SHAPE_GRID:
	.incbin "games/tla/src/map/map_147/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map147_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map147_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map147_SCENE:
	.incbin "games/tla/src/map/map_147/scene.parts.lz"
