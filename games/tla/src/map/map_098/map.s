	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map098
Field_Map098:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map098, .LField_Map098_METATILES
	map_block Field_Map098, .LField_Map098_SHAPES
	map_block Field_Map098, .LField_Map098_GRID
	map_block Field_Map098, .LField_Map098_SHAPE_GRID
	map_block Field_Map098, .LField_Map098_ANIMATION
	map_block Field_Map098, .LField_Map098_BLEND
	map_block Field_Map098, .LField_Map098_SCENE
.LField_Map098_METATILES:
	.incbin "map/map_098/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map098_SHAPES:
	.incbin "map/map_098/shapes.table.lz"
	.balign 4, 0
.LField_Map098_GRID:
	.incbin "map/map_098/grid.parts.lz"
	.balign 4, 0
.LField_Map098_SHAPE_GRID:
	.incbin "map/map_098/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map098_ANIMATION:
	.incbin "map/map_097/animation.script.lz"
	.balign 4, 0
.LField_Map098_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map098_SCENE:
	.incbin "map/map_098/scene.parts.lz"
