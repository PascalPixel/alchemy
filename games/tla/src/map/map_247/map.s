	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map247
Field_Map247:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map247, .LField_Map247_CELL_PICTURES
	map_block Field_Map247, .LField_Map247_SHAPES
	map_block Field_Map247, .LField_Map247_GRID
	map_block Field_Map247, .LField_Map247_SHAPE_GRID
	map_block Field_Map247, .LField_Map247_ANIMATION
	map_block Field_Map247
	map_block Field_Map247, .LField_Map247_SCENE
.LField_Map247_CELL_PICTURES:
	.incbin "games/tla/src/map/map_247/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map247_SHAPES:
	.incbin "games/tla/src/map/map_247/shapes.table.lz"
	.balign 4, 0
.LField_Map247_GRID:
	.incbin "games/tla/src/map/map_247/grid.parts.lz"
	.balign 4, 0
.LField_Map247_SHAPE_GRID:
	.incbin "games/tla/src/map/map_247/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map247_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map247_SCENE:
	.incbin "games/tla/src/map/map_247/scene.parts.lz"
	.balign 4, 0
