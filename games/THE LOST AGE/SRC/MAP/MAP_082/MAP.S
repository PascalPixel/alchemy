@ Field_Map082: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map082
Field_Map082:
	map_header 0, 0, 128, 128, 2, 1, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 5, 5, 0x1f, 0x1f
	map_block Field_Map082, .LField_Map082_METATILES
	map_block Field_Map082, .LField_Map082_SHAPES
	map_block Field_Map082, .LField_Map082_GRID
	map_block Field_Map082, .LField_Map082_SHAPE_GRID
	map_block Field_Map082, .LField_Map082_ANIMATION
	map_block Field_Map082, .LField_Map082_BLEND
	map_block Field_Map082, .LField_Map082_SCENE
.LField_Map082_METATILES:
	.incbin "MAP/MAP_082/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map082_SHAPES:
	.incbin "MAP/MAP_082/SHAPES.table.lz"
	.balign 4, 0
.LField_Map082_GRID:
	.incbin "MAP/MAP_082/GRID.parts.lz"
	.balign 4, 0
.LField_Map082_SHAPE_GRID:
	.incbin "MAP/MAP_082/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map082_ANIMATION:
	.incbin "MAP/MAP_082/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map082_BLEND:
	.incbin "MAP/MAP_082/BLEND.script.lz"
	.balign 4, 0
.LField_Map082_SCENE:
	.incbin "MAP/MAP_082/SCENE.parts.lz"
