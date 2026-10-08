	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map188
Field_Map188:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map188, .LField_Map188_METATILES
	map_block Field_Map188, .LField_Map188_SHAPES
	map_block Field_Map188, .LField_Map188_GRID
	map_block Field_Map188, .LField_Map188_SHAPE_GRID
	map_block Field_Map188, .LField_Map188_ANIMATION
	map_block Field_Map188
	map_block Field_Map188, .LField_Map188_SCENE
.LField_Map188_METATILES:
	.incbin "map/map_188/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map188_SHAPES:
	.incbin "map/map_188/shapes.table.lz"
	.balign 4, 0
.LField_Map188_GRID:
	.incbin "map/map_188/grid.parts.lz"
	.balign 4, 0
.LField_Map188_SHAPE_GRID:
	.incbin "map/map_188/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map188_ANIMATION:
	.incbin "map/map_188/animation.script.lz"
	.balign 4, 0
.LField_Map188_SCENE:
	.incbin "map/map_188/scene.parts.lz"
