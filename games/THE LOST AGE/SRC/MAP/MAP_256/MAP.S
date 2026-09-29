@ Field_Map256: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map256
Field_Map256:
	map_header 0, 0, 90, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 66, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map256, .LField_Map256_METATILES
	map_block Field_Map256, .LField_Map256_SHAPES
	map_block Field_Map256, .LField_Map256_GRID
	map_block Field_Map256, .LField_Map256_SHAPE_GRID
	map_block Field_Map256, .LField_Map256_ANIMATION
	map_block Field_Map256, .LField_Map256_BLEND
	map_block Field_Map256, .LField_Map256_SCENE
.LField_Map256_METATILES:
	.incbin "MAP/MAP_256/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map256_SHAPES:
	.incbin "MAP/MAP_256/SHAPES.table.lz"
	.balign 4, 0
.LField_Map256_GRID:
	.incbin "MAP/MAP_256/GRID.parts.lz"
	.balign 4, 0
.LField_Map256_SHAPE_GRID:
	.incbin "MAP/MAP_256/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map256_ANIMATION:
	.incbin "MAP/MAP_256/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map256_BLEND:
	.incbin "MAP/MAP_256/BLEND.script.lz"
	.balign 4, 0
.LField_Map256_SCENE:
	.incbin "MAP/MAP_256/SCENE.parts.lz"
