@ Field_Map196: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map196
Field_Map196:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map196, .LField_Map196_METATILES
	map_block Field_Map196, .LField_Map196_SHAPES
	map_block Field_Map196, .LField_Map196_GRID
	map_block Field_Map196, .LField_Map196_SHAPE_GRID
	map_block Field_Map196, .LField_Map196_ANIMATION
	map_block Field_Map196, .LField_Map196_BLEND
	map_block Field_Map196, .LField_Map196_SCENE
.LField_Map196_METATILES:
	.incbin "MAP/MAP_196/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map196_SHAPES:
	.incbin "MAP/MAP_196/SHAPES.table.lz"
	.balign 4, 0
.LField_Map196_GRID:
	.incbin "MAP/MAP_196/GRID.parts.lz"
	.balign 4, 0
.LField_Map196_SHAPE_GRID:
	.incbin "MAP/MAP_196/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map196_ANIMATION:
	.incbin "MAP/MAP_196/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map196_BLEND:
	.incbin "MAP/MAP_196/BLEND.script.lz"
	.balign 4, 0
.LField_Map196_SCENE:
	.incbin "MAP/MAP_196/SCENE.parts.lz"
