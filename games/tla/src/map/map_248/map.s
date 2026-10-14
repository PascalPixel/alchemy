	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map248
Field_Map248:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map248, .LField_Map248_METATILES
	map_block Field_Map248, .LField_Map248_SHAPES
	map_block Field_Map248, .LField_Map248_GRID
	map_block Field_Map248, .LField_Map248_SHAPE_GRID
	map_block Field_Map248, .LField_Map248_ANIMATION
	map_block Field_Map248
	map_block Field_Map248, .LField_Map248_SCENE
.LField_Map248_METATILES:
	.incbin "games/tla/src/map/map_248/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map248_SHAPES:
	.incbin "games/tla/src/map/map_248/shapes.table.lz"
	.balign 4, 0
.LField_Map248_GRID:
	.incbin "games/tla/src/map/map_248/grid.parts.lz"
	.balign 4, 0
.LField_Map248_SHAPE_GRID:
	.incbin "games/tla/src/map/map_248/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map248_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map248_SCENE:
	.incbin "games/tla/src/map/map_248/scene.parts.lz"
