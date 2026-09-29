@ Field_Map227: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map227
Field_Map227:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map227, .LField_Map227_METATILES
	map_block Field_Map227, .LField_Map227_SHAPES
	map_block Field_Map227, .LField_Map227_GRID
	map_block Field_Map227, .LField_Map227_SHAPE_GRID
	map_block Field_Map227, .LField_Map227_ANIMATION
	map_block Field_Map227, .LField_Map227_BLEND
	map_block Field_Map227, .LField_Map227_SCENE
.LField_Map227_METATILES:
	.incbin "MAP/MAP_227/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map227_SHAPES:
	.incbin "MAP/MAP_227/SHAPES.table.lz"
	.balign 4, 0
.LField_Map227_GRID:
	.incbin "MAP/MAP_227/GRID.parts.lz"
	.balign 4, 0
.LField_Map227_SHAPE_GRID:
	.incbin "MAP/MAP_227/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map227_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map227_BLEND:
	.incbin "MAP/MAP_227/BLEND.script.lz"
	.balign 4, 0
.LField_Map227_SCENE:
	.incbin "MAP/MAP_227/SCENE.parts.lz"
