	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map015
Field_Map015:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map015, .LField_Map015_METATILES
	map_block Field_Map015, .LField_Map015_SHAPES
	map_block Field_Map015, .LField_Map015_GRID
	map_block Field_Map015, .LField_Map015_SHAPE_GRID
	map_block Field_Map015, .LField_Map015_ANIMATION
	map_block Field_Map015, .LField_Map015_BLEND
	map_block Field_Map015, .LField_Map015_SCENE
.LField_Map015_METATILES:
	.incbin "games/tla/src/map/map_015/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map015_SHAPES:
	.incbin "games/tla/src/map/map_015/shapes.table.lz"
	.balign 4, 0
.LField_Map015_GRID:
	.incbin "games/tla/src/map/map_015/grid.parts.lz"
	.balign 4, 0
.LField_Map015_SHAPE_GRID:
	.incbin "games/tla/src/map/map_015/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map015_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map015_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map015_SCENE:
	.incbin "games/tla/src/map/map_015/scene.parts.lz"
	.balign 4, 0
