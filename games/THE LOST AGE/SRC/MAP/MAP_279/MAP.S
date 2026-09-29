@ Field_Map279: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map279
Field_Map279:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map279, .LField_Map279_METATILES
	map_block Field_Map279, .LField_Map279_SHAPES
	map_block Field_Map279, .LField_Map279_GRID
	map_block Field_Map279, .LField_Map279_SHAPE_GRID
	map_block Field_Map279, .LField_Map279_ANIMATION
	map_block Field_Map279, .LField_Map279_BLEND
	map_block Field_Map279, .LField_Map279_SCENE
.LField_Map279_METATILES:
	.incbin "MAP/MAP_279/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map279_SHAPES:
	.incbin "MAP/MAP_279/SHAPES.table.lz"
	.balign 4, 0
.LField_Map279_GRID:
	.incbin "MAP/MAP_279/GRID.parts.lz"
	.balign 4, 0
.LField_Map279_SHAPE_GRID:
	.incbin "MAP/MAP_279/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map279_ANIMATION:
	.incbin "MAP/MAP_279/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map279_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map279_SCENE:
	.incbin "MAP/MAP_279/SCENE.parts.lz"
