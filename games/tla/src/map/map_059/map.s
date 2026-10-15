	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map059
Field_Map059:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map059, .LField_Map059_METATILES
	map_block Field_Map059, .LField_Map059_SHAPES
	map_block Field_Map059, .LField_Map059_GRID
	map_block Field_Map059, .LField_Map059_SHAPE_GRID
	map_block Field_Map059, .LField_Map059_ANIMATION
	map_block Field_Map059, .LField_Map059_BLEND
	map_block Field_Map059, .LField_Map059_SCENE
.LField_Map059_METATILES:
	.incbin "games/tla/src/map/map_059/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map059_SHAPES:
	.incbin "games/tla/src/map/map_059/shapes.table.lz"
	.balign 4, 0
.LField_Map059_GRID:
	.incbin "games/tla/src/map/map_059/grid.parts.lz"
	.balign 4, 0
.LField_Map059_SHAPE_GRID:
	.incbin "games/tla/src/map/map_059/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map059_ANIMATION:
	.incbin "games/tla/src/map/map_052/animation.script.lz"
	.balign 4, 0
.LField_Map059_BLEND:
	.incbin "games/common/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map059_SCENE:
	.incbin "games/tla/src/map/map_059/scene.parts.lz"
	.balign 4, 0
