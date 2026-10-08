	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map115
Field_Map115:
	map_header 0, 0, 128, 70, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map115, .LField_Map115_METATILES
	map_block Field_Map115, .LField_Map115_SHAPES
	map_block Field_Map115, .LField_Map115_GRID
	map_block Field_Map115, .LField_Map115_SHAPE_GRID
	map_block Field_Map115, .LField_Map115_ANIMATION
	map_block Field_Map115, .LField_Map115_BLEND
	map_block Field_Map115, .LField_Map115_SCENE
.LField_Map115_METATILES:
	.incbin "map/map_115/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "map/map_115/shapes.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "map/map_115/grid.parts.lz"
	.balign 4, 0
.LField_Map115_SHAPE_GRID:
	.incbin "map/map_115/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map115_SCENE:
	.incbin "map/map_115/scene.parts.lz"
