	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map176
Field_Map176:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map176, .LField_Map176_METATILES
	map_block Field_Map176, .LField_Map176_SHAPES
	map_block Field_Map176, .LField_Map176_GRID
	map_block Field_Map176, .LField_Map176_SHAPE_GRID
	map_block Field_Map176, .LField_Map176_ANIMATION
	map_block Field_Map176, .LField_Map176_BLEND
	map_block Field_Map176, .LField_Map176_SCENE
.LField_Map176_METATILES:
	.incbin "games/tla/src/map/map_176/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map176_SHAPES:
	.incbin "games/tla/src/map/map_176/shapes.table.lz"
	.balign 4, 0
.LField_Map176_GRID:
	.incbin "games/tla/src/map/map_176/grid.parts.lz"
	.balign 4, 0
.LField_Map176_SHAPE_GRID:
	.incbin "games/tla/src/map/map_176/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map176_ANIMATION:
	.incbin "games/tla/src/map/map_176/animation.script.lz"
	.balign 4, 0
.LField_Map176_BLEND:
	.incbin "games/tla/src/map/map_057/blend.script.lz"
	.balign 4, 0
.LField_Map176_SCENE:
	.incbin "games/tla/src/map/map_176/scene.parts.lz"
	.balign 4, 0
