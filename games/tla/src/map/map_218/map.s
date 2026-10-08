	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map218
Field_Map218:
	map_header 0, 0, 104, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 104, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map218, .LField_Map218_METATILES
	map_block Field_Map218, .LField_Map218_SHAPES
	map_block Field_Map218, .LField_Map218_GRID
	map_block Field_Map218, .LField_Map218_SHAPE_GRID
	map_block Field_Map218, .LField_Map218_ANIMATION
	map_block Field_Map218, .LField_Map218_BLEND
	map_block Field_Map218, .LField_Map218_SCENE
.LField_Map218_METATILES:
	.incbin "map/map_218/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map218_SHAPES:
	.incbin "map/map_218/shapes.table.lz"
	.balign 4, 0
.LField_Map218_GRID:
	.incbin "map/map_218/grid.parts.lz"
	.balign 4, 0
.LField_Map218_SHAPE_GRID:
	.incbin "map/map_218/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map218_ANIMATION:
	.incbin "map/map_218/animation.script.lz"
	.balign 4, 0
.LField_Map218_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map218_SCENE:
	.incbin "map/map_218/scene.parts.lz"
