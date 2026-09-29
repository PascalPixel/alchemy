@ Field_Map202: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map202
Field_Map202:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map202, .LField_Map202_METATILES
	map_block Field_Map202, .LField_Map202_SHAPES
	map_block Field_Map202, .LField_Map202_GRID
	map_block Field_Map202, .LField_Map202_SHAPE_GRID
	map_block Field_Map202, .LField_Map202_ANIMATION
	map_block Field_Map202, .LField_Map202_BLEND
	map_block Field_Map202, .LField_Map202_SCENE
.LField_Map202_METATILES:
	.incbin "MAP/MAP_202/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map202_SHAPES:
	.incbin "MAP/MAP_202/SHAPES.table.lz"
	.balign 4, 0
.LField_Map202_GRID:
	.incbin "MAP/MAP_202/GRID.parts.lz"
	.balign 4, 0
.LField_Map202_SHAPE_GRID:
	.incbin "MAP/MAP_202/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map202_ANIMATION:
	.incbin "MAP/MAP_202/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map202_BLEND:
	.incbin "MAP/MAP_202/BLEND.script.lz"
	.balign 4, 0
.LField_Map202_SCENE:
	.incbin "MAP/MAP_202/SCENE.parts.lz"
