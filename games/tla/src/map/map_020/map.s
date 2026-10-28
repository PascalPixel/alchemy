	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map020
Field_Map020:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map020, .LField_Map020_CELL_PICTURES
	map_block Field_Map020, .LField_Map020_SHAPES
	map_block Field_Map020, .LField_Map020_GRID
	map_block Field_Map020, .LField_Map020_SHAPE_GRID
	map_block Field_Map020, .LField_Map020_ANIMATION
	map_block Field_Map020, .LField_Map020_BLEND
	map_block Field_Map020, .LField_Map020_SCENE
.LField_Map020_CELL_PICTURES:
	.incbin "games/tla/src/map/map_020/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map020_SHAPES:
	.incbin "games/tla/src/map/map_020/shapes.table.lz"
	.balign 4, 0
.LField_Map020_GRID:
	.incbin "games/tla/src/map/map_020/grid.parts.lz"
	.balign 4, 0
.LField_Map020_SHAPE_GRID:
	.incbin "games/tla/src/map/map_020/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map020_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map020_BLEND:
	.incbin "games/tla/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map020_SCENE:
	.incbin "games/tla/src/map/map_020/scene.parts.lz"
	.balign 4, 0
