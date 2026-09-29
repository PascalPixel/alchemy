@ Field_Map222: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map222
Field_Map222:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map222, .LField_Map222_METATILES
	map_block Field_Map222, .LField_Map222_SHAPES
	map_block Field_Map222, .LField_Map222_GRID
	map_block Field_Map222, .LField_Map222_SHAPE_GRID
	map_block Field_Map222, .LField_Map222_ANIMATION
	map_block Field_Map222, .LField_Map222_BLEND
	map_block Field_Map222, .LField_Map222_SCENE
.LField_Map222_METATILES:
	.incbin "MAP/MAP_222/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map222_SHAPES:
	.incbin "MAP/MAP_222/SHAPES.table.lz"
	.balign 4, 0
.LField_Map222_GRID:
	.incbin "MAP/MAP_222/GRID.parts.lz"
	.balign 4, 0
.LField_Map222_SHAPE_GRID:
	.incbin "MAP/MAP_222/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map222_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map222_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map222_SCENE:
	.incbin "MAP/MAP_222/SCENE.parts.lz"
