	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map094
Field_Map094:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map094, .LField_Map094_CELL_PICTURES
	map_block Field_Map094, .LField_Map094_SHAPES
	map_block Field_Map094, .LField_Map094_GRID
	map_block Field_Map094, .LField_Map094_SHAPE_GRID
	map_block Field_Map094, .LField_Map094_ANIMATION
	map_block Field_Map094
	map_block Field_Map094, .LField_Map094_SCENE
.LField_Map094_CELL_PICTURES:
	.incbin "games/tla/src/map/map_094/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map094_SHAPES:
	.incbin "games/tla/src/map/map_094/shapes.table.lz"
	.balign 4, 0
.LField_Map094_GRID:
	.incbin "games/tla/src/map/map_094/grid.parts.lz"
	.balign 4, 0
.LField_Map094_SHAPE_GRID:
	.incbin "games/tla/src/map/map_094/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map094_ANIMATION:
	.incbin "games/tla/src/map/map_021/animation.script.lz"
	.balign 4, 0
.LField_Map094_SCENE:
	.incbin "games/tla/src/map/map_094/scene.parts.lz"
