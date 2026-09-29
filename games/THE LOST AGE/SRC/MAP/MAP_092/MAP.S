@ Field_Map092: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map092
Field_Map092:
	map_header 0, 0, 90, 90, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map092, .LField_Map092_METATILES
	map_block Field_Map092, .LField_Map092_SHAPES
	map_block Field_Map092, .LField_Map092_GRID
	map_block Field_Map092, .LField_Map092_SHAPE_GRID
	map_block Field_Map092, .LField_Map092_ANIMATION
	map_block Field_Map092, .LField_Map092_BLEND
	map_block Field_Map092, .LField_Map092_SCENE
.LField_Map092_METATILES:
	.incbin "MAP/MAP_092/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map092_SHAPES:
	.incbin "MAP/MAP_092/SHAPES.table.lz"
	.balign 4, 0
.LField_Map092_GRID:
	.incbin "MAP/MAP_092/GRID.parts.lz"
	.balign 4, 0
.LField_Map092_SHAPE_GRID:
	.incbin "MAP/MAP_092/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map092_ANIMATION:
	.incbin "MAP/MAP_092/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map092_BLEND:
	.incbin "MAP/MAP_092/BLEND.script.lz"
	.balign 4, 0
.LField_Map092_SCENE:
	.incbin "MAP/MAP_092/SCENE.parts.lz"
