	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map251
Field_Map251:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map251, .LField_Map251_METATILES
	map_block Field_Map251, .LField_Map251_SHAPES
	map_block Field_Map251, .LField_Map251_GRID
	map_block Field_Map251, .LField_Map251_SHAPE_GRID
	map_block Field_Map251, .LField_Map251_ANIMATION
	map_block Field_Map251, .LField_Map251_BLEND
	map_block Field_Map251, .LField_Map251_SCENE
.LField_Map251_METATILES:
	.incbin "map/map_251/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map251_SHAPES:
	.incbin "map/map_251/shapes.table.lz"
	.balign 4, 0
.LField_Map251_GRID:
	.incbin "map/map_251/grid.parts.lz"
	.balign 4, 0
.LField_Map251_SHAPE_GRID:
	.incbin "map/map_251/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map251_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map251_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map251_SCENE:
	.incbin "map/map_251/scene.parts.lz"
