	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map051
Field_Map051:
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map051, .LField_Map051_CELL_PICTURES
	map_offset Field_Map051, .LField_Map051_SHAPES
	map_offset Field_Map051, .LField_Map051_GRID
	map_offset Field_Map051, .LField_Map051_ANIMATION
	map_offset Field_Map051, .LField_Map051_BLEND
	map_offset Field_Map051, .LField_Map051_MARKERS
.LField_Map051_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_051/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "games/tbs/src/map/map_051/shapes.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "games/tbs/src/map/map_051/map_de.grid.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "games/tbs/src/map/map_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map051_MARKERS:
	treasure_spot 9, 11, 100
	treasure_spot 21, 18, 101
	treasure_spot 7, 24, 102
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map051, .LField_Map051_CELL_PICTURES
	map_offset Field_Map051, .LField_Map051_SHAPES
	map_offset Field_Map051, .LField_Map051_GRID
	map_offset Field_Map051, .LField_Map051_ANIMATION
	map_offset Field_Map051, .LField_Map051_BLEND
	map_offset Field_Map051, .LField_Map051_MARKERS
.LField_Map051_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_051/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "games/tbs/src/map/map_051/shapes.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "games/tbs/src/map/map_051/map_es.grid.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "games/tbs/src/map/map_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map051_MARKERS:
	treasure_spot 9, 11, 100
	treasure_spot 21, 18, 101
	treasure_spot 7, 24, 102
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map051, .LField_Map051_CELL_PICTURES
	map_offset Field_Map051, .LField_Map051_SHAPES
	map_offset Field_Map051, .LField_Map051_GRID
	map_offset Field_Map051, .LField_Map051_ANIMATION
	map_offset Field_Map051, .LField_Map051_BLEND
	map_offset Field_Map051, .LField_Map051_MARKERS
.LField_Map051_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_051/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "games/tbs/src/map/map_051/shapes.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "games/tbs/src/map/map_051/map_es.grid.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "games/tbs/src/map/map_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map051_MARKERS:
	treasure_spot 9, 11, 100
	treasure_spot 21, 18, 101
	treasure_spot 7, 24, 102
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map051, .LField_Map051_CELL_PICTURES
	map_offset Field_Map051, .LField_Map051_SHAPES
	map_offset Field_Map051, .LField_Map051_GRID
	map_offset Field_Map051, .LField_Map051_ANIMATION
	map_offset Field_Map051, .LField_Map051_BLEND
	map_offset Field_Map051, .LField_Map051_MARKERS
.LField_Map051_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_051/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "games/tbs/src/map/map_051/shapes.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "games/tbs/src/map/map_051/map_es.grid.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "games/tbs/src/map/map_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map051_MARKERS:
	treasure_spot 9, 11, 100
	treasure_spot 21, 18, 101
	treasure_spot 7, 24, 102
	treasure_spots_end
	.else
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map051, .LField_Map051_CELL_PICTURES
	map_offset Field_Map051, .LField_Map051_SHAPES
	map_offset Field_Map051, .LField_Map051_GRID
	map_offset Field_Map051, .LField_Map051_ANIMATION
	map_offset Field_Map051, .LField_Map051_BLEND
	map_offset Field_Map051, .LField_Map051_MARKERS
.LField_Map051_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_051/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "games/tbs/src/map/map_051/shapes.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "games/tbs/src/map/map_051/map.grid.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "games/tbs/src/map/map_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map051_MARKERS:
	treasure_spot 9, 11, 100
	treasure_spot 21, 18, 101
	treasure_spot 7, 24, 102
	treasure_spots_end
	.endif
	.endif
	.endif
	.endif
	.global Tileset_Set41Palette
Tileset_Set41Palette:
	.incbin "games/tbs/src/graphics/tileset/set_41/palette.gbapal.lz"
	.byte 0xc2, 0x82, 0x04	@ PACKER: left in the buffer by Field_Map051+0x145
	.global Tileset_Set41TilesA
Tileset_Set41TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_41/tiles_a.4bpp.mtf"
	.global Tileset_Set41TilesB
Tileset_Set41TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_41/tiles_b.4bpp.mtf"
	.byte 0x69	@ PACKER: left in the buffer by Tileset_Set41TilesA+0x14b7
	.global Tileset_Set41TilesC
Tileset_Set41TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_41/tiles_c.4bpp.mtf"
	.byte 0xde	@ PACKER: left in the buffer by Tileset_Set40TilesA+0x2633
	.global Tileset_Set41TilesD
Tileset_Set41TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_41/tiles_d.4bpp.mtf"
