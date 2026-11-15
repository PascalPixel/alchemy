	.global Field_ShamanVillageAroma
Field_ShamanVillageAroma:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShamanVillageAroma, .LField_ShamanVillageAroma_CELL_PICTURES
	map_block Field_ShamanVillageAroma, .LField_ShamanVillageAroma_SHAPES
	map_block Field_ShamanVillageAroma, .LField_ShamanVillageAroma_GRID
	map_block Field_ShamanVillageAroma, .LField_ShamanVillageAroma_SHAPE_GRID
	map_block Field_ShamanVillageAroma, .LField_ShamanVillageAroma_ANIMATION
	map_block Field_ShamanVillageAroma, .LField_ShamanVillageAroma_BLEND
	map_block Field_ShamanVillageAroma, .LField_ShamanVillageAroma_SCENE
.LField_ShamanVillageAroma_CELL_PICTURES:
	.incbin "games/tla/src/map/shaman_village_aroma/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_ShamanVillageAroma_SHAPES:
	.incbin "games/tla/src/map/shaman_village_aroma/shapes.table.lz"
	.balign 4, 0
.LField_ShamanVillageAroma_GRID:
	.incbin "games/tla/src/map/shaman_village_aroma/map.grid.lz"
	.balign 4, 0
.LField_ShamanVillageAroma_SHAPE_GRID:
	.incbin "games/tla/src/map/shaman_village_aroma/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShamanVillageAroma_ANIMATION:
	.incbin "games/tla/src/map/shaman_village_2/animation.script.lz"
	.balign 4, 0
.LField_ShamanVillageAroma_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShamanVillageAroma_SCENE:
	.incbin "games/tla/src/map/shaman_village_aroma/scene.scene.lz"
	.global Tileset_PaletteShamanVillageAroma
Tileset_PaletteShamanVillageAroma:
	.incbin "games/tla/src/graphics/tileset/palettes/shaman_village_aroma.gbapal.lz"
	.byte 0x27, 0xef	@ PACKER: left in the buffer by Field_ShamanVillage3+0x152
