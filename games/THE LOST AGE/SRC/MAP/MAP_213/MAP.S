@ Field_Map213: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map213
Field_Map213:
	map_header 0, 0, 130, 130, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map213, .LField_Map213_METATILES
	map_block Field_Map213, .LField_Map213_SHAPES
	map_block Field_Map213, .LField_Map213_GRID
	map_block Field_Map213, .LField_Map213_SHAPE_GRID
	map_block Field_Map213, .LField_Map213_ANIMATION
	map_block Field_Map213, .LField_Map213_BLEND
	map_block Field_Map213, .LField_Map213_SCENE
.LField_Map213_METATILES:
	.incbin "MAP/MAP_213/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map213_SHAPES:
	.incbin "MAP/MAP_213/SHAPES.table.lz"
	.balign 4, 0
.LField_Map213_GRID:
	.incbin "MAP/MAP_213/GRID.parts.lz"
	.balign 4, 0
.LField_Map213_SHAPE_GRID:
	.incbin "MAP/MAP_213/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map213_ANIMATION:
	.incbin "MAP/MAP_211/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map213_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map213_SCENE:
	.incbin "MAP/MAP_213/SCENE.parts.lz"
