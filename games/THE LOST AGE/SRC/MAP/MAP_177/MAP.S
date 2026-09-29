@ Field_Map177: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map177
Field_Map177:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map177, .LField_Map177_METATILES
	map_block Field_Map177, .LField_Map177_SHAPES
	map_block Field_Map177, .LField_Map177_GRID
	map_block Field_Map177, .LField_Map177_SHAPE_GRID
	map_block Field_Map177, .LField_Map177_ANIMATION
	map_block Field_Map177, .LField_Map177_BLEND
	map_block Field_Map177, .LField_Map177_SCENE
.LField_Map177_METATILES:
	.incbin "MAP/MAP_177/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map177_SHAPES:
	.incbin "MAP/MAP_177/SHAPES.table.lz"
	.balign 4, 0
.LField_Map177_GRID:
	.incbin "MAP/MAP_177/GRID.parts.lz"
	.balign 4, 0
.LField_Map177_SHAPE_GRID:
	.incbin "MAP/MAP_177/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map177_ANIMATION:
	.incbin "MAP/MAP_177/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map177_BLEND:
	.incbin "MAP/MAP_057/BLEND.script.lz"
	.balign 4, 0
.LField_Map177_SCENE:
	.incbin "MAP/MAP_177/SCENE.parts.lz"
