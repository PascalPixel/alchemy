@ Field_Map259: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map259
Field_Map259:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map259, .LField_Map259_METATILES
	map_block Field_Map259, .LField_Map259_SHAPES
	map_block Field_Map259, .LField_Map259_GRID
	map_block Field_Map259, .LField_Map259_SHAPE_GRID
	map_block Field_Map259, .LField_Map259_ANIMATION
	map_block Field_Map259, .LField_Map259_BLEND
	map_block Field_Map259, .LField_Map259_SCENE
.LField_Map259_METATILES:
	.incbin "MAP/MAP_259/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map259_SHAPES:
	.incbin "MAP/MAP_259/SHAPES.table.lz"
	.balign 4, 0
.LField_Map259_GRID:
	.incbin "MAP/MAP_259/GRID.parts.lz"
	.balign 4, 0
.LField_Map259_SHAPE_GRID:
	.incbin "MAP/MAP_259/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map259_ANIMATION:
	.incbin "MAP/MAP_259/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map259_BLEND:
	.incbin "MAP/MAP_003/BLEND.script.lz"
	.balign 4, 0
.LField_Map259_SCENE:
	.incbin "MAP/MAP_259/SCENE.parts.lz"
