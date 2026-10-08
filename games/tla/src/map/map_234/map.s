	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map234
Field_Map234:
	map_header 0, 0, 96, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map234, .LField_Map234_METATILES
	map_block Field_Map234, .LField_Map234_SHAPES
	map_block Field_Map234, .LField_Map234_GRID
	map_block Field_Map234, .LField_Map234_SHAPE_GRID
	map_block Field_Map234, .LField_Map234_ANIMATION
	map_block Field_Map234, .LField_Map234_BLEND
	map_block Field_Map234, .LField_Map234_SCENE
.LField_Map234_METATILES:
	.incbin "map/map_234/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map234_SHAPES:
	.incbin "map/map_234/shapes.table.lz"
	.balign 4, 0
.LField_Map234_GRID:
	.incbin "map/map_234/grid.parts.lz"
	.balign 4, 0
.LField_Map234_SHAPE_GRID:
	.incbin "map/map_234/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map234_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map234_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map234_SCENE:
	.incbin "map/map_234/scene.parts.lz"
