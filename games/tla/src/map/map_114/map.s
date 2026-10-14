	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map114
Field_Map114:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map114, .LField_Map114_METATILES
	map_block Field_Map114, .LField_Map114_SHAPES
	map_block Field_Map114, .LField_Map114_GRID
	map_block Field_Map114, .LField_Map114_SHAPE_GRID
	map_block Field_Map114, .LField_Map114_ANIMATION
	map_block Field_Map114, .LField_Map114_BLEND
	map_block Field_Map114, .LField_Map114_SCENE
.LField_Map114_METATILES:
	.incbin "games/tla/src/map/map_114/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map114_SHAPES:
	.incbin "games/tla/src/map/map_114/shapes.table.lz"
	.balign 4, 0
.LField_Map114_GRID:
	.incbin "games/tla/src/map/map_114/grid.parts.lz"
	.balign 4, 0
.LField_Map114_SHAPE_GRID:
	.incbin "games/tla/src/map/map_114/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map114_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map114_BLEND:
	.incbin "games/tla/src/map/map_114/blend.script.lz"
	.balign 4, 0
.LField_Map114_SCENE:
	.incbin "games/tla/src/map/map_114/scene.parts.lz"
