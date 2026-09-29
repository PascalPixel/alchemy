@ Field_Map034: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map034
Field_Map034:
	map_header 0, 0, 128, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map034, .LField_Map034_METATILES
	map_block Field_Map034, .LField_Map034_SHAPES
	map_block Field_Map034, .LField_Map034_GRID
	map_block Field_Map034, .LField_Map034_SHAPE_GRID
	map_block Field_Map034, .LField_Map034_ANIMATION
	map_block Field_Map034, .LField_Map034_BLEND
	map_block Field_Map034, .LField_Map034_SCENE
.LField_Map034_METATILES:
	.incbin "MAP/MAP_034/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map034_SHAPES:
	.incbin "MAP/MAP_034/SHAPES.table.lz"
	.balign 4, 0
.LField_Map034_GRID:
	.incbin "MAP/MAP_034/GRID.parts.lz"
	.balign 4, 0
.LField_Map034_SHAPE_GRID:
	.incbin "MAP/MAP_034/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map034_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map034_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map034_SCENE:
	.incbin "MAP/MAP_034/SCENE.parts.lz"
