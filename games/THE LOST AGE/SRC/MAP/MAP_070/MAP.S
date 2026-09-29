@ Field_Map070: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map070
Field_Map070:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map070, .LField_Map070_METATILES
	map_block Field_Map070, .LField_Map070_SHAPES
	map_block Field_Map070, .LField_Map070_GRID
	map_block Field_Map070, .LField_Map070_SHAPE_GRID
	map_block Field_Map070, .LField_Map070_ANIMATION
	map_block Field_Map070, .LField_Map070_BLEND
	map_block Field_Map070, .LField_Map070_SCENE
.LField_Map070_METATILES:
	.incbin "MAP/MAP_070/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map070_SHAPES:
	.incbin "MAP/MAP_070/SHAPES.table.lz"
	.balign 4, 0
.LField_Map070_GRID:
	.incbin "MAP/MAP_070/GRID.parts.lz"
	.balign 4, 0
.LField_Map070_SHAPE_GRID:
	.incbin "MAP/MAP_070/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map070_ANIMATION:
	.incbin "MAP/MAP_070/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map070_BLEND:
	.incbin "MAP/MAP_070/BLEND.script.lz"
	.balign 4, 0
.LField_Map070_SCENE:
	.incbin "MAP/MAP_070/SCENE.parts.lz"
