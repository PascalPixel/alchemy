/* Routines copied to RAM as one block across several modules: the length
   of each copy runs from its entry to the end label of its last module. */
Resource_Decode2CodeSize = Resource_Decode2End - Resource_Decode2;
Render_DecodeFrameCodeSize = Render_DecodeFrameEnd - Render_DecodeFrame;
Tile_Decompress4bppCodeSize = Tile_Decompress4bppEnd - Tile_Decompress4bpp;
Tile_ExpandMaskedCodeSize = Tile_ExpandMaskedEnd - Tile_ExpandMasked;
Tile_ExpandOpaqueCodeSize = Tile_ExpandOpaqueEnd - Tile_ExpandOpaque;
SentouKouka_TenkaiCodeSize = SentouKouka_TenkaiEnd - SentouKouka_Tenkai;
Object_UpdateAllCodeSize = Object_UpdateAllEnd - Object_UpdateAll;
Tile_BuildMetatilesCodeSize = Tile_BuildMetatilesEnd - Tile_BuildMetatiles;
Transform_UpdateVerticesSize = Transform_UpdateVerticesEnd - Transform_UpdateVertices;
