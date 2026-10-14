	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map063
Field_Map063:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map063, .LField_Map063_METATILES
	map_block Field_Map063, .LField_Map063_SHAPES
	map_block Field_Map063, .LField_Map063_GRID
	map_block Field_Map063, .LField_Map063_SHAPE_GRID
	map_block Field_Map063, .LField_Map063_ANIMATION
	map_block Field_Map063, .LField_Map063_BLEND
	map_block Field_Map063, .LField_Map063_SCENE
.LField_Map063_METATILES:
	.incbin "games/tla/src/map/map_063/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map063_SHAPES:
	.incbin "games/tla/src/map/map_063/shapes.table.lz"
	.balign 4, 0
.LField_Map063_GRID:
	.incbin "games/tla/src/map/map_063/grid.parts.lz"
	.balign 4, 0
.LField_Map063_SHAPE_GRID:
	.incbin "games/tla/src/map/map_063/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map063_ANIMATION:
	.incbin "games/tla/src/map/map_062/animation.script.lz"
	.balign 4, 0
.LField_Map063_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map063_SCENE:
	.incbin "games/tla/src/map/map_063/scene.parts.lz"
