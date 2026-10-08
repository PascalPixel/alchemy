	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map076
Field_Map076:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map076, .LField_Map076_METATILES
	map_block Field_Map076, .LField_Map076_SHAPES
	map_block Field_Map076, .LField_Map076_GRID
	map_block Field_Map076, .LField_Map076_SHAPE_GRID
	map_block Field_Map076, .LField_Map076_ANIMATION
	map_block Field_Map076, .LField_Map076_BLEND
	map_block Field_Map076, .LField_Map076_SCENE
.LField_Map076_METATILES:
	.incbin "map/map_076/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map076_SHAPES:
	.incbin "map/map_076/shapes.table.lz"
	.balign 4, 0
.LField_Map076_GRID:
	.incbin "map/map_076/grid.parts.lz"
	.balign 4, 0
.LField_Map076_SHAPE_GRID:
	.incbin "map/map_076/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map076_ANIMATION:
	.incbin "map/map_076/animation.script.lz"
	.balign 4, 0
.LField_Map076_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map076_SCENE:
	.incbin "map/map_076/scene.parts.lz"
