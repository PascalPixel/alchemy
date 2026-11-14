	.global Field_Vale
Field_Vale:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Vale, .LField_Vale_CELL_PICTURES
	map_offset Field_Vale, .LField_Vale_SHAPES
	map_offset Field_Vale, .LField_Vale_GRID
	map_offset Field_Vale, .LField_Vale_ANIMATION
	map_offset Field_Vale
	map_offset Field_Vale, .LField_Vale_MARKERS
.LField_Vale_CELL_PICTURES:
	.incbin "games/tbs/src/map/vale/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Vale_SHAPES:
	.incbin "games/tbs/src/map/vale/shapes.table.lz"
	.balign 4, 0
.LField_Vale_GRID:
	.incbin "games/tbs/src/map/vale/map_es.grid.lz"
	.balign 4, 0
.LField_Vale_ANIMATION:
	.incbin "games/tbs/src/map/vale/tileset_anims.script.lz"
	.balign 4, 0
.LField_Vale_MARKERS:
	treasure_spot 24, 55, 100
	treasure_spot 53, 56, 101
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Vale, .LField_Vale_CELL_PICTURES
	map_offset Field_Vale, .LField_Vale_SHAPES
	map_offset Field_Vale, .LField_Vale_GRID
	map_offset Field_Vale, .LField_Vale_ANIMATION
	map_offset Field_Vale
	map_offset Field_Vale, .LField_Vale_MARKERS
.LField_Vale_CELL_PICTURES:
	.incbin "games/tbs/src/map/vale/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Vale_SHAPES:
	.incbin "games/tbs/src/map/vale/shapes.table.lz"
	.balign 4, 0
.LField_Vale_GRID:
	.incbin "games/tbs/src/map/vale/map_es.grid.lz"
	.balign 4, 0
.LField_Vale_ANIMATION:
	.incbin "games/tbs/src/map/vale/tileset_anims.script.lz"
	.balign 4, 0
.LField_Vale_MARKERS:
	treasure_spot 24, 55, 100
	treasure_spot 53, 56, 101
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Vale, .LField_Vale_CELL_PICTURES
	map_offset Field_Vale, .LField_Vale_SHAPES
	map_offset Field_Vale, .LField_Vale_GRID
	map_offset Field_Vale, .LField_Vale_ANIMATION
	map_offset Field_Vale
	map_offset Field_Vale, .LField_Vale_MARKERS
.LField_Vale_CELL_PICTURES:
	.incbin "games/tbs/src/map/vale/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Vale_SHAPES:
	.incbin "games/tbs/src/map/vale/shapes.table.lz"
	.balign 4, 0
.LField_Vale_GRID:
	.incbin "games/tbs/src/map/vale/map_it.grid.lz"
	.balign 4, 0
.LField_Vale_ANIMATION:
	.incbin "games/tbs/src/map/vale/tileset_anims.script.lz"
	.balign 4, 0
.LField_Vale_MARKERS:
	treasure_spot 24, 55, 100
	treasure_spot 53, 56, 101
	treasure_spots_end
	.else
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Vale, .LField_Vale_CELL_PICTURES
	map_offset Field_Vale, .LField_Vale_SHAPES
	map_offset Field_Vale, .LField_Vale_GRID
	map_offset Field_Vale, .LField_Vale_ANIMATION
	map_offset Field_Vale
	map_offset Field_Vale, .LField_Vale_MARKERS
.LField_Vale_CELL_PICTURES:
	.incbin "games/tbs/src/map/vale/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Vale_SHAPES:
	.incbin "games/tbs/src/map/vale/shapes.table.lz"
	.balign 4, 0
.LField_Vale_GRID:
	.incbin "games/tbs/src/map/vale/map.grid.lz"
	.balign 4, 0
.LField_Vale_ANIMATION:
	.incbin "games/tbs/src/map/vale/tileset_anims.script.lz"
	.balign 4, 0
.LField_Vale_MARKERS:
	treasure_spot 24, 55, 100
	treasure_spot 53, 56, 101
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_ValePalette
Tileset_ValePalette:
	.incbin "games/tbs/src/graphics/tileset/vale/palette.gbapal.lz"
	.byte 0xb3	@ PACKER: left in the buffer by Field_Vale+0x193
	.global Tileset_ValeTilesA
Tileset_ValeTilesA:
	.incbin "games/tbs/src/graphics/tileset/vale/tiles_a.4bpp.mtf"
	.byte 0x97	@ PACKER: left in the buffer by Field_Vale+0x2c53
	.global Tileset_ValeTilesB
Tileset_ValeTilesB:
	.incbin "games/tbs/src/graphics/tileset/vale/tiles_b.4bpp.mtf"
	.global Tileset_ValeTilesC
Tileset_ValeTilesC:
	.incbin "games/tbs/src/graphics/tileset/vale/tiles_c.4bpp.mtf"
	.global Tileset_ValeTilesD
Tileset_ValeTilesD:
	.incbin "games/tbs/src/graphics/tileset/vale/tiles_d.4bpp.mtf"
	.byte 0x8f	@ PACKER: left in the buffer by Tileset_ValeTilesC+0x1ecb
	.balign 4, 0
