	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map110
Field_Map110:
	map_header 0, 0, 100, 70, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map110, .LField_Map110_METATILES
	map_block Field_Map110, .LField_Map110_SHAPES
	map_block Field_Map110, .LField_Map110_GRID
	map_block Field_Map110, .LField_Map110_SHAPE_GRID
	map_block Field_Map110, .LField_Map110_ANIMATION
	map_block Field_Map110, .LField_Map110_BLEND
	map_block Field_Map110, .LField_Map110_SCENE
.LField_Map110_METATILES:
	.incbin "games/tla/src/map/map_110/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map110_SHAPES:
	.incbin "games/tla/src/map/map_107/shapes.table.lz"
	.balign 4, 0
.LField_Map110_GRID:
	.incbin "games/tla/src/map/map_110/grid.parts.lz"
	.balign 4, 0
.LField_Map110_SHAPE_GRID:
	.incbin "games/tla/src/map/map_110/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map110_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map110_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map110_SCENE:
	.incbin "games/tla/src/map/map_110/scene.parts.lz"
