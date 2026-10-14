	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map081
Field_Map081:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map081, .LField_Map081_METATILES
	map_block Field_Map081, .LField_Map081_SHAPES
	map_block Field_Map081, .LField_Map081_GRID
	map_block Field_Map081, .LField_Map081_SHAPE_GRID
	map_block Field_Map081, .LField_Map081_ANIMATION
	map_block Field_Map081, .LField_Map081_BLEND
	map_block Field_Map081, .LField_Map081_SCENE
.LField_Map081_METATILES:
	.incbin "games/tla/src/map/map_081/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map081_SHAPES:
	.incbin "games/tla/src/map/map_081/shapes.table.lz"
	.balign 4, 0
.LField_Map081_GRID:
	.incbin "games/tla/src/map/map_081/grid.parts.lz"
	.balign 4, 0
.LField_Map081_SHAPE_GRID:
	.incbin "games/tla/src/map/map_081/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map081_ANIMATION:
	.incbin "games/tla/src/map/map_078/animation.script.lz"
	.balign 4, 0
.LField_Map081_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map081_SCENE:
	.incbin "games/tla/src/map/map_081/scene.parts.lz"
