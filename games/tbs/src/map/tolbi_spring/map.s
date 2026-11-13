	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_TolbiSpring
Field_TolbiSpring:
	map_header 0, 0, 30, 22, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 132, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_TolbiSpring, .LField_TolbiSpring_CELL_PICTURES
	map_offset Field_TolbiSpring, .LField_TolbiSpring_SHAPES
	map_offset Field_TolbiSpring, .LField_TolbiSpring_GRID
	map_offset Field_TolbiSpring, .LField_TolbiSpring_ANIMATION
	map_offset Field_TolbiSpring, .LField_TolbiSpring_BLEND
	map_offset Field_TolbiSpring, .LField_TolbiSpring_MARKERS
.LField_TolbiSpring_CELL_PICTURES:
	.incbin "games/tbs/src/map/tolbi_spring/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TolbiSpring_SHAPES:
	.incbin "games/tbs/src/map/tolbi_spring/shapes.table.lz"
	.balign 4, 0
.LField_TolbiSpring_GRID:
	.incbin "games/tbs/src/map/tolbi_spring/map.grid.lz"
	.balign 4, 0
.LField_TolbiSpring_ANIMATION:
	.incbin "games/tbs/src/map/tolbi_spring/tileset_anims.script.lz"
	.balign 4, 0
.LField_TolbiSpring_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_TolbiSpring_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set67Palette
Tileset_Set67Palette:
	.incbin "games/tbs/src/graphics/tileset/set_67/palette.gbapal.lz"
	.global Tileset_Set67TilesA
Tileset_Set67TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_67/tiles_a.4bpp.mtf"
	.byte 0x4e, 0x14	@ PACKER: left in the buffer by Tileset_Set66TilesC+0x1856
	.global Tileset_Set67TilesB
Tileset_Set67TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_67/tiles_b.4bpp.mtf"
	.byte 0x58, 0xfe, 0x8a	@ PACKER: left in the buffer by Tileset_Set67TilesA+0xc6d
	.global Tileset_Set67TilesC
Tileset_Set67TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_67/tiles_c.4bpp.mtf"
	.byte 0x0f, 0x7c, 0xfe	@ PACKER: left in the buffer by Tileset_Set67TilesB+0x2a5
	.global Tileset_Set67TilesD
Tileset_Set67TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_67/tiles_d.4bpp.mtf"
	.byte 0xc4, 0x02	@ PACKER: left in the buffer by Tileset_Set67TilesA+0xe82
