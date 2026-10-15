	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map120
Field_Map120:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map120, .LField_Map120_METATILES
	map_block Field_Map120, .LField_Map120_SHAPES
	map_block Field_Map120, .LField_Map120_GRID
	map_block Field_Map120, .LField_Map120_SHAPE_GRID
	map_block Field_Map120, .LField_Map120_ANIMATION
	map_block Field_Map120, .LField_Map120_BLEND
	map_block Field_Map120, .LField_Map120_SCENE
.LField_Map120_METATILES:
	.incbin "games/tla/src/map/map_120/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map120_SHAPES:
	.incbin "games/tla/src/map/map_120/shapes.table.lz"
	.balign 4, 0
.LField_Map120_GRID:
	.incbin "games/tla/src/map/map_120/grid.parts.lz"
	.balign 4, 0
.LField_Map120_SHAPE_GRID:
	.incbin "games/tla/src/map/map_120/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map120_ANIMATION:
	.incbin "games/tla/src/map/map_021/animation.script.lz"
	.balign 4, 0
.LField_Map120_BLEND:
	.incbin "games/tla/src/map/map_021/blend.script.lz"
	.balign 4, 0
.LField_Map120_SCENE:
	.incbin "games/tla/src/map/map_120/scene.parts.lz"
	.balign 4, 0
