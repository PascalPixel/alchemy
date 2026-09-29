@ Field_Map281: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map281
Field_Map281:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map281, .LField_Map281_METATILES
	map_block Field_Map281, .LField_Map281_SHAPES
	map_block Field_Map281, .LField_Map281_GRID
	map_block Field_Map281, .LField_Map281_SHAPE_GRID
	map_block Field_Map281, .LField_Map281_ANIMATION
	map_block Field_Map281, .LField_Map281_BLEND
	map_block Field_Map281, .LField_Map281_SCENE
.LField_Map281_METATILES:
	.incbin "MAP/MAP_281/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map281_SHAPES:
	.incbin "MAP/MAP_281/SHAPES.table.lz"
	.balign 4, 0
.LField_Map281_GRID:
	.incbin "MAP/MAP_281/GRID.parts.lz"
	.balign 4, 0
.LField_Map281_SHAPE_GRID:
	.incbin "MAP/MAP_281/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map281_ANIMATION:
	.incbin "MAP/MAP_281/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map281_BLEND:
	.incbin "MAP/MAP_281/BLEND.script.lz"
	.balign 4, 0
.LField_Map281_SCENE:
	.incbin "MAP/MAP_281/SCENE.parts.lz"
