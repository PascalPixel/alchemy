@ Field_Map212: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map212
Field_Map212:
	map_header 0, 0, 130, 112, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map212, .LField_Map212_METATILES
	map_block Field_Map212, .LField_Map212_SHAPES
	map_block Field_Map212, .LField_Map212_GRID
	map_block Field_Map212, .LField_Map212_SHAPE_GRID
	map_block Field_Map212, .LField_Map212_ANIMATION
	map_block Field_Map212, .LField_Map212_BLEND
	map_block Field_Map212, .LField_Map212_SCENE
.LField_Map212_METATILES:
	.incbin "MAP/MAP_212/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map212_SHAPES:
	.incbin "MAP/MAP_212/SHAPES.table.lz"
	.balign 4, 0
.LField_Map212_GRID:
	.incbin "MAP/MAP_212/GRID.parts.lz"
	.balign 4, 0
.LField_Map212_SHAPE_GRID:
	.incbin "MAP/MAP_212/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map212_ANIMATION:
	.incbin "MAP/MAP_212/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map212_BLEND:
	.incbin "MAP/MAP_212/BLEND.script.lz"
	.balign 4, 0
.LField_Map212_SCENE:
	.incbin "MAP/MAP_212/SCENE.parts.lz"
