	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map023
Field_Map023:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map023, .LField_Map023_METATILES
	map_block Field_Map023, .LField_Map023_SHAPES
	map_block Field_Map023, .LField_Map023_GRID
	map_block Field_Map023, .LField_Map023_SHAPE_GRID
	map_block Field_Map023, .LField_Map023_ANIMATION
	map_block Field_Map023, .LField_Map023_BLEND
	map_block Field_Map023, .LField_Map023_SCENE
.LField_Map023_METATILES:
	.incbin "games/tla/src/map/map_023/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map023_SHAPES:
	.incbin "games/tla/src/map/map_023/shapes.table.lz"
	.balign 4, 0
.LField_Map023_GRID:
	.incbin "games/tla/src/map/map_023/grid.parts.lz"
	.balign 4, 0
.LField_Map023_SHAPE_GRID:
	.incbin "games/tla/src/map/map_023/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map023_ANIMATION:
	.incbin "games/tla/src/map/map_022/animation.script.lz"
	.balign 4, 0
.LField_Map023_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map023_SCENE:
	.incbin "games/tla/src/map/map_023/scene.parts.lz"
