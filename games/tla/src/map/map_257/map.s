	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map257
Field_Map257:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map257, .LField_Map257_METATILES
	map_block Field_Map257, .LField_Map257_SHAPES
	map_block Field_Map257, .LField_Map257_GRID
	map_block Field_Map257, .LField_Map257_SHAPE_GRID
	map_block Field_Map257, .LField_Map257_ANIMATION
	map_block Field_Map257, .LField_Map257_BLEND
	map_block Field_Map257, .LField_Map257_SCENE
.LField_Map257_METATILES:
	.incbin "games/tla/src/map/map_257/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map257_SHAPES:
	.incbin "games/tla/src/map/map_257/shapes.table.lz"
	.balign 4, 0
.LField_Map257_GRID:
	.incbin "games/tla/src/map/map_257/grid.parts.lz"
	.balign 4, 0
.LField_Map257_SHAPE_GRID:
	.incbin "games/tla/src/map/map_257/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map257_ANIMATION:
	.incbin "games/common/src/map/map_052/animation.script.lz"
	.balign 4, 0
.LField_Map257_BLEND:
	.incbin "games/common/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map257_SCENE:
	.incbin "games/tla/src/map/map_257/scene.parts.lz"
	.balign 4, 0
