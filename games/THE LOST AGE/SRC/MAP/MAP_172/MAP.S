@ Field_Map172: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map172
Field_Map172:
	map_header 0, 0, 128, 96, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, -3, 3, 0x1f, 0x1f
	map_block Field_Map172, .LField_Map172_METATILES
	map_block Field_Map172, .LField_Map172_SHAPES
	map_block Field_Map172, .LField_Map172_GRID
	map_block Field_Map172, .LField_Map172_SHAPE_GRID
	map_block Field_Map172, .LField_Map172_ANIMATION
	map_block Field_Map172, .LField_Map172_BLEND
	map_block Field_Map172, .LField_Map172_SCENE
.LField_Map172_METATILES:
	.incbin "MAP/MAP_172/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map172_SHAPES:
	.incbin "MAP/MAP_172/SHAPES.table.lz"
	.balign 4, 0
.LField_Map172_GRID:
	.incbin "MAP/MAP_172/GRID.parts.lz"
	.balign 4, 0
.LField_Map172_SHAPE_GRID:
	.incbin "MAP/MAP_172/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map172_ANIMATION:
	.incbin "MAP/MAP_172/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map172_BLEND:
	.incbin "MAP/MAP_172/BLEND.script.lz"
	.balign 4, 0
.LField_Map172_SCENE:
	.incbin "MAP/MAP_172/SCENE.parts.lz"
