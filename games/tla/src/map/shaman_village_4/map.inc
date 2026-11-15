	.global Field_ShamanVillage4
Field_ShamanVillage4:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShamanVillage4, .LField_ShamanVillage4_CELL_PICTURES
	map_block Field_ShamanVillage4, .LField_ShamanVillage4_SHAPES
	map_block Field_ShamanVillage4, .LField_ShamanVillage4_GRID
	map_block Field_ShamanVillage4, .LField_ShamanVillage4_SHAPE_GRID
	map_block Field_ShamanVillage4, .LField_ShamanVillage4_ANIMATION
	map_block Field_ShamanVillage4, .LField_ShamanVillage4_BLEND
	map_block Field_ShamanVillage4, .LField_ShamanVillage4_SCENE
.LField_ShamanVillage4_CELL_PICTURES:
	.incbin "games/tla/src/map/shaman_village_4/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_ShamanVillage4_SHAPES:
	.incbin "games/tla/src/map/shaman_village_4/shapes.table.lz"
	.balign 4, 0
.LField_ShamanVillage4_GRID:
	.incbin "games/tla/src/map/shaman_village_4/map.grid.lz"
	.balign 4, 0
.LField_ShamanVillage4_SHAPE_GRID:
	.incbin "games/tla/src/map/shaman_village_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShamanVillage4_ANIMATION:
	.incbin "games/tla/src/map/shaman_village_4/animation.script.lz"
	.balign 4, 0
.LField_ShamanVillage4_BLEND:
	.incbin "games/tla/src/map/daila_interiors_1/blend.script.lz"
	.balign 4, 0
.LField_ShamanVillage4_SCENE:
	.incbin "games/tla/src/map/shaman_village_4/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ShamanVillage4Palette
Tileset_ShamanVillage4Palette:
	.incbin "games/tla/src/graphics/tileset/shaman_village_4/palette.gbapal.lz"
	.byte 0xf7, 0x99, 0x1a	@ PACKER: left in the buffer by Field_ShamanVillage4+0x1d9
	.global Tileset_ShamanVillage4TilesA
Tileset_ShamanVillage4TilesA:
	.incbin "games/tla/src/graphics/tileset/shaman_village_4/tiles_a.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Field_ShamanVillage4+0x1b33
	.global Tileset_ShamanVillage4TilesB
Tileset_ShamanVillage4TilesB:
	.incbin "games/tla/src/graphics/tileset/shaman_village_4/tiles_b.4bpp.mtf"
	.global Tileset_ShamanVillage4TilesC
Tileset_ShamanVillage4TilesC:
	.incbin "games/tla/src/graphics/tileset/shaman_village_4/tiles_c.4bpp.mtf"
	.byte 0x3a, 0x6c, 0xb7	@ PACKER: left in the buffer by Tileset_ShamanVillage4TilesA+0x1a01
	.global Tileset_ShamanVillage4TilesD
Tileset_ShamanVillage4TilesD:
	.incbin "games/tla/src/graphics/tileset/shaman_village_4/tiles_d.4bpp.mtf"
	.byte 0x84, 0xb0, 0xa3	@ PACKER: left in the buffer by Tileset_ShamanVillage4TilesC+0x11b9
	.global Tileset_PaletteShamanVillage4A
Tileset_PaletteShamanVillage4A:
	.incbin "games/tla/src/graphics/tileset/palettes/shaman_village_4_a.gbapal.lz"
	.byte 0xce, 0xe0, 0x07	@ PACKER: left in the buffer by Tileset_ShamanVillage4TilesD+0x1e5
	.global Tileset_PaletteShamanVillage4B
Tileset_PaletteShamanVillage4B:
	.incbin "games/tla/src/graphics/tileset/palettes/shaman_village_4_b.gbapal.lz"
	.byte 0xf2	@ PACKER: left in the buffer by Tileset_PaletteShamanVillage4A+0x18f
