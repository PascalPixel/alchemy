	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map263
Field_Map263:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map263, .LField_Map263_CELL_PICTURES
	map_block Field_Map263, .LField_Map263_SHAPES
	map_block Field_Map263, .LField_Map263_GRID
	map_block Field_Map263, .LField_Map263_SHAPE_GRID
	map_block Field_Map263, .LField_Map263_ANIMATION
	map_block Field_Map263
	map_block Field_Map263, .LField_Map263_SCENE
.LField_Map263_CELL_PICTURES:
	.incbin "games/tla/src/map/map_263/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map263_SHAPES:
	.incbin "games/tla/src/map/map_263/shapes.table.lz"
	.balign 4, 0
.LField_Map263_GRID:
	.incbin "games/tla/src/map/map_263/grid.parts.lz"
	.balign 4, 0
.LField_Map263_SHAPE_GRID:
	.incbin "games/tla/src/map/map_263/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map263_ANIMATION:
	.incbin "games/tla/src/map/map_263/animation.script.lz"
	.balign 4, 0
.LField_Map263_SCENE:
	.incbin "games/tla/src/map/map_263/scene.parts.lz"
