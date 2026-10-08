	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map158
Field_Map158:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map158, .LField_Map158_METATILES
	map_block Field_Map158, .LField_Map158_SHAPES
	map_block Field_Map158, .LField_Map158_GRID
	map_block Field_Map158, .LField_Map158_SHAPE_GRID
	map_block Field_Map158, .LField_Map158_ANIMATION
	map_block Field_Map158, .LField_Map158_BLEND
	map_block Field_Map158, .LField_Map158_SCENE
.LField_Map158_METATILES:
	.incbin "map/map_158/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map158_SHAPES:
	.incbin "map/map_158/shapes.table.lz"
	.balign 4, 0
.LField_Map158_GRID:
	.incbin "map/map_158/grid.parts.lz"
	.balign 4, 0
.LField_Map158_SHAPE_GRID:
	.incbin "map/map_158/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map158_ANIMATION:
	.incbin "map/map_158/animation.script.lz"
	.balign 4, 0
.LField_Map158_BLEND:
	.incbin "map/map_150/blend.script.lz"
	.balign 4, 0
.LField_Map158_SCENE:
	.incbin "map/map_158/scene.parts.lz"
