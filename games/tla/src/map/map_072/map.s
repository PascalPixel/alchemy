	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map072
Field_Map072:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map072, .LField_Map072_METATILES
	map_block Field_Map072, .LField_Map072_SHAPES
	map_block Field_Map072, .LField_Map072_GRID
	map_block Field_Map072, .LField_Map072_SHAPE_GRID
	map_block Field_Map072, .LField_Map072_ANIMATION
	map_block Field_Map072
	map_block Field_Map072, .LField_Map072_SCENE
.LField_Map072_METATILES:
	.incbin "games/tla/src/map/map_072/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map072_SHAPES:
	.incbin "games/tla/src/map/map_072/shapes.table.lz"
	.balign 4, 0
.LField_Map072_GRID:
	.incbin "games/tla/src/map/map_072/grid.parts.lz"
	.balign 4, 0
.LField_Map072_SHAPE_GRID:
	.incbin "games/tla/src/map/map_072/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map072_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map072_SCENE:
	.incbin "games/tla/src/map/map_072/scene.parts.lz"
