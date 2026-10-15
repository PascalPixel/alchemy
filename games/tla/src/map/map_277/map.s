	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map277
Field_Map277:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map277, .LField_Map277_METATILES
	map_block Field_Map277, .LField_Map277_SHAPES
	map_block Field_Map277, .LField_Map277_GRID
	map_block Field_Map277, .LField_Map277_SHAPE_GRID
	map_block Field_Map277, .LField_Map277_ANIMATION
	map_block Field_Map277, .LField_Map277_BLEND
	map_block Field_Map277, .LField_Map277_SCENE
.LField_Map277_METATILES:
	.incbin "games/tla/src/map/map_277/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map277_SHAPES:
	.incbin "games/common/src/map/map_058/shapes.table.lz"
	.balign 4, 0
.LField_Map277_GRID:
	.incbin "games/tla/src/map/map_277/grid.parts.lz"
	.balign 4, 0
.LField_Map277_SHAPE_GRID:
	.incbin "games/tla/src/map/map_277/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map277_ANIMATION:
	.incbin "games/common/src/map/map_058/animation.script.lz"
	.balign 4, 0
.LField_Map277_BLEND:
	.incbin "games/tla/src/map/map_277/blend.script.lz"
	.balign 4, 0
.LField_Map277_SCENE:
	.incbin "games/tla/src/map/map_277/scene.parts.lz"
	.balign 4, 0
