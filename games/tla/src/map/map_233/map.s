	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map233
Field_Map233:
	map_header 0, 0, 84, 240, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 84, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 168, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map233, .LField_Map233_METATILES
	map_block Field_Map233, .LField_Map233_SHAPES
	map_block Field_Map233, .LField_Map233_GRID
	map_block Field_Map233, .LField_Map233_SHAPE_GRID
	map_block Field_Map233, .LField_Map233_ANIMATION
	map_block Field_Map233, .LField_Map233_BLEND
	map_block Field_Map233, .LField_Map233_SCENE
.LField_Map233_METATILES:
	.incbin "games/tla/src/map/map_233/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map233_SHAPES:
	.incbin "games/tla/src/map/map_233/shapes.table.lz"
	.balign 4, 0
.LField_Map233_GRID:
	.incbin "games/tla/src/map/map_233/grid.parts.lz"
	.balign 4, 0
.LField_Map233_SHAPE_GRID:
	.incbin "games/tla/src/map/map_233/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map233_ANIMATION:
	.incbin "games/tla/src/map/map_233/animation.script.lz"
	.balign 4, 0
.LField_Map233_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map233_SCENE:
	.incbin "games/tla/src/map/map_233/scene.parts.lz"
	.balign 4, 0
