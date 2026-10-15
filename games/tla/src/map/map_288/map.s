	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map288
Field_Map288:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map288, .LField_Map288_METATILES
	map_block Field_Map288, .LField_Map288_SHAPES
	map_block Field_Map288, .LField_Map288_GRID
	map_block Field_Map288, .LField_Map288_SHAPE_GRID
	map_block Field_Map288, .LField_Map288_ANIMATION
	map_block Field_Map288, .LField_Map288_BLEND
	map_block Field_Map288, .LField_Map288_SCENE
.LField_Map288_METATILES:
	.incbin "games/tla/src/map/map_288/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map288_SHAPES:
	.incbin "games/tla/src/map/map_288/shapes.table.lz"
	.balign 4, 0
.LField_Map288_GRID:
	.incbin "games/tla/src/map/map_288/grid.parts.lz"
	.balign 4, 0
.LField_Map288_SHAPE_GRID:
	.incbin "games/tla/src/map/map_288/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map288_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map288_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map288_SCENE:
	.incbin "games/tla/src/map/map_288/scene.parts.lz"
	.balign 4, 0
