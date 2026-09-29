@ Field_Map233: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map233
Field_Map233:
	map_header 0, 0, 84, 240, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 84, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 168, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map233, .LField_Map233_METATILES
	map_block Field_Map233, .LField_Map233_SHAPES
	map_block Field_Map233, .LField_Map233_GRID
	map_block Field_Map233, .LField_Map233_SHAPE_GRID
	map_block Field_Map233, .LField_Map233_ANIMATION
	map_block Field_Map233, .LField_Map233_BLEND
	map_block Field_Map233, .LField_Map233_SCENE
.LField_Map233_METATILES:
	.incbin "MAP/MAP_233/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map233_SHAPES:
	.incbin "MAP/MAP_233/SHAPES.table.lz"
	.balign 4, 0
.LField_Map233_GRID:
	.incbin "MAP/MAP_233/GRID.parts.lz"
	.balign 4, 0
.LField_Map233_SHAPE_GRID:
	.incbin "MAP/MAP_233/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map233_ANIMATION:
	.incbin "MAP/MAP_233/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map233_BLEND:
	.incbin "MAP/MAP_233/BLEND.script.lz"
	.balign 4, 0
.LField_Map233_SCENE:
	.incbin "MAP/MAP_233/SCENE.parts.lz"
