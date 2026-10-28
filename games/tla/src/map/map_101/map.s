	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map101
Field_Map101:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map101, .LField_Map101_CELL_PICTURES
	map_block Field_Map101, .LField_Map101_SHAPES
	map_block Field_Map101, .LField_Map101_GRID
	map_block Field_Map101, .LField_Map101_SHAPE_GRID
	map_block Field_Map101, .LField_Map101_ANIMATION
	map_block Field_Map101, .LField_Map101_BLEND
	map_block Field_Map101, .LField_Map101_SCENE
.LField_Map101_CELL_PICTURES:
	.incbin "games/tla/src/map/map_101/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map101_SHAPES:
	.incbin "games/tla/src/map/map_101/shapes.table.lz"
	.balign 4, 0
.LField_Map101_GRID:
	.incbin "games/tla/src/map/map_101/grid.parts.lz"
	.balign 4, 0
.LField_Map101_SHAPE_GRID:
	.incbin "games/tla/src/map/map_101/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map101_ANIMATION:
	.incbin "games/tla/src/map/map_035/animation.script.lz"
	.balign 4, 0
.LField_Map101_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map101_SCENE:
	.incbin "games/tla/src/map/map_101/scene.parts.lz"
	.balign 4, 0
