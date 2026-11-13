	.global Field_Colosseum
Field_Colosseum:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Colosseum, .LField_Colosseum_CELL_PICTURES
	map_offset Field_Colosseum, .LField_Colosseum_SHAPES
	map_offset Field_Colosseum, .LField_Colosseum_GRID
	map_offset Field_Colosseum, .LField_Colosseum_ANIMATION
	map_offset Field_Colosseum, .LField_Colosseum_BLEND
	map_offset Field_Colosseum, .LField_Colosseum_MARKERS
.LField_Colosseum_CELL_PICTURES:
	.incbin "games/tbs/src/map/colosseum/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Colosseum_SHAPES:
	.incbin "games/tbs/src/map/colosseum/shapes.table.lz"
	.balign 4, 0
.LField_Colosseum_GRID:
	.incbin "games/tbs/src/map/colosseum/map.grid.lz"
	.balign 4, 0
.LField_Colosseum_ANIMATION:
	.incbin "games/tbs/src/map/colosseum/tileset_anims.script.lz"
	.balign 4, 0
.LField_Colosseum_BLEND:
	.incbin "games/tbs/src/map/colosseum/blend.script.lz"
	.balign 4, 0
.LField_Colosseum_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteColosseum
Tileset_PaletteColosseum:
	.incbin "games/tbs/src/graphics/tileset/palettes/colosseum.gbapal.lz"
	.byte 0x9d, 0xe1, 0xfd	@ PACKER: left in the buffer by Field_Colosseum+0x189
	.balign 4, 0
