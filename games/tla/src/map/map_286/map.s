	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map286
Field_Map286:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map286, .LField_Map286_CELL_PICTURES
	map_block Field_Map286, .LField_Map286_SHAPES
	map_block Field_Map286, .LField_Map286_GRID
	map_block Field_Map286, .LField_Map286_SHAPE_GRID
	map_block Field_Map286, .LField_Map286_ANIMATION
	map_block Field_Map286, .LField_Map286_BLEND
	map_block Field_Map286, .LField_Map286_SCENE
.LField_Map286_CELL_PICTURES:
	.incbin "games/tla/src/map/map_286/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map286_SHAPES:
	.incbin "games/tla/src/map/map_286/shapes.table.lz"
	.balign 4, 0
.LField_Map286_GRID:
	.incbin "games/tla/src/map/map_286/grid.parts.lz"
	.balign 4, 0
.LField_Map286_SHAPE_GRID:
	.incbin "games/tla/src/map/map_286/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map286_ANIMATION:
	.incbin "games/tla/src/map/map_035/animation.script.lz"
	.balign 4, 0
.LField_Map286_BLEND:
	.incbin "games/tla/src/map/map_035/blend.script.lz"
	.balign 4, 0
.LField_Map286_SCENE:
	.incbin "games/tla/src/map/map_286/scene.parts.lz"
