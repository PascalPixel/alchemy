	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map037
Field_Map037:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map037, .LField_Map037_METATILES
	map_block Field_Map037, .LField_Map037_SHAPES
	map_block Field_Map037, .LField_Map037_GRID
	map_block Field_Map037, .LField_Map037_SHAPE_GRID
	map_block Field_Map037, .LField_Map037_ANIMATION
	map_block Field_Map037, .LField_Map037_BLEND
	map_block Field_Map037, .LField_Map037_SCENE
.LField_Map037_METATILES:
	.incbin "games/tla/src/map/map_037/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map037_SHAPES:
	.incbin "games/tla/src/map/map_037/shapes.table.lz"
	.balign 4, 0
.LField_Map037_GRID:
	.incbin "games/tla/src/map/map_037/grid.parts.lz"
	.balign 4, 0
.LField_Map037_SHAPE_GRID:
	.incbin "games/tla/src/map/map_037/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map037_ANIMATION:
	.incbin "games/tla/src/map/map_035/animation.script.lz"
	.balign 4, 0
.LField_Map037_BLEND:
	.incbin "games/tla/src/map/map_035/blend.script.lz"
	.balign 4, 0
.LField_Map037_SCENE:
	.incbin "games/tla/src/map/map_037/scene.parts.lz"
