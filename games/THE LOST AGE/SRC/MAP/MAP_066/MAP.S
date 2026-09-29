@ Field_Map066: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map066
Field_Map066:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map066, .LField_Map066_METATILES
	map_block Field_Map066, .LField_Map066_SHAPES
	map_block Field_Map066, .LField_Map066_GRID
	map_block Field_Map066, .LField_Map066_SHAPE_GRID
	map_block Field_Map066, .LField_Map066_ANIMATION
	map_block Field_Map066, .LField_Map066_BLEND
	map_block Field_Map066, .LField_Map066_SCENE
.LField_Map066_METATILES:
	.incbin "MAP/MAP_066/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map066_SHAPES:
	.incbin "MAP/MAP_066/SHAPES.table.lz"
	.balign 4, 0
.LField_Map066_GRID:
	.incbin "MAP/MAP_066/GRID.parts.lz"
	.balign 4, 0
.LField_Map066_SHAPE_GRID:
	.incbin "MAP/MAP_066/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map066_ANIMATION:
	.incbin "MAP/MAP_066/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map066_BLEND:
	.incbin "MAP/MAP_066/BLEND.script.lz"
	.balign 4, 0
.LField_Map066_SCENE:
	.incbin "MAP/MAP_066/SCENE.parts.lz"
