	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map166
Field_Map166:
	map_header 0, 0, 128, 128, 2, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 164, 4, 4, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map166, .LField_Map166_METATILES
	map_block Field_Map166, .LField_Map166_SHAPES
	map_block Field_Map166, .LField_Map166_GRID
	map_block Field_Map166, .LField_Map166_SHAPE_GRID
	map_block Field_Map166, .LField_Map166_ANIMATION
	map_block Field_Map166, .LField_Map166_BLEND
	map_block Field_Map166, .LField_Map166_SCENE
.LField_Map166_METATILES:
	.incbin "games/tla/src/map/map_166/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map166_SHAPES:
	.incbin "games/tla/src/map/map_166/shapes.table.lz"
	.balign 4, 0
.LField_Map166_GRID:
	.incbin "games/tla/src/map/map_166/grid.parts.lz"
	.balign 4, 0
.LField_Map166_SHAPE_GRID:
	.incbin "games/tla/src/map/map_166/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map166_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map166_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map166_SCENE:
	.incbin "games/tla/src/map/map_166/scene.parts.lz"
	.balign 4, 0
