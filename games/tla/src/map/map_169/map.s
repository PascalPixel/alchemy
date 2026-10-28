	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map169
Field_Map169:
	map_header 0, 0, 130, 112, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map169, .LField_Map169_CELL_PICTURES
	map_block Field_Map169, .LField_Map169_SHAPES
	map_block Field_Map169, .LField_Map169_GRID
	map_block Field_Map169, .LField_Map169_SHAPE_GRID
	map_block Field_Map169, .LField_Map169_ANIMATION
	map_block Field_Map169, .LField_Map169_BLEND
	map_block Field_Map169, .LField_Map169_SCENE
.LField_Map169_CELL_PICTURES:
	.incbin "games/tla/src/map/map_169/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map169_SHAPES:
	.incbin "games/tla/src/map/map_169/shapes.table.lz"
	.balign 4, 0
.LField_Map169_GRID:
	.incbin "games/tla/src/map/map_169/grid.parts.lz"
	.balign 4, 0
.LField_Map169_SHAPE_GRID:
	.incbin "games/tla/src/map/map_169/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map169_ANIMATION:
	.incbin "games/tla/src/map/map_169/animation.script.lz"
	.balign 4, 0
.LField_Map169_BLEND:
	.incbin "games/tla/src/map/map_169/blend.script.lz"
	.balign 4, 0
.LField_Map169_SCENE:
	.incbin "games/tla/src/map/map_169/scene.parts.lz"
	.balign 4, 0
