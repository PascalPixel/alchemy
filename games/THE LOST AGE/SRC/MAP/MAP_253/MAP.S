@ Field_Map253: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map253
Field_Map253:
	map_header 0, 0, 92, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map253, .LField_Map253_METATILES
	map_block Field_Map253, .LField_Map253_SHAPES
	map_block Field_Map253, .LField_Map253_GRID
	map_block Field_Map253, .LField_Map253_SHAPE_GRID
	map_block Field_Map253, .LField_Map253_ANIMATION
	map_block Field_Map253, .LField_Map253_BLEND
	map_block Field_Map253, .LField_Map253_SCENE
.LField_Map253_METATILES:
	.incbin "MAP/MAP_253/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map253_SHAPES:
	.incbin "MAP/MAP_253/SHAPES.table.lz"
	.balign 4, 0
.LField_Map253_GRID:
	.incbin "MAP/MAP_253/GRID.parts.lz"
	.balign 4, 0
.LField_Map253_SHAPE_GRID:
	.incbin "MAP/MAP_253/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map253_ANIMATION:
	.incbin "MAP/MAP_253/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map253_BLEND:
	.incbin "MAP/MAP_253/BLEND.script.lz"
	.balign 4, 0
.LField_Map253_SCENE:
	.incbin "MAP/MAP_253/SCENE.parts.lz"
