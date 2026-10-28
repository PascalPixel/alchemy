	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map214
Field_Map214:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map214, .LField_Map214_CELL_PICTURES
	map_block Field_Map214, .LField_Map214_SHAPES
	map_block Field_Map214, .LField_Map214_GRID
	map_block Field_Map214, .LField_Map214_SHAPE_GRID
	map_block Field_Map214, .LField_Map214_ANIMATION
	map_block Field_Map214, .LField_Map214_BLEND
	map_block Field_Map214, .LField_Map214_SCENE
.LField_Map214_CELL_PICTURES:
	.incbin "games/tla/src/map/map_214/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map214_SHAPES:
	.incbin "games/tla/src/map/map_214/shapes.table.lz"
	.balign 4, 0
.LField_Map214_GRID:
	.incbin "games/tla/src/map/map_214/grid.parts.lz"
	.balign 4, 0
.LField_Map214_SHAPE_GRID:
	.incbin "games/tla/src/map/map_214/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map214_ANIMATION:
	.incbin "games/tla/src/map/map_211/animation.script.lz"
	.balign 4, 0
.LField_Map214_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map214_SCENE:
	.incbin "games/tla/src/map/map_214/scene.parts.lz"
	.balign 4, 0
