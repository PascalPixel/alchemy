	.global Field_GabombaStatueBoneArmletChest
Field_GabombaStatueBoneArmletChest:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueBoneArmletChest, .LField_GabombaStatueBoneArmletChest_CELL_TILES
	map_block Field_GabombaStatueBoneArmletChest, .LField_GabombaStatueBoneArmletChest_SHAPES
	map_block Field_GabombaStatueBoneArmletChest, .LField_GabombaStatueBoneArmletChest_GRID
	map_block Field_GabombaStatueBoneArmletChest, .LField_GabombaStatueBoneArmletChest_SHAPE_GRID
	map_block Field_GabombaStatueBoneArmletChest, .LField_GabombaStatueBoneArmletChest_ANIMATION
	map_block Field_GabombaStatueBoneArmletChest, .LField_GabombaStatueBoneArmletChest_BLEND
	map_block Field_GabombaStatueBoneArmletChest, .LField_GabombaStatueBoneArmletChest_SCENE
.LField_GabombaStatueBoneArmletChest_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_statue_bone_armlet_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GabombaStatueBoneArmletChest_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_bone_armlet_chest/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueBoneArmletChest_GRID:
	.incbin "games/tla/src/map/gabomba_statue_bone_armlet_chest/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueBoneArmletChest_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_bone_armlet_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueBoneArmletChest_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueBoneArmletChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueBoneArmletChest_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_bone_armlet_chest/scene.scene.lz"
	.balign 4, 0
	.global Tileset_GabombaStatueBoneArmletChestPalette
Tileset_GabombaStatueBoneArmletChestPalette:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_bone_armlet_chest/palette.gbapal.lz"
	.byte 0x9f	@ PACKER: left in the buffer by Field_GabombaStatue1+0xe3
	.global Tileset_GabombaStatueBoneArmletChestTilesA
Tileset_GabombaStatueBoneArmletChestTilesA:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_bone_armlet_chest/tiles_a.4bpp.mtf"
	.byte 0x22, 0xe3, 0x07	@ PACKER: left in the buffer by Field_KibomboHouses+0x1949
	.global Tileset_GabombaStatueBoneArmletChestTilesB
Tileset_GabombaStatueBoneArmletChestTilesB:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_bone_armlet_chest/tiles_b.4bpp.mtf"
	.global Tileset_GabombaStatueBoneArmletChestTilesC
Tileset_GabombaStatueBoneArmletChestTilesC:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_bone_armlet_chest/tiles_c.4bpp.mtf"
	.global Tileset_GabombaStatueBoneArmletChestTilesD
Tileset_GabombaStatueBoneArmletChestTilesD:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_bone_armlet_chest/tiles_d.4bpp.mtf"
	.byte 0x20	@ PACKER: left in the buffer by Tileset_GabombaStatueBoneArmletChestTilesC+0x13f
