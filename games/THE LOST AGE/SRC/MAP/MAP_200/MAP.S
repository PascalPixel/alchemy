@ Field_Map200: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map200
Field_Map200:
	map_header 0, 0, 128, 128, 2, 2, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0x1f, 0x1f
	map_layer 0, 128, 12, 12, 0, 0, 0xff, 0xff
	map_block Field_Map200, .LField_Map200_METATILES
	map_block Field_Map200, .LField_Map200_SHAPES
	map_block Field_Map200, .LField_Map200_GRID
	map_block Field_Map200, .LField_Map200_SHAPE_GRID
	map_block Field_Map200, .LField_Map200_ANIMATION
	map_block Field_Map200
	map_block Field_Map200, .LField_Map200_SCENE
.LField_Map200_METATILES:
	.incbin "MAP/MAP_200/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map200_SHAPES:
	.incbin "MAP/MAP_200/SHAPES.table.lz"
	.balign 4, 0
.LField_Map200_GRID:
	.incbin "MAP/MAP_200/GRID.parts.lz"
	.balign 4, 0
.LField_Map200_SHAPE_GRID:
	.incbin "MAP/MAP_200/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map200_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map200_SCENE:
	.incbin "MAP/MAP_200/SCENE.parts.lz"
