@ Field_Map208: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map208
Field_Map208:
	map_header 0, 0, 128, 128, 2, 2, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map208, .LField_Map208_METATILES
	map_block Field_Map208, .LField_Map208_SHAPES
	map_block Field_Map208, .LField_Map208_GRID
	map_block Field_Map208, .LField_Map208_SHAPE_GRID
	map_block Field_Map208, .LField_Map208_ANIMATION
	map_block Field_Map208, .LField_Map208_BLEND
	map_block Field_Map208, .LField_Map208_SCENE
.LField_Map208_METATILES:
	.incbin "MAP/MAP_208/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map208_SHAPES:
	.incbin "MAP/MAP_208/SHAPES.table.lz"
	.balign 4, 0
.LField_Map208_GRID:
	.incbin "MAP/MAP_208/GRID.parts.lz"
	.balign 4, 0
.LField_Map208_SHAPE_GRID:
	.incbin "MAP/MAP_208/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map208_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map208_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map208_SCENE:
	.incbin "MAP/MAP_208/SCENE.parts.lz"
