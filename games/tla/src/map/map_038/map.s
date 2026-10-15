	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map038
Field_Map038:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map038, .LField_Map038_METATILES
	map_block Field_Map038, .LField_Map038_SHAPES
	map_block Field_Map038, .LField_Map038_GRID
	map_block Field_Map038, .LField_Map038_SHAPE_GRID
	map_block Field_Map038, .LField_Map038_ANIMATION
	map_block Field_Map038, .LField_Map038_BLEND
	map_block Field_Map038, .LField_Map038_SCENE
.LField_Map038_METATILES:
	.incbin "games/tla/src/map/map_038/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map038_SHAPES:
	.incbin "games/common/src/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map038_GRID:
	.incbin "games/tla/src/map/map_038/grid.parts.lz"
	.balign 4, 0
.LField_Map038_SHAPE_GRID:
	.incbin "games/tla/src/map/map_038/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map038_ANIMATION:
	.incbin "games/tla/src/map/map_038/animation.script.lz"
	.balign 4, 0
.LField_Map038_BLEND:
	.incbin "games/tla/src/map/map_038/blend.script.lz"
	.balign 4, 0
.LField_Map038_SCENE:
	.incbin "games/tla/src/map/map_038/scene.parts.lz"
	.balign 4, 0
