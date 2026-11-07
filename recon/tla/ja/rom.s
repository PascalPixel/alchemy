	.section .rom.000133c4, "ax"
	.balign 4
	.global System_InstallIwramAndIrqs
	.type System_InstallIwramAndIrqs, %function
	.thumb_func
System_InstallIwramAndIrqs:
	.incbin "baserom.gba", 0x000133c4, 0x00000074
	.section .rom.00015778, "ax"
	.global Render_ProjectPoint
	.type Render_ProjectPoint, %function
	.thumb_func
Render_ProjectPoint:
	.incbin "baserom.gba", 0x00015778, 0x000000b8
	.section .rom.000162da, "ax"
	.incbin "baserom.gba", 0x000162da, 0x00000002
	.section .rom.00016e02, "ax"
	.incbin "baserom.gba", 0x00016e02, 0x00000002
	.section .rom.000212d8, "ax"
	.incbin "baserom.gba", 0x000212d8, 0x00000194
	.section .rom.0002279e, "ax"
	.incbin "baserom.gba", 0x0002279e, 0x00000002
	.section .rom.000233a6, "ax"
	.incbin "baserom.gba", 0x000233a6, 0x00000002
	.section .rom.00023f9c, "ax"
	.global Func_08023f9c
	.type Func_08023f9c, %function
	.thumb_func
Func_08023f9c:
	.incbin "baserom.gba", 0x00023f9c, 0x0000071c
	.section .rom.00025830, "ax"
	.global Func_08025830
	.type Func_08025830, %function
	.thumb_func
Func_08025830:
	.incbin "baserom.gba", 0x00025830, 0x00000328
	.section .rom.00025c8a, "ax"
	.incbin "baserom.gba", 0x00025c8a, 0x00000002
	.section .rom.000267bc, "ax"
	.global Func_080267bc
	.type Func_080267bc, %function
	.thumb_func
Func_080267bc:
	.incbin "baserom.gba", 0x000267bc, 0x00000614
	.section .rom.00029278, "ax"
	.global Func_080292fc
	.type Func_080292fc, %function
	.thumb_func
Func_080292fc:
	.incbin "baserom.gba", 0x00029278, 0x00000274
	.global Func_08029570
	.type Func_08029570, %function
	.thumb_func
Func_08029570:
	.incbin "baserom.gba", 0x000294ec, 0x000002a0
	.section .rom.0002a634, "ax"
	.global Func_0802a6b8
	.type Func_0802a6b8, %function
	.thumb_func
Func_0802a6b8:
	.incbin "baserom.gba", 0x0002a634, 0x000003bc
	.section .rom.0002b3cc, "ax"
	.global Func_0802b450
	.type Func_0802b450, %function
	.thumb_func
Func_0802b450:
	.incbin "baserom.gba", 0x0002b3cc, 0x00000140
	.section .rom.0002b914, "ax"
	.global Func_0802b998
	.type Func_0802b998, %function
	.thumb_func
Func_0802b998:
	.incbin "baserom.gba", 0x0002b914, 0x00000370
	.section .rom.0002bd4c, "ax"
	.balign 4
	.global Func_0802bdd0
	.type Func_0802bdd0, %function
	.thumb_func
Func_0802bdd0:
	.incbin "baserom.gba", 0x0002bd4c, 0x0000007c
	.section .rom.0002bdc8, "ax"
	.global Func_0802be4c
	.type Func_0802be4c, %function
	.thumb_func
Func_0802be4c:
	.incbin "baserom.gba", 0x0002bdc8, 0x0000023c
	.section .rom.0002c508, "ax"
	.global Func_0802c58c
	.type Func_0802c58c, %function
	.thumb_func
Func_0802c58c:
	.incbin "baserom.gba", 0x0002c508, 0x000002d8
	.section .rom.0002dcec, "ax"
	.global Func_0802dd70
	.type Func_0802dd70, %function
	.thumb_func
Func_0802dd70:
	.incbin "baserom.gba", 0x0002dcec, 0x0000011c
	.section .rom.0003938e, "ax"
	.incbin "baserom.gba", 0x0003938e, 0x00000002
	.section .rom.00039446, "ax"
	.incbin "baserom.gba", 0x00039446, 0x00000002
	.section .rom.000394da, "ax"
	.incbin "baserom.gba", 0x000394da, 0x00000002
	.section .rom.0003971e, "ax"
	.incbin "baserom.gba", 0x0003971e, 0x00000002
	.section .rom.00039858, "ax"
	.global Message_StepScript
	.type Message_StepScript, %function
	.thumb_func
Message_StepScript:
	.incbin "baserom.gba", 0x00039858, 0x0000060c
	.section .rom.0003a3da, "ax"
	.incbin "baserom.gba", 0x0003a3da, 0x00000002
	.section .rom.0003a890, "ax"
	.balign 4
	.global Func_0803a8fc
	.type Func_0803a8fc, %function
	.thumb_func
Func_0803a8fc:
	.incbin "baserom.gba", 0x0003a890, 0x000001b4
	.section .rom.0003aa84, "ax"
	.global Text_DrawWideStringAtOffset
	.type Text_DrawWideStringAtOffset, %function
	.thumb_func
Text_DrawWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aa84, 0x00000144
	.section .rom.0003ae38, "ax"
	.global Message_Decode
	.type Message_Decode, %function
	.thumb_func
Message_Decode:
	.incbin "baserom.gba", 0x0003ae38, 0x00000944
	.section .rom.0003b77c, "ax"
	.global Window_FitOnScreen
	.type Window_FitOnScreen, %function
	.thumb_func
Window_FitOnScreen:
	.incbin "baserom.gba", 0x0003b77c, 0x000000f8
	.section .rom.0003b924, "ax"
	.global Text_MeasureEntry
	.type Text_MeasureEntry, %function
	.thumb_func
Text_MeasureEntry:
	.incbin "baserom.gba", 0x0003b924, 0x000007b8
	.section .rom.0003c0dc, "ax"
	.global Func_0803c068
	.type Func_0803c068, %function
	.thumb_func
Func_0803c068:
	.incbin "baserom.gba", 0x0003c0dc, 0x00000100
	.section .rom.0003c204, "ax"
	.balign 4
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c204, 0x000000d4
	.global Func_0803c274
	.type Func_0803c274, %function
	.thumb_func
Func_0803c274:
	.incbin "baserom.gba", 0x0003c2d8, 0x00000158
	.section .rom.0003f07a, "ax"
	.incbin "baserom.gba", 0x0003f07a, 0x00000002
	.section .rom.0003f306, "ax"
	.incbin "baserom.gba", 0x0003f306, 0x00000002
	.section .rom.0003fe5c, "ax"
	.global Menu_OpenWorkspaceOptions
	.type Menu_OpenWorkspaceOptions, %function
	.thumb_func
Menu_OpenWorkspaceOptions:
	.incbin "baserom.gba", 0x0003fe5c, 0x000003b0
	.section .rom.000408bc, "ax"
	.global Func_08040798
	.type Func_08040798, %function
	.thumb_func
Func_08040798:
	.incbin "baserom.gba", 0x000408bc, 0x000001f4
	.section .rom.00040b6a, "ax"
	.incbin "baserom.gba", 0x00040b6a, 0x00000002
	.section .rom.000417ee, "ax"
	.incbin "baserom.gba", 0x000417ee, 0x00000002
	.section .rom.00042152, "ax"
	.incbin "baserom.gba", 0x00042152, 0x00000002
	.section .rom.000422ca, "ax"
	.incbin "baserom.gba", 0x000422ca, 0x00000002
	.section .rom.000423b8, "ax"
	.balign 4
	.global Text_DrawPrefixedNumberAtOffset
	.type Text_DrawPrefixedNumberAtOffset, %function
	.thumb_func
Text_DrawPrefixedNumberAtOffset:
	.incbin "baserom.gba", 0x000423b8, 0x000000a0
	.section .rom.000426be, "ax"
	.incbin "baserom.gba", 0x000426be, 0x00000002
	.section .rom.00042824, "ax"
	.global Func_080426e0
	.type Func_080426e0, %function
	.thumb_func
Func_080426e0:
	.incbin "baserom.gba", 0x00042824, 0x00000128
	.section .rom.00043e1c, "ax"
	.global Func_08043cd8
	.type Func_08043cd8, %function
	.thumb_func
Func_08043cd8:
	.incbin "baserom.gba", 0x00043e1c, 0x000006d4
	.section .rom.00044640, "ax"
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x00044640, 0x00000610
	.section .rom.000454b6, "ax"
	.incbin "baserom.gba", 0x000454b6, 0x00000002
	.section .rom.00045722, "ax"
	.incbin "baserom.gba", 0x00045722, 0x00000002
	.section .rom.0004597a, "ax"
	.incbin "baserom.gba", 0x0004597a, 0x00000002
	.section .rom.000459ca, "ax"
	.incbin "baserom.gba", 0x000459ca, 0x00000002
	.global Func_080457d0
	.type Func_080457d0, %function
	.thumb_func
Func_080457d0:
	.incbin "baserom.gba", 0x000459cc, 0x00000968
	.section .rom.00046612, "ax"
	.incbin "baserom.gba", 0x00046612, 0x000006f2
	.global Func_08046b08
	.type Func_08046b08, %function
	.thumb_func
Func_08046b08:
	.incbin "baserom.gba", 0x00046d04, 0x00001b88
	.global Func_0804868c
	.type Func_0804868c, %function
	.thumb_func
Func_0804868c:
	.incbin "baserom.gba", 0x0004888c, 0x00000ab8
	.global Func_08049154
	.type Func_08049154, %function
	.thumb_func
Func_08049154:
	.incbin "baserom.gba", 0x00049344, 0x00000860
	.section .rom.00049bf2, "ax"
	.incbin "baserom.gba", 0x00049bf2, 0x00000002
	.section .rom.0004aa4c, "ax"
	.global Func_0804a85c
	.type Func_0804a85c, %function
	.thumb_func
Func_0804a85c:
	.incbin "baserom.gba", 0x0004aa4c, 0x00000e44
	.section .rom.0004b9b0, "ax"
	.balign 4
	.global Message_ShowLocalizedAndWait
	.type Message_ShowLocalizedAndWait, %function
	.thumb_func
Message_ShowLocalizedAndWait:
	.incbin "baserom.gba", 0x0004b9b0, 0x000000bc
	.section .rom.0004baa6, "ax"
	.incbin "baserom.gba", 0x0004baa6, 0x00000002
	.global Func_0804b8b8
	.type Func_0804b8b8, %function
	.thumb_func
Func_0804b8b8:
	.incbin "baserom.gba", 0x0004baa8, 0x00000200
	.section .rom.0004bd92, "ax"
	.incbin "baserom.gba", 0x0004bd92, 0x00000002
	.section .rom.0004d676, "ax"
	.incbin "baserom.gba", 0x0004d676, 0x00000002
	.section .rom.0004d9fc, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004d9fc, 0x0000016c
	.section .rom.0004dcba, "ax"
	.incbin "baserom.gba", 0x0004dcba, 0x00000002
	.section .rom.000a83f6, "ax"
	.incbin "baserom.gba", 0x000a83f6, 0x00000002
	.section .rom.000a8df0, "ax"
	.global Func_080addf0
	.type Func_080addf0, %function
	.thumb_func
Func_080addf0:
	.incbin "baserom.gba", 0x000a8df0, 0x000002e8
	.section .rom.000a9c62, "ax"
	.incbin "baserom.gba", 0x000a9c62, 0x00000002
	.section .rom.000aa0de, "ax"
	.incbin "baserom.gba", 0x000aa0de, 0x00000002
	.section .rom.000ac068, "ax"
	.global Func_080b1088
	.type Func_080b1088, %function
	.thumb_func
Func_080b1088:
	.incbin "baserom.gba", 0x000ac068, 0x00000160
	.section .rom.000cb6c6, "ax"
	.incbin "baserom.gba", 0x000cb6c6, 0x00000002
	.section .rom.000cb91c, "ax"
	.global FieldSequence_Run
	.type FieldSequence_Run, %function
	.thumb_func
FieldSequence_Run:
	.incbin "baserom.gba", 0x000cb91c, 0x00000c28
	.section .rom.000ccd6e, "ax"
	.incbin "baserom.gba", 0x000ccd6e, 0x00000002
	.section .rom.000cd800, "ax"
	.balign 4
	.global Func_080cd808
	.type Func_080cd808, %function
	.thumb_func
Func_080cd808:
	.incbin "baserom.gba", 0x000cd800, 0x00000114
	.section .rom.000cdc6a, "ax"
	.incbin "baserom.gba", 0x000cdc6a, 0x00000002
	.section .rom.000ce2fc, "ax"
	.global Func_080ce31c
	.type Func_080ce31c, %function
	.thumb_func
Func_080ce31c:
	.incbin "baserom.gba", 0x000ce2fc, 0x0000013c
	.section .rom.000d181e, "ax"
	.incbin "baserom.gba", 0x000d181e, 0x00000002
	.section .rom.000d19f6, "ax"
	.incbin "baserom.gba", 0x000d19f6, 0x00000002
	.section .rom.000d3356, "ax"
	.incbin "baserom.gba", 0x000d3356, 0x00000002
	.section .rom.000d343e, "ax"
	.incbin "baserom.gba", 0x000d343e, 0x00000002
	.section .rom.000d41c6, "ax"
	.incbin "baserom.gba", 0x000d41c6, 0x00000002
	.section .rom.000d5396, "ax"
	.incbin "baserom.gba", 0x000d5396, 0x00000002
	.section .rom.000d564e, "ax"
	.incbin "baserom.gba", 0x000d564e, 0x00000002
	.section .rom.000d57c2, "ax"
	.incbin "baserom.gba", 0x000d57c2, 0x00000002
	.section .rom.000d5a7e, "ax"
	.incbin "baserom.gba", 0x000d5a7e, 0x00000002
	.section .rom.000d7504, "ax"
	.global Func_080d7524
	.type Func_080d7524, %function
	.thumb_func
Func_080d7524:
	.incbin "baserom.gba", 0x000d7504, 0x0000024c
	.section .rom.000d7808, "ax"
	.incbin "baserom.gba", 0x000d7808, 0x000000f0
	.section .rom.000d9ed6, "ax"
	.incbin "baserom.gba", 0x000d9ed6, 0x00000002
	.section .rom.000da1e6, "ax"
	.incbin "baserom.gba", 0x000da1e6, 0x00000002
	.section .rom.000db962, "ax"
	.incbin "baserom.gba", 0x000db962, 0x00000002
	.section .rom.000dc7a4, "ax"
	.balign 4
	.global BattleEffect_CleanupSceneObjects
	.type BattleEffect_CleanupSceneObjects, %function
	.thumb_func
BattleEffect_CleanupSceneObjects:
	.incbin "baserom.gba", 0x000dc7a4, 0x0000016c
	.section .rom.000dd010, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
	.incbin "baserom.gba", 0x000dd010, 0x000003e8
	.section .rom.000dd4e2, "ax"
	.incbin "baserom.gba", 0x000dd4e2, 0x00000002
	.section .rom.000dd9ea, "ax"
	.incbin "baserom.gba", 0x000dd9ea, 0x00000002
	.section .rom.000df89a, "ax"
	.incbin "baserom.gba", 0x000df89a, 0x00000002
	.section .rom.000e037e, "ax"
	.incbin "baserom.gba", 0x000e037e, 0x00000002
	.section .rom.000e2220, "ax"
	.global Func_080e2264
	.type Func_080e2264, %function
	.thumb_func
Func_080e2264:
	.incbin "baserom.gba", 0x000e2220, 0x00000238
	.section .rom.000e2730, "ax"
	.global Func_080e2774
	.type Func_080e2774, %function
	.thumb_func
Func_080e2774:
	.incbin "baserom.gba", 0x000e2730, 0x00000160
	.section .rom.000e2bf4, "ax"
	.global Func_080e2c38
	.type Func_080e2c38, %function
	.thumb_func
Func_080e2c38:
	.incbin "baserom.gba", 0x000e2bf4, 0x00000108
	.section .rom.000e46ec, "ax"
	.balign 4
	.global Func_080e4730
	.type Func_080e4730, %function
	.thumb_func
Func_080e4730:
	.incbin "baserom.gba", 0x000e46ec, 0x000002fc
	.section .rom.000e5a20, "ax"
	.incbin "baserom.gba", 0x000e5a20, 0x0000028c
	.section .rom.000e5d18, "ax"
	.balign 4
	.global Func_080e5d5c
	.type Func_080e5d5c, %function
	.thumb_func
Func_080e5d5c:
	.incbin "baserom.gba", 0x000e5d18, 0x00000598
	.section .rom.000e63bc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e63bc, 0x000004c8
	.section .rom.000e688c, "ax"
	.balign 4
	.global Func_080e68d0
	.type Func_080e68d0, %function
	.thumb_func
Func_080e68d0:
	.incbin "baserom.gba", 0x000e688c, 0x00000828
	.section .rom.000e7800, "ax"
	.balign 4
	.global Func_080e7844
	.type Func_080e7844, %function
	.thumb_func
Func_080e7844:
	.incbin "baserom.gba", 0x000e7800, 0x0000074c
	.section .rom.000e877c, "ax"
	.global Func_080e87c4
	.type Func_080e87c4, %function
	.thumb_func
Func_080e87c4:
	.incbin "baserom.gba", 0x000e877c, 0x00000220
	.global Func_080e89e4
	.type Func_080e89e4, %function
	.thumb_func
Func_080e89e4:
	.incbin "baserom.gba", 0x000e899c, 0x00000148
	.global Func_080e8b44
	.type Func_080e8b44, %function
	.thumb_func
Func_080e8b44:
	.incbin "baserom.gba", 0x000e8ae4, 0x00000108
	.global Func_080e8c9c
	.type Func_080e8c9c, %function
	.thumb_func
Func_080e8c9c:
	.incbin "baserom.gba", 0x000e8bec, 0x00000054
	.section .rom.000ea704, "ax"
	.global Func_080ea7c0
	.type Func_080ea7c0, %function
	.thumb_func
Func_080ea7c0:
	.incbin "baserom.gba", 0x000ea704, 0x000000e8
	.section .rom.000ea818, "ax"
	.balign 4
	.global Func_080ea8d4
	.type Func_080ea8d4, %function
	.thumb_func
Func_080ea8d4:
	.incbin "baserom.gba", 0x000ea818, 0x00000140
	.section .rom.000eab14, "ax"
	.balign 4
	.global Func_080eabd0
	.type Func_080eabd0, %function
	.thumb_func
Func_080eabd0:
	.incbin "baserom.gba", 0x000eab14, 0x00000118
	.section .rom.000ead40, "ax"
	.global Func_080eadfc
	.type Func_080eadfc, %function
	.thumb_func
Func_080eadfc:
	.incbin "baserom.gba", 0x000ead40, 0x000000b8
	.section .rom.000eb1da, "ax"
	.incbin "baserom.gba", 0x000eb1da, 0x00000002
	.section .rom.000eb768, "ax"
	.balign 4
	.global Func_080eb824
	.type Func_080eb824, %function
	.thumb_func
Func_080eb824:
	.incbin "baserom.gba", 0x000eb768, 0x0000010c
	.section .rom.000ec114, "ax"
	.global Func_080ec1d0
	.thumb_func
Func_080ec1d0:
	.incbin "baserom.gba", 0x000ec114, 0x000000cc
	.section .rom.000ec200, "ax"
	.incbin "baserom.gba", 0x000ec200, 0x00000b38
	.section .rom.000ed1b0, "ax"
	.incbin "baserom.gba", 0x000ed1b0, 0x000004e4
	.section .rom.000ef728, "ax"
	.incbin "baserom.gba", 0x000ef728, 0x00000008
	.section .rom.000f3890, "ax"
	.global Data_080f3984
Data_080f3984:
	.incbin "baserom.gba", 0x000f3890, 0x00000040
	.section .rom.000f8170, "ax"
	.global Func_080f8170
	.type Func_080f8170, %function
	.thumb_func
Func_080f8170:
	.incbin "baserom.gba", 0x000f8170, 0x00000498
	.section .rom.000f933e, "ax"
	.incbin "baserom.gba", 0x000f933e, 0x00000002
	.section .rom.000f942a, "ax"
	.incbin "baserom.gba", 0x000f942a, 0x00000002
	.section .rom.000f942c, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x000f942c, 0x00000db0
	.section .rom.000fac0e, "ax"
	.incbin "baserom.gba", 0x000fac0e, 0x00000002
	.section .rom.000fb48a, "ax"
	.incbin "baserom.gba", 0x000fb48a, 0x00000002
	.section .rom.000fbe10, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fbe10, 0x000002e0
	.section .rom.000fc2a4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fc2a4, 0x000001a0
	.section .rom.000fc680, "ax"
	.incbin "baserom.gba", 0x000fc680, 0x000003fc
	.section .rom.000fcfc4, "ax"
	.global Func_080fd000
	.type Func_080fd000, %function
	.thumb_func
Func_080fd000:
	.incbin "baserom.gba", 0x000fcfc4, 0x00000290
	.section .rom.000fd2a8, "ax"
	.global Func_080fd2e8
	.type Func_080fd2e8, %function
	.thumb_func
Func_080fd2e8:
	.incbin "baserom.gba", 0x000fd2a8, 0x00000244
	.section .rom.000fd51c, "ax"
	.incbin "baserom.gba", 0x000fd51c, 0x000001c8
	.section .rom.000fe8c6, "ax"
	.incbin "baserom.gba", 0x000fe8c6, 0x00000002
	.section .rom.000feef4, "ax"
	.balign 4
	.global Func_080feec0
	.type Func_080feec0, %function
	.thumb_func
Func_080feec0:
	.incbin "baserom.gba", 0x000feef4, 0x00000334
	.section .rom.000ff3a4, "ax"
	.global Func_080ff370
	.type Func_080ff370, %function
	.thumb_func
Func_080ff370:
	.incbin "baserom.gba", 0x000ff3a4, 0x0000024c
	.section .rom.000ff882, "ax"
	.incbin "baserom.gba", 0x000ff882, 0x00000002
	.section .rom.000ff980, "ax"
	.incbin "baserom.gba", 0x000ff980, 0x00000208
	.section .rom.000ffd00, "ax"
	.global Func_080ffcd4
	.type Func_080ffcd4, %function
	.thumb_func
Func_080ffcd4:
	.incbin "baserom.gba", 0x000ffd00, 0x00000210
	.section .rom.00100248, "ax"
	.global Func_0810021c
	.type Func_0810021c, %function
	.thumb_func
Func_0810021c:
	.incbin "baserom.gba", 0x00100248, 0x00000278
	.section .rom.00100740, "ax"
	.balign 4
	.global Func_08100738
	.type Func_08100738, %function
	.thumb_func
Func_08100738:
	.incbin "baserom.gba", 0x00100740, 0x000000c0
	.section .rom.00100d46, "ax"
	.incbin "baserom.gba", 0x00100d46, 0x00000002
	.section .rom.00101640, "ax"
	.global Func_08101638
Func_08101638:
	.incbin "baserom.gba", 0x00101640, 0x00000228
	.section .rom.00101ad0, "ax"
	.global DjinnMenu_DrawPage
DjinnMenu_DrawPage:
	.incbin "baserom.gba", 0x00101ad0, 0x00000144
	.section .rom.00102010, "ax"
	.global Func_08102008
Func_08102008:
	.incbin "baserom.gba", 0x00102010, 0x0000102c
	.section .rom.001031ee, "ax"
	.incbin "baserom.gba", 0x001031ee, 0x00000002
	.global Func_08103218
	.type Func_08103218, %function
	.thumb_func
Func_08103218:
	.incbin "baserom.gba", 0x001031f0, 0x000007e4
	.section .rom.0010499e, "ax"
	.incbin "baserom.gba", 0x0010499e, 0x00000002
	.section .rom.00104ece, "ax"
	.incbin "baserom.gba", 0x00104ece, 0x00000002
	.section .rom.00104fb6, "ax"
	.incbin "baserom.gba", 0x00104fb6, 0x00000002
	.section .rom.0010517e, "ax"
	.incbin "baserom.gba", 0x0010517e, 0x00000002
	.section .rom.00108b70, "ax"
	.global Func_08108b70
	.type Func_08108b70, %function
	.thumb_func
Func_08108b70:
	.incbin "baserom.gba", 0x00108b70, 0x000004e0
	.section .rom.0010916e, "ax"
	.incbin "baserom.gba", 0x0010916e, 0x00000002
	.section .rom.0010928a, "ax"
	.incbin "baserom.gba", 0x0010928a, 0x00000002
	.section .rom.001093a4, "ax"
	.incbin "baserom.gba", 0x001093a4, 0x0000020c
	.section .rom.00109ae8, "ax"
	.global Func_08109ad8
	.type Func_08109ad8, %function
	.thumb_func
Func_08109ad8:
	.incbin "baserom.gba", 0x00109ae8, 0x000001d4
	.section .rom.0010a498, "ax"
	.incbin "baserom.gba", 0x0010a498, 0x000001dc
	.section .rom.0010ac00, "ax"
	.global Func_0810abf0
	.type Func_0810abf0, %function
	.thumb_func
Func_0810abf0:
	.incbin "baserom.gba", 0x0010ac00, 0x000002b4
	.section .rom.0010b05a, "ax"
	.incbin "baserom.gba", 0x0010b05a, 0x00000002
	.section .rom.0011843c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011843c, 0x00000188
	.section .rom.00119052, "ax"
	.incbin "baserom.gba", 0x00119052, 0x00000002
	.section .rom.001197f0, "ax"
	.global Func_081197f0
	.type Func_081197f0, %function
	.thumb_func
Func_081197f0:
	.incbin "baserom.gba", 0x001197f0, 0x00000844
	.section .rom.0011b376, "ax"
	.incbin "baserom.gba", 0x0011b376, 0x00000002
	.section .rom.0011b756, "ax"
	.incbin "baserom.gba", 0x0011b756, 0x00000002
	.section .rom.0011c532, "ax"
	.incbin "baserom.gba", 0x0011c532, 0x00000002
	.section .rom.0011c666, "ax"
	.incbin "baserom.gba", 0x0011c666, 0x00000002
	.section .rom.0011e366, "ax"
	.incbin "baserom.gba", 0x0011e366, 0x00000002
	.section .rom.0011e3a6, "ax"
	.incbin "baserom.gba", 0x0011e3a6, 0x00000002
	.section .rom.0011e7d6, "ax"
	.incbin "baserom.gba", 0x0011e7d6, 0x00000002
	.section .rom.0011ff02, "ax"
	.incbin "baserom.gba", 0x0011ff02, 0x00000002
	.section .rom.00120450, "ax"
	.global BattleTarget_Execute
	.type BattleTarget_Execute, %function
	.thumb_func
BattleTarget_Execute:
	.incbin "baserom.gba", 0x00120450, 0x0000206c
	.section .rom.00123642, "ax"
	.incbin "baserom.gba", 0x00123642, 0x00000002
	.section .rom.00124e1c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00124e1c, 0x00000570
	.section .rom.001266fa, "ax"
	.incbin "baserom.gba", 0x001266fa, 0x00000002
	.section .rom.0012693e, "ax"
	.incbin "baserom.gba", 0x0012693e, 0x00000002
	.section .rom.001269a6, "ax"
	.incbin "baserom.gba", 0x001269a6, 0x00000002
	.section .rom.00127064, "ax"
	.global Func_08127068
	.type Func_08127068, %function
	.thumb_func
Func_08127068:
	.incbin "baserom.gba", 0x00127064, 0x00000248
	.section .rom.00127b9a, "ax"
	.incbin "baserom.gba", 0x00127b9a, 0x00000002
	.section .rom.00127cd0, "ax"
	.incbin "baserom.gba", 0x00127cd0, 0x000003cc
	.section .rom.0013d098, "ax"
	.balign 4
	.global Func_0813d098
	.type Func_0813d098, %function
	.thumb_func
Func_0813d098:
	.incbin "baserom.gba", 0x0013d098, 0x0000107c
	.section .rom.0013fd84, "ax"
	.global BattleEffect_RunCruelRuin
	.type BattleEffect_RunCruelRuin, %function
	.thumb_func
BattleEffect_RunCruelRuin:
	.incbin "baserom.gba", 0x0013fd84, 0x000017b8
	.global BattleEffect_RunEclipse
	.type BattleEffect_RunEclipse, %function
	.thumb_func
BattleEffect_RunEclipse:
	.incbin "baserom.gba", 0x0014153c, 0x00001408
	.section .rom.00145784, "ax"
	.balign 4
	.global BattleEffect_RunCondemn
	.type BattleEffect_RunCondemn, %function
	.thumb_func
BattleEffect_RunCondemn:
	.incbin "baserom.gba", 0x00145784, 0x000004e4
	.section .rom.00146a6c, "ax"
	.global BattleEffect_RunShiningStar
	.type BattleEffect_RunShiningStar, %function
	.thumb_func
BattleEffect_RunShiningStar:
	.incbin "baserom.gba", 0x00146a6c, 0x00000394
	.section .rom.00148458, "ax"
	.balign 4
	.global BattleEffect_RunNereid
	.type BattleEffect_RunNereid, %function
	.thumb_func
BattleEffect_RunNereid:
	.incbin "baserom.gba", 0x00148458, 0x000005fc
	.section .rom.0014a812, "ax"
	.incbin "baserom.gba", 0x0014a812, 0x00000002
	.section .rom.0014b160, "ax"
	.global BattleEffect_RunCurse
	.type BattleEffect_RunCurse, %function
	.thumb_func
BattleEffect_RunCurse:
	.incbin "baserom.gba", 0x0014b160, 0x000001f0
	.section .rom.0014b9e8, "ax"
	.balign 4
	.global Func_0814b9e8
	.type Func_0814b9e8, %function
	.thumb_func
Func_0814b9e8:
	.incbin "baserom.gba", 0x0014b9e8, 0x000004f8
	.global BattleEffect_RunZagan
	.type BattleEffect_RunZagan, %function
	.thumb_func
BattleEffect_RunZagan:
	.incbin "baserom.gba", 0x0014bee0, 0x00000a48
	.section .rom.0014cd46, "ax"
	.incbin "baserom.gba", 0x0014cd46, 0x00000002
	.section .rom.0014e744, "ax"
	.global BattleEffect_RunBreak
	.type BattleEffect_RunBreak, %function
	.thumb_func
BattleEffect_RunBreak:
	.incbin "baserom.gba", 0x0014e744, 0x000002f8
	.section .rom.0014fab8, "ax"
	.balign 4
	.global Func_0814fab8
	.type Func_0814fab8, %function
	.thumb_func
Func_0814fab8:
	.incbin "baserom.gba", 0x0014fab8, 0x000009f0
	.section .rom.001504d6, "ax"
	.incbin "baserom.gba", 0x001504d6, 0x00000002
	.section .rom.0015613e, "ax"
	.incbin "baserom.gba", 0x0015613e, 0x00000002
	.section .rom.00156140, "ax"
	.global BattleEffect_RunProjectileVolley
	.type BattleEffect_RunProjectileVolley, %function
	.thumb_func
BattleEffect_RunProjectileVolley:
	.incbin "baserom.gba", 0x00156140, 0x000013a0
	.section .rom.00157636, "ax"
	.incbin "baserom.gba", 0x00157636, 0x00000002
	.global BattleEffect_RunMercury
	.type BattleEffect_RunMercury, %function
	.thumb_func
BattleEffect_RunMercury:
	.incbin "baserom.gba", 0x00157638, 0x00000398
	.section .rom.00158dc8, "ax"
	.balign 4
	.global BattleEffect_RunThor
	.type BattleEffect_RunThor, %function
	.thumb_func
BattleEffect_RunThor:
	.incbin "baserom.gba", 0x00158dc8, 0x00000ee4
	.section .rom.0015b434, "ax"
	.global Func_0815b434
	.type Func_0815b434, %function
	.thumb_func
Func_0815b434:
	.incbin "baserom.gba", 0x0015b434, 0x000000dc
	.global Func_0815b510
	.type Func_0815b510, %function
	.thumb_func
Func_0815b510:
	.incbin "baserom.gba", 0x0015b510, 0x00000124
	.section .rom.0015b764, "ax"
	.balign 4
	.global BattleEffect_RunUlysses
	.type BattleEffect_RunUlysses, %function
	.thumb_func
BattleEffect_RunUlysses:
	.incbin "baserom.gba", 0x0015b764, 0x000011f4
	.section .rom.0015cbd0, "ax"
	.global BattleEffect_RunSpiderWeb
	.type BattleEffect_RunSpiderWeb, %function
	.thumb_func
BattleEffect_RunSpiderWeb:
	.incbin "baserom.gba", 0x0015cbd0, 0x00000218
	.global BattleEffect_RunAzul
	.type BattleEffect_RunAzul, %function
	.thumb_func
BattleEffect_RunAzul:
	.incbin "baserom.gba", 0x0015cde8, 0x00001388
	.section .rom.0015e288, "ax"
	.balign 4
	.global BattleUnit_ProjectToScreen
BattleUnit_ProjectToScreen:
	.incbin "baserom.gba", 0x0015e288, 0x00000074
	.section .rom.0015f16c, "ax"
	.balign 4
	.global Func_0815f16c
	.type Func_0815f16c, %function
	.thumb_func
Func_0815f16c:
	.incbin "baserom.gba", 0x0015f16c, 0x00004764
	.section .rom.00163e14, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00163e14, 0x00000618
	.section .rom.0016729c, "ax"
	.global BattleEffect_RunJudgment
	.type BattleEffect_RunJudgment, %function
	.thumb_func
BattleEffect_RunJudgment:
	.incbin "baserom.gba", 0x0016729c, 0x000017a4
	.section .rom.0016aed2, "ax"
	.incbin "baserom.gba", 0x0016aed2, 0x00000002
	.section .rom.0016aeec, "ax"
	.balign 4
	.global Func_0816aeec
Func_0816aeec:
	.incbin "baserom.gba", 0x0016aeec, 0x000007e8
	.section .rom.0016b6ec, "ax"
	.balign 4
	.global Func_0816b6ec
Func_0816b6ec:
	.incbin "baserom.gba", 0x0016b6ec, 0x00000584
	.global BattleEffect_RunDiamondBerg
	.type BattleEffect_RunDiamondBerg, %function
	.thumb_func
BattleEffect_RunDiamondBerg:
	.incbin "baserom.gba", 0x0016bc70, 0x00000604
	.global BattleEffect_RunCrystal
	.type BattleEffect_RunCrystal, %function
	.thumb_func
BattleEffect_RunCrystal:
	.incbin "baserom.gba", 0x0016c274, 0x00000484
	.global BattleEffect_RunGeode
	.type BattleEffect_RunGeode, %function
	.thumb_func
BattleEffect_RunGeode:
	.incbin "baserom.gba", 0x0016c6f8, 0x00000a38
	.global BattleEffect_RunNirvana
	.type BattleEffect_RunNirvana, %function
	.thumb_func
BattleEffect_RunNirvana:
	.incbin "baserom.gba", 0x0016d130, 0x000002a8
	.section .rom.0016d724, "ax"
	.global BattleEffect_RunSoulShatter
	.type BattleEffect_RunSoulShatter, %function
	.thumb_func
BattleEffect_RunSoulShatter:
	.incbin "baserom.gba", 0x0016d724, 0x0000052c
	.section .rom.0016f080, "ax"
	.global BattleEffect_RunWyrdCurse
	.type BattleEffect_RunWyrdCurse, %function
	.thumb_func
BattleEffect_RunWyrdCurse:
	.incbin "baserom.gba", 0x0016f080, 0x00000514
	.section .rom.0016fca8, "ax"
	.balign 4
	.global Func_0816fca8
Func_0816fca8:
	.incbin "baserom.gba", 0x0016fca8, 0x000006bc
	.global BattleEffect_RunMegaera
	.type BattleEffect_RunMegaera, %function
	.thumb_func
BattleEffect_RunMegaera:
	.incbin "baserom.gba", 0x00170364, 0x00000a48
	.section .rom.00171430, "ax"
	.global BattleEffect_RunLightSurge
	.type BattleEffect_RunLightSurge, %function
	.thumb_func
BattleEffect_RunLightSurge:
	.incbin "baserom.gba", 0x00171430, 0x00000744
	.section .rom.00171bb0, "ax"
	.balign 4
	.global Func_08171bb0
Func_08171bb0:
	.incbin "baserom.gba", 0x00171bb0, 0x00000f38
	.section .rom.00172b00, "ax"
	.balign 4
	.global Func_08172b00
Func_08172b00:
	.incbin "baserom.gba", 0x00172b00, 0x00000a70
	.section .rom.00179398, "ax"
	.global BattleEffect_RunRagingHeat
	.type BattleEffect_RunRagingHeat, %function
	.thumb_func
BattleEffect_RunRagingHeat:
	.incbin "baserom.gba", 0x00179398, 0x00000600
	.section .rom.00179f1a, "ax"
	.incbin "baserom.gba", 0x00179f1a, 0x00000002
	.global BattleEffect_RunFlora
	.type BattleEffect_RunFlora, %function
	.thumb_func
BattleEffect_RunFlora:
	.incbin "baserom.gba", 0x00179f1c, 0x000016bc
	.section .rom.0017bd8c, "ax"
	.global BattleEffect_RunFirePuppet
	.type BattleEffect_RunFirePuppet, %function
	.thumb_func
BattleEffect_RunFirePuppet:
	.incbin "baserom.gba", 0x0017bd8c, 0x000004f4
	.section .rom.0017c6b0, "ax"
	.global BattleEffect_RunFume
	.type BattleEffect_RunFume, %function
	.thumb_func
BattleEffect_RunFume:
	.incbin "baserom.gba", 0x0017c6b0, 0x00000584
	.section .rom.0017d728, "ax"
	.balign 4
	.global BattleEffect_RunRisingDragonEnemy
	.type BattleEffect_RunRisingDragonEnemy, %function
	.thumb_func
BattleEffect_RunRisingDragonEnemy:
	.incbin "baserom.gba", 0x0017d728, 0x00000954
	.section .rom.0017ea5c, "ax"
	.global BattleEffect_RunDaedalus
	.type BattleEffect_RunDaedalus, %function
	.thumb_func
BattleEffect_RunDaedalus:
	.incbin "baserom.gba", 0x0017ea5c, 0x0000223c
	.global BattleEffect_RunDaedalusAlt
	.type BattleEffect_RunDaedalusAlt, %function
	.thumb_func
BattleEffect_RunDaedalusAlt:
	.incbin "baserom.gba", 0x00180c98, 0x00000540
	.global BattleEffect_RunVengeance
	.type BattleEffect_RunVengeance, %function
	.thumb_func
BattleEffect_RunVengeance:
	.incbin "baserom.gba", 0x001811d8, 0x0000073c
	.section .rom.00182aac, "ax"
	.balign 4
	.global BattleEffect_RunCoatlicue
	.type BattleEffect_RunCoatlicue, %function
	.thumb_func
BattleEffect_RunCoatlicue:
	.incbin "baserom.gba", 0x00182aac, 0x00001954
	.global BattleEffect_RunCatastrophe
	.type BattleEffect_RunCatastrophe, %function
	.thumb_func
BattleEffect_RunCatastrophe:
	.incbin "baserom.gba", 0x00184400, 0x00002a00
	.global BattleEffect_RunHaures
	.type BattleEffect_RunHaures, %function
	.thumb_func
BattleEffect_RunHaures:
	.incbin "baserom.gba", 0x00186e00, 0x000017f4
	.section .rom.00188718, "ax"
	.global BattleEffect_RunDeathPlunge
	.type BattleEffect_RunDeathPlunge, %function
	.thumb_func
BattleEffect_RunDeathPlunge:
	.incbin "baserom.gba", 0x00188718, 0x0000059c
	.section .rom.0018ad44, "ax"
	.balign 4
	.global Func_0818ad40
Func_0818ad40:
	.incbin "baserom.gba", 0x0018ad44, 0x00000b28
	.section .rom.0018bc9c, "ax"
	.balign 4
	.global Func_0818bc98
Func_0818bc98:
	.incbin "baserom.gba", 0x0018bc9c, 0x00000b08
	.section .rom.0018c7bc, "ax"
	.balign 4
	.global Func_0818c7b8
Func_0818c7b8:
	.incbin "baserom.gba", 0x0018c7bc, 0x000002f0
	.section .rom.0018cb78, "ax"
	.global BattleEffect_RunTrident
	.type BattleEffect_RunTrident, %function
	.thumb_func
BattleEffect_RunTrident:
	.incbin "baserom.gba", 0x0018cb78, 0x00000a1c
	.section .rom.0018de40, "ax"
	.incbin "baserom.gba", 0x0018de40, 0x00000120
	.global BattleEffect_RunMeteorBlow
	.type BattleEffect_RunMeteorBlow, %function
	.thumb_func
BattleEffect_RunMeteorBlow:
	.incbin "baserom.gba", 0x0018df60, 0x00000e48
	.section .rom.0018f624, "ax"
	.balign 4
	.global Func_0818f620
	.type Func_0818f620, %function
	.thumb_func
Func_0818f620:
	.incbin "baserom.gba", 0x0018f624, 0x000008dc
	.global BattleEffect_RunMegiddo
	.type BattleEffect_RunMegiddo, %function
	.thumb_func
BattleEffect_RunMegiddo:
	.incbin "baserom.gba", 0x0018ff00, 0x00000c78
	.global BattleEffect_RunOdyssey
	.type BattleEffect_RunOdyssey, %function
	.thumb_func
BattleEffect_RunOdyssey:
	.incbin "baserom.gba", 0x00190b78, 0x000009bc
	.section .rom.0019195c, "ax"
	.incbin "baserom.gba", 0x0019195c, 0x00000188
	.section .rom.00192850, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00192850, 0x00000450
	.global BattleEffect_RunIris
	.type BattleEffect_RunIris, %function
	.thumb_func
BattleEffect_RunIris:
	.incbin "baserom.gba", 0x00192ca0, 0x00002e00
	.section .rom.001a08c4, "ax"
	.global Func_081a08c4
	.type Func_081a08c4, %function
	.thumb_func
Func_081a08c4:
	.incbin "baserom.gba", 0x001a08c4, 0x00000128
	.section .rom.001a0bd4, "ax"
	.balign 4
	.global DisplayScroll_DrawMessageLine
	.type DisplayScroll_DrawMessageLine, %function
	.thumb_func
DisplayScroll_DrawMessageLine:
	.incbin "baserom.gba", 0x001a0bd4, 0x000000c4
	.section .rom.001a14f8, "ax"
	.global Func_081a150c
	.type Func_081a150c, %function
	.thumb_func
Func_081a150c:
	.incbin "baserom.gba", 0x001a14f8, 0x000000d0
	.section .rom.001a68b4, "ax"
	.global Func_081a68b4
	.type Func_081a68b4, %function
	.thumb_func
Func_081a68b4:
	.incbin "baserom.gba", 0x001a68b4, 0x00000200
	.section .rom.001a6b50, "ax"
	.global Func_081a6b50
	.type Func_081a6b50, %function
	.thumb_func
Func_081a6b50:
	.incbin "baserom.gba", 0x001a6b50, 0x00000350
	.section .rom.001a729c, "ax"
	.balign 4
	.global Func_081a729c
	.type Func_081a729c, %function
	.thumb_func
Func_081a729c:
	.incbin "baserom.gba", 0x001a729c, 0x00000278
	.section .rom.001a7514, "ax"
	.global Title_NoOpPalette
	.type Title_NoOpPalette, %function
	.thumb_func
Title_NoOpPalette:
	.incbin "baserom.gba", 0x001a7514, 0x00000004
	.section .rom.001ac15c, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x001ac15c, 0x000011ec
	.section .rom.001b243c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001b243c, 0x00000d14
	.section .rom.001b3364, "ax"
	.global LuckyWheels_Run
	.type LuckyWheels_Run, %function
	.thumb_func
LuckyWheels_Run:
	.incbin "baserom.gba", 0x001b3364, 0x00000988
	.section .rom.001b8272, "ax"
	.incbin "baserom.gba", 0x001b8272, 0x00000002
	.global Func_081b8274
	.type Func_081b8274, %function
	.thumb_func
Func_081b8274:
	.incbin "baserom.gba", 0x001b8274, 0x000000d8
	.section .rom.001c0428, "ax"
	.global Func_081c0428
Func_081c0428:
	.incbin "baserom.gba", 0x001c0428, 0x00000064
	.section .rom.001c04cc, "ax"
	.incbin "baserom.gba", 0x001c04cc, 0x0000011c
	.section .rom.001c075a, "ax"
	.global Func_081c075a
	.type Func_081c075a, %function
	.thumb_func
Func_081c075a:
	.incbin "baserom.gba", 0x001c075a, 0x00000018
	.section .rom.00890301, "ax"
	.incbin "baserom.gba", 0x00890301, 0x00000003
	.section .rom.008ababd, "ax"
	.incbin "baserom.gba", 0x008ababd, 0x00000003
	.section .rom.0093c3d1, "ax"
	.incbin "baserom.gba", 0x0093c3d1, 0x00000003
	.section .rom.00949efa, "ax"
	.incbin "baserom.gba", 0x00949efa, 0x00000002
	.section .rom.0096a4c7, "ax"
	.incbin "baserom.gba", 0x0096a4c7, 0x00000001
	.section .rom.00bad68e, "ax"
	.incbin "baserom.gba", 0x00bad68e, 0x00000002
	.section .rom.00c2e3bd, "ax"
	.incbin "baserom.gba", 0x00c2e3bd, 0x00000003
	.section .rom.00d57dea, "ax"
	.incbin "baserom.gba", 0x00d57dea, 0x00000002
	.section .rom.00e0df49, "ax"
	.incbin "baserom.gba", 0x00e0df49, 0x00000003
	.section .rom.00eba2e5, "ax"
	.incbin "baserom.gba", 0x00eba2e5, 0x00000003
	.section .rom.0004020c, "ax"
	.global Func_080400e8
	.type Func_080400e8, %function
	.thumb_func
Func_080400e8:
	.incbin "baserom.gba", 0x0004020c, 0x000004c4
	.section .rom.00049c20, "ax"
	.balign 4
	.global Func_08049a30
	.type Func_08049a30, %function
	.thumb_func
Func_08049a30:
	.incbin "baserom.gba", 0x00049c20, 0x00000704
	.section .rom.0004a324, "ax"
	.balign 4
	.global Func_0804a134
	.type Func_0804a134, %function
	.thumb_func
Func_0804a134:
	.incbin "baserom.gba", 0x0004a324, 0x00000728
