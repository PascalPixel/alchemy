@ Field_Map157: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map157
Field_Map157:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map157, .LField_Map157_METATILES
	map_block Field_Map157, .LField_Map157_SHAPES
	map_block Field_Map157, .LField_Map157_GRID
	map_block Field_Map157, .LField_Map157_SHAPE_GRID
	map_block Field_Map157, .LField_Map157_ANIMATION
	map_block Field_Map157, .LField_Map157_BLEND
	map_block Field_Map157, .LField_Map157_SCENE
.LField_Map157_METATILES:
	.incbin "MAP/MAP_157/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map157_SHAPES:
	.incbin "MAP/MAP_157/SHAPES.table.lz"
	.balign 4, 0
.LField_Map157_GRID:
	.incbin "MAP/MAP_157/GRID.parts.lz"
	.balign 4, 0
.LField_Map157_SHAPE_GRID:
	.incbin "MAP/MAP_157/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map157_ANIMATION:
	.incbin "MAP/MAP_157/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map157_BLEND:
	.incbin "MAP/MAP_157/BLEND.script.lz"
	.balign 4, 0
.LField_Map157_SCENE:
	.incbin "MAP/MAP_157/SCENE.parts.lz"
