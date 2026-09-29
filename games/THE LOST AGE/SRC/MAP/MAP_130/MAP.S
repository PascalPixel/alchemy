@ Field_Map130: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map130
Field_Map130:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map130, .LField_Map130_METATILES
	map_block Field_Map130, .LField_Map130_SHAPES
	map_block Field_Map130, .LField_Map130_GRID
	map_block Field_Map130, .LField_Map130_SHAPE_GRID
	map_block Field_Map130, .LField_Map130_ANIMATION
	map_block Field_Map130
	map_block Field_Map130, .LField_Map130_SCENE
.LField_Map130_METATILES:
	.incbin "MAP/MAP_130/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map130_SHAPES:
	.incbin "MAP/MAP_130/SHAPES.table.lz"
	.balign 4, 0
.LField_Map130_GRID:
	.incbin "MAP/MAP_130/GRID.parts.lz"
	.balign 4, 0
.LField_Map130_SHAPE_GRID:
	.incbin "MAP/MAP_130/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map130_ANIMATION:
	.incbin "MAP/MAP_130/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map130_SCENE:
	.incbin "MAP/MAP_130/SCENE.parts.lz"
