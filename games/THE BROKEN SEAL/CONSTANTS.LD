/* Numbers the C loads from literal pools that no table or catalogue names
   yet. Each leaves when its last user switches to the name its owner
   defines: scene numbers are the rows of FIELD/SCENES.S. */
Value_00000002 = 0x00000002;
Value_00000013 = 0x00000013;
Value_0000001b = 0x0000001b;
Data_0000001f = 0x0000001f;
Value_00000021 = 0x00000021;
Value_00000036 = 0x00000036;
Value_00000037 = 0x00000037;
Value_00000038 = 0x00000038;
Value_00000039 = 0x00000039;
Value_0000003a = 0x0000003a;
Value_0000003b = 0x0000003b;
Value_0000003c = 0x0000003c;
Value_0000003d = 0x0000003d;
Value_0000004b = 0x0000004b;
Value_0000004c = 0x0000004c;
Value_0000004d = 0x0000004d;
Value_0000004e = 0x0000004e;
Value_0000004f = 0x0000004f;
Value_00000054 = 0x00000054;
Value_0000005a = 0x0000005a;
Value_0000006c = 0x0000006c;
Value_0000006e = 0x0000006e;
Value_0000006f = 0x0000006f;
Value_00000073 = 0x00000073;
Value_00000076 = 0x00000076;
Value_0000007a = 0x0000007a;
Value_0000007d = 0x0000007d;
Value_0000007e = 0x0000007e;
Value_00000083 = 0x00000083;
Value_00000084 = 0x00000084;
Value_0000008c = 0x0000008c;
Value_0000008d = 0x0000008d;
Value_00000092 = 0x00000092;
Value_0000009e = 0x0000009e;
Value_000000a1 = 0x000000a1;
Value_000000a6 = 0x000000a6;
Value_000000ab = 0x000000ab;
Value_000000ac = 0x000000ac;
Value_000000af = 0x000000af;
Value_000000b4 = 0x000000b4;
Value_000000b8 = 0x000000b8;
Value_000000b9 = 0x000000b9;
Value_000000ba = 0x000000ba;
Value_000000bb = 0x000000bb;
Value_000000be = 0x000000be;
Value_000000c8 = 0x000000c8;
Value_000000cb = 0x000000cb;
Value_000000ce = 0x000000ce;
Value_000000cf = 0x000000cf;
Value_000000d5 = 0x000000d5;
Data_000000e8 = 0x000000e8;
Value_000000f0 = 0x000000f0;
Data_000000f1 = 0x000000f1;
Value_000000f1 = 0x000000f1;
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
Value_00000018 = 0x00000018;
Value_00000050 = 0x00000050;
Value_00000075 = 0x00000075;
Value_00000182 = 0x00000182;
gVal4 = 0x00000d4c;
Value_00000c9b = 0x00000c9b;
Value_00000cc6 = 0x00000cc6;
Value_00000cf1 = 0x00000cf1;
