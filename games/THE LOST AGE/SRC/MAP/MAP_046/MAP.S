@ Field_Map046: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map046
Field_Map046:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map046, .LField_Map046_METATILES
	map_block Field_Map046, .LField_Map046_SHAPES
	map_block Field_Map046, .LField_Map046_GRID
	map_block Field_Map046, .LField_Map046_SHAPE_GRID
	map_block Field_Map046, .LField_Map046_ANIMATION
	map_block Field_Map046, .LField_Map046_BLEND
	map_block Field_Map046, .LField_Map046_SCENE
.LField_Map046_METATILES:
	.incbin "MAP/MAP_046/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map046_SHAPES:
	.incbin "MAP/MAP_046/SHAPES.table.lz"
	.balign 4, 0
.LField_Map046_GRID:
	.incbin "MAP/MAP_046/GRID.parts.lz"
	.balign 4, 0
.LField_Map046_SHAPE_GRID:
	.incbin "MAP/MAP_046/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map046_ANIMATION:
	.incbin "MAP/MAP_046/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map046_BLEND:
	.incbin "MAP/MAP_046/BLEND.script.lz"
	.balign 4, 0
.LField_Map046_SCENE:
	.incbin "MAP/MAP_046/SCENE.parts.lz"
