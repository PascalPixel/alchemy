	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map209
Field_Map209:
	map_header 0, 0, 128, 128, 2, 2, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map209, .LField_Map209_CELL_PICTURES
	map_block Field_Map209, .LField_Map209_SHAPES
	map_block Field_Map209, .LField_Map209_GRID
	map_block Field_Map209, .LField_Map209_SHAPE_GRID
	map_block Field_Map209, .LField_Map209_ANIMATION
	map_block Field_Map209, .LField_Map209_BLEND
	map_block Field_Map209, .LField_Map209_SCENE
.LField_Map209_CELL_PICTURES:
	.incbin "games/tla/src/map/map_209/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map209_SHAPES:
	.incbin "games/tla/src/map/map_209/shapes.table.lz"
	.balign 4, 0
.LField_Map209_GRID:
	.incbin "games/tla/src/map/map_209/grid.parts.lz"
	.balign 4, 0
.LField_Map209_SHAPE_GRID:
	.incbin "games/tla/src/map/map_209/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map209_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map209_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map209_SCENE:
	.incbin "games/tla/src/map/map_209/scene.parts.lz"
	.balign 4, 0
