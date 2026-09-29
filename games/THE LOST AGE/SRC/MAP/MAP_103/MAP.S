@ Field_Map103: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map103
Field_Map103:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map103, .LField_Map103_METATILES
	map_block Field_Map103, .LField_Map103_SHAPES
	map_block Field_Map103, .LField_Map103_GRID
	map_block Field_Map103, .LField_Map103_SHAPE_GRID
	map_block Field_Map103, .LField_Map103_ANIMATION
	map_block Field_Map103, .LField_Map103_BLEND
	map_block Field_Map103, .LField_Map103_SCENE
.LField_Map103_METATILES:
	.incbin "MAP/MAP_103/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map103_SHAPES:
	.incbin "MAP/MAP_103/SHAPES.table.lz"
	.balign 4, 0
.LField_Map103_GRID:
	.incbin "MAP/MAP_103/GRID.parts.lz"
	.balign 4, 0
.LField_Map103_SHAPE_GRID:
	.incbin "MAP/MAP_103/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map103_ANIMATION:
	.incbin "MAP/MAP_103/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map103_BLEND:
	.incbin "MAP/MAP_103/BLEND.script.lz"
	.balign 4, 0
.LField_Map103_SCENE:
	.incbin "MAP/MAP_103/SCENE.parts.lz"
