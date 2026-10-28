	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map239
Field_Map239:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map239, .LField_Map239_CELL_PICTURES
	map_block Field_Map239, .LField_Map239_SHAPES
	map_block Field_Map239, .LField_Map239_GRID
	map_block Field_Map239, .LField_Map239_SHAPE_GRID
	map_block Field_Map239, .LField_Map239_ANIMATION
	map_block Field_Map239, .LField_Map239_BLEND
	map_block Field_Map239, .LField_Map239_SCENE
.LField_Map239_CELL_PICTURES:
	.incbin "games/tla/src/map/map_239/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map239_SHAPES:
	.incbin "games/tla/src/map/map_239/shapes.table.lz"
	.balign 4, 0
.LField_Map239_GRID:
	.incbin "games/tla/src/map/map_239/grid.parts.lz"
	.balign 4, 0
.LField_Map239_SHAPE_GRID:
	.incbin "games/tla/src/map/map_239/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map239_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map239_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map239_SCENE:
	.incbin "games/tla/src/map/map_239/scene.parts.lz"
