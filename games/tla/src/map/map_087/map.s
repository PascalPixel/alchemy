	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map087
Field_Map087:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map087, .LField_Map087_METATILES
	map_block Field_Map087, .LField_Map087_SHAPES
	map_block Field_Map087, .LField_Map087_GRID
	map_block Field_Map087, .LField_Map087_SHAPE_GRID
	map_block Field_Map087, .LField_Map087_ANIMATION
	map_block Field_Map087, .LField_Map087_BLEND
	map_block Field_Map087, .LField_Map087_SCENE
.LField_Map087_METATILES:
	.incbin "games/tla/src/map/map_087/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map087_SHAPES:
	.incbin "games/tla/src/map/map_087/shapes.table.lz"
	.balign 4, 0
.LField_Map087_GRID:
	.incbin "games/tla/src/map/map_087/grid.parts.lz"
	.balign 4, 0
.LField_Map087_SHAPE_GRID:
	.incbin "games/tla/src/map/map_087/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map087_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map087_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map087_SCENE:
	.incbin "games/tla/src/map/map_087/scene.parts.lz"
