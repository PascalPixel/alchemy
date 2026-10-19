	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map001
Field_Map001:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map001, .LField_Map001_METATILES
	map_block Field_Map001, .LField_Map001_SHAPES
	map_block Field_Map001, .LField_Map001_GRID
	map_block Field_Map001, .LField_Map001_SHAPE_GRID
	map_block Field_Map001, .LField_Map001_ANIMATION
	map_block Field_Map001, .LField_Map001_BLEND
	map_block Field_Map001, .LField_Map001_SCENE
.LField_Map001_METATILES:
	.incbin "games/tla/src/map/map_001/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map001_SHAPES:
	.incbin "games/tla/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map001_GRID:
	.incbin "games/tla/src/map/map_001/grid.parts.lz"
	.balign 4, 0
.LField_Map001_SHAPE_GRID:
	.incbin "games/tla/src/map/map_001/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map001_ANIMATION:
	.incbin "games/tla/src/map/map_001/animation.script.lz"
	.balign 4, 0
.LField_Map001_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map001_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
