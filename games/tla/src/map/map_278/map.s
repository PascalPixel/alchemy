	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map278
Field_Map278:
	map_header 0, 0, 96, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map278, .LField_Map278_METATILES
	map_block Field_Map278, .LField_Map278_SHAPES
	map_block Field_Map278, .LField_Map278_GRID
	map_block Field_Map278, .LField_Map278_SHAPE_GRID
	map_block Field_Map278, .LField_Map278_ANIMATION
	map_block Field_Map278, .LField_Map278_BLEND
	map_block Field_Map278, .LField_Map278_SCENE
.LField_Map278_METATILES:
	.incbin "games/tla/src/map/map_278/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map278_SHAPES:
	.incbin "games/tla/src/map/map_278/shapes.table.lz"
	.balign 4, 0
.LField_Map278_GRID:
	.incbin "games/tla/src/map/map_278/grid.parts.lz"
	.balign 4, 0
.LField_Map278_SHAPE_GRID:
	.incbin "games/tla/src/map/map_278/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map278_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map278_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map278_SCENE:
	.incbin "games/tla/src/map/map_278/scene.parts.lz"
