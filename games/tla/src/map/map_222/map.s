	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map222
Field_Map222:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map222, .LField_Map222_METATILES
	map_block Field_Map222, .LField_Map222_SHAPES
	map_block Field_Map222, .LField_Map222_GRID
	map_block Field_Map222, .LField_Map222_SHAPE_GRID
	map_block Field_Map222, .LField_Map222_ANIMATION
	map_block Field_Map222, .LField_Map222_BLEND
	map_block Field_Map222, .LField_Map222_SCENE
.LField_Map222_METATILES:
	.incbin "games/tla/src/map/map_222/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map222_SHAPES:
	.incbin "games/tla/src/map/map_222/shapes.table.lz"
	.balign 4, 0
.LField_Map222_GRID:
	.incbin "games/tla/src/map/map_222/grid.parts.lz"
	.balign 4, 0
.LField_Map222_SHAPE_GRID:
	.incbin "games/tla/src/map/map_222/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map222_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map222_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map222_SCENE:
	.incbin "games/tla/src/map/map_222/scene.parts.lz"
	.balign 4, 0
