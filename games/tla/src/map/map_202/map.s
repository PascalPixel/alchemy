	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map202
Field_Map202:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map202, .LField_Map202_METATILES
	map_block Field_Map202, .LField_Map202_SHAPES
	map_block Field_Map202, .LField_Map202_GRID
	map_block Field_Map202, .LField_Map202_SHAPE_GRID
	map_block Field_Map202, .LField_Map202_ANIMATION
	map_block Field_Map202, .LField_Map202_BLEND
	map_block Field_Map202, .LField_Map202_SCENE
.LField_Map202_METATILES:
	.incbin "games/tla/src/map/map_202/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map202_SHAPES:
	.incbin "games/tla/src/map/map_202/shapes.table.lz"
	.balign 4, 0
.LField_Map202_GRID:
	.incbin "games/tla/src/map/map_202/grid.parts.lz"
	.balign 4, 0
.LField_Map202_SHAPE_GRID:
	.incbin "games/tla/src/map/map_202/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map202_ANIMATION:
	.incbin "games/tla/src/map/map_202/animation.script.lz"
	.balign 4, 0
.LField_Map202_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map202_SCENE:
	.incbin "games/tla/src/map/map_202/scene.parts.lz"
