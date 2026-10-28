	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map139
Field_Map139:
	map_header 0, 0, 90, 100, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map139, .LField_Map139_CELL_PICTURES
	map_block Field_Map139, .LField_Map139_SHAPES
	map_block Field_Map139, .LField_Map139_GRID
	map_block Field_Map139, .LField_Map139_SHAPE_GRID
	map_block Field_Map139, .LField_Map139_ANIMATION
	map_block Field_Map139, .LField_Map139_BLEND
	map_block Field_Map139, .LField_Map139_SCENE
.LField_Map139_CELL_PICTURES:
	.incbin "games/tla/src/map/map_139/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map139_SHAPES:
	.incbin "games/tla/src/map/map_139/shapes.table.lz"
	.balign 4, 0
.LField_Map139_GRID:
	.incbin "games/tla/src/map/map_139/grid.parts.lz"
	.balign 4, 0
.LField_Map139_SHAPE_GRID:
	.incbin "games/tla/src/map/map_139/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map139_ANIMATION:
	.incbin "games/tla/src/map/map_139/animation.script.lz"
	.balign 4, 0
.LField_Map139_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map139_SCENE:
	.incbin "games/tla/src/map/map_139/scene.parts.lz"
