	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map119
Field_Map119:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map119, .LField_Map119_METATILES
	map_block Field_Map119, .LField_Map119_SHAPES
	map_block Field_Map119, .LField_Map119_GRID
	map_block Field_Map119, .LField_Map119_SHAPE_GRID
	map_block Field_Map119, .LField_Map119_ANIMATION
	map_block Field_Map119, .LField_Map119_BLEND
	map_block Field_Map119, .LField_Map119_SCENE
.LField_Map119_METATILES:
	.incbin "map/map_119/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map119_SHAPES:
	.incbin "map/map_119/shapes.table.lz"
	.balign 4, 0
.LField_Map119_GRID:
	.incbin "map/map_119/grid.parts.lz"
	.balign 4, 0
.LField_Map119_SHAPE_GRID:
	.incbin "map/map_119/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map119_ANIMATION:
	.incbin "map/map_021/animation.script.lz"
	.balign 4, 0
.LField_Map119_BLEND:
	.incbin "map/map_021/blend.script.lz"
	.balign 4, 0
.LField_Map119_SCENE:
	.incbin "map/map_119/scene.parts.lz"
