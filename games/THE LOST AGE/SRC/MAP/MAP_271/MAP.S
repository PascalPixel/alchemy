@ Field_Map271: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map271
Field_Map271:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map271, .LField_Map271_METATILES
	map_block Field_Map271, .LField_Map271_SHAPES
	map_block Field_Map271, .LField_Map271_GRID
	map_block Field_Map271, .LField_Map271_SHAPE_GRID
	map_block Field_Map271, .LField_Map271_ANIMATION
	map_block Field_Map271, .LField_Map271_BLEND
	map_block Field_Map271, .LField_Map271_SCENE
.LField_Map271_METATILES:
	.incbin "MAP/MAP_271/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map271_SHAPES:
	.incbin "MAP/MAP_271/SHAPES.table.lz"
	.balign 4, 0
.LField_Map271_GRID:
	.incbin "MAP/MAP_271/GRID.parts.lz"
	.balign 4, 0
.LField_Map271_SHAPE_GRID:
	.incbin "MAP/MAP_271/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map271_ANIMATION:
	.incbin "MAP/MAP_271/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map271_BLEND:
	.incbin "MAP/MAP_271/BLEND.script.lz"
	.balign 4, 0
.LField_Map271_SCENE:
	.incbin "MAP/MAP_271/SCENE.parts.lz"
