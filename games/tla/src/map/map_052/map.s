	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map052
Field_Map052:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map052, .LField_Map052_METATILES
	map_block Field_Map052, .LField_Map052_SHAPES
	map_block Field_Map052, .LField_Map052_GRID
	map_block Field_Map052, .LField_Map052_SHAPE_GRID
	map_block Field_Map052, .LField_Map052_ANIMATION
	map_block Field_Map052, .LField_Map052_BLEND
	map_block Field_Map052, .LField_Map052_SCENE
.LField_Map052_METATILES:
	.incbin "map/map_052/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map052_SHAPES:
	.incbin "map/map_052/shapes.table.lz"
	.balign 4, 0
.LField_Map052_GRID:
	.incbin "map/map_052/grid.parts.lz"
	.balign 4, 0
.LField_Map052_SHAPE_GRID:
	.incbin "map/map_052/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map052_ANIMATION:
	.incbin "map/map_052/animation.script.lz"
	.balign 4, 0
.LField_Map052_BLEND:
	.incbin "common/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map052_SCENE:
	.incbin "map/map_052/scene.parts.lz"
