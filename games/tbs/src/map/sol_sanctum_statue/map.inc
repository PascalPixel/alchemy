	.global Field_SolSanctumStatue
Field_SolSanctumStatue:
	map_header 0, 0, 100, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SolSanctumStatue, .LField_SolSanctumStatue_CELL_PICTURES
	map_offset Field_SolSanctumStatue, .LField_SolSanctumStatue_SHAPES
	map_offset Field_SolSanctumStatue, .LField_SolSanctumStatue_GRID
	map_offset Field_SolSanctumStatue, .LField_SolSanctumStatue_ANIMATION
	map_offset Field_SolSanctumStatue, .LField_SolSanctumStatue_BLEND
	map_offset Field_SolSanctumStatue, .LField_SolSanctumStatue_MARKERS
.LField_SolSanctumStatue_CELL_PICTURES:
	.incbin "games/tbs/src/map/sol_sanctum_statue/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_SolSanctumStatue_SHAPES:
	.incbin "games/tbs/src/map/sol_sanctum_statue/shapes.table.lz"
	.balign 4, 0
.LField_SolSanctumStatue_GRID:
	.incbin "games/tbs/src/map/sol_sanctum_statue/map.grid.lz"
	.balign 4, 0
.LField_SolSanctumStatue_ANIMATION:
	.incbin "games/tbs/src/map/sol_sanctum_statue/tileset_anims.script.lz"
	.balign 4, 0
.LField_SolSanctumStatue_BLEND:
	.incbin "games/tbs/src/map/sol_sanctum_statue/blend.script.lz"
	.balign 4, 0
.LField_SolSanctumStatue_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_SolSanctumStatuePalette
Tileset_SolSanctumStatuePalette:
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_statue/palette_es.gbapal.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_statue/palette_es.gbapal.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_statue/palette_es.gbapal.lz"
	.else
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_statue/palette.gbapal.lz"
	.endif
	.endif
	.endif
	.byte 0xd4	@ PACKER: left in the buffer by Field_SolSanctumStatue+0x133
	.global Tileset_SolSanctumStatueTilesA
Tileset_SolSanctumStatueTilesA:
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_statue/tiles_a_es.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_statue/tiles_a_es.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_statue/tiles_a_es.4bpp.mtf"
	.else
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_statue/tiles_a.4bpp.mtf"
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x89, 0x99	@ PACKER: left in the buffer by Tileset_SolSanctumLunaSolRoomsTilesA+0x2482
	.else
	.ifdef TBS_EDITION_FR
	.byte 0x89, 0x99	@ PACKER: left in the buffer by Tileset_SolSanctumLunaSolRoomsTilesA+0x2482
	.else
	.ifdef TBS_EDITION_IT
	.byte 0x89, 0x99	@ PACKER: left in the buffer by Tileset_SolSanctumLunaSolRoomsTilesA+0x2482
	.else
	.byte 0x06	@ PACKER: left in the buffer by Tileset_SolSanctumLunaSolRoomsTilesA+0x247f
	.endif
	.endif
	.endif
	.global Tileset_SolSanctumStatueTilesB
Tileset_SolSanctumStatueTilesB:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_statue/tiles_b.4bpp.mtf"
	.ifdef TBS_EDITION_JA
	.byte 0xee	@ PACKER: left in the buffer by Tileset_SolSanctumStatueTilesA+0xd7b
	.else
	.ifdef TBS_EDITION_EN
	.byte 0xee	@ PACKER: left in the buffer by Tileset_SolSanctumStatueTilesA+0xd7b
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xee	@ PACKER: left in the buffer by Tileset_SolSanctumStatueTilesA+0xd7b
	.else
	.byte 0x86	@ PACKER: left in the buffer by Tileset_SolSanctumStatueTilesA+0xd7b
	.endif
	.endif
	.endif
	.global Tileset_SolSanctumStatueTilesC
Tileset_SolSanctumStatueTilesC:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_statue/tiles_c.4bpp.mtf"
	.byte 0x1c, 0x40	@ PACKER: left in the buffer by Tileset_SolSanctumStatueTilesB+0xade
	.balign 4, 0
