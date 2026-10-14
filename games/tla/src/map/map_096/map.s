	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map096
Field_Map096:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map096, .LField_Map096_METATILES
	map_block Field_Map096, .LField_Map096_SHAPES
	map_block Field_Map096, .LField_Map096_GRID
	map_block Field_Map096, .LField_Map096_SHAPE_GRID
	map_block Field_Map096, .LField_Map096_ANIMATION
	map_block Field_Map096, .LField_Map096_BLEND
	map_block Field_Map096, .LField_Map096_SCENE
.LField_Map096_METATILES:
	.incbin "games/tla/src/map/map_096/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map096_SHAPES:
	.incbin "games/tla/src/map/map_096/shapes.table.lz"
	.balign 4, 0
.LField_Map096_GRID:
	.incbin "games/tla/src/map/map_096/grid.parts.lz"
	.balign 4, 0
.LField_Map096_SHAPE_GRID:
	.incbin "games/tla/src/map/map_096/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map096_ANIMATION:
	.incbin "games/tla/src/map/map_096/animation.script.lz"
	.balign 4, 0
.LField_Map096_BLEND:
	.incbin "games/common/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map096_SCENE:
	.incbin "games/tla/src/map/map_096/scene.parts.lz"
