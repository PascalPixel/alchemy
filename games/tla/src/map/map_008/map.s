	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map008
Field_Map008:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map008, .LField_Map008_METATILES
	map_block Field_Map008, .LField_Map008_SHAPES
	map_block Field_Map008, .LField_Map008_GRID
	map_block Field_Map008, .LField_Map008_SHAPE_GRID
	map_block Field_Map008, .LField_Map008_ANIMATION
	map_block Field_Map008
	map_block Field_Map008, .LField_Map008_SCENE
.LField_Map008_METATILES:
	.incbin "games/tla/src/map/map_008/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map008_SHAPES:
	.incbin "games/tla/src/map/map_008/shapes.table.lz"
	.balign 4, 0
.LField_Map008_GRID:
	.incbin "games/tla/src/map/map_008/grid.parts.lz"
	.balign 4, 0
.LField_Map008_SHAPE_GRID:
	.incbin "games/tla/src/map/map_008/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map008_ANIMATION:
	.incbin "games/tla/src/map/map_008/animation.script.lz"
	.balign 4, 0
.LField_Map008_SCENE:
	.incbin "games/tla/src/map/map_008/scene.parts.lz"
