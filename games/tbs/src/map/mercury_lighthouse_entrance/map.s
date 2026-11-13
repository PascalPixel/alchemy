	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_MercuryLighthouseEntrance
Field_MercuryLighthouseEntrance:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_MercuryLighthouseEntrance, .LField_MercuryLighthouseEntrance_CELL_PICTURES
	map_offset Field_MercuryLighthouseEntrance, .LField_MercuryLighthouseEntrance_SHAPES
	map_offset Field_MercuryLighthouseEntrance, .LField_MercuryLighthouseEntrance_GRID
	map_offset Field_MercuryLighthouseEntrance, .LField_MercuryLighthouseEntrance_ANIMATION
	map_offset Field_MercuryLighthouseEntrance, .LField_MercuryLighthouseEntrance_BLEND
	map_offset Field_MercuryLighthouseEntrance, .LField_MercuryLighthouseEntrance_MARKERS
.LField_MercuryLighthouseEntrance_CELL_PICTURES:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MercuryLighthouseEntrance_SHAPES:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/shapes.table.lz"
	.balign 4, 0
.LField_MercuryLighthouseEntrance_GRID:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/map.grid.lz"
	.balign 4, 0
.LField_MercuryLighthouseEntrance_ANIMATION:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_MercuryLighthouseEntrance_BLEND:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/blend.script.lz"
	.balign 4, 0
.LField_MercuryLighthouseEntrance_MARKERS:
	treasure_spot 57, 19, 100
	treasure_spot 47, 54, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set43Palette
Tileset_Set43Palette:
	.incbin "games/tbs/src/graphics/tileset/set_43/palette.gbapal.lz"
	.byte 0xc0, 0xcf, 0x02	@ PACKER: left in the buffer by Field_MercuryLighthouseEntrance+0x16d
	.global Tileset_Set43TilesA
Tileset_Set43TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_43/tiles_a.4bpp.mtf"
	.byte 0x2b	@ PACKER: left in the buffer by Tileset_Set42TilesA+0x28bf
	.global Tileset_Set43TilesB
Tileset_Set43TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_43/tiles_b.4bpp.mtf"
	.byte 0xe5	@ PACKER: left in the buffer by Tileset_Set43TilesA+0x27c7
	.global Tileset_Set43TilesC
Tileset_Set43TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_43/tiles_c.4bpp.mtf"
	.byte 0x7a, 0x40, 0xe2	@ PACKER: left in the buffer by Tileset_Set43TilesB+0x12f9
	.global Tileset_Set43TilesD
Tileset_Set43TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_43/tiles_d.4bpp.mtf"
	.byte 0x0c	@ PACKER: left in the buffer by Tileset_Set43TilesB+0x1943
