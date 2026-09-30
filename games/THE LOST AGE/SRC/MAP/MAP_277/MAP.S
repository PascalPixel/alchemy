@ Field_Map277: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map277
Field_Map277:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map277, .LField_Map277_METATILES
	map_block Field_Map277, .LField_Map277_SHAPES
	map_block Field_Map277, .LField_Map277_GRID
	map_block Field_Map277, .LField_Map277_SHAPE_GRID
	map_block Field_Map277, .LField_Map277_ANIMATION
	map_block Field_Map277, .LField_Map277_BLEND
	map_block Field_Map277, .LField_Map277_SCENE
.LField_Map277_METATILES:
	.incbin "MAP/MAP_277/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map277_SHAPES:
	.incbin "COMMON/MAP/MAP_058/SHAPES.table.lz"
	.balign 4, 0
.LField_Map277_GRID:
	.incbin "MAP/MAP_277/GRID.parts.lz"
	.balign 4, 0
.LField_Map277_SHAPE_GRID:
	.incbin "MAP/MAP_277/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map277_ANIMATION:
	.incbin "COMMON/MAP/MAP_058/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map277_BLEND:
	.incbin "MAP/MAP_277/BLEND.script.lz"
	.balign 4, 0
.LField_Map277_SCENE:
	.incbin "MAP/MAP_277/SCENE.parts.lz"
