@ Field_Map201: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map201
Field_Map201:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map201, .LField_Map201_METATILES
	map_block Field_Map201, .LField_Map201_SHAPES
	map_block Field_Map201, .LField_Map201_GRID
	map_block Field_Map201, .LField_Map201_SHAPE_GRID
	map_block Field_Map201, .LField_Map201_ANIMATION
	map_block Field_Map201, .LField_Map201_BLEND
	map_block Field_Map201, .LField_Map201_SCENE
.LField_Map201_METATILES:
	.incbin "MAP/MAP_201/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map201_SHAPES:
	.incbin "MAP/MAP_072/SHAPES.table.lz"
	.balign 4, 0
.LField_Map201_GRID:
	.incbin "MAP/MAP_201/GRID.parts.lz"
	.balign 4, 0
.LField_Map201_SHAPE_GRID:
	.incbin "MAP/MAP_201/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map201_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map201_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map201_SCENE:
	.incbin "MAP/MAP_201/SCENE.parts.lz"
