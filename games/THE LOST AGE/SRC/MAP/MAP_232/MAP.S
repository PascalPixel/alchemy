@ Field_Map232: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map232
Field_Map232:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map232, .LField_Map232_METATILES
	map_block Field_Map232, .LField_Map232_SHAPES
	map_block Field_Map232, .LField_Map232_GRID
	map_block Field_Map232, .LField_Map232_SHAPE_GRID
	map_block Field_Map232, .LField_Map232_ANIMATION
	map_block Field_Map232, .LField_Map232_BLEND
	map_block Field_Map232, .LField_Map232_SCENE
.LField_Map232_METATILES:
	.incbin "MAP/MAP_232/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map232_SHAPES:
	.incbin "MAP/MAP_232/SHAPES.table.lz"
	.balign 4, 0
.LField_Map232_GRID:
	.incbin "MAP/MAP_232/GRID.parts.lz"
	.balign 4, 0
.LField_Map232_SHAPE_GRID:
	.incbin "MAP/MAP_232/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map232_ANIMATION:
	.incbin "MAP/MAP_182/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map232_BLEND:
	.incbin "MAP/MAP_232/BLEND.script.lz"
	.balign 4, 0
.LField_Map232_SCENE:
	.incbin "MAP/MAP_232/SCENE.parts.lz"
