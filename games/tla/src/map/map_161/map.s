	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map161
Field_Map161:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map161, .LField_Map161_METATILES
	map_block Field_Map161, .LField_Map161_SHAPES
	map_block Field_Map161, .LField_Map161_GRID
	map_block Field_Map161, .LField_Map161_SHAPE_GRID
	map_block Field_Map161, .LField_Map161_ANIMATION
	map_block Field_Map161, .LField_Map161_BLEND
	map_block Field_Map161, .LField_Map161_SCENE
.LField_Map161_METATILES:
	.incbin "games/tla/src/map/map_161/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map161_SHAPES:
	.incbin "games/tla/src/map/map_161/shapes.table.lz"
	.balign 4, 0
.LField_Map161_GRID:
	.incbin "games/tla/src/map/map_161/grid.parts.lz"
	.balign 4, 0
.LField_Map161_SHAPE_GRID:
	.incbin "games/tla/src/map/map_161/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map161_ANIMATION:
	.incbin "games/tla/src/map/map_161/animation.script.lz"
	.balign 4, 0
.LField_Map161_BLEND:
	.incbin "games/tla/src/map/map_161/blend.script.lz"
	.balign 4, 0
.LField_Map161_SCENE:
	.incbin "games/tla/src/map/map_161/scene.parts.lz"
	.balign 4, 0
