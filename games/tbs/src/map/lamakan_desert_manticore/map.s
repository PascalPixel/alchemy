	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_LamakanDesertManticore
Field_LamakanDesertManticore:
	map_header 0, 0, 192, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LamakanDesertManticore, .LField_LamakanDesertManticore_CELL_PICTURES
	map_offset Field_LamakanDesertManticore, .LField_LamakanDesertManticore_SHAPES
	map_offset Field_LamakanDesertManticore, .LField_LamakanDesertManticore_GRID
	map_offset Field_LamakanDesertManticore, .LField_LamakanDesertManticore_ANIMATION
	map_offset Field_LamakanDesertManticore
	map_offset Field_LamakanDesertManticore, .LField_LamakanDesertManticore_MARKERS
.LField_LamakanDesertManticore_CELL_PICTURES:
	.incbin "games/tbs/src/map/lamakan_desert_manticore/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LamakanDesertManticore_SHAPES:
	.incbin "games/tbs/src/map/lamakan_desert_manticore/shapes.table.lz"
	.balign 4, 0
.LField_LamakanDesertManticore_GRID:
	.incbin "games/tbs/src/map/lamakan_desert_manticore/map.grid.lz"
	.balign 4, 0
.LField_LamakanDesertManticore_ANIMATION:
	.incbin "games/tbs/src/map/lamakan_desert_smog/tileset_anims.script.lz"
	.balign 4, 0
.LField_LamakanDesertManticore_MARKERS:
	treasure_spot 10, 15, 107
	treasure_spots_end
	.balign 4, 0
