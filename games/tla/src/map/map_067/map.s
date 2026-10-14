	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map067
Field_Map067:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map067, .LField_Map067_METATILES
	map_block Field_Map067, .LField_Map067_SHAPES
	map_block Field_Map067, .LField_Map067_GRID
	map_block Field_Map067, .LField_Map067_SHAPE_GRID
	map_block Field_Map067, .LField_Map067_ANIMATION
	map_block Field_Map067, .LField_Map067_BLEND
	map_block Field_Map067, .LField_Map067_SCENE
.LField_Map067_METATILES:
	.incbin "games/tla/src/map/map_067/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map067_SHAPES:
	.incbin "games/tla/src/map/map_067/shapes.table.lz"
	.balign 4, 0
.LField_Map067_GRID:
	.incbin "games/tla/src/map/map_067/grid.parts.lz"
	.balign 4, 0
.LField_Map067_SHAPE_GRID:
	.incbin "games/tla/src/map/map_067/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map067_ANIMATION:
	.incbin "games/tla/src/map/map_067/animation.script.lz"
	.balign 4, 0
.LField_Map067_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map067_SCENE:
	.incbin "games/tla/src/map/map_067/scene.parts.lz"
