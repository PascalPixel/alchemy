@ Field_Map242: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map242
Field_Map242:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map242, .LField_Map242_METATILES
	map_block Field_Map242, .LField_Map242_SHAPES
	map_block Field_Map242, .LField_Map242_GRID
	map_block Field_Map242, .LField_Map242_SHAPE_GRID
	map_block Field_Map242, .LField_Map242_ANIMATION
	map_block Field_Map242
	map_block Field_Map242, .LField_Map242_SCENE
.LField_Map242_METATILES:
	.incbin "MAP/MAP_242/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map242_SHAPES:
	.incbin "MAP/MAP_242/SHAPES.table.lz"
	.balign 4, 0
.LField_Map242_GRID:
	.incbin "MAP/MAP_242/GRID.parts.lz"
	.balign 4, 0
.LField_Map242_SHAPE_GRID:
	.incbin "MAP/MAP_242/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map242_ANIMATION:
	.incbin "MAP/MAP_242/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map242_SCENE:
	.incbin "MAP/MAP_242/SCENE.parts.lz"
