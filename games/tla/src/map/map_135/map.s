	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map135
Field_Map135:
	map_header 0, 0, 90, 100, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map135, .LField_Map135_METATILES
	map_block Field_Map135, .LField_Map135_SHAPES
	map_block Field_Map135, .LField_Map135_GRID
	map_block Field_Map135, .LField_Map135_SHAPE_GRID
	map_block Field_Map135, .LField_Map135_ANIMATION
	map_block Field_Map135, .LField_Map135_BLEND
	map_block Field_Map135, .LField_Map135_SCENE
.LField_Map135_METATILES:
	.incbin "map/map_135/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map135_SHAPES:
	.incbin "map/map_135/shapes.table.lz"
	.balign 4, 0
.LField_Map135_GRID:
	.incbin "map/map_135/grid.parts.lz"
	.balign 4, 0
.LField_Map135_SHAPE_GRID:
	.incbin "map/map_135/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map135_ANIMATION:
	.incbin "map/map_135/animation.script.lz"
	.balign 4, 0
.LField_Map135_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map135_SCENE:
	.incbin "map/map_135/scene.parts.lz"
