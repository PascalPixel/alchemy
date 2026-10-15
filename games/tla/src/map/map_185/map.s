	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map185
Field_Map185:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map185, .LField_Map185_METATILES
	map_block Field_Map185, .LField_Map185_SHAPES
	map_block Field_Map185, .LField_Map185_GRID
	map_block Field_Map185, .LField_Map185_SHAPE_GRID
	map_block Field_Map185, .LField_Map185_ANIMATION
	map_block Field_Map185, .LField_Map185_BLEND
	map_block Field_Map185, .LField_Map185_SCENE
.LField_Map185_METATILES:
	.incbin "games/tla/src/map/map_185/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map185_SHAPES:
	.incbin "games/tla/src/map/map_185/shapes.table.lz"
	.balign 4, 0
.LField_Map185_GRID:
	.incbin "games/tla/src/map/map_185/grid.parts.lz"
	.balign 4, 0
.LField_Map185_SHAPE_GRID:
	.incbin "games/tla/src/map/map_185/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map185_ANIMATION:
	.incbin "games/tla/src/map/map_183/animation.script.lz"
	.balign 4, 0
.LField_Map185_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map185_SCENE:
	.incbin "games/tla/src/map/map_185/scene.parts.lz"
	.balign 4, 0
