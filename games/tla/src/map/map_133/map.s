	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map133
Field_Map133:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map133, .LField_Map133_METATILES
	map_block Field_Map133, .LField_Map133_SHAPES
	map_block Field_Map133, .LField_Map133_GRID
	map_block Field_Map133, .LField_Map133_SHAPE_GRID
	map_block Field_Map133, .LField_Map133_ANIMATION
	map_block Field_Map133, .LField_Map133_BLEND
	map_block Field_Map133, .LField_Map133_SCENE
.LField_Map133_METATILES:
	.incbin "map/map_133/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map133_SHAPES:
	.incbin "map/map_133/shapes.table.lz"
	.balign 4, 0
.LField_Map133_GRID:
	.incbin "map/map_133/grid.parts.lz"
	.balign 4, 0
.LField_Map133_SHAPE_GRID:
	.incbin "map/map_133/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map133_ANIMATION:
	.incbin "map/map_133/animation.script.lz"
	.balign 4, 0
.LField_Map133_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map133_SCENE:
	.incbin "map/map_133/scene.parts.lz"
