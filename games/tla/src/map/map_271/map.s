	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map271
Field_Map271:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map271, .LField_Map271_METATILES
	map_block Field_Map271, .LField_Map271_SHAPES
	map_block Field_Map271, .LField_Map271_GRID
	map_block Field_Map271, .LField_Map271_SHAPE_GRID
	map_block Field_Map271, .LField_Map271_ANIMATION
	map_block Field_Map271, .LField_Map271_BLEND
	map_block Field_Map271, .LField_Map271_SCENE
.LField_Map271_METATILES:
	.incbin "games/tla/src/map/map_271/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map271_SHAPES:
	.incbin "games/tla/src/map/map_271/shapes.table.lz"
	.balign 4, 0
.LField_Map271_GRID:
	.incbin "games/tla/src/map/map_271/grid.parts.lz"
	.balign 4, 0
.LField_Map271_SHAPE_GRID:
	.incbin "games/tla/src/map/map_271/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map271_ANIMATION:
	.incbin "games/tla/src/map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map271_BLEND:
	.incbin "games/tla/src/map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map271_SCENE:
	.incbin "games/tla/src/map/map_271/scene.parts.lz"
