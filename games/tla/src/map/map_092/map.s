	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map092
Field_Map092:
	map_header 0, 0, 90, 90, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map092, .LField_Map092_CELL_PICTURES
	map_block Field_Map092, .LField_Map092_SHAPES
	map_block Field_Map092, .LField_Map092_GRID
	map_block Field_Map092, .LField_Map092_SHAPE_GRID
	map_block Field_Map092, .LField_Map092_ANIMATION
	map_block Field_Map092, .LField_Map092_BLEND
	map_block Field_Map092, .LField_Map092_SCENE
.LField_Map092_CELL_PICTURES:
	.incbin "games/tla/src/map/map_092/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map092_SHAPES:
	.incbin "games/tla/src/map/map_092/shapes.table.lz"
	.balign 4, 0
.LField_Map092_GRID:
	.incbin "games/tla/src/map/map_092/grid.parts.lz"
	.balign 4, 0
.LField_Map092_SHAPE_GRID:
	.incbin "games/tla/src/map/map_092/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map092_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map092_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map092_SCENE:
	.incbin "games/tla/src/map/map_092/scene.parts.lz"
	.balign 4, 0
