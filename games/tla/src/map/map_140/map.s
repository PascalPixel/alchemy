	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map140
Field_Map140:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map140, .LField_Map140_METATILES
	map_block Field_Map140, .LField_Map140_SHAPES
	map_block Field_Map140, .LField_Map140_GRID
	map_block Field_Map140, .LField_Map140_SHAPE_GRID
	map_block Field_Map140, .LField_Map140_ANIMATION
	map_block Field_Map140, .LField_Map140_BLEND
	map_block Field_Map140, .LField_Map140_SCENE
.LField_Map140_METATILES:
	.incbin "games/tla/src/map/map_140/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map140_SHAPES:
	.incbin "games/tla/src/map/map_140/shapes.table.lz"
	.balign 4, 0
.LField_Map140_GRID:
	.incbin "games/tla/src/map/map_140/grid.parts.lz"
	.balign 4, 0
.LField_Map140_SHAPE_GRID:
	.incbin "games/tla/src/map/map_140/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map140_ANIMATION:
	.incbin "games/tla/src/map/map_139/animation.script.lz"
	.balign 4, 0
.LField_Map140_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map140_SCENE:
	.incbin "games/tla/src/map/map_140/scene.parts.lz"
	.balign 4, 0
