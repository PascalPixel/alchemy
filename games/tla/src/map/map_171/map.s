	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map171
Field_Map171:
	map_header 0, 0, 130, 112, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map171, .LField_Map171_METATILES
	map_block Field_Map171, .LField_Map171_SHAPES
	map_block Field_Map171, .LField_Map171_GRID
	map_block Field_Map171, .LField_Map171_SHAPE_GRID
	map_block Field_Map171, .LField_Map171_ANIMATION
	map_block Field_Map171, .LField_Map171_BLEND
	map_block Field_Map171, .LField_Map171_SCENE
.LField_Map171_METATILES:
	.incbin "games/tla/src/map/map_171/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map171_SHAPES:
	.incbin "games/tla/src/map/map_171/shapes.table.lz"
	.balign 4, 0
.LField_Map171_GRID:
	.incbin "games/tla/src/map/map_171/grid.parts.lz"
	.balign 4, 0
.LField_Map171_SHAPE_GRID:
	.incbin "games/tla/src/map/map_171/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map171_ANIMATION:
	.incbin "games/tla/src/map/map_168/animation.script.lz"
	.balign 4, 0
.LField_Map171_BLEND:
	.incbin "games/tla/src/map/map_169/blend.script.lz"
	.balign 4, 0
.LField_Map171_SCENE:
	.incbin "games/tla/src/map/map_171/scene.parts.lz"
	.balign 4, 0
