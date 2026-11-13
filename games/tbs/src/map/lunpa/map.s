	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Lunpa
Field_Lunpa:
	map_header 0, 0, 56, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Lunpa, .LField_Lunpa_CELL_PICTURES
	map_offset Field_Lunpa, .LField_Lunpa_SHAPES
	map_offset Field_Lunpa, .LField_Lunpa_GRID
	map_offset Field_Lunpa, .LField_Lunpa_ANIMATION
	map_offset Field_Lunpa, .LField_Lunpa_BLEND
	map_offset Field_Lunpa, .LField_Lunpa_MARKERS
.LField_Lunpa_CELL_PICTURES:
	.incbin "games/tbs/src/map/lunpa/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Lunpa_SHAPES:
	.incbin "games/tbs/src/map/lunpa/shapes.table.lz"
	.balign 4, 0
.LField_Lunpa_GRID:
	.incbin "games/tbs/src/map/lunpa/map.grid.lz"
	.balign 4, 0
.LField_Lunpa_ANIMATION:
	.incbin "games/tbs/src/map/kolima/tileset_anims.script.lz"
	.balign 4, 0
.LField_Lunpa_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_Lunpa_MARKERS:
	treasure_spot 1, 11, 100
	treasure_spot 24, 12, 101
	treasure_spot 7, 16, 102
	treasure_spot 28, 31, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_LunpaPalette
Tileset_LunpaPalette:
	.incbin "games/tbs/src/graphics/tileset/lunpa/palette.gbapal.lz"
	.byte 0x14, 0x41, 0xb9	@ PACKER: left in the buffer by Field_Lunpa+0x189
	.global Tileset_LunpaTilesA
Tileset_LunpaTilesA:
	.incbin "games/tbs/src/graphics/tileset/lunpa/tiles_a.4bpp.mtf"
	.byte 0x09, 0x02	@ PACKER: left in the buffer by Field_Lunpa+0x1a82
	.global Tileset_LunpaTilesB
Tileset_LunpaTilesB:
	.incbin "games/tbs/src/graphics/tileset/lunpa/tiles_b.4bpp.mtf"
