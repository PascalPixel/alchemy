	.global Field_GarohHypnosSwordChest
Field_GarohHypnosSwordChest:
	map_header 0, 0, 128, 128, 2, 3, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 11, 14, 0, 0, 0x1f, 0x1f
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GarohHypnosSwordChest, .LField_GarohHypnosSwordChest_CELL_PICTURES
	map_block Field_GarohHypnosSwordChest, .LField_GarohHypnosSwordChest_SHAPES
	map_block Field_GarohHypnosSwordChest, .LField_GarohHypnosSwordChest_GRID
	map_block Field_GarohHypnosSwordChest, .LField_GarohHypnosSwordChest_SHAPE_GRID
	map_block Field_GarohHypnosSwordChest, .LField_GarohHypnosSwordChest_ANIMATION
	map_block Field_GarohHypnosSwordChest, .LField_GarohHypnosSwordChest_BLEND
	map_block Field_GarohHypnosSwordChest, .LField_GarohHypnosSwordChest_SCENE
.LField_GarohHypnosSwordChest_CELL_PICTURES:
	.incbin "games/tla/src/map/garoh_hypnos_sword_chest/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_GarohHypnosSwordChest_SHAPES:
	.incbin "games/tla/src/map/garoh_hypnos_sword_chest/shapes.table.lz"
	.balign 4, 0
.LField_GarohHypnosSwordChest_GRID:
	.incbin "games/tla/src/map/garoh_hypnos_sword_chest/map.grid.lz"
	.balign 4, 0
.LField_GarohHypnosSwordChest_SHAPE_GRID:
	.incbin "games/tla/src/map/garoh_hypnos_sword_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_GarohHypnosSwordChest_ANIMATION:
	.incbin "games/tla/src/map/garoh_hypnos_sword_chest/animation.script.lz"
	.balign 4, 0
.LField_GarohHypnosSwordChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GarohHypnosSwordChest_SCENE:
	.incbin "games/tla/src/map/garoh_hypnos_sword_chest/scene.scene.lz"
	.global Tileset_PaletteGarohHypnosSwordChest
Tileset_PaletteGarohHypnosSwordChest:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh_hypnos_sword_chest.gbapal.lz"
	.byte 0x98, 0xc4	@ PACKER: left in the buffer by Field_Garoh2+0x182
