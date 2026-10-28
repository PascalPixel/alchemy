	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map005
Field_Map005:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map005, .LField_Map005_CELL_PICTURES
	map_block Field_Map005, .LField_Map005_SHAPES
	map_block Field_Map005, .LField_Map005_GRID
	map_block Field_Map005, .LField_Map005_SHAPE_GRID
	map_block Field_Map005, .LField_Map005_ANIMATION
	map_block Field_Map005, .LField_Map005_BLEND
	map_block Field_Map005, .LField_Map005_SCENE
.LField_Map005_CELL_PICTURES:
	.incbin "games/tla/src/map/map_005/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map005_SHAPES:
	.incbin "games/tla/src/map/map_005/shapes.table.lz"
	.balign 4, 0
.LField_Map005_GRID:
	.incbin "games/tla/src/map/map_005/grid.parts.lz"
	.balign 4, 0
.LField_Map005_SHAPE_GRID:
	.incbin "games/tla/src/map/map_005/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map005_ANIMATION:
	.incbin "games/tla/src/map/map_005/animation.script.lz"
	.balign 4, 0
.LField_Map005_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map005_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
