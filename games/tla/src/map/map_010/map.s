	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map010
Field_Map010:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map010, .LField_Map010_METATILES
	map_block Field_Map010, .LField_Map010_SHAPES
	map_block Field_Map010, .LField_Map010_GRID
	map_block Field_Map010, .LField_Map010_SHAPE_GRID
	map_block Field_Map010, .LField_Map010_ANIMATION
	map_block Field_Map010, .LField_Map010_BLEND
	map_block Field_Map010, .LField_Map010_SCENE
.LField_Map010_METATILES:
	.incbin "games/tla/src/map/map_010/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map010_SHAPES:
	.incbin "games/tla/src/map/map_010/shapes.table.lz"
	.balign 4, 0
.LField_Map010_GRID:
	.incbin "games/tla/src/map/map_010/grid.parts.lz"
	.balign 4, 0
.LField_Map010_SHAPE_GRID:
	.incbin "games/tla/src/map/map_010/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map010_ANIMATION:
	.incbin "games/tla/src/map/map_010/animation.script.lz"
	.balign 4, 0
.LField_Map010_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map010_SCENE:
	.incbin "games/tla/src/map/map_010/scene.parts.lz"
	.balign 4, 0
