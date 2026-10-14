	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map232
Field_Map232:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map232, .LField_Map232_METATILES
	map_block Field_Map232, .LField_Map232_SHAPES
	map_block Field_Map232, .LField_Map232_GRID
	map_block Field_Map232, .LField_Map232_SHAPE_GRID
	map_block Field_Map232, .LField_Map232_ANIMATION
	map_block Field_Map232, .LField_Map232_BLEND
	map_block Field_Map232, .LField_Map232_SCENE
.LField_Map232_METATILES:
	.incbin "games/tla/src/map/map_232/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map232_SHAPES:
	.incbin "games/tla/src/map/map_232/shapes.table.lz"
	.balign 4, 0
.LField_Map232_GRID:
	.incbin "games/tla/src/map/map_232/grid.parts.lz"
	.balign 4, 0
.LField_Map232_SHAPE_GRID:
	.incbin "games/tla/src/map/map_232/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map232_ANIMATION:
	.incbin "games/tla/src/map/map_182/animation.script.lz"
	.balign 4, 0
.LField_Map232_BLEND:
	.incbin "games/tla/src/map/map_232/blend.script.lz"
	.balign 4, 0
.LField_Map232_SCENE:
	.incbin "games/tla/src/map/map_232/scene.parts.lz"
