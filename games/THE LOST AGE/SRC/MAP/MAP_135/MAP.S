@ Field_Map135: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map135
Field_Map135:
	map_header 0, 0, 90, 100, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map135, .LField_Map135_METATILES
	map_block Field_Map135, .LField_Map135_SHAPES
	map_block Field_Map135, .LField_Map135_GRID
	map_block Field_Map135, .LField_Map135_SHAPE_GRID
	map_block Field_Map135, .LField_Map135_ANIMATION
	map_block Field_Map135, .LField_Map135_BLEND
	map_block Field_Map135, .LField_Map135_SCENE
.LField_Map135_METATILES:
	.incbin "MAP/MAP_135/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map135_SHAPES:
	.incbin "MAP/MAP_135/SHAPES.table.lz"
	.balign 4, 0
.LField_Map135_GRID:
	.incbin "MAP/MAP_135/GRID.parts.lz"
	.balign 4, 0
.LField_Map135_SHAPE_GRID:
	.incbin "MAP/MAP_135/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map135_ANIMATION:
	.incbin "MAP/MAP_135/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map135_BLEND:
	.incbin "MAP/MAP_135/BLEND.script.lz"
	.balign 4, 0
.LField_Map135_SCENE:
	.incbin "MAP/MAP_135/SCENE.parts.lz"
