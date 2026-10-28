	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map002
Field_Map002:
	map_header 0, 0, 64, 68, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map002, .LField_Map002_CELL_PICTURES
	map_block Field_Map002, .LField_Map002_SHAPES
	map_block Field_Map002, .LField_Map002_GRID
	map_block Field_Map002, .LField_Map002_SHAPE_GRID
	map_block Field_Map002, .LField_Map002_ANIMATION
	map_block Field_Map002, .LField_Map002_BLEND
	map_block Field_Map002, .LField_Map002_SCENE
.LField_Map002_CELL_PICTURES:
	.incbin "games/tla/src/map/map_002/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map002_SHAPES:
	.incbin "games/tla/src/map/map_002/shapes.table.lz"
	.balign 4, 0
.LField_Map002_GRID:
	.incbin "games/tla/src/map/map_002/grid.parts.lz"
	.balign 4, 0
.LField_Map002_SHAPE_GRID:
	.incbin "games/tla/src/map/map_002/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map002_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map002_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map002_SCENE:
	.incbin "games/tla/src/map/map_002/scene.parts.lz"
	.balign 4, 0
