	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map297
Field_Map297:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map297, .LField_Map297_CELL_PICTURES
	map_block Field_Map297, .LField_Map297_SHAPES
	map_block Field_Map297, .LField_Map297_GRID
	map_block Field_Map297, .LField_Map297_SHAPE_GRID
	map_block Field_Map297
	map_block Field_Map297
	map_block Field_Map297, .LField_Map297_SCENE
.LField_Map297_CELL_PICTURES:
	.incbin "games/tla/src/map/map_000/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map297_SHAPES:
	.incbin "games/tla/src/map/map_297/shapes.table.lz"
	.balign 4, 0
.LField_Map297_GRID:
	.incbin "games/tla/src/map/map_297/grid.parts.lz"
	.balign 4, 0
.LField_Map297_SHAPE_GRID:
	.incbin "games/tla/src/map/map_297/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map297_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
