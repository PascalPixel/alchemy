	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map017
Field_Map017:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map017, .LField_Map017_METATILES
	map_block Field_Map017, .LField_Map017_SHAPES
	map_block Field_Map017, .LField_Map017_GRID
	map_block Field_Map017, .LField_Map017_SHAPE_GRID
	map_block Field_Map017, .LField_Map017_ANIMATION
	map_block Field_Map017, .LField_Map017_BLEND
	map_block Field_Map017, .LField_Map017_SCENE
.LField_Map017_METATILES:
	.incbin "games/tla/src/map/map_017/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map017_SHAPES:
	.incbin "games/tla/src/map/map_017/shapes.table.lz"
	.balign 4, 0
.LField_Map017_GRID:
	.incbin "games/tla/src/map/map_017/grid.parts.lz"
	.balign 4, 0
.LField_Map017_SHAPE_GRID:
	.incbin "games/tla/src/map/map_017/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map017_ANIMATION:
	.incbin "games/tla/src/map/map_017/animation.script.lz"
	.balign 4, 0
.LField_Map017_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map017_SCENE:
	.incbin "games/tla/src/map/map_017/scene.parts.lz"
	.balign 4, 0
