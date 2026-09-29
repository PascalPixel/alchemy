@ Field_Map112: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map112
Field_Map112:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map112, .LField_Map112_METATILES
	map_block Field_Map112, .LField_Map112_SHAPES
	map_block Field_Map112, .LField_Map112_GRID
	map_block Field_Map112, .LField_Map112_SHAPE_GRID
	map_block Field_Map112, .LField_Map112_ANIMATION
	map_block Field_Map112, .LField_Map112_BLEND
	map_block Field_Map112, .LField_Map112_SCENE
.LField_Map112_METATILES:
	.incbin "MAP/MAP_112/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map112_SHAPES:
	.incbin "MAP/MAP_112/SHAPES.table.lz"
	.balign 4, 0
.LField_Map112_GRID:
	.incbin "MAP/MAP_112/GRID.parts.lz"
	.balign 4, 0
.LField_Map112_SHAPE_GRID:
	.incbin "MAP/MAP_112/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map112_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map112_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map112_SCENE:
	.incbin "MAP/MAP_112/SCENE.parts.lz"
