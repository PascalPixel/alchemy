	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map065
Field_Map065:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map065, .LField_Map065_METATILES
	map_block Field_Map065, .LField_Map065_SHAPES
	map_block Field_Map065, .LField_Map065_GRID
	map_block Field_Map065, .LField_Map065_SHAPE_GRID
	map_block Field_Map065, .LField_Map065_ANIMATION
	map_block Field_Map065, .LField_Map065_BLEND
	map_block Field_Map065, .LField_Map065_SCENE
.LField_Map065_METATILES:
	.incbin "games/tla/src/map/map_065/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map065_SHAPES:
	.incbin "games/tla/src/map/map_025/shapes.table.lz"
	.balign 4, 0
.LField_Map065_GRID:
	.incbin "games/tla/src/map/map_065/grid.parts.lz"
	.balign 4, 0
.LField_Map065_SHAPE_GRID:
	.incbin "games/tla/src/map/map_065/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map065_ANIMATION:
	.incbin "games/tla/src/map/map_062/animation.script.lz"
	.balign 4, 0
.LField_Map065_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map065_SCENE:
	.incbin "games/tla/src/map/map_065/scene.parts.lz"
	.balign 4, 0
