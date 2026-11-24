	.global Field_ContigoShops
Field_ContigoShops:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ContigoShops, .LField_ContigoShops_CELL_TILES
	map_block Field_ContigoShops, .LField_ContigoShops_SHAPES
	map_block Field_ContigoShops, .LField_ContigoShops_GRID
	map_block Field_ContigoShops, .LField_ContigoShops_SHAPE_GRID
	map_block Field_ContigoShops, .LField_ContigoShops_ANIMATION
	map_block Field_ContigoShops, .LField_ContigoShops_BLEND
	map_block Field_ContigoShops, .LField_ContigoShops_SCENE
.LField_ContigoShops_CELL_TILES:
	.incbin "games/tla/src/map/contigo_shops/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ContigoShops_SHAPES:
	.incbin "games/tla/src/map/contigo_shops/shapes.table.lz"
	.balign 4, 0
.LField_ContigoShops_GRID:
	.incbin "games/tla/src/map/contigo_shops/map.grid.lz"
	.balign 4, 0
.LField_ContigoShops_SHAPE_GRID:
	.incbin "games/tla/src/map/contigo_shops/shape_grid.plane.lz"
	.balign 4, 0
.LField_ContigoShops_ANIMATION:
	.incbin "games/tla/src/map/contigo_shops/animation.script.lz"
	.balign 4, 0
.LField_ContigoShops_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_ContigoShops_SCENE:
	.incbin "games/tla/src/map/contigo_shops/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ContigoShopsPalette
Tileset_ContigoShopsPalette:
	.incbin "games/tla/src/graphics/tileset/contigo_shops/palette.gbapal.lz"
	.byte 0x80, 0x07, 0x98	@ PACKER: left in the buffer by Field_Contigo2+0x1d5
	.global Tileset_ContigoShopsTilesA
Tileset_ContigoShopsTilesA:
	.incbin "games/tla/src/graphics/tileset/contigo_shops/tiles_a.4bpp.mtf"
	.global Tileset_ContigoShopsTilesB
Tileset_ContigoShopsTilesB:
	.incbin "games/tla/src/graphics/tileset/contigo_shops/tiles_b.4bpp.mtf"
	.byte 0x5b, 0x44, 0x03	@ PACKER: left in the buffer by Tileset_ContigoShopsTilesA+0x1679
	.global Tileset_ContigoShopsTilesC
Tileset_ContigoShopsTilesC:
	.incbin "games/tla/src/graphics/tileset/contigo_shops/tiles_c.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x1d, 0xff, 0xf3	@ PACKER: left in the buffer by Tileset_ContigoShopsTilesA+0x1751
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.global Tileset_ContigoShopsTilesD
Tileset_ContigoShopsTilesD:
	.incbin "games/tla/src/graphics/tileset/contigo_shops/tiles_d.4bpp.mtf"
	.global Tileset_PaletteContigoShops
Tileset_PaletteContigoShops:
	.incbin "games/tla/src/graphics/tileset/palettes/contigo_shops.gbapal.lz"
	.byte 0xa7	@ PACKER: left in the buffer by Tileset_ContigoShopsTilesD+0x1cb
