@ Field_Map167: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map167
Field_Map167:
	map_header 0, 0, 130, 112, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map167, .LField_Map167_METATILES
	map_block Field_Map167, .LField_Map167_SHAPES
	map_block Field_Map167, .LField_Map167_GRID
	map_block Field_Map167, .LField_Map167_SHAPE_GRID
	map_block Field_Map167, .LField_Map167_ANIMATION
	map_block Field_Map167, .LField_Map167_BLEND
	map_block Field_Map167, .LField_Map167_SCENE
.LField_Map167_METATILES:
	.incbin "MAP/MAP_167/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map167_SHAPES:
	.incbin "MAP/MAP_167/SHAPES.table.lz"
	.balign 4, 0
.LField_Map167_GRID:
	.incbin "MAP/MAP_167/GRID.parts.lz"
	.balign 4, 0
.LField_Map167_SHAPE_GRID:
	.incbin "MAP/MAP_167/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map167_ANIMATION:
	.incbin "MAP/MAP_167/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map167_BLEND:
	.incbin "MAP/MAP_167/BLEND.script.lz"
	.balign 4, 0
.LField_Map167_SCENE:
	.incbin "MAP/MAP_167/SCENE.parts.lz"
