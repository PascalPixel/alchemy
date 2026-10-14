	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map033
Field_Map033:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map033, .LField_Map033_METATILES
	map_block Field_Map033, .LField_Map033_SHAPES
	map_block Field_Map033, .LField_Map033_GRID
	map_block Field_Map033, .LField_Map033_SHAPE_GRID
	map_block Field_Map033, .LField_Map033_ANIMATION
	map_block Field_Map033, .LField_Map033_BLEND
	map_block Field_Map033, .LField_Map033_SCENE
.LField_Map033_METATILES:
	.incbin "games/tla/src/map/map_033/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map033_SHAPES:
	.incbin "games/tla/src/map/map_033/shapes.table.lz"
	.balign 4, 0
.LField_Map033_GRID:
	.incbin "games/tla/src/map/map_033/grid.parts.lz"
	.balign 4, 0
.LField_Map033_SHAPE_GRID:
	.incbin "games/tla/src/map/map_033/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map033_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map033_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map033_SCENE:
	.incbin "games/tla/src/map/map_033/scene.parts.lz"
