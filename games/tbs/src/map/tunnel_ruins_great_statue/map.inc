	.global Field_TunnelRuinsGreatStatue
Field_TunnelRuinsGreatStatue:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_TunnelRuinsGreatStatue, .LField_TunnelRuinsGreatStatue_CELL_PICTURES
	map_offset Field_TunnelRuinsGreatStatue, .LField_TunnelRuinsGreatStatue_SHAPES
	map_offset Field_TunnelRuinsGreatStatue, .LField_TunnelRuinsGreatStatue_GRID
	map_offset Field_TunnelRuinsGreatStatue, .LField_TunnelRuinsGreatStatue_ANIMATION
	map_offset Field_TunnelRuinsGreatStatue, .LField_TunnelRuinsGreatStatue_BLEND
	map_offset Field_TunnelRuinsGreatStatue, .LField_TunnelRuinsGreatStatue_MARKERS
.LField_TunnelRuinsGreatStatue_CELL_PICTURES:
	.incbin "games/tbs/src/map/tunnel_ruins_great_statue/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_TunnelRuinsGreatStatue_SHAPES:
	.incbin "games/tbs/src/map/tunnel_ruins_great_statue/shapes.table.lz"
	.balign 4, 0
.LField_TunnelRuinsGreatStatue_GRID:
	.incbin "games/tbs/src/map/tunnel_ruins_great_statue/map.grid.lz"
	.balign 4, 0
.LField_TunnelRuinsGreatStatue_ANIMATION:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_TunnelRuinsGreatStatue_BLEND:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/blend.script.lz"
	.balign 4, 0
.LField_TunnelRuinsGreatStatue_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteTunnelRuins
Tileset_PaletteTunnelRuins:
	.incbin "games/tbs/src/graphics/tileset/palettes/tunnel_ruins.gbapal.lz"
	.byte 0xec, 0x5c	@ PACKER: left in the buffer by Field_TunnelRuinsGreatStatue+0x17e
	.balign 4, 0
