@ Field_Map061: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map061
Field_Map061:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map061, .LField_Map061_METATILES
	map_block Field_Map061, .LField_Map061_SHAPES
	map_block Field_Map061, .LField_Map061_GRID
	map_block Field_Map061, .LField_Map061_SHAPE_GRID
	map_block Field_Map061, .LField_Map061_ANIMATION
	map_block Field_Map061, .LField_Map061_BLEND
	map_block Field_Map061, .LField_Map061_SCENE
.LField_Map061_METATILES:
	.incbin "MAP/MAP_061/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map061_SHAPES:
	.incbin "MAP/MAP_061/SHAPES.table.lz"
	.balign 4, 0
.LField_Map061_GRID:
	.incbin "MAP/MAP_061/GRID.parts.lz"
	.balign 4, 0
.LField_Map061_SHAPE_GRID:
	.incbin "MAP/MAP_061/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map061_ANIMATION:
	.incbin "MAP/MAP_061/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map061_BLEND:
	.incbin "MAP/MAP_061/BLEND.script.lz"
	.balign 4, 0
.LField_Map061_SCENE:
	.incbin "MAP/MAP_061/SCENE.parts.lz"
