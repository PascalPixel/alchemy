	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map086
Field_Map086:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map086, .LField_Map086_METATILES
	map_block Field_Map086, .LField_Map086_SHAPES
	map_block Field_Map086, .LField_Map086_GRID
	map_block Field_Map086, .LField_Map086_SHAPE_GRID
	map_block Field_Map086, .LField_Map086_ANIMATION
	map_block Field_Map086, .LField_Map086_BLEND
	map_block Field_Map086, .LField_Map086_SCENE
.LField_Map086_METATILES:
	.incbin "games/tla/src/map/map_086/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map086_SHAPES:
	.incbin "games/tla/src/map/map_086/shapes.table.lz"
	.balign 4, 0
.LField_Map086_GRID:
	.incbin "games/tla/src/map/map_086/grid.parts.lz"
	.balign 4, 0
.LField_Map086_SHAPE_GRID:
	.incbin "games/tla/src/map/map_086/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map086_ANIMATION:
	.incbin "games/tla/src/map/map_078/animation.script.lz"
	.balign 4, 0
.LField_Map086_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map086_SCENE:
	.incbin "games/tla/src/map/map_086/scene.parts.lz"
	.balign 4, 0
