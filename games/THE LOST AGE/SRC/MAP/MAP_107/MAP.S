@ Field_Map107: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map107
Field_Map107:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map107, .LField_Map107_METATILES
	map_block Field_Map107, .LField_Map107_SHAPES
	map_block Field_Map107, .LField_Map107_GRID
	map_block Field_Map107, .LField_Map107_SHAPE_GRID
	map_block Field_Map107, .LField_Map107_ANIMATION
	map_block Field_Map107, .LField_Map107_BLEND
	map_block Field_Map107, .LField_Map107_SCENE
.LField_Map107_METATILES:
	.incbin "MAP/MAP_107/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map107_SHAPES:
	.incbin "MAP/MAP_107/SHAPES.table.lz"
	.balign 4, 0
.LField_Map107_GRID:
	.incbin "MAP/MAP_107/GRID.parts.lz"
	.balign 4, 0
.LField_Map107_SHAPE_GRID:
	.incbin "MAP/MAP_107/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map107_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map107_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map107_SCENE:
	.incbin "MAP/MAP_107/SCENE.parts.lz"
