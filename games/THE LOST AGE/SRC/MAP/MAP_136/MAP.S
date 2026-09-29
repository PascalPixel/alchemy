@ Field_Map136: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map136
Field_Map136:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map136, .LField_Map136_METATILES
	map_block Field_Map136, .LField_Map136_SHAPES
	map_block Field_Map136, .LField_Map136_GRID
	map_block Field_Map136, .LField_Map136_SHAPE_GRID
	map_block Field_Map136, .LField_Map136_ANIMATION
	map_block Field_Map136, .LField_Map136_BLEND
	map_block Field_Map136, .LField_Map136_SCENE
.LField_Map136_METATILES:
	.incbin "MAP/MAP_136/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map136_SHAPES:
	.incbin "MAP/MAP_136/SHAPES.table.lz"
	.balign 4, 0
.LField_Map136_GRID:
	.incbin "MAP/MAP_136/GRID.parts.lz"
	.balign 4, 0
.LField_Map136_SHAPE_GRID:
	.incbin "MAP/MAP_136/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map136_ANIMATION:
	.incbin "MAP/MAP_136/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map136_BLEND:
	.incbin "MAP/MAP_136/BLEND.script.lz"
	.balign 4, 0
.LField_Map136_SCENE:
	.incbin "MAP/MAP_136/SCENE.parts.lz"
