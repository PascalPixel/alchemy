@ Field_Map178: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map178
Field_Map178:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map178, .LField_Map178_METATILES
	map_block Field_Map178, .LField_Map178_SHAPES
	map_block Field_Map178, .LField_Map178_GRID
	map_block Field_Map178, .LField_Map178_SHAPE_GRID
	map_block Field_Map178, .LField_Map178_ANIMATION
	map_block Field_Map178, .LField_Map178_BLEND
	map_block Field_Map178, .LField_Map178_SCENE
.LField_Map178_METATILES:
	.incbin "MAP/MAP_178/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map178_SHAPES:
	.incbin "MAP/MAP_178/SHAPES.table.lz"
	.balign 4, 0
.LField_Map178_GRID:
	.incbin "MAP/MAP_178/GRID.parts.lz"
	.balign 4, 0
.LField_Map178_SHAPE_GRID:
	.incbin "MAP/MAP_178/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map178_ANIMATION:
	.incbin "MAP/MAP_041/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map178_BLEND:
	.incbin "MAP/MAP_010/BLEND.script.lz"
	.balign 4, 0
.LField_Map178_SCENE:
	.incbin "MAP/MAP_178/SCENE.parts.lz"
