@ Field_Map266: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map266
Field_Map266:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map266, .LField_Map266_METATILES
	map_block Field_Map266, .LField_Map266_SHAPES
	map_block Field_Map266, .LField_Map266_GRID
	map_block Field_Map266, .LField_Map266_SHAPE_GRID
	map_block Field_Map266, .LField_Map266_ANIMATION
	map_block Field_Map266, .LField_Map266_BLEND
	map_block Field_Map266, .LField_Map266_SCENE
.LField_Map266_METATILES:
	.incbin "MAP/MAP_266/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map266_SHAPES:
	.incbin "MAP/MAP_266/SHAPES.table.lz"
	.balign 4, 0
.LField_Map266_GRID:
	.incbin "MAP/MAP_266/GRID.parts.lz"
	.balign 4, 0
.LField_Map266_SHAPE_GRID:
	.incbin "MAP/MAP_266/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map266_ANIMATION:
	.incbin "MAP/MAP_266/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map266_BLEND:
	.incbin "MAP/MAP_266/BLEND.script.lz"
	.balign 4, 0
.LField_Map266_SCENE:
	.incbin "MAP/MAP_266/SCENE.parts.lz"
