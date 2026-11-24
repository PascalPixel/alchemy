	.global Field_ShamanVillageHouses
Field_ShamanVillageHouses:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShamanVillageHouses, .LField_ShamanVillageHouses_CELL_TILES
	map_block Field_ShamanVillageHouses, .LField_ShamanVillageHouses_SHAPES
	map_block Field_ShamanVillageHouses, .LField_ShamanVillageHouses_GRID
	map_block Field_ShamanVillageHouses, .LField_ShamanVillageHouses_SHAPE_GRID
	map_block Field_ShamanVillageHouses, .LField_ShamanVillageHouses_ANIMATION
	map_block Field_ShamanVillageHouses, .LField_ShamanVillageHouses_BLEND
	map_block Field_ShamanVillageHouses, .LField_ShamanVillageHouses_SCENE
.LField_ShamanVillageHouses_CELL_TILES:
	.incbin "games/tla/src/map/shaman_village_houses/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_ShamanVillageHouses_SHAPES:
	.incbin "games/tla/src/map/shaman_village_houses/shapes.table.lz"
	.balign 4, 0
.LField_ShamanVillageHouses_GRID:
	.incbin "games/tla/src/map/shaman_village_houses/map.grid.lz"
	.balign 4, 0
.LField_ShamanVillageHouses_SHAPE_GRID:
	.incbin "games/tla/src/map/shaman_village_houses/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShamanVillageHouses_ANIMATION:
	.incbin "games/tla/src/map/shaman_village_houses/animation.script.lz"
	.balign 4, 0
.LField_ShamanVillageHouses_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_ShamanVillageHouses_SCENE:
	.incbin "games/tla/src/map/shaman_village_houses/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ShamanVillageHousesPalette
Tileset_ShamanVillageHousesPalette:
	.incbin "games/tla/src/graphics/tileset/shaman_village_houses/palette.gbapal.lz"
	.byte 0xf7, 0x99, 0x1a	@ PACKER: left in the buffer by Field_ShamanVillage4+0x1d9
	.global Tileset_ShamanVillageHousesTilesA
Tileset_ShamanVillageHousesTilesA:
	.incbin "games/tla/src/graphics/tileset/shaman_village_houses/tiles_a.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Field_ShamanVillage4+0x1b33
	.global Tileset_ShamanVillageHousesTilesB
Tileset_ShamanVillageHousesTilesB:
	.incbin "games/tla/src/graphics/tileset/shaman_village_houses/tiles_b.4bpp.mtf"
	.global Tileset_ShamanVillageHousesTilesC
Tileset_ShamanVillageHousesTilesC:
	.incbin "games/tla/src/graphics/tileset/shaman_village_houses/tiles_c.4bpp.mtf"
	.byte 0x3a, 0x6c, 0xb7	@ PACKER: left in the buffer by Tileset_ShamanVillageHousesTilesA+0x1a01
	.global Tileset_ShamanVillageHousesTilesD
Tileset_ShamanVillageHousesTilesD:
	.incbin "games/tla/src/graphics/tileset/shaman_village_houses/tiles_d.4bpp.mtf"
	.byte 0x84, 0xb0, 0xa3	@ PACKER: left in the buffer by Tileset_ShamanVillageHousesTilesC+0x11b9
	.global Tileset_PaletteShamanVillageHousesA
Tileset_PaletteShamanVillageHousesA:
	.incbin "games/tla/src/graphics/tileset/palettes/shaman_village_houses_a.gbapal.lz"
	.byte 0xce, 0xe0, 0x07	@ PACKER: left in the buffer by Tileset_ShamanVillageHousesTilesD+0x1e5
	.global Tileset_PaletteShamanVillageHousesB
Tileset_PaletteShamanVillageHousesB:
	.incbin "games/tla/src/graphics/tileset/palettes/shaman_village_houses_b.gbapal.lz"
	.byte 0xf2	@ PACKER: left in the buffer by Tileset_PaletteShamanVillageHousesA+0x18f
