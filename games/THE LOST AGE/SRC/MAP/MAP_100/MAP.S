@ Field_Map100: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map100
Field_Map100:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map100, .LField_Map100_METATILES
	map_block Field_Map100, .LField_Map100_SHAPES
	map_block Field_Map100, .LField_Map100_GRID
	map_block Field_Map100, .LField_Map100_SHAPE_GRID
	map_block Field_Map100, .LField_Map100_ANIMATION
	map_block Field_Map100, .LField_Map100_BLEND
	map_block Field_Map100, .LField_Map100_SCENE
.LField_Map100_METATILES:
	.incbin "MAP/MAP_100/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map100_SHAPES:
	.incbin "MAP/MAP_100/SHAPES.table.lz"
	.balign 4, 0
.LField_Map100_GRID:
	.incbin "MAP/MAP_100/GRID.parts.lz"
	.balign 4, 0
.LField_Map100_SHAPE_GRID:
	.incbin "MAP/MAP_100/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map100_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map100_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map100_SCENE:
	.incbin "MAP/MAP_100/SCENE.parts.lz"
