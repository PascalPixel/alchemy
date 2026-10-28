	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map291
Field_Map291:
	map_header 0, 0, 100, 64, 3, 1, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map291, .LField_Map291_CELL_PICTURES
	map_block Field_Map291, .LField_Map291_SHAPES
	map_block Field_Map291, .LField_Map291_GRID
	map_block Field_Map291, .LField_Map291_SHAPE_GRID
	map_block Field_Map291, .LField_Map291_ANIMATION
	map_block Field_Map291, .LField_Map291_BLEND
	map_block Field_Map291, .LField_Map291_SCENE
.LField_Map291_CELL_PICTURES:
	.incbin "games/tla/src/map/map_291/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map291_SHAPES:
	.incbin "games/tla/src/map/map_291/shapes.table.lz"
	.balign 4, 0
.LField_Map291_GRID:
	.incbin "games/tla/src/map/map_291/grid.parts.lz"
	.balign 4, 0
.LField_Map291_SHAPE_GRID:
	.incbin "games/tla/src/map/map_291/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map291_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map291_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map291_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
