	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map011
Field_Map011:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map011, .LField_Map011_METATILES
	map_block Field_Map011, .LField_Map011_SHAPES
	map_block Field_Map011, .LField_Map011_GRID
	map_block Field_Map011, .LField_Map011_SHAPE_GRID
	map_block Field_Map011, .LField_Map011_ANIMATION
	map_block Field_Map011, .LField_Map011_BLEND
	map_block Field_Map011, .LField_Map011_SCENE
.LField_Map011_METATILES:
	.incbin "games/tla/src/map/map_011/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map011_SHAPES:
	.incbin "games/tla/src/map/map_011/shapes.table.lz"
	.balign 4, 0
.LField_Map011_GRID:
	.incbin "games/tla/src/map/map_011/grid.parts.lz"
	.balign 4, 0
.LField_Map011_SHAPE_GRID:
	.incbin "games/tla/src/map/map_011/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map011_ANIMATION:
	.incbin "games/tla/src/map/map_011/animation.script.lz"
	.balign 4, 0
.LField_Map011_BLEND:
	.incbin "games/tla/src/map/map_011/blend.script.lz"
	.balign 4, 0
.LField_Map011_SCENE:
	.incbin "games/tla/src/map/map_011/scene.parts.lz"
