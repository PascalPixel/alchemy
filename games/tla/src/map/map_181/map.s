	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map181
Field_Map181:
	map_header 0, 0, 96, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map181, .LField_Map181_METATILES
	map_block Field_Map181, .LField_Map181_SHAPES
	map_block Field_Map181, .LField_Map181_GRID
	map_block Field_Map181, .LField_Map181_SHAPE_GRID
	map_block Field_Map181, .LField_Map181_ANIMATION
	map_block Field_Map181, .LField_Map181_BLEND
	map_block Field_Map181, .LField_Map181_SCENE
.LField_Map181_METATILES:
	.incbin "games/tla/src/map/map_181/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map181_SHAPES:
	.incbin "games/tla/src/map/map_181/shapes.table.lz"
	.balign 4, 0
.LField_Map181_GRID:
	.incbin "games/tla/src/map/map_181/grid.parts.lz"
	.balign 4, 0
.LField_Map181_SHAPE_GRID:
	.incbin "games/tla/src/map/map_181/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map181_ANIMATION:
	.incbin "games/tla/src/map/map_181/animation.script.lz"
	.balign 4, 0
.LField_Map181_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map181_SCENE:
	.incbin "games/tla/src/map/map_181/scene.parts.lz"
