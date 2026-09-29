@ Field_Map193: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map193
Field_Map193:
	map_header 0, 0, 30, 22, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 132, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map193, .LField_Map193_METATILES
	map_block Field_Map193, .LField_Map193_SHAPES
	map_block Field_Map193, .LField_Map193_GRID
	map_block Field_Map193, .LField_Map193_SHAPE_GRID
	map_block Field_Map193, .LField_Map193_ANIMATION
	map_block Field_Map193, .LField_Map193_BLEND
	map_block Field_Map193, .LField_Map193_SCENE
.LField_Map193_METATILES:
	.incbin "MAP/MAP_193/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map193_SHAPES:
	.incbin "MAP/MAP_193/SHAPES.table.lz"
	.balign 4, 0
.LField_Map193_GRID:
	.incbin "MAP/MAP_193/GRID.parts.lz"
	.balign 4, 0
.LField_Map193_SHAPE_GRID:
	.incbin "MAP/MAP_193/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map193_ANIMATION:
	.incbin "MAP/MAP_193/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map193_BLEND:
	.incbin "MAP/MAP_193/BLEND.script.lz"
	.balign 4, 0
.LField_Map193_SCENE:
	.incbin "MAP/MAP_193/SCENE.parts.lz"
