@ Field_Map051: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map051
Field_Map051:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map051, .LField_Map051_METATILES
	map_block Field_Map051, .LField_Map051_SHAPES
	map_block Field_Map051, .LField_Map051_GRID
	map_block Field_Map051, .LField_Map051_SHAPE_GRID
	map_block Field_Map051, .LField_Map051_ANIMATION
	map_block Field_Map051, .LField_Map051_BLEND
	map_block Field_Map051, .LField_Map051_SCENE
.LField_Map051_METATILES:
	.incbin "MAP/MAP_051/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "MAP/MAP_051/SHAPES.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "MAP/MAP_051/GRID.parts.lz"
	.balign 4, 0
.LField_Map051_SHAPE_GRID:
	.incbin "MAP/MAP_051/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "MAP/MAP_051/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "MAP/MAP_051/BLEND.script.lz"
	.balign 4, 0
.LField_Map051_SCENE:
	.incbin "MAP/MAP_051/SCENE.parts.lz"
