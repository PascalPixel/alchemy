@ Field_Map119: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map119
Field_Map119:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map119, .LField_Map119_METATILES
	map_block Field_Map119, .LField_Map119_SHAPES
	map_block Field_Map119, .LField_Map119_GRID
	map_block Field_Map119, .LField_Map119_SHAPE_GRID
	map_block Field_Map119, .LField_Map119_ANIMATION
	map_block Field_Map119, .LField_Map119_BLEND
	map_block Field_Map119, .LField_Map119_SCENE
.LField_Map119_METATILES:
	.incbin "MAP/MAP_119/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map119_SHAPES:
	.incbin "MAP/MAP_119/SHAPES.table.lz"
	.balign 4, 0
.LField_Map119_GRID:
	.incbin "MAP/MAP_119/GRID.parts.lz"
	.balign 4, 0
.LField_Map119_SHAPE_GRID:
	.incbin "MAP/MAP_119/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map119_ANIMATION:
	.incbin "MAP/MAP_119/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map119_BLEND:
	.incbin "MAP/MAP_119/BLEND.script.lz"
	.balign 4, 0
.LField_Map119_SCENE:
	.incbin "MAP/MAP_119/SCENE.parts.lz"
