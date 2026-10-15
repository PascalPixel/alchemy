	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map035
Field_Map035:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map035, .LField_Map035_METATILES
	map_block Field_Map035, .LField_Map035_SHAPES
	map_block Field_Map035, .LField_Map035_GRID
	map_block Field_Map035, .LField_Map035_SHAPE_GRID
	map_block Field_Map035, .LField_Map035_ANIMATION
	map_block Field_Map035, .LField_Map035_BLEND
	map_block Field_Map035, .LField_Map035_SCENE
.LField_Map035_METATILES:
	.incbin "games/tla/src/map/map_035/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map035_SHAPES:
	.incbin "games/tla/src/map/map_035/shapes.table.lz"
	.balign 4, 0
.LField_Map035_GRID:
	.incbin "games/tla/src/map/map_035/grid.parts.lz"
	.balign 4, 0
.LField_Map035_SHAPE_GRID:
	.incbin "games/tla/src/map/map_035/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map035_ANIMATION:
	.incbin "games/tla/src/map/map_035/animation.script.lz"
	.balign 4, 0
.LField_Map035_BLEND:
	.incbin "games/tla/src/map/map_035/blend.script.lz"
	.balign 4, 0
.LField_Map035_SCENE:
	.incbin "games/tla/src/map/map_035/scene.parts.lz"
	.balign 4, 0
