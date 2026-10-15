	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map049
Field_Map049:
	map_header 0, 0, 74, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 74, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map049, .LField_Map049_METATILES
	map_block Field_Map049, .LField_Map049_SHAPES
	map_block Field_Map049, .LField_Map049_GRID
	map_block Field_Map049, .LField_Map049_SHAPE_GRID
	map_block Field_Map049, .LField_Map049_ANIMATION
	map_block Field_Map049, .LField_Map049_BLEND
	map_block Field_Map049, .LField_Map049_SCENE
.LField_Map049_METATILES:
	.incbin "games/tla/src/map/map_049/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map049_SHAPES:
	.incbin "games/tla/src/map/map_049/shapes.table.lz"
	.balign 4, 0
.LField_Map049_GRID:
	.incbin "games/tla/src/map/map_049/grid.parts.lz"
	.balign 4, 0
.LField_Map049_SHAPE_GRID:
	.incbin "games/tla/src/map/map_049/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map049_ANIMATION:
	.incbin "games/tla/src/map/map_049/animation.script.lz"
	.balign 4, 0
.LField_Map049_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map049_SCENE:
	.incbin "games/tla/src/map/map_049/scene.parts.lz"
	.balign 4, 0
