@ Unidentified ROM data, read from your own ROM at build time as early pret
@ projects read their base ROM. Each section shrinks as its data gains source.
	.section .unidentified.08000000,"a"
	.global Resource_Data000
Resource_Data000:
	.incbin "baserom.gba", 0x00000000, 0x000000c0
	.section .unidentified.080178b4,"a"
	.global Data_080178b4
Data_080178b4:
	.incbin "baserom.gba", 0x000178b4, 0x00000136
	.global Data_080179ea
Data_080179ea:
	.incbin "baserom.gba", 0x000179ea, 0x00000106
	.global Data_08017af0
Data_08017af0:
	.incbin "baserom.gba", 0x00017af0, 0x00000020
	.global Data_08017b10
Data_08017b10:
	.incbin "baserom.gba", 0x00017b10, 0x000001c0
	.global Data_08017cd0
Data_08017cd0:
	.incbin "baserom.gba", 0x00017cd0, 0x00000014
	.section .unidentified.08017d08,"a"
	.global Data_08017d08
Data_08017d08:
	.incbin "baserom.gba", 0x00017d08, 0x00000008
	.global Data_08017d10
Data_08017d10:
	.incbin "baserom.gba", 0x00017d10, 0x00000054
	.global Flash_Chips
Flash_Chips:
	.incbin "baserom.gba", 0x00017d64, 0x00000014
	.section .unidentified.08017dc0,"a"
	.incbin "baserom.gba", 0x00017dc0, 0x00000024
	.section .unidentified.08017dfc,"a"
	.incbin "baserom.gba", 0x00017dfc, 0x00000018
	.global Flash_ChipUnknown
Flash_ChipUnknown:
	.incbin "baserom.gba", 0x00017e14, 0x00000058
	.section .unidentified.08017e90,"a"
	.incbin "baserom.gba", 0x00017e90, 0x0000008c
	.section .unidentified.08017f24,"a"
	.incbin "baserom.gba", 0x00017f24, 0x00000018
	.global Flash_ChipAtmel
Flash_ChipAtmel:
	.incbin "baserom.gba", 0x00017f3c, 0x0000002c
	.global Flash_ChipAtmelLayout
Flash_ChipAtmelLayout:
	.incbin "baserom.gba", 0x00017f68, 0x0000002c
	.section .unidentified.08017fbc,"a"
	.incbin "baserom.gba", 0x00017fbc, 0x00008044
	.section .unidentified.080203a8,"a"
	.global Data_080203a8
Data_080203a8:
	.incbin "baserom.gba", 0x000203a8, 0x00000500
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "baserom.gba", 0x000208a8, 0x00000100
	.global Data_080209a8
Data_080209a8:
	.incbin "baserom.gba", 0x000209a8, 0x00000400
	.section .unidentified.0802e89c,"a"
	.global Data_0802e89c
Data_0802e89c:
	.incbin "baserom.gba", 0x0002e89c, 0x00000080
	.global Data_0802e91c
Data_0802e91c:
	.incbin "baserom.gba", 0x0002e91c, 0x000001a8
	.global Data_0802eac4
Data_0802eac4:
	.incbin "baserom.gba", 0x0002eac4, 0x00000010
	.global Data_0802ead4
Data_0802ead4:
	.incbin "baserom.gba", 0x0002ead4, 0x00000008
	.global Data_0802eadc
Data_0802eadc:
	.incbin "baserom.gba", 0x0002eadc, 0x00000008
	.global Data_0802eae4
Data_0802eae4:
	.incbin "baserom.gba", 0x0002eae4, 0x00000020
	.global Data_0802eb04
Data_0802eb04:
	.incbin "baserom.gba", 0x0002eb04, 0x00000040
	.global Data_0802eb44
Data_0802eb44:
	.incbin "baserom.gba", 0x0002eb44, 0x00000008
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0002eb4c, 0x00000044
	.global Data_0802eb90
Data_0802eb90:
	.incbin "baserom.gba", 0x0002eb90, 0x00000008
	.global Data_0802eb98
Data_0802eb98:
	.incbin "baserom.gba", 0x0002eb98, 0x00000030
	.global Data_0802ebc8
Data_0802ebc8:
	.incbin "baserom.gba", 0x0002ebc8, 0x00000080
	.global Data_0802ec48
Data_0802ec48:
	.incbin "baserom.gba", 0x0002ec48, 0x00000014
	.global Data_0802ec5c
Data_0802ec5c:
	.incbin "baserom.gba", 0x0002ec5c, 0x00000020
	.global Data_0802ec7c
Data_0802ec7c:
	.incbin "baserom.gba", 0x0002ec7c, 0x0000000c
	.global Data_0802ec88
Data_0802ec88:
	.incbin "baserom.gba", 0x0002ec88, 0x0000000c
	.global Data_0802ec94
Data_0802ec94:
	.incbin "baserom.gba", 0x0002ec94, 0x0000000c
	.global Data_0802eca0
Data_0802eca0:
	.incbin "baserom.gba", 0x0002eca0, 0x00000040
	.global Data_0802ece0
Data_0802ece0:
	.incbin "baserom.gba", 0x0002ece0, 0x00000048
	.global Data_0802ed28
Data_0802ed28:
	.incbin "baserom.gba", 0x0002ed28, 0x0000000c
	.global Data_0802ed34
Data_0802ed34:
	.incbin "baserom.gba", 0x0002ed34, 0x00000090
	.global Data_0802edc4
Data_0802edc4:
	.incbin "baserom.gba", 0x0002edc4, 0x00000100
	.global Data_0802eec4
Data_0802eec4:
	.incbin "baserom.gba", 0x0002eec4, 0x00000100
	.global Data_0802efc4
Data_0802efc4:
	.incbin "baserom.gba", 0x0002efc4, 0x00000040
	.global Data_0802f004
Data_0802f004:
	.incbin "baserom.gba", 0x0002f004, 0x00000050
	.global Data_0802f054
Data_0802f054:
	.incbin "baserom.gba", 0x0002f054, 0x00000010
	.global Data_0802f064
Data_0802f064:
	.incbin "baserom.gba", 0x0002f064, 0x0000002c
	.global Battle_FormationPlacementScale
Battle_FormationPlacementScale:
	.incbin "baserom.gba", 0x0002f090, 0x00000008
	.global Data_0802f098
Data_0802f098:
	.incbin "baserom.gba", 0x0002f098, 0x00000018
	.global Data_0802f0b0
Data_0802f0b0:
	.incbin "baserom.gba", 0x0002f0b0, 0x00000018
	.global Data_0802f0c8
Data_0802f0c8:
	.incbin "baserom.gba", 0x0002f0c8, 0x00000018
	.global Data_0802f0e0
Data_0802f0e0:
	.incbin "baserom.gba", 0x0002f0e0, 0x00000018
	.global Data_0802f0f8
Data_0802f0f8:
	.incbin "baserom.gba", 0x0002f0f8, 0x00000018
	.global Data_0802f110
Data_0802f110:
	.incbin "baserom.gba", 0x0002f110, 0x00000018
	.global Data_0802f128
Data_0802f128:
	.incbin "baserom.gba", 0x0002f128, 0x00000018
	.global Data_0802f140
Data_0802f140:
	.incbin "baserom.gba", 0x0002f140, 0x00000018
	.global Data_0802f158
Data_0802f158:
	.incbin "baserom.gba", 0x0002f158, 0x00000018
	.global Data_0802f170
Data_0802f170:
	.incbin "baserom.gba", 0x0002f170, 0x00000018
	.global Data_0802f188
Data_0802f188:
	.incbin "baserom.gba", 0x0002f188, 0x00000018
	.global Data_0802f1a0
Data_0802f1a0:
	.incbin "baserom.gba", 0x0002f1a0, 0x00000018
	.global Data_0802f1b8
Data_0802f1b8:
	.incbin "baserom.gba", 0x0002f1b8, 0x00000018
	.global ObjectDispatch_Table4
ObjectDispatch_Table4:
	.incbin "baserom.gba", 0x0002f1d0, 0x00000018
	.global Data_0802f1e8
Data_0802f1e8:
	.incbin "baserom.gba", 0x0002f1e8, 0x00000018
	.global ObjectDispatch_Table6
ObjectDispatch_Table6:
	.incbin "baserom.gba", 0x0002f200, 0x00000004
	.global Data_0802f204
Data_0802f204:
	.incbin "baserom.gba", 0x0002f204, 0x000000d8
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x0002f2dc, 0x000000a4
	.global Data_0802f380
Data_0802f380:
	.incbin "baserom.gba", 0x0002f380, 0x00000f90
	.section .unidentified.08030320,"a"
	.incbin "baserom.gba", 0x00030320, 0x00007ce0
	.section .unidentified.0804e584,"a"
	.global Data_0804e584
Data_0804e584:
	.incbin "baserom.gba", 0x0004e584, 0x00000100
	.global Data_0804e684
Data_0804e684:
	.incbin "baserom.gba", 0x0004e684, 0x000000bc
	.global Data_0804e740
Data_0804e740:
	.incbin "baserom.gba", 0x0004e740, 0x0000009c
	.global Data_0804e7dc
Data_0804e7dc:
	.incbin "baserom.gba", 0x0004e7dc, 0x00000298
	.global UiIcon_OverlayPointerTable
UiIcon_OverlayPointerTable:
	.incbin "baserom.gba", 0x0004ea74, 0x000000e4
	.global Data_0804eb58
Data_0804eb58:
	.incbin "baserom.gba", 0x0004eb58, 0x000005cc
	.global Data_0804f124
Data_0804f124:
	.incbin "baserom.gba", 0x0004f124, 0x000058f0
	.global Data_08054a14
Data_08054a14:
	.incbin "baserom.gba", 0x00054a14, 0x00000410
	.global Data_08054e24
Data_08054e24:
	.incbin "baserom.gba", 0x00054e24, 0x000041d0
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.incbin "baserom.gba", 0x00058ff4, 0x00000804
	.global Data_080597f8
Data_080597f8:
	.incbin "baserom.gba", 0x000597f8, 0x00000080
	.global Data_08059878
Data_08059878:
	.incbin "baserom.gba", 0x00059878, 0x00000080
	.global Data_080598f8
Data_080598f8:
	.incbin "baserom.gba", 0x000598f8, 0x00000080
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00059978, 0x00000768
	.global Data_0805a0e0
Data_0805a0e0:
	.incbin "baserom.gba", 0x0005a0e0, 0x00000400
	.section .unidentified.0805c0e0,"a"
	.incbin "baserom.gba", 0x0005c0e0, 0x0000002c
	.global Data_0805c10c
Data_0805c10c:
	.incbin "baserom.gba", 0x0005c10c, 0x00000020
	.global Data_0805c12c
Data_0805c12c:
	.incbin "baserom.gba", 0x0005c12c, 0x00000050
	.global Data_0805c17c
Data_0805c17c:
	.incbin "baserom.gba", 0x0005c17c, 0x00000038
	.global Data_0805c1b4
Data_0805c1b4:
	.incbin "baserom.gba", 0x0005c1b4, 0x00000010
	.global Data_0805c1c4
Data_0805c1c4:
	.incbin "baserom.gba", 0x0005c1c4, 0x00000400
	.global Data_0805c5c4
Data_0805c5c4:
	.incbin "baserom.gba", 0x0005c5c4, 0x00000400
	.global Data_0805c9c4
Data_0805c9c4:
	.incbin "baserom.gba", 0x0005c9c4, 0x00002000
	.global Data_0805e9c4
Data_0805e9c4:
	.incbin "baserom.gba", 0x0005e9c4, 0x00000048
	.global Data_0805ea0c
Data_0805ea0c:
	.incbin "baserom.gba", 0x0005ea0c, 0x00000010
	.global Data_0805ea1c
Data_0805ea1c:
	.incbin "baserom.gba", 0x0005ea1c, 0x00000060
	.global Data_0805ea7c
Data_0805ea7c:
	.incbin "baserom.gba", 0x0005ea7c, 0x00000003
	.global Data_0805ea7f
Data_0805ea7f:
	.incbin "baserom.gba", 0x0005ea7f, 0x00000002
	.global Data_0805ea81
Data_0805ea81:
	.incbin "baserom.gba", 0x0005ea81, 0x00000002
	.global Data_0805ea83
Data_0805ea83:
	.incbin "baserom.gba", 0x0005ea83, 0x00000006
	.global Data_0805ea89
Data_0805ea89:
	.incbin "baserom.gba", 0x0005ea89, 0x00000006
	.global Data_0805ea8f
Data_0805ea8f:
	.incbin "baserom.gba", 0x0005ea8f, 0x00000009
	.global Data_0805ea98
Data_0805ea98:
	.incbin "baserom.gba", 0x0005ea98, 0x00000010
	.global Data_0805eaa8
Data_0805eaa8:
	.incbin "baserom.gba", 0x0005eaa8, 0x00000008
	.global Data_0805eab0
Data_0805eab0:
	.incbin "baserom.gba", 0x0005eab0, 0x00000008
	.global Data_0805eab8
Data_0805eab8:
	.incbin "baserom.gba", 0x0005eab8, 0x00000004
	.global Data_0805eabc
Data_0805eabc:
	.incbin "baserom.gba", 0x0005eabc, 0x00000094
	.global Data_0805eb50
Data_0805eb50:
	.incbin "baserom.gba", 0x0005eb50, 0x00000008
	.global Data_0805eb58
Data_0805eb58:
	.incbin "baserom.gba", 0x0005eb58, 0x00000024
	.global Data_0805eb7c
Data_0805eb7c:
	.incbin "baserom.gba", 0x0005eb7c, 0x00000108
	.global Data_0805ec84
Data_0805ec84:
	.incbin "baserom.gba", 0x0005ec84, 0x00000800
	.global Data_0805f484
Data_0805f484:
	.incbin "baserom.gba", 0x0005f484, 0x000000e0
	.global Data_0805f564
Data_0805f564:
	.incbin "baserom.gba", 0x0005f564, 0x00000010
	.global Data_0805f574
Data_0805f574:
	.incbin "baserom.gba", 0x0005f574, 0x00000011
	.global Data_0805f585
Data_0805f585:
	.incbin "baserom.gba", 0x0005f585, 0x00000067
	.global Data_0805f5ec
Data_0805f5ec:
	.incbin "baserom.gba", 0x0005f5ec, 0x00000054
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x0005f640, 0x00000016
	.global Data_0805f656
Data_0805f656:
	.incbin "baserom.gba", 0x0005f656, 0x00000008
	.global Data_0805f65e
Data_0805f65e:
	.incbin "baserom.gba", 0x0005f65e, 0x00000008
	.global Data_0805f666
Data_0805f666:
	.incbin "baserom.gba", 0x0005f666, 0x00000010
	.global Data_0805f676
Data_0805f676:
	.incbin "baserom.gba", 0x0005f676, 0x00000060
	.global Data_0805f6d6
Data_0805f6d6:
	.incbin "baserom.gba", 0x0005f6d6, 0x0000000a
	.global Data_0805f6e0
Data_0805f6e0:
	.incbin "baserom.gba", 0x0005f6e0, 0x00000020
	.global Data_0805f700
Data_0805f700:
	.incbin "baserom.gba", 0x0005f700, 0x00000030
	.global Data_0805f730
Data_0805f730:
	.incbin "baserom.gba", 0x0005f730, 0x00000040
	.global Graphics_ExpandNibbleTable
Graphics_ExpandNibbleTable:
	.incbin "baserom.gba", 0x0005f770, 0x00000040
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x0005f7b0, 0x00000008
	.global Data_0805f7b8
Data_0805f7b8:
	.incbin "baserom.gba", 0x0005f7b8, 0x00000020
	.global Data_0805f7d8
Data_0805f7d8:
	.incbin "baserom.gba", 0x0005f7d8, 0x00000080
	.global Data_0805f858
Data_0805f858:
	.incbin "baserom.gba", 0x0005f858, 0x00000010
	.global Data_0805f868
Data_0805f868:
	.incbin "baserom.gba", 0x0005f868, 0x00000020
	.global Data_0805f888
Data_0805f888:
	.incbin "baserom.gba", 0x0005f888, 0x00000004
	.global Data_0805f88c
Data_0805f88c:
	.incbin "baserom.gba", 0x0005f88c, 0x00000004
	.global Data_0805f890
Data_0805f890:
	.incbin "baserom.gba", 0x0005f890, 0x00000004
	.global Data_0805f894
Data_0805f894:
	.incbin "baserom.gba", 0x0005f894, 0x00000003
	.global Data_0805f897
Data_0805f897:
	.incbin "baserom.gba", 0x0005f897, 0x00000004
	.global Data_0805f89b
Data_0805f89b:
	.incbin "baserom.gba", 0x0005f89b, 0x00000004
	.global Data_0805f89f
Data_0805f89f:
	.incbin "baserom.gba", 0x0005f89f, 0x00000008
	.global Data_0805f8a7
Data_0805f8a7:
	.incbin "baserom.gba", 0x0005f8a7, 0x0000000c
	.global Data_0805f8b3
Data_0805f8b3:
	.incbin "baserom.gba", 0x0005f8b3, 0x0000000c
	.global Data_0805f8bf
Data_0805f8bf:
	.incbin "baserom.gba", 0x0005f8bf, 0x00000019
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x0005f8d8, 0x00000004
	.global Menu_HexDigitsString
Menu_HexDigitsString:
	.incbin "baserom.gba", 0x0005f8dc, 0x00000014
	.global Data_0805f8f0
Data_0805f8f0:
	.incbin "baserom.gba", 0x0005f8f0, 0x00000008
	.global Data_0805f8f8
Data_0805f8f8:
	.incbin "baserom.gba", 0x0005f8f8, 0x00000008
	.global Data_0805f900
Data_0805f900:
	.incbin "baserom.gba", 0x0005f900, 0x00000008
	.global Data_0805f908
Data_0805f908:
	.incbin "baserom.gba", 0x0005f908, 0x00000008
	.global Data_0805f910
Data_0805f910:
	.incbin "baserom.gba", 0x0005f910, 0x00000004
	.section .unidentified.080aa0dc,"a"
	.global Data_080aa0dc
Data_080aa0dc:
	.incbin "baserom.gba", 0x000aa0dc, 0x00000003
	.global Data_080aa0df
Data_080aa0df:
	.incbin "baserom.gba", 0x000aa0df, 0x00000003
	.global Data_080aa0e2
Data_080aa0e2:
	.incbin "baserom.gba", 0x000aa0e2, 0x00000003
	.global Data_080aa0e5
Data_080aa0e5:
	.incbin "baserom.gba", 0x000aa0e5, 0x00000003
	.global Data_080aa0e8
Data_080aa0e8:
	.incbin "baserom.gba", 0x000aa0e8, 0x00000040
	.global Data_080aa128
Data_080aa128:
	.incbin "baserom.gba", 0x000aa128, 0x00000048
	.global Data_080aa170
Data_080aa170:
	.incbin "baserom.gba", 0x000aa170, 0x00000048
	.global Data_080aa1b8
Data_080aa1b8:
	.incbin "baserom.gba", 0x000aa1b8, 0x00000120
	.global Data_080aa2d8
Data_080aa2d8:
	.incbin "baserom.gba", 0x000aa2d8, 0x00000104
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000aa3dc, 0x00002c24
	.section .unidentified.080b127c,"a"
	.global Data_080b127c
Data_080b127c:
	.incbin "baserom.gba", 0x000b127c, 0x00000008
	.global Data_080b1284
Data_080b1284:
	.incbin "baserom.gba", 0x000b1284, 0x0000000c
	.global Data_080b1290
Data_080b1290:
	.incbin "baserom.gba", 0x000b1290, 0x00000038
	.global Data_080b12c8
Data_080b12c8:
	.incbin "baserom.gba", 0x000b12c8, 0x00000c60
	.section .unidentified.080b1f2c,"a"
	.global Data_080b1f2c
Data_080b1f2c:
	.incbin "baserom.gba", 0x000b1f2c, 0x00000014
	.global Data_080b1f40
Data_080b1f40:
	.incbin "baserom.gba", 0x000b1f40, 0x00000400
	.global Data_080b2340
Data_080b2340:
	.incbin "baserom.gba", 0x000b2340, 0x00000024
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x000b2364, 0x000058b0
	.global Data_080b7c14
Data_080b7c14:
	.incbin "baserom.gba", 0x000b7c14, 0x00002268
	.global Data_080b9e7c
Data_080b9e7c:
	.incbin "baserom.gba", 0x000b9e7c, 0x000070d0
	.global Data_080c0f4c
Data_080c0f4c:
	.incbin "baserom.gba", 0x000c0f4c, 0x000005a0
	.global Summon_OrderList
Summon_OrderList:
	.incbin "baserom.gba", 0x000c14ec, 0x00000020
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x000c150c, 0x000000e8
	.global Data_080c15f4
Data_080c15f4:
	.incbin "baserom.gba", 0x000c15f4, 0x00005010
	.global Data_080c6604
Data_080c6604:
	.incbin "baserom.gba", 0x000c6604, 0x00000040
	.global Data_080c6644
Data_080c6644:
	.incbin "baserom.gba", 0x000c6644, 0x00000040
	.global Data_080c6684
Data_080c6684:
	.incbin "baserom.gba", 0x000c6684, 0x00000480
	.global Data_080c6b04
Data_080c6b04:
	.incbin "baserom.gba", 0x000c6b04, 0x00000014
	.global Data_080c6b18
Data_080c6b18:
	.incbin "baserom.gba", 0x000c6b18, 0x0000005e
	.global Data_080c6b76
Data_080c6b76:
	.incbin "baserom.gba", 0x000c6b76, 0x0000003a
	.global Djinn_Definitions
Djinn_Definitions:
	.incbin "baserom.gba", 0x000c6bb0, 0x00001450
	.section .unidentified.080c89cc,"a"
	.incbin "baserom.gba", 0x000c89cc, 0x00000a00
	.section .unidentified.080ed80c,"a"
	.global Data_080ed80c
Data_080ed80c:
	.incbin "baserom.gba", 0x000ed80c, 0x00000100
	.global Data_080ed90c
Data_080ed90c:
	.incbin "baserom.gba", 0x000ed90c, 0x00000100
	.global Data_080eda0c
Data_080eda0c:
	.incbin "baserom.gba", 0x000eda0c, 0x000000c0
	.global Data_080edacc
Data_080edacc:
	.incbin "baserom.gba", 0x000edacc, 0x00000c08
	.global Data_080ee6d4
Data_080ee6d4:
	.incbin "baserom.gba", 0x000ee6d4, 0x000006e8
	.global Data_080eedbc
Data_080eedbc:
	.incbin "baserom.gba", 0x000eedbc, 0x00000178
	.global Data_080eef34
Data_080eef34:
	.incbin "baserom.gba", 0x000eef34, 0x00000020
	.global Data_080eef54
Data_080eef54:
	.incbin "baserom.gba", 0x000eef54, 0x00000140
	.global Data_080ef094
Data_080ef094:
	.incbin "baserom.gba", 0x000ef094, 0x00000410
	.global Data_080ef4a4
Data_080ef4a4:
	.incbin "baserom.gba", 0x000ef4a4, 0x00000378
	.section .unidentified.080ef824,"a"
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x000ef824, 0x00000160
	.global Data_080ef984
Data_080ef984:
	.incbin "baserom.gba", 0x000ef984, 0x00000398
	.global Data_080efd1c
Data_080efd1c:
	.incbin "baserom.gba", 0x000efd1c, 0x00000054
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "baserom.gba", 0x000efd70, 0x000001b2
	.global Data_080eff22
Data_080eff22:
	.incbin "baserom.gba", 0x000eff22, 0x00000006
	.global Data_080eff28
Data_080eff28:
	.incbin "baserom.gba", 0x000eff28, 0x000000b0
	.global Data_080effd8
Data_080effd8:
	.incbin "baserom.gba", 0x000effd8, 0x0000009c
	.global Data_080f0074
Data_080f0074:
	.incbin "baserom.gba", 0x000f0074, 0x00000120
	.global BattleFx_ParticleScript
BattleFx_ParticleScript:
	.incbin "baserom.gba", 0x000f0194, 0x00000024
	.global Data_080f01b8
Data_080f01b8:
	.incbin "baserom.gba", 0x000f01b8, 0x0000000c
	.global Data_080f01c4
Data_080f01c4:
	.incbin "baserom.gba", 0x000f01c4, 0x00000022
	.global Data_080f01e6
Data_080f01e6:
	.incbin "baserom.gba", 0x000f01e6, 0x00000020
	.global Data_080f0206
Data_080f0206:
	.incbin "baserom.gba", 0x000f0206, 0x00000040
	.global Data_080f0246
Data_080f0246:
	.incbin "baserom.gba", 0x000f0246, 0x00000040
	.global Data_080f0286
Data_080f0286:
	.incbin "baserom.gba", 0x000f0286, 0x00000040
	.global Data_080f02c6
Data_080f02c6:
	.incbin "baserom.gba", 0x000f02c6, 0x00000042
	.global Data_080f0308
Data_080f0308:
	.incbin "baserom.gba", 0x000f0308, 0x000003d0
	.global Data_080f06d8
Data_080f06d8:
	.incbin "baserom.gba", 0x000f06d8, 0x00000184
	.global Data_080f085c
Data_080f085c:
	.incbin "baserom.gba", 0x000f085c, 0x00000018
	.global Data_080f0874
Data_080f0874:
	.incbin "baserom.gba", 0x000f0874, 0x00000018
	.global Data_080f088c
Data_080f088c:
	.incbin "baserom.gba", 0x000f088c, 0x00000004
	.global Data_080f0890
Data_080f0890:
	.incbin "baserom.gba", 0x000f0890, 0x00000020
	.global Data_080f08b0
Data_080f08b0:
	.incbin "baserom.gba", 0x000f08b0, 0x00000200
	.global Data_080f0ab0
Data_080f0ab0:
	.incbin "baserom.gba", 0x000f0ab0, 0x000000a0
	.global Data_080f0b50
Data_080f0b50:
	.incbin "baserom.gba", 0x000f0b50, 0x0000002c
	.global Data_080f0b7c
Data_080f0b7c:
	.incbin "baserom.gba", 0x000f0b7c, 0x00000080
	.global Data_080f0bfc
Data_080f0bfc:
	.incbin "baserom.gba", 0x000f0bfc, 0x00000008
	.global Data_080f0c04
Data_080f0c04:
	.incbin "baserom.gba", 0x000f0c04, 0x00000018
	.global Data_080f0c1c
Data_080f0c1c:
	.incbin "baserom.gba", 0x000f0c1c, 0x00000047
	.global Data_080f0c63
Data_080f0c63:
	.incbin "baserom.gba", 0x000f0c63, 0x00000055
	.global Data_080f0cb8
Data_080f0cb8:
	.incbin "baserom.gba", 0x000f0cb8, 0x0000004f
	.global Data_080f0d07
Data_080f0d07:
	.incbin "baserom.gba", 0x000f0d07, 0x00000057
	.global Data_080f0d5e
Data_080f0d5e:
	.incbin "baserom.gba", 0x000f0d5e, 0x0000005d
	.global Data_080f0dbb
Data_080f0dbb:
	.incbin "baserom.gba", 0x000f0dbb, 0x00000045
	.global Data_080f0e00
Data_080f0e00:
	.incbin "baserom.gba", 0x000f0e00, 0x00000054
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000f0e54, 0x00000004
	.global Data_080f0e58
Data_080f0e58:
	.incbin "baserom.gba", 0x000f0e58, 0x00000008
	.global Data_080f0e60
Data_080f0e60:
	.incbin "baserom.gba", 0x000f0e60, 0x00000018
	.global Data_080f0e78
Data_080f0e78:
	.incbin "baserom.gba", 0x000f0e78, 0x00000024
	.global Data_080f0e9c
Data_080f0e9c:
	.incbin "baserom.gba", 0x000f0e9c, 0x00000008
	.global Data_080f0ea4
Data_080f0ea4:
	.incbin "baserom.gba", 0x000f0ea4, 0x00000020
	.global Data_080f0ec4
Data_080f0ec4:
	.incbin "baserom.gba", 0x000f0ec4, 0x00000020
	.global Data_080f0ee4
Data_080f0ee4:
	.incbin "baserom.gba", 0x000f0ee4, 0x00000004
	.global Data_080f0ee8
Data_080f0ee8:
	.incbin "baserom.gba", 0x000f0ee8, 0x00000010
	.global Data_080f0ef8
Data_080f0ef8:
	.incbin "baserom.gba", 0x000f0ef8, 0x00000034
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000f0f2c, 0x00000008
	.global Data_080f0f34
Data_080f0f34:
	.incbin "baserom.gba", 0x000f0f34, 0x00000020
	.global Data_080f0f54
Data_080f0f54:
	.incbin "baserom.gba", 0x000f0f54, 0x00000010
	.global Data_080f0f64
Data_080f0f64:
	.incbin "baserom.gba", 0x000f0f64, 0x0000002c
	.global Data_080f0f90
Data_080f0f90:
	.incbin "baserom.gba", 0x000f0f90, 0x00000010
	.global Data_080f0fa0
Data_080f0fa0:
	.incbin "baserom.gba", 0x000f0fa0, 0x00000020
	.global Data_080f0fc0
Data_080f0fc0:
	.incbin "baserom.gba", 0x000f0fc0, 0x00000020
	.global Data_080f0fe0
Data_080f0fe0:
	.incbin "baserom.gba", 0x000f0fe0, 0x00000060
	.global Data_080f1040
Data_080f1040:
	.incbin "baserom.gba", 0x000f1040, 0x000000c0
	.global Data_080f1100
Data_080f1100:
	.incbin "baserom.gba", 0x000f1100, 0x00000020
	.global Data_080f1120
Data_080f1120:
	.incbin "baserom.gba", 0x000f1120, 0x00000020
	.global Data_080f1140
Data_080f1140:
	.incbin "baserom.gba", 0x000f1140, 0x00000668
	.global Field_SceneTable
Field_SceneTable:
	.incbin "baserom.gba", 0x000f17a8, 0x00000a2c
	.global Data_080f21d4
Data_080f21d4:
	.incbin "baserom.gba", 0x000f21d4, 0x00000030
	.global Data_080f2204
Data_080f2204:
	.incbin "baserom.gba", 0x000f2204, 0x00000c98
	.global Data_080f2e9c
Data_080f2e9c:
	.incbin "baserom.gba", 0x000f2e9c, 0x0000038c
	.global Data_080f3228
Data_080f3228:
	.incbin "baserom.gba", 0x000f3228, 0x00000008
	.global Data_080f3230
Data_080f3230:
	.incbin "baserom.gba", 0x000f3230, 0x000000a0
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x000f32d0, 0x00000010
	.global Data_080f32e0
Data_080f32e0:
	.incbin "baserom.gba", 0x000f32e0, 0x00000020
	.global Data_080f3300
Data_080f3300:
	.incbin "baserom.gba", 0x000f3300, 0x0000000c
	.global Data_080f330c
Data_080f330c:
	.incbin "baserom.gba", 0x000f330c, 0x00000004
	.global Data_080f3310
Data_080f3310:
	.incbin "baserom.gba", 0x000f3310, 0x0000010c
	.global Data_080f341c
Data_080f341c:
	.incbin "baserom.gba", 0x000f341c, 0x0000000c
	.global Data_080f3428
Data_080f3428:
	.incbin "baserom.gba", 0x000f3428, 0x000000bc
	.global Data_080f34e4
Data_080f34e4:
	.incbin "baserom.gba", 0x000f34e4, 0x00000004
	.global Data_080f34e8
Data_080f34e8:
	.incbin "baserom.gba", 0x000f34e8, 0x0000000c
	.global Data_080f34f4
Data_080f34f4:
	.incbin "baserom.gba", 0x000f34f4, 0x0000000c
	.global Data_080f3500
Data_080f3500:
	.incbin "baserom.gba", 0x000f3500, 0x0000000c
	.global Data_080f350c
Data_080f350c:
	.incbin "baserom.gba", 0x000f350c, 0x000000bc
	.global Data_080f35c8
Data_080f35c8:
	.incbin "baserom.gba", 0x000f35c8, 0x0000004c
	.global Data_080f3614
Data_080f3614:
	.incbin "baserom.gba", 0x000f3614, 0x00000128
	.global Data_080f373c
Data_080f373c:
	.incbin "baserom.gba", 0x000f373c, 0x00000014
	.global Data_080f3750
Data_080f3750:
	.incbin "baserom.gba", 0x000f3750, 0x00000014
	.global Data_080f3764
Data_080f3764:
	.incbin "baserom.gba", 0x000f3764, 0x0000001c
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x000f3780, 0x00000018
	.global Data_080f3798
Data_080f3798:
	.incbin "baserom.gba", 0x000f3798, 0x000000c6
	.global Data_080f385e
Data_080f385e:
	.incbin "baserom.gba", 0x000f385e, 0x00000054
	.global Data_080f38b2
Data_080f38b2:
	.incbin "baserom.gba", 0x000f38b2, 0x0000004f
	.global Data_080f3901
Data_080f3901:
	.incbin "baserom.gba", 0x000f3901, 0x0000003b
	.global Data_080f393c
Data_080f393c:
	.incbin "baserom.gba", 0x000f393c, 0x00000004
	.global Data_080f3940
Data_080f3940:
	.incbin "baserom.gba", 0x000f3940, 0x00000004
	.global Data_080f3944
Data_080f3944:
	.incbin "baserom.gba", 0x000f3944, 0x00000004
	.global Data_080f3948
Data_080f3948:
	.incbin "baserom.gba", 0x000f3948, 0x00000004
	.global Data_080f394c
Data_080f394c:
	.incbin "baserom.gba", 0x000f394c, 0x00000004
	.global Data_080f3950
Data_080f3950:
	.incbin "baserom.gba", 0x000f3950, 0x00000004
	.global Data_080f3954
Data_080f3954:
	.incbin "baserom.gba", 0x000f3954, 0x00000020
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x000f3974, 0x00000004
	.global Data_080f3978
Data_080f3978:
	.incbin "baserom.gba", 0x000f3978, 0x0000000c
	.global Data_080f3984
Data_080f3984:
	.incbin "baserom.gba", 0x000f3984, 0x00000040
	.global Data_080f39c4
Data_080f39c4:
	.incbin "baserom.gba", 0x000f39c4, 0x00000200
	.global Data_080f3bc4
Data_080f3bc4:
	.incbin "baserom.gba", 0x000f3bc4, 0x00000018
	.global Data_080f3bdc
Data_080f3bdc:
	.incbin "baserom.gba", 0x000f3bdc, 0x00000020
	.global Data_080f3bfc
Data_080f3bfc:
	.incbin "baserom.gba", 0x000f3bfc, 0x00000020
	.global Data_080f3c1c
Data_080f3c1c:
	.incbin "baserom.gba", 0x000f3c1c, 0x000043e4
	.section .unidentified.081055f8,"a"
	.incbin "baserom.gba", 0x001055f8, 0x00000240
	.global Data_08105838
Data_08105838:
	.incbin "baserom.gba", 0x00105838, 0x00000100
	.global Data_08105938
Data_08105938:
	.incbin "baserom.gba", 0x00105938, 0x00000004
	.global Ui_HpString
Ui_HpString:
	.incbin "baserom.gba", 0x0010593c, 0x00000004
	.global Ui_SlashString
Ui_SlashString:
	.incbin "baserom.gba", 0x00105940, 0x00000004
	.global Ui_PpString
Ui_PpString:
	.incbin "baserom.gba", 0x00105944, 0x00000004
	.global Data_08105948
Data_08105948:
	.incbin "baserom.gba", 0x00105948, 0x00000020
	.global Data_08105968
Data_08105968:
	.incbin "baserom.gba", 0x00105968, 0x00000004
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x0010596c, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x00105970, 0x00000004
	.global Data_08105974
Data_08105974:
	.incbin "baserom.gba", 0x00105974, 0x00000004
	.global Data_08105978
Data_08105978:
	.incbin "baserom.gba", 0x00105978, 0x00000004
	.global Data_0810597c
Data_0810597c:
	.incbin "baserom.gba", 0x0010597c, 0x00000004
	.global Data_08105980
Data_08105980:
	.incbin "baserom.gba", 0x00105980, 0x00000004
	.global Data_08105984
Data_08105984:
	.incbin "baserom.gba", 0x00105984, 0x00000030
	.global Data_081059b4
Data_081059b4:
	.incbin "baserom.gba", 0x001059b4, 0x00000020
	.global Data_081059d4
Data_081059d4:
	.incbin "baserom.gba", 0x001059d4, 0x00000004
	.global Data_081059d8
Data_081059d8:
	.incbin "baserom.gba", 0x001059d8, 0x00000004
	.global Data_081059dc
Data_081059dc:
	.incbin "baserom.gba", 0x001059dc, 0x00000009
	.global Data_081059e5
Data_081059e5:
	.incbin "baserom.gba", 0x001059e5, 0x00000009
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x001059ee, 0x0000000d
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x001059fb, 0x0000000d
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x00105a08, 0x00000018
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x00105a20, 0x00000018
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x00105a38, 0x00000008
	.global Data_08105a40
Data_08105a40:
	.incbin "baserom.gba", 0x00105a40, 0x00000010
	.global Data_08105a50
Data_08105a50:
	.incbin "baserom.gba", 0x00105a50, 0x000025b0
	.section .unidentified.0810c008,"a"
	.global Data_0810c008
Data_0810c008:
	.incbin "baserom.gba", 0x0010c008, 0x00000140
	.global Data_0810c148
Data_0810c148:
	.incbin "baserom.gba", 0x0010c148, 0x00000100
	.global Data_0810c248
Data_0810c248:
	.incbin "baserom.gba", 0x0010c248, 0x00000100
	.global Data_0810c348
Data_0810c348:
	.incbin "baserom.gba", 0x0010c348, 0x0000003c
	.global Data_0810c384
Data_0810c384:
	.incbin "baserom.gba", 0x0010c384, 0x0000000a
	.global Data_0810c38e
Data_0810c38e:
	.incbin "baserom.gba", 0x0010c38e, 0x00000066
	.global Data_0810c3f4
Data_0810c3f4:
	.incbin "baserom.gba", 0x0010c3f4, 0x00000840
	.global Data_0810cc34
Data_0810cc34:
	.incbin "baserom.gba", 0x0010cc34, 0x000002f4
	.global Data_0810cf28
Data_0810cf28:
	.incbin "baserom.gba", 0x0010cf28, 0x00000004
	.global Data_0810cf2c
Data_0810cf2c:
	.incbin "baserom.gba", 0x0010cf2c, 0x0000b0d4
	.section .unidentified.081287c4,"a"
	.incbin "baserom.gba", 0x001287c4, 0x00000004
	.global Data_081287c8
Data_081287c8:
	.incbin "baserom.gba", 0x001287c8, 0x0000000c
	.global Data_081287d4
Data_081287d4:
	.incbin "baserom.gba", 0x001287d4, 0x0000000e
	.global Data_081287e2
Data_081287e2:
	.incbin "baserom.gba", 0x001287e2, 0x0000000e
	.global Data_081287f0
Data_081287f0:
	.incbin "baserom.gba", 0x001287f0, 0x0000000e
	.global Data_081287fe
Data_081287fe:
	.incbin "baserom.gba", 0x001287fe, 0x0000000e
	.global Data_0812880c
Data_0812880c:
	.incbin "baserom.gba", 0x0012880c, 0x0000000e
	.global Data_0812881a
Data_0812881a:
	.incbin "baserom.gba", 0x0012881a, 0x0000000e
	.global Data_08128828
Data_08128828:
	.incbin "baserom.gba", 0x00128828, 0x0000000e
	.global Data_08128836
Data_08128836:
	.incbin "baserom.gba", 0x00128836, 0x0000000e
	.global Data_08128844
Data_08128844:
	.incbin "baserom.gba", 0x00128844, 0x00000024
	.global Data_08128868
Data_08128868:
	.incbin "baserom.gba", 0x00128868, 0x00000030
	.global HitFalloff
HitFalloff:
	.incbin "baserom.gba", 0x00128898, 0x00000008
	.global PpLossFalloff
PpLossFalloff:
	.incbin "baserom.gba", 0x001288a0, 0x00000018
	.global HpHealFalloff
HpHealFalloff:
	.incbin "baserom.gba", 0x001288b8, 0x00000018
	.global PpDmgFalloff
PpDmgFalloff:
	.incbin "baserom.gba", 0x001288d0, 0x00000018
	.global HpDmgFalloff5
HpDmgFalloff5:
	.incbin "baserom.gba", 0x001288e8, 0x00000018
	.global HpDmgFalloff8
HpDmgFalloff8:
	.incbin "baserom.gba", 0x00128900, 0x00000018
	.global HpDmgFalloff6
HpDmgFalloff6:
	.incbin "baserom.gba", 0x00128918, 0x00000018
	.global PpHealFalloff
PpHealFalloff:
	.incbin "baserom.gba", 0x00128930, 0x00000018
	.global HpDmgFalloff
HpDmgFalloff:
	.incbin "baserom.gba", 0x00128948, 0x00000018
	.global Data_08128960
Data_08128960:
	.incbin "baserom.gba", 0x00128960, 0x0000000c
	.global Data_0812896c
Data_0812896c:
	.incbin "baserom.gba", 0x0012896c, 0x00000006
	.global Data_08128972
Data_08128972:
	.incbin "baserom.gba", 0x00128972, 0x00000006
	.incbin "baserom.gba", 0x00128978, 0x000002d8
	.global Data_08128c50
Data_08128c50:
	.incbin "baserom.gba", 0x00128c50, 0x00000b74
	.global Data_081297c4
Data_081297c4:
	.incbin "baserom.gba", 0x001297c4, 0x00000048
	.global Data_0812980c
Data_0812980c:
	.incbin "baserom.gba", 0x0012980c, 0x0000001c
	.global Data_08129828
Data_08129828:
	.incbin "baserom.gba", 0x00129828, 0x00000008
	.global Data_08129830
Data_08129830:
	.incbin "baserom.gba", 0x00129830, 0x0000013c
	.global Data_0812996c
Data_0812996c:
	.incbin "baserom.gba", 0x0012996c, 0x00000800
	.global Data_0812a16c
Data_0812a16c:
	.incbin "baserom.gba", 0x0012a16c, 0x00000008
	.global Data_0812a174
Data_0812a174:
	.incbin "baserom.gba", 0x0012a174, 0x00000002
	.global Data_0812a176
Data_0812a176:
	.incbin "baserom.gba", 0x0012a176, 0x00002956
	.global Data_0812cacc
Data_0812cacc:
	.incbin "baserom.gba", 0x0012cacc, 0x00000004
	.global Data_0812cad0
Data_0812cad0:
	.incbin "baserom.gba", 0x0012cad0, 0x000000e0
	.global Data_0812cbb0
Data_0812cbb0:
	.incbin "baserom.gba", 0x0012cbb0, 0x00000028
	.global Data_0812cbd8
Data_0812cbd8:
	.incbin "baserom.gba", 0x0012cbd8, 0x00000028
	.global Data_0812cc00
Data_0812cc00:
	.incbin "baserom.gba", 0x0012cc00, 0x00000028
	.global Data_0812cc28
Data_0812cc28:
	.incbin "baserom.gba", 0x0012cc28, 0x00000034
	.global Data_0812cc5c
Data_0812cc5c:
	.incbin "baserom.gba", 0x0012cc5c, 0x00000008
	.global Data_0812cc64
Data_0812cc64:
	.incbin "baserom.gba", 0x0012cc64, 0x00000008
	.global Data_0812cc6c
Data_0812cc6c:
	.incbin "baserom.gba", 0x0012cc6c, 0x00000008
	.global Data_0812cc74
Data_0812cc74:
	.incbin "baserom.gba", 0x0012cc74, 0x00000100
	.global Data_0812cd74
Data_0812cd74:
	.incbin "baserom.gba", 0x0012cd74, 0x000000e0
	.global Data_0812ce54
Data_0812ce54:
	.incbin "baserom.gba", 0x0012ce54, 0x00000020
	.section .unidentified.0812ce7c,"a"
	.global Data_0812ce7c
Data_0812ce7c:
	.incbin "baserom.gba", 0x0012ce7c, 0x00000018
	.global Data_0812ce94
Data_0812ce94:
	.incbin "baserom.gba", 0x0012ce94, 0x00003dc8
	.global Data_08130c5c
Data_08130c5c:
	.incbin "baserom.gba", 0x00130c5c, 0x000000b0
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "baserom.gba", 0x00130d0c, 0x000072f4
	.section .unidentified.08196dd8,"a"
	.global Data_08196dd8
Data_08196dd8:
	.incbin "baserom.gba", 0x00196dd8, 0x00000008
	.global Data_08196de0
Data_08196de0:
	.incbin "baserom.gba", 0x00196de0, 0x00000018
	.global Data_08196df8
Data_08196df8:
	.incbin "baserom.gba", 0x00196df8, 0x00000014
	.global Data_08196e0c
Data_08196e0c:
	.incbin "baserom.gba", 0x00196e0c, 0x00000008
	.global Data_08196e14
Data_08196e14:
	.incbin "baserom.gba", 0x00196e14, 0x00000008
	.global Data_08196e1c
Data_08196e1c:
	.incbin "baserom.gba", 0x00196e1c, 0x00000010
	.global Data_08196e2c
Data_08196e2c:
	.incbin "baserom.gba", 0x00196e2c, 0x00000018
	.global Data_08196e44
Data_08196e44:
	.incbin "baserom.gba", 0x00196e44, 0x00000008
	.global Data_08196e4c
Data_08196e4c:
	.incbin "baserom.gba", 0x00196e4c, 0x00000008
	.global Data_08196e54
Data_08196e54:
	.incbin "baserom.gba", 0x00196e54, 0x00000008
	.global Data_08196e5c
Data_08196e5c:
	.incbin "baserom.gba", 0x00196e5c, 0x00000008
	.global Data_08196e64
Data_08196e64:
	.incbin "baserom.gba", 0x00196e64, 0x00000008
	.global Data_08196e6c
Data_08196e6c:
	.incbin "baserom.gba", 0x00196e6c, 0x00000008
	.global Data_08196e74
Data_08196e74:
	.incbin "baserom.gba", 0x00196e74, 0x00000008
	.global Data_08196e7c
Data_08196e7c:
	.incbin "baserom.gba", 0x00196e7c, 0x00000008
	.global Data_08196e84
Data_08196e84:
	.incbin "baserom.gba", 0x00196e84, 0x00000008
	.global Data_08196e8c
Data_08196e8c:
	.incbin "baserom.gba", 0x00196e8c, 0x00000008
	.global Data_08196e94
Data_08196e94:
	.incbin "baserom.gba", 0x00196e94, 0x00000008
	.global Data_08196e9c
Data_08196e9c:
	.incbin "baserom.gba", 0x00196e9c, 0x0000000c
	.global Data_08196ea8
Data_08196ea8:
	.incbin "baserom.gba", 0x00196ea8, 0x00000008
	.global Data_08196eb0
Data_08196eb0:
	.incbin "baserom.gba", 0x00196eb0, 0x00000008
	.global Data_08196eb8
Data_08196eb8:
	.incbin "baserom.gba", 0x00196eb8, 0x00000008
	.global Data_08196ec0
Data_08196ec0:
	.incbin "baserom.gba", 0x00196ec0, 0x00000008
	.global Data_08196ec8
Data_08196ec8:
	.incbin "baserom.gba", 0x00196ec8, 0x00000008
	.global Data_08196ed0
Data_08196ed0:
	.incbin "baserom.gba", 0x00196ed0, 0x00000008
	.global Data_08196ed8
Data_08196ed8:
	.incbin "baserom.gba", 0x00196ed8, 0x00000008
	.global Data_08196ee0
Data_08196ee0:
	.incbin "baserom.gba", 0x00196ee0, 0x00000008
	.global Data_08196ee8
Data_08196ee8:
	.incbin "baserom.gba", 0x00196ee8, 0x00000008
	.global Data_08196ef0
Data_08196ef0:
	.incbin "baserom.gba", 0x00196ef0, 0x00000008
	.global Data_08196ef8
Data_08196ef8:
	.incbin "baserom.gba", 0x00196ef8, 0x00000008
	.global Data_08196f00
Data_08196f00:
	.incbin "baserom.gba", 0x00196f00, 0x00000008
	.global Data_08196f08
Data_08196f08:
	.incbin "baserom.gba", 0x00196f08, 0x00000008
	.global Data_08196f10
Data_08196f10:
	.incbin "baserom.gba", 0x00196f10, 0x00000008
	.global Data_08196f18
Data_08196f18:
	.incbin "baserom.gba", 0x00196f18, 0x00000008
	.global Data_08196f20
Data_08196f20:
	.incbin "baserom.gba", 0x00196f20, 0x00000008
	.global Data_08196f28
Data_08196f28:
	.incbin "baserom.gba", 0x00196f28, 0x00000008
	.global Data_08196f30
Data_08196f30:
	.incbin "baserom.gba", 0x00196f30, 0x00000018
	.global Data_08196f48
Data_08196f48:
	.incbin "baserom.gba", 0x00196f48, 0x00000008
	.global Data_08196f50
Data_08196f50:
	.incbin "baserom.gba", 0x00196f50, 0x00000014
	.global Data_08196f64
Data_08196f64:
	.incbin "baserom.gba", 0x00196f64, 0x0000001c
	.global Data_08196f80
Data_08196f80:
	.incbin "baserom.gba", 0x00196f80, 0x0000001c
	.global Data_08196f9c
Data_08196f9c:
	.incbin "baserom.gba", 0x00196f9c, 0x00000020
	.global Data_08196fbc
Data_08196fbc:
	.incbin "baserom.gba", 0x00196fbc, 0x00000020
	.section .unidentified.081973f0,"a"
	.global Data_081973f0
Data_081973f0:
	.incbin "baserom.gba", 0x001973f0, 0x00000020
	.global Data_08197410
Data_08197410:
	.incbin "baserom.gba", 0x00197410, 0x00000014
	.global Data_08197424
Data_08197424:
	.incbin "baserom.gba", 0x00197424, 0x00000014
	.global Data_08197438
Data_08197438:
	.incbin "baserom.gba", 0x00197438, 0x00000014
	.global Data_0819744c
Data_0819744c:
	.incbin "baserom.gba", 0x0019744c, 0x00000012
	.global Data_0819745e
Data_0819745e:
	.incbin "baserom.gba", 0x0019745e, 0x00000009
	.global Data_08197467
Data_08197467:
	.incbin "baserom.gba", 0x00197467, 0x00000006
	.global Data_0819746d
Data_0819746d:
	.incbin "baserom.gba", 0x0019746d, 0x00000006
	.global Data_08197473
Data_08197473:
	.incbin "baserom.gba", 0x00197473, 0x00000007
	.global Data_0819747a
Data_0819747a:
	.incbin "baserom.gba", 0x0019747a, 0x0000000c
	.global Data_08197486
Data_08197486:
	.incbin "baserom.gba", 0x00197486, 0x0000000c
	.global Data_08197492
Data_08197492:
	.incbin "baserom.gba", 0x00197492, 0x00000006
	.global Data_08197498
Data_08197498:
	.incbin "baserom.gba", 0x00197498, 0x00000006
	.global Data_0819749e
Data_0819749e:
	.incbin "baserom.gba", 0x0019749e, 0x0000000f
	.global Data_081974ad
Data_081974ad:
	.incbin "baserom.gba", 0x001974ad, 0x0000000f
	.global Data_081974bc
Data_081974bc:
	.incbin "baserom.gba", 0x001974bc, 0x00000020
	.global Data_081974dc
Data_081974dc:
	.incbin "baserom.gba", 0x001974dc, 0x00000018
	.global Data_081974f4
Data_081974f4:
	.incbin "baserom.gba", 0x001974f4, 0x0000000c
	.global Data_08197500
Data_08197500:
	.incbin "baserom.gba", 0x00197500, 0x0000000c
	.global Data_0819750c
Data_0819750c:
	.incbin "baserom.gba", 0x0019750c, 0x0000000e
	.global Data_0819751a
Data_0819751a:
	.incbin "baserom.gba", 0x0019751a, 0x00000007
	.global Data_08197521
Data_08197521:
	.incbin "baserom.gba", 0x00197521, 0x00000006
	.global Data_08197527
Data_08197527:
	.incbin "baserom.gba", 0x00197527, 0x0000000c
	.global Data_08197533
Data_08197533:
	.incbin "baserom.gba", 0x00197533, 0x00000014
	.global Data_08197547
Data_08197547:
	.incbin "baserom.gba", 0x00197547, 0x00000008
	.global Data_0819754f
Data_0819754f:
	.incbin "baserom.gba", 0x0019754f, 0x00000093
	.global Data_081975e2
Data_081975e2:
	.incbin "baserom.gba", 0x001975e2, 0x00000006
	.global Data_081975e8
Data_081975e8:
	.incbin "baserom.gba", 0x001975e8, 0x00000006
	.global Data_081975ee
Data_081975ee:
	.incbin "baserom.gba", 0x001975ee, 0x0000000c
	.global Data_081975fa
Data_081975fa:
	.incbin "baserom.gba", 0x001975fa, 0x00000006
	.global Data_08197600
Data_08197600:
	.incbin "baserom.gba", 0x00197600, 0x00000005
	.global Data_08197605
Data_08197605:
	.incbin "baserom.gba", 0x00197605, 0x00000004
	.global Data_08197609
Data_08197609:
	.incbin "baserom.gba", 0x00197609, 0x00000021
	.global Data_0819762a
Data_0819762a:
	.incbin "baserom.gba", 0x0019762a, 0x00000022
	.global Data_0819764c
Data_0819764c:
	.incbin "baserom.gba", 0x0019764c, 0x00000044
	.global Data_08197690
Data_08197690:
	.incbin "baserom.gba", 0x00197690, 0x00000021
	.global Data_081976b1
Data_081976b1:
	.incbin "baserom.gba", 0x001976b1, 0x00000021
	.global Data_081976d2
Data_081976d2:
	.incbin "baserom.gba", 0x001976d2, 0x00000004
	.global Data_081976d6
Data_081976d6:
	.incbin "baserom.gba", 0x001976d6, 0x00000004
	.global Data_081976da
Data_081976da:
	.incbin "baserom.gba", 0x001976da, 0x00000006
	.global Data_081976e0
Data_081976e0:
	.incbin "baserom.gba", 0x001976e0, 0x00000010
	.global Data_081976f0
Data_081976f0:
	.incbin "baserom.gba", 0x001976f0, 0x00000008
	.global Data_081976f8
Data_081976f8:
	.incbin "baserom.gba", 0x001976f8, 0x00000008
	.global Data_08197700
Data_08197700:
	.incbin "baserom.gba", 0x00197700, 0x00000002
	.global Data_08197702
Data_08197702:
	.incbin "baserom.gba", 0x00197702, 0x00000002
	.global Data_08197704
Data_08197704:
	.incbin "baserom.gba", 0x00197704, 0x00000006
	.global Data_0819770a
Data_0819770a:
	.incbin "baserom.gba", 0x0019770a, 0x00000006
	.global Data_08197710
Data_08197710:
	.incbin "baserom.gba", 0x00197710, 0x00000006
	.global Data_08197716
Data_08197716:
	.incbin "baserom.gba", 0x00197716, 0x00000006
	.global Data_0819771c
Data_0819771c:
	.incbin "baserom.gba", 0x0019771c, 0x00000004
	.global Data_08197720
Data_08197720:
	.incbin "baserom.gba", 0x00197720, 0x00000004
	.global Data_08197724
Data_08197724:
	.incbin "baserom.gba", 0x00197724, 0x00000004
	.global Data_08197728
Data_08197728:
	.incbin "baserom.gba", 0x00197728, 0x00000004
	.global Data_0819772c
Data_0819772c:
	.incbin "baserom.gba", 0x0019772c, 0x00000004
	.global Data_08197730
Data_08197730:
	.incbin "baserom.gba", 0x00197730, 0x00000004
	.global Data_08197734
Data_08197734:
	.incbin "baserom.gba", 0x00197734, 0x00000010
	.global Data_08197744
Data_08197744:
	.incbin "baserom.gba", 0x00197744, 0x00000004
	.global Data_08197748
Data_08197748:
	.incbin "baserom.gba", 0x00197748, 0x00000007
	.global Data_0819774f
Data_0819774f:
	.incbin "baserom.gba", 0x0019774f, 0x00000011
	.global Data_08197760
Data_08197760:
	.incbin "baserom.gba", 0x00197760, 0x00000044
	.global Data_081977a4
Data_081977a4:
	.incbin "baserom.gba", 0x001977a4, 0x00000020
	.global Data_081977c4
Data_081977c4:
	.incbin "baserom.gba", 0x001977c4, 0x00000020
	.global Data_081977e4
Data_081977e4:
	.incbin "baserom.gba", 0x001977e4, 0x00000004
	.global Data_081977e8
Data_081977e8:
	.incbin "baserom.gba", 0x001977e8, 0x00000004
	.global Data_081977ec
Data_081977ec:
	.incbin "baserom.gba", 0x001977ec, 0x00000004
	.global Data_081977f0
Data_081977f0:
	.incbin "baserom.gba", 0x001977f0, 0x00000004
	.global Data_081977f4
Data_081977f4:
	.incbin "baserom.gba", 0x001977f4, 0x00000004
	.global Data_081977f8
Data_081977f8:
	.incbin "baserom.gba", 0x001977f8, 0x00000022
	.global Data_0819781a
Data_0819781a:
	.incbin "baserom.gba", 0x0019781a, 0x0000000c
	.global Data_08197826
Data_08197826:
	.incbin "baserom.gba", 0x00197826, 0x0000000e
	.global Data_08197834
Data_08197834:
	.incbin "baserom.gba", 0x00197834, 0x00000030
	.global Data_08197864
Data_08197864:
	.incbin "baserom.gba", 0x00197864, 0x00000008
	.global Data_0819786c
Data_0819786c:
	.incbin "baserom.gba", 0x0019786c, 0x00000008
	.global Data_08197874
Data_08197874:
	.incbin "baserom.gba", 0x00197874, 0x00000006
	.global Data_0819787a
Data_0819787a:
	.incbin "baserom.gba", 0x0019787a, 0x00000003
	.global Data_0819787d
Data_0819787d:
	.incbin "baserom.gba", 0x0019787d, 0x00000003
	.global Data_08197880
Data_08197880:
	.incbin "baserom.gba", 0x00197880, 0x00000028
	.global Data_081978a8
Data_081978a8:
	.incbin "baserom.gba", 0x001978a8, 0x00000012
	.global Data_081978ba
Data_081978ba:
	.incbin "baserom.gba", 0x001978ba, 0x00000012
	.global Data_081978cc
Data_081978cc:
	.incbin "baserom.gba", 0x001978cc, 0x00000024
	.global Data_081978f0
Data_081978f0:
	.incbin "baserom.gba", 0x001978f0, 0x0000000e
	.global Data_081978fe
Data_081978fe:
	.incbin "baserom.gba", 0x001978fe, 0x0000000e
	.global Data_0819790c
Data_0819790c:
	.incbin "baserom.gba", 0x0019790c, 0x0000000c
	.global Data_08197918
Data_08197918:
	.incbin "baserom.gba", 0x00197918, 0x0000000c
	.global Data_08197924
Data_08197924:
	.incbin "baserom.gba", 0x00197924, 0x00000018
	.global Data_0819793c
Data_0819793c:
	.incbin "baserom.gba", 0x0019793c, 0x00000002
	.global Data_0819793e
Data_0819793e:
	.incbin "baserom.gba", 0x0019793e, 0x00000009
	.global Data_08197947
Data_08197947:
	.incbin "baserom.gba", 0x00197947, 0x00000009
	.global Data_08197950
Data_08197950:
	.incbin "baserom.gba", 0x00197950, 0x00000008
	.global Data_08197958
Data_08197958:
	.incbin "baserom.gba", 0x00197958, 0x00000003
	.global Data_0819795b
Data_0819795b:
	.incbin "baserom.gba", 0x0019795b, 0x00000003
	.global Data_0819795e
Data_0819795e:
	.incbin "baserom.gba", 0x0019795e, 0x00000004
	.global Data_08197962
Data_08197962:
	.incbin "baserom.gba", 0x00197962, 0x00000006
	.global Data_08197968
Data_08197968:
	.incbin "baserom.gba", 0x00197968, 0x00000004
	.global Data_0819796c
Data_0819796c:
	.incbin "baserom.gba", 0x0019796c, 0x00000002
	.global Data_0819796e
Data_0819796e:
	.incbin "baserom.gba", 0x0019796e, 0x00000004
	.global Data_08197972
Data_08197972:
	.incbin "baserom.gba", 0x00197972, 0x00000006
	.global Data_08197978
Data_08197978:
	.incbin "baserom.gba", 0x00197978, 0x00000006
	.global Data_0819797e
Data_0819797e:
	.incbin "baserom.gba", 0x0019797e, 0x00000006
	.global Data_08197984
Data_08197984:
	.incbin "baserom.gba", 0x00197984, 0x0000000c
	.global Data_08197990
Data_08197990:
	.incbin "baserom.gba", 0x00197990, 0x00000008
	.global Data_08197998
Data_08197998:
	.incbin "baserom.gba", 0x00197998, 0x00000010
	.global Data_081979a8
Data_081979a8:
	.incbin "baserom.gba", 0x001979a8, 0x00000006
	.global Data_081979ae
Data_081979ae:
	.incbin "baserom.gba", 0x001979ae, 0x00000009
	.global Data_081979b7
Data_081979b7:
	.incbin "baserom.gba", 0x001979b7, 0x00000009
	.global Data_081979c0
Data_081979c0:
	.incbin "baserom.gba", 0x001979c0, 0x0000000c
	.global Data_081979cc
Data_081979cc:
	.incbin "baserom.gba", 0x001979cc, 0x0000000e
	.global Data_081979da
Data_081979da:
	.incbin "baserom.gba", 0x001979da, 0x00000004
	.global Data_081979de
Data_081979de:
	.incbin "baserom.gba", 0x001979de, 0x0000003c
	.global Data_08197a1a
Data_08197a1a:
	.incbin "baserom.gba", 0x00197a1a, 0x00000006
	.global Data_08197a20
Data_08197a20:
	.incbin "baserom.gba", 0x00197a20, 0x00000003
	.global Data_08197a23
Data_08197a23:
	.incbin "baserom.gba", 0x00197a23, 0x0000000c
	.global Data_08197a2f
Data_08197a2f:
	.incbin "baserom.gba", 0x00197a2f, 0x00000005
	.global Data_08197a34
Data_08197a34:
	.incbin "baserom.gba", 0x00197a34, 0x00000004
	.global Data_08197a38
Data_08197a38:
	.incbin "baserom.gba", 0x00197a38, 0x0000002c
	.global Data_08197a64
Data_08197a64:
	.incbin "baserom.gba", 0x00197a64, 0x00000030
	.global Data_08197a94
Data_08197a94:
	.incbin "baserom.gba", 0x00197a94, 0x00000004
	.global Data_08197a98
Data_08197a98:
	.incbin "baserom.gba", 0x00197a98, 0x00000004
	.global Data_08197a9c
Data_08197a9c:
	.incbin "baserom.gba", 0x00197a9c, 0x00000004
	.global Data_08197aa0
Data_08197aa0:
	.incbin "baserom.gba", 0x00197aa0, 0x0000075c
	.global Data_081981fc
Data_081981fc:
	.incbin "baserom.gba", 0x001981fc, 0x00000006
	.global Data_08198202
Data_08198202:
	.incbin "baserom.gba", 0x00198202, 0x0000000a
	.global Data_0819820c
Data_0819820c:
	.incbin "baserom.gba", 0x0019820c, 0x00000005
	.global Data_08198211
Data_08198211:
	.incbin "baserom.gba", 0x00198211, 0x00000005
	.global Data_08198216
Data_08198216:
	.incbin "baserom.gba", 0x00198216, 0x00000006
	.global Data_0819821c
Data_0819821c:
	.incbin "baserom.gba", 0x0019821c, 0x00000004
	.global Data_08198220
Data_08198220:
	.incbin "baserom.gba", 0x00198220, 0x0000000a
	.global Data_0819822a
Data_0819822a:
	.incbin "baserom.gba", 0x0019822a, 0x00000005
	.global Data_0819822f
Data_0819822f:
	.incbin "baserom.gba", 0x0019822f, 0x00000005
	.global Data_08198234
Data_08198234:
	.incbin "baserom.gba", 0x00198234, 0x0000000a
	.global Data_0819823e
Data_0819823e:
	.incbin "baserom.gba", 0x0019823e, 0x00000006
	.global Data_08198244
Data_08198244:
	.incbin "baserom.gba", 0x00198244, 0x0000000e
	.global Data_08198252
Data_08198252:
	.incbin "baserom.gba", 0x00198252, 0x0000000e
	.global Data_08198260
Data_08198260:
	.incbin "baserom.gba", 0x00198260, 0x00000020
	.global Data_08198280
Data_08198280:
	.incbin "baserom.gba", 0x00198280, 0x00000004
	.global Data_08198284
Data_08198284:
	.incbin "baserom.gba", 0x00198284, 0x00000006
	.global Data_0819828a
Data_0819828a:
	.incbin "baserom.gba", 0x0019828a, 0x00000003
	.global Data_0819828d
Data_0819828d:
	.incbin "baserom.gba", 0x0019828d, 0x00000003
	.global Data_08198290
Data_08198290:
	.incbin "baserom.gba", 0x00198290, 0x00000004
	.global Data_08198294
Data_08198294:
	.incbin "baserom.gba", 0x00198294, 0x0000000b
	.global Data_0819829f
Data_0819829f:
	.incbin "baserom.gba", 0x0019829f, 0x0000000b
	.global Data_081982aa
Data_081982aa:
	.incbin "baserom.gba", 0x001982aa, 0x00000016
	.global Data_081982c0
Data_081982c0:
	.incbin "baserom.gba", 0x001982c0, 0x00000003
	.global Data_081982c3
Data_081982c3:
	.incbin "baserom.gba", 0x001982c3, 0x00000003
	.global Data_081982c6
Data_081982c6:
	.incbin "baserom.gba", 0x001982c6, 0x00000006
	.global Data_081982cc
Data_081982cc:
	.incbin "baserom.gba", 0x001982cc, 0x00000006
	.global Data_081982d2
Data_081982d2:
	.incbin "baserom.gba", 0x001982d2, 0x00000003
	.global Data_081982d5
Data_081982d5:
	.incbin "baserom.gba", 0x001982d5, 0x00000003
	.global Data_081982d8
Data_081982d8:
	.incbin "baserom.gba", 0x001982d8, 0x00000003
	.global Data_081982db
Data_081982db:
	.incbin "baserom.gba", 0x001982db, 0x00000003
	.global Data_081982de
Data_081982de:
	.incbin "baserom.gba", 0x001982de, 0x00000006
	.global Data_081982e4
Data_081982e4:
	.incbin "baserom.gba", 0x001982e4, 0x00000010
	.global Data_081982f4
Data_081982f4:
	.incbin "baserom.gba", 0x001982f4, 0x00000003
	.global Data_081982f7
Data_081982f7:
	.incbin "baserom.gba", 0x001982f7, 0x00000009
	.global Data_08198300
Data_08198300:
	.incbin "baserom.gba", 0x00198300, 0x00000003
	.global Data_08198303
Data_08198303:
	.incbin "baserom.gba", 0x00198303, 0x00000006
	.global Data_08198309
Data_08198309:
	.incbin "baserom.gba", 0x00198309, 0x00000006
	.global Data_0819830f
Data_0819830f:
	.incbin "baserom.gba", 0x0019830f, 0x00000007
	.global Data_08198316
Data_08198316:
	.incbin "baserom.gba", 0x00198316, 0x0000000c
	.global Data_08198322
Data_08198322:
	.incbin "baserom.gba", 0x00198322, 0x00000026
	.global Data_08198348
Data_08198348:
	.incbin "baserom.gba", 0x00198348, 0x00000009
	.global Data_08198351
Data_08198351:
	.incbin "baserom.gba", 0x00198351, 0x00000008
	.global Data_08198359
Data_08198359:
	.incbin "baserom.gba", 0x00198359, 0x00000009
	.global Data_08198362
Data_08198362:
	.incbin "baserom.gba", 0x00198362, 0x00000010
	.global Data_08198372
Data_08198372:
	.incbin "baserom.gba", 0x00198372, 0x00000006
	.global Data_08198378
Data_08198378:
	.incbin "baserom.gba", 0x00198378, 0x00000003
	.global Data_0819837b
Data_0819837b:
	.incbin "baserom.gba", 0x0019837b, 0x00000008
	.global Data_08198383
Data_08198383:
	.incbin "baserom.gba", 0x00198383, 0x00000009
	.global Data_0819838c
Data_0819838c:
	.incbin "baserom.gba", 0x0019838c, 0x00000010
	.global Data_0819839c
Data_0819839c:
	.incbin "baserom.gba", 0x0019839c, 0x00000006
	.global Data_081983a2
Data_081983a2:
	.incbin "baserom.gba", 0x001983a2, 0x0000000a
	.global Data_081983ac
Data_081983ac:
	.incbin "baserom.gba", 0x001983ac, 0x0000000e
	.global Data_081983ba
Data_081983ba:
	.incbin "baserom.gba", 0x001983ba, 0x0000000e
	.global Data_081983c8
Data_081983c8:
	.incbin "baserom.gba", 0x001983c8, 0x00000004
	.global Data_081983cc
Data_081983cc:
	.incbin "baserom.gba", 0x001983cc, 0x00000008
	.global Data_081983d4
Data_081983d4:
	.incbin "baserom.gba", 0x001983d4, 0x00000003
	.global Data_081983d7
Data_081983d7:
	.incbin "baserom.gba", 0x001983d7, 0x00000003
	.global Data_081983da
Data_081983da:
	.incbin "baserom.gba", 0x001983da, 0x00000006
	.global Data_081983e0
Data_081983e0:
	.incbin "baserom.gba", 0x001983e0, 0x00000004
	.global Data_081983e4
Data_081983e4:
	.incbin "baserom.gba", 0x001983e4, 0x00000006
	.global Data_081983ea
Data_081983ea:
	.incbin "baserom.gba", 0x001983ea, 0x00000003
	.global Data_081983ed
Data_081983ed:
	.incbin "baserom.gba", 0x001983ed, 0x00000010
	.global Data_081983fd
Data_081983fd:
	.incbin "baserom.gba", 0x001983fd, 0x00000008
	.global Data_08198405
Data_08198405:
	.incbin "baserom.gba", 0x00198405, 0x00000007
	.global Data_0819840c
Data_0819840c:
	.incbin "baserom.gba", 0x0019840c, 0x00000008
	.global Data_08198414
Data_08198414:
	.incbin "baserom.gba", 0x00198414, 0x0000000e
	.global Data_08198422
Data_08198422:
	.incbin "baserom.gba", 0x00198422, 0x00000010
	.global Data_08198432
Data_08198432:
	.incbin "baserom.gba", 0x00198432, 0x00000008
	.global Data_0819843a
Data_0819843a:
	.incbin "baserom.gba", 0x0019843a, 0x00000008
	.global Data_08198442
Data_08198442:
	.incbin "baserom.gba", 0x00198442, 0x00000003
	.global Data_08198445
Data_08198445:
	.incbin "baserom.gba", 0x00198445, 0x00000007
	.global Data_0819844c
Data_0819844c:
	.incbin "baserom.gba", 0x0019844c, 0x00000008
	.global Data_08198454
Data_08198454:
	.incbin "baserom.gba", 0x00198454, 0x0000000e
	.global Data_08198462
Data_08198462:
	.incbin "baserom.gba", 0x00198462, 0x0000000c
	.global Data_0819846e
Data_0819846e:
	.incbin "baserom.gba", 0x0019846e, 0x00000004
	.global Data_08198472
Data_08198472:
	.incbin "baserom.gba", 0x00198472, 0x00000003
	.global Data_08198475
Data_08198475:
	.incbin "baserom.gba", 0x00198475, 0x00000003
	.global Data_08198478
Data_08198478:
	.incbin "baserom.gba", 0x00198478, 0x0000005f
	.global Data_081984d7
Data_081984d7:
	.incbin "baserom.gba", 0x001984d7, 0x00000007
	.global Data_081984de
Data_081984de:
	.incbin "baserom.gba", 0x001984de, 0x00000007
	.global Data_081984e5
Data_081984e5:
	.incbin "baserom.gba", 0x001984e5, 0x00000007
	.global Data_081984ec
Data_081984ec:
	.incbin "baserom.gba", 0x001984ec, 0x00000008
	.global Data_081984f4
Data_081984f4:
	.incbin "baserom.gba", 0x001984f4, 0x0000000e
	.global Data_08198502
Data_08198502:
	.incbin "baserom.gba", 0x00198502, 0x00000008
	.global Data_0819850a
Data_0819850a:
	.incbin "baserom.gba", 0x0019850a, 0x00000004
	.global Data_0819850e
Data_0819850e:
	.incbin "baserom.gba", 0x0019850e, 0x00000005
	.global Data_08198513
Data_08198513:
	.incbin "baserom.gba", 0x00198513, 0x00000004
	.global Data_08198517
Data_08198517:
	.incbin "baserom.gba", 0x00198517, 0x00000005
	.global Data_0819851c
Data_0819851c:
	.incbin "baserom.gba", 0x0019851c, 0x00000008
	.global Data_08198524
Data_08198524:
	.incbin "baserom.gba", 0x00198524, 0x00000004
	.global Data_08198528
Data_08198528:
	.incbin "baserom.gba", 0x00198528, 0x00000009
	.global Data_08198531
Data_08198531:
	.incbin "baserom.gba", 0x00198531, 0x00000009
	.global Data_0819853a
Data_0819853a:
	.incbin "baserom.gba", 0x0019853a, 0x00000012
	.global Data_0819854c
Data_0819854c:
	.incbin "baserom.gba", 0x0019854c, 0x00000009
	.global Data_08198555
Data_08198555:
	.incbin "baserom.gba", 0x00198555, 0x00000009
	.global Data_0819855e
Data_0819855e:
	.incbin "baserom.gba", 0x0019855e, 0x00000004
	.global Data_08198562
Data_08198562:
	.incbin "baserom.gba", 0x00198562, 0x00000004
	.global Data_08198566
Data_08198566:
	.incbin "baserom.gba", 0x00198566, 0x00000040
	.global Data_081985a6
Data_081985a6:
	.incbin "baserom.gba", 0x001985a6, 0x00000005
	.global Data_081985ab
Data_081985ab:
	.incbin "baserom.gba", 0x001985ab, 0x00000005
	.global Data_081985b0
Data_081985b0:
	.incbin "baserom.gba", 0x001985b0, 0x00000003
	.global Data_081985b3
Data_081985b3:
	.incbin "baserom.gba", 0x001985b3, 0x00000068
	.global Data_0819861b
Data_0819861b:
	.incbin "baserom.gba", 0x0019861b, 0x00000005
	.global Data_08198620
Data_08198620:
	.incbin "baserom.gba", 0x00198620, 0x00000012
	.global Data_08198632
Data_08198632:
	.incbin "baserom.gba", 0x00198632, 0x0000000a
	.global Data_0819863c
Data_0819863c:
	.incbin "baserom.gba", 0x0019863c, 0x00000006
	.global Data_08198642
Data_08198642:
	.incbin "baserom.gba", 0x00198642, 0x00000003
	.global Data_08198645
Data_08198645:
	.incbin "baserom.gba", 0x00198645, 0x00000003
	.global Data_08198648
Data_08198648:
	.incbin "baserom.gba", 0x00198648, 0x00000006
	.global Data_0819864e
Data_0819864e:
	.incbin "baserom.gba", 0x0019864e, 0x00000006
	.global Data_08198654
Data_08198654:
	.incbin "baserom.gba", 0x00198654, 0x00000007
	.global Data_0819865b
Data_0819865b:
	.incbin "baserom.gba", 0x0019865b, 0x00000007
	.global Data_08198662
Data_08198662:
	.incbin "baserom.gba", 0x00198662, 0x00000012
	.global Data_08198674
Data_08198674:
	.incbin "baserom.gba", 0x00198674, 0x00000012
	.global Data_08198686
Data_08198686:
	.incbin "baserom.gba", 0x00198686, 0x00000024
	.global Data_081986aa
Data_081986aa:
	.incbin "baserom.gba", 0x001986aa, 0x0000001e
	.global Data_081986c8
Data_081986c8:
	.incbin "baserom.gba", 0x001986c8, 0x00000006
	.global Data_081986ce
Data_081986ce:
	.incbin "baserom.gba", 0x001986ce, 0x00000006
	.global Data_081986d4
Data_081986d4:
	.incbin "baserom.gba", 0x001986d4, 0x0000000c
	.global Data_081986e0
Data_081986e0:
	.incbin "baserom.gba", 0x001986e0, 0x0000000c
	.global Data_081986ec
Data_081986ec:
	.incbin "baserom.gba", 0x001986ec, 0x00000006
	.global Data_081986f2
Data_081986f2:
	.incbin "baserom.gba", 0x001986f2, 0x00000136
	.global Data_08198828
Data_08198828:
	.incbin "baserom.gba", 0x00198828, 0x00000008
	.global Data_08198830
Data_08198830:
	.incbin "baserom.gba", 0x00198830, 0x0000000c
	.global Data_0819883c
Data_0819883c:
	.incbin "baserom.gba", 0x0019883c, 0x0000000c
	.global Data_08198848
Data_08198848:
	.incbin "baserom.gba", 0x00198848, 0x00000008
	.global Data_08198850
Data_08198850:
	.incbin "baserom.gba", 0x00198850, 0x00000008
	.global Data_08198858
Data_08198858:
	.incbin "baserom.gba", 0x00198858, 0x00000008
	.global Data_08198860
Data_08198860:
	.incbin "baserom.gba", 0x00198860, 0x00000006
	.global Data_08198866
Data_08198866:
	.incbin "baserom.gba", 0x00198866, 0x00000006
	.global Data_0819886c
Data_0819886c:
	.incbin "baserom.gba", 0x0019886c, 0x0000000c
	.global Data_08198878
Data_08198878:
	.incbin "baserom.gba", 0x00198878, 0x00000006
	.global Data_0819887e
Data_0819887e:
	.incbin "baserom.gba", 0x0019887e, 0x00000004
	.global Data_08198882
Data_08198882:
	.incbin "baserom.gba", 0x00198882, 0x00000004
	.global Data_08198886
Data_08198886:
	.incbin "baserom.gba", 0x00198886, 0x00000004
	.global Data_0819888a
Data_0819888a:
	.incbin "baserom.gba", 0x0019888a, 0x00000014
	.global Data_0819889e
Data_0819889e:
	.incbin "baserom.gba", 0x0019889e, 0x00000014
	.global Data_081988b2
Data_081988b2:
	.incbin "baserom.gba", 0x001988b2, 0x00000014
	.global Data_081988c6
Data_081988c6:
	.incbin "baserom.gba", 0x001988c6, 0x00000014
	.global Data_081988da
Data_081988da:
	.incbin "baserom.gba", 0x001988da, 0x00000014
	.global Data_081988ee
Data_081988ee:
	.incbin "baserom.gba", 0x001988ee, 0x0000000e
	.global Data_081988fc
Data_081988fc:
	.incbin "baserom.gba", 0x001988fc, 0x0000000e
	.global Data_0819890a
Data_0819890a:
	.incbin "baserom.gba", 0x0019890a, 0x0000000e
	.global Data_08198918
Data_08198918:
	.incbin "baserom.gba", 0x00198918, 0x0000000e
	.global Data_08198926
Data_08198926:
	.incbin "baserom.gba", 0x00198926, 0x0000000e
	.global Data_08198934
Data_08198934:
	.incbin "baserom.gba", 0x00198934, 0x00000006
	.global Data_0819893a
Data_0819893a:
	.incbin "baserom.gba", 0x0019893a, 0x00000003
	.global Data_0819893d
Data_0819893d:
	.incbin "baserom.gba", 0x0019893d, 0x00000003
	.global Data_08198940
Data_08198940:
	.incbin "baserom.gba", 0x00198940, 0x00000006
	.global Data_08198946
Data_08198946:
	.incbin "baserom.gba", 0x00198946, 0x00000008
	.global Data_0819894e
Data_0819894e:
	.incbin "baserom.gba", 0x0019894e, 0x00000004
	.global Data_08198952
Data_08198952:
	.incbin "baserom.gba", 0x00198952, 0x00000004
	.global Data_08198956
Data_08198956:
	.incbin "baserom.gba", 0x00198956, 0x00000004
	.global Data_0819895a
Data_0819895a:
	.incbin "baserom.gba", 0x0019895a, 0x00000004
	.global Data_0819895e
Data_0819895e:
	.incbin "baserom.gba", 0x0019895e, 0x00000004
	.global Data_08198962
Data_08198962:
	.incbin "baserom.gba", 0x00198962, 0x00000004
	.global Data_08198966
Data_08198966:
	.incbin "baserom.gba", 0x00198966, 0x00000004
	.global Data_0819896a
Data_0819896a:
	.incbin "baserom.gba", 0x0019896a, 0x0000000a
	.global Data_08198974
Data_08198974:
	.incbin "baserom.gba", 0x00198974, 0x00000010
	.global Data_08198984
Data_08198984:
	.incbin "baserom.gba", 0x00198984, 0x0000000c
	.global Data_08198990
Data_08198990:
	.incbin "baserom.gba", 0x00198990, 0x0000000c
	.global Data_0819899c
Data_0819899c:
	.incbin "baserom.gba", 0x0019899c, 0x00000008
	.global Data_081989a4
Data_081989a4:
	.incbin "baserom.gba", 0x001989a4, 0x00000008
	.global Data_081989ac
Data_081989ac:
	.incbin "baserom.gba", 0x001989ac, 0x00000008
	.global Data_081989b4
Data_081989b4:
	.incbin "baserom.gba", 0x001989b4, 0x00000008
	.global Data_081989bc
Data_081989bc:
	.incbin "baserom.gba", 0x001989bc, 0x00000008
	.global Data_081989c4
Data_081989c4:
	.incbin "baserom.gba", 0x001989c4, 0x00000008
	.global Data_081989cc
Data_081989cc:
	.incbin "baserom.gba", 0x001989cc, 0x0000000b
	.global Data_081989d7
Data_081989d7:
	.incbin "baserom.gba", 0x001989d7, 0x0000000b
	.global Data_081989e2
Data_081989e2:
	.incbin "baserom.gba", 0x001989e2, 0x00000008
	.global Data_081989ea
Data_081989ea:
	.incbin "baserom.gba", 0x001989ea, 0x00000008
	.global Data_081989f2
Data_081989f2:
	.incbin "baserom.gba", 0x001989f2, 0x00000010
	.global Data_08198a02
Data_08198a02:
	.incbin "baserom.gba", 0x00198a02, 0x0000002a
	.global Data_08198a2c
Data_08198a2c:
	.incbin "baserom.gba", 0x00198a2c, 0x0000001c
	.global Data_08198a48
Data_08198a48:
	.incbin "baserom.gba", 0x00198a48, 0x0000000e
	.global Data_08198a56
Data_08198a56:
	.incbin "baserom.gba", 0x00198a56, 0x0000000e
	.global Data_08198a64
Data_08198a64:
	.incbin "baserom.gba", 0x00198a64, 0x00000009
	.global Data_08198a6d
Data_08198a6d:
	.incbin "baserom.gba", 0x00198a6d, 0x00000009
	.global Data_08198a76
Data_08198a76:
	.incbin "baserom.gba", 0x00198a76, 0x0000000e
	.global Data_08198a84
Data_08198a84:
	.incbin "baserom.gba", 0x00198a84, 0x0000000e
	.global Data_08198a92
Data_08198a92:
	.incbin "baserom.gba", 0x00198a92, 0x00000006
	.global Data_08198a98
Data_08198a98:
	.incbin "baserom.gba", 0x00198a98, 0x00000006
	.global Data_08198a9e
Data_08198a9e:
	.incbin "baserom.gba", 0x00198a9e, 0x00000006
	.global Data_08198aa4
Data_08198aa4:
	.incbin "baserom.gba", 0x00198aa4, 0x00000010
	.global Data_08198ab4
Data_08198ab4:
	.incbin "baserom.gba", 0x00198ab4, 0x0000000e
	.global Data_08198ac2
Data_08198ac2:
	.incbin "baserom.gba", 0x00198ac2, 0x0000000c
	.global Data_08198ace
Data_08198ace:
	.incbin "baserom.gba", 0x00198ace, 0x00000006
	.global Data_08198ad4
Data_08198ad4:
	.incbin "baserom.gba", 0x00198ad4, 0x00000006
	.global Data_08198ada
Data_08198ada:
	.incbin "baserom.gba", 0x00198ada, 0x00000009
	.global Data_08198ae3
Data_08198ae3:
	.incbin "baserom.gba", 0x00198ae3, 0x00000009
	.global Data_08198aec
Data_08198aec:
	.incbin "baserom.gba", 0x00198aec, 0x00000010
	.global Data_08198afc
Data_08198afc:
	.incbin "baserom.gba", 0x00198afc, 0x0000000e
	.global Data_08198b0a
Data_08198b0a:
	.incbin "baserom.gba", 0x00198b0a, 0x0000001a
	.global Data_08198b24
Data_08198b24:
	.incbin "baserom.gba", 0x00198b24, 0x00000002
	.global Data_08198b26
Data_08198b26:
	.incbin "baserom.gba", 0x00198b26, 0x00000004
	.global Data_08198b2a
Data_08198b2a:
	.incbin "baserom.gba", 0x00198b2a, 0x00000004
	.global Data_08198b2e
Data_08198b2e:
	.incbin "baserom.gba", 0x00198b2e, 0x00000004
	.global Data_08198b32
Data_08198b32:
	.incbin "baserom.gba", 0x00198b32, 0x00000010
	.global Data_08198b42
Data_08198b42:
	.incbin "baserom.gba", 0x00198b42, 0x00000010
	.global Data_08198b52
Data_08198b52:
	.incbin "baserom.gba", 0x00198b52, 0x00000004
	.global Data_08198b56
Data_08198b56:
	.incbin "baserom.gba", 0x00198b56, 0x00000004
	.global Data_08198b5a
Data_08198b5a:
	.incbin "baserom.gba", 0x00198b5a, 0x00000004
	.global Data_08198b5e
Data_08198b5e:
	.incbin "baserom.gba", 0x00198b5e, 0x0000000e
	.global Data_08198b6c
Data_08198b6c:
	.incbin "baserom.gba", 0x00198b6c, 0x0000000e
	.global Data_08198b7a
Data_08198b7a:
	.incbin "baserom.gba", 0x00198b7a, 0x00000008
	.global Data_08198b82
Data_08198b82:
	.incbin "baserom.gba", 0x00198b82, 0x00000008
	.global Data_08198b8a
Data_08198b8a:
	.incbin "baserom.gba", 0x00198b8a, 0x00000004
	.global Data_08198b8e
Data_08198b8e:
	.incbin "baserom.gba", 0x00198b8e, 0x00000004
	.global Data_08198b92
Data_08198b92:
	.incbin "baserom.gba", 0x00198b92, 0x00000004
	.global Data_08198b96
Data_08198b96:
	.incbin "baserom.gba", 0x00198b96, 0x00000002
	.global Data_08198b98
Data_08198b98:
	.incbin "baserom.gba", 0x00198b98, 0x00000002
	.global Data_08198b9a
Data_08198b9a:
	.incbin "baserom.gba", 0x00198b9a, 0x00000002
	.global Data_08198b9c
Data_08198b9c:
	.incbin "baserom.gba", 0x00198b9c, 0x00000014
	.global Data_08198bb0
Data_08198bb0:
	.incbin "baserom.gba", 0x00198bb0, 0x0000000a
	.global Data_08198bba
Data_08198bba:
	.incbin "baserom.gba", 0x00198bba, 0x0000000a
	.global Data_08198bc4
Data_08198bc4:
	.incbin "baserom.gba", 0x00198bc4, 0x0000000a
	.global Data_08198bce
Data_08198bce:
	.incbin "baserom.gba", 0x00198bce, 0x0000000e
	.global Data_08198bdc
Data_08198bdc:
	.incbin "baserom.gba", 0x00198bdc, 0x0000000e
	.global Data_08198bea
Data_08198bea:
	.incbin "baserom.gba", 0x00198bea, 0x00000004
	.global Data_08198bee
Data_08198bee:
	.incbin "baserom.gba", 0x00198bee, 0x00000006
	.global Data_08198bf4
Data_08198bf4:
	.incbin "baserom.gba", 0x00198bf4, 0x00000004
	.global Data_08198bf8
Data_08198bf8:
	.incbin "baserom.gba", 0x00198bf8, 0x00000004
	.global Data_08198bfc
Data_08198bfc:
	.incbin "baserom.gba", 0x00198bfc, 0x00000010
	.global Data_08198c0c
Data_08198c0c:
	.incbin "baserom.gba", 0x00198c0c, 0x00000004
	.global Data_08198c10
Data_08198c10:
	.incbin "baserom.gba", 0x00198c10, 0x00000004
	.global Data_08198c14
Data_08198c14:
	.incbin "baserom.gba", 0x00198c14, 0x00000004
	.global Data_08198c18
Data_08198c18:
	.incbin "baserom.gba", 0x00198c18, 0x00000010
	.global Data_08198c28
Data_08198c28:
	.incbin "baserom.gba", 0x00198c28, 0x0000000c
	.global Data_08198c34
Data_08198c34:
	.incbin "baserom.gba", 0x00198c34, 0x00000030
	.global Data_08198c64
Data_08198c64:
	.incbin "baserom.gba", 0x00198c64, 0x00000003
	.global Data_08198c67
Data_08198c67:
	.incbin "baserom.gba", 0x00198c67, 0x00000005
	.global Data_08198c6c
Data_08198c6c:
	.incbin "baserom.gba", 0x00198c6c, 0x00000040
	.global Data_08198cac
Data_08198cac:
	.incbin "baserom.gba", 0x00198cac, 0x00000080
	.global Data_08198d2c
Data_08198d2c:
	.incbin "baserom.gba", 0x00198d2c, 0x000000cc
	.global Data_08198df8
Data_08198df8:
	.incbin "baserom.gba", 0x00198df8, 0x000000cc
	.global Data_08198ec4
Data_08198ec4:
	.incbin "baserom.gba", 0x00198ec4, 0x0000018c
	.global Data_08199050
Data_08199050:
	.incbin "baserom.gba", 0x00199050, 0x00000040
	.global Data_08199090
Data_08199090:
	.incbin "baserom.gba", 0x00199090, 0x00000040
	.global Data_081990d0
Data_081990d0:
	.incbin "baserom.gba", 0x001990d0, 0x000000cc
	.global Data_0819919c
Data_0819919c:
	.incbin "baserom.gba", 0x0019919c, 0x00000008
	.global Data_081991a4
Data_081991a4:
	.incbin "baserom.gba", 0x001991a4, 0x0000000c
	.global Data_081991b0
Data_081991b0:
	.incbin "baserom.gba", 0x001991b0, 0x00000010
	.global Data_081991c0
Data_081991c0:
	.incbin "baserom.gba", 0x001991c0, 0x00000010
	.global Data_081991d0
Data_081991d0:
	.incbin "baserom.gba", 0x001991d0, 0x00000010
	.global Data_081991e0
Data_081991e0:
	.incbin "baserom.gba", 0x001991e0, 0x00000010
	.global Data_081991f0
Data_081991f0:
	.incbin "baserom.gba", 0x001991f0, 0x00000010
	.global Data_08199200
Data_08199200:
	.incbin "baserom.gba", 0x00199200, 0x00000010
	.global Data_08199210
Data_08199210:
	.incbin "baserom.gba", 0x00199210, 0x00000010
	.global Data_08199220
Data_08199220:
	.incbin "baserom.gba", 0x00199220, 0x00000024
	.global Data_08199244
Data_08199244:
	.incbin "baserom.gba", 0x00199244, 0x00000024
	.global Data_08199268
Data_08199268:
	.incbin "baserom.gba", 0x00199268, 0x00000024
	.global Data_0819928c
Data_0819928c:
	.incbin "baserom.gba", 0x0019928c, 0x00000024
	.global Data_081992b0
Data_081992b0:
	.incbin "baserom.gba", 0x001992b0, 0x00000048
	.global Data_081992f8
Data_081992f8:
	.incbin "baserom.gba", 0x001992f8, 0x00000024
	.global Data_0819931c
Data_0819931c:
	.incbin "baserom.gba", 0x0019931c, 0x00000024
	.global Data_08199340
Data_08199340:
	.incbin "baserom.gba", 0x00199340, 0x00000024
	.global Data_08199364
Data_08199364:
	.incbin "baserom.gba", 0x00199364, 0x00000024
	.global Data_08199388
Data_08199388:
	.incbin "baserom.gba", 0x00199388, 0x00000048
	.global Data_081993d0
Data_081993d0:
	.incbin "baserom.gba", 0x001993d0, 0x00000004
	.global Data_081993d4
Data_081993d4:
	.incbin "baserom.gba", 0x001993d4, 0x00000004
	.global Data_081993d8
Data_081993d8:
	.incbin "baserom.gba", 0x001993d8, 0x00000008
	.global Data_081993e0
Data_081993e0:
	.incbin "baserom.gba", 0x001993e0, 0x00000008
	.global Data_081993e8
Data_081993e8:
	.incbin "baserom.gba", 0x001993e8, 0x00000008
	.global Data_081993f0
Data_081993f0:
	.incbin "baserom.gba", 0x001993f0, 0x00000010
	.global Data_08199400
Data_08199400:
	.incbin "baserom.gba", 0x00199400, 0x0000000c
	.global Data_0819940c
Data_0819940c:
	.incbin "baserom.gba", 0x0019940c, 0x00000015
	.global Data_08199421
Data_08199421:
	.incbin "baserom.gba", 0x00199421, 0x00000005
	.global Data_08199426
Data_08199426:
	.incbin "baserom.gba", 0x00199426, 0x00000008
	.global Data_0819942e
Data_0819942e:
	.incbin "baserom.gba", 0x0019942e, 0x00000008
	.global Data_08199436
Data_08199436:
	.incbin "baserom.gba", 0x00199436, 0x00000008
	.global Data_0819943e
Data_0819943e:
	.incbin "baserom.gba", 0x0019943e, 0x00000008
	.global Data_08199446
Data_08199446:
	.incbin "baserom.gba", 0x00199446, 0x00000004
	.global Data_0819944a
Data_0819944a:
	.incbin "baserom.gba", 0x0019944a, 0x00000006
	.global Data_08199450
Data_08199450:
	.incbin "baserom.gba", 0x00199450, 0x00000024
	.global Data_08199474
Data_08199474:
	.incbin "baserom.gba", 0x00199474, 0x00000010
	.global Data_08199484
Data_08199484:
	.incbin "baserom.gba", 0x00199484, 0x00000024
	.global Data_081994a8
Data_081994a8:
	.incbin "baserom.gba", 0x001994a8, 0x00000010
	.global Data_081994b8
Data_081994b8:
	.incbin "baserom.gba", 0x001994b8, 0x00000018
	.global Data_081994d0
Data_081994d0:
	.incbin "baserom.gba", 0x001994d0, 0x00000010
	.global Data_081994e0
Data_081994e0:
	.incbin "baserom.gba", 0x001994e0, 0x00000010
	.global Data_081994f0
Data_081994f0:
	.incbin "baserom.gba", 0x001994f0, 0x00000018
	.global Data_08199508
Data_08199508:
	.incbin "baserom.gba", 0x00199508, 0x0000000c
	.global Data_08199514
Data_08199514:
	.incbin "baserom.gba", 0x00199514, 0x00000014
	.global Data_08199528
Data_08199528:
	.incbin "baserom.gba", 0x00199528, 0x00000050
	.global Data_08199578
Data_08199578:
	.incbin "baserom.gba", 0x00199578, 0x0000000a
	.global Data_08199582
Data_08199582:
	.incbin "baserom.gba", 0x00199582, 0x0000000a
	.global Data_0819958c
Data_0819958c:
	.incbin "baserom.gba", 0x0019958c, 0x00000004
	.global Data_08199590
Data_08199590:
	.incbin "baserom.gba", 0x00199590, 0x00000002
	.global Data_08199592
Data_08199592:
	.incbin "baserom.gba", 0x00199592, 0x0000000a
	.global Data_0819959c
Data_0819959c:
	.incbin "baserom.gba", 0x0019959c, 0x00000030
	.global Data_081995cc
Data_081995cc:
	.incbin "baserom.gba", 0x001995cc, 0x00000020
	.global Data_081995ec
Data_081995ec:
	.incbin "baserom.gba", 0x001995ec, 0x00000007
	.global Data_081995f3
Data_081995f3:
	.incbin "baserom.gba", 0x001995f3, 0x00000015
	.global Data_08199608
Data_08199608:
	.incbin "baserom.gba", 0x00199608, 0x00000024
	.global Data_0819962c
Data_0819962c:
	.incbin "baserom.gba", 0x0019962c, 0x00000024
	.global Data_08199650
Data_08199650:
	.incbin "baserom.gba", 0x00199650, 0x00000010
	.global Data_08199660
Data_08199660:
	.incbin "baserom.gba", 0x00199660, 0x0000000c
	.global Data_0819966c
Data_0819966c:
	.incbin "baserom.gba", 0x0019966c, 0x00000003
	.global Data_0819966f
Data_0819966f:
	.incbin "baserom.gba", 0x0019966f, 0x0000000f
	.global Data_0819967e
Data_0819967e:
	.incbin "baserom.gba", 0x0019967e, 0x00000010
	.global Data_0819968e
Data_0819968e:
	.incbin "baserom.gba", 0x0019968e, 0x0000001e
	.global Data_081996ac
Data_081996ac:
	.incbin "baserom.gba", 0x001996ac, 0x0000000f
	.global Data_081996bb
Data_081996bb:
	.incbin "baserom.gba", 0x001996bb, 0x0000000f
	.global Data_081996ca
Data_081996ca:
	.incbin "baserom.gba", 0x001996ca, 0x0000001e
	.global Data_081996e8
Data_081996e8:
	.incbin "baserom.gba", 0x001996e8, 0x00000012
	.global Data_081996fa
Data_081996fa:
	.incbin "baserom.gba", 0x001996fa, 0x00000009
	.global Data_08199703
Data_08199703:
	.incbin "baserom.gba", 0x00199703, 0x00000009
	.global Data_0819970c
Data_0819970c:
	.incbin "baserom.gba", 0x0019970c, 0x00000009
	.global Data_08199715
Data_08199715:
	.incbin "baserom.gba", 0x00199715, 0x00000009
	.global Data_0819971e
Data_0819971e:
	.incbin "baserom.gba", 0x0019971e, 0x00000009
	.global Data_08199727
Data_08199727:
	.incbin "baserom.gba", 0x00199727, 0x00000009
	.global Data_08199730
Data_08199730:
	.incbin "baserom.gba", 0x00199730, 0x00000009
	.global Data_08199739
Data_08199739:
	.incbin "baserom.gba", 0x00199739, 0x00000009
	.global Data_08199742
Data_08199742:
	.incbin "baserom.gba", 0x00199742, 0x00000004
	.global Data_08199746
Data_08199746:
	.incbin "baserom.gba", 0x00199746, 0x00000008
	.global Data_0819974e
Data_0819974e:
	.incbin "baserom.gba", 0x0019974e, 0x00000006
	.global Data_08199754
Data_08199754:
	.incbin "baserom.gba", 0x00199754, 0x00000002
	.global Data_08199756
Data_08199756:
	.incbin "baserom.gba", 0x00199756, 0x00000002
	.global Data_08199758
Data_08199758:
	.incbin "baserom.gba", 0x00199758, 0x00000004
	.global Data_0819975c
Data_0819975c:
	.incbin "baserom.gba", 0x0019975c, 0x00000004
	.global Data_08199760
Data_08199760:
	.incbin "baserom.gba", 0x00199760, 0x00000004
	.global Data_08199764
Data_08199764:
	.incbin "baserom.gba", 0x00199764, 0x00000004
	.global Data_08199768
Data_08199768:
	.incbin "baserom.gba", 0x00199768, 0x00000024
	.global Data_0819978c
Data_0819978c:
	.incbin "baserom.gba", 0x0019978c, 0x00000024
	.global Data_081997b0
Data_081997b0:
	.incbin "baserom.gba", 0x001997b0, 0x0000001c
	.global Data_081997cc
Data_081997cc:
	.incbin "baserom.gba", 0x001997cc, 0x0000001c
	.global Data_081997e8
Data_081997e8:
	.incbin "baserom.gba", 0x001997e8, 0x0000000c
	.global Data_081997f4
Data_081997f4:
	.incbin "baserom.gba", 0x001997f4, 0x00000016
	.global Data_0819980a
Data_0819980a:
	.incbin "baserom.gba", 0x0019980a, 0x00000006
	.global Data_08199810
Data_08199810:
	.incbin "baserom.gba", 0x00199810, 0x00000006
	.global Data_08199816
Data_08199816:
	.incbin "baserom.gba", 0x00199816, 0x0000000e
	.global Data_08199824
Data_08199824:
	.incbin "baserom.gba", 0x00199824, 0x00000007
	.global Data_0819982b
Data_0819982b:
	.incbin "baserom.gba", 0x0019982b, 0x00000007
	.global Data_08199832
Data_08199832:
	.incbin "baserom.gba", 0x00199832, 0x00000008
	.global Data_0819983a
Data_0819983a:
	.incbin "baserom.gba", 0x0019983a, 0x00000022
	.global Data_0819985c
Data_0819985c:
	.incbin "baserom.gba", 0x0019985c, 0x00000024
	.global Data_08199880
Data_08199880:
	.incbin "baserom.gba", 0x00199880, 0x00000024
	.global Data_081998a4
Data_081998a4:
	.incbin "baserom.gba", 0x001998a4, 0x0000004e
	.global Data_081998f2
Data_081998f2:
	.incbin "baserom.gba", 0x001998f2, 0x00000024
	.global Data_08199916
Data_08199916:
	.incbin "baserom.gba", 0x00199916, 0x00000048
	.global Data_0819995e
Data_0819995e:
	.incbin "baserom.gba", 0x0019995e, 0x00000048
	.global Data_081999a6
Data_081999a6:
	.incbin "baserom.gba", 0x001999a6, 0x00000048
	.global Data_081999ee
Data_081999ee:
	.incbin "baserom.gba", 0x001999ee, 0x00000004
	.global Data_081999f2
Data_081999f2:
	.incbin "baserom.gba", 0x001999f2, 0x00000020
	.global Data_08199a12
Data_08199a12:
	.incbin "baserom.gba", 0x00199a12, 0x0000002c
	.global Data_08199a3e
Data_08199a3e:
	.incbin "baserom.gba", 0x00199a3e, 0x00000006
	.global Data_08199a44
Data_08199a44:
	.incbin "baserom.gba", 0x00199a44, 0x00000008
	.global Data_08199a4c
Data_08199a4c:
	.incbin "baserom.gba", 0x00199a4c, 0x000001a0
	.global Data_08199bec
Data_08199bec:
	.incbin "baserom.gba", 0x00199bec, 0x0000000d
	.global Data_08199bf9
Data_08199bf9:
	.incbin "baserom.gba", 0x00199bf9, 0x00000136
	.global Data_08199d2f
Data_08199d2f:
	.incbin "baserom.gba", 0x00199d2f, 0x00000002
	.global Data_08199d31
Data_08199d31:
	.incbin "baserom.gba", 0x00199d31, 0x00000007
	.global Data_08199d38
Data_08199d38:
	.incbin "baserom.gba", 0x00199d38, 0x00000008
	.global Data_08199d40
Data_08199d40:
	.incbin "baserom.gba", 0x00199d40, 0x0000000e
	.global Data_08199d4e
Data_08199d4e:
	.incbin "baserom.gba", 0x00199d4e, 0x00000007
	.global Data_08199d55
Data_08199d55:
	.incbin "baserom.gba", 0x00199d55, 0x00000007
	.global Data_08199d5c
Data_08199d5c:
	.incbin "baserom.gba", 0x00199d5c, 0x00000004
	.global Data_08199d60
Data_08199d60:
	.incbin "baserom.gba", 0x00199d60, 0x00000004
	.global Data_08199d64
Data_08199d64:
	.incbin "baserom.gba", 0x00199d64, 0x00000030
	.global Data_08199d94
Data_08199d94:
	.incbin "baserom.gba", 0x00199d94, 0x00000003
	.global Data_08199d97
Data_08199d97:
	.incbin "baserom.gba", 0x00199d97, 0x00000004
	.global Data_08199d9b
Data_08199d9b:
	.incbin "baserom.gba", 0x00199d9b, 0x00000004
	.global Data_08199d9f
Data_08199d9f:
	.incbin "baserom.gba", 0x00199d9f, 0x00000004
	.global Data_08199da3
Data_08199da3:
	.incbin "baserom.gba", 0x00199da3, 0x0000000c
	.global Data_08199daf
Data_08199daf:
	.incbin "baserom.gba", 0x00199daf, 0x0000000c
	.global Data_08199dbb
Data_08199dbb:
	.incbin "baserom.gba", 0x00199dbb, 0x0000000c
	.global Data_08199dc7
Data_08199dc7:
	.incbin "baserom.gba", 0x00199dc7, 0x00000004
	.global Data_08199dcb
Data_08199dcb:
	.incbin "baserom.gba", 0x00199dcb, 0x00000008
	.global Data_08199dd3
Data_08199dd3:
	.incbin "baserom.gba", 0x00199dd3, 0x00000008
	.global Data_08199ddb
Data_08199ddb:
	.incbin "baserom.gba", 0x00199ddb, 0x00000009
	.global Data_08199de4
Data_08199de4:
	.incbin "baserom.gba", 0x00199de4, 0x00000040
	.global Data_08199e24
Data_08199e24:
	.incbin "baserom.gba", 0x00199e24, 0x00000030
	.global Data_08199e54
Data_08199e54:
	.incbin "baserom.gba", 0x00199e54, 0x00000004
	.global Data_08199e58
Data_08199e58:
	.incbin "baserom.gba", 0x00199e58, 0x00000004
	.global Data_08199e5c
Data_08199e5c:
	.incbin "baserom.gba", 0x00199e5c, 0x00000004
	.global Data_08199e60
Data_08199e60:
	.incbin "baserom.gba", 0x00199e60, 0x00000004
	.global Data_08199e64
Data_08199e64:
	.incbin "baserom.gba", 0x00199e64, 0x00000004
	.global Data_08199e68
Data_08199e68:
	.incbin "baserom.gba", 0x00199e68, 0x00000004
	.global Data_08199e6c
Data_08199e6c:
	.incbin "baserom.gba", 0x00199e6c, 0x00000004
	.global Data_08199e70
Data_08199e70:
	.incbin "baserom.gba", 0x00199e70, 0x00000007
	.global Data_08199e77
Data_08199e77:
	.incbin "baserom.gba", 0x00199e77, 0x00000007
	.global Data_08199e7e
Data_08199e7e:
	.incbin "baserom.gba", 0x00199e7e, 0x0000000e
	.global Data_08199e8c
Data_08199e8c:
	.incbin "baserom.gba", 0x00199e8c, 0x00000004
	.global Data_08199e90
Data_08199e90:
	.incbin "baserom.gba", 0x00199e90, 0x00000004
	.global Data_08199e94
Data_08199e94:
	.incbin "baserom.gba", 0x00199e94, 0x00000004
	.global Data_08199e98
Data_08199e98:
	.incbin "baserom.gba", 0x00199e98, 0x00000004
	.global Data_08199e9c
Data_08199e9c:
	.incbin "baserom.gba", 0x00199e9c, 0x00000004
	.global Data_08199ea0
Data_08199ea0:
	.incbin "baserom.gba", 0x00199ea0, 0x00000004
	.global Data_08199ea4
Data_08199ea4:
	.incbin "baserom.gba", 0x00199ea4, 0x00000010
	.global Data_08199eb4
Data_08199eb4:
	.incbin "baserom.gba", 0x00199eb4, 0x00000008
	.global Data_08199ebc
Data_08199ebc:
	.incbin "baserom.gba", 0x00199ebc, 0x00000009
	.global Data_08199ec5
Data_08199ec5:
	.incbin "baserom.gba", 0x00199ec5, 0x00000007
	.global Data_08199ecc
Data_08199ecc:
	.incbin "baserom.gba", 0x00199ecc, 0x0000000a
	.global Data_08199ed6
Data_08199ed6:
	.incbin "baserom.gba", 0x00199ed6, 0x0000000a
	.global Data_08199ee0
Data_08199ee0:
	.incbin "baserom.gba", 0x00199ee0, 0x0000000a
	.global Data_08199eea
Data_08199eea:
	.incbin "baserom.gba", 0x00199eea, 0x00000010
	.global Data_08199efa
Data_08199efa:
	.incbin "baserom.gba", 0x00199efa, 0x00000010
	.global Data_08199f0a
Data_08199f0a:
	.incbin "baserom.gba", 0x00199f0a, 0x00000010
	.global Data_08199f1a
Data_08199f1a:
	.incbin "baserom.gba", 0x00199f1a, 0x00000008
	.global Data_08199f22
Data_08199f22:
	.incbin "baserom.gba", 0x00199f22, 0x00000008
	.global Data_08199f2a
Data_08199f2a:
	.incbin "baserom.gba", 0x00199f2a, 0x00000004
	.global Data_08199f2e
Data_08199f2e:
	.incbin "baserom.gba", 0x00199f2e, 0x00000006
	.global Data_08199f34
Data_08199f34:
	.incbin "baserom.gba", 0x00199f34, 0x00000004
	.global Data_08199f38
Data_08199f38:
	.incbin "baserom.gba", 0x00199f38, 0x00000004
	.global Data_08199f3c
Data_08199f3c:
	.incbin "baserom.gba", 0x00199f3c, 0x00000004
	.global Data_08199f40
Data_08199f40:
	.incbin "baserom.gba", 0x00199f40, 0x00000008
	.global Data_08199f48
Data_08199f48:
	.incbin "baserom.gba", 0x00199f48, 0x00000004
	.global Data_08199f4c
Data_08199f4c:
	.incbin "baserom.gba", 0x00199f4c, 0x0000003c
	.global Data_08199f88
Data_08199f88:
	.incbin "baserom.gba", 0x00199f88, 0x00000010
	.global Data_08199f98
Data_08199f98:
	.incbin "baserom.gba", 0x00199f98, 0x00000024
	.global Data_08199fbc
Data_08199fbc:
	.incbin "baserom.gba", 0x00199fbc, 0x0000000c
	.global Data_08199fc8
Data_08199fc8:
	.incbin "baserom.gba", 0x00199fc8, 0x0000000a
	.global Data_08199fd2
Data_08199fd2:
	.incbin "baserom.gba", 0x00199fd2, 0x0000000c
	.global Data_08199fde
Data_08199fde:
	.incbin "baserom.gba", 0x00199fde, 0x0000000f
	.global Data_08199fed
Data_08199fed:
	.incbin "baserom.gba", 0x00199fed, 0x0000000f
	.global Data_08199ffc
Data_08199ffc:
	.incbin "baserom.gba", 0x00199ffc, 0x0000001e
	.global Data_0819a01a
Data_0819a01a:
	.incbin "baserom.gba", 0x0019a01a, 0x0000001e
	.global Data_0819a038
Data_0819a038:
	.incbin "baserom.gba", 0x0019a038, 0x00000008
	.global Data_0819a040
Data_0819a040:
	.incbin "baserom.gba", 0x0019a040, 0x00000008
	.global Data_0819a048
Data_0819a048:
	.incbin "baserom.gba", 0x0019a048, 0x0000000c
	.global Data_0819a054
Data_0819a054:
	.incbin "baserom.gba", 0x0019a054, 0x0000000c
	.global Data_0819a060
Data_0819a060:
	.incbin "baserom.gba", 0x0019a060, 0x00000006
	.global Data_0819a066
Data_0819a066:
	.incbin "baserom.gba", 0x0019a066, 0x00000006
	.global Data_0819a06c
Data_0819a06c:
	.incbin "baserom.gba", 0x0019a06c, 0x00000006
	.global Data_0819a072
Data_0819a072:
	.incbin "baserom.gba", 0x0019a072, 0x00000006
	.global Data_0819a078
Data_0819a078:
	.incbin "baserom.gba", 0x0019a078, 0x00000006
	.global Data_0819a07e
Data_0819a07e:
	.incbin "baserom.gba", 0x0019a07e, 0x00000006
	.global Data_0819a084
Data_0819a084:
	.incbin "baserom.gba", 0x0019a084, 0x00000028
	.global Data_0819a0ac
Data_0819a0ac:
	.incbin "baserom.gba", 0x0019a0ac, 0x00000028
	.global Data_0819a0d4
Data_0819a0d4:
	.incbin "baserom.gba", 0x0019a0d4, 0x0000006c
	.global Data_0819a140
Data_0819a140:
	.incbin "baserom.gba", 0x0019a140, 0x00000020
	.global Data_0819a160
Data_0819a160:
	.incbin "baserom.gba", 0x0019a160, 0x00005ea0
	.section .unidentified.081a1928,"a"
	.incbin "baserom.gba", 0x001a1928, 0x00000080
	.global Data_081a19a8
Data_081a19a8:
	.incbin "baserom.gba", 0x001a19a8, 0x000006b1
	.global Data_081a2059
Data_081a2059:
	.incbin "baserom.gba", 0x001a2059, 0x00000063
	.global Data_081a20bc
Data_081a20bc:
	.incbin "baserom.gba", 0x001a20bc, 0x00000344
	.global Data_081a2400
Data_081a2400:
	.incbin "baserom.gba", 0x001a2400, 0x00003c00
	.section .unidentified.081a8288,"a"
	.global Data_081a8288
Data_081a8288:
	.incbin "baserom.gba", 0x001a8288, 0x000000f0
	.global Data_081a8378
Data_081a8378:
	.incbin "baserom.gba", 0x001a8378, 0x0000016e
	.global Data_081a84e6
Data_081a84e6:
	.incbin "baserom.gba", 0x001a84e6, 0x00000258
	.global Data_081a873e
Data_081a873e:
	.incbin "baserom.gba", 0x001a873e, 0x0000003c
	.global Data_081a877a
Data_081a877a:
	.incbin "baserom.gba", 0x001a877a, 0x00000040
	.global Data_081a87ba
Data_081a87ba:
	.incbin "baserom.gba", 0x001a87ba, 0x00000040
	.global Data_081a87fa
Data_081a87fa:
	.incbin "baserom.gba", 0x001a87fa, 0x00003806
	.section .unidentified.081ad348,"a"
	.incbin "baserom.gba", 0x001ad348, 0x00000020
	.global Data_081ad368
Data_081ad368:
	.incbin "baserom.gba", 0x001ad368, 0x00000004
	.global Data_081ad36c
Data_081ad36c:
	.incbin "baserom.gba", 0x001ad36c, 0x00000008
	.global Data_081ad374
Data_081ad374:
	.incbin "baserom.gba", 0x001ad374, 0x00000012
	.global Data_081ad386
Data_081ad386:
	.incbin "baserom.gba", 0x001ad386, 0x00004c7a
	.section .unidentified.081b4888,"a"
	.global Data_081b4888
Data_081b4888:
	.incbin "baserom.gba", 0x001b4888, 0x00000014
	.global Data_081b489c
Data_081b489c:
	.incbin "baserom.gba", 0x001b489c, 0x00000006
	.global Data_081b48a2
Data_081b48a2:
	.incbin "baserom.gba", 0x001b48a2, 0x00000008
	.global Data_081b48aa
Data_081b48aa:
	.incbin "baserom.gba", 0x001b48aa, 0x0000000e
	.global Data_081b48b8
Data_081b48b8:
	.incbin "baserom.gba", 0x001b48b8, 0x0000000e
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x001b48c6, 0x0000373a
	.section .unidentified.081ba30c,"a"
	.global Data_081ba30c
Data_081ba30c:
	.incbin "baserom.gba", 0x001ba30c, 0x00005500
	.global Func_081bf80c
Func_081bf80c:
	.incbin "baserom.gba", 0x001bf80c, 0x00000010
	.global Func_081bf81c
Func_081bf81c:
	.incbin "baserom.gba", 0x001bf81c, 0x000007e4
	.section .unidentified.081c3430,"a"
	.global Data_081c3430
Data_081c3430:
	.incbin "baserom.gba", 0x001c3430, 0x0000000c
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x001c343c, 0x00000090
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x001c34cc, 0x000000b4
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x001c3580, 0x00000030
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x001c35b0, 0x00000018
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x001c35c8, 0x00000084
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x001c364c, 0x00000018
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x001c3664, 0x0000003c
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x001c36a0, 0x00000010
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x001c36b0, 0x00000031
	.global Data_081c36e1
Data_081c36e1:
	.incbin "baserom.gba", 0x001c36e1, 0x00000003
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x001c36e4, 0x00000030
	.section .unidentified.081c43b0,"a"
	.incbin "baserom.gba", 0x001c43b0, 0x00000090
	.section .unidentified.081c44d0,"a"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x001c44d0, 0x00000060
	.section .unidentified.082f9030,"a"
	.incbin "baserom.gba", 0x002f9030, 0x00006fd0
	.section .unidentified.086322b2,"a"
	.incbin "baserom.gba", 0x006322b2, 0x0004dd4e
	.section .unidentified.08682000,"a"
	.global Resource_Data002
Resource_Data002:
	.incbin "baserom.gba", 0x00682000, 0x00000010
	.section .unidentified.086848d0,"a"
	.global Resource_Data015
Resource_Data015:
	.incbin "baserom.gba", 0x006848d0, 0x000000c0
	.section .unidentified.0868a3fe,"a"
	.incbin "baserom.gba", 0x0068a3fe, 0x00000002
	.global Resource_Data017
Resource_Data017:
	.incbin "baserom.gba", 0x0068a400, 0x000086f8
	.global Resource_Data018
Resource_Data018:
	.incbin "baserom.gba", 0x00692af8, 0x00005250
	.section .unidentified.086a48dd,"a"
	.incbin "baserom.gba", 0x006a48dd, 0x00000003
	.global Resource_Data01B
Resource_Data01B:
	.incbin "baserom.gba", 0x006a48e0, 0x00000200
	.global Resource_Data01C
Resource_Data01C:
	.incbin "baserom.gba", 0x006a4ae0, 0x00000770
	.global Resource_Data01D
Resource_Data01D:
	.incbin "baserom.gba", 0x006a5250, 0x00000984
	.global Resource_Data01E
Resource_Data01E:
	.incbin "baserom.gba", 0x006a5bd4, 0x00000878
	.global Resource_Data01F
Resource_Data01F:
	.incbin "baserom.gba", 0x006a644c, 0x000005d4
	.global Resource_Data020
Resource_Data020:
	.incbin "baserom.gba", 0x006a6a20, 0x00000828
	.global Resource_Data021
Resource_Data021:
	.incbin "baserom.gba", 0x006a7248, 0x0000186c
	.section .unidentified.086aa06b,"a"
	.incbin "baserom.gba", 0x006aa06b, 0x00000001
	.global Resource_Data023
Resource_Data023:
	.incbin "baserom.gba", 0x006aa06c, 0x0000761c
	.global Resource_Data024
Resource_Data024:
	.incbin "baserom.gba", 0x006b1688, 0x0001bf8c
	.global Resource_Data025
Resource_Data025:
	.incbin "baserom.gba", 0x006cd614, 0x000002b4
	.global Resource_Data026
Resource_Data026:
	.incbin "baserom.gba", 0x006cd8c8, 0x00000f0c
	.section .unidentified.086d11c2,"a"
	.incbin "baserom.gba", 0x006d11c2, 0x00000002
	.section .unidentified.086d5606,"a"
	.incbin "baserom.gba", 0x006d5606, 0x00000002
	.section .unidentified.086e5d76,"a"
	.incbin "baserom.gba", 0x006e5d76, 0x00000002
	.section .unidentified.086e9366,"a"
	.incbin "baserom.gba", 0x006e9366, 0x00000002
	.section .unidentified.086f4e0a,"a"
	.incbin "baserom.gba", 0x006f4e0a, 0x00000002
	.section .unidentified.087054ba,"a"
	.incbin "baserom.gba", 0x007054ba, 0x00000002
	.section .unidentified.0870cf5a,"a"
	.incbin "baserom.gba", 0x0070cf5a, 0x00000002
	.section .unidentified.0871079a,"a"
	.incbin "baserom.gba", 0x0071079a, 0x00000002
	.section .unidentified.08719bce,"a"
	.incbin "baserom.gba", 0x00719bce, 0x00000002
	.section .unidentified.08720d7a,"a"
	.incbin "baserom.gba", 0x00720d7a, 0x00000002
	.section .unidentified.08728c36,"a"
	.incbin "baserom.gba", 0x00728c36, 0x00000002
	.section .unidentified.0872d6c6,"a"
	.incbin "baserom.gba", 0x0072d6c6, 0x00000002
	.section .unidentified.0873197a,"a"
	.incbin "baserom.gba", 0x0073197a, 0x00000002
	.section .unidentified.087352fa,"a"
	.incbin "baserom.gba", 0x007352fa, 0x00000002
	.section .unidentified.08741b36,"a"
	.incbin "baserom.gba", 0x00741b36, 0x00000002
	.section .unidentified.0874d276,"a"
	.incbin "baserom.gba", 0x0074d276, 0x00000002
	.section .unidentified.08750646,"a"
	.incbin "baserom.gba", 0x00750646, 0x00000002
	.section .unidentified.08762562,"a"
	.incbin "baserom.gba", 0x00762562, 0x00000002
	.section .unidentified.08765fca,"a"
	.incbin "baserom.gba", 0x00765fca, 0x00000002
	.section .unidentified.087699ba,"a"
	.incbin "baserom.gba", 0x007699ba, 0x00000002
	.section .unidentified.08779f22,"a"
	.incbin "baserom.gba", 0x00779f22, 0x00000002
	.section .unidentified.0877de52,"a"
	.incbin "baserom.gba", 0x0077de52, 0x00000002
	.section .unidentified.08781876,"a"
	.incbin "baserom.gba", 0x00781876, 0x00000002
	.section .unidentified.08789b96,"a"
	.incbin "baserom.gba", 0x00789b96, 0x00000002
	.section .unidentified.08791d02,"a"
	.incbin "baserom.gba", 0x00791d02, 0x00000002
	.section .unidentified.0879efce,"a"
	.incbin "baserom.gba", 0x0079efce, 0x00000002
	.section .unidentified.087a2e8a,"a"
	.incbin "baserom.gba", 0x007a2e8a, 0x00000002
	.section .unidentified.087a6c86,"a"
	.incbin "baserom.gba", 0x007a6c86, 0x00000002
	.section .unidentified.087ab156,"a"
	.incbin "baserom.gba", 0x007ab156, 0x00000002
	.section .unidentified.087b2b32,"a"
	.incbin "baserom.gba", 0x007b2b32, 0x00000002
	.section .unidentified.087b715e,"a"
	.incbin "baserom.gba", 0x007b715e, 0x00000002
	.section .unidentified.087bedda,"a"
	.incbin "baserom.gba", 0x007bedda, 0x00000002
	.section .unidentified.087c29d6,"a"
	.incbin "baserom.gba", 0x007c29d6, 0x00000002
	.section .unidentified.087c6386,"a"
	.incbin "baserom.gba", 0x007c6386, 0x00000002
	.section .unidentified.087ca7d6,"a"
	.incbin "baserom.gba", 0x007ca7d6, 0x00000002
	.section .unidentified.087ce3ce,"a"
	.incbin "baserom.gba", 0x007ce3ce, 0x00000002
	.section .unidentified.087da02e,"a"
	.incbin "baserom.gba", 0x007da02e, 0x00000002
	.section .unidentified.087ddc7e,"a"
	.incbin "baserom.gba", 0x007ddc7e, 0x00000002
	.section .unidentified.087e61fe,"a"
	.incbin "baserom.gba", 0x007e61fe, 0x00000002
	.section .unidentified.087f284e,"a"
	.incbin "baserom.gba", 0x007f284e, 0x00000002
	.section .unidentified.087f9302,"a"
	.incbin "baserom.gba", 0x007f9302, 0x00000002
	.section .unidentified.088025ca,"a"
	.incbin "baserom.gba", 0x008025ca, 0x00000002
	.section .unidentified.08809fee,"a"
	.incbin "baserom.gba", 0x00809fee, 0x00000002
	.section .unidentified.0881fc6e,"a"
	.incbin "baserom.gba", 0x0081fc6e, 0x00000002
	.section .unidentified.088290b2,"a"
	.incbin "baserom.gba", 0x008290b2, 0x00000002
	.section .unidentified.088322ce,"a"
	.incbin "baserom.gba", 0x008322ce, 0x00000002
	.section .unidentified.0883fbf6,"a"
	.incbin "baserom.gba", 0x0083fbf6, 0x00000002
	.section .unidentified.08845a4a,"a"
	.incbin "baserom.gba", 0x00845a4a, 0x00000002
	.section .unidentified.0884b76a,"a"
	.incbin "baserom.gba", 0x0084b76a, 0x00000002
	.section .unidentified.0884beed,"a"
	.incbin "baserom.gba", 0x0084beed, 0x00000003
	.section .unidentified.0884d83f,"a"
	.incbin "baserom.gba", 0x0084d83f, 0x00000001
	.section .unidentified.0885065d,"a"
	.incbin "baserom.gba", 0x0085065d, 0x00000003
	.section .unidentified.08854572,"a"
	.incbin "baserom.gba", 0x00854572, 0x00000002
	.section .unidentified.08857595,"a"
	.incbin "baserom.gba", 0x00857595, 0x00000003
	.global Resource_Data087
Resource_Data087:
	.incbin "baserom.gba", 0x00857598, 0x000009bc
	.section .unidentified.088583a9,"a"
	.incbin "baserom.gba", 0x008583a9, 0x00000003
	.section .unidentified.088591ad,"a"
	.incbin "baserom.gba", 0x008591ad, 0x00000003
	.section .unidentified.0885963f,"a"
	.incbin "baserom.gba", 0x0085963f, 0x00000001
	.section .unidentified.088599ad,"a"
	.incbin "baserom.gba", 0x008599ad, 0x00000003
	.section .unidentified.0885a56b,"a"
	.incbin "baserom.gba", 0x0085a56b, 0x00000001
	.section .unidentified.0885a649,"a"
	.incbin "baserom.gba", 0x0085a649, 0x00000003
	.section .unidentified.0885a806,"a"
	.incbin "baserom.gba", 0x0085a806, 0x00000002
	.section .unidentified.0885abd1,"a"
	.incbin "baserom.gba", 0x0085abd1, 0x00000003
	.section .unidentified.0885af9d,"a"
	.incbin "baserom.gba", 0x0085af9d, 0x00000003
	.section .unidentified.0885b336,"a"
	.incbin "baserom.gba", 0x0085b336, 0x00000002
	.section .unidentified.0885b99b,"a"
	.incbin "baserom.gba", 0x0085b99b, 0x00000001
	.section .unidentified.0885e2cf,"a"
	.incbin "baserom.gba", 0x0085e2cf, 0x00000001
	.section .unidentified.0885ea4a,"a"
	.incbin "baserom.gba", 0x0085ea4a, 0x00000002
	.section .unidentified.0886008f,"a"
	.incbin "baserom.gba", 0x0086008f, 0x00000001
	.section .unidentified.088605b9,"a"
	.incbin "baserom.gba", 0x008605b9, 0x00000003
	.section .unidentified.08860dc2,"a"
	.incbin "baserom.gba", 0x00860dc2, 0x00000002
	.section .unidentified.0886133b,"a"
	.incbin "baserom.gba", 0x0086133b, 0x00000001
	.section .unidentified.08862a7a,"a"
	.incbin "baserom.gba", 0x00862a7a, 0x00000002
	.section .unidentified.08865943,"a"
	.incbin "baserom.gba", 0x00865943, 0x00000001
	.section .unidentified.08865bf5,"a"
	.incbin "baserom.gba", 0x00865bf5, 0x00000003
	.section .unidentified.08866fd7,"a"
	.incbin "baserom.gba", 0x00866fd7, 0x00000001
	.section .unidentified.0886b813,"a"
	.incbin "baserom.gba", 0x0086b813, 0x00000001
	.section .unidentified.0886efb9,"a"
	.incbin "baserom.gba", 0x0086efb9, 0x00000003
	.section .unidentified.08870cde,"a"
	.incbin "baserom.gba", 0x00870cde, 0x00000002
	.section .unidentified.08872bdd,"a"
	.incbin "baserom.gba", 0x00872bdd, 0x00000003
	.section .unidentified.0887458b,"a"
	.incbin "baserom.gba", 0x0087458b, 0x00000001
	.section .unidentified.08875b03,"a"
	.incbin "baserom.gba", 0x00875b03, 0x00000001
	.section .unidentified.08879736,"a"
	.incbin "baserom.gba", 0x00879736, 0x00000002
	.section .unidentified.0887a294,"a"
	.global Resource_Data0B0
Resource_Data0B0:
	.incbin "baserom.gba", 0x0087a294, 0x00001c50
	.global Resource_Data0B1
Resource_Data0B1:
	.incbin "baserom.gba", 0x0087bee4, 0x00000440
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "baserom.gba", 0x0087c324, 0x0000024c
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "baserom.gba", 0x0087c570, 0x00000198
	.global Resource_Data0B4
Resource_Data0B4:
	.incbin "baserom.gba", 0x0087c708, 0x0000082c
	.global Resource_Data0B5
Resource_Data0B5:
	.incbin "baserom.gba", 0x0087cf34, 0x00000e98
	.section .unidentified.0887e403,"a"
	.incbin "baserom.gba", 0x0087e403, 0x00000001
	.section .unidentified.0888056d,"a"
	.incbin "baserom.gba", 0x0088056d, 0x00000003
	.section .unidentified.08880678,"a"
	.global Resource_Data0BA
Resource_Data0BA:
	.incbin "baserom.gba", 0x00880678, 0x0000024c
	.global Resource_Data0BB
Resource_Data0BB:
	.incbin "baserom.gba", 0x008808c4, 0x00000184
	.section .unidentified.08881301,"a"
	.incbin "baserom.gba", 0x00881301, 0x00000003
	.section .unidentified.08882ee5,"a"
	.incbin "baserom.gba", 0x00882ee5, 0x00000003
	.section .unidentified.08884a42,"a"
	.incbin "baserom.gba", 0x00884a42, 0x00000002
	.section .unidentified.08884e81,"a"
	.incbin "baserom.gba", 0x00884e81, 0x00000003
	.section .unidentified.08885296,"a"
	.incbin "baserom.gba", 0x00885296, 0x00000002
	.global Resource_Data0C1
Resource_Data0C1:
	.incbin "baserom.gba", 0x00885298, 0x00000338
	.global Resource_Data0C2
Resource_Data0C2:
	.incbin "baserom.gba", 0x008855d0, 0x000010cc
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "baserom.gba", 0x0088669c, 0x000002e8
	.global Resource_Data0C4
Resource_Data0C4:
	.incbin "baserom.gba", 0x00886984, 0x00000154
	.section .unidentified.08888fe3,"a"
	.incbin "baserom.gba", 0x00888fe3, 0x00000001
	.section .unidentified.0888999f,"a"
	.incbin "baserom.gba", 0x0088999f, 0x00000001
	.section .unidentified.0888aae2,"a"
	.incbin "baserom.gba", 0x0088aae2, 0x00000002
	.global Resource_Data0C8
Resource_Data0C8:
	.incbin "baserom.gba", 0x0088aae4, 0x00000c7c
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "baserom.gba", 0x0088b760, 0x0000002c
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "baserom.gba", 0x0088b78c, 0x0000002c
	.global Resource_Data0CB
Resource_Data0CB:
	.incbin "baserom.gba", 0x0088b7b8, 0x000007c0
	.section .unidentified.0888d4f3,"a"
	.incbin "baserom.gba", 0x0088d4f3, 0x00000001
	.section .unidentified.0888d81d,"a"
	.incbin "baserom.gba", 0x0088d81d, 0x00000003
	.global Resource_Data0CE
Resource_Data0CE:
	.incbin "baserom.gba", 0x0088d820, 0x00000528
	.global Resource_Data0CF
Resource_Data0CF:
	.incbin "baserom.gba", 0x0088dd48, 0x00000530
	.global Resource_Data0D0
Resource_Data0D0:
	.incbin "baserom.gba", 0x0088e278, 0x000005f0
	.section .unidentified.0888eff1,"a"
	.incbin "baserom.gba", 0x0088eff1, 0x00000003
	.global Resource_Data0D2
Resource_Data0D2:
	.incbin "baserom.gba", 0x0088eff4, 0x00000200
	.global Resource_Data0D3
Resource_Data0D3:
	.incbin "baserom.gba", 0x0088f1f4, 0x00000420
	.section .unidentified.0888fc3d,"a"
	.incbin "baserom.gba", 0x0088fc3d, 0x00000003
	.section .unidentified.08890047,"a"
	.incbin "baserom.gba", 0x00890047, 0x00000001
	.global Resource_Data0D7
Resource_Data0D7:
	.incbin "baserom.gba", 0x00890048, 0x0000025c
	.global Resource_Data0D8
Resource_Data0D8:
	.incbin "baserom.gba", 0x008902a4, 0x0000057c
	.section .unidentified.08890ac9,"a"
	.incbin "baserom.gba", 0x00890ac9, 0x00000003
	.global Resource_Data0DA
Resource_Data0DA:
	.incbin "baserom.gba", 0x00890acc, 0x0000095c
	.section .unidentified.08891ae5,"a"
	.incbin "baserom.gba", 0x00891ae5, 0x00000003
	.global Resource_Data0DC
Resource_Data0DC:
	.incbin "baserom.gba", 0x00891ae8, 0x00002ab0
	.global Resource_Data0DD
Resource_Data0DD:
	.incbin "baserom.gba", 0x00894598, 0x000011cc
	.section .unidentified.08895dc3,"a"
	.incbin "baserom.gba", 0x00895dc3, 0x00000001
	.section .unidentified.08896dfd,"a"
	.incbin "baserom.gba", 0x00896dfd, 0x00000003
	.section .unidentified.08897453,"a"
	.incbin "baserom.gba", 0x00897453, 0x00000001
	.section .unidentified.08897ad1,"a"
	.incbin "baserom.gba", 0x00897ad1, 0x00000003
	.section .unidentified.088980f1,"a"
	.incbin "baserom.gba", 0x008980f1, 0x00000003
	.section .unidentified.088994be,"a"
	.incbin "baserom.gba", 0x008994be, 0x00000002
	.section .unidentified.0889a502,"a"
	.incbin "baserom.gba", 0x0089a502, 0x00000002
	.section .unidentified.0889af8b,"a"
	.incbin "baserom.gba", 0x0089af8b, 0x00000001
	.global Resource_Data0E9
Resource_Data0E9:
	.incbin "baserom.gba", 0x0089af8c, 0x000002cc
	.section .unidentified.0889bf91,"a"
	.incbin "baserom.gba", 0x0089bf91, 0x00000003
	.section .unidentified.0889d1cd,"a"
	.incbin "baserom.gba", 0x0089d1cd, 0x00000003
	.section .unidentified.0889db17,"a"
	.incbin "baserom.gba", 0x0089db17, 0x00000001
	.global Resource_Data0EE
Resource_Data0EE:
	.incbin "baserom.gba", 0x0089db18, 0x0000065c
	.global Resource_Data0EF
Resource_Data0EF:
	.incbin "baserom.gba", 0x0089e174, 0x0000052c
	.global Resource_Data0F0
Resource_Data0F0:
	.incbin "baserom.gba", 0x0089e6a0, 0x000022bc
	.global Resource_Data0F1
Resource_Data0F1:
	.incbin "baserom.gba", 0x008a095c, 0x00001794
	.global Resource_Data0F2
Resource_Data0F2:
	.incbin "baserom.gba", 0x008a20f0, 0x000006e4
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x008a27d4, 0x00001f4c
	.section .unidentified.088a4c54,"a"
	.global Resource_Data0F5
Resource_Data0F5:
	.incbin "baserom.gba", 0x008a4c54, 0x000011d8
	.section .unidentified.088a631a,"a"
	.incbin "baserom.gba", 0x008a631a, 0x00000002
	.global Resource_Data0F7
Resource_Data0F7:
	.incbin "baserom.gba", 0x008a631c, 0x00000648
	.global Resource_Data0F8
Resource_Data0F8:
	.incbin "baserom.gba", 0x008a6964, 0x00000c24
	.global Resource_Data0F9
Resource_Data0F9:
	.incbin "baserom.gba", 0x008a7588, 0x000003c4
	.global Resource_Data0FA
Resource_Data0FA:
	.incbin "baserom.gba", 0x008a794c, 0x000001c8
	.global Resource_Data0FB
Resource_Data0FB:
	.incbin "baserom.gba", 0x008a7b14, 0x0000054c
	.global Resource_Data0FC
Resource_Data0FC:
	.incbin "baserom.gba", 0x008a8060, 0x0000034c
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x008a83ac, 0x0000076c
	.section .unidentified.088a915b,"a"
	.incbin "baserom.gba", 0x008a915b, 0x00000001
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a915c, 0x000000a8
	.section .unidentified.088a951a,"a"
	.incbin "baserom.gba", 0x008a951a, 0x00000002
	.global Resource_Data101
Resource_Data101:
	.incbin "baserom.gba", 0x008a951c, 0x000009a8
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x008a9ec4, 0x000002f8
	.global Resource_Data103
Resource_Data103:
	.incbin "baserom.gba", 0x008aa1bc, 0x00000b30
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x008aacec, 0x00000100
	.section .unidentified.088aba5d,"a"
	.incbin "baserom.gba", 0x008aba5d, 0x00000003
	.section .unidentified.088ac2a5,"a"
	.incbin "baserom.gba", 0x008ac2a5, 0x00000003
	.global Resource_Data107
Resource_Data107:
	.incbin "baserom.gba", 0x008ac2a8, 0x00001200
	.section .unidentified.088ae367,"a"
	.incbin "baserom.gba", 0x008ae367, 0x00000001
	.section .unidentified.088aee43,"a"
	.incbin "baserom.gba", 0x008aee43, 0x00000001
	.section .unidentified.088af506,"a"
	.incbin "baserom.gba", 0x008af506, 0x00000002
	.section .unidentified.088af7ed,"a"
	.incbin "baserom.gba", 0x008af7ed, 0x00000003
	.section .unidentified.088b0b53,"a"
	.incbin "baserom.gba", 0x008b0b53, 0x00000001
	.section .unidentified.088b0f3d,"a"
	.incbin "baserom.gba", 0x008b0f3d, 0x00000003
	.section .unidentified.088b130f,"a"
	.incbin "baserom.gba", 0x008b130f, 0x00000001
	.section .unidentified.088b1baf,"a"
	.incbin "baserom.gba", 0x008b1baf, 0x00000001
	.section .unidentified.088b2052,"a"
	.incbin "baserom.gba", 0x008b2052, 0x00000002
	.section .unidentified.088b320b,"a"
	.incbin "baserom.gba", 0x008b320b, 0x00000001
	.section .unidentified.088b3664,"a"
	.global Resource_Data119
Resource_Data119:
	.incbin "baserom.gba", 0x008b3664, 0x0000106c
	.global Resource_Data11A
Resource_Data11A:
	.incbin "baserom.gba", 0x008b46d0, 0x00000fc8
	.section .unidentified.088b58f2,"a"
	.incbin "baserom.gba", 0x008b58f2, 0x00000002
	.section .unidentified.088b734d,"a"
	.incbin "baserom.gba", 0x008b734d, 0x00000003
	.global Resource_Data11E
Resource_Data11E:
	.incbin "baserom.gba", 0x008b7350, 0x000001b4
	.section .unidentified.088b7899,"a"
	.incbin "baserom.gba", 0x008b7899, 0x00000003
	.section .unidentified.088b9622,"a"
	.incbin "baserom.gba", 0x008b9622, 0x00000002
	.global Resource_Data121
Resource_Data121:
	.incbin "baserom.gba", 0x008b9624, 0x00000278
	.section .unidentified.088b9d62,"a"
	.incbin "baserom.gba", 0x008b9d62, 0x00000002
	.section .unidentified.088bb937,"a"
	.incbin "baserom.gba", 0x008bb937, 0x00000001
	.section .unidentified.088bd535,"a"
	.incbin "baserom.gba", 0x008bd535, 0x00000003
	.section .unidentified.088bd756,"a"
	.incbin "baserom.gba", 0x008bd756, 0x00000002
	.global Resource_Data127
Resource_Data127:
	.incbin "baserom.gba", 0x008bd758, 0x0000043c
	.section .unidentified.088bdca5,"a"
	.incbin "baserom.gba", 0x008bdca5, 0x00000003
	.section .unidentified.088be287,"a"
	.incbin "baserom.gba", 0x008be287, 0x00000001
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x008be288, 0x000004a0
	.section .unidentified.088bf43a,"a"
	.incbin "baserom.gba", 0x008bf43a, 0x00000002
	.section .unidentified.088bf97c,"a"
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x008bf97c, 0x00000840
	.section .unidentified.088c054b,"a"
	.incbin "baserom.gba", 0x008c054b, 0x00000001
	.global Resource_Data12F
Resource_Data12F:
	.incbin "baserom.gba", 0x008c054c, 0x00001258
	.section .unidentified.088c2292,"a"
	.incbin "baserom.gba", 0x008c2292, 0x00000002
	.section .unidentified.088c3069,"a"
	.incbin "baserom.gba", 0x008c3069, 0x00000003
	.section .unidentified.088c3896,"a"
	.incbin "baserom.gba", 0x008c3896, 0x00000002
	.section .unidentified.088c3c88,"a"
	.global Resource_Data134
Resource_Data134:
	.incbin "baserom.gba", 0x008c3c88, 0x000001bc
	.global Resource_Data135
Resource_Data135:
	.incbin "baserom.gba", 0x008c3e44, 0x000001bc
	.global Resource_Data136
Resource_Data136:
	.incbin "baserom.gba", 0x008c4000, 0x00000940
	.global Resource_Data137
Resource_Data137:
	.incbin "baserom.gba", 0x008c4940, 0x00000418
	.section .unidentified.088c56c9,"a"
	.incbin "baserom.gba", 0x008c56c9, 0x00000003
	.section .unidentified.088c5a77,"a"
	.incbin "baserom.gba", 0x008c5a77, 0x00000001
	.section .unidentified.088c7a1b,"a"
	.incbin "baserom.gba", 0x008c7a1b, 0x00000001
	.section .unidentified.088c87b7,"a"
	.incbin "baserom.gba", 0x008c87b7, 0x00000001
	.section .unidentified.088c89d3,"a"
	.incbin "baserom.gba", 0x008c89d3, 0x00000001
	.section .unidentified.088c8ccf,"a"
	.incbin "baserom.gba", 0x008c8ccf, 0x00000001
	.section .unidentified.088c9070,"a"
	.global Resource_Data143
Resource_Data143:
	.incbin "baserom.gba", 0x008c9070, 0x000016b0
	.section .unidentified.088cae7d,"a"
	.incbin "baserom.gba", 0x008cae7d, 0x00000003
	.section .unidentified.088cbc4b,"a"
	.incbin "baserom.gba", 0x008cbc4b, 0x00000001
	.global Resource_Data146
Resource_Data146:
	.incbin "baserom.gba", 0x008cbc4c, 0x00000c74
	.section .unidentified.088cceb5,"a"
	.incbin "baserom.gba", 0x008cceb5, 0x00000003
	.section .unidentified.088cd40f,"a"
	.incbin "baserom.gba", 0x008cd40f, 0x00000001
	.section .unidentified.088cecf9,"a"
	.incbin "baserom.gba", 0x008cecf9, 0x00000003
	.global Resource_Data14F
Resource_Data14F:
	.incbin "baserom.gba", 0x008cecfc, 0x000008a0
	.section .unidentified.088cf977,"a"
	.incbin "baserom.gba", 0x008cf977, 0x00000001
	.section .unidentified.088cfc01,"a"
	.incbin "baserom.gba", 0x008cfc01, 0x00000003
	.section .unidentified.088cffa7,"a"
	.incbin "baserom.gba", 0x008cffa7, 0x00000001
	.section .unidentified.088d0202,"a"
	.incbin "baserom.gba", 0x008d0202, 0x00000002
	.section .unidentified.088d05ba,"a"
	.incbin "baserom.gba", 0x008d05ba, 0x00000002
	.section .unidentified.088d1aa3,"a"
	.incbin "baserom.gba", 0x008d1aa3, 0x00000001
	.section .unidentified.088d3066,"a"
	.incbin "baserom.gba", 0x008d3066, 0x00000002
	.global Resource_Data15A
Resource_Data15A:
	.incbin "baserom.gba", 0x008d3068, 0x00000a6c
	.section .unidentified.088d48aa,"a"
	.incbin "baserom.gba", 0x008d48aa, 0x00000002
	.section .unidentified.088d4c2d,"a"
	.incbin "baserom.gba", 0x008d4c2d, 0x00000003
	.section .unidentified.088d58dd,"a"
	.incbin "baserom.gba", 0x008d58dd, 0x00000003
	.section .unidentified.088d6425,"a"
	.incbin "baserom.gba", 0x008d6425, 0x00000003
	.global Resource_Data160
Resource_Data160:
	.incbin "baserom.gba", 0x008d6428, 0x00000198
	.global Resource_Data161
Resource_Data161:
	.incbin "baserom.gba", 0x008d65c0, 0x0000088c
	.section .unidentified.088d7a1a,"a"
	.incbin "baserom.gba", 0x008d7a1a, 0x00000002
	.section .unidentified.088d7f32,"a"
	.incbin "baserom.gba", 0x008d7f32, 0x00000002
	.global Resource_Data16C
Resource_Data16C:
	.incbin "baserom.gba", 0x008d7f34, 0x00000624
	.section .unidentified.088d8979,"a"
	.incbin "baserom.gba", 0x008d8979, 0x00000003
	.section .unidentified.088d8c13,"a"
	.incbin "baserom.gba", 0x008d8c13, 0x00000001
	.section .unidentified.088da114,"a"
	.global Resource_Data170
Resource_Data170:
	.incbin "baserom.gba", 0x008da114, 0x0000049c
	.global Resource_Data171
Resource_Data171:
	.incbin "baserom.gba", 0x008da5b0, 0x0000198c
	.section .unidentified.088dc326,"a"
	.incbin "baserom.gba", 0x008dc326, 0x00000002
	.section .unidentified.088de29b,"a"
	.incbin "baserom.gba", 0x008de29b, 0x00000001
	.section .unidentified.088de76b,"a"
	.incbin "baserom.gba", 0x008de76b, 0x00000001
	.global Resource_Data176
Resource_Data176:
	.incbin "baserom.gba", 0x008de76c, 0x00000694
	.global Resource_Data177
Resource_Data177:
	.incbin "baserom.gba", 0x008dee00, 0x00000a34
	.global Resource_Data178
Resource_Data178:
	.incbin "baserom.gba", 0x008df834, 0x00000bfc
	.section .unidentified.088e0c05,"a"
	.incbin "baserom.gba", 0x008e0c05, 0x00000003
	.section .unidentified.088e16d6,"a"
	.incbin "baserom.gba", 0x008e16d6, 0x00000002
	.section .unidentified.088e2213,"a"
	.incbin "baserom.gba", 0x008e2213, 0x00000001
	.global Resource_Data17C
Resource_Data17C:
	.incbin "baserom.gba", 0x008e2214, 0x00000640
	.global Resource_Data17D
Resource_Data17D:
	.incbin "baserom.gba", 0x008e2854, 0x00001588
	.global Resource_Data17E
Resource_Data17E:
	.incbin "baserom.gba", 0x008e3ddc, 0x00000064
	.section .unidentified.088e41ab,"a"
	.incbin "baserom.gba", 0x008e41ab, 0x00000001
	.section .unidentified.088e4749,"a"
	.incbin "baserom.gba", 0x008e4749, 0x00000003
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e474c, 0x00000204
	.section .unidentified.088e4c89,"a"
	.incbin "baserom.gba", 0x008e4c89, 0x00000003
	.global Resource_Data183
Resource_Data183:
	.incbin "baserom.gba", 0x008e4c8c, 0x00001018
	.global Resource_Data184
Resource_Data184:
	.incbin "baserom.gba", 0x008e5ca4, 0x0000166c
	.section .unidentified.088e92ae,"a"
	.incbin "baserom.gba", 0x008e92ae, 0x00000002
	.section .unidentified.088e9a51,"a"
	.incbin "baserom.gba", 0x008e9a51, 0x00000003
	.section .unidentified.088eaa68,"a"
	.global Resource_Data189
Resource_Data189:
	.incbin "baserom.gba", 0x008eaa68, 0x0000037c
	.global Resource_Data18A
Resource_Data18A:
	.incbin "baserom.gba", 0x008eade4, 0x00000430
	.global Resource_Data18B
Resource_Data18B:
	.incbin "baserom.gba", 0x008eb214, 0x000010cc
	.section .unidentified.088ec364,"a"
	.global Resource_Data18D
Resource_Data18D:
	.incbin "baserom.gba", 0x008ec364, 0x000004e8
	.section .unidentified.088ec954,"a"
	.global Resource_Data190
Resource_Data190:
	.incbin "baserom.gba", 0x008ec954, 0x000006b8
	.section .unidentified.088ed91e,"a"
	.incbin "baserom.gba", 0x008ed91e, 0x00000002
	.global Resource_Data192
Resource_Data192:
	.incbin "baserom.gba", 0x008ed920, 0x00001b34
	.global Resource_Data193
Resource_Data193:
	.incbin "baserom.gba", 0x008ef454, 0x00001050
	.section .unidentified.088f1549,"a"
	.incbin "baserom.gba", 0x008f1549, 0x00000003
	.section .unidentified.088f1748,"a"
	.global Resource_Data197
Resource_Data197:
	.incbin "baserom.gba", 0x008f1748, 0x00041868
	.global Resource_Data198
Resource_Data198:
	.incbin "baserom.gba", 0x00932fb0, 0x000093c4
	.global Resource_Data199
Resource_Data199:
	.incbin "baserom.gba", 0x0093c374, 0x00000028
	.section .unidentified.0893c589,"a"
	.incbin "baserom.gba", 0x0093c589, 0x00000003
	.global Resource_Data19B
Resource_Data19B:
	.incbin "baserom.gba", 0x0093c58c, 0x00000154
	.global Resource_Data19C
Resource_Data19C:
	.incbin "baserom.gba", 0x0093c6e0, 0x000004b8
	.section .unidentified.0893e302,"a"
	.incbin "baserom.gba", 0x0093e302, 0x00000002
	.section .unidentified.0893f839,"a"
	.incbin "baserom.gba", 0x0093f839, 0x00000003
	.section .unidentified.0894168f,"a"
	.incbin "baserom.gba", 0x0094168f, 0x00000001
	.section .unidentified.089437a3,"a"
	.incbin "baserom.gba", 0x009437a3, 0x00000001
	.section .unidentified.0894397c,"a"
	.global Resource_Data1A4
Resource_Data1A4:
	.incbin "baserom.gba", 0x0094397c, 0x000001e4
	.global Resource_Data1A5
Resource_Data1A5:
	.incbin "baserom.gba", 0x00943b60, 0x000003c0
	.section .unidentified.0894659b,"a"
	.incbin "baserom.gba", 0x0094659b, 0x00000001
	.section .unidentified.08946fa3,"a"
	.incbin "baserom.gba", 0x00946fa3, 0x00000001
	.section .unidentified.08949cc2,"a"
	.incbin "baserom.gba", 0x00949cc2, 0x00000002
	.section .unidentified.08949e94,"a"
	.global Resource_Data1AD
Resource_Data1AD:
	.incbin "baserom.gba", 0x00949e94, 0x00000008
	.global Resource_Data1AE
Resource_Data1AE:
	.incbin "baserom.gba", 0x00949e9c, 0x00000460
	.section .unidentified.0894b8ee,"a"
	.incbin "baserom.gba", 0x0094b8ee, 0x00000002
	.section .unidentified.0894cd5e,"a"
	.incbin "baserom.gba", 0x0094cd5e, 0x00000002
	.section .unidentified.0894d652,"a"
	.incbin "baserom.gba", 0x0094d652, 0x00000002
	.section .unidentified.0894df79,"a"
	.incbin "baserom.gba", 0x0094df79, 0x00000003
	.section .unidentified.0894ee6a,"a"
	.incbin "baserom.gba", 0x0094ee6a, 0x00000002
	.section .unidentified.0894fa57,"a"
	.incbin "baserom.gba", 0x0094fa57, 0x00000001
	.section .unidentified.0894fc32,"a"
	.incbin "baserom.gba", 0x0094fc32, 0x00000002
	.global Resource_Data1B6
Resource_Data1B6:
	.incbin "baserom.gba", 0x0094fc34, 0x000001f0
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0094fe24, 0x000002d8
	.section .unidentified.08950992,"a"
	.incbin "baserom.gba", 0x00950992, 0x00000002
	.section .unidentified.08952dcd,"a"
	.incbin "baserom.gba", 0x00952dcd, 0x00000003
	.section .unidentified.0895339f,"a"
	.incbin "baserom.gba", 0x0095339f, 0x00000001
	.section .unidentified.0895383f,"a"
	.incbin "baserom.gba", 0x0095383f, 0x00000001
	.section .unidentified.08953b92,"a"
	.incbin "baserom.gba", 0x00953b92, 0x00000002
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x00953b94, 0x000002f8
	.section .unidentified.08953f65,"a"
	.incbin "baserom.gba", 0x00953f65, 0x00000003
	.global Resource_Data1C0
Resource_Data1C0:
	.incbin "baserom.gba", 0x00953f68, 0x000002f0
	.section .unidentified.089542ea,"a"
	.incbin "baserom.gba", 0x009542ea, 0x00000002
	.section .unidentified.08954e9d,"a"
	.incbin "baserom.gba", 0x00954e9d, 0x00000003
	.section .unidentified.08955b4a,"a"
	.incbin "baserom.gba", 0x00955b4a, 0x00000002
	.section .unidentified.0895664e,"a"
	.incbin "baserom.gba", 0x0095664e, 0x00000002
	.section .unidentified.0895758d,"a"
	.incbin "baserom.gba", 0x0095758d, 0x00000003
	.section .unidentified.08957dc9,"a"
	.incbin "baserom.gba", 0x00957dc9, 0x00000003
	.section .unidentified.0895926e,"a"
	.incbin "baserom.gba", 0x0095926e, 0x00000002
	.section .unidentified.08959d26,"a"
	.incbin "baserom.gba", 0x00959d26, 0x00000002
	.section .unidentified.0895a051,"a"
	.incbin "baserom.gba", 0x0095a051, 0x00000003
	.section .unidentified.0895af56,"a"
	.incbin "baserom.gba", 0x0095af56, 0x00000002
	.section .unidentified.0895b758,"a"
	.global Resource_Data1D5
Resource_Data1D5:
	.incbin "baserom.gba", 0x0095b758, 0x00000400
	.section .unidentified.08967f12,"a"
	.incbin "baserom.gba", 0x00967f12, 0x00000002
	.global Resource_Data1D8
Resource_Data1D8:
	.incbin "baserom.gba", 0x00967f14, 0x00000100
	.global Resource_Data1D9
Resource_Data1D9:
	.incbin "baserom.gba", 0x00968014, 0x000004c8
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x009684dc, 0x00000268
	.global Resource_Data1DB
Resource_Data1DB:
	.incbin "baserom.gba", 0x00968744, 0x000001c8
	.section .unidentified.08968d73,"a"
	.incbin "baserom.gba", 0x00968d73, 0x00000001
	.section .unidentified.08968f7f,"a"
	.incbin "baserom.gba", 0x00968f7f, 0x00000001
	.section .unidentified.08969183,"a"
	.incbin "baserom.gba", 0x00969183, 0x00000001
	.section .unidentified.08969212,"a"
	.incbin "baserom.gba", 0x00969212, 0x00000002
	.section .unidentified.089696f4,"a"
	.global Resource_Data1E3
Resource_Data1E3:
	.incbin "baserom.gba", 0x009696f4, 0x00000070
	.section .unidentified.089697bf,"a"
	.incbin "baserom.gba", 0x009697bf, 0x00000001
	.section .unidentified.089697f2,"a"
	.incbin "baserom.gba", 0x009697f2, 0x00000002
	.section .unidentified.089699be,"a"
	.incbin "baserom.gba", 0x009699be, 0x00000002
	.global Resource_Data1E8
Resource_Data1E8:
	.incbin "baserom.gba", 0x009699c0, 0x0000024c
	.global Resource_Data1E9
Resource_Data1E9:
	.incbin "baserom.gba", 0x00969c0c, 0x000000e4
	.section .unidentified.08969da9,"a"
	.incbin "baserom.gba", 0x00969da9, 0x00000003
	.section .unidentified.08969f7e,"a"
	.incbin "baserom.gba", 0x00969f7e, 0x00000002
	.global Resource_Data1ED
Resource_Data1ED:
	.incbin "baserom.gba", 0x00969f80, 0x00000098
	.global Resource_Data1EE
Resource_Data1EE:
	.incbin "baserom.gba", 0x0096a018, 0x00000024
	.section .unidentified.0896a18e,"a"
	.incbin "baserom.gba", 0x0096a18e, 0x00000002
	.section .unidentified.0896a271,"a"
	.incbin "baserom.gba", 0x0096a271, 0x00000003
	.section .unidentified.0896a341,"a"
	.incbin "baserom.gba", 0x0096a341, 0x00000003
	.section .unidentified.0896a467,"a"
	.incbin "baserom.gba", 0x0096a467, 0x00000001
	.section .unidentified.0896a496,"a"
	.incbin "baserom.gba", 0x0096a496, 0x00000002
	.section .unidentified.0896a6fa,"a"
	.incbin "baserom.gba", 0x0096a6fa, 0x00000002
	.section .unidentified.0896a797,"a"
	.incbin "baserom.gba", 0x0096a797, 0x00000001
	.section .unidentified.0896a7fc,"a"
	.global Resource_Data1F7
Resource_Data1F7:
	.incbin "baserom.gba", 0x0096a7fc, 0x00000400
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x0096abfc, 0x000000b0
	.global Resource_Data1F9
Resource_Data1F9:
	.incbin "baserom.gba", 0x0096acac, 0x00000560
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "baserom.gba", 0x0096b20c, 0x00000074
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "baserom.gba", 0x0096b280, 0x00000048
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "baserom.gba", 0x0096b2c8, 0x00000050
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "baserom.gba", 0x0096b318, 0x00000050
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x0096b368, 0x00000058
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "baserom.gba", 0x0096b3c0, 0x00000058
	.global Resource_Data200
Resource_Data200:
	.incbin "baserom.gba", 0x0096b418, 0x00000040
	.global Resource_Data201
Resource_Data201:
	.incbin "baserom.gba", 0x0096b458, 0x00000044
	.global Resource_Data202
Resource_Data202:
	.incbin "baserom.gba", 0x0096b49c, 0x00000054
	.section .unidentified.08970727,"a"
	.incbin "baserom.gba", 0x00970727, 0x00000001
	.section .unidentified.08973672,"a"
	.incbin "baserom.gba", 0x00973672, 0x00000002
	.section .unidentified.08977295,"a"
	.incbin "baserom.gba", 0x00977295, 0x00000003
	.section .unidentified.089794e1,"a"
	.incbin "baserom.gba", 0x009794e1, 0x00000003
	.section .unidentified.0897ea7d,"a"
	.incbin "baserom.gba", 0x0097ea7d, 0x00000003
	.section .unidentified.089822f7,"a"
	.incbin "baserom.gba", 0x009822f7, 0x00000001
	.section .unidentified.08988161,"a"
	.incbin "baserom.gba", 0x00988161, 0x00000003
	.section .unidentified.08989235,"a"
	.incbin "baserom.gba", 0x00989235, 0x00000003
	.section .unidentified.0898a2a1,"a"
	.incbin "baserom.gba", 0x0098a2a1, 0x00000003
	.section .unidentified.0898f8fb,"a"
	.incbin "baserom.gba", 0x0098f8fb, 0x00000001
	.section .unidentified.08992a3d,"a"
	.incbin "baserom.gba", 0x00992a3d, 0x00000003
	.section .unidentified.08994add,"a"
	.incbin "baserom.gba", 0x00994add, 0x00000003
	.section .unidentified.08996dfd,"a"
	.incbin "baserom.gba", 0x00996dfd, 0x00000003
	.section .unidentified.08999a76,"a"
	.incbin "baserom.gba", 0x00999a76, 0x00000002
	.section .unidentified.0899b971,"a"
	.incbin "baserom.gba", 0x0099b971, 0x00000003
	.section .unidentified.089a3eb5,"a"
	.incbin "baserom.gba", 0x009a3eb5, 0x00000003
	.section .unidentified.089acb37,"a"
	.incbin "baserom.gba", 0x009acb37, 0x00000001
	.section .unidentified.089af6b1,"a"
	.incbin "baserom.gba", 0x009af6b1, 0x00000003
	.section .unidentified.089b60ee,"a"
	.incbin "baserom.gba", 0x009b60ee, 0x00000002
	.section .unidentified.089b8be2,"a"
	.incbin "baserom.gba", 0x009b8be2, 0x00000002
	.section .unidentified.089ba6d2,"a"
	.incbin "baserom.gba", 0x009ba6d2, 0x00000002
	.section .unidentified.089bcfe8,"a"
	.global Resource_Data21F
Resource_Data21F:
	.incbin "baserom.gba", 0x009bcfe8, 0x00003440
	.section .unidentified.089c4dce,"a"
	.incbin "baserom.gba", 0x009c4dce, 0x00000002
	.section .unidentified.089c5d6a,"a"
	.incbin "baserom.gba", 0x009c5d6a, 0x00000002
	.section .unidentified.089c8495,"a"
	.incbin "baserom.gba", 0x009c8495, 0x00000003
	.section .unidentified.089c9baa,"a"
	.incbin "baserom.gba", 0x009c9baa, 0x00000002
	.section .unidentified.089d23c3,"a"
	.incbin "baserom.gba", 0x009d23c3, 0x00000001
	.section .unidentified.089d668b,"a"
	.incbin "baserom.gba", 0x009d668b, 0x00000001
	.section .unidentified.089ddec5,"a"
	.incbin "baserom.gba", 0x009ddec5, 0x00000003
	.section .unidentified.089df6aa,"a"
	.incbin "baserom.gba", 0x009df6aa, 0x00000002
	.section .unidentified.089e2441,"a"
	.incbin "baserom.gba", 0x009e2441, 0x00000003
	.section .unidentified.089e348b,"a"
	.incbin "baserom.gba", 0x009e348b, 0x00000001
	.section .unidentified.089eba1b,"a"
	.incbin "baserom.gba", 0x009eba1b, 0x00000001
	.section .unidentified.089ed815,"a"
	.incbin "baserom.gba", 0x009ed815, 0x00000003
	.section .unidentified.089eebdb,"a"
	.incbin "baserom.gba", 0x009eebdb, 0x00000001
	.section .unidentified.089f5add,"a"
	.incbin "baserom.gba", 0x009f5add, 0x00000003
	.section .unidentified.089f7dde,"a"
	.incbin "baserom.gba", 0x009f7dde, 0x00000002
	.section .unidentified.089fd02b,"a"
	.incbin "baserom.gba", 0x009fd02b, 0x00000001
	.section .unidentified.08a0242a,"a"
	.incbin "baserom.gba", 0x00a0242a, 0x00000002
	.section .unidentified.08a045de,"a"
	.incbin "baserom.gba", 0x00a045de, 0x00000002
	.section .unidentified.08a08745,"a"
	.incbin "baserom.gba", 0x00a08745, 0x00000003
	.section .unidentified.08a0b96a,"a"
	.incbin "baserom.gba", 0x00a0b96a, 0x00000002
	.section .unidentified.08a0d06e,"a"
	.incbin "baserom.gba", 0x00a0d06e, 0x00000002
	.section .unidentified.08a13c56,"a"
	.incbin "baserom.gba", 0x00a13c56, 0x00000002
	.section .unidentified.08a20236,"a"
	.incbin "baserom.gba", 0x00a20236, 0x00000002
	.section .unidentified.08a232ca,"a"
	.incbin "baserom.gba", 0x00a232ca, 0x00000002
	.section .unidentified.08a25b35,"a"
	.incbin "baserom.gba", 0x00a25b35, 0x00000003
	.section .unidentified.08a27626,"a"
	.incbin "baserom.gba", 0x00a27626, 0x00000002
	.section .unidentified.08a2843e,"a"
	.incbin "baserom.gba", 0x00a2843e, 0x00000002
	.section .unidentified.08a29129,"a"
	.incbin "baserom.gba", 0x00a29129, 0x00000003
	.section .unidentified.08a325e6,"a"
	.incbin "baserom.gba", 0x00a325e6, 0x00000002
	.section .unidentified.08a332f1,"a"
	.incbin "baserom.gba", 0x00a332f1, 0x00000003
	.section .unidentified.08a34555,"a"
	.incbin "baserom.gba", 0x00a34555, 0x00000003
	.section .unidentified.08a35487,"a"
	.incbin "baserom.gba", 0x00a35487, 0x00000001
	.section .unidentified.08a360f7,"a"
	.incbin "baserom.gba", 0x00a360f7, 0x00000001
	.section .unidentified.08a36d0f,"a"
	.incbin "baserom.gba", 0x00a36d0f, 0x00000001
	.section .unidentified.08a37483,"a"
	.incbin "baserom.gba", 0x00a37483, 0x00000001
	.section .unidentified.08a38d13,"a"
	.incbin "baserom.gba", 0x00a38d13, 0x00000001
	.section .unidentified.08a396e1,"a"
	.incbin "baserom.gba", 0x00a396e1, 0x00000003
	.section .unidentified.08a3a2ff,"a"
	.incbin "baserom.gba", 0x00a3a2ff, 0x00000001
	.section .unidentified.08a3c9b3,"a"
	.incbin "baserom.gba", 0x00a3c9b3, 0x00000001
	.section .unidentified.08a3f0c6,"a"
	.incbin "baserom.gba", 0x00a3f0c6, 0x00000002
	.section .unidentified.08a4401b,"a"
	.incbin "baserom.gba", 0x00a4401b, 0x00000001
	.section .unidentified.08a48345,"a"
	.incbin "baserom.gba", 0x00a48345, 0x00000003
	.section .unidentified.08a48f32,"a"
	.incbin "baserom.gba", 0x00a48f32, 0x00000002
	.section .unidentified.08a4dae7,"a"
	.incbin "baserom.gba", 0x00a4dae7, 0x00000001
	.section .unidentified.08a4fe51,"a"
	.incbin "baserom.gba", 0x00a4fe51, 0x00000003
	.section .unidentified.08a517f3,"a"
	.incbin "baserom.gba", 0x00a517f3, 0x00000001
	.section .unidentified.08a560b5,"a"
	.incbin "baserom.gba", 0x00a560b5, 0x00000003
	.section .unidentified.08a594c5,"a"
	.incbin "baserom.gba", 0x00a594c5, 0x00000003
	.section .unidentified.08a5d58d,"a"
	.incbin "baserom.gba", 0x00a5d58d, 0x00000003
	.section .unidentified.08a60c56,"a"
	.incbin "baserom.gba", 0x00a60c56, 0x00000002
	.section .unidentified.08a68c27,"a"
	.incbin "baserom.gba", 0x00a68c27, 0x00000001
	.section .unidentified.08a6ff37,"a"
	.incbin "baserom.gba", 0x00a6ff37, 0x00000001
	.section .unidentified.08a74eb7,"a"
	.incbin "baserom.gba", 0x00a74eb7, 0x00000001
	.section .unidentified.08a79939,"a"
	.incbin "baserom.gba", 0x00a79939, 0x00000003
	.global Resource_Data26D
Resource_Data26D:
	.incbin "baserom.gba", 0x00a7993c, 0x0000000c
	.global Resource_Data26E
Resource_Data26E:
	.incbin "baserom.gba", 0x00a79948, 0x00000150
	.global Resource_Data26F
Resource_Data26F:
	.incbin "baserom.gba", 0x00a79a98, 0x00000140
	.global Resource_Data270
Resource_Data270:
	.incbin "baserom.gba", 0x00a79bd8, 0x00000140
	.global Resource_Data271
Resource_Data271:
	.incbin "baserom.gba", 0x00a79d18, 0x00000140
	.section .unidentified.08a7b0b5,"a"
	.incbin "baserom.gba", 0x00a7b0b5, 0x00000003
	.section .unidentified.08a7b286,"a"
	.incbin "baserom.gba", 0x00a7b286, 0x00000002
	.section .unidentified.08a7d327,"a"
	.incbin "baserom.gba", 0x00a7d327, 0x00000001
	.section .unidentified.08a7e2b4,"a"
	.global Resource_Data276
Resource_Data276:
	.incbin "baserom.gba", 0x00a7e2b4, 0x000022d8
	.section .unidentified.08a817e0,"a"
	.global Resource_Data278
Resource_Data278:
	.incbin "baserom.gba", 0x00a817e0, 0x0000461c
	.section .unidentified.08a85f02,"a"
	.incbin "baserom.gba", 0x00a85f02, 0x00000002
	.section .unidentified.08a870ed,"a"
	.incbin "baserom.gba", 0x00a870ed, 0x00000003
	.section .unidentified.08a88d75,"a"
	.incbin "baserom.gba", 0x00a88d75, 0x00000003
	.section .unidentified.08a89283,"a"
	.incbin "baserom.gba", 0x00a89283, 0x00000001
	.section .unidentified.08a893c3,"a"
	.incbin "baserom.gba", 0x00a893c3, 0x00000001
	.section .unidentified.08a8b046,"a"
	.incbin "baserom.gba", 0x00a8b046, 0x00000002
	.section .unidentified.08a8da1e,"a"
	.incbin "baserom.gba", 0x00a8da1e, 0x00000002
	.section .unidentified.08a901e7,"a"
	.incbin "baserom.gba", 0x00a901e7, 0x00000001
	.section .unidentified.08a91707,"a"
	.incbin "baserom.gba", 0x00a91707, 0x00000001
	.section .unidentified.08a9304b,"a"
	.incbin "baserom.gba", 0x00a9304b, 0x00000001
	.section .unidentified.08a94af9,"a"
	.incbin "baserom.gba", 0x00a94af9, 0x00000003
	.section .unidentified.08a94c6d,"a"
	.incbin "baserom.gba", 0x00a94c6d, 0x00000003
	.section .unidentified.08a97a51,"a"
	.incbin "baserom.gba", 0x00a97a51, 0x00000003
	.section .unidentified.08a9a2fb,"a"
	.incbin "baserom.gba", 0x00a9a2fb, 0x00000001
	.section .unidentified.08a9e792,"a"
	.incbin "baserom.gba", 0x00a9e792, 0x00000002
	.section .unidentified.08a9ffc9,"a"
	.incbin "baserom.gba", 0x00a9ffc9, 0x00000003
	.section .unidentified.08aa1b0a,"a"
	.incbin "baserom.gba", 0x00aa1b0a, 0x00000002
	.section .unidentified.08aa4607,"a"
	.incbin "baserom.gba", 0x00aa4607, 0x00000001
	.section .unidentified.08aa66e5,"a"
	.incbin "baserom.gba", 0x00aa66e5, 0x00000003
	.section .unidentified.08aa8931,"a"
	.incbin "baserom.gba", 0x00aa8931, 0x00000003
	.section .unidentified.08aa8a73,"a"
	.incbin "baserom.gba", 0x00aa8a73, 0x00000001
	.section .unidentified.08aaa146,"a"
	.incbin "baserom.gba", 0x00aaa146, 0x00000002
	.section .unidentified.08aaa246,"a"
	.incbin "baserom.gba", 0x00aaa246, 0x00000002
	.section .unidentified.08aac139,"a"
	.incbin "baserom.gba", 0x00aac139, 0x00000003
	.section .unidentified.08aadb11,"a"
	.incbin "baserom.gba", 0x00aadb11, 0x00000003
	.section .unidentified.08ab0542,"a"
	.incbin "baserom.gba", 0x00ab0542, 0x00000002
	.section .unidentified.08ab5793,"a"
	.incbin "baserom.gba", 0x00ab5793, 0x00000001
	.section .unidentified.08ab80d7,"a"
	.incbin "baserom.gba", 0x00ab80d7, 0x00000001
	.section .unidentified.08ab95aa,"a"
	.incbin "baserom.gba", 0x00ab95aa, 0x00000002
	.section .unidentified.08abe375,"a"
	.incbin "baserom.gba", 0x00abe375, 0x00000003
	.section .unidentified.08ac101f,"a"
	.incbin "baserom.gba", 0x00ac101f, 0x00000001
	.section .unidentified.08ac4257,"a"
	.incbin "baserom.gba", 0x00ac4257, 0x00000001
	.section .unidentified.08ac43ef,"a"
	.incbin "baserom.gba", 0x00ac43ef, 0x00000001
	.section .unidentified.08ac71ef,"a"
	.incbin "baserom.gba", 0x00ac71ef, 0x00000001
	.section .unidentified.08ac89a3,"a"
	.incbin "baserom.gba", 0x00ac89a3, 0x00000001
	.section .unidentified.08ac993e,"a"
	.incbin "baserom.gba", 0x00ac993e, 0x00000002
	.section .unidentified.08acb10a,"a"
	.incbin "baserom.gba", 0x00acb10a, 0x00000002
	.section .unidentified.08acbf9f,"a"
	.incbin "baserom.gba", 0x00acbf9f, 0x00000001
	.section .unidentified.08acc146,"a"
	.incbin "baserom.gba", 0x00acc146, 0x00000002
	.section .unidentified.08acf0ff,"a"
	.incbin "baserom.gba", 0x00acf0ff, 0x00000001
	.section .unidentified.08acfda5,"a"
	.incbin "baserom.gba", 0x00acfda5, 0x00000003
	.section .unidentified.08ad1375,"a"
	.incbin "baserom.gba", 0x00ad1375, 0x00000003
	.section .unidentified.08ad27cb,"a"
	.incbin "baserom.gba", 0x00ad27cb, 0x00000001
	.section .unidentified.08ad329b,"a"
	.incbin "baserom.gba", 0x00ad329b, 0x00000001
	.section .unidentified.08ad3ddb,"a"
	.incbin "baserom.gba", 0x00ad3ddb, 0x00000001
	.section .unidentified.08ad5545,"a"
	.incbin "baserom.gba", 0x00ad5545, 0x00000003
	.section .unidentified.08ad682d,"a"
	.incbin "baserom.gba", 0x00ad682d, 0x00000003
	.section .unidentified.08ad76bb,"a"
	.incbin "baserom.gba", 0x00ad76bb, 0x00000001
	.section .unidentified.08ad9341,"a"
	.incbin "baserom.gba", 0x00ad9341, 0x00000003
	.section .unidentified.08adae27,"a"
	.incbin "baserom.gba", 0x00adae27, 0x00000001
	.section .unidentified.08adc311,"a"
	.incbin "baserom.gba", 0x00adc311, 0x00000003
	.section .unidentified.08ae253b,"a"
	.incbin "baserom.gba", 0x00ae253b, 0x00000001
	.section .unidentified.08ae2a0d,"a"
	.incbin "baserom.gba", 0x00ae2a0d, 0x00000003
	.section .unidentified.08ae3b55,"a"
	.incbin "baserom.gba", 0x00ae3b55, 0x00000003
	.section .unidentified.08ae3d06,"a"
	.incbin "baserom.gba", 0x00ae3d06, 0x00000002
	.section .unidentified.08ae7af7,"a"
	.incbin "baserom.gba", 0x00ae7af7, 0x00000001
	.section .unidentified.08ae8633,"a"
	.incbin "baserom.gba", 0x00ae8633, 0x00000001
	.section .unidentified.08ae9771,"a"
	.incbin "baserom.gba", 0x00ae9771, 0x00000003
	.section .unidentified.08aeb53e,"a"
	.incbin "baserom.gba", 0x00aeb53e, 0x00000002
	.section .unidentified.08aed52e,"a"
	.incbin "baserom.gba", 0x00aed52e, 0x00000002
	.section .unidentified.08aedff1,"a"
	.incbin "baserom.gba", 0x00aedff1, 0x00000003
	.section .unidentified.08aef9f3,"a"
	.incbin "baserom.gba", 0x00aef9f3, 0x00000001
	.section .unidentified.08aefb13,"a"
	.incbin "baserom.gba", 0x00aefb13, 0x00000001
	.section .unidentified.08af1eb3,"a"
	.incbin "baserom.gba", 0x00af1eb3, 0x00000001
	.section .unidentified.08af30ff,"a"
	.incbin "baserom.gba", 0x00af30ff, 0x00000001
	.section .unidentified.08af548f,"a"
	.incbin "baserom.gba", 0x00af548f, 0x00000001
	.section .unidentified.08af6d8d,"a"
	.incbin "baserom.gba", 0x00af6d8d, 0x00000003
	.section .unidentified.08af7d7e,"a"
	.incbin "baserom.gba", 0x00af7d7e, 0x00000002
	.section .unidentified.08afa31a,"a"
	.incbin "baserom.gba", 0x00afa31a, 0x00000002
	.section .unidentified.08afe942,"a"
	.incbin "baserom.gba", 0x00afe942, 0x00000002
	.section .unidentified.08aff007,"a"
	.incbin "baserom.gba", 0x00aff007, 0x00000001
	.section .unidentified.08b01a9d,"a"
	.incbin "baserom.gba", 0x00b01a9d, 0x00000003
	.section .unidentified.08b01bee,"a"
	.incbin "baserom.gba", 0x00b01bee, 0x00000002
	.section .unidentified.08b03ffe,"a"
	.incbin "baserom.gba", 0x00b03ffe, 0x00000002
	.section .unidentified.08b0617f,"a"
	.incbin "baserom.gba", 0x00b0617f, 0x00000001
	.section .unidentified.08b062bf,"a"
	.incbin "baserom.gba", 0x00b062bf, 0x00000001
	.section .unidentified.08b08d5e,"a"
	.incbin "baserom.gba", 0x00b08d5e, 0x00000002
	.section .unidentified.08b08eaf,"a"
	.incbin "baserom.gba", 0x00b08eaf, 0x00000001
	.section .unidentified.08b0b2be,"a"
	.incbin "baserom.gba", 0x00b0b2be, 0x00000002
	.section .unidentified.08b0d43f,"a"
	.incbin "baserom.gba", 0x00b0d43f, 0x00000001
	.section .unidentified.08b0d57f,"a"
	.incbin "baserom.gba", 0x00b0d57f, 0x00000001
	.section .unidentified.08b10bae,"a"
	.incbin "baserom.gba", 0x00b10bae, 0x00000002
	.section .unidentified.08b13112,"a"
	.incbin "baserom.gba", 0x00b13112, 0x00000002
	.section .unidentified.08b15293,"a"
	.incbin "baserom.gba", 0x00b15293, 0x00000001
	.section .unidentified.08b153d3,"a"
	.incbin "baserom.gba", 0x00b153d3, 0x00000001
	.section .unidentified.08b16883,"a"
	.incbin "baserom.gba", 0x00b16883, 0x00000001
	.section .unidentified.08b1698e,"a"
	.incbin "baserom.gba", 0x00b1698e, 0x00000002
	.section .unidentified.08b184e6,"a"
	.incbin "baserom.gba", 0x00b184e6, 0x00000002
	.section .unidentified.08b19b89,"a"
	.incbin "baserom.gba", 0x00b19b89, 0x00000003
	.section .unidentified.08b19d4a,"a"
	.incbin "baserom.gba", 0x00b19d4a, 0x00000002
	.global Resource_Data2EF
Resource_Data2EF:
	.incbin "baserom.gba", 0x00b19d4c, 0x00000d9c
	.section .unidentified.08b1c73e,"a"
	.incbin "baserom.gba", 0x00b1c73e, 0x00000002
	.section .unidentified.08b1df95,"a"
	.incbin "baserom.gba", 0x00b1df95, 0x00000003
	.section .unidentified.08b1e156,"a"
	.incbin "baserom.gba", 0x00b1e156, 0x00000002
	.section .unidentified.08b223d1,"a"
	.incbin "baserom.gba", 0x00b223d1, 0x00000003
	.section .unidentified.08b22592,"a"
	.incbin "baserom.gba", 0x00b22592, 0x00000002
	.section .unidentified.08b23009,"a"
	.incbin "baserom.gba", 0x00b23009, 0x00000003
	.section .unidentified.08b23111,"a"
	.incbin "baserom.gba", 0x00b23111, 0x00000003
	.section .unidentified.08b24dd2,"a"
	.incbin "baserom.gba", 0x00b24dd2, 0x00000002
	.section .unidentified.08b2655f,"a"
	.incbin "baserom.gba", 0x00b2655f, 0x00000001
	.section .unidentified.08b26815,"a"
	.incbin "baserom.gba", 0x00b26815, 0x00000003
	.section .unidentified.08b28325,"a"
	.incbin "baserom.gba", 0x00b28325, 0x00000003
	.section .unidentified.08b2abb2,"a"
	.incbin "baserom.gba", 0x00b2abb2, 0x00000002
	.section .unidentified.08b2d3a2,"a"
	.incbin "baserom.gba", 0x00b2d3a2, 0x00000002
	.section .unidentified.08b2e342,"a"
	.incbin "baserom.gba", 0x00b2e342, 0x00000002
	.section .unidentified.08b30495,"a"
	.incbin "baserom.gba", 0x00b30495, 0x00000003
	.section .unidentified.08b33b22,"a"
	.incbin "baserom.gba", 0x00b33b22, 0x00000002
	.section .unidentified.08b358b7,"a"
	.incbin "baserom.gba", 0x00b358b7, 0x00000001
	.section .unidentified.08b376d5,"a"
	.incbin "baserom.gba", 0x00b376d5, 0x00000003
	.section .unidentified.08b391b1,"a"
	.incbin "baserom.gba", 0x00b391b1, 0x00000003
	.section .unidentified.08b3a79a,"a"
	.incbin "baserom.gba", 0x00b3a79a, 0x00000002
	.section .unidentified.08b3cd82,"a"
	.incbin "baserom.gba", 0x00b3cd82, 0x00000002
	.section .unidentified.08b3cefb,"a"
	.incbin "baserom.gba", 0x00b3cefb, 0x00000001
	.section .unidentified.08b3e3ba,"a"
	.incbin "baserom.gba", 0x00b3e3ba, 0x00000002
	.section .unidentified.08b3fbbe,"a"
	.incbin "baserom.gba", 0x00b3fbbe, 0x00000002
	.section .unidentified.08b41536,"a"
	.incbin "baserom.gba", 0x00b41536, 0x00000002
	.section .unidentified.08b42456,"a"
	.incbin "baserom.gba", 0x00b42456, 0x00000002
	.section .unidentified.08b43f02,"a"
	.incbin "baserom.gba", 0x00b43f02, 0x00000002
	.section .unidentified.08b4400f,"a"
	.incbin "baserom.gba", 0x00b4400f, 0x00000001
	.section .unidentified.08b4613f,"a"
	.incbin "baserom.gba", 0x00b4613f, 0x00000001
	.section .unidentified.08b47d15,"a"
	.incbin "baserom.gba", 0x00b47d15, 0x00000003
	.section .unidentified.08b48327,"a"
	.incbin "baserom.gba", 0x00b48327, 0x00000001
	.section .unidentified.08b48467,"a"
	.incbin "baserom.gba", 0x00b48467, 0x00000001
	.section .unidentified.08b4ab67,"a"
	.incbin "baserom.gba", 0x00b4ab67, 0x00000001
	.section .unidentified.08b4c3c6,"a"
	.incbin "baserom.gba", 0x00b4c3c6, 0x00000002
	.section .unidentified.08b4c9d7,"a"
	.incbin "baserom.gba", 0x00b4c9d7, 0x00000001
	.section .unidentified.08b4cb17,"a"
	.incbin "baserom.gba", 0x00b4cb17, 0x00000001
	.section .unidentified.08b4d992,"a"
	.incbin "baserom.gba", 0x00b4d992, 0x00000002
	.section .unidentified.08b4daa9,"a"
	.incbin "baserom.gba", 0x00b4daa9, 0x00000003
	.section .unidentified.08b4fd7f,"a"
	.incbin "baserom.gba", 0x00b4fd7f, 0x00000001
	.section .unidentified.08b519d2,"a"
	.incbin "baserom.gba", 0x00b519d2, 0x00000002
	.section .unidentified.08b52017,"a"
	.incbin "baserom.gba", 0x00b52017, 0x00000001
	.section .unidentified.08b52157,"a"
	.incbin "baserom.gba", 0x00b52157, 0x00000001
	.section .unidentified.08b52d5d,"a"
	.incbin "baserom.gba", 0x00b52d5d, 0x00000003
	.section .unidentified.08b5530f,"a"
	.incbin "baserom.gba", 0x00b5530f, 0x00000001
	.section .unidentified.08b56fce,"a"
	.incbin "baserom.gba", 0x00b56fce, 0x00000002
	.section .unidentified.08b58a9a,"a"
	.incbin "baserom.gba", 0x00b58a9a, 0x00000002
	.section .unidentified.08b59b6e,"a"
	.incbin "baserom.gba", 0x00b59b6e, 0x00000002
	.section .unidentified.08b59cd5,"a"
	.incbin "baserom.gba", 0x00b59cd5, 0x00000003
	.section .unidentified.08b5aeee,"a"
	.incbin "baserom.gba", 0x00b5aeee, 0x00000002
	.section .unidentified.08b5bacd,"a"
	.incbin "baserom.gba", 0x00b5bacd, 0x00000003
	.section .unidentified.08b5bc3a,"a"
	.incbin "baserom.gba", 0x00b5bc3a, 0x00000002
	.section .unidentified.08b5ea8b,"a"
	.incbin "baserom.gba", 0x00b5ea8b, 0x00000001
	.section .unidentified.08b6124a,"a"
	.incbin "baserom.gba", 0x00b6124a, 0x00000002
	.section .unidentified.08b672fb,"a"
	.incbin "baserom.gba", 0x00b672fb, 0x00000001
	.section .unidentified.08b68e2b,"a"
	.incbin "baserom.gba", 0x00b68e2b, 0x00000001
	.section .unidentified.08b6b162,"a"
	.incbin "baserom.gba", 0x00b6b162, 0x00000002
	.section .unidentified.08b6c313,"a"
	.incbin "baserom.gba", 0x00b6c313, 0x00000001
	.section .unidentified.08b6dc35,"a"
	.incbin "baserom.gba", 0x00b6dc35, 0x00000003
	.section .unidentified.08b6fac1,"a"
	.incbin "baserom.gba", 0x00b6fac1, 0x00000003
	.section .unidentified.08b71ca2,"a"
	.incbin "baserom.gba", 0x00b71ca2, 0x00000002
	.section .unidentified.08b74496,"a"
	.incbin "baserom.gba", 0x00b74496, 0x00000002
	.section .unidentified.08b75669,"a"
	.incbin "baserom.gba", 0x00b75669, 0x00000003
	.section .unidentified.08b75755,"a"
	.incbin "baserom.gba", 0x00b75755, 0x00000003
	.section .unidentified.08b7898e,"a"
	.incbin "baserom.gba", 0x00b7898e, 0x00000002
	.section .unidentified.08b79005,"a"
	.incbin "baserom.gba", 0x00b79005, 0x00000003
	.section .unidentified.08b7cdc7,"a"
	.incbin "baserom.gba", 0x00b7cdc7, 0x00000001
	.section .unidentified.08b7f059,"a"
	.incbin "baserom.gba", 0x00b7f059, 0x00000003
	.section .unidentified.08b80a76,"a"
	.incbin "baserom.gba", 0x00b80a76, 0x00000002
	.section .unidentified.08b81b83,"a"
	.incbin "baserom.gba", 0x00b81b83, 0x00000001
	.section .unidentified.08b84d22,"a"
	.incbin "baserom.gba", 0x00b84d22, 0x00000002
	.section .unidentified.08b85f1a,"a"
	.incbin "baserom.gba", 0x00b85f1a, 0x00000002
	.section .unidentified.08b86ddd,"a"
	.incbin "baserom.gba", 0x00b86ddd, 0x00000003
	.section .unidentified.08b88e7e,"a"
	.incbin "baserom.gba", 0x00b88e7e, 0x00000002
	.section .unidentified.08b8a8d3,"a"
	.incbin "baserom.gba", 0x00b8a8d3, 0x00000001
	.section .unidentified.08b8bd9e,"a"
	.incbin "baserom.gba", 0x00b8bd9e, 0x00000002
	.section .unidentified.08b8cf55,"a"
	.incbin "baserom.gba", 0x00b8cf55, 0x00000003
	.section .unidentified.08b8dfa2,"a"
	.incbin "baserom.gba", 0x00b8dfa2, 0x00000002
	.section .unidentified.08b8e097,"a"
	.incbin "baserom.gba", 0x00b8e097, 0x00000001
	.section .unidentified.08b8f66a,"a"
	.incbin "baserom.gba", 0x00b8f66a, 0x00000002
	.section .unidentified.08b8f889,"a"
	.incbin "baserom.gba", 0x00b8f889, 0x00000003
	.section .unidentified.08b9196d,"a"
	.incbin "baserom.gba", 0x00b9196d, 0x00000003
	.section .unidentified.08b91b32,"a"
	.incbin "baserom.gba", 0x00b91b32, 0x00000002
	.section .unidentified.08b93bf5,"a"
	.incbin "baserom.gba", 0x00b93bf5, 0x00000003
	.section .unidentified.08b93d25,"a"
	.incbin "baserom.gba", 0x00b93d25, 0x00000003
	.section .unidentified.08b967de,"a"
	.incbin "baserom.gba", 0x00b967de, 0x00000002
	.section .unidentified.08b98336,"a"
	.incbin "baserom.gba", 0x00b98336, 0x00000002
	.section .unidentified.08b9927d,"a"
	.incbin "baserom.gba", 0x00b9927d, 0x00000003
	.section .unidentified.08b9ac86,"a"
	.incbin "baserom.gba", 0x00b9ac86, 0x00000002
	.section .unidentified.08b9c5aa,"a"
	.incbin "baserom.gba", 0x00b9c5aa, 0x00000002
	.section .unidentified.08b9e07a,"a"
	.incbin "baserom.gba", 0x00b9e07a, 0x00000002
	.section .unidentified.08b9f4ee,"a"
	.incbin "baserom.gba", 0x00b9f4ee, 0x00000002
	.section .unidentified.08ba0bb2,"a"
	.incbin "baserom.gba", 0x00ba0bb2, 0x00000002
	.section .unidentified.08ba20d5,"a"
	.incbin "baserom.gba", 0x00ba20d5, 0x00000003
	.section .unidentified.08ba6303,"a"
	.incbin "baserom.gba", 0x00ba6303, 0x00000001
	.section .unidentified.08ba6fb1,"a"
	.incbin "baserom.gba", 0x00ba6fb1, 0x00000003
	.section .unidentified.08ba7d31,"a"
	.incbin "baserom.gba", 0x00ba7d31, 0x00000003
	.section .unidentified.08baa9db,"a"
	.incbin "baserom.gba", 0x00baa9db, 0x00000001
	.section .unidentified.08baab2b,"a"
	.incbin "baserom.gba", 0x00baab2b, 0x00000001
	.section .unidentified.08bad636,"a"
	.incbin "baserom.gba", 0x00bad636, 0x00000002
	.section .unidentified.08baf23f,"a"
	.incbin "baserom.gba", 0x00baf23f, 0x00000001
	.section .unidentified.08bb0c46,"a"
	.incbin "baserom.gba", 0x00bb0c46, 0x00000002
	.section .unidentified.08bb2961,"a"
	.incbin "baserom.gba", 0x00bb2961, 0x00000003
	.section .unidentified.08bb2ab3,"a"
	.incbin "baserom.gba", 0x00bb2ab3, 0x00000001
	.section .unidentified.08bb44ba,"a"
	.incbin "baserom.gba", 0x00bb44ba, 0x00000002
	.section .unidentified.08bb8242,"a"
	.incbin "baserom.gba", 0x00bb8242, 0x00000002
	.section .unidentified.08bb83d5,"a"
	.incbin "baserom.gba", 0x00bb83d5, 0x00000003
	.section .unidentified.08bbd3df,"a"
	.incbin "baserom.gba", 0x00bbd3df, 0x00000001
	.section .unidentified.08bbfa61,"a"
	.incbin "baserom.gba", 0x00bbfa61, 0x00000003
	.section .unidentified.08bbfd97,"a"
	.incbin "baserom.gba", 0x00bbfd97, 0x00000001
	.section .unidentified.08bc1a23,"a"
	.incbin "baserom.gba", 0x00bc1a23, 0x00000001
	.section .unidentified.08bc3ccd,"a"
	.incbin "baserom.gba", 0x00bc3ccd, 0x00000003
	.section .unidentified.08bc61ed,"a"
	.incbin "baserom.gba", 0x00bc61ed, 0x00000003
	.section .unidentified.08bc633a,"a"
	.incbin "baserom.gba", 0x00bc633a, 0x00000002
	.section .unidentified.08bc81bf,"a"
	.incbin "baserom.gba", 0x00bc81bf, 0x00000001
	.section .unidentified.08bc9766,"a"
	.incbin "baserom.gba", 0x00bc9766, 0x00000002
	.section .unidentified.08bcb711,"a"
	.incbin "baserom.gba", 0x00bcb711, 0x00000003
	.section .unidentified.08bcc682,"a"
	.incbin "baserom.gba", 0x00bcc682, 0x00000002
	.section .unidentified.08bcf253,"a"
	.incbin "baserom.gba", 0x00bcf253, 0x00000001
	.section .unidentified.08bd48f2,"a"
	.incbin "baserom.gba", 0x00bd48f2, 0x00000002
	.section .unidentified.08bd607d,"a"
	.incbin "baserom.gba", 0x00bd607d, 0x00000003
	.section .unidentified.08bd70ef,"a"
	.incbin "baserom.gba", 0x00bd70ef, 0x00000001
	.section .unidentified.08bd7c8b,"a"
	.incbin "baserom.gba", 0x00bd7c8b, 0x00000001
	.section .unidentified.08bd7d47,"a"
	.incbin "baserom.gba", 0x00bd7d47, 0x00000001
	.section .unidentified.08bd8a86,"a"
	.incbin "baserom.gba", 0x00bd8a86, 0x00000002
	.section .unidentified.08bd9463,"a"
	.incbin "baserom.gba", 0x00bd9463, 0x00000001
	.section .unidentified.08bda07b,"a"
	.incbin "baserom.gba", 0x00bda07b, 0x00000001
	.section .unidentified.08bdbf36,"a"
	.incbin "baserom.gba", 0x00bdbf36, 0x00000002
	.section .unidentified.08bdc052,"a"
	.incbin "baserom.gba", 0x00bdc052, 0x00000002
	.section .unidentified.08bde8d3,"a"
	.incbin "baserom.gba", 0x00bde8d3, 0x00000001
	.section .unidentified.08be005e,"a"
	.incbin "baserom.gba", 0x00be005e, 0x00000002
	.section .unidentified.08be3f8e,"a"
	.incbin "baserom.gba", 0x00be3f8e, 0x00000002
	.section .unidentified.08be40d7,"a"
	.incbin "baserom.gba", 0x00be40d7, 0x00000001
	.section .unidentified.08be6787,"a"
	.incbin "baserom.gba", 0x00be6787, 0x00000001
	.section .unidentified.08be8f2a,"a"
	.incbin "baserom.gba", 0x00be8f2a, 0x00000002
	.section .unidentified.08be9cb5,"a"
	.incbin "baserom.gba", 0x00be9cb5, 0x00000003
	.section .unidentified.08becaf6,"a"
	.incbin "baserom.gba", 0x00becaf6, 0x00000002
	.section .unidentified.08bef11d,"a"
	.incbin "baserom.gba", 0x00bef11d, 0x00000003
	.section .unidentified.08bf0e09,"a"
	.incbin "baserom.gba", 0x00bf0e09, 0x00000003
	.section .unidentified.08bf2509,"a"
	.incbin "baserom.gba", 0x00bf2509, 0x00000003
	.section .unidentified.08bf3a85,"a"
	.incbin "baserom.gba", 0x00bf3a85, 0x00000003
	.section .unidentified.08bf51f7,"a"
	.incbin "baserom.gba", 0x00bf51f7, 0x00000001
	.section .unidentified.08bf7e82,"a"
	.incbin "baserom.gba", 0x00bf7e82, 0x00000002
	.section .unidentified.08bf8ece,"a"
	.incbin "baserom.gba", 0x00bf8ece, 0x00000002
	.section .unidentified.08bf9fcb,"a"
	.incbin "baserom.gba", 0x00bf9fcb, 0x00000001
	.section .unidentified.08bfb79f,"a"
	.incbin "baserom.gba", 0x00bfb79f, 0x00000001
	.section .unidentified.08bfcc63,"a"
	.incbin "baserom.gba", 0x00bfcc63, 0x00000001
	.section .unidentified.08bff196,"a"
	.incbin "baserom.gba", 0x00bff196, 0x00000002
	.section .unidentified.08bff2ca,"a"
	.incbin "baserom.gba", 0x00bff2ca, 0x00000002
	.section .unidentified.08c01a36,"a"
	.incbin "baserom.gba", 0x00c01a36, 0x00000002
	.section .unidentified.08c03809,"a"
	.incbin "baserom.gba", 0x00c03809, 0x00000003
	.section .unidentified.08c070ed,"a"
	.incbin "baserom.gba", 0x00c070ed, 0x00000003
	.section .unidentified.08c09853,"a"
	.incbin "baserom.gba", 0x00c09853, 0x00000001
	.section .unidentified.08c0b1bd,"a"
	.incbin "baserom.gba", 0x00c0b1bd, 0x00000003
	.section .unidentified.08c0d9e6,"a"
	.incbin "baserom.gba", 0x00c0d9e6, 0x00000002
	.section .unidentified.08c11dc5,"a"
	.incbin "baserom.gba", 0x00c11dc5, 0x00000003
	.section .unidentified.08c1516f,"a"
	.incbin "baserom.gba", 0x00c1516f, 0x00000001
	.section .unidentified.08c15f62,"a"
	.incbin "baserom.gba", 0x00c15f62, 0x00000002
	.section .unidentified.08c16cda,"a"
	.incbin "baserom.gba", 0x00c16cda, 0x00000002
	.section .unidentified.08c190cd,"a"
	.incbin "baserom.gba", 0x00c190cd, 0x00000003
	.section .unidentified.08c1ee02,"a"
	.incbin "baserom.gba", 0x00c1ee02, 0x00000002
	.section .unidentified.08c25181,"a"
	.incbin "baserom.gba", 0x00c25181, 0x00000003
	.section .unidentified.08c25b87,"a"
	.incbin "baserom.gba", 0x00c25b87, 0x00000001
	.section .unidentified.08c27635,"a"
	.incbin "baserom.gba", 0x00c27635, 0x00000003
	.global Resource_Data3CC
Resource_Data3CC:
	.incbin "baserom.gba", 0x00c27638, 0x00001490
	.section .unidentified.08c28f2f,"a"
	.incbin "baserom.gba", 0x00c28f2f, 0x00000001
	.section .unidentified.08c290ee,"a"
	.incbin "baserom.gba", 0x00c290ee, 0x00000002
	.section .unidentified.08c2b8f6,"a"
	.incbin "baserom.gba", 0x00c2b8f6, 0x00000002
	.section .unidentified.08c2ba4e,"a"
	.incbin "baserom.gba", 0x00c2ba4e, 0x00000002
	.section .unidentified.08c2e395,"a"
	.incbin "baserom.gba", 0x00c2e395, 0x00000003
	.section .unidentified.08c305b9,"a"
	.incbin "baserom.gba", 0x00c305b9, 0x00000003
	.section .unidentified.08c31aae,"a"
	.incbin "baserom.gba", 0x00c31aae, 0x00000002
	.section .unidentified.08c33d11,"a"
	.incbin "baserom.gba", 0x00c33d11, 0x00000003
	.section .unidentified.08c365bf,"a"
	.incbin "baserom.gba", 0x00c365bf, 0x00000001
	.section .unidentified.08c38552,"a"
	.incbin "baserom.gba", 0x00c38552, 0x00000002
	.section .unidentified.08c39573,"a"
	.incbin "baserom.gba", 0x00c39573, 0x00000001
	.section .unidentified.08c396b3,"a"
	.incbin "baserom.gba", 0x00c396b3, 0x00000001
	.section .unidentified.08c3c2d2,"a"
	.incbin "baserom.gba", 0x00c3c2d2, 0x00000002
	.section .unidentified.08c3e756,"a"
	.incbin "baserom.gba", 0x00c3e756, 0x00000002
	.section .unidentified.08c4083d,"a"
	.incbin "baserom.gba", 0x00c4083d, 0x00000003
	.section .unidentified.08c420a1,"a"
	.incbin "baserom.gba", 0x00c420a1, 0x00000003
	.section .unidentified.08c44607,"a"
	.incbin "baserom.gba", 0x00c44607, 0x00000001
	.section .unidentified.08c4476d,"a"
	.incbin "baserom.gba", 0x00c4476d, 0x00000003
	.section .unidentified.08c46f67,"a"
	.incbin "baserom.gba", 0x00c46f67, 0x00000001
	.section .unidentified.08c48f9b,"a"
	.incbin "baserom.gba", 0x00c48f9b, 0x00000001
	.section .unidentified.08c4abc2,"a"
	.incbin "baserom.gba", 0x00c4abc2, 0x00000002
	.section .unidentified.08c4d5e2,"a"
	.incbin "baserom.gba", 0x00c4d5e2, 0x00000002
	.section .unidentified.08c4e14d,"a"
	.incbin "baserom.gba", 0x00c4e14d, 0x00000003
	.section .unidentified.08c4e233,"a"
	.incbin "baserom.gba", 0x00c4e233, 0x00000001
	.section .unidentified.08c4fb7d,"a"
	.incbin "baserom.gba", 0x00c4fb7d, 0x00000003
	.section .unidentified.08c514e3,"a"
	.incbin "baserom.gba", 0x00c514e3, 0x00000001
	.section .unidentified.08c528b1,"a"
	.incbin "baserom.gba", 0x00c528b1, 0x00000003
	.section .unidentified.08c52997,"a"
	.incbin "baserom.gba", 0x00c52997, 0x00000001
	.section .unidentified.08c53505,"a"
	.incbin "baserom.gba", 0x00c53505, 0x00000003
	.section .unidentified.08c535eb,"a"
	.incbin "baserom.gba", 0x00c535eb, 0x00000001
	.section .unidentified.08c5434f,"a"
	.incbin "baserom.gba", 0x00c5434f, 0x00000001
	.section .unidentified.08c54433,"a"
	.incbin "baserom.gba", 0x00c54433, 0x00000001
	.section .unidentified.08c54c39,"a"
	.incbin "baserom.gba", 0x00c54c39, 0x00000003
	.section .unidentified.08c54d1f,"a"
	.incbin "baserom.gba", 0x00c54d1f, 0x00000001
	.section .unidentified.08c5587f,"a"
	.incbin "baserom.gba", 0x00c5587f, 0x00000001
	.section .unidentified.08c560fd,"a"
	.incbin "baserom.gba", 0x00c560fd, 0x00000003
	.section .unidentified.08c561fa,"a"
	.incbin "baserom.gba", 0x00c561fa, 0x00000002
	.section .unidentified.08c58053,"a"
	.incbin "baserom.gba", 0x00c58053, 0x00000001
	.section .unidentified.08c58e61,"a"
	.incbin "baserom.gba", 0x00c58e61, 0x00000003
	.section .unidentified.08c5a0f5,"a"
	.incbin "baserom.gba", 0x00c5a0f5, 0x00000003
	.section .unidentified.08c5b0d1,"a"
	.incbin "baserom.gba", 0x00c5b0d1, 0x00000003
	.section .unidentified.08c5d4d7,"a"
	.incbin "baserom.gba", 0x00c5d4d7, 0x00000001
	.section .unidentified.08c5d617,"a"
	.incbin "baserom.gba", 0x00c5d617, 0x00000001
	.section .unidentified.08c5dce5,"a"
	.incbin "baserom.gba", 0x00c5dce5, 0x00000003
	.section .unidentified.08c5ddbb,"a"
	.incbin "baserom.gba", 0x00c5ddbb, 0x00000001
	.section .unidentified.08c5e631,"a"
	.incbin "baserom.gba", 0x00c5e631, 0x00000003
	.section .unidentified.08c5eae2,"a"
	.incbin "baserom.gba", 0x00c5eae2, 0x00000002
	.section .unidentified.08c5ff5d,"a"
	.incbin "baserom.gba", 0x00c5ff5d, 0x00000003
	.section .unidentified.08c60773,"a"
	.incbin "baserom.gba", 0x00c60773, 0x00000001
	.section .unidentified.08c61409,"a"
	.incbin "baserom.gba", 0x00c61409, 0x00000003
	.section .unidentified.08c615ae,"a"
	.incbin "baserom.gba", 0x00c615ae, 0x00000002
	.section .unidentified.08c64472,"a"
	.incbin "baserom.gba", 0x00c64472, 0x00000002
	.section .unidentified.08c65df1,"a"
	.incbin "baserom.gba", 0x00c65df1, 0x00000003
	.section .unidentified.08c66e3e,"a"
	.incbin "baserom.gba", 0x00c66e3e, 0x00000002
	.section .unidentified.08c694af,"a"
	.incbin "baserom.gba", 0x00c694af, 0x00000001
	.section .unidentified.08c6a235,"a"
	.incbin "baserom.gba", 0x00c6a235, 0x00000003
	.section .unidentified.08c6b099,"a"
	.incbin "baserom.gba", 0x00c6b099, 0x00000003
	.section .unidentified.08c6e583,"a"
	.incbin "baserom.gba", 0x00c6e583, 0x00000001
	.section .unidentified.08c75d77,"a"
	.incbin "baserom.gba", 0x00c75d77, 0x00000001
	.section .unidentified.08c7c371,"a"
	.incbin "baserom.gba", 0x00c7c371, 0x00000003
	.section .unidentified.08c7edaa,"a"
	.incbin "baserom.gba", 0x00c7edaa, 0x00000002
	.section .unidentified.08c8167b,"a"
	.incbin "baserom.gba", 0x00c8167b, 0x00000001
	.section .unidentified.08c85ec7,"a"
	.incbin "baserom.gba", 0x00c85ec7, 0x00000001
	.section .unidentified.08c86ec5,"a"
	.incbin "baserom.gba", 0x00c86ec5, 0x00000003
	.section .unidentified.08c86fde,"a"
	.incbin "baserom.gba", 0x00c86fde, 0x00000002
	.section .unidentified.08c88586,"a"
	.incbin "baserom.gba", 0x00c88586, 0x00000002
	.section .unidentified.08c89fe3,"a"
	.incbin "baserom.gba", 0x00c89fe3, 0x00000001
	.section .unidentified.08c8a11e,"a"
	.incbin "baserom.gba", 0x00c8a11e, 0x00000002
	.section .unidentified.08c8b625,"a"
	.incbin "baserom.gba", 0x00c8b625, 0x00000003
	.section .unidentified.08c8e8ae,"a"
	.incbin "baserom.gba", 0x00c8e8ae, 0x00000002
	.section .unidentified.08c8fcf5,"a"
	.incbin "baserom.gba", 0x00c8fcf5, 0x00000003
	.section .unidentified.08c929ed,"a"
	.incbin "baserom.gba", 0x00c929ed, 0x00000003
	.section .unidentified.08c94e23,"a"
	.incbin "baserom.gba", 0x00c94e23, 0x00000001
	.section .unidentified.08c94fff,"a"
	.incbin "baserom.gba", 0x00c94fff, 0x00000001
	.section .unidentified.08c98113,"a"
	.incbin "baserom.gba", 0x00c98113, 0x00000001
	.section .unidentified.08c99596,"a"
	.incbin "baserom.gba", 0x00c99596, 0x00000002
	.section .unidentified.08c9a5e5,"a"
	.incbin "baserom.gba", 0x00c9a5e5, 0x00000003
	.section .unidentified.08c9c44a,"a"
	.incbin "baserom.gba", 0x00c9c44a, 0x00000002
	.section .unidentified.08c9c5e5,"a"
	.incbin "baserom.gba", 0x00c9c5e5, 0x00000003
	.section .unidentified.08c9c743,"a"
	.incbin "baserom.gba", 0x00c9c743, 0x00000001
	.section .unidentified.08c9d6c2,"a"
	.incbin "baserom.gba", 0x00c9d6c2, 0x00000002
	.section .unidentified.08c9d7da,"a"
	.incbin "baserom.gba", 0x00c9d7da, 0x00000002
	.section .unidentified.08c9f86d,"a"
	.incbin "baserom.gba", 0x00c9f86d, 0x00000003
	.section .unidentified.08ca16ef,"a"
	.incbin "baserom.gba", 0x00ca16ef, 0x00000001
	.section .unidentified.08ca3c15,"a"
	.incbin "baserom.gba", 0x00ca3c15, 0x00000003
	.section .unidentified.08ca3d22,"a"
	.incbin "baserom.gba", 0x00ca3d22, 0x00000002
	.section .unidentified.08ca5db5,"a"
	.incbin "baserom.gba", 0x00ca5db5, 0x00000003
	.section .unidentified.08ca928b,"a"
	.incbin "baserom.gba", 0x00ca928b, 0x00000001
	.section .unidentified.08ca9d75,"a"
	.incbin "baserom.gba", 0x00ca9d75, 0x00000003
	.section .unidentified.08ca9ee5,"a"
	.incbin "baserom.gba", 0x00ca9ee5, 0x00000003
	.section .unidentified.08cacdff,"a"
	.incbin "baserom.gba", 0x00cacdff, 0x00000001
	.section .unidentified.08caed8a,"a"
	.incbin "baserom.gba", 0x00caed8a, 0x00000002
	.section .unidentified.08cb06f5,"a"
	.incbin "baserom.gba", 0x00cb06f5, 0x00000003
	.section .unidentified.08cb12dd,"a"
	.incbin "baserom.gba", 0x00cb12dd, 0x00000003
	.section .unidentified.08cb2f4f,"a"
	.incbin "baserom.gba", 0x00cb2f4f, 0x00000001
	.section .unidentified.08cb5327,"a"
	.incbin "baserom.gba", 0x00cb5327, 0x00000001
	.section .unidentified.08cb61e2,"a"
	.incbin "baserom.gba", 0x00cb61e2, 0x00000002
	.section .unidentified.08cb6392,"a"
	.incbin "baserom.gba", 0x00cb6392, 0x00000002
	.section .unidentified.08cb89bf,"a"
	.incbin "baserom.gba", 0x00cb89bf, 0x00000001
	.section .unidentified.08cbad0a,"a"
	.incbin "baserom.gba", 0x00cbad0a, 0x00000002
	.section .unidentified.08cbae62,"a"
	.incbin "baserom.gba", 0x00cbae62, 0x00000002
	.section .unidentified.08cbe42f,"a"
	.incbin "baserom.gba", 0x00cbe42f, 0x00000001
	.section .unidentified.08cbfcb1,"a"
	.incbin "baserom.gba", 0x00cbfcb1, 0x00000003
	.section .unidentified.08cc19d9,"a"
	.incbin "baserom.gba", 0x00cc19d9, 0x00000003
	.section .unidentified.08cc50fa,"a"
	.incbin "baserom.gba", 0x00cc50fa, 0x00000002
	.section .unidentified.08cca283,"a"
	.incbin "baserom.gba", 0x00cca283, 0x00000001
	.section .unidentified.08ccc851,"a"
	.incbin "baserom.gba", 0x00ccc851, 0x00000003
	.section .unidentified.08cce1dd,"a"
	.incbin "baserom.gba", 0x00cce1dd, 0x00000003
	.section .unidentified.08ccf30a,"a"
	.incbin "baserom.gba", 0x00ccf30a, 0x00000002
	.section .unidentified.08ccfeba,"a"
	.incbin "baserom.gba", 0x00ccfeba, 0x00000002
	.section .unidentified.08cd1882,"a"
	.incbin "baserom.gba", 0x00cd1882, 0x00000002
	.section .unidentified.08cd1966,"a"
	.incbin "baserom.gba", 0x00cd1966, 0x00000002
	.section .unidentified.08cd3dc3,"a"
	.incbin "baserom.gba", 0x00cd3dc3, 0x00000001
	.section .unidentified.08cd3f03,"a"
	.incbin "baserom.gba", 0x00cd3f03, 0x00000001
	.section .unidentified.08cd7579,"a"
	.incbin "baserom.gba", 0x00cd7579, 0x00000003
	.section .unidentified.08cd76dd,"a"
	.incbin "baserom.gba", 0x00cd76dd, 0x00000003
	.section .unidentified.08cd9dbd,"a"
	.incbin "baserom.gba", 0x00cd9dbd, 0x00000003
	.section .unidentified.08cdcc6d,"a"
	.incbin "baserom.gba", 0x00cdcc6d, 0x00000003
	.section .unidentified.08cde1e2,"a"
	.incbin "baserom.gba", 0x00cde1e2, 0x00000002
	.section .unidentified.08ce22de,"a"
	.incbin "baserom.gba", 0x00ce22de, 0x00000002
	.section .unidentified.08ce4563,"a"
	.incbin "baserom.gba", 0x00ce4563, 0x00000001
	.section .unidentified.08ce6147,"a"
	.incbin "baserom.gba", 0x00ce6147, 0x00000001
	.section .unidentified.08ce7e3d,"a"
	.incbin "baserom.gba", 0x00ce7e3d, 0x00000003
	.section .unidentified.08ceab8d,"a"
	.incbin "baserom.gba", 0x00ceab8d, 0x00000003
	.section .unidentified.08cecbd5,"a"
	.incbin "baserom.gba", 0x00cecbd5, 0x00000003
	.section .unidentified.08cf1276,"a"
	.incbin "baserom.gba", 0x00cf1276, 0x00000002
	.section .unidentified.08cf1e05,"a"
	.incbin "baserom.gba", 0x00cf1e05, 0x00000003
	.section .unidentified.08cf5461,"a"
	.incbin "baserom.gba", 0x00cf5461, 0x00000003
	.section .unidentified.08cf5589,"a"
	.incbin "baserom.gba", 0x00cf5589, 0x00000003
	.section .unidentified.08cf779a,"a"
	.incbin "baserom.gba", 0x00cf779a, 0x00000002
	.section .unidentified.08cf792b,"a"
	.incbin "baserom.gba", 0x00cf792b, 0x00000001
	.section .unidentified.08cfeddd,"a"
	.incbin "baserom.gba", 0x00cfeddd, 0x00000003
	.section .unidentified.08d015b3,"a"
	.incbin "baserom.gba", 0x00d015b3, 0x00000001
	.section .unidentified.08d03d0e,"a"
	.incbin "baserom.gba", 0x00d03d0e, 0x00000002
	.section .unidentified.08d03ec1,"a"
	.incbin "baserom.gba", 0x00d03ec1, 0x00000003
	.section .unidentified.08d055c5,"a"
	.incbin "baserom.gba", 0x00d055c5, 0x00000003
	.section .unidentified.08d0761f,"a"
	.incbin "baserom.gba", 0x00d0761f, 0x00000001
	.section .unidentified.08d08969,"a"
	.incbin "baserom.gba", 0x00d08969, 0x00000003
	.section .unidentified.08d0a3b3,"a"
	.incbin "baserom.gba", 0x00d0a3b3, 0x00000001
	.section .unidentified.08d0a4d7,"a"
	.incbin "baserom.gba", 0x00d0a4d7, 0x00000001
	.section .unidentified.08d0a9a9,"a"
	.incbin "baserom.gba", 0x00d0a9a9, 0x00000003
	.section .unidentified.08d0cb8d,"a"
	.incbin "baserom.gba", 0x00d0cb8d, 0x00000003
	.section .unidentified.08d0d061,"a"
	.incbin "baserom.gba", 0x00d0d061, 0x00000003
	.section .unidentified.08d0f597,"a"
	.incbin "baserom.gba", 0x00d0f597, 0x00000001
	.section .unidentified.08d0f71f,"a"
	.incbin "baserom.gba", 0x00d0f71f, 0x00000001
	.section .unidentified.08d110d2,"a"
	.incbin "baserom.gba", 0x00d110d2, 0x00000002
	.section .unidentified.08d1232d,"a"
	.incbin "baserom.gba", 0x00d1232d, 0x00000003
	.section .unidentified.08d13fc1,"a"
	.incbin "baserom.gba", 0x00d13fc1, 0x00000003
	.section .unidentified.08d140f7,"a"
	.incbin "baserom.gba", 0x00d140f7, 0x00000001
	.section .unidentified.08d15d63,"a"
	.incbin "baserom.gba", 0x00d15d63, 0x00000001
	.section .unidentified.08d16f1b,"a"
	.incbin "baserom.gba", 0x00d16f1b, 0x00000001
	.section .unidentified.08d17c69,"a"
	.incbin "baserom.gba", 0x00d17c69, 0x00000003
	.section .unidentified.08d18df2,"a"
	.incbin "baserom.gba", 0x00d18df2, 0x00000002
	.section .unidentified.08d1a8eb,"a"
	.incbin "baserom.gba", 0x00d1a8eb, 0x00000001
	.section .unidentified.08d1bcf7,"a"
	.incbin "baserom.gba", 0x00d1bcf7, 0x00000001
	.section .unidentified.08d1c24b,"a"
	.incbin "baserom.gba", 0x00d1c24b, 0x00000001
	.section .unidentified.08d1c885,"a"
	.incbin "baserom.gba", 0x00d1c885, 0x00000003
	.section .unidentified.08d1d7ef,"a"
	.incbin "baserom.gba", 0x00d1d7ef, 0x00000001
	.section .unidentified.08d1f89d,"a"
	.incbin "baserom.gba", 0x00d1f89d, 0x00000003
	.section .unidentified.08d21bd5,"a"
	.incbin "baserom.gba", 0x00d21bd5, 0x00000003
	.section .unidentified.08d24236,"a"
	.incbin "baserom.gba", 0x00d24236, 0x00000002
	.section .unidentified.08d243cb,"a"
	.incbin "baserom.gba", 0x00d243cb, 0x00000001
	.section .unidentified.08d25f7f,"a"
	.incbin "baserom.gba", 0x00d25f7f, 0x00000001
	.section .unidentified.08d260df,"a"
	.incbin "baserom.gba", 0x00d260df, 0x00000001
	.section .unidentified.08d28b83,"a"
	.incbin "baserom.gba", 0x00d28b83, 0x00000001
	.global Resource_Data4B3
Resource_Data4B3:
	.incbin "baserom.gba", 0x00d28b84, 0x000021b0
	.section .unidentified.08d2cbaa,"a"
	.incbin "baserom.gba", 0x00d2cbaa, 0x00000002
	.section .unidentified.08d2cceb,"a"
	.incbin "baserom.gba", 0x00d2cceb, 0x00000001
	.section .unidentified.08d2ea0f,"a"
	.incbin "baserom.gba", 0x00d2ea0f, 0x00000001
	.section .unidentified.08d3069b,"a"
	.incbin "baserom.gba", 0x00d3069b, 0x00000001
	.section .unidentified.08d32677,"a"
	.incbin "baserom.gba", 0x00d32677, 0x00000001
	.section .unidentified.08d33c63,"a"
	.incbin "baserom.gba", 0x00d33c63, 0x00000001
	.section .unidentified.08d33da1,"a"
	.incbin "baserom.gba", 0x00d33da1, 0x00000003
	.section .unidentified.08d35d75,"a"
	.incbin "baserom.gba", 0x00d35d75, 0x00000003
	.section .unidentified.08d35e69,"a"
	.incbin "baserom.gba", 0x00d35e69, 0x00000003
	.section .unidentified.08d36f49,"a"
	.incbin "baserom.gba", 0x00d36f49, 0x00000003
	.section .unidentified.08d38e23,"a"
	.incbin "baserom.gba", 0x00d38e23, 0x00000001
	.section .unidentified.08d39ddb,"a"
	.incbin "baserom.gba", 0x00d39ddb, 0x00000001
	.section .unidentified.08d3bd22,"a"
	.incbin "baserom.gba", 0x00d3bd22, 0x00000002
	.section .unidentified.08d3d92d,"a"
	.incbin "baserom.gba", 0x00d3d92d, 0x00000003
	.section .unidentified.08d3da79,"a"
	.incbin "baserom.gba", 0x00d3da79, 0x00000003
	.section .unidentified.08d3fb37,"a"
	.incbin "baserom.gba", 0x00d3fb37, 0x00000001
	.section .unidentified.08d43c26,"a"
	.incbin "baserom.gba", 0x00d43c26, 0x00000002
	.section .unidentified.08d479d8,"a"
	.global Resource_Data4C8
Resource_Data4C8:
	.incbin "baserom.gba", 0x00d479d8, 0x00001f0c
	.global Resource_Data4C9
Resource_Data4C9:
	.incbin "baserom.gba", 0x00d498e4, 0x00002364
	.global Resource_Data4CA
Resource_Data4CA:
	.incbin "baserom.gba", 0x00d4bc48, 0x0000202c
	.global Resource_Data4CB
Resource_Data4CB:
	.incbin "baserom.gba", 0x00d4dc74, 0x0000180c
	.section .unidentified.08d50bcd,"a"
	.incbin "baserom.gba", 0x00d50bcd, 0x00000003
	.section .unidentified.08d50cfa,"a"
	.incbin "baserom.gba", 0x00d50cfa, 0x00000002
	.section .unidentified.08d5264b,"a"
	.incbin "baserom.gba", 0x00d5264b, 0x00000001
	.section .unidentified.08d53d2f,"a"
	.incbin "baserom.gba", 0x00d53d2f, 0x00000001
	.section .unidentified.08d55549,"a"
	.incbin "baserom.gba", 0x00d55549, 0x00000003
	.section .unidentified.08d5618b,"a"
	.incbin "baserom.gba", 0x00d5618b, 0x00000001
	.section .unidentified.08d57c6d,"a"
	.incbin "baserom.gba", 0x00d57c6d, 0x00000003
	.section .unidentified.08d57d72,"a"
	.incbin "baserom.gba", 0x00d57d72, 0x00000002
	.section .unidentified.08d57eb2,"a"
	.incbin "baserom.gba", 0x00d57eb2, 0x00000002
	.section .unidentified.08d5987e,"a"
	.incbin "baserom.gba", 0x00d5987e, 0x00000002
	.section .unidentified.08d59975,"a"
	.incbin "baserom.gba", 0x00d59975, 0x00000003
	.section .unidentified.08d5c7c9,"a"
	.incbin "baserom.gba", 0x00d5c7c9, 0x00000003
	.section .unidentified.08d5c972,"a"
	.incbin "baserom.gba", 0x00d5c972, 0x00000002
	.section .unidentified.08d5f58e,"a"
	.incbin "baserom.gba", 0x00d5f58e, 0x00000002
	.section .unidentified.08d622c1,"a"
	.incbin "baserom.gba", 0x00d622c1, 0x00000003
	.section .unidentified.08d63fab,"a"
	.incbin "baserom.gba", 0x00d63fab, 0x00000001
	.section .unidentified.08d67825,"a"
	.incbin "baserom.gba", 0x00d67825, 0x00000003
	.section .unidentified.08d6797d,"a"
	.incbin "baserom.gba", 0x00d6797d, 0x00000003
	.section .unidentified.08d69ec1,"a"
	.incbin "baserom.gba", 0x00d69ec1, 0x00000003
	.section .unidentified.08d6dd86,"a"
	.incbin "baserom.gba", 0x00d6dd86, 0x00000002
	.section .unidentified.08d6feb6,"a"
	.incbin "baserom.gba", 0x00d6feb6, 0x00000002
	.section .unidentified.08d740c2,"a"
	.incbin "baserom.gba", 0x00d740c2, 0x00000002
	.section .unidentified.08d74296,"a"
	.incbin "baserom.gba", 0x00d74296, 0x00000002
	.section .unidentified.08d762d6,"a"
	.incbin "baserom.gba", 0x00d762d6, 0x00000002
	.section .unidentified.08d776cd,"a"
	.incbin "baserom.gba", 0x00d776cd, 0x00000003
	.section .unidentified.08d7820a,"a"
	.incbin "baserom.gba", 0x00d7820a, 0x00000002
	.section .unidentified.08d79375,"a"
	.incbin "baserom.gba", 0x00d79375, 0x00000003
	.section .unidentified.08d7a421,"a"
	.incbin "baserom.gba", 0x00d7a421, 0x00000003
	.section .unidentified.08d7a5cd,"a"
	.incbin "baserom.gba", 0x00d7a5cd, 0x00000003
	.section .unidentified.08d7c0da,"a"
	.incbin "baserom.gba", 0x00d7c0da, 0x00000002
	.section .unidentified.08d7daf7,"a"
	.incbin "baserom.gba", 0x00d7daf7, 0x00000001
	.section .unidentified.08d7e97a,"a"
	.incbin "baserom.gba", 0x00d7e97a, 0x00000002
	.section .unidentified.08d7ffc9,"a"
	.incbin "baserom.gba", 0x00d7ffc9, 0x00000003
	.section .unidentified.08d8132b,"a"
	.incbin "baserom.gba", 0x00d8132b, 0x00000001
	.section .unidentified.08d821df,"a"
	.incbin "baserom.gba", 0x00d821df, 0x00000001
	.section .unidentified.08d837cd,"a"
	.incbin "baserom.gba", 0x00d837cd, 0x00000003
	.section .unidentified.08d84d13,"a"
	.incbin "baserom.gba", 0x00d84d13, 0x00000001
	.section .unidentified.08d85add,"a"
	.incbin "baserom.gba", 0x00d85add, 0x00000003
	.section .unidentified.08d85c4b,"a"
	.incbin "baserom.gba", 0x00d85c4b, 0x00000001
	.section .unidentified.08d86c0a,"a"
	.incbin "baserom.gba", 0x00d86c0a, 0x00000002
	.section .unidentified.08d876d3,"a"
	.incbin "baserom.gba", 0x00d876d3, 0x00000001
	.section .unidentified.08d8826d,"a"
	.incbin "baserom.gba", 0x00d8826d, 0x00000003
	.section .unidentified.08d883bd,"a"
	.incbin "baserom.gba", 0x00d883bd, 0x00000003
	.section .unidentified.08d8c66b,"a"
	.incbin "baserom.gba", 0x00d8c66b, 0x00000001
	.section .unidentified.08d8e105,"a"
	.incbin "baserom.gba", 0x00d8e105, 0x00000003
	.section .unidentified.08d8fadb,"a"
	.incbin "baserom.gba", 0x00d8fadb, 0x00000001
	.section .unidentified.08d8fc5a,"a"
	.incbin "baserom.gba", 0x00d8fc5a, 0x00000002
	.section .unidentified.08d9388a,"a"
	.incbin "baserom.gba", 0x00d9388a, 0x00000002
	.section .unidentified.08d955e7,"a"
	.incbin "baserom.gba", 0x00d955e7, 0x00000001
	.section .unidentified.08d98c11,"a"
	.incbin "baserom.gba", 0x00d98c11, 0x00000003
	.section .unidentified.08d98d62,"a"
	.incbin "baserom.gba", 0x00d98d62, 0x00000002
	.section .unidentified.08d9e95e,"a"
	.incbin "baserom.gba", 0x00d9e95e, 0x00000002
	.section .unidentified.08da129b,"a"
	.incbin "baserom.gba", 0x00da129b, 0x00000001
	.section .unidentified.08da2bdd,"a"
	.incbin "baserom.gba", 0x00da2bdd, 0x00000003
	.section .unidentified.08da2d46,"a"
	.incbin "baserom.gba", 0x00da2d46, 0x00000002
	.section .unidentified.08da9b45,"a"
	.incbin "baserom.gba", 0x00da9b45, 0x00000003
	.section .unidentified.08daa123,"a"
	.incbin "baserom.gba", 0x00daa123, 0x00000001
	.section .unidentified.08dab02b,"a"
	.incbin "baserom.gba", 0x00dab02b, 0x00000001
	.section .unidentified.08dad873,"a"
	.incbin "baserom.gba", 0x00dad873, 0x00000001
	.section .unidentified.08daee92,"a"
	.incbin "baserom.gba", 0x00daee92, 0x00000002
	.section .unidentified.08db07f3,"a"
	.incbin "baserom.gba", 0x00db07f3, 0x00000001
	.section .unidentified.08db35db,"a"
	.incbin "baserom.gba", 0x00db35db, 0x00000001
	.section .unidentified.08db3756,"a"
	.incbin "baserom.gba", 0x00db3756, 0x00000002
	.section .unidentified.08db5e87,"a"
	.incbin "baserom.gba", 0x00db5e87, 0x00000001
	.section .unidentified.08db749f,"a"
	.incbin "baserom.gba", 0x00db749f, 0x00000001
	.section .unidentified.08db9d12,"a"
	.incbin "baserom.gba", 0x00db9d12, 0x00000002
	.section .unidentified.08dbd049,"a"
	.incbin "baserom.gba", 0x00dbd049, 0x00000003
	.section .unidentified.08dbe6aa,"a"
	.incbin "baserom.gba", 0x00dbe6aa, 0x00000002
	.section .unidentified.08dbff6a,"a"
	.incbin "baserom.gba", 0x00dbff6a, 0x00000002
	.section .unidentified.08dc18b9,"a"
	.incbin "baserom.gba", 0x00dc18b9, 0x00000003
	.section .unidentified.08dc291f,"a"
	.incbin "baserom.gba", 0x00dc291f, 0x00000001
	.section .unidentified.08dc2a87,"a"
	.incbin "baserom.gba", 0x00dc2a87, 0x00000001
	.section .unidentified.08dc50aa,"a"
	.incbin "baserom.gba", 0x00dc50aa, 0x00000002
	.section .unidentified.08dc51ed,"a"
	.incbin "baserom.gba", 0x00dc51ed, 0x00000003
	.section .unidentified.08dc6aaa,"a"
	.incbin "baserom.gba", 0x00dc6aaa, 0x00000002
	.section .unidentified.08dc9a0e,"a"
	.incbin "baserom.gba", 0x00dc9a0e, 0x00000002
	.section .unidentified.08dcc3a6,"a"
	.incbin "baserom.gba", 0x00dcc3a6, 0x00000002
	.section .unidentified.08dd0cfe,"a"
	.incbin "baserom.gba", 0x00dd0cfe, 0x00000002
	.section .unidentified.08dd3327,"a"
	.incbin "baserom.gba", 0x00dd3327, 0x00000001
	.section .unidentified.08dd34fb,"a"
	.incbin "baserom.gba", 0x00dd34fb, 0x00000001
	.section .unidentified.08dd681a,"a"
	.incbin "baserom.gba", 0x00dd681a, 0x00000002
	.section .unidentified.08dd69f1,"a"
	.incbin "baserom.gba", 0x00dd69f1, 0x00000003
	.section .unidentified.08dd9d65,"a"
	.incbin "baserom.gba", 0x00dd9d65, 0x00000003
	.section .unidentified.08ddb4b9,"a"
	.incbin "baserom.gba", 0x00ddb4b9, 0x00000003
	.section .unidentified.08ddc447,"a"
	.incbin "baserom.gba", 0x00ddc447, 0x00000001
	.section .unidentified.08ddddbd,"a"
	.incbin "baserom.gba", 0x00ddddbd, 0x00000003
	.section .unidentified.08dddef7,"a"
	.incbin "baserom.gba", 0x00dddef7, 0x00000001
	.section .unidentified.08ddfa39,"a"
	.incbin "baserom.gba", 0x00ddfa39, 0x00000003
	.section .unidentified.08de3011,"a"
	.incbin "baserom.gba", 0x00de3011, 0x00000003
	.section .unidentified.08de3c9b,"a"
	.incbin "baserom.gba", 0x00de3c9b, 0x00000001
	.section .unidentified.08de3ddb,"a"
	.incbin "baserom.gba", 0x00de3ddb, 0x00000001
	.section .unidentified.08de5b9f,"a"
	.incbin "baserom.gba", 0x00de5b9f, 0x00000001
	.section .unidentified.08de5d12,"a"
	.incbin "baserom.gba", 0x00de5d12, 0x00000002
	.section .unidentified.08de817a,"a"
	.incbin "baserom.gba", 0x00de817a, 0x00000002
	.section .unidentified.08de9b16,"a"
	.incbin "baserom.gba", 0x00de9b16, 0x00000002
	.section .unidentified.08deb94b,"a"
	.incbin "baserom.gba", 0x00deb94b, 0x00000001
	.section .unidentified.08dec402,"a"
	.incbin "baserom.gba", 0x00dec402, 0x00000002
	.section .unidentified.08dede1e,"a"
	.incbin "baserom.gba", 0x00dede1e, 0x00000002
	.section .unidentified.08df0aea,"a"
	.incbin "baserom.gba", 0x00df0aea, 0x00000002
	.section .unidentified.08df0cc5,"a"
	.incbin "baserom.gba", 0x00df0cc5, 0x00000003
	.section .unidentified.08df27fb,"a"
	.incbin "baserom.gba", 0x00df27fb, 0x00000001
	.section .unidentified.08df58c5,"a"
	.incbin "baserom.gba", 0x00df58c5, 0x00000003
	.section .unidentified.08df6a81,"a"
	.incbin "baserom.gba", 0x00df6a81, 0x00000003
	.section .unidentified.08df6c69,"a"
	.incbin "baserom.gba", 0x00df6c69, 0x00000003
	.section .unidentified.08df6dfb,"a"
	.incbin "baserom.gba", 0x00df6dfb, 0x00000001
	.section .unidentified.08df7b82,"a"
	.incbin "baserom.gba", 0x00df7b82, 0x00000002
	.section .unidentified.08df7d17,"a"
	.incbin "baserom.gba", 0x00df7d17, 0x00000001
	.section .unidentified.08df9dcb,"a"
	.incbin "baserom.gba", 0x00df9dcb, 0x00000001
	.section .unidentified.08dfb0e7,"a"
	.incbin "baserom.gba", 0x00dfb0e7, 0x00000001
	.section .unidentified.08dfc2b1,"a"
	.incbin "baserom.gba", 0x00dfc2b1, 0x00000003
	.section .unidentified.08dfe8c1,"a"
	.incbin "baserom.gba", 0x00dfe8c1, 0x00000003
	.section .unidentified.08e01e4e,"a"
	.incbin "baserom.gba", 0x00e01e4e, 0x00000002
	.section .unidentified.08e01f8f,"a"
	.incbin "baserom.gba", 0x00e01f8f, 0x00000001
	.section .unidentified.08e06317,"a"
	.incbin "baserom.gba", 0x00e06317, 0x00000001
	.section .unidentified.08e06a36,"a"
	.incbin "baserom.gba", 0x00e06a36, 0x00000002
	.section .unidentified.08e07df5,"a"
	.incbin "baserom.gba", 0x00e07df5, 0x00000003
	.section .unidentified.08e08e8f,"a"
	.incbin "baserom.gba", 0x00e08e8f, 0x00000001
	.section .unidentified.08e0bf0f,"a"
	.incbin "baserom.gba", 0x00e0bf0f, 0x00000001
	.section .unidentified.08e0cf8f,"a"
	.incbin "baserom.gba", 0x00e0cf8f, 0x00000001
	.section .unidentified.08e0e011,"a"
	.incbin "baserom.gba", 0x00e0e011, 0x00000003
	.section .unidentified.08e0eb82,"a"
	.incbin "baserom.gba", 0x00e0eb82, 0x00000002
	.section .unidentified.08e0fad3,"a"
	.incbin "baserom.gba", 0x00e0fad3, 0x00000001
	.section .unidentified.08e0fc51,"a"
	.incbin "baserom.gba", 0x00e0fc51, 0x00000003
	.section .unidentified.08e131ab,"a"
	.incbin "baserom.gba", 0x00e131ab, 0x00000001
	.section .unidentified.08e147de,"a"
	.incbin "baserom.gba", 0x00e147de, 0x00000002
	.section .unidentified.08e1491f,"a"
	.incbin "baserom.gba", 0x00e1491f, 0x00000001
	.section .unidentified.08e1726a,"a"
	.incbin "baserom.gba", 0x00e1726a, 0x00000002
	.section .unidentified.08e1fc12,"a"
	.incbin "baserom.gba", 0x00e1fc12, 0x00000002
	.global Resource_Data57B
Resource_Data57B:
	.incbin "baserom.gba", 0x00e1fc14, 0x00000fdc
	.section .unidentified.08e227bd,"a"
	.incbin "baserom.gba", 0x00e227bd, 0x00000003
	.section .unidentified.08e246fd,"a"
	.incbin "baserom.gba", 0x00e246fd, 0x00000003
	.section .unidentified.08e25b71,"a"
	.incbin "baserom.gba", 0x00e25b71, 0x00000003
	.section .unidentified.08e2782b,"a"
	.incbin "baserom.gba", 0x00e2782b, 0x00000001
	.section .unidentified.08e29133,"a"
	.incbin "baserom.gba", 0x00e29133, 0x00000001
	.section .unidentified.08e2aa79,"a"
	.incbin "baserom.gba", 0x00e2aa79, 0x00000003
	.section .unidentified.08e2c12b,"a"
	.incbin "baserom.gba", 0x00e2c12b, 0x00000001
	.section .unidentified.08e2d9d5,"a"
	.incbin "baserom.gba", 0x00e2d9d5, 0x00000003
	.section .unidentified.08e2ec7e,"a"
	.incbin "baserom.gba", 0x00e2ec7e, 0x00000002
	.section .unidentified.08e30453,"a"
	.incbin "baserom.gba", 0x00e30453, 0x00000001
	.section .unidentified.08e315e5,"a"
	.incbin "baserom.gba", 0x00e315e5, 0x00000003
	.section .unidentified.08e32bc3,"a"
	.incbin "baserom.gba", 0x00e32bc3, 0x00000001
	.section .unidentified.08e348da,"a"
	.incbin "baserom.gba", 0x00e348da, 0x00000002
	.section .unidentified.08e34a19,"a"
	.incbin "baserom.gba", 0x00e34a19, 0x00000003
	.section .unidentified.08e3693e,"a"
	.incbin "baserom.gba", 0x00e3693e, 0x00000002
	.section .unidentified.08e36a67,"a"
	.incbin "baserom.gba", 0x00e36a67, 0x00000001
	.section .unidentified.08e38c37,"a"
	.incbin "baserom.gba", 0x00e38c37, 0x00000001
	.section .unidentified.08e3b207,"a"
	.incbin "baserom.gba", 0x00e3b207, 0x00000001
	.section .unidentified.08e3d519,"a"
	.incbin "baserom.gba", 0x00e3d519, 0x00000003
	.section .unidentified.08e3dd8d,"a"
	.incbin "baserom.gba", 0x00e3dd8d, 0x00000003
	.section .unidentified.08e3decf,"a"
	.incbin "baserom.gba", 0x00e3decf, 0x00000001
	.section .unidentified.08e40f85,"a"
	.incbin "baserom.gba", 0x00e40f85, 0x00000003
	.section .unidentified.08e4266b,"a"
	.incbin "baserom.gba", 0x00e4266b, 0x00000001
	.section .unidentified.08e44233,"a"
	.incbin "baserom.gba", 0x00e44233, 0x00000001
	.section .unidentified.08e45dae,"a"
	.incbin "baserom.gba", 0x00e45dae, 0x00000002
	.section .unidentified.08e461f9,"a"
	.incbin "baserom.gba", 0x00e461f9, 0x00000003
	.section .unidentified.08e4835a,"a"
	.incbin "baserom.gba", 0x00e4835a, 0x00000002
	.section .unidentified.08e48473,"a"
	.incbin "baserom.gba", 0x00e48473, 0x00000001
	.section .unidentified.08e4929d,"a"
	.incbin "baserom.gba", 0x00e4929d, 0x00000003
	.section .unidentified.08e493ff,"a"
	.incbin "baserom.gba", 0x00e493ff, 0x00000001
	.section .unidentified.08e4dc67,"a"
	.incbin "baserom.gba", 0x00e4dc67, 0x00000001
	.section .unidentified.08e50325,"a"
	.incbin "baserom.gba", 0x00e50325, 0x00000003
	.section .unidentified.08e50903,"a"
	.incbin "baserom.gba", 0x00e50903, 0x00000001
	.section .unidentified.08e52896,"a"
	.incbin "baserom.gba", 0x00e52896, 0x00000002
	.section .unidentified.08e54076,"a"
	.incbin "baserom.gba", 0x00e54076, 0x00000002
	.section .unidentified.08e541da,"a"
	.incbin "baserom.gba", 0x00e541da, 0x00000002
	.section .unidentified.08e569d2,"a"
	.incbin "baserom.gba", 0x00e569d2, 0x00000002
	.section .unidentified.08e56b8b,"a"
	.incbin "baserom.gba", 0x00e56b8b, 0x00000001
	.section .unidentified.08e56d13,"a"
	.incbin "baserom.gba", 0x00e56d13, 0x00000001
	.section .unidentified.08e58535,"a"
	.incbin "baserom.gba", 0x00e58535, 0x00000003
	.section .unidentified.08e5911e,"a"
	.incbin "baserom.gba", 0x00e5911e, 0x00000002
	.section .unidentified.08e5a657,"a"
	.incbin "baserom.gba", 0x00e5a657, 0x00000001
	.section .unidentified.08e5a7f2,"a"
	.incbin "baserom.gba", 0x00e5a7f2, 0x00000002
	.section .unidentified.08e5ca9e,"a"
	.incbin "baserom.gba", 0x00e5ca9e, 0x00000002
	.section .unidentified.08e5ec65,"a"
	.incbin "baserom.gba", 0x00e5ec65, 0x00000003
	.section .unidentified.08e60363,"a"
	.incbin "baserom.gba", 0x00e60363, 0x00000001
	.section .unidentified.08e60e87,"a"
	.incbin "baserom.gba", 0x00e60e87, 0x00000001
	.section .unidentified.08e6365f,"a"
	.incbin "baserom.gba", 0x00e6365f, 0x00000001
	.section .unidentified.08e6450a,"a"
	.incbin "baserom.gba", 0x00e6450a, 0x00000002
	.section .unidentified.08e65d2a,"a"
	.incbin "baserom.gba", 0x00e65d2a, 0x00000002
	.section .unidentified.08e680d1,"a"
	.incbin "baserom.gba", 0x00e680d1, 0x00000003
	.section .unidentified.08e687ba,"a"
	.incbin "baserom.gba", 0x00e687ba, 0x00000002
	.section .unidentified.08e693a6,"a"
	.incbin "baserom.gba", 0x00e693a6, 0x00000002
	.global Resource_Data5BC
Resource_Data5BC:
	.incbin "baserom.gba", 0x00e693a8, 0x00000ac4
	.section .unidentified.08e6b41b,"a"
	.incbin "baserom.gba", 0x00e6b41b, 0x00000001
	.section .unidentified.08e6b5ce,"a"
	.incbin "baserom.gba", 0x00e6b5ce, 0x00000002
	.section .unidentified.08e6d80d,"a"
	.incbin "baserom.gba", 0x00e6d80d, 0x00000003
	.section .unidentified.08e6e333,"a"
	.incbin "baserom.gba", 0x00e6e333, 0x00000001
	.section .unidentified.08e7103d,"a"
	.incbin "baserom.gba", 0x00e7103d, 0x00000003
	.section .unidentified.08e727c5,"a"
	.incbin "baserom.gba", 0x00e727c5, 0x00000003
	.section .unidentified.08e758bf,"a"
	.incbin "baserom.gba", 0x00e758bf, 0x00000001
	.section .unidentified.08e7698d,"a"
	.incbin "baserom.gba", 0x00e7698d, 0x00000003
	.section .unidentified.08e77f49,"a"
	.incbin "baserom.gba", 0x00e77f49, 0x00000003
	.section .unidentified.08e78dc4,"a"
	.global Resource_Data5C9
Resource_Data5C9:
	.incbin "baserom.gba", 0x00e78dc4, 0x000007d0
	.section .unidentified.08e7a433,"a"
	.incbin "baserom.gba", 0x00e7a433, 0x00000001
	.section .unidentified.08e7b28b,"a"
	.incbin "baserom.gba", 0x00e7b28b, 0x00000001
	.section .unidentified.08e7c25d,"a"
	.incbin "baserom.gba", 0x00e7c25d, 0x00000003
	.section .unidentified.08e7c3d7,"a"
	.incbin "baserom.gba", 0x00e7c3d7, 0x00000001
	.section .unidentified.08e7c54e,"a"
	.incbin "baserom.gba", 0x00e7c54e, 0x00000002
	.section .unidentified.08e7c6e6,"a"
	.incbin "baserom.gba", 0x00e7c6e6, 0x00000002
	.section .unidentified.08e7c877,"a"
	.incbin "baserom.gba", 0x00e7c877, 0x00000001
	.section .unidentified.08e7eb23,"a"
	.incbin "baserom.gba", 0x00e7eb23, 0x00000001
	.section .unidentified.08e7ec3b,"a"
	.incbin "baserom.gba", 0x00e7ec3b, 0x00000001
	.section .unidentified.08e80a3d,"a"
	.incbin "baserom.gba", 0x00e80a3d, 0x00000003
	.section .unidentified.08e82502,"a"
	.incbin "baserom.gba", 0x00e82502, 0x00000002
	.section .unidentified.08e82c2a,"a"
	.incbin "baserom.gba", 0x00e82c2a, 0x00000002
	.section .unidentified.08e84026,"a"
	.incbin "baserom.gba", 0x00e84026, 0x00000002
	.section .unidentified.08e84efa,"a"
	.incbin "baserom.gba", 0x00e84efa, 0x00000002
	.section .unidentified.08e8503e,"a"
	.incbin "baserom.gba", 0x00e8503e, 0x00000002
	.section .unidentified.08e873ef,"a"
	.incbin "baserom.gba", 0x00e873ef, 0x00000001
	.section .unidentified.08e89c1d,"a"
	.incbin "baserom.gba", 0x00e89c1d, 0x00000003
	.section .unidentified.08e8a01a,"a"
	.incbin "baserom.gba", 0x00e8a01a, 0x00000002
	.section .unidentified.08e8b4ba,"a"
	.incbin "baserom.gba", 0x00e8b4ba, 0x00000002
	.section .unidentified.08e8b60e,"a"
	.incbin "baserom.gba", 0x00e8b60e, 0x00000002
	.section .unidentified.08e8ef5b,"a"
	.incbin "baserom.gba", 0x00e8ef5b, 0x00000001
	.section .unidentified.08e903c6,"a"
	.incbin "baserom.gba", 0x00e903c6, 0x00000002
	.section .unidentified.08e9189f,"a"
	.incbin "baserom.gba", 0x00e9189f, 0x00000001
	.section .unidentified.08e919f2,"a"
	.incbin "baserom.gba", 0x00e919f2, 0x00000002
	.section .unidentified.08e953ff,"a"
	.incbin "baserom.gba", 0x00e953ff, 0x00000001
	.section .unidentified.08e965d7,"a"
	.incbin "baserom.gba", 0x00e965d7, 0x00000001
	.section .unidentified.08e97de2,"a"
	.incbin "baserom.gba", 0x00e97de2, 0x00000002
	.section .unidentified.08e97f32,"a"
	.incbin "baserom.gba", 0x00e97f32, 0x00000002
	.section .unidentified.08e991b5,"a"
	.incbin "baserom.gba", 0x00e991b5, 0x00000003
	.section .unidentified.08e9a63f,"a"
	.incbin "baserom.gba", 0x00e9a63f, 0x00000001
	.section .unidentified.08e9a7a6,"a"
	.incbin "baserom.gba", 0x00e9a7a6, 0x00000002
	.section .unidentified.08e9d767,"a"
	.incbin "baserom.gba", 0x00e9d767, 0x00000001
	.section .unidentified.08e9e9d5,"a"
	.incbin "baserom.gba", 0x00e9e9d5, 0x00000003
	.section .unidentified.08ea1562,"a"
	.incbin "baserom.gba", 0x00ea1562, 0x00000002
	.section .unidentified.08ea1f49,"a"
	.incbin "baserom.gba", 0x00ea1f49, 0x00000003
	.section .unidentified.08ea28ea,"a"
	.incbin "baserom.gba", 0x00ea28ea, 0x00000002
	.section .unidentified.08ea2a5e,"a"
	.incbin "baserom.gba", 0x00ea2a5e, 0x00000002
	.section .unidentified.08ea456f,"a"
	.incbin "baserom.gba", 0x00ea456f, 0x00000001
	.section .unidentified.08ea4691,"a"
	.incbin "baserom.gba", 0x00ea4691, 0x00000003
	.section .unidentified.08ea718f,"a"
	.incbin "baserom.gba", 0x00ea718f, 0x00000001
	.section .unidentified.08ea7f86,"a"
	.incbin "baserom.gba", 0x00ea7f86, 0x00000002
	.section .unidentified.08ea9f25,"a"
	.incbin "baserom.gba", 0x00ea9f25, 0x00000003
	.section .unidentified.08eab255,"a"
	.incbin "baserom.gba", 0x00eab255, 0x00000003
	.section .unidentified.08eab3a9,"a"
	.incbin "baserom.gba", 0x00eab3a9, 0x00000003
	.section .unidentified.08eabf5b,"a"
	.incbin "baserom.gba", 0x00eabf5b, 0x00000001
	.section .unidentified.08ead735,"a"
	.incbin "baserom.gba", 0x00ead735, 0x00000003
	.section .unidentified.08eae895,"a"
	.incbin "baserom.gba", 0x00eae895, 0x00000003
	.section .unidentified.08eaf9b3,"a"
	.incbin "baserom.gba", 0x00eaf9b3, 0x00000001
	.section .unidentified.08eafbba,"a"
	.incbin "baserom.gba", 0x00eafbba, 0x00000002
	.section .unidentified.08eb08af,"a"
	.incbin "baserom.gba", 0x00eb08af, 0x00000001
	.section .unidentified.08eb3051,"a"
	.incbin "baserom.gba", 0x00eb3051, 0x00000003
	.section .unidentified.08eb47c6,"a"
	.incbin "baserom.gba", 0x00eb47c6, 0x00000002
	.section .unidentified.08eb5a83,"a"
	.incbin "baserom.gba", 0x00eb5a83, 0x00000001
	.section .unidentified.08eb62c6,"a"
	.incbin "baserom.gba", 0x00eb62c6, 0x00000002
	.global Resource_Data60D
Resource_Data60D:
	.incbin "baserom.gba", 0x00eb62c8, 0x0000060c
	.section .unidentified.08eb6e4b,"a"
	.incbin "baserom.gba", 0x00eb6e4b, 0x00000001
	.section .unidentified.08eb6f8b,"a"
	.incbin "baserom.gba", 0x00eb6f8b, 0x00000001
	.section .unidentified.08eb70cb,"a"
	.incbin "baserom.gba", 0x00eb70cb, 0x00000001
	.section .unidentified.08eb7c2d,"a"
	.incbin "baserom.gba", 0x00eb7c2d, 0x00000003
	.section .unidentified.08eba3d1,"a"
	.incbin "baserom.gba", 0x00eba3d1, 0x00000003
	.section .unidentified.08ebbb46,"a"
	.incbin "baserom.gba", 0x00ebbb46, 0x00000002
	.section .unidentified.08ebce03,"a"
	.incbin "baserom.gba", 0x00ebce03, 0x00000001
	.section .unidentified.08ebd646,"a"
	.incbin "baserom.gba", 0x00ebd646, 0x00000002
	.section .unidentified.08ebdbe5,"a"
	.incbin "baserom.gba", 0x00ebdbe5, 0x00000003
	.global Resource_Data61A
Resource_Data61A:
	.incbin "baserom.gba", 0x00ebdbe8, 0x0000000c
	.global Resource_Data61B
Resource_Data61B:
	.incbin "baserom.gba", 0x00ebdbf4, 0x00000150
	.global Resource_Data61C
Resource_Data61C:
	.incbin "baserom.gba", 0x00ebdd44, 0x00000140
	.global Resource_Data61D
Resource_Data61D:
	.incbin "baserom.gba", 0x00ebde84, 0x00000140
	.global Resource_Data61E
Resource_Data61E:
	.incbin "baserom.gba", 0x00ebdfc4, 0x00000140
	.section .unidentified.08ebe819,"a"
	.incbin "baserom.gba", 0x00ebe819, 0x00000003
	.global Resource_Data620
Resource_Data620:
	.incbin "baserom.gba", 0x00ebe81c, 0x0000000c
	.global Resource_Data621
Resource_Data621:
	.incbin "baserom.gba", 0x00ebe828, 0x00000150
	.global Resource_Data622
Resource_Data622:
	.incbin "baserom.gba", 0x00ebe978, 0x00000140
	.global Resource_Data623
Resource_Data623:
	.incbin "baserom.gba", 0x00ebeab8, 0x00000140
	.global Resource_Data624
Resource_Data624:
	.incbin "baserom.gba", 0x00ebebf8, 0x00000140
	.section .unidentified.08ebf8f9,"a"
	.incbin "baserom.gba", 0x00ebf8f9, 0x00000003
	.global Resource_Data626
Resource_Data626:
	.incbin "baserom.gba", 0x00ebf8fc, 0x0000000c
	.global Resource_Data627
Resource_Data627:
	.incbin "baserom.gba", 0x00ebf908, 0x00000150
	.global Resource_Data628
Resource_Data628:
	.incbin "baserom.gba", 0x00ebfa58, 0x00000140
	.global Resource_Data629
Resource_Data629:
	.incbin "baserom.gba", 0x00ebfb98, 0x00000140
	.global Resource_Data62A
Resource_Data62A:
	.incbin "baserom.gba", 0x00ebfcd8, 0x00000140
	.section .unidentified.08ec03e5,"a"
	.incbin "baserom.gba", 0x00ec03e5, 0x00000003
	.global Resource_Data62C
Resource_Data62C:
	.incbin "baserom.gba", 0x00ec03e8, 0x0000000c
	.global Resource_Data62D
Resource_Data62D:
	.incbin "baserom.gba", 0x00ec03f4, 0x00000150
	.global Resource_Data62E
Resource_Data62E:
	.incbin "baserom.gba", 0x00ec0544, 0x00000140
	.global Resource_Data62F
Resource_Data62F:
	.incbin "baserom.gba", 0x00ec0684, 0x00000140
	.global Resource_Data630
Resource_Data630:
	.incbin "baserom.gba", 0x00ec07c4, 0x00000140
	.section .unidentified.08ec1281,"a"
	.incbin "baserom.gba", 0x00ec1281, 0x00000003
	.global Resource_Data632
Resource_Data632:
	.incbin "baserom.gba", 0x00ec1284, 0x0000000c
	.global Resource_Data633
Resource_Data633:
	.incbin "baserom.gba", 0x00ec1290, 0x00000150
	.global Resource_Data634
Resource_Data634:
	.incbin "baserom.gba", 0x00ec13e0, 0x00000140
	.global Resource_Data635
Resource_Data635:
	.incbin "baserom.gba", 0x00ec1520, 0x00000140
	.global Resource_Data636
Resource_Data636:
	.incbin "baserom.gba", 0x00ec1660, 0x00000140
	.section .unidentified.08ec1bc9,"a"
	.incbin "baserom.gba", 0x00ec1bc9, 0x00000003
	.global Resource_Data638
Resource_Data638:
	.incbin "baserom.gba", 0x00ec1bcc, 0x0000000c
	.global Resource_Data639
Resource_Data639:
	.incbin "baserom.gba", 0x00ec1bd8, 0x00000150
	.global Resource_Data63A
Resource_Data63A:
	.incbin "baserom.gba", 0x00ec1d28, 0x00000140
	.global Resource_Data63B
Resource_Data63B:
	.incbin "baserom.gba", 0x00ec1e68, 0x00000140
	.global Resource_Data63C
Resource_Data63C:
	.incbin "baserom.gba", 0x00ec1fa8, 0x00000140
	.section .unidentified.08ec2711,"a"
	.incbin "baserom.gba", 0x00ec2711, 0x00000003
	.global Resource_Data63E
Resource_Data63E:
	.incbin "baserom.gba", 0x00ec2714, 0x0000000c
	.global Resource_Data63F
Resource_Data63F:
	.incbin "baserom.gba", 0x00ec2720, 0x00000150
	.global Resource_Data640
Resource_Data640:
	.incbin "baserom.gba", 0x00ec2870, 0x00000140
	.global Resource_Data641
Resource_Data641:
	.incbin "baserom.gba", 0x00ec29b0, 0x00000140
	.global Resource_Data642
Resource_Data642:
	.incbin "baserom.gba", 0x00ec2af0, 0x00000140
	.section .unidentified.08ec3359,"a"
	.incbin "baserom.gba", 0x00ec3359, 0x00000003
	.global Resource_Data644
Resource_Data644:
	.incbin "baserom.gba", 0x00ec335c, 0x0000000c
	.global Resource_Data645
Resource_Data645:
	.incbin "baserom.gba", 0x00ec3368, 0x00000150
	.global Resource_Data646
Resource_Data646:
	.incbin "baserom.gba", 0x00ec34b8, 0x00000140
	.global Resource_Data647
Resource_Data647:
	.incbin "baserom.gba", 0x00ec35f8, 0x00000140
	.global Resource_Data648
Resource_Data648:
	.incbin "baserom.gba", 0x00ec3738, 0x00000140
	.section .unidentified.08f79646,"a"
	.incbin "baserom.gba", 0x00f79646, 0x00000002
