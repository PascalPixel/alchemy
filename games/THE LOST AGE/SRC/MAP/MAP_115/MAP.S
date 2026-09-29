@ Field_Map115: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
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
	.incbin "MAP/MAP_115/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "MAP/MAP_115/SHAPES.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "MAP/MAP_115/GRID.parts.lz"
	.balign 4, 0
.LField_Map115_SHAPE_GRID:
	.incbin "MAP/MAP_115/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "MAP/MAP_115/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "MAP/MAP_115/BLEND.script.lz"
	.balign 4, 0
.LField_Map115_SCENE:
	.incbin "MAP/MAP_115/SCENE.parts.lz"
