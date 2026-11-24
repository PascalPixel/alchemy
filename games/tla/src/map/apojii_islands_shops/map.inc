	.global Field_ApojiiIslandsShops
Field_ApojiiIslandsShops:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ApojiiIslandsShops, .LField_ApojiiIslandsShops_CELL_TILES
	map_block Field_ApojiiIslandsShops, .LField_ApojiiIslandsShops_SHAPES
	map_block Field_ApojiiIslandsShops, .LField_ApojiiIslandsShops_GRID
	map_block Field_ApojiiIslandsShops, .LField_ApojiiIslandsShops_SHAPE_GRID
	map_block Field_ApojiiIslandsShops, .LField_ApojiiIslandsShops_ANIMATION
	map_block Field_ApojiiIslandsShops, .LField_ApojiiIslandsShops_BLEND
	map_block Field_ApojiiIslandsShops, .LField_ApojiiIslandsShops_SCENE
.LField_ApojiiIslandsShops_CELL_TILES:
	.incbin "games/tla/src/map/apojii_islands_shops/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ApojiiIslandsShops_SHAPES:
	.incbin "games/tla/src/map/apojii_islands_shops/shapes.table.lz"
	.balign 4, 0
.LField_ApojiiIslandsShops_GRID:
	.incbin "games/tla/src/map/apojii_islands_shops/map.grid.lz"
	.balign 4, 0
.LField_ApojiiIslandsShops_SHAPE_GRID:
	.incbin "games/tla/src/map/apojii_islands_shops/shape_grid.plane.lz"
	.balign 4, 0
.LField_ApojiiIslandsShops_ANIMATION:
	.incbin "games/tla/src/map/yallam_rooms/animation.script.lz"
	.balign 4, 0
.LField_ApojiiIslandsShops_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_ApojiiIslandsShops_SCENE:
	.incbin "games/tla/src/map/apojii_islands_shops/scene.scene.lz"
	.global Tileset_ApojiiIslandsShopsPalette
Tileset_ApojiiIslandsShopsPalette:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_shops/palette.gbapal.lz"
	.byte 0x17, 0x03, 0xcc	@ PACKER: left in the buffer by Field_ApojiiIslandsShops+0x1c5
	.global Tileset_ApojiiIslandsShopsTilesA
Tileset_ApojiiIslandsShopsTilesA:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_shops/tiles_a.4bpp.mtf"
	.byte 0x3e, 0xe0, 0xde	@ PACKER: left in the buffer by Field_ApojiiIslandsShops+0x1989
	.global Tileset_ApojiiIslandsShopsTilesB
Tileset_ApojiiIslandsShopsTilesB:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_shops/tiles_b.4bpp.mtf"
	.byte 0xa8, 0x53	@ PACKER: left in the buffer by Tileset_ApojiiIslandsShopsTilesA+0x112a
	.global Tileset_ApojiiIslandsShopsTilesC
Tileset_ApojiiIslandsShopsTilesC:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_shops/tiles_c.4bpp.mtf"
	.byte 0x72, 0x1c	@ PACKER: left in the buffer by Tileset_ApojiiIslandsShopsTilesB+0xbae
	.global Tileset_ApojiiIslandsShopsTilesD
Tileset_ApojiiIslandsShopsTilesD:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_shops/tiles_d.4bpp.mtf"
