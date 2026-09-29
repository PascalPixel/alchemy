@ Field_Map174: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map174
Field_Map174:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map174, .LField_Map174_METATILES
	map_block Field_Map174, .LField_Map174_SHAPES
	map_block Field_Map174, .LField_Map174_GRID
	map_block Field_Map174, .LField_Map174_SHAPE_GRID
	map_block Field_Map174, .LField_Map174_ANIMATION
	map_block Field_Map174, .LField_Map174_BLEND
	map_block Field_Map174, .LField_Map174_SCENE
.LField_Map174_METATILES:
	.incbin "MAP/MAP_174/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map174_SHAPES:
	.incbin "MAP/MAP_174/SHAPES.table.lz"
	.balign 4, 0
.LField_Map174_GRID:
	.incbin "MAP/MAP_174/GRID.parts.lz"
	.balign 4, 0
.LField_Map174_SHAPE_GRID:
	.incbin "MAP/MAP_174/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map174_ANIMATION:
	.incbin "MAP/MAP_174/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map174_BLEND:
	.incbin "MAP/MAP_057/BLEND.script.lz"
	.balign 4, 0
.LField_Map174_SCENE:
	.incbin "MAP/MAP_174/SCENE.parts.lz"
