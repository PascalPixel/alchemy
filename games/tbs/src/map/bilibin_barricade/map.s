	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_BilibinBarricade
Field_BilibinBarricade:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_CELL_PICTURES
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_SHAPES
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_GRID
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_ANIMATION
	map_offset Field_BilibinBarricade
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_MARKERS
.LField_BilibinBarricade_CELL_PICTURES:
	.incbin "games/tbs/src/map/bilibin_barricade/cell_pictures_es.delta0.lz"
	.balign 4, 0
.LField_BilibinBarricade_SHAPES:
	.incbin "games/tbs/src/map/bilibin_barricade/shapes.table.lz"
	.balign 4, 0
.LField_BilibinBarricade_GRID:
	.incbin "games/tbs/src/map/bilibin_barricade/map_es.grid.lz"
	.balign 4, 0
.LField_BilibinBarricade_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_barricade/tileset_anims.script.lz"
	.balign 4, 0
.LField_BilibinBarricade_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_CELL_PICTURES
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_SHAPES
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_GRID
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_ANIMATION
	map_offset Field_BilibinBarricade
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_MARKERS
.LField_BilibinBarricade_CELL_PICTURES:
	.incbin "games/tbs/src/map/bilibin_barricade/cell_pictures_es.delta0.lz"
	.balign 4, 0
.LField_BilibinBarricade_SHAPES:
	.incbin "games/tbs/src/map/bilibin_barricade/shapes.table.lz"
	.balign 4, 0
.LField_BilibinBarricade_GRID:
	.incbin "games/tbs/src/map/bilibin_barricade/map_es.grid.lz"
	.balign 4, 0
.LField_BilibinBarricade_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_barricade/tileset_anims.script.lz"
	.balign 4, 0
.LField_BilibinBarricade_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_CELL_PICTURES
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_SHAPES
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_GRID
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_ANIMATION
	map_offset Field_BilibinBarricade
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_MARKERS
.LField_BilibinBarricade_CELL_PICTURES:
	.incbin "games/tbs/src/map/bilibin_barricade/cell_pictures_es.delta0.lz"
	.balign 4, 0
.LField_BilibinBarricade_SHAPES:
	.incbin "games/tbs/src/map/bilibin_barricade/shapes.table.lz"
	.balign 4, 0
.LField_BilibinBarricade_GRID:
	.incbin "games/tbs/src/map/bilibin_barricade/map_es.grid.lz"
	.balign 4, 0
.LField_BilibinBarricade_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_barricade/tileset_anims.script.lz"
	.balign 4, 0
.LField_BilibinBarricade_MARKERS:
	treasure_spots_end
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_CELL_PICTURES
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_SHAPES
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_GRID
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_ANIMATION
	map_offset Field_BilibinBarricade
	map_offset Field_BilibinBarricade, .LField_BilibinBarricade_MARKERS
.LField_BilibinBarricade_CELL_PICTURES:
	.incbin "games/tbs/src/map/bilibin_barricade/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_BilibinBarricade_SHAPES:
	.incbin "games/tbs/src/map/bilibin_barricade/shapes.table.lz"
	.balign 4, 0
.LField_BilibinBarricade_GRID:
	.incbin "games/tbs/src/map/bilibin_barricade/map.grid.lz"
	.balign 4, 0
.LField_BilibinBarricade_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_barricade/tileset_anims.script.lz"
	.balign 4, 0
.LField_BilibinBarricade_MARKERS:
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_BilibinBarricadePalette
Tileset_BilibinBarricadePalette:
	.incbin "games/tbs/src/graphics/tileset/bilibin_barricade/palette.gbapal.lz"
	.byte 0x08	@ PACKER: left in the buffer by Field_BilibinBarricade+0x183
	.global Tileset_BilibinBarricadeTilesA
Tileset_BilibinBarricadeTilesA:
	.incbin "games/tbs/src/graphics/tileset/bilibin_barricade/tiles_a.4bpp.mtf"
	.byte 0x9e	@ PACKER: left in the buffer by Tileset_Set23TilesA+0x2b3b
	.global Tileset_BilibinBarricadeTilesB
Tileset_BilibinBarricadeTilesB:
	.incbin "games/tbs/src/graphics/tileset/bilibin_barricade/tiles_b.4bpp.mtf"
	.byte 0xf1, 0xb3, 0x21	@ PACKER: left in the buffer by Tileset_BilibinBarricadeTilesA+0x2779
	.global Tileset_BilibinBarricadeTilesC
Tileset_BilibinBarricadeTilesC:
	.incbin "games/tbs/src/graphics/tileset/bilibin_barricade/tiles_c.4bpp.mtf"
	.byte 0xc9, 0xe1	@ PACKER: left in the buffer by Tileset_BilibinBarricadeTilesB+0xfea
