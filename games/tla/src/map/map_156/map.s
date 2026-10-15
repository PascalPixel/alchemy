	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map156
Field_Map156:
	map_header 0, 0, 64, 64, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map156, .LField_Map156_METATILES
	map_block Field_Map156, .LField_Map156_SHAPES
	map_block Field_Map156, .LField_Map156_GRID
	map_block Field_Map156, .LField_Map156_SHAPE_GRID
	map_block Field_Map156, .LField_Map156_ANIMATION
	map_block Field_Map156, .LField_Map156_BLEND
	map_block Field_Map156, .LField_Map156_SCENE
.LField_Map156_METATILES:
	.incbin "games/tla/src/map/map_156/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map156_SHAPES:
	.incbin "games/tla/src/map/map_156/shapes.table.lz"
	.balign 4, 0
.LField_Map156_GRID:
	.incbin "games/tla/src/map/map_156/grid.parts.lz"
	.balign 4, 0
.LField_Map156_SHAPE_GRID:
	.incbin "games/tla/src/map/map_156/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map156_ANIMATION:
	.incbin "games/tla/src/map/map_156/animation.script.lz"
	.balign 4, 0
.LField_Map156_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map156_SCENE:
	.incbin "games/tla/src/map/map_156/scene.parts.lz"
	.balign 4, 0
