@ Field_Map124: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map124
Field_Map124:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map124, .LField_Map124_METATILES
	map_block Field_Map124, .LField_Map124_SHAPES
	map_block Field_Map124, .LField_Map124_GRID
	map_block Field_Map124, .LField_Map124_SHAPE_GRID
	map_block Field_Map124, .LField_Map124_ANIMATION
	map_block Field_Map124, .LField_Map124_BLEND
	map_block Field_Map124, .LField_Map124_SCENE
.LField_Map124_METATILES:
	.incbin "MAP/MAP_124/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map124_SHAPES:
	.incbin "MAP/MAP_124/SHAPES.table.lz"
	.balign 4, 0
.LField_Map124_GRID:
	.incbin "MAP/MAP_124/GRID.parts.lz"
	.balign 4, 0
.LField_Map124_SHAPE_GRID:
	.incbin "MAP/MAP_124/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map124_ANIMATION:
	.incbin "MAP/MAP_124/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map124_BLEND:
	.incbin "MAP/MAP_010/BLEND.script.lz"
	.balign 4, 0
.LField_Map124_SCENE:
	.incbin "MAP/MAP_124/SCENE.parts.lz"
