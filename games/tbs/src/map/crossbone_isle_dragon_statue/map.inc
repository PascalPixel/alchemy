	.global Field_CrossboneIsleDragonStatue
Field_CrossboneIsleDragonStatue:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleDragonStatue, .LField_CrossboneIsleDragonStatue_CELL_PICTURES
	map_offset Field_CrossboneIsleDragonStatue, .LField_CrossboneIsleDragonStatue_SHAPES
	map_offset Field_CrossboneIsleDragonStatue, .LField_CrossboneIsleDragonStatue_GRID
	map_offset Field_CrossboneIsleDragonStatue, .LField_CrossboneIsleDragonStatue_ANIMATION
	map_offset Field_CrossboneIsleDragonStatue, .LField_CrossboneIsleDragonStatue_BLEND
	map_offset Field_CrossboneIsleDragonStatue, .LField_CrossboneIsleDragonStatue_MARKERS
.LField_CrossboneIsleDragonStatue_CELL_PICTURES:
	.incbin "games/tbs/src/map/crossbone_isle_dragon_statue/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_CrossboneIsleDragonStatue_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_dragon_statue/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleDragonStatue_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_dragon_statue/map.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleDragonStatue_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleDragonStatue_BLEND:
	.incbin "games/tbs/src/map/crossbone_isle_dragon_statue/blend.script.lz"
	.balign 4, 0
.LField_CrossboneIsleDragonStatue_MARKERS:
	treasure_spot 4, 12, 101
	treasure_spot 14, 13, 100
	treasure_spot 14, 14, 102
	treasure_spot 25, 16, 103
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteCrossboneIsleDragonStatue
Tileset_PaletteCrossboneIsleDragonStatue:
	.incbin "games/tbs/src/graphics/tileset/palettes/crossbone_isle_dragon_statue.gbapal.lz"
	.balign 4, 0
