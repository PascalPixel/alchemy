	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map009
Field_Map009:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map009, .LField_Map009_METATILES
	map_block Field_Map009, .LField_Map009_SHAPES
	map_block Field_Map009, .LField_Map009_GRID
	map_block Field_Map009, .LField_Map009_SHAPE_GRID
	map_block Field_Map009, .LField_Map009_ANIMATION
	map_block Field_Map009
	map_block Field_Map009, .LField_Map009_SCENE
.LField_Map009_METATILES:
	.incbin "map/map_009/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map009_SHAPES:
	.incbin "map/map_009/shapes.table.lz"
	.balign 4, 0
.LField_Map009_GRID:
	.incbin "map/map_009/grid.parts.lz"
	.balign 4, 0
.LField_Map009_SHAPE_GRID:
	.incbin "map/map_009/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map009_ANIMATION:
	.incbin "map/map_008/animation.script.lz"
	.balign 4, 0
.LField_Map009_SCENE:
	.incbin "map/map_009/scene.parts.lz"
