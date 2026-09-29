@ Field_Map185: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map185
Field_Map185:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map185, .LField_Map185_METATILES
	map_block Field_Map185, .LField_Map185_SHAPES
	map_block Field_Map185, .LField_Map185_GRID
	map_block Field_Map185, .LField_Map185_SHAPE_GRID
	map_block Field_Map185, .LField_Map185_ANIMATION
	map_block Field_Map185, .LField_Map185_BLEND
	map_block Field_Map185, .LField_Map185_SCENE
.LField_Map185_METATILES:
	.incbin "MAP/MAP_185/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map185_SHAPES:
	.incbin "MAP/MAP_185/SHAPES.table.lz"
	.balign 4, 0
.LField_Map185_GRID:
	.incbin "MAP/MAP_185/GRID.parts.lz"
	.balign 4, 0
.LField_Map185_SHAPE_GRID:
	.incbin "MAP/MAP_185/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map185_ANIMATION:
	.incbin "MAP/MAP_185/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map185_BLEND:
	.incbin "MAP/MAP_185/BLEND.script.lz"
	.balign 4, 0
.LField_Map185_SCENE:
	.incbin "MAP/MAP_185/SCENE.parts.lz"
