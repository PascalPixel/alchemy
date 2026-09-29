@ Field_Map194: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map194
Field_Map194:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map194, .LField_Map194_METATILES
	map_block Field_Map194, .LField_Map194_SHAPES
	map_block Field_Map194, .LField_Map194_GRID
	map_block Field_Map194, .LField_Map194_SHAPE_GRID
	map_block Field_Map194, .LField_Map194_ANIMATION
	map_block Field_Map194, .LField_Map194_BLEND
	map_block Field_Map194, .LField_Map194_SCENE
.LField_Map194_METATILES:
	.incbin "MAP/MAP_194/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map194_SHAPES:
	.incbin "MAP/MAP_194/SHAPES.table.lz"
	.balign 4, 0
.LField_Map194_GRID:
	.incbin "MAP/MAP_194/GRID.parts.lz"
	.balign 4, 0
.LField_Map194_SHAPE_GRID:
	.incbin "MAP/MAP_194/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map194_ANIMATION:
	.incbin "MAP/MAP_194/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map194_BLEND:
	.incbin "MAP/MAP_194/BLEND.script.lz"
	.balign 4, 0
.LField_Map194_SCENE:
	.incbin "MAP/MAP_194/SCENE.parts.lz"
