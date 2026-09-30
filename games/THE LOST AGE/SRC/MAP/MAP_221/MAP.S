@ Field_Map221: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map221
Field_Map221:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map221, .LField_Map221_METATILES
	map_block Field_Map221, .LField_Map221_SHAPES
	map_block Field_Map221, .LField_Map221_GRID
	map_block Field_Map221, .LField_Map221_SHAPE_GRID
	map_block Field_Map221, .LField_Map221_ANIMATION
	map_block Field_Map221, .LField_Map221_BLEND
	map_block Field_Map221, .LField_Map221_SCENE
.LField_Map221_METATILES:
	.incbin "MAP/MAP_221/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map221_SHAPES:
	.incbin "MAP/MAP_221/SHAPES.table.lz"
	.balign 4, 0
.LField_Map221_GRID:
	.incbin "MAP/MAP_221/GRID.parts.lz"
	.balign 4, 0
.LField_Map221_SHAPE_GRID:
	.incbin "MAP/MAP_221/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map221_ANIMATION:
	.incbin "MAP/MAP_221/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map221_BLEND:
	.incbin "COMMON/MAP/MAP_018/BLEND.script.lz"
	.balign 4, 0
.LField_Map221_SCENE:
	.incbin "MAP/MAP_221/SCENE.parts.lz"
