@ Field_Map250: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map250
Field_Map250:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map250, .LField_Map250_METATILES
	map_block Field_Map250, .LField_Map250_SHAPES
	map_block Field_Map250, .LField_Map250_GRID
	map_block Field_Map250, .LField_Map250_SHAPE_GRID
	map_block Field_Map250, .LField_Map250_ANIMATION
	map_block Field_Map250
	map_block Field_Map250, .LField_Map250_SCENE
.LField_Map250_METATILES:
	.incbin "MAP/MAP_250/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map250_SHAPES:
	.incbin "MAP/MAP_250/SHAPES.table.lz"
	.balign 4, 0
.LField_Map250_GRID:
	.incbin "MAP/MAP_250/GRID.parts.lz"
	.balign 4, 0
.LField_Map250_SHAPE_GRID:
	.incbin "MAP/MAP_250/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map250_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map250_SCENE:
	.incbin "MAP/MAP_250/SCENE.parts.lz"
