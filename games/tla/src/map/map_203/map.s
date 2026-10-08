	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map203
Field_Map203:
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map203, .LField_Map203_METATILES
	map_block Field_Map203, .LField_Map203_SHAPES
	map_block Field_Map203, .LField_Map203_GRID
	map_block Field_Map203, .LField_Map203_SHAPE_GRID
	map_block Field_Map203, .LField_Map203_ANIMATION
	map_block Field_Map203, .LField_Map203_BLEND
	map_block Field_Map203, .LField_Map203_SCENE
.LField_Map203_METATILES:
	.incbin "map/map_203/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map203_SHAPES:
	.incbin "map/map_203/shapes.table.lz"
	.balign 4, 0
.LField_Map203_GRID:
	.incbin "map/map_203/grid.parts.lz"
	.balign 4, 0
.LField_Map203_SHAPE_GRID:
	.incbin "map/map_203/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map203_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map203_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map203_SCENE:
	.incbin "map/map_203/scene.parts.lz"
