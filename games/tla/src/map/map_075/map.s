	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map075
Field_Map075:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map075, .LField_Map075_METATILES
	map_block Field_Map075, .LField_Map075_SHAPES
	map_block Field_Map075, .LField_Map075_GRID
	map_block Field_Map075, .LField_Map075_SHAPE_GRID
	map_block Field_Map075, .LField_Map075_ANIMATION
	map_block Field_Map075, .LField_Map075_BLEND
	map_block Field_Map075, .LField_Map075_SCENE
.LField_Map075_METATILES:
	.incbin "map/map_075/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map075_SHAPES:
	.incbin "map/map_075/shapes.table.lz"
	.balign 4, 0
.LField_Map075_GRID:
	.incbin "map/map_075/grid.parts.lz"
	.balign 4, 0
.LField_Map075_SHAPE_GRID:
	.incbin "map/map_075/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map075_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map075_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map075_SCENE:
	.incbin "map/map_075/scene.parts.lz"
