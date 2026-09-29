@ Field_Map109: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map109
Field_Map109:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map109, .LField_Map109_METATILES
	map_block Field_Map109, .LField_Map109_SHAPES
	map_block Field_Map109, .LField_Map109_GRID
	map_block Field_Map109, .LField_Map109_SHAPE_GRID
	map_block Field_Map109, .LField_Map109_ANIMATION
	map_block Field_Map109, .LField_Map109_BLEND
	map_block Field_Map109, .LField_Map109_SCENE
.LField_Map109_METATILES:
	.incbin "MAP/MAP_109/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map109_SHAPES:
	.incbin "MAP/MAP_109/SHAPES.table.lz"
	.balign 4, 0
.LField_Map109_GRID:
	.incbin "MAP/MAP_109/GRID.parts.lz"
	.balign 4, 0
.LField_Map109_SHAPE_GRID:
	.incbin "MAP/MAP_109/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map109_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map109_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map109_SCENE:
	.incbin "MAP/MAP_109/SCENE.parts.lz"
