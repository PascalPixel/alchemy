	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map193
Field_Map193:
	map_header 0, 0, 30, 22, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 132, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map193, .LField_Map193_METATILES
	map_block Field_Map193, .LField_Map193_SHAPES
	map_block Field_Map193, .LField_Map193_GRID
	map_block Field_Map193, .LField_Map193_SHAPE_GRID
	map_block Field_Map193, .LField_Map193_ANIMATION
	map_block Field_Map193, .LField_Map193_BLEND
	map_block Field_Map193, .LField_Map193_SCENE
.LField_Map193_METATILES:
	.incbin "games/tla/src/map/map_193/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map193_SHAPES:
	.incbin "games/common/src/map/map_170/shapes.table.lz"
	.balign 4, 0
.LField_Map193_GRID:
	.incbin "games/tla/src/map/map_193/grid.parts.lz"
	.balign 4, 0
.LField_Map193_SHAPE_GRID:
	.incbin "games/tla/src/map/map_193/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map193_ANIMATION:
	.incbin "games/common/src/map/map_170/animation.script.lz"
	.balign 4, 0
.LField_Map193_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map193_SCENE:
	.incbin "games/tla/src/map/map_193/scene.parts.lz"
