/* Numbers the C loads from literal pools that no table or catalogue names
   yet. Each leaves when its last user switches to the name its owner
   defines: scene numbers are the rows of FIELD/SCENES.S and resource
   numbers the rows of SYSTEM/RESOURCE/DIRECTORY.S. */
Data_0000001f = 0x0000001f;
Value_0000007e = 0x0000007e;
Value_00000925 = 0x00000925;
/* Routines copied to RAM as one block across several modules: the length
   of each copy runs from its entry to the end label of its last module. */
Resource_Decode2CodeSize = Resource_Decode2End - Resource_Decode2;
Render_DecodeFrameCodeSize = Render_DecodeFrameEnd - Render_DecodeFrame;
Tile_Decompress4bppCodeSize = Tile_Decompress4bppEnd - Tile_Decompress4bpp;
Tile_ExpandMaskedCodeSize = Tile_ExpandMaskedEnd - Tile_ExpandMasked;
Tile_ExpandOpaqueCodeSize = Tile_ExpandOpaqueEnd - Tile_ExpandOpaque;
SentouKouka_TenkaiCodeSize = SentouKouka_TenkaiEnd - SentouKouka_Tenkai;
/* The sound driver's link-time settings: the number of music players and
   the mixer's scanline limit (0: none). */
Sound_PlayerCount = 0x00000008;
Sound_MaxLines = 0x00000000;
