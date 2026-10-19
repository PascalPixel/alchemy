	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map003
Field_Map003:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map003, .LField_Map003_METATILES
	map_block Field_Map003, .LField_Map003_SHAPES
	map_block Field_Map003, .LField_Map003_GRID
	map_block Field_Map003, .LField_Map003_SHAPE_GRID
	map_block Field_Map003, .LField_Map003_ANIMATION
	map_block Field_Map003, .LField_Map003_BLEND
	map_block Field_Map003, .LField_Map003_SCENE
.LField_Map003_METATILES:
	.incbin "games/tla/src/map/map_003/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map003_SHAPES:
	.incbin "games/tla/src/map/map_003/shapes.table.lz"
	.balign 4, 0
.LField_Map003_GRID:
	.incbin "games/tla/src/map/map_003/grid.parts.lz"
	.balign 4, 0
.LField_Map003_SHAPE_GRID:
	.incbin "games/tla/src/map/map_003/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map003_ANIMATION:
	.incbin "games/tla/src/map/map_003/animation.script.lz"
	.balign 4, 0
.LField_Map003_BLEND:
	.incbin "games/tla/src/map/map_003/blend.script.lz"
	.balign 4, 0
.LField_Map003_SCENE:
	.incbin "games/tla/src/map/map_003/scene.parts.lz"
	.balign 4, 0
