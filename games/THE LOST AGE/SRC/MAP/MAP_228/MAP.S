@ Field_Map228: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map228
Field_Map228:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map228, .LField_Map228_METATILES
	map_block Field_Map228, .LField_Map228_SHAPES
	map_block Field_Map228, .LField_Map228_GRID
	map_block Field_Map228, .LField_Map228_SHAPE_GRID
	map_block Field_Map228, .LField_Map228_ANIMATION
	map_block Field_Map228
	map_block Field_Map228, .LField_Map228_SCENE
.LField_Map228_METATILES:
	.incbin "MAP/MAP_228/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map228_SHAPES:
	.incbin "MAP/MAP_228/SHAPES.table.lz"
	.balign 4, 0
.LField_Map228_GRID:
	.incbin "MAP/MAP_228/GRID.parts.lz"
	.balign 4, 0
.LField_Map228_SHAPE_GRID:
	.incbin "MAP/MAP_228/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map228_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map228_SCENE:
	.incbin "MAP/MAP_228/SCENE.parts.lz"
