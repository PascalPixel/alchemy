@ Field_Map116: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map116
Field_Map116:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map116, .LField_Map116_METATILES
	map_block Field_Map116, .LField_Map116_SHAPES
	map_block Field_Map116, .LField_Map116_GRID
	map_block Field_Map116, .LField_Map116_SHAPE_GRID
	map_block Field_Map116, .LField_Map116_ANIMATION
	map_block Field_Map116, .LField_Map116_BLEND
	map_block Field_Map116, .LField_Map116_SCENE
.LField_Map116_METATILES:
	.incbin "MAP/MAP_116/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map116_SHAPES:
	.incbin "MAP/MAP_116/SHAPES.table.lz"
	.balign 4, 0
.LField_Map116_GRID:
	.incbin "MAP/MAP_116/GRID.parts.lz"
	.balign 4, 0
.LField_Map116_SHAPE_GRID:
	.incbin "MAP/MAP_116/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map116_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map116_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map116_SCENE:
	.incbin "MAP/MAP_116/SCENE.parts.lz"
