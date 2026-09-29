@ Field_Map246: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map246
Field_Map246:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map246, .LField_Map246_METATILES
	map_block Field_Map246, .LField_Map246_SHAPES
	map_block Field_Map246, .LField_Map246_GRID
	map_block Field_Map246, .LField_Map246_SHAPE_GRID
	map_block Field_Map246, .LField_Map246_ANIMATION
	map_block Field_Map246
	map_block Field_Map246, .LField_Map246_SCENE
.LField_Map246_METATILES:
	.incbin "MAP/MAP_246/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map246_SHAPES:
	.incbin "MAP/MAP_246/SHAPES.table.lz"
	.balign 4, 0
.LField_Map246_GRID:
	.incbin "MAP/MAP_246/GRID.parts.lz"
	.balign 4, 0
.LField_Map246_SHAPE_GRID:
	.incbin "MAP/MAP_246/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map246_ANIMATION:
	.incbin "MAP/MAP_246/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map246_SCENE:
	.incbin "MAP/MAP_246/SCENE.parts.lz"
