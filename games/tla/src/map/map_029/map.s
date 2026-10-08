	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map029
Field_Map029:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map029, .LField_Map029_METATILES
	map_block Field_Map029, .LField_Map029_SHAPES
	map_block Field_Map029, .LField_Map029_GRID
	map_block Field_Map029, .LField_Map029_SHAPE_GRID
	map_block Field_Map029, .LField_Map029_ANIMATION
	map_block Field_Map029, .LField_Map029_BLEND
	map_block Field_Map029, .LField_Map029_SCENE
.LField_Map029_METATILES:
	.incbin "map/map_029/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map029_SHAPES:
	.incbin "map/map_029/shapes.table.lz"
	.balign 4, 0
.LField_Map029_GRID:
	.incbin "map/map_029/grid.parts.lz"
	.balign 4, 0
.LField_Map029_SHAPE_GRID:
	.incbin "map/map_029/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map029_ANIMATION:
	.incbin "map/map_021/animation.script.lz"
	.balign 4, 0
.LField_Map029_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map029_SCENE:
	.incbin "map/map_029/scene.parts.lz"
