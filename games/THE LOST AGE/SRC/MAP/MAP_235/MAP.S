@ Field_Map235: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map235
Field_Map235:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map235, .LField_Map235_METATILES
	map_block Field_Map235, .LField_Map235_SHAPES
	map_block Field_Map235, .LField_Map235_GRID
	map_block Field_Map235, .LField_Map235_SHAPE_GRID
	map_block Field_Map235, .LField_Map235_ANIMATION
	map_block Field_Map235, .LField_Map235_BLEND
	map_block Field_Map235, .LField_Map235_SCENE
.LField_Map235_METATILES:
	.incbin "MAP/MAP_235/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map235_SHAPES:
	.incbin "MAP/MAP_235/SHAPES.table.lz"
	.balign 4, 0
.LField_Map235_GRID:
	.incbin "MAP/MAP_235/GRID.parts.lz"
	.balign 4, 0
.LField_Map235_SHAPE_GRID:
	.incbin "MAP/MAP_235/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map235_ANIMATION:
	.incbin "MAP/MAP_235/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map235_BLEND:
	.incbin "MAP/MAP_235/BLEND.script.lz"
	.balign 4, 0
.LField_Map235_SCENE:
	.incbin "MAP/MAP_235/SCENE.parts.lz"
