@ Field_Map176: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map176
Field_Map176:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map176, .LField_Map176_METATILES
	map_block Field_Map176, .LField_Map176_SHAPES
	map_block Field_Map176, .LField_Map176_GRID
	map_block Field_Map176, .LField_Map176_SHAPE_GRID
	map_block Field_Map176, .LField_Map176_ANIMATION
	map_block Field_Map176, .LField_Map176_BLEND
	map_block Field_Map176, .LField_Map176_SCENE
.LField_Map176_METATILES:
	.incbin "MAP/MAP_176/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map176_SHAPES:
	.incbin "MAP/MAP_176/SHAPES.table.lz"
	.balign 4, 0
.LField_Map176_GRID:
	.incbin "MAP/MAP_176/GRID.parts.lz"
	.balign 4, 0
.LField_Map176_SHAPE_GRID:
	.incbin "MAP/MAP_176/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map176_ANIMATION:
	.incbin "MAP/MAP_176/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map176_BLEND:
	.incbin "MAP/MAP_176/BLEND.script.lz"
	.balign 4, 0
.LField_Map176_SCENE:
	.incbin "MAP/MAP_176/SCENE.parts.lz"
