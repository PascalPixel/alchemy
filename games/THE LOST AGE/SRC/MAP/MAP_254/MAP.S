@ Field_Map254: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map254
Field_Map254:
	map_header 0, 0, 38, 50, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 38, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 50, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map254, .LField_Map254_METATILES
	map_block Field_Map254, .LField_Map254_SHAPES
	map_block Field_Map254, .LField_Map254_GRID
	map_block Field_Map254, .LField_Map254_SHAPE_GRID
	map_block Field_Map254, .LField_Map254_ANIMATION
	map_block Field_Map254, .LField_Map254_BLEND
	map_block Field_Map254, .LField_Map254_SCENE
.LField_Map254_METATILES:
	.incbin "MAP/MAP_254/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map254_SHAPES:
	.incbin "MAP/MAP_254/SHAPES.table.lz"
	.balign 4, 0
.LField_Map254_GRID:
	.incbin "MAP/MAP_254/GRID.parts.lz"
	.balign 4, 0
.LField_Map254_SHAPE_GRID:
	.incbin "MAP/MAP_254/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map254_ANIMATION:
	.incbin "MAP/MAP_254/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map254_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map254_SCENE:
	.incbin "MAP/MAP_254/SCENE.parts.lz"
