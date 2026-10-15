	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map272
Field_Map272:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map272, .LField_Map272_METATILES
	map_block Field_Map272, .LField_Map272_SHAPES
	map_block Field_Map272, .LField_Map272_GRID
	map_block Field_Map272, .LField_Map272_SHAPE_GRID
	map_block Field_Map272, .LField_Map272_ANIMATION
	map_block Field_Map272, .LField_Map272_BLEND
	map_block Field_Map272, .LField_Map272_SCENE
.LField_Map272_METATILES:
	.incbin "games/tla/src/map/map_272/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map272_SHAPES:
	.incbin "games/tla/src/map/map_272/shapes.table.lz"
	.balign 4, 0
.LField_Map272_GRID:
	.incbin "games/tla/src/map/map_272/grid.parts.lz"
	.balign 4, 0
.LField_Map272_SHAPE_GRID:
	.incbin "games/tla/src/map/map_272/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map272_ANIMATION:
	.incbin "games/tla/src/map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map272_BLEND:
	.incbin "games/tla/src/map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map272_SCENE:
	.incbin "games/tla/src/map/map_272/scene.parts.lz"
	.balign 4, 0
