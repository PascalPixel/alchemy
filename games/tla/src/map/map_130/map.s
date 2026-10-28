	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map130
Field_Map130:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map130, .LField_Map130_CELL_PICTURES
	map_block Field_Map130, .LField_Map130_SHAPES
	map_block Field_Map130, .LField_Map130_GRID
	map_block Field_Map130, .LField_Map130_SHAPE_GRID
	map_block Field_Map130, .LField_Map130_ANIMATION
	map_block Field_Map130
	map_block Field_Map130, .LField_Map130_SCENE
.LField_Map130_CELL_PICTURES:
	.incbin "games/tla/src/map/map_130/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map130_SHAPES:
	.incbin "games/tla/src/map/map_130/shapes.table.lz"
	.balign 4, 0
.LField_Map130_GRID:
	.incbin "games/tla/src/map/map_130/grid.parts.lz"
	.balign 4, 0
.LField_Map130_SHAPE_GRID:
	.incbin "games/tla/src/map/map_130/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map130_ANIMATION:
	.incbin "games/tla/src/map/map_130/animation.script.lz"
	.balign 4, 0
.LField_Map130_SCENE:
	.incbin "games/tla/src/map/map_130/scene.parts.lz"
	.balign 4, 0
