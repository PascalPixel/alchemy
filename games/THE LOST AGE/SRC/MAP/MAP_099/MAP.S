@ Field_Map099: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map099
Field_Map099:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map099, .LField_Map099_METATILES
	map_block Field_Map099, .LField_Map099_SHAPES
	map_block Field_Map099, .LField_Map099_GRID
	map_block Field_Map099, .LField_Map099_SHAPE_GRID
	map_block Field_Map099, .LField_Map099_ANIMATION
	map_block Field_Map099, .LField_Map099_BLEND
	map_block Field_Map099, .LField_Map099_SCENE
.LField_Map099_METATILES:
	.incbin "MAP/MAP_099/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map099_SHAPES:
	.incbin "MAP/MAP_099/SHAPES.table.lz"
	.balign 4, 0
.LField_Map099_GRID:
	.incbin "MAP/MAP_099/GRID.parts.lz"
	.balign 4, 0
.LField_Map099_SHAPE_GRID:
	.incbin "MAP/MAP_099/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map099_ANIMATION:
	.incbin "MAP/MAP_099/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map099_BLEND:
	.incbin "MAP/MAP_099/BLEND.script.lz"
	.balign 4, 0
.LField_Map099_SCENE:
	.incbin "MAP/MAP_099/SCENE.parts.lz"
