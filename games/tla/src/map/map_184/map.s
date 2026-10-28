	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map184
Field_Map184:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map184, .LField_Map184_CELL_PICTURES
	map_block Field_Map184, .LField_Map184_SHAPES
	map_block Field_Map184, .LField_Map184_GRID
	map_block Field_Map184, .LField_Map184_SHAPE_GRID
	map_block Field_Map184, .LField_Map184_ANIMATION
	map_block Field_Map184, .LField_Map184_BLEND
	map_block Field_Map184, .LField_Map184_SCENE
.LField_Map184_CELL_PICTURES:
	.incbin "games/tla/src/map/map_184/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map184_SHAPES:
	.incbin "games/tla/src/map/map_184/shapes.table.lz"
	.balign 4, 0
.LField_Map184_GRID:
	.incbin "games/tla/src/map/map_184/grid.parts.lz"
	.balign 4, 0
.LField_Map184_SHAPE_GRID:
	.incbin "games/tla/src/map/map_184/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map184_ANIMATION:
	.incbin "games/tla/src/map/map_182/animation.script.lz"
	.balign 4, 0
.LField_Map184_BLEND:
	.incbin "games/tla/src/map/map_030/blend.script.lz"
	.balign 4, 0
.LField_Map184_SCENE:
	.incbin "games/tla/src/map/map_184/scene.parts.lz"
	.balign 4, 0
