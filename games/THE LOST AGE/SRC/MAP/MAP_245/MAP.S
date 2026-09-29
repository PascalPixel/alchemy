@ Field_Map245: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map245
Field_Map245:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map245, .LField_Map245_METATILES
	map_block Field_Map245, .LField_Map245_SHAPES
	map_block Field_Map245, .LField_Map245_GRID
	map_block Field_Map245, .LField_Map245_SHAPE_GRID
	map_block Field_Map245, .LField_Map245_ANIMATION
	map_block Field_Map245
	map_block Field_Map245, .LField_Map245_SCENE
.LField_Map245_METATILES:
	.incbin "MAP/MAP_245/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map245_SHAPES:
	.incbin "MAP/MAP_245/SHAPES.table.lz"
	.balign 4, 0
.LField_Map245_GRID:
	.incbin "MAP/MAP_245/GRID.parts.lz"
	.balign 4, 0
.LField_Map245_SHAPE_GRID:
	.incbin "MAP/MAP_245/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map245_ANIMATION:
	.incbin "MAP/MAP_245/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map245_SCENE:
	.incbin "MAP/MAP_245/SCENE.parts.lz"
