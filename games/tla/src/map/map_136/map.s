	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map136
Field_Map136:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map136, .LField_Map136_METATILES
	map_block Field_Map136, .LField_Map136_SHAPES
	map_block Field_Map136, .LField_Map136_GRID
	map_block Field_Map136, .LField_Map136_SHAPE_GRID
	map_block Field_Map136, .LField_Map136_ANIMATION
	map_block Field_Map136, .LField_Map136_BLEND
	map_block Field_Map136, .LField_Map136_SCENE
.LField_Map136_METATILES:
	.incbin "games/tla/src/map/map_136/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map136_SHAPES:
	.incbin "games/tla/src/map/map_136/shapes.table.lz"
	.balign 4, 0
.LField_Map136_GRID:
	.incbin "games/tla/src/map/map_136/grid.parts.lz"
	.balign 4, 0
.LField_Map136_SHAPE_GRID:
	.incbin "games/tla/src/map/map_136/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map136_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map136_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map136_SCENE:
	.incbin "games/tla/src/map/map_136/scene.parts.lz"
	.balign 4, 0
