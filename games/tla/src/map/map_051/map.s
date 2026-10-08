	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map051
Field_Map051:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map051, .LField_Map051_METATILES
	map_block Field_Map051, .LField_Map051_SHAPES
	map_block Field_Map051, .LField_Map051_GRID
	map_block Field_Map051, .LField_Map051_SHAPE_GRID
	map_block Field_Map051, .LField_Map051_ANIMATION
	map_block Field_Map051, .LField_Map051_BLEND
	map_block Field_Map051, .LField_Map051_SCENE
.LField_Map051_METATILES:
	.incbin "map/map_051/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "map/map_051/shapes.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "map/map_051/grid.parts.lz"
	.balign 4, 0
.LField_Map051_SHAPE_GRID:
	.incbin "map/map_051/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "map/map_051/animation.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "map/map_051/blend.script.lz"
	.balign 4, 0
.LField_Map051_SCENE:
	.incbin "map/map_051/scene.parts.lz"
