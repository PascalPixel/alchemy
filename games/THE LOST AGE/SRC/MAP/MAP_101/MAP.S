@ Field_Map101: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map101
Field_Map101:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map101, .LField_Map101_METATILES
	map_block Field_Map101, .LField_Map101_SHAPES
	map_block Field_Map101, .LField_Map101_GRID
	map_block Field_Map101, .LField_Map101_SHAPE_GRID
	map_block Field_Map101, .LField_Map101_ANIMATION
	map_block Field_Map101, .LField_Map101_BLEND
	map_block Field_Map101, .LField_Map101_SCENE
.LField_Map101_METATILES:
	.incbin "MAP/MAP_101/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map101_SHAPES:
	.incbin "MAP/MAP_101/SHAPES.table.lz"
	.balign 4, 0
.LField_Map101_GRID:
	.incbin "MAP/MAP_101/GRID.parts.lz"
	.balign 4, 0
.LField_Map101_SHAPE_GRID:
	.incbin "MAP/MAP_101/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map101_ANIMATION:
	.incbin "MAP/MAP_035/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map101_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map101_SCENE:
	.incbin "MAP/MAP_101/SCENE.parts.lz"
