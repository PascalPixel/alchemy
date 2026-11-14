	.global Field_KolimaBridge
Field_KolimaBridge:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KolimaBridge, .LField_KolimaBridge_CELL_PICTURES
	map_offset Field_KolimaBridge, .LField_KolimaBridge_SHAPES
	map_offset Field_KolimaBridge, .LField_KolimaBridge_GRID
	map_offset Field_KolimaBridge, .LField_KolimaBridge_ANIMATION
	map_offset Field_KolimaBridge, .LField_KolimaBridge_BLEND
	map_offset Field_KolimaBridge, .LField_KolimaBridge_MARKERS
.LField_KolimaBridge_CELL_PICTURES:
	.incbin "games/tbs/src/map/kolima_bridge/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_KolimaBridge_SHAPES:
	.incbin "games/tbs/src/map/kolima_bridge/shapes.table.lz"
	.balign 4, 0
.LField_KolimaBridge_GRID:
	.incbin "games/tbs/src/map/kolima_bridge/map_es.grid.lz"
	.balign 4, 0
.LField_KolimaBridge_ANIMATION:
	.incbin "games/tbs/src/map/kolima_bridge/tileset_anims.script.lz"
	.balign 4, 0
.LField_KolimaBridge_BLEND:
	.incbin "games/tbs/src/map/kolima_bridge/blend.script.lz"
	.balign 4, 0
.LField_KolimaBridge_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KolimaBridge, .LField_KolimaBridge_CELL_PICTURES
	map_offset Field_KolimaBridge, .LField_KolimaBridge_SHAPES
	map_offset Field_KolimaBridge, .LField_KolimaBridge_GRID
	map_offset Field_KolimaBridge, .LField_KolimaBridge_ANIMATION
	map_offset Field_KolimaBridge, .LField_KolimaBridge_BLEND
	map_offset Field_KolimaBridge, .LField_KolimaBridge_MARKERS
.LField_KolimaBridge_CELL_PICTURES:
	.incbin "games/tbs/src/map/kolima_bridge/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_KolimaBridge_SHAPES:
	.incbin "games/tbs/src/map/kolima_bridge/shapes.table.lz"
	.balign 4, 0
.LField_KolimaBridge_GRID:
	.incbin "games/tbs/src/map/kolima_bridge/map_es.grid.lz"
	.balign 4, 0
.LField_KolimaBridge_ANIMATION:
	.incbin "games/tbs/src/map/kolima_bridge/tileset_anims.script.lz"
	.balign 4, 0
.LField_KolimaBridge_BLEND:
	.incbin "games/tbs/src/map/kolima_bridge/blend.script.lz"
	.balign 4, 0
.LField_KolimaBridge_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KolimaBridge, .LField_KolimaBridge_CELL_PICTURES
	map_offset Field_KolimaBridge, .LField_KolimaBridge_SHAPES
	map_offset Field_KolimaBridge, .LField_KolimaBridge_GRID
	map_offset Field_KolimaBridge, .LField_KolimaBridge_ANIMATION
	map_offset Field_KolimaBridge, .LField_KolimaBridge_BLEND
	map_offset Field_KolimaBridge, .LField_KolimaBridge_MARKERS
.LField_KolimaBridge_CELL_PICTURES:
	.incbin "games/tbs/src/map/kolima_bridge/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_KolimaBridge_SHAPES:
	.incbin "games/tbs/src/map/kolima_bridge/shapes.table.lz"
	.balign 4, 0
.LField_KolimaBridge_GRID:
	.incbin "games/tbs/src/map/kolima_bridge/map_es.grid.lz"
	.balign 4, 0
.LField_KolimaBridge_ANIMATION:
	.incbin "games/tbs/src/map/kolima_bridge/tileset_anims.script.lz"
	.balign 4, 0
.LField_KolimaBridge_BLEND:
	.incbin "games/tbs/src/map/kolima_bridge/blend.script.lz"
	.balign 4, 0
.LField_KolimaBridge_MARKERS:
	treasure_spots_end
	.else
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KolimaBridge, .LField_KolimaBridge_CELL_PICTURES
	map_offset Field_KolimaBridge, .LField_KolimaBridge_SHAPES
	map_offset Field_KolimaBridge, .LField_KolimaBridge_GRID
	map_offset Field_KolimaBridge, .LField_KolimaBridge_ANIMATION
	map_offset Field_KolimaBridge, .LField_KolimaBridge_BLEND
	map_offset Field_KolimaBridge, .LField_KolimaBridge_MARKERS
.LField_KolimaBridge_CELL_PICTURES:
	.incbin "games/tbs/src/map/kolima_bridge/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_KolimaBridge_SHAPES:
	.incbin "games/tbs/src/map/kolima_bridge/shapes.table.lz"
	.balign 4, 0
.LField_KolimaBridge_GRID:
	.incbin "games/tbs/src/map/kolima_bridge/map.grid.lz"
	.balign 4, 0
.LField_KolimaBridge_ANIMATION:
	.incbin "games/tbs/src/map/kolima_bridge/tileset_anims.script.lz"
	.balign 4, 0
.LField_KolimaBridge_BLEND:
	.incbin "games/tbs/src/map/kolima_bridge/blend.script.lz"
	.balign 4, 0
.LField_KolimaBridge_MARKERS:
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_KolimaBridgePalette
Tileset_KolimaBridgePalette:
	.incbin "games/tbs/src/graphics/tileset/kolima_bridge/palette.gbapal.lz"
	.byte 0x9e, 0x99, 0xb7	@ PACKER: left in the buffer by Field_KolimaBridge+0x17d
	.global Tileset_KolimaBridgeTilesA
Tileset_KolimaBridgeTilesA:
	.incbin "games/tbs/src/graphics/tileset/kolima_bridge/tiles_a.4bpp.mtf"
	.global Tileset_KolimaBridgeTilesB
Tileset_KolimaBridgeTilesB:
	.incbin "games/tbs/src/graphics/tileset/kolima_bridge/tiles_b.4bpp.mtf"
	.byte 0x4d, 0x0c, 0x83	@ PACKER: left in the buffer by Tileset_KolimaBridgeTilesA+0x1d6d
	.global Tileset_KolimaBridgeTilesC
Tileset_KolimaBridgeTilesC:
	.incbin "games/tbs/src/graphics/tileset/kolima_bridge/tiles_c.4bpp.mtf"
	.byte 0xfa, 0x80	@ PACKER: left in the buffer by Tileset_KolimaBridgeTilesB+0x1042
	.global Tileset_KolimaBridgeTilesD
Tileset_KolimaBridgeTilesD:
	.incbin "games/tbs/src/graphics/tileset/kolima_bridge/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_KolimaBridgeTilesC+0xe22
	.balign 4, 0
