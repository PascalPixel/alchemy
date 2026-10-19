	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map289
Field_Map289:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map289, .LField_Map289_METATILES
	map_block Field_Map289, .LField_Map289_SHAPES
	map_block Field_Map289, .LField_Map289_GRID
	map_block Field_Map289, .LField_Map289_SHAPE_GRID
	map_block Field_Map289, .LField_Map289_ANIMATION
	map_block Field_Map289, .LField_Map289_BLEND
	map_block Field_Map289, .LField_Map289_SCENE
.LField_Map289_METATILES:
	.incbin "games/tla/src/map/map_289/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map289_SHAPES:
	.incbin "games/tla/src/map/map_179/shapes.table.lz"
	.balign 4, 0
.LField_Map289_GRID:
	.incbin "games/tla/src/map/map_289/grid.parts.lz"
	.balign 4, 0
.LField_Map289_SHAPE_GRID:
	.incbin "games/tla/src/map/map_289/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map289_ANIMATION:
	.incbin "games/tla/src/map/map_289/animation.script.lz"
	.balign 4, 0
.LField_Map289_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map289_SCENE:
	.incbin "games/tla/src/map/map_236/scene.parts.lz"
	.balign 4, 0
