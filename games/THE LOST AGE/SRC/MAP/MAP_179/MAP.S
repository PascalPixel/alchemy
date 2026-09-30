@ Field_Map179: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map179
Field_Map179:
	map_header 0, 0, 128, 128, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 4, -4, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map179, .LField_Map179_METATILES
	map_block Field_Map179, .LField_Map179_SHAPES
	map_block Field_Map179, .LField_Map179_GRID
	map_block Field_Map179, .LField_Map179_SHAPE_GRID
	map_block Field_Map179, .LField_Map179_ANIMATION
	map_block Field_Map179, .LField_Map179_BLEND
	map_block Field_Map179, .LField_Map179_SCENE
.LField_Map179_METATILES:
	.incbin "MAP/MAP_179/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map179_SHAPES:
	.incbin "COMMON/MAP/MAP_015/SHAPES.table.lz"
	.balign 4, 0
.LField_Map179_GRID:
	.incbin "MAP/MAP_179/GRID.parts.lz"
	.balign 4, 0
.LField_Map179_SHAPE_GRID:
	.incbin "MAP/MAP_179/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map179_ANIMATION:
	.incbin "MAP/MAP_179/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map179_BLEND:
	.incbin "MAP/MAP_179/BLEND.script.lz"
	.balign 4, 0
.LField_Map179_SCENE:
	.incbin "MAP/MAP_179/SCENE.parts.lz"
