@ Field_Map001: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map001
Field_Map001:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map001, .LField_Map001_METATILES
	map_block Field_Map001, .LField_Map001_SHAPES
	map_block Field_Map001, .LField_Map001_GRID
	map_block Field_Map001, .LField_Map001_SHAPE_GRID
	map_block Field_Map001, .LField_Map001_ANIMATION
	map_block Field_Map001, .LField_Map001_BLEND
	map_block Field_Map001, .LField_Map001_SCENE
.LField_Map001_METATILES:
	.incbin "MAP/MAP_001/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map001_SHAPES:
	.incbin "MAP/MAP_001/SHAPES.table.lz"
	.balign 4, 0
.LField_Map001_GRID:
	.incbin "MAP/MAP_001/GRID.parts.lz"
	.balign 4, 0
.LField_Map001_SHAPE_GRID:
	.incbin "MAP/MAP_001/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map001_ANIMATION:
	.incbin "MAP/MAP_001/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map001_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map001_SCENE:
	.incbin "MAP/MAP_001/SCENE.parts.lz"
