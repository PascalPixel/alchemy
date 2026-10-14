	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map220
Field_Map220:
	map_header 0, 0, 128, 128, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 1, 1, 0x1f, 0x1f
	map_block Field_Map220, .LField_Map220_METATILES
	map_block Field_Map220, .LField_Map220_SHAPES
	map_block Field_Map220, .LField_Map220_GRID
	map_block Field_Map220, .LField_Map220_SHAPE_GRID
	map_block Field_Map220, .LField_Map220_ANIMATION
	map_block Field_Map220, .LField_Map220_BLEND
	map_block Field_Map220, .LField_Map220_SCENE
.LField_Map220_METATILES:
	.incbin "games/tla/src/map/map_220/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map220_SHAPES:
	.incbin "games/tla/src/map/map_220/shapes.table.lz"
	.balign 4, 0
.LField_Map220_GRID:
	.incbin "games/tla/src/map/map_220/grid.parts.lz"
	.balign 4, 0
.LField_Map220_SHAPE_GRID:
	.incbin "games/tla/src/map/map_220/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map220_ANIMATION:
	.incbin "games/tla/src/map/map_220/animation.script.lz"
	.balign 4, 0
.LField_Map220_BLEND:
	.incbin "games/tla/src/map/map_220/blend.script.lz"
	.balign 4, 0
.LField_Map220_SCENE:
	.incbin "games/tla/src/map/map_220/scene.parts.lz"
