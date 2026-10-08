	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map191
Field_Map191:
	map_header 0, 0, 128, 112, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map191, .LField_Map191_METATILES
	map_block Field_Map191, .LField_Map191_SHAPES
	map_block Field_Map191, .LField_Map191_GRID
	map_block Field_Map191, .LField_Map191_SHAPE_GRID
	map_block Field_Map191, .LField_Map191_ANIMATION
	map_block Field_Map191, .LField_Map191_BLEND
	map_block Field_Map191, .LField_Map191_SCENE
.LField_Map191_METATILES:
	.incbin "map/map_191/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map191_SHAPES:
	.incbin "map/map_191/shapes.table.lz"
	.balign 4, 0
.LField_Map191_GRID:
	.incbin "map/map_191/grid.parts.lz"
	.balign 4, 0
.LField_Map191_SHAPE_GRID:
	.incbin "map/map_191/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map191_ANIMATION:
	.incbin "map/map_191/animation.script.lz"
	.balign 4, 0
.LField_Map191_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map191_SCENE:
	.incbin "map/map_191/scene.parts.lz"
