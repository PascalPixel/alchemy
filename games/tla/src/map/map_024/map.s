	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map024
Field_Map024:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map024, .LField_Map024_METATILES
	map_block Field_Map024, .LField_Map024_SHAPES
	map_block Field_Map024, .LField_Map024_GRID
	map_block Field_Map024, .LField_Map024_SHAPE_GRID
	map_block Field_Map024, .LField_Map024_ANIMATION
	map_block Field_Map024, .LField_Map024_BLEND
	map_block Field_Map024, .LField_Map024_SCENE
.LField_Map024_METATILES:
	.incbin "games/tla/src/map/map_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map024_SHAPES:
	.incbin "games/tla/src/map/map_024/shapes.table.lz"
	.balign 4, 0
.LField_Map024_GRID:
	.incbin "games/tla/src/map/map_024/grid.parts.lz"
	.balign 4, 0
.LField_Map024_SHAPE_GRID:
	.incbin "games/tla/src/map/map_024/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map024_ANIMATION:
	.incbin "games/tla/src/map/map_024/animation.script.lz"
	.balign 4, 0
.LField_Map024_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map024_SCENE:
	.incbin "games/tla/src/map/map_024/scene.parts.lz"
	.balign 4, 0
