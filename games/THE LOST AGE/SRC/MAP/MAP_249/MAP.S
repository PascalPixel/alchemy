@ Field_Map249: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map249
Field_Map249:
	map_header 0, 0, 204, 64, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map249, .LField_Map249_METATILES
	map_block Field_Map249, .LField_Map249_SHAPES
	map_block Field_Map249, .LField_Map249_GRID
	map_block Field_Map249, .LField_Map249_SHAPE_GRID
	map_block Field_Map249, .LField_Map249_ANIMATION
	map_block Field_Map249
	map_block Field_Map249, .LField_Map249_SCENE
.LField_Map249_METATILES:
	.incbin "MAP/MAP_249/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map249_SHAPES:
	.incbin "MAP/MAP_249/SHAPES.table.lz"
	.balign 4, 0
.LField_Map249_GRID:
	.incbin "MAP/MAP_249/GRID.parts.lz"
	.balign 4, 0
.LField_Map249_SHAPE_GRID:
	.incbin "MAP/MAP_249/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map249_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map249_SCENE:
	.incbin "MAP/MAP_249/SCENE.parts.lz"
