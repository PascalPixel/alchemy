	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map118
Field_Map118:
	map_header 0, 0, 90, 100, 3, 3, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map118, .LField_Map118_METATILES
	map_block Field_Map118, .LField_Map118_SHAPES
	map_block Field_Map118, .LField_Map118_GRID
	map_block Field_Map118, .LField_Map118_SHAPE_GRID
	map_block Field_Map118, .LField_Map118_ANIMATION
	map_block Field_Map118, .LField_Map118_BLEND
	map_block Field_Map118, .LField_Map118_SCENE
.LField_Map118_METATILES:
	.incbin "map/map_118/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map118_SHAPES:
	.incbin "map/map_118/shapes.table.lz"
	.balign 4, 0
.LField_Map118_GRID:
	.incbin "map/map_118/grid.parts.lz"
	.balign 4, 0
.LField_Map118_SHAPE_GRID:
	.incbin "map/map_118/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map118_ANIMATION:
	.incbin "map/map_021/animation.script.lz"
	.balign 4, 0
.LField_Map118_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map118_SCENE:
	.incbin "map/map_118/scene.parts.lz"
