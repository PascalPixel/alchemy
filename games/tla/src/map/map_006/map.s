	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map006
Field_Map006:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map006, .LField_Map006_CELL_PICTURES
	map_block Field_Map006, .LField_Map006_SHAPES
	map_block Field_Map006, .LField_Map006_GRID
	map_block Field_Map006, .LField_Map006_SHAPE_GRID
	map_block Field_Map006, .LField_Map006_ANIMATION
	map_block Field_Map006, .LField_Map006_BLEND
	map_block Field_Map006, .LField_Map006_SCENE
.LField_Map006_CELL_PICTURES:
	.incbin "games/tla/src/map/map_006/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map006_SHAPES:
	.incbin "games/tla/src/map/map_006/shapes.table.lz"
	.balign 4, 0
.LField_Map006_GRID:
	.incbin "games/tla/src/map/map_006/grid.parts.lz"
	.balign 4, 0
.LField_Map006_SHAPE_GRID:
	.incbin "games/tla/src/map/map_006/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map006_ANIMATION:
	.incbin "games/tla/src/map/map_006/animation.script.lz"
	.balign 4, 0
.LField_Map006_BLEND:
	.incbin "games/tla/src/map/map_006/blend.script.lz"
	.balign 4, 0
.LField_Map006_SCENE:
	.incbin "games/tla/src/map/map_006/scene.parts.lz"
