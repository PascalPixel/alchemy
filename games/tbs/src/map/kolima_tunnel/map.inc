	.global Field_KolimaTunnel
Field_KolimaTunnel:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KolimaTunnel, .LField_KolimaTunnel_CELL_PICTURES
	map_offset Field_KolimaTunnel, .LField_KolimaTunnel_SHAPES
	map_offset Field_KolimaTunnel, .LField_KolimaTunnel_GRID
	map_offset Field_KolimaTunnel, .LField_KolimaTunnel_ANIMATION
	map_offset Field_KolimaTunnel, .LField_KolimaTunnel_BLEND
	map_offset Field_KolimaTunnel, .LField_KolimaTunnel_MARKERS
.LField_KolimaTunnel_CELL_PICTURES:
	.incbin "games/tbs/src/map/kolima_tunnel/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_KolimaTunnel_SHAPES:
	.incbin "games/tbs/src/map/kolima_tunnel/shapes.table.lz"
	.balign 4, 0
.LField_KolimaTunnel_GRID:
	.incbin "games/tbs/src/map/kolima_tunnel/map.grid.lz"
	.balign 4, 0
.LField_KolimaTunnel_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_KolimaTunnel_BLEND:
	.incbin "games/tbs/src/map/kolima_tunnel/blend.script.lz"
	.balign 4, 0
.LField_KolimaTunnel_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteKolimaTunnel
Tileset_PaletteKolimaTunnel:
	.incbin "games/tbs/src/graphics/tileset/palettes/kolima_tunnel.gbapal.lz"
	.byte 0x94, 0x01, 0x06	@ PACKER: left in the buffer by Field_KolimaTunnel+0x159
	.balign 4, 0
