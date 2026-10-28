	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map122
Field_Map122:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map122, .LField_Map122_CELL_PICTURES
	map_block Field_Map122, .LField_Map122_SHAPES
	map_block Field_Map122, .LField_Map122_GRID
	map_block Field_Map122, .LField_Map122_SHAPE_GRID
	map_block Field_Map122, .LField_Map122_ANIMATION
	map_block Field_Map122, .LField_Map122_BLEND
	map_block Field_Map122, .LField_Map122_SCENE
.LField_Map122_CELL_PICTURES:
	.incbin "games/tla/src/map/map_122/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map122_SHAPES:
	.incbin "games/tla/src/map/map_122/shapes.table.lz"
	.balign 4, 0
.LField_Map122_GRID:
	.incbin "games/tla/src/map/map_122/grid.parts.lz"
	.balign 4, 0
.LField_Map122_SHAPE_GRID:
	.incbin "games/tla/src/map/map_122/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map122_ANIMATION:
	.incbin "games/tla/src/map/map_122/animation.script.lz"
	.balign 4, 0
.LField_Map122_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map122_SCENE:
	.incbin "games/tla/src/map/map_122/scene.parts.lz"
