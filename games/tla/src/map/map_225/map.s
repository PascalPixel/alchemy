	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map225
Field_Map225:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map225, .LField_Map225_METATILES
	map_block Field_Map225, .LField_Map225_SHAPES
	map_block Field_Map225, .LField_Map225_GRID
	map_block Field_Map225, .LField_Map225_SHAPE_GRID
	map_block Field_Map225, .LField_Map225_ANIMATION
	map_block Field_Map225, .LField_Map225_BLEND
	map_block Field_Map225, .LField_Map225_SCENE
.LField_Map225_METATILES:
	.incbin "map/map_225/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map225_SHAPES:
	.incbin "map/map_225/shapes.table.lz"
	.balign 4, 0
.LField_Map225_GRID:
	.incbin "map/map_225/grid.parts.lz"
	.balign 4, 0
.LField_Map225_SHAPE_GRID:
	.incbin "map/map_225/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map225_ANIMATION:
	.incbin "map/map_224/animation.script.lz"
	.balign 4, 0
.LField_Map225_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map225_SCENE:
	.incbin "map/map_225/scene.parts.lz"
