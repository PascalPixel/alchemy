@ Field_Map220: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map220
Field_Map220:
	map_header 0, 0, 128, 128, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 1, 1, 0x1f, 0x1f
	map_block Field_Map220, .LField_Map220_METATILES
	map_block Field_Map220, .LField_Map220_SHAPES
	map_block Field_Map220, .LField_Map220_GRID
	map_block Field_Map220, .LField_Map220_SHAPE_GRID
	map_block Field_Map220, .LField_Map220_ANIMATION
	map_block Field_Map220, .LField_Map220_BLEND
	map_block Field_Map220, .LField_Map220_SCENE
.LField_Map220_METATILES:
	.incbin "MAP/MAP_220/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map220_SHAPES:
	.incbin "MAP/MAP_220/SHAPES.table.lz"
	.balign 4, 0
.LField_Map220_GRID:
	.incbin "MAP/MAP_220/GRID.parts.lz"
	.balign 4, 0
.LField_Map220_SHAPE_GRID:
	.incbin "MAP/MAP_220/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map220_ANIMATION:
	.incbin "MAP/MAP_220/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map220_BLEND:
	.incbin "MAP/MAP_220/BLEND.script.lz"
	.balign 4, 0
.LField_Map220_SCENE:
	.incbin "MAP/MAP_220/SCENE.parts.lz"
