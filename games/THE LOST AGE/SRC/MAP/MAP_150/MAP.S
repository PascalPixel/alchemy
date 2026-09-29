@ Field_Map150: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map150
Field_Map150:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map150, .LField_Map150_METATILES
	map_block Field_Map150, .LField_Map150_SHAPES
	map_block Field_Map150, .LField_Map150_GRID
	map_block Field_Map150, .LField_Map150_SHAPE_GRID
	map_block Field_Map150, .LField_Map150_ANIMATION
	map_block Field_Map150, .LField_Map150_BLEND
	map_block Field_Map150, .LField_Map150_SCENE
.LField_Map150_METATILES:
	.incbin "MAP/MAP_150/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map150_SHAPES:
	.incbin "MAP/MAP_150/SHAPES.table.lz"
	.balign 4, 0
.LField_Map150_GRID:
	.incbin "MAP/MAP_150/GRID.parts.lz"
	.balign 4, 0
.LField_Map150_SHAPE_GRID:
	.incbin "MAP/MAP_150/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map150_ANIMATION:
	.incbin "MAP/MAP_150/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map150_BLEND:
	.incbin "MAP/MAP_150/BLEND.script.lz"
	.balign 4, 0
.LField_Map150_SCENE:
	.incbin "MAP/MAP_150/SCENE.parts.lz"
