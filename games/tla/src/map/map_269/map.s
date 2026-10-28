	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map269
Field_Map269:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map269, .LField_Map269_CELL_PICTURES
	map_block Field_Map269, .LField_Map269_SHAPES
	map_block Field_Map269, .LField_Map269_GRID
	map_block Field_Map269, .LField_Map269_SHAPE_GRID
	map_block Field_Map269, .LField_Map269_ANIMATION
	map_block Field_Map269, .LField_Map269_BLEND
	map_block Field_Map269, .LField_Map269_SCENE
.LField_Map269_CELL_PICTURES:
	.incbin "games/tla/src/map/map_269/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map269_SHAPES:
	.incbin "games/tla/src/map/map_269/shapes.table.lz"
	.balign 4, 0
.LField_Map269_GRID:
	.incbin "games/tla/src/map/map_269/grid.parts.lz"
	.balign 4, 0
.LField_Map269_SHAPE_GRID:
	.incbin "games/tla/src/map/map_269/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map269_ANIMATION:
	.incbin "games/tla/src/map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map269_BLEND:
	.incbin "games/tla/src/map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map269_SCENE:
	.incbin "games/tla/src/map/map_269/scene.parts.lz"
