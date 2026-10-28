	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map019
Field_Map019:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map019, .LField_Map019_CELL_PICTURES
	map_block Field_Map019, .LField_Map019_SHAPES
	map_block Field_Map019, .LField_Map019_GRID
	map_block Field_Map019, .LField_Map019_SHAPE_GRID
	map_block Field_Map019, .LField_Map019_ANIMATION
	map_block Field_Map019
	map_block Field_Map019, .LField_Map019_SCENE
.LField_Map019_CELL_PICTURES:
	.incbin "games/tla/src/map/map_019/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map019_SHAPES:
	.incbin "games/tla/src/map/map_019/shapes.table.lz"
	.balign 4, 0
.LField_Map019_GRID:
	.incbin "games/tla/src/map/map_019/grid.parts.lz"
	.balign 4, 0
.LField_Map019_SHAPE_GRID:
	.incbin "games/tla/src/map/map_019/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map019_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map019_SCENE:
	.incbin "games/tla/src/map/map_019/scene.parts.lz"
