@ Field_Map169: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map169
Field_Map169:
	map_header 0, 0, 130, 112, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map169, .LField_Map169_METATILES
	map_block Field_Map169, .LField_Map169_SHAPES
	map_block Field_Map169, .LField_Map169_GRID
	map_block Field_Map169, .LField_Map169_SHAPE_GRID
	map_block Field_Map169, .LField_Map169_ANIMATION
	map_block Field_Map169, .LField_Map169_BLEND
	map_block Field_Map169, .LField_Map169_SCENE
.LField_Map169_METATILES:
	.incbin "MAP/MAP_169/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map169_SHAPES:
	.incbin "MAP/MAP_169/SHAPES.table.lz"
	.balign 4, 0
.LField_Map169_GRID:
	.incbin "MAP/MAP_169/GRID.parts.lz"
	.balign 4, 0
.LField_Map169_SHAPE_GRID:
	.incbin "MAP/MAP_169/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map169_ANIMATION:
	.incbin "MAP/MAP_169/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map169_BLEND:
	.incbin "MAP/MAP_169/BLEND.script.lz"
	.balign 4, 0
.LField_Map169_SCENE:
	.incbin "MAP/MAP_169/SCENE.parts.lz"
