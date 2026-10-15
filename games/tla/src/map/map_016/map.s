	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map016
Field_Map016:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map016, .LField_Map016_METATILES
	map_block Field_Map016, .LField_Map016_SHAPES
	map_block Field_Map016, .LField_Map016_GRID
	map_block Field_Map016, .LField_Map016_SHAPE_GRID
	map_block Field_Map016, .LField_Map016_ANIMATION
	map_block Field_Map016, .LField_Map016_BLEND
	map_block Field_Map016, .LField_Map016_SCENE
.LField_Map016_METATILES:
	.incbin "games/tla/src/map/map_016/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map016_SHAPES:
	.incbin "games/tla/src/map/map_016/shapes.table.lz"
	.balign 4, 0
.LField_Map016_GRID:
	.incbin "games/tla/src/map/map_016/grid.parts.lz"
	.balign 4, 0
.LField_Map016_SHAPE_GRID:
	.incbin "games/tla/src/map/map_016/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map016_ANIMATION:
	.incbin "games/tla/src/map/map_016/animation.script.lz"
	.balign 4, 0
.LField_Map016_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map016_SCENE:
	.incbin "games/tla/src/map/map_016/scene.parts.lz"
	.balign 4, 0
