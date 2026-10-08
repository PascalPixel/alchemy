	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map148
Field_Map148:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map148, .LField_Map148_METATILES
	map_block Field_Map148, .LField_Map148_SHAPES
	map_block Field_Map148, .LField_Map148_GRID
	map_block Field_Map148, .LField_Map148_SHAPE_GRID
	map_block Field_Map148, .LField_Map148_ANIMATION
	map_block Field_Map148, .LField_Map148_BLEND
	map_block Field_Map148, .LField_Map148_SCENE
.LField_Map148_METATILES:
	.incbin "map/map_148/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map148_SHAPES:
	.incbin "map/map_148/shapes.table.lz"
	.balign 4, 0
.LField_Map148_GRID:
	.incbin "map/map_148/grid.parts.lz"
	.balign 4, 0
.LField_Map148_SHAPE_GRID:
	.incbin "map/map_148/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map148_ANIMATION:
	.incbin "map/map_148/animation.script.lz"
	.balign 4, 0
.LField_Map148_BLEND:
	.incbin "map/map_148/blend.script.lz"
	.balign 4, 0
.LField_Map148_SCENE:
	.incbin "map/map_148/scene.parts.lz"
