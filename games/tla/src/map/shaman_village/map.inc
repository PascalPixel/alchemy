	.global Field_ShamanVillage
Field_ShamanVillage:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShamanVillage, .LField_ShamanVillage_CELL_TILES
	map_block Field_ShamanVillage, .LField_ShamanVillage_SHAPES
	map_block Field_ShamanVillage, .LField_ShamanVillage_GRID
	map_block Field_ShamanVillage, .LField_ShamanVillage_SHAPE_GRID
	map_block Field_ShamanVillage, .LField_ShamanVillage_ANIMATION
	map_block Field_ShamanVillage, .LField_ShamanVillage_BLEND
	map_block Field_ShamanVillage, .LField_ShamanVillage_SCENE
.LField_ShamanVillage_CELL_TILES:
	.incbin "games/tla/src/map/shaman_village/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_ShamanVillage_SHAPES:
	.incbin "games/tla/src/map/shaman_village/shapes.table.lz"
	.balign 4, 0
.LField_ShamanVillage_GRID:
	.incbin "games/tla/src/map/shaman_village/map.grid.lz"
	.balign 4, 0
.LField_ShamanVillage_SHAPE_GRID:
	.incbin "games/tla/src/map/shaman_village/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShamanVillage_ANIMATION:
	.incbin "games/tla/src/map/shaman_village/animation.script.lz"
	.balign 4, 0
.LField_ShamanVillage_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShamanVillage_SCENE:
	.incbin "games/tla/src/map/shaman_village/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ShamanVillagePalette
Tileset_ShamanVillagePalette:
	.incbin "games/tla/src/graphics/tileset/shaman_village/palette.gbapal.lz"
	.byte 0xd1, 0x0c	@ PACKER: left in the buffer by Field_ShamanVillage2+0x172
	.global Tileset_ShamanVillageTilesA
Tileset_ShamanVillageTilesA:
	.incbin "games/tla/src/graphics/tileset/shaman_village/tiles_a.4bpp.mtf"
	.byte 0xf6, 0xf1	@ PACKER: left in the buffer by Field_Contigo2+0x2466
	.global Tileset_ShamanVillageTilesB
Tileset_ShamanVillageTilesB:
	.incbin "games/tla/src/graphics/tileset/shaman_village/tiles_b.4bpp.mtf"
	.byte 0xf3, 0x7d	@ PACKER: left in the buffer by Tileset_ShamanVillageTilesA+0x199a
	.global Tileset_ShamanVillageTilesC
Tileset_ShamanVillageTilesC:
	.incbin "games/tla/src/graphics/tileset/shaman_village/tiles_c.4bpp.mtf"
	.byte 0x28	@ PACKER: left in the buffer by Tileset_ShamanVillageTilesA+0x1e33
	.global Tileset_ShamanVillageTilesD
Tileset_ShamanVillageTilesD:
	.incbin "games/tla/src/graphics/tileset/shaman_village/tiles_d.4bpp.mtf"
	.byte 0x05, 0xe0	@ PACKER: left in the buffer by Tileset_ShamanVillageTilesC+0xab6
