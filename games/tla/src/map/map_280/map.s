	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map280
Field_Map280:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map280, .LField_Map280_METATILES
	map_block Field_Map280, .LField_Map280_SHAPES
	map_block Field_Map280, .LField_Map280_GRID
	map_block Field_Map280, .LField_Map280_SHAPE_GRID
	map_block Field_Map280, .LField_Map280_ANIMATION
	map_block Field_Map280, .LField_Map280_BLEND
	map_block Field_Map280, .LField_Map280_SCENE
.LField_Map280_METATILES:
	.incbin "games/tla/src/map/map_280/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map280_SHAPES:
	.incbin "games/tla/src/map/map_280/shapes.table.lz"
	.balign 4, 0
.LField_Map280_GRID:
	.incbin "games/tla/src/map/map_280/grid.parts.lz"
	.balign 4, 0
.LField_Map280_SHAPE_GRID:
	.incbin "games/tla/src/map/map_280/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map280_ANIMATION:
	.incbin "games/tla/src/map/map_279/animation.script.lz"
	.balign 4, 0
.LField_Map280_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map280_SCENE:
	.incbin "games/tla/src/map/map_280/scene.parts.lz"
