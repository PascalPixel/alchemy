@ Field_Map182: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map182
Field_Map182:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map182, .LField_Map182_METATILES
	map_block Field_Map182, .LField_Map182_SHAPES
	map_block Field_Map182, .LField_Map182_GRID
	map_block Field_Map182, .LField_Map182_SHAPE_GRID
	map_block Field_Map182, .LField_Map182_ANIMATION
	map_block Field_Map182, .LField_Map182_BLEND
	map_block Field_Map182, .LField_Map182_SCENE
.LField_Map182_METATILES:
	.incbin "MAP/MAP_182/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map182_SHAPES:
	.incbin "MAP/MAP_182/SHAPES.table.lz"
	.balign 4, 0
.LField_Map182_GRID:
	.incbin "MAP/MAP_182/GRID.parts.lz"
	.balign 4, 0
.LField_Map182_SHAPE_GRID:
	.incbin "MAP/MAP_182/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map182_ANIMATION:
	.incbin "MAP/MAP_182/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map182_BLEND:
	.incbin "MAP/MAP_182/BLEND.script.lz"
	.balign 4, 0
.LField_Map182_SCENE:
	.incbin "MAP/MAP_182/SCENE.parts.lz"
