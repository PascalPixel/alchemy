	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map268
Field_Map268:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map268, .LField_Map268_CELL_PICTURES
	map_block Field_Map268, .LField_Map268_SHAPES
	map_block Field_Map268, .LField_Map268_GRID
	map_block Field_Map268, .LField_Map268_SHAPE_GRID
	map_block Field_Map268, .LField_Map268_ANIMATION
	map_block Field_Map268, .LField_Map268_BLEND
	map_block Field_Map268, .LField_Map268_SCENE
.LField_Map268_CELL_PICTURES:
	.incbin "games/tla/src/map/map_268/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map268_SHAPES:
	.incbin "games/tla/src/map/map_268/shapes.table.lz"
	.balign 4, 0
.LField_Map268_GRID:
	.incbin "games/tla/src/map/map_268/grid.parts.lz"
	.balign 4, 0
.LField_Map268_SHAPE_GRID:
	.incbin "games/tla/src/map/map_268/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map268_ANIMATION:
	.incbin "games/tla/src/map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map268_BLEND:
	.incbin "games/tla/src/map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map268_SCENE:
	.incbin "games/tla/src/map/map_268/scene.parts.lz"
	.balign 4, 0
