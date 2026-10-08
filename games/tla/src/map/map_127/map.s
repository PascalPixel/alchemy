	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map127
Field_Map127:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map127, .LField_Map127_METATILES
	map_block Field_Map127, .LField_Map127_SHAPES
	map_block Field_Map127, .LField_Map127_GRID
	map_block Field_Map127, .LField_Map127_SHAPE_GRID
	map_block Field_Map127, .LField_Map127_ANIMATION
	map_block Field_Map127
	map_block Field_Map127, .LField_Map127_SCENE
.LField_Map127_METATILES:
	.incbin "map/map_127/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map127_SHAPES:
	.incbin "map/map_127/shapes.table.lz"
	.balign 4, 0
.LField_Map127_GRID:
	.incbin "map/map_127/grid.parts.lz"
	.balign 4, 0
.LField_Map127_SHAPE_GRID:
	.incbin "map/map_127/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map127_ANIMATION:
	.incbin "map/map_127/animation.script.lz"
	.balign 4, 0
.LField_Map127_SCENE:
	.incbin "map/map_127/scene.parts.lz"
