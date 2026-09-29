@ Field_Map104: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map104
Field_Map104:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map104, .LField_Map104_METATILES
	map_block Field_Map104, .LField_Map104_SHAPES
	map_block Field_Map104, .LField_Map104_GRID
	map_block Field_Map104, .LField_Map104_SHAPE_GRID
	map_block Field_Map104, .LField_Map104_ANIMATION
	map_block Field_Map104, .LField_Map104_BLEND
	map_block Field_Map104, .LField_Map104_SCENE
.LField_Map104_METATILES:
	.incbin "MAP/MAP_104/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map104_SHAPES:
	.incbin "MAP/MAP_104/SHAPES.table.lz"
	.balign 4, 0
.LField_Map104_GRID:
	.incbin "MAP/MAP_104/GRID.parts.lz"
	.balign 4, 0
.LField_Map104_SHAPE_GRID:
	.incbin "MAP/MAP_104/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map104_ANIMATION:
	.incbin "MAP/MAP_104/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map104_BLEND:
	.incbin "MAP/MAP_104/BLEND.script.lz"
	.balign 4, 0
.LField_Map104_SCENE:
	.incbin "MAP/MAP_104/SCENE.parts.lz"
