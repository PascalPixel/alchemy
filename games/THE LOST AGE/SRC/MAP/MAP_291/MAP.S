@ Field_Map291: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map291
Field_Map291:
	map_header 0, 0, 100, 64, 3, 1, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map291, .LField_Map291_METATILES
	map_block Field_Map291, .LField_Map291_SHAPES
	map_block Field_Map291, .LField_Map291_GRID
	map_block Field_Map291, .LField_Map291_SHAPE_GRID
	map_block Field_Map291, .LField_Map291_ANIMATION
	map_block Field_Map291, .LField_Map291_BLEND
	map_block Field_Map291, .LField_Map291_SCENE
.LField_Map291_METATILES:
	.incbin "MAP/MAP_291/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map291_SHAPES:
	.incbin "MAP/MAP_291/SHAPES.table.lz"
	.balign 4, 0
.LField_Map291_GRID:
	.incbin "MAP/MAP_291/GRID.parts.lz"
	.balign 4, 0
.LField_Map291_SHAPE_GRID:
	.incbin "MAP/MAP_291/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map291_ANIMATION:
	.incbin "MAP/MAP_291/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map291_BLEND:
	.incbin "MAP/MAP_291/BLEND.script.lz"
	.balign 4, 0
.LField_Map291_SCENE:
	.incbin "MAP/MAP_291/SCENE.parts.lz"
