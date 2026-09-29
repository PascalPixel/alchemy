@ Field_Map186: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map186
Field_Map186:
	map_header 0, 0, 128, 128, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map186, .LField_Map186_METATILES
	map_block Field_Map186, .LField_Map186_SHAPES
	map_block Field_Map186, .LField_Map186_GRID
	map_block Field_Map186, .LField_Map186_SHAPE_GRID
	map_block Field_Map186, .LField_Map186_ANIMATION
	map_block Field_Map186, .LField_Map186_BLEND
	map_block Field_Map186, .LField_Map186_SCENE
.LField_Map186_METATILES:
	.incbin "MAP/MAP_186/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map186_SHAPES:
	.incbin "MAP/MAP_186/SHAPES.table.lz"
	.balign 4, 0
.LField_Map186_GRID:
	.incbin "MAP/MAP_186/GRID.parts.lz"
	.balign 4, 0
.LField_Map186_SHAPE_GRID:
	.incbin "MAP/MAP_186/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map186_ANIMATION:
	.incbin "MAP/MAP_183/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map186_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map186_SCENE:
	.incbin "MAP/MAP_186/SCENE.parts.lz"
