	.global Field_ShamanVillage3
Field_ShamanVillage3:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShamanVillage3, .LField_ShamanVillage3_CELL_PICTURES
	map_block Field_ShamanVillage3, .LField_ShamanVillage3_SHAPES
	map_block Field_ShamanVillage3, .LField_ShamanVillage3_GRID
	map_block Field_ShamanVillage3, .LField_ShamanVillage3_SHAPE_GRID
	map_block Field_ShamanVillage3, .LField_ShamanVillage3_ANIMATION
	map_block Field_ShamanVillage3, .LField_ShamanVillage3_BLEND
	map_block Field_ShamanVillage3, .LField_ShamanVillage3_SCENE
.LField_ShamanVillage3_CELL_PICTURES:
	.incbin "games/tla/src/map/shaman_village_3/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_ShamanVillage3_SHAPES:
	.incbin "games/tla/src/map/shaman_village_3/shapes.table.lz"
	.balign 4, 0
.LField_ShamanVillage3_GRID:
	.incbin "games/tla/src/map/shaman_village_3/map.grid.lz"
	.balign 4, 0
.LField_ShamanVillage3_SHAPE_GRID:
	.incbin "games/tla/src/map/shaman_village_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShamanVillage3_ANIMATION:
	.incbin "games/tla/src/map/shaman_village_2/animation.script.lz"
	.balign 4, 0
.LField_ShamanVillage3_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShamanVillage3_SCENE:
	.incbin "games/tla/src/map/shaman_village_3/scene.scene.lz"
	.global Tileset_PaletteShamanVillage3
Tileset_PaletteShamanVillage3:
	.incbin "games/tla/src/graphics/tileset/palettes/shaman_village_3.gbapal.lz"
	.byte 0x27, 0xef	@ PACKER: left in the buffer by Field_ShamanVillage3+0x152
