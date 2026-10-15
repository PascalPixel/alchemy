	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map229
Field_Map229:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map229, .LField_Map229_METATILES
	map_block Field_Map229, .LField_Map229_SHAPES
	map_block Field_Map229, .LField_Map229_GRID
	map_block Field_Map229, .LField_Map229_SHAPE_GRID
	map_block Field_Map229, .LField_Map229_ANIMATION
	map_block Field_Map229
	map_block Field_Map229, .LField_Map229_SCENE
.LField_Map229_METATILES:
	.incbin "games/tla/src/map/map_229/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map229_SHAPES:
	.incbin "games/tla/src/map/map_229/shapes.table.lz"
	.balign 4, 0
.LField_Map229_GRID:
	.incbin "games/tla/src/map/map_229/grid.parts.lz"
	.balign 4, 0
.LField_Map229_SHAPE_GRID:
	.incbin "games/tla/src/map/map_229/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map229_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map229_SCENE:
	.incbin "games/tla/src/map/map_229/scene.parts.lz"
	.balign 4, 0
