	.global Field_ShamanVillage2
Field_ShamanVillage2:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShamanVillage2, .LField_ShamanVillage2_CELL_PICTURES
	map_block Field_ShamanVillage2, .LField_ShamanVillage2_SHAPES
	map_block Field_ShamanVillage2, .LField_ShamanVillage2_GRID
	map_block Field_ShamanVillage2, .LField_ShamanVillage2_SHAPE_GRID
	map_block Field_ShamanVillage2, .LField_ShamanVillage2_ANIMATION
	map_block Field_ShamanVillage2, .LField_ShamanVillage2_BLEND
	map_block Field_ShamanVillage2, .LField_ShamanVillage2_SCENE
.LField_ShamanVillage2_CELL_PICTURES:
	.incbin "games/tla/src/map/shaman_village_2/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_ShamanVillage2_SHAPES:
	.incbin "games/tla/src/map/shaman_village_2/shapes.table.lz"
	.balign 4, 0
.LField_ShamanVillage2_GRID:
	.incbin "games/tla/src/map/shaman_village_2/map.grid.lz"
	.balign 4, 0
.LField_ShamanVillage2_SHAPE_GRID:
	.incbin "games/tla/src/map/shaman_village_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShamanVillage2_ANIMATION:
	.incbin "games/tla/src/map/shaman_village_2/animation.script.lz"
	.balign 4, 0
.LField_ShamanVillage2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShamanVillage2_SCENE:
	.incbin "games/tla/src/map/shaman_village_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ShamanVillage2Palette
Tileset_ShamanVillage2Palette:
	.incbin "games/tla/src/graphics/tileset/shaman_village_2/palette.gbapal.lz"
	.byte 0xd1, 0x0c	@ PACKER: left in the buffer by Field_ShamanVillage2+0x172
	.global Tileset_ShamanVillage2TilesA
Tileset_ShamanVillage2TilesA:
	.incbin "games/tla/src/graphics/tileset/shaman_village_2/tiles_a.4bpp.mtf"
	.byte 0xf6, 0xf1	@ PACKER: left in the buffer by Field_Contigo2+0x2466
	.global Tileset_ShamanVillage2TilesB
Tileset_ShamanVillage2TilesB:
	.incbin "games/tla/src/graphics/tileset/shaman_village_2/tiles_b.4bpp.mtf"
	.byte 0xf3, 0x7d	@ PACKER: left in the buffer by Tileset_ShamanVillage2TilesA+0x199a
	.global Tileset_ShamanVillage2TilesC
Tileset_ShamanVillage2TilesC:
	.incbin "games/tla/src/graphics/tileset/shaman_village_2/tiles_c.4bpp.mtf"
	.byte 0x28	@ PACKER: left in the buffer by Tileset_ShamanVillage2TilesA+0x1e33
	.global Tileset_ShamanVillage2TilesD
Tileset_ShamanVillage2TilesD:
	.incbin "games/tla/src/graphics/tileset/shaman_village_2/tiles_d.4bpp.mtf"
	.byte 0x05, 0xe0	@ PACKER: left in the buffer by Tileset_ShamanVillage2TilesC+0xab6
