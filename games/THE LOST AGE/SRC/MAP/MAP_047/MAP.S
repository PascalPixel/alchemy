@ Field_Map047: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map047
Field_Map047:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map047, .LField_Map047_METATILES
	map_block Field_Map047, .LField_Map047_SHAPES
	map_block Field_Map047, .LField_Map047_GRID
	map_block Field_Map047, .LField_Map047_SHAPE_GRID
	map_block Field_Map047, .LField_Map047_ANIMATION
	map_block Field_Map047, .LField_Map047_BLEND
	map_block Field_Map047, .LField_Map047_SCENE
.LField_Map047_METATILES:
	.incbin "MAP/MAP_047/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map047_SHAPES:
	.incbin "MAP/MAP_047/SHAPES.table.lz"
	.balign 4, 0
.LField_Map047_GRID:
	.incbin "MAP/MAP_047/GRID.parts.lz"
	.balign 4, 0
.LField_Map047_SHAPE_GRID:
	.incbin "MAP/MAP_047/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map047_ANIMATION:
	.incbin "MAP/MAP_047/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map047_BLEND:
	.incbin "MAP/MAP_047/BLEND.script.lz"
	.balign 4, 0
.LField_Map047_SCENE:
	.incbin "MAP/MAP_047/SCENE.parts.lz"
