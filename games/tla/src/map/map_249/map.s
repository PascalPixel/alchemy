	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map249
Field_Map249:
	map_header 0, 0, 204, 64, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map249, .LField_Map249_METATILES
	map_block Field_Map249, .LField_Map249_SHAPES
	map_block Field_Map249, .LField_Map249_GRID
	map_block Field_Map249, .LField_Map249_SHAPE_GRID
	map_block Field_Map249, .LField_Map249_ANIMATION
	map_block Field_Map249
	map_block Field_Map249, .LField_Map249_SCENE
.LField_Map249_METATILES:
	.incbin "games/tla/src/map/map_249/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map249_SHAPES:
	.incbin "games/tla/src/map/map_249/shapes.table.lz"
	.balign 4, 0
.LField_Map249_GRID:
	.incbin "games/tla/src/map/map_249/grid.parts.lz"
	.balign 4, 0
.LField_Map249_SHAPE_GRID:
	.incbin "games/tla/src/map/map_249/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map249_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map249_SCENE:
	.incbin "games/tla/src/map/map_249/scene.parts.lz"
	.balign 4, 0
