	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_ValePlaza
Field_ValePlaza:
	map_header 0, 0, 60, 114, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ValePlaza, .LField_ValePlaza_CELL_PICTURES
	map_offset Field_ValePlaza, .LField_ValePlaza_SHAPES
	map_offset Field_ValePlaza, .LField_ValePlaza_GRID
	map_offset Field_ValePlaza, .LField_ValePlaza_ANIMATION
	map_offset Field_ValePlaza
	map_offset Field_ValePlaza, .LField_ValePlaza_MARKERS
.LField_ValePlaza_CELL_PICTURES:
	.incbin "games/tbs/src/map/vale_plaza/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ValePlaza_SHAPES:
	.incbin "games/tbs/src/map/vale_plaza/shapes.table.lz"
	.balign 4, 0
.LField_ValePlaza_GRID:
	.incbin "games/tbs/src/map/vale_plaza/map.grid.lz"
	.balign 4, 0
.LField_ValePlaza_ANIMATION:
	.incbin "games/tbs/src/map/vale_plaza/tileset_anims.script.lz"
	.balign 4, 0
.LField_ValePlaza_MARKERS:
	treasure_spot 13, 44, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set02Palette
Tileset_Set02Palette:
	.incbin "games/tbs/src/graphics/tileset/set_02/palette.gbapal.lz"
	.byte 0xc7	@ PACKER: left in the buffer by Field_ValePlaza+0x17f
	.global Tileset_Set02TilesA
Tileset_Set02TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_02/tiles_a.4bpp.mtf"
	.byte 0xfc, 0x89, 0x00	@ PACKER: left in the buffer by Field_ValePlaza+0x27b9
	.global Tileset_Set02TilesB
Tileset_Set02TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_02/tiles_b.4bpp.mtf"
	.byte 0xdc, 0x73	@ PACKER: left in the buffer by Tileset_Set02TilesA+0x2646
	.global Tileset_Set02TilesC
Tileset_Set02TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_02/tiles_c.4bpp.mtf"
	.byte 0x5c, 0x40	@ PACKER: left in the buffer by Tileset_Set02TilesA+0x26e2
	.global Tileset_Set02TilesD
Tileset_Set02TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_02/tiles_d.4bpp.mtf"
	.byte 0xf7	@ PACKER: left in the buffer by Tileset_Set02TilesC+0x1073
