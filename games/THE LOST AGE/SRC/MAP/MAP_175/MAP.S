@ Field_Map175: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map175
Field_Map175:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map175, .LField_Map175_METATILES
	map_block Field_Map175, .LField_Map175_SHAPES
	map_block Field_Map175, .LField_Map175_GRID
	map_block Field_Map175, .LField_Map175_SHAPE_GRID
	map_block Field_Map175, .LField_Map175_ANIMATION
	map_block Field_Map175, .LField_Map175_BLEND
	map_block Field_Map175, .LField_Map175_SCENE
.LField_Map175_METATILES:
	.incbin "MAP/MAP_175/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map175_SHAPES:
	.incbin "MAP/MAP_175/SHAPES.table.lz"
	.balign 4, 0
.LField_Map175_GRID:
	.incbin "MAP/MAP_175/GRID.parts.lz"
	.balign 4, 0
.LField_Map175_SHAPE_GRID:
	.incbin "MAP/MAP_175/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map175_ANIMATION:
	.incbin "MAP/MAP_175/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map175_BLEND:
	.incbin "MAP/MAP_057/BLEND.script.lz"
	.balign 4, 0
.LField_Map175_SCENE:
	.incbin "MAP/MAP_175/SCENE.parts.lz"
