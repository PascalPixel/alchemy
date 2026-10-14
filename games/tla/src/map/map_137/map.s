	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map137
Field_Map137:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map137, .LField_Map137_METATILES
	map_block Field_Map137, .LField_Map137_SHAPES
	map_block Field_Map137, .LField_Map137_GRID
	map_block Field_Map137, .LField_Map137_SHAPE_GRID
	map_block Field_Map137, .LField_Map137_ANIMATION
	map_block Field_Map137, .LField_Map137_BLEND
	map_block Field_Map137, .LField_Map137_SCENE
.LField_Map137_METATILES:
	.incbin "games/tla/src/map/map_137/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map137_SHAPES:
	.incbin "games/tla/src/map/map_137/shapes.table.lz"
	.balign 4, 0
.LField_Map137_GRID:
	.incbin "games/tla/src/map/map_137/grid.parts.lz"
	.balign 4, 0
.LField_Map137_SHAPE_GRID:
	.incbin "games/tla/src/map/map_137/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map137_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map137_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map137_SCENE:
	.incbin "games/tla/src/map/map_137/scene.parts.lz"
