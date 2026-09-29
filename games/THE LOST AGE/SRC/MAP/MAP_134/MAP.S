@ Field_Map134: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map134
Field_Map134:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map134, .LField_Map134_METATILES
	map_block Field_Map134, .LField_Map134_SHAPES
	map_block Field_Map134, .LField_Map134_GRID
	map_block Field_Map134, .LField_Map134_SHAPE_GRID
	map_block Field_Map134, .LField_Map134_ANIMATION
	map_block Field_Map134, .LField_Map134_BLEND
	map_block Field_Map134, .LField_Map134_SCENE
.LField_Map134_METATILES:
	.incbin "MAP/MAP_134/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "MAP/MAP_134/SHAPES.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "MAP/MAP_134/GRID.parts.lz"
	.balign 4, 0
.LField_Map134_SHAPE_GRID:
	.incbin "MAP/MAP_134/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "MAP/MAP_134/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map134_SCENE:
	.incbin "MAP/MAP_134/SCENE.parts.lz"
