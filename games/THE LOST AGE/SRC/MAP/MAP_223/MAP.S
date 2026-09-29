@ Field_Map223: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map223
Field_Map223:
	map_header 0, 0, 64, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map223, .LField_Map223_METATILES
	map_block Field_Map223, .LField_Map223_SHAPES
	map_block Field_Map223, .LField_Map223_GRID
	map_block Field_Map223, .LField_Map223_SHAPE_GRID
	map_block Field_Map223, .LField_Map223_ANIMATION
	map_block Field_Map223, .LField_Map223_BLEND
	map_block Field_Map223, .LField_Map223_SCENE
.LField_Map223_METATILES:
	.incbin "MAP/MAP_223/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map223_SHAPES:
	.incbin "MAP/MAP_223/SHAPES.table.lz"
	.balign 4, 0
.LField_Map223_GRID:
	.incbin "MAP/MAP_223/GRID.parts.lz"
	.balign 4, 0
.LField_Map223_SHAPE_GRID:
	.incbin "MAP/MAP_223/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map223_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map223_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map223_SCENE:
	.incbin "MAP/MAP_223/SCENE.parts.lz"
