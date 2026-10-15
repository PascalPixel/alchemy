	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map201
Field_Map201:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map201, .LField_Map201_METATILES
	map_block Field_Map201, .LField_Map201_SHAPES
	map_block Field_Map201, .LField_Map201_GRID
	map_block Field_Map201, .LField_Map201_SHAPE_GRID
	map_block Field_Map201, .LField_Map201_ANIMATION
	map_block Field_Map201, .LField_Map201_BLEND
	map_block Field_Map201, .LField_Map201_SCENE
.LField_Map201_METATILES:
	.incbin "games/tla/src/map/map_201/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map201_SHAPES:
	.incbin "games/tla/src/map/map_072/shapes.table.lz"
	.balign 4, 0
.LField_Map201_GRID:
	.incbin "games/tla/src/map/map_201/grid.parts.lz"
	.balign 4, 0
.LField_Map201_SHAPE_GRID:
	.incbin "games/tla/src/map/map_201/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map201_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map201_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map201_SCENE:
	.incbin "games/tla/src/map/map_201/scene.parts.lz"
	.balign 4, 0
