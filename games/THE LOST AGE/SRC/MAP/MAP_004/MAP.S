@ Field_Map004: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map004
Field_Map004:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map004, .LField_Map004_METATILES
	map_block Field_Map004, .LField_Map004_SHAPES
	map_block Field_Map004, .LField_Map004_GRID
	map_block Field_Map004, .LField_Map004_SHAPE_GRID
	map_block Field_Map004, .LField_Map004_ANIMATION
	map_block Field_Map004, .LField_Map004_BLEND
	map_block Field_Map004, .LField_Map004_SCENE
.LField_Map004_METATILES:
	.incbin "COMMON/MAP/MAP_152/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map004_SHAPES:
	.incbin "COMMON/MAP/MAP_152/SHAPES.table.lz"
	.balign 4, 0
.LField_Map004_GRID:
	.incbin "MAP/MAP_004/GRID.parts.lz"
	.balign 4, 0
.LField_Map004_SHAPE_GRID:
	.incbin "MAP/MAP_004/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map004_ANIMATION:
	.incbin "COMMON/MAP/MAP_031/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map004_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map004_SCENE:
	.incbin "MAP/MAP_000/SCENE.parts.lz"
