@ Field_Map064: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map064
Field_Map064:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map064, .LField_Map064_METATILES
	map_block Field_Map064, .LField_Map064_SHAPES
	map_block Field_Map064, .LField_Map064_GRID
	map_block Field_Map064, .LField_Map064_SHAPE_GRID
	map_block Field_Map064, .LField_Map064_ANIMATION
	map_block Field_Map064, .LField_Map064_BLEND
	map_block Field_Map064, .LField_Map064_SCENE
.LField_Map064_METATILES:
	.incbin "MAP/MAP_064/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map064_SHAPES:
	.incbin "MAP/MAP_064/SHAPES.table.lz"
	.balign 4, 0
.LField_Map064_GRID:
	.incbin "MAP/MAP_064/GRID.parts.lz"
	.balign 4, 0
.LField_Map064_SHAPE_GRID:
	.incbin "MAP/MAP_064/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map064_ANIMATION:
	.incbin "MAP/MAP_062/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map064_BLEND:
	.incbin "MAP/MAP_060/BLEND.script.lz"
	.balign 4, 0
.LField_Map064_SCENE:
	.incbin "MAP/MAP_064/SCENE.parts.lz"
