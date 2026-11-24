	.global Field_ShamanVillageNorthEnd
Field_ShamanVillageNorthEnd:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShamanVillageNorthEnd, .LField_ShamanVillageNorthEnd_CELL_TILES
	map_block Field_ShamanVillageNorthEnd, .LField_ShamanVillageNorthEnd_SHAPES
	map_block Field_ShamanVillageNorthEnd, .LField_ShamanVillageNorthEnd_GRID
	map_block Field_ShamanVillageNorthEnd, .LField_ShamanVillageNorthEnd_SHAPE_GRID
	map_block Field_ShamanVillageNorthEnd, .LField_ShamanVillageNorthEnd_ANIMATION
	map_block Field_ShamanVillageNorthEnd, .LField_ShamanVillageNorthEnd_BLEND
	map_block Field_ShamanVillageNorthEnd, .LField_ShamanVillageNorthEnd_SCENE
.LField_ShamanVillageNorthEnd_CELL_TILES:
	.incbin "games/tla/src/map/shaman_village_north_end/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_ShamanVillageNorthEnd_SHAPES:
	.incbin "games/tla/src/map/shaman_village_north_end/shapes.table.lz"
	.balign 4, 0
.LField_ShamanVillageNorthEnd_GRID:
	.incbin "games/tla/src/map/shaman_village_north_end/map.grid.lz"
	.balign 4, 0
.LField_ShamanVillageNorthEnd_SHAPE_GRID:
	.incbin "games/tla/src/map/shaman_village_north_end/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShamanVillageNorthEnd_ANIMATION:
	.incbin "games/tla/src/map/shaman_village/animation.script.lz"
	.balign 4, 0
.LField_ShamanVillageNorthEnd_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShamanVillageNorthEnd_SCENE:
	.incbin "games/tla/src/map/shaman_village_north_end/scene.scene.lz"
	.global Tileset_PaletteShamanVillageNorthEnd
Tileset_PaletteShamanVillageNorthEnd:
	.incbin "games/tla/src/graphics/tileset/palettes/shaman_village_north_end.gbapal.lz"
	.byte 0x27, 0xef	@ PACKER: left in the buffer by Field_ShamanVillage3+0x152
