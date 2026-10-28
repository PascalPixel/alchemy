	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map143
Field_Map143:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map143, .LField_Map143_CELL_PICTURES
	map_block Field_Map143, .LField_Map143_SHAPES
	map_block Field_Map143, .LField_Map143_GRID
	map_block Field_Map143, .LField_Map143_SHAPE_GRID
	map_block Field_Map143, .LField_Map143_ANIMATION
	map_block Field_Map143, .LField_Map143_BLEND
	map_block Field_Map143, .LField_Map143_SCENE
.LField_Map143_CELL_PICTURES:
	.incbin "games/tla/src/map/map_143/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map143_SHAPES:
	.incbin "games/tla/src/map/map_143/shapes.table.lz"
	.balign 4, 0
.LField_Map143_GRID:
	.incbin "games/tla/src/map/map_143/grid.parts.lz"
	.balign 4, 0
.LField_Map143_SHAPE_GRID:
	.incbin "games/tla/src/map/map_143/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map143_ANIMATION:
	.incbin "games/tla/src/map/map_142/animation.script.lz"
	.balign 4, 0
.LField_Map143_BLEND:
	.incbin "games/tla/src/map/map_143/blend.script.lz"
	.balign 4, 0
.LField_Map143_SCENE:
	.incbin "games/tla/src/map/map_143/scene.parts.lz"
	.balign 4, 0
