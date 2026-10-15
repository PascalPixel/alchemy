	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map048
Field_Map048:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map048, .LField_Map048_METATILES
	map_block Field_Map048, .LField_Map048_SHAPES
	map_block Field_Map048, .LField_Map048_GRID
	map_block Field_Map048, .LField_Map048_SHAPE_GRID
	map_block Field_Map048, .LField_Map048_ANIMATION
	map_block Field_Map048
	map_block Field_Map048, .LField_Map048_SCENE
.LField_Map048_METATILES:
	.incbin "games/tla/src/map/map_048/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map048_SHAPES:
	.incbin "games/tla/src/map/map_048/shapes.table.lz"
	.balign 4, 0
.LField_Map048_GRID:
	.incbin "games/tla/src/map/map_048/grid.parts.lz"
	.balign 4, 0
.LField_Map048_SHAPE_GRID:
	.incbin "games/tla/src/map/map_048/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map048_ANIMATION:
	.incbin "games/tla/src/map/map_048/animation.script.lz"
	.balign 4, 0
.LField_Map048_SCENE:
	.incbin "games/tla/src/map/map_048/scene.parts.lz"
	.balign 4, 0
