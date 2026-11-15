	.global Field_ShamanVillage1
Field_ShamanVillage1:
	map_header 0, 0, 64, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShamanVillage1, .LField_ShamanVillage1_CELL_PICTURES
	map_block Field_ShamanVillage1, .LField_ShamanVillage1_SHAPES
	map_block Field_ShamanVillage1, .LField_ShamanVillage1_GRID
	map_block Field_ShamanVillage1, .LField_ShamanVillage1_SHAPE_GRID
	map_block Field_ShamanVillage1, .LField_ShamanVillage1_ANIMATION
	map_block Field_ShamanVillage1, .LField_ShamanVillage1_BLEND
	map_block Field_ShamanVillage1, .LField_ShamanVillage1_SCENE
.LField_ShamanVillage1_CELL_PICTURES:
	.incbin "games/tla/src/map/shaman_village_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ShamanVillage1_SHAPES:
	.incbin "games/tla/src/map/shaman_village_1/shapes.table.lz"
	.balign 4, 0
.LField_ShamanVillage1_GRID:
	.incbin "games/tla/src/map/shaman_village_1/map.grid.lz"
	.balign 4, 0
.LField_ShamanVillage1_SHAPE_GRID:
	.incbin "games/tla/src/map/shaman_village_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShamanVillage1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_ShamanVillage1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShamanVillage1_SCENE:
	.incbin "games/tla/src/map/shaman_village_1/scene.scene.lz"
	.global Tileset_ShamanVillage1Palette
Tileset_ShamanVillage1Palette:
	.incbin "games/tla/src/graphics/tileset/shaman_village_1/palette.gbapal.lz"
	.byte 0x92, 0x44, 0x80	@ PACKER: left in the buffer by Field_ShamanVillage1+0x135
	.global Tileset_ShamanVillage1TilesA
Tileset_ShamanVillage1TilesA:
	.incbin "games/tla/src/graphics/tileset/shaman_village_1/tiles_a.4bpp.mtf"
	.global Tileset_ShamanVillage1TilesB
Tileset_ShamanVillage1TilesB:
	.incbin "games/tla/src/graphics/tileset/shaman_village_1/tiles_b.4bpp.mtf"
	.byte 0xf2, 0x8b, 0xdd	@ PACKER: left in the buffer by Tileset_ShamanVillage1TilesA+0x1365
	.global Tileset_ShamanVillage1TilesC
Tileset_ShamanVillage1TilesC:
	.incbin "games/tla/src/graphics/tileset/shaman_village_1/tiles_c.4bpp.mtf"
	.byte 0x16	@ PACKER: left in the buffer by Tileset_ShamanVillage1TilesB+0xc87
	.global Tileset_ShamanVillage1TilesD
Tileset_ShamanVillage1TilesD:
	.incbin "games/tla/src/graphics/tileset/shaman_village_1/tiles_d.4bpp.mtf"
	.byte 0x0e	@ PACKER: left in the buffer by Tileset_ShamanVillage1TilesC+0x13f
