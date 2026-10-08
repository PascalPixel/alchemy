	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map164
Field_Map164:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map164, .LField_Map164_METATILES
	map_block Field_Map164, .LField_Map164_SHAPES
	map_block Field_Map164, .LField_Map164_GRID
	map_block Field_Map164, .LField_Map164_SHAPE_GRID
	map_block Field_Map164, .LField_Map164_ANIMATION
	map_block Field_Map164, .LField_Map164_BLEND
	map_block Field_Map164, .LField_Map164_SCENE
.LField_Map164_METATILES:
	.incbin "map/map_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "map/map_164/shapes.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "map/map_164/grid.parts.lz"
	.balign 4, 0
.LField_Map164_SHAPE_GRID:
	.incbin "map/map_164/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map164_SCENE:
	.incbin "map/map_164/scene.parts.lz"
