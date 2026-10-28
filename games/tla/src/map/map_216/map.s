	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map216
Field_Map216:
	map_header 0, 0, 130, 112, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map216, .LField_Map216_CELL_PICTURES
	map_block Field_Map216, .LField_Map216_SHAPES
	map_block Field_Map216, .LField_Map216_GRID
	map_block Field_Map216, .LField_Map216_SHAPE_GRID
	map_block Field_Map216, .LField_Map216_ANIMATION
	map_block Field_Map216, .LField_Map216_BLEND
	map_block Field_Map216, .LField_Map216_SCENE
.LField_Map216_CELL_PICTURES:
	.incbin "games/tla/src/map/map_216/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map216_SHAPES:
	.incbin "games/tla/src/map/map_216/shapes.table.lz"
	.balign 4, 0
.LField_Map216_GRID:
	.incbin "games/tla/src/map/map_216/grid.parts.lz"
	.balign 4, 0
.LField_Map216_SHAPE_GRID:
	.incbin "games/tla/src/map/map_216/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map216_ANIMATION:
	.incbin "games/tla/src/map/map_216/animation.script.lz"
	.balign 4, 0
.LField_Map216_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map216_SCENE:
	.incbin "games/tla/src/map/map_216/scene.parts.lz"
	.balign 4, 0
