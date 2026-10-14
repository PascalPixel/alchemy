	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map227
Field_Map227:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map227, .LField_Map227_METATILES
	map_block Field_Map227, .LField_Map227_SHAPES
	map_block Field_Map227, .LField_Map227_GRID
	map_block Field_Map227, .LField_Map227_SHAPE_GRID
	map_block Field_Map227, .LField_Map227_ANIMATION
	map_block Field_Map227, .LField_Map227_BLEND
	map_block Field_Map227, .LField_Map227_SCENE
.LField_Map227_METATILES:
	.incbin "games/tla/src/map/map_227/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map227_SHAPES:
	.incbin "games/tla/src/map/map_227/shapes.table.lz"
	.balign 4, 0
.LField_Map227_GRID:
	.incbin "games/tla/src/map/map_227/grid.parts.lz"
	.balign 4, 0
.LField_Map227_SHAPE_GRID:
	.incbin "games/tla/src/map/map_227/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map227_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map227_BLEND:
	.incbin "games/tla/src/map/map_227/blend.script.lz"
	.balign 4, 0
.LField_Map227_SCENE:
	.incbin "games/tla/src/map/map_227/scene.parts.lz"
