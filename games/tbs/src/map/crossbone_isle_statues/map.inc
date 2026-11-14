	.global Field_CrossboneIsleStatues
Field_CrossboneIsleStatues:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleStatues, .LField_CrossboneIsleStatues_CELL_PICTURES
	map_offset Field_CrossboneIsleStatues, .LField_CrossboneIsleStatues_SHAPES
	map_offset Field_CrossboneIsleStatues, .LField_CrossboneIsleStatues_GRID
	map_offset Field_CrossboneIsleStatues, .LField_CrossboneIsleStatues_ANIMATION
	map_offset Field_CrossboneIsleStatues, .LField_CrossboneIsleStatues_BLEND
	map_offset Field_CrossboneIsleStatues, .LField_CrossboneIsleStatues_MARKERS
.LField_CrossboneIsleStatues_CELL_PICTURES:
	.incbin "games/tbs/src/map/crossbone_isle_statues/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_CrossboneIsleStatues_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_statues/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleStatues_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_statues/map.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleStatues_ANIMATION:
	.incbin "games/tbs/src/map/crossbone_isle_statues/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleStatues_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_CrossboneIsleStatues_MARKERS:
	treasure_spot 17, 3, 100
	treasure_spot 26, 8, 102
	treasure_spot 16, 16, 101
	treasure_spot 4, 24, 103
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteCrossboneIsleStatues
Tileset_PaletteCrossboneIsleStatues:
	.incbin "games/tbs/src/graphics/tileset/palettes/crossbone_isle_statues.gbapal.lz"
	.byte 0xc7, 0x0c, 0xb4	@ PACKER: left in the buffer by Field_CrossboneIsleStatues+0x179
	.balign 4, 0
