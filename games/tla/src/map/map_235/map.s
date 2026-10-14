	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map235
Field_Map235:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map235, .LField_Map235_METATILES
	map_block Field_Map235, .LField_Map235_SHAPES
	map_block Field_Map235, .LField_Map235_GRID
	map_block Field_Map235, .LField_Map235_SHAPE_GRID
	map_block Field_Map235, .LField_Map235_ANIMATION
	map_block Field_Map235, .LField_Map235_BLEND
	map_block Field_Map235, .LField_Map235_SCENE
.LField_Map235_METATILES:
	.incbin "games/tla/src/map/map_235/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map235_SHAPES:
	.incbin "games/tla/src/map/map_235/shapes.table.lz"
	.balign 4, 0
.LField_Map235_GRID:
	.incbin "games/tla/src/map/map_235/grid.parts.lz"
	.balign 4, 0
.LField_Map235_SHAPE_GRID:
	.incbin "games/tla/src/map/map_235/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map235_ANIMATION:
	.incbin "games/tla/src/map/map_235/animation.script.lz"
	.balign 4, 0
.LField_Map235_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map235_SCENE:
	.incbin "games/tla/src/map/map_235/scene.parts.lz"
