@ Field_Map071: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map071
Field_Map071:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map071, .LField_Map071_METATILES
	map_block Field_Map071, .LField_Map071_SHAPES
	map_block Field_Map071, .LField_Map071_GRID
	map_block Field_Map071, .LField_Map071_SHAPE_GRID
	map_block Field_Map071, .LField_Map071_ANIMATION
	map_block Field_Map071, .LField_Map071_BLEND
	map_block Field_Map071, .LField_Map071_SCENE
.LField_Map071_METATILES:
	.incbin "MAP/MAP_071/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map071_SHAPES:
	.incbin "MAP/MAP_071/SHAPES.table.lz"
	.balign 4, 0
.LField_Map071_GRID:
	.incbin "MAP/MAP_071/GRID.parts.lz"
	.balign 4, 0
.LField_Map071_SHAPE_GRID:
	.incbin "MAP/MAP_071/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map071_ANIMATION:
	.incbin "MAP/MAP_071/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map071_BLEND:
	.incbin "MAP/MAP_071/BLEND.script.lz"
	.balign 4, 0
.LField_Map071_SCENE:
	.incbin "MAP/MAP_071/SCENE.parts.lz"
