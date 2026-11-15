	.global Field_ShamanVillageInn
Field_ShamanVillageInn:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShamanVillageInn, .LField_ShamanVillageInn_CELL_PICTURES
	map_block Field_ShamanVillageInn, .LField_ShamanVillageInn_SHAPES
	map_block Field_ShamanVillageInn, .LField_ShamanVillageInn_GRID
	map_block Field_ShamanVillageInn, .LField_ShamanVillageInn_SHAPE_GRID
	map_block Field_ShamanVillageInn, .LField_ShamanVillageInn_ANIMATION
	map_block Field_ShamanVillageInn, .LField_ShamanVillageInn_BLEND
	map_block Field_ShamanVillageInn, .LField_ShamanVillageInn_SCENE
.LField_ShamanVillageInn_CELL_PICTURES:
	.incbin "games/tla/src/map/shaman_village_inn/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_ShamanVillageInn_SHAPES:
	.incbin "games/tla/src/map/shaman_village_inn/shapes.table.lz"
	.balign 4, 0
.LField_ShamanVillageInn_GRID:
	.incbin "games/tla/src/map/shaman_village_inn/map.grid.lz"
	.balign 4, 0
.LField_ShamanVillageInn_SHAPE_GRID:
	.incbin "games/tla/src/map/shaman_village_inn/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShamanVillageInn_ANIMATION:
	.incbin "games/tla/src/map/shaman_village_inn/animation.script.lz"
	.balign 4, 0
.LField_ShamanVillageInn_BLEND:
	.incbin "games/tla/src/map/daila_interiors_houses/blend.script.lz"
	.balign 4, 0
.LField_ShamanVillageInn_SCENE:
	.incbin "games/tla/src/map/shaman_village_inn/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ShamanVillageInnPalette
Tileset_ShamanVillageInnPalette:
	.incbin "games/tla/src/graphics/tileset/shaman_village_inn/palette.gbapal.lz"
	.byte 0xf7, 0x99, 0x1a	@ PACKER: left in the buffer by Field_ShamanVillage4+0x1d9
	.global Tileset_ShamanVillageInnTilesA
Tileset_ShamanVillageInnTilesA:
	.incbin "games/tla/src/graphics/tileset/shaman_village_inn/tiles_a.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Field_ShamanVillage4+0x1b33
	.global Tileset_ShamanVillageInnTilesB
Tileset_ShamanVillageInnTilesB:
	.incbin "games/tla/src/graphics/tileset/shaman_village_inn/tiles_b.4bpp.mtf"
	.global Tileset_ShamanVillageInnTilesC
Tileset_ShamanVillageInnTilesC:
	.incbin "games/tla/src/graphics/tileset/shaman_village_inn/tiles_c.4bpp.mtf"
	.byte 0x3a, 0x6c, 0xb7	@ PACKER: left in the buffer by Tileset_ShamanVillageInnTilesA+0x1a01
	.global Tileset_ShamanVillageInnTilesD
Tileset_ShamanVillageInnTilesD:
	.incbin "games/tla/src/graphics/tileset/shaman_village_inn/tiles_d.4bpp.mtf"
	.byte 0x84, 0xb0, 0xa3	@ PACKER: left in the buffer by Tileset_ShamanVillageInnTilesC+0x11b9
	.global Tileset_PaletteShamanVillageInnA
Tileset_PaletteShamanVillageInnA:
	.incbin "games/tla/src/graphics/tileset/palettes/shaman_village_4_a.gbapal.lz"
	.byte 0xce, 0xe0, 0x07	@ PACKER: left in the buffer by Tileset_ShamanVillageInnTilesD+0x1e5
	.global Tileset_PaletteShamanVillageInnB
Tileset_PaletteShamanVillageInnB:
	.incbin "games/tla/src/graphics/tileset/palettes/shaman_village_4_b.gbapal.lz"
	.byte 0xf2	@ PACKER: left in the buffer by Tileset_PaletteShamanVillageInnA+0x18f
