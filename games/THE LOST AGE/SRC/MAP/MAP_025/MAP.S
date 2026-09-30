@ Field_Map025: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map025
Field_Map025:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map025, .LField_Map025_METATILES
	map_block Field_Map025, .LField_Map025_SHAPES
	map_block Field_Map025, .LField_Map025_GRID
	map_block Field_Map025, .LField_Map025_SHAPE_GRID
	map_block Field_Map025, .LField_Map025_ANIMATION
	map_block Field_Map025, .LField_Map025_BLEND
	map_block Field_Map025, .LField_Map025_SCENE
.LField_Map025_METATILES:
	.incbin "MAP/MAP_025/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map025_SHAPES:
	.incbin "COMMON/MAP/MAP_130/SHAPES.table.lz"
	.balign 4, 0
.LField_Map025_GRID:
	.incbin "MAP/MAP_025/GRID.parts.lz"
	.balign 4, 0
.LField_Map025_SHAPE_GRID:
	.incbin "MAP/MAP_025/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map025_ANIMATION:
	.incbin "MAP/MAP_025/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map025_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map025_SCENE:
	.incbin "MAP/MAP_025/SCENE.parts.lz"
