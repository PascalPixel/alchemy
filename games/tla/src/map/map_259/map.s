	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map259
Field_Map259:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map259, .LField_Map259_METATILES
	map_block Field_Map259, .LField_Map259_SHAPES
	map_block Field_Map259, .LField_Map259_GRID
	map_block Field_Map259, .LField_Map259_SHAPE_GRID
	map_block Field_Map259, .LField_Map259_ANIMATION
	map_block Field_Map259, .LField_Map259_BLEND
	map_block Field_Map259, .LField_Map259_SCENE
.LField_Map259_METATILES:
	.incbin "map/map_259/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map259_SHAPES:
	.incbin "map/map_259/shapes.table.lz"
	.balign 4, 0
.LField_Map259_GRID:
	.incbin "map/map_259/grid.parts.lz"
	.balign 4, 0
.LField_Map259_SHAPE_GRID:
	.incbin "map/map_259/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map259_ANIMATION:
	.incbin "map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map259_BLEND:
	.incbin "common/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map259_SCENE:
	.incbin "map/map_259/scene.parts.lz"
