	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map007
Field_Map007:
	map_header 0, 0, 128, 128, 3, 3, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, -1, -2, 0x1, 0x1
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map007, .LField_Map007_METATILES
	map_block Field_Map007, .LField_Map007_SHAPES
	map_block Field_Map007, .LField_Map007_GRID
	map_block Field_Map007, .LField_Map007_SHAPE_GRID
	map_block Field_Map007, .LField_Map007_ANIMATION
	map_block Field_Map007, .LField_Map007_BLEND
	map_block Field_Map007, .LField_Map007_SCENE
.LField_Map007_METATILES:
	.incbin "games/tla/src/map/map_007/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map007_SHAPES:
	.incbin "games/tla/src/map/map_007/shapes.table.lz"
	.balign 4, 0
.LField_Map007_GRID:
	.incbin "games/tla/src/map/map_007/grid.parts.lz"
	.balign 4, 0
.LField_Map007_SHAPE_GRID:
	.incbin "games/tla/src/map/map_007/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map007_ANIMATION:
	.incbin "games/tla/src/map/map_007/animation.script.lz"
	.balign 4, 0
.LField_Map007_BLEND:
	.incbin "games/tla/src/map/map_007/blend.script.lz"
	.balign 4, 0
.LField_Map007_SCENE:
	.incbin "games/tla/src/map/map_007/scene.parts.lz"
