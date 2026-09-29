@ Field_Map154: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map154
Field_Map154:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map154, .LField_Map154_METATILES
	map_block Field_Map154, .LField_Map154_SHAPES
	map_block Field_Map154, .LField_Map154_GRID
	map_block Field_Map154, .LField_Map154_SHAPE_GRID
	map_block Field_Map154, .LField_Map154_ANIMATION
	map_block Field_Map154, .LField_Map154_BLEND
	map_block Field_Map154, .LField_Map154_SCENE
.LField_Map154_METATILES:
	.incbin "MAP/MAP_154/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map154_SHAPES:
	.incbin "MAP/MAP_154/SHAPES.table.lz"
	.balign 4, 0
.LField_Map154_GRID:
	.incbin "MAP/MAP_154/GRID.parts.lz"
	.balign 4, 0
.LField_Map154_SHAPE_GRID:
	.incbin "MAP/MAP_154/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map154_ANIMATION:
	.incbin "MAP/MAP_154/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map154_BLEND:
	.incbin "MAP/MAP_154/BLEND.script.lz"
	.balign 4, 0
.LField_Map154_SCENE:
	.incbin "MAP/MAP_154/SCENE.parts.lz"
