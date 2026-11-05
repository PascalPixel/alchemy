	.section .rom.000133f0, "ax"
	.balign 4
	.global System_InstallIwramAndIrqs
	.type System_InstallIwramAndIrqs, %function
	.thumb_func
System_InstallIwramAndIrqs:
	.incbin "baserom.gba", 0x000133f0, 0x00000074
	.section .rom.000134dc, "ax"
	.global System_WaitForFrameInterrupt
	.type System_WaitForFrameInterrupt, %function
	.thumb_func
System_WaitForFrameInterrupt:
	.incbin "baserom.gba", 0x000134dc, 0x000000b0
	.section .rom.000141a0, "ax"
	.global VramCache_AllocateBlocks
	.type VramCache_AllocateBlocks, %function
	.thumb_func
VramCache_AllocateBlocks:
	.incbin "baserom.gba", 0x000141a0, 0x00000080
	.section .rom.0001553c, "ax"
	.global Transform_BuildLookAt
	.type Transform_BuildLookAt, %function
	.thumb_func
Transform_BuildLookAt:
	.incbin "baserom.gba", 0x0001553c, 0x000001d8
	.section .rom.000157a4, "ax"
	.global Render_ProjectPoint
	.type Render_ProjectPoint, %function
	.thumb_func
Render_ProjectPoint:
	.incbin "baserom.gba", 0x000157a4, 0x000000b8
	.section .rom.00016306, "ax"
	.incbin "baserom.gba", 0x00016306, 0x00000002
	.section .rom.00016e2e, "ax"
	.incbin "baserom.gba", 0x00016e2e, 0x00000002
	.section .rom.000212d8, "ax"
	.incbin "baserom.gba", 0x000212d8, 0x0000042c
	.section .rom.00022010, "ax"
	.global Func_08022010
	.type Func_08022010, %function
	.thumb_func
Func_08022010:
	.incbin "baserom.gba", 0x00022010, 0x000000e0
	.section .rom.00022318, "ax"
	.global Func_08022318
	.type Func_08022318, %function
	.thumb_func
Func_08022318:
	.incbin "baserom.gba", 0x00022318, 0x00000234
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
	.section .rom.000292fc, "ax"
	.global Func_080292fc
	.type Func_080292fc, %function
	.thumb_func
Func_080292fc:
	.incbin "baserom.gba", 0x000292fc, 0x00000274
	.global Func_08029570
	.type Func_08029570, %function
	.thumb_func
Func_08029570:
	.incbin "baserom.gba", 0x00029570, 0x000002a0
	.section .rom.000298c0, "ax"
	.global Func_080298c0
	.type Func_080298c0, %function
	.thumb_func
Func_080298c0:
	.incbin "baserom.gba", 0x000298c0, 0x00000864
	.global Func_0802a124
	.type Func_0802a124, %function
	.thumb_func
Func_0802a124:
	.incbin "baserom.gba", 0x0002a124, 0x00000408
	.section .rom.0002a6b8, "ax"
	.global Func_0802a6b8
	.type Func_0802a6b8, %function
	.thumb_func
Func_0802a6b8:
	.incbin "baserom.gba", 0x0002a6b8, 0x000003bc
	.section .rom.0002b450, "ax"
	.global Func_0802b450
	.type Func_0802b450, %function
	.thumb_func
Func_0802b450:
	.incbin "baserom.gba", 0x0002b450, 0x00000140
	.section .rom.0002b998, "ax"
	.global Func_0802b998
	.type Func_0802b998, %function
	.thumb_func
Func_0802b998:
	.incbin "baserom.gba", 0x0002b998, 0x00000370
	.section .rom.0002bdd0, "ax"
	.balign 4
	.global Func_0802bdd0
	.type Func_0802bdd0, %function
	.thumb_func
Func_0802bdd0:
	.incbin "baserom.gba", 0x0002bdd0, 0x0000007c
	.section .rom.0002be4c, "ax"
	.global Func_0802be4c
	.type Func_0802be4c, %function
	.thumb_func
Func_0802be4c:
	.incbin "baserom.gba", 0x0002be4c, 0x0000023c
	.section .rom.0002c58c, "ax"
	.global Func_0802c58c
	.type Func_0802c58c, %function
	.thumb_func
Func_0802c58c:
	.incbin "baserom.gba", 0x0002c58c, 0x000002d8
	.section .rom.0002dd70, "ax"
	.global Func_0802dd70
	.type Func_0802dd70, %function
	.thumb_func
Func_0802dd70:
	.incbin "baserom.gba", 0x0002dd70, 0x0000011c
	.section .rom.0002de8c, "ax"
	.global Func_0802de8c
	.type Func_0802de8c, %function
	.thumb_func
Func_0802de8c:
	.incbin "baserom.gba", 0x0002de8c, 0x000005b8
	.section .rom.0003939a, "ax"
	.incbin "baserom.gba", 0x0003939a, 0x00000002
	.section .rom.00039452, "ax"
	.incbin "baserom.gba", 0x00039452, 0x00000002
	.section .rom.000394e6, "ax"
	.incbin "baserom.gba", 0x000394e6, 0x00000002
	.section .rom.0003954c, "ax"
	.balign 4
	.global Message_Start
	.type Message_Start, %function
	.thumb_func
Message_Start:
	.incbin "baserom.gba", 0x0003954c, 0x000000b0
	.section .rom.0003972a, "ax"
	.incbin "baserom.gba", 0x0003972a, 0x00000002
	.section .rom.00039864, "ax"
	.global Message_StepScript
	.type Message_StepScript, %function
	.thumb_func
Message_StepScript:
	.incbin "baserom.gba", 0x00039864, 0x00000650
	.section .rom.0003a42a, "ax"
	.incbin "baserom.gba", 0x0003a42a, 0x00000002
	.section .rom.0003aaae, "ax"
	.incbin "baserom.gba", 0x0003aaae, 0x00000002
	.section .rom.0003ac2c, "ax"
	.incbin "baserom.gba", 0x0003ac2c, 0x0000001c
	.section .rom.0003aea0, "ax"
	.incbin "baserom.gba", 0x0003aea0, 0x000001a4
	.section .rom.0003b044, "ax"
	.global Message_Decode
	.type Message_Decode, %function
	.thumb_func
Message_Decode:
	.incbin "baserom.gba", 0x0003b044, 0x00000768
	.section .rom.0003bb6e, "ax"
	.incbin "baserom.gba", 0x0003bb6e, 0x00000002
	.global Func_0803bb58
	.type Func_0803bb58, %function
	.thumb_func
Func_0803bb58:
	.incbin "baserom.gba", 0x0003bb70, 0x0000027c
	.global Func_0803bde4
	.thumb_func
Func_0803bde4:
	.incbin "baserom.gba", 0x0003bdec, 0x00000284
	.section .rom.0003c176, "ax"
	.incbin "baserom.gba", 0x0003c176, 0x00000002
	.section .rom.0003c1a0, "ax"
	.balign 4
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c1a0, 0x000000dc
	.section .rom.0003c27c, "ax"
	.global Func_0803c274
	.type Func_0803c274, %function
	.thumb_func
Func_0803c274:
	.incbin "baserom.gba", 0x0003c27c, 0x00000104
	.section .rom.0003e828, "ax"
	.balign 4
	.global Func_0803e7c8
	.type Func_0803e7c8, %function
	.thumb_func
Func_0803e7c8:
	.incbin "baserom.gba", 0x0003e828, 0x00000124
	.section .rom.0003ec3c, "ax"
	.balign 4
	.global Menu_StepRight
	.type Menu_StepRight, %function
	.thumb_func
Menu_StepRight:
	.incbin "baserom.gba", 0x0003ec3c, 0x000001bc
	.section .rom.0003efa6, "ax"
	.incbin "baserom.gba", 0x0003efa6, 0x00000002
	.section .rom.0003f232, "ax"
	.incbin "baserom.gba", 0x0003f232, 0x00000002
	.section .rom.0003f428, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0003f428, 0x00000248
	.section .rom.0003fd88, "ax"
	.global Menu_OpenWorkspaceOptions
	.type Menu_OpenWorkspaceOptions, %function
	.thumb_func
Menu_OpenWorkspaceOptions:
	.incbin "baserom.gba", 0x0003fd88, 0x000003c0
	.section .rom.00040aa6, "ax"
	.incbin "baserom.gba", 0x00040aa6, 0x00000002
	.section .rom.0004172a, "ax"
	.incbin "baserom.gba", 0x0004172a, 0x00000002
	.section .rom.0004172c, "ax"
	.global Text_DrawStringAsTiles
	.type Text_DrawStringAsTiles, %function
	.thumb_func
Text_DrawStringAsTiles:
	.incbin "baserom.gba", 0x0004172c, 0x000003fc
	.section .rom.0004207a, "ax"
	.incbin "baserom.gba", 0x0004207a, 0x00000002
	.section .rom.000421f2, "ax"
	.incbin "baserom.gba", 0x000421f2, 0x00000002
	.section .rom.000422e0, "ax"
	.balign 4
	.global Text_DrawPrefixedNumberAtOffset
	.type Text_DrawPrefixedNumberAtOffset, %function
	.thumb_func
Text_DrawPrefixedNumberAtOffset:
	.incbin "baserom.gba", 0x000422e0, 0x000000a0
	.section .rom.000425e6, "ax"
	.incbin "baserom.gba", 0x000425e6, 0x00000002
	.section .rom.0004274c, "ax"
	.global Func_080426e0
	.type Func_080426e0, %function
	.thumb_func
Func_080426e0:
	.incbin "baserom.gba", 0x0004274c, 0x00000128
	.section .rom.00043d44, "ax"
	.global Func_08043cd8
	.type Func_08043cd8, %function
	.thumb_func
Func_08043cd8:
	.incbin "baserom.gba", 0x00043d44, 0x0000066c
	.section .rom.000445c4, "ax"
	.balign 4
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x000445c4, 0x0000051c
	.section .rom.00044e54, "ax"
	.global Func_08044dc8
	.type Func_08044dc8, %function
	.thumb_func
Func_08044dc8:
	.incbin "baserom.gba", 0x00044e54, 0x00000184
	.section .rom.00045346, "ax"
	.incbin "baserom.gba", 0x00045346, 0x00000002
	.section .rom.000455b2, "ax"
	.incbin "baserom.gba", 0x000455b2, 0x00000002
	.section .rom.0004580a, "ax"
	.incbin "baserom.gba", 0x0004580a, 0x00000002
	.section .rom.0004585a, "ax"
	.incbin "baserom.gba", 0x0004585a, 0x00000002
	.section .rom.0004649e, "ax"
	.incbin "baserom.gba", 0x0004649e, 0x00000002
	.section .rom.00046b94, "ax"
	.incbin "baserom.gba", 0x00046b94, 0x000026f8
	.section .rom.00049b3a, "ax"
	.incbin "baserom.gba", 0x00049b3a, 0x00000002
	.section .rom.0004a994, "ax"
	.incbin "baserom.gba", 0x0004a994, 0x00000f28
	.section .rom.0004bad2, "ax"
	.incbin "baserom.gba", 0x0004bad2, 0x00000206
	.section .rom.0004bdc2, "ax"
	.incbin "baserom.gba", 0x0004bdc2, 0x00000002
	.section .rom.0004bdec, "ax"
	.incbin "baserom.gba", 0x0004bdec, 0x00000038
	.section .rom.0004be24, "ax"
	.global Func_0804bc08
	.type Func_0804bc08, %function
	.thumb_func
Func_0804bc08:
	.incbin "baserom.gba", 0x0004be24, 0x000011a0
	.section .rom.0004d69e, "ax"
	.incbin "baserom.gba", 0x0004d69e, 0x00000002
	.section .rom.0004d9f0, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004d9f0, 0x0000016c
	.section .rom.0004dcae, "ax"
	.incbin "baserom.gba", 0x0004dcae, 0x00000002
	.section .rom.000b63f6, "ax"
	.incbin "baserom.gba", 0x000b63f6, 0x00000002
	.section .rom.000b7410, "ax"
	.balign 4
	.global Func_080ae410
	.type Func_080ae410, %function
	.thumb_func
Func_080ae410:
	.incbin "baserom.gba", 0x000b7410, 0x000001ec
	.section .rom.000b7830, "ax"
	.incbin "baserom.gba", 0x000b7830, 0x0000000c
	.section .rom.000b7874, "ax"
	.global Func_080ae868
	.type Func_080ae868, %function
	.thumb_func
Func_080ae868:
	.incbin "baserom.gba", 0x000b7874, 0x000001c8
	.section .rom.000b7c72, "ax"
	.incbin "baserom.gba", 0x000b7c72, 0x00000002
	.section .rom.000b7c74, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000b7c74, 0x0000003c
	.section .rom.000b80ee, "ax"
	.incbin "baserom.gba", 0x000b80ee, 0x00000002
	.section .rom.000b9150, "ax"
	.global Class_Determine
	.type Class_Determine, %function
	.thumb_func
Class_Determine:
	.incbin "baserom.gba", 0x000b9150, 0x00000144
	.section .rom.000d36e4, "ax"
	.global Func_080ca6e4
	.type Func_080ca6e4, %function
	.thumb_func
Func_080ca6e4:
	.incbin "baserom.gba", 0x000d36e4, 0x00000004
	.section .rom.000d46c6, "ax"
	.incbin "baserom.gba", 0x000d46c6, 0x00000002
	.section .rom.000d491c, "ax"
	.global FieldSequence_Run
	.type FieldSequence_Run, %function
	.thumb_func
FieldSequence_Run:
	.incbin "baserom.gba", 0x000d491c, 0x00000c30
	.section .rom.000d57c4, "ax"
	.global Func_080cc7c4
Func_080cc7c4:
	.incbin "baserom.gba", 0x000d57c4, 0x000001d0
	.section .rom.000d5d76, "ax"
	.incbin "baserom.gba", 0x000d5d76, 0x00000002
	.section .rom.000d5d78, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000d5d78, 0x0000010c
	.section .rom.000d6808, "ax"
	.incbin "baserom.gba", 0x000d6808, 0x00000114
	.section .rom.000d70aa, "ax"
	.incbin "baserom.gba", 0x000d70aa, 0x00000002
	.section .rom.000d731c, "ax"
	.global Func_080ce31c
	.type Func_080ce31c, %function
	.thumb_func
Func_080ce31c:
	.incbin "baserom.gba", 0x000d731c, 0x0000013c
	.section .rom.000d7f22, "ax"
	.incbin "baserom.gba", 0x000d7f22, 0x00000002
	.section .rom.000d8554, "ax"
	.global Func_080cf554
	.type Func_080cf554, %function
	.thumb_func
Func_080cf554:
	.incbin "baserom.gba", 0x000d8554, 0x000001a8
	.section .rom.000d91cc, "ax"
	.global ScreenTransition_Start
	.type ScreenTransition_Start, %function
	.thumb_func
ScreenTransition_Start:
	.incbin "baserom.gba", 0x000d91cc, 0x00000354
	.section .rom.000d9e1c, "ax"
	.global BattleEffect_BuildBuffer
	.type BattleEffect_BuildBuffer, %function
	.thumb_func
BattleEffect_BuildBuffer:
	.incbin "baserom.gba", 0x000d9e1c, 0x00000868
	.section .rom.000da75e, "ax"
	.incbin "baserom.gba", 0x000da75e, 0x00000002
	.section .rom.000da760, "ax"
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000da760, 0x0000004c
	.section .rom.000da83e, "ax"
	.incbin "baserom.gba", 0x000da83e, 0x00000002
	.section .rom.000daa16, "ax"
	.incbin "baserom.gba", 0x000daa16, 0x00000002
	.section .rom.000daf20, "ax"
	.global Func_080d1f20
	.type Func_080d1f20, %function
	.thumb_func
Func_080d1f20:
	.incbin "baserom.gba", 0x000daf20, 0x00000164
	.section .rom.000db60c, "ax"
	.balign 4
	.global PartyInventory_GiveItem
	.type PartyInventory_GiveItem, %function
	.thumb_func
PartyInventory_GiveItem:
	.incbin "baserom.gba", 0x000db60c, 0x000001d4
	.section .rom.000dbc90, "ax"
	.balign 4
	.global Func_080d2c9c
	.type Func_080d2c9c, %function
	.thumb_func
Func_080d2c9c:
	.incbin "baserom.gba", 0x000dbc90, 0x00000028
	.section .rom.000dbcb8, "ax"
	.global Func_080d2cc4
	.type Func_080d2cc4, %function
	.thumb_func
Func_080d2cc4:
	.incbin "baserom.gba", 0x000dbcb8, 0x00000044
	.section .rom.000dbcfc, "ax"
	.global Func_080d2d08
	.type Func_080d2d08, %function
	.thumb_func
Func_080d2d08:
	.incbin "baserom.gba", 0x000dbcfc, 0x00000068
	.section .rom.000dbd64, "ax"
	.global Func_080d2d70
	.type Func_080d2d70, %function
	.thumb_func
Func_080d2d70:
	.incbin "baserom.gba", 0x000dbd64, 0x00000014
	.section .rom.000dc36a, "ax"
	.incbin "baserom.gba", 0x000dc36a, 0x00000002
	.section .rom.000dc452, "ax"
	.incbin "baserom.gba", 0x000dc452, 0x00000002
	.section .rom.000dc932, "ax"
	.incbin "baserom.gba", 0x000dc932, 0x00000002
	.section .rom.000dd1da, "ax"
	.incbin "baserom.gba", 0x000dd1da, 0x00000002
	.section .rom.000ddcfc, "ax"
	.balign 4
	.global Func_080d4d08
	.type Func_080d4d08, %function
	.thumb_func
Func_080d4d08:
	.incbin "baserom.gba", 0x000ddcfc, 0x000003f0
	.section .rom.000de3aa, "ax"
	.incbin "baserom.gba", 0x000de3aa, 0x00000002
	.section .rom.000de662, "ax"
	.incbin "baserom.gba", 0x000de662, 0x00000002
	.section .rom.000de7d6, "ax"
	.incbin "baserom.gba", 0x000de7d6, 0x00000002
	.section .rom.000dea92, "ax"
	.incbin "baserom.gba", 0x000dea92, 0x00000002
	.section .rom.000df668, "ax"
	.global Func_080d6674
	.type Func_080d6674, %function
	.thumb_func
Func_080d6674:
	.incbin "baserom.gba", 0x000df668, 0x00000174
	.section .rom.000df9e8, "ax"
	.global Func_080d69f4
	.type Func_080d69f4, %function
	.thumb_func
Func_080d69f4:
	.incbin "baserom.gba", 0x000df9e8, 0x0000019c
	.section .rom.000e0518, "ax"
	.global Func_080d7524
	.type Func_080d7524, %function
	.thumb_func
Func_080d7524:
	.incbin "baserom.gba", 0x000e0518, 0x00000264
	.section .rom.000e0840, "ax"
	.incbin "baserom.gba", 0x000e0840, 0x000000f0
	.section .rom.000e19f4, "ax"
	.global Func_080d8a00
	.type Func_080d8a00, %function
	.thumb_func
Func_080d8a00:
	.incbin "baserom.gba", 0x000e19f4, 0x00000210
	.section .rom.000e1c04, "ax"
	.global Func_080d8c10
	.type Func_080d8c10, %function
	.thumb_func
Func_080d8c10:
	.incbin "baserom.gba", 0x000e1c04, 0x00000130
	.section .rom.000e1dfc, "ax"
	.global Func_080d8e08
	.type Func_080d8e08, %function
	.thumb_func
Func_080d8e08:
	.incbin "baserom.gba", 0x000e1dfc, 0x000001a0
	.section .rom.000e1fec, "ax"
	.global Func_080d8ff8
	.type Func_080d8ff8, %function
	.thumb_func
Func_080d8ff8:
	.incbin "baserom.gba", 0x000e1fec, 0x0000010c
	.section .rom.000e2188, "ax"
	.global Func_080d9194
	.type Func_080d9194, %function
	.thumb_func
Func_080d9194:
	.incbin "baserom.gba", 0x000e2188, 0x00000110
	.section .rom.000e2f0e, "ax"
	.incbin "baserom.gba", 0x000e2f0e, 0x00000002
	.section .rom.000e30a4, "ax"
	.global Func_080da0b0
	.type Func_080da0b0, %function
	.thumb_func
Func_080da0b0:
	.incbin "baserom.gba", 0x000e30a4, 0x00000108
	.section .rom.000e321e, "ax"
	.incbin "baserom.gba", 0x000e321e, 0x00000002
	.section .rom.000e3220, "ax"
	.global Func_080da22c
	.type Func_080da22c, %function
	.thumb_func
Func_080da22c:
	.incbin "baserom.gba", 0x000e3220, 0x00000108
	.section .rom.000e395a, "ax"
	.incbin "baserom.gba", 0x000e395a, 0x00000002
	.section .rom.000e40a4, "ax"
	.global Func_080db0b0
	.type Func_080db0b0, %function
	.thumb_func
Func_080db0b0:
	.incbin "baserom.gba", 0x000e40a4, 0x00000394
	.section .rom.000e499a, "ax"
	.incbin "baserom.gba", 0x000e499a, 0x00000002
	.section .rom.000e4b6c, "ax"
	.balign 4
	.global Func_080dbb78
Func_080dbb78:
	.global Map_GetCellAtPosition
	.type Map_GetCellAtPosition, %function
	.thumb_func
Map_GetCellAtPosition:
	.incbin "baserom.gba", 0x000e4b6c, 0x0000008c
	.section .rom.000e4d4e, "ax"
	.incbin "baserom.gba", 0x000e4d4e, 0x00000002
	.section .rom.000e4d9c, "ax"
	.global Func_080dbda8
	.type Func_080dbda8, %function
	.thumb_func
Func_080dbda8:
	.incbin "baserom.gba", 0x000e4d9c, 0x00000020
	.section .rom.000e4dbc, "ax"
	.global Func_080dbdc8
	.type Func_080dbdc8, %function
	.thumb_func
Func_080dbdc8:
	.incbin "baserom.gba", 0x000e4dbc, 0x0000000c
	.section .rom.000e4dc8, "ax"
	.global Func_080dbdd4
	.type Func_080dbdd4, %function
	.thumb_func
Func_080dbdd4:
	.incbin "baserom.gba", 0x000e4dc8, 0x00000014
	.section .rom.000e4ddc, "ax"
	.global Func_080dbde8
	.type Func_080dbde8, %function
	.thumb_func
Func_080dbde8:
	.incbin "baserom.gba", 0x000e4ddc, 0x0000000c
	.section .rom.000e4de8, "ax"
	.global Func_080dbdf4
	.type Func_080dbdf4, %function
	.thumb_func
Func_080dbdf4:
	.incbin "baserom.gba", 0x000e4de8, 0x00000014
	.section .rom.000e4dfc, "ax"
	.global Func_080dbe08
	.type Func_080dbe08, %function
	.thumb_func
Func_080dbe08:
	.incbin "baserom.gba", 0x000e4dfc, 0x0000003c
	.section .rom.000e4e38, "ax"
	.global Func_080dbe44
	.type Func_080dbe44, %function
	.thumb_func
Func_080dbe44:
	.incbin "baserom.gba", 0x000e4e38, 0x0000003c
	.section .rom.000e4e74, "ax"
	.global Func_080dbe80
Func_080dbe80:
	.incbin "baserom.gba", 0x000e4e74, 0x00000050
	.global Func_080dbed0
	.type Func_080dbed0, %function
	.thumb_func
Func_080dbed0:
	.incbin "baserom.gba", 0x000e4ec4, 0x000000c4
	.section .rom.000e56cc, "ax"
	.global Func_080dc6d8
	.type Func_080dc6d8, %function
	.thumb_func
Func_080dc6d8:
	.incbin "baserom.gba", 0x000e56cc, 0x000000f4
	.section .rom.000e57dc, "ax"
	.balign 4
	.global BattleEffect_CleanupSceneObjects
	.type BattleEffect_CleanupSceneObjects, %function
	.thumb_func
BattleEffect_CleanupSceneObjects:
	.incbin "baserom.gba", 0x000e57dc, 0x0000016c
	.section .rom.000e6048, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
	.incbin "baserom.gba", 0x000e6048, 0x000003e8
	.section .rom.000e651a, "ax"
	.incbin "baserom.gba", 0x000e651a, 0x00000002
	.section .rom.000e6a22, "ax"
	.incbin "baserom.gba", 0x000e6a22, 0x00000002
	.section .rom.000e88d2, "ax"
	.incbin "baserom.gba", 0x000e88d2, 0x00000002
	.section .rom.000e93b6, "ax"
	.incbin "baserom.gba", 0x000e93b6, 0x00000002
	.section .rom.000e960c, "ax"
	.global Func_080e0618
	.type Func_080e0618, %function
	.thumb_func
Func_080e0618:
	.incbin "baserom.gba", 0x000e960c, 0x00000360
	.section .rom.000ea43c, "ax"
	.global BattleEffect_SpawnScaledArcObjects
	.type BattleEffect_SpawnScaledArcObjects, %function
	.thumb_func
BattleEffect_SpawnScaledArcObjects:
	.incbin "baserom.gba", 0x000ea43c, 0x000000c4
	.section .rom.000eaf20, "ax"
	.global Func_080e1f2c
	.type Func_080e1f2c, %function
	.thumb_func
Func_080e1f2c:
	.incbin "baserom.gba", 0x000eaf20, 0x00000338
	.section .rom.000eb258, "ax"
	.global Func_080e2264
	.type Func_080e2264, %function
	.thumb_func
Func_080e2264:
	.incbin "baserom.gba", 0x000eb258, 0x00000238
	.section .rom.000eb490, "ax"
	.global Func_080e249c
	.type Func_080e249c, %function
	.thumb_func
Func_080e249c:
	.incbin "baserom.gba", 0x000eb490, 0x0000014c
	.section .rom.000eb70c, "ax"
	.incbin "baserom.gba", 0x000eb70c, 0x0000005c
	.section .rom.000eb768, "ax"
	.global Func_080e2774
	.type Func_080e2774, %function
	.thumb_func
Func_080e2774:
	.incbin "baserom.gba", 0x000eb768, 0x00000160
	.section .rom.000ebac0, "ax"
	.global Func_080e2acc
	.type Func_080e2acc, %function
	.thumb_func
Func_080e2acc:
	.incbin "baserom.gba", 0x000ebac0, 0x0000016c
	.section .rom.000ebc2c, "ax"
	.global Func_080e2c38
	.type Func_080e2c38, %function
	.thumb_func
Func_080e2c38:
	.incbin "baserom.gba", 0x000ebc2c, 0x00000108
	.section .rom.000ec8bc, "ax"
	.global Func_080e38c8
	.type Func_080e38c8, %function
	.thumb_func
Func_080e38c8:
	.incbin "baserom.gba", 0x000ec8bc, 0x00000434
	.section .rom.000ed724, "ax"
	.balign 4
	.global Func_080e4730
	.type Func_080e4730, %function
	.thumb_func
Func_080e4730:
	.incbin "baserom.gba", 0x000ed724, 0x000002fc
	.section .rom.000eda28, "ax"
	.global Func_080e4a34
	.type Func_080e4a34, %function
	.thumb_func
Func_080e4a34:
	.incbin "baserom.gba", 0x000eda28, 0x00000368
	.section .rom.000ee720, "ax"
	.global Func_080e572c
	.type Func_080e572c, %function
	.thumb_func
Func_080e572c:
	.incbin "baserom.gba", 0x000ee720, 0x000005c4
	.section .rom.000eed50, "ax"
	.global Func_080e5d5c
	.type Func_080e5d5c, %function
	.thumb_func
Func_080e5d5c:
	.incbin "baserom.gba", 0x000eed50, 0x0000062c
	.section .rom.000ef3f4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000ef3f4, 0x000004c8
	.section .rom.000ef8c4, "ax"
	.global Func_080e68d0
	.type Func_080e68d0, %function
	.thumb_func
Func_080e68d0:
	.incbin "baserom.gba", 0x000ef8c4, 0x00000828
	.section .rom.000f0838, "ax"
	.global Func_080e7844
	.type Func_080e7844, %function
	.thumb_func
Func_080e7844:
	.incbin "baserom.gba", 0x000f0838, 0x00000750
	.section .rom.000f1070, "ax"
	.global Func_080e807c
	.type Func_080e807c, %function
	.thumb_func
Func_080e807c:
	.incbin "baserom.gba", 0x000f1070, 0x00000250
	.section .rom.000f17b8, "ax"
	.global Func_080e87c4
	.type Func_080e87c4, %function
	.thumb_func
Func_080e87c4:
	.incbin "baserom.gba", 0x000f17b8, 0x00000220
	.section .rom.000f2084, "ax"
	.global Func_080e9090
	.type Func_080e9090, %function
	.thumb_func
Func_080e9090:
	.incbin "baserom.gba", 0x000f2084, 0x000001c0
	.section .rom.000f3140, "ax"
	.balign 4
	.global Func_080ea14c
	.type Func_080ea14c, %function
	.thumb_func
Func_080ea14c:
	.incbin "baserom.gba", 0x000f3140, 0x00000674
	.section .rom.000f37b4, "ax"
	.global Func_080ea7c0
	.type Func_080ea7c0, %function
	.thumb_func
Func_080ea7c0:
	.global Func_080ea7c0
	.type Func_080ea7c0, %function
	.thumb_func
Func_080ea7c0:
	.incbin "baserom.gba", 0x000f37b4, 0x000000e8
	.section .rom.000f38c6, "ax"
	.incbin "baserom.gba", 0x000f38c6, 0x00000002
	.section .rom.000f38c8, "ax"
	.global Func_080ea8d4
	.type Func_080ea8d4, %function
	.thumb_func
Func_080ea8d4:
	.incbin "baserom.gba", 0x000f38c8, 0x00000140
	.section .rom.000f3a08, "ax"
	.global Func_080eaa14
	.type Func_080eaa14, %function
	.thumb_func
Func_080eaa14:
	.incbin "baserom.gba", 0x000f3a08, 0x0000015c
	.section .rom.000f3bc4, "ax"
	.global Func_080eabd0
	.type Func_080eabd0, %function
	.thumb_func
Func_080eabd0:
	.incbin "baserom.gba", 0x000f3bc4, 0x00000118
	.section .rom.000f3df0, "ax"
	.global Func_080eadfc
	.type Func_080eadfc, %function
	.thumb_func
Func_080eadfc:
	.incbin "baserom.gba", 0x000f3df0, 0x000000b8
	.section .rom.000f428a, "ax"
	.incbin "baserom.gba", 0x000f428a, 0x00000002
	.section .rom.000f4818, "ax"
	.balign 4
	.global Func_080eb824
	.type Func_080eb824, %function
	.thumb_func
Func_080eb824:
	.incbin "baserom.gba", 0x000f4818, 0x0000010c
	.section .rom.000f4954, "ax"
	.global Func_080eb960
	.type Func_080eb960, %function
	.thumb_func
Func_080eb960:
	.incbin "baserom.gba", 0x000f4954, 0x000002d0
	.section .rom.000f52ac, "ax"
	.incbin "baserom.gba", 0x000f52ac, 0x00000cbc
	.section .rom.000f6298, "ax"
	.incbin "baserom.gba", 0x000f6298, 0x000004e4
	.section .rom.000f8810, "ax"
	.incbin "baserom.gba", 0x000f8810, 0x00000008
	.section .rom.000fc978, "ax"
	.global Data_080f3984
Data_080f3984:
	.incbin "baserom.gba", 0x000fc978, 0x00000040
	.section .rom.000fe170, "ax"
	.global Func_080f8170
	.type Func_080f8170, %function
	.thumb_func
Func_080f8170:
	.incbin "baserom.gba", 0x000fe170, 0x00000494
	.section .rom.000ff3aa, "ax"
	.incbin "baserom.gba", 0x000ff3aa, 0x00000002
	.section .rom.000ff496, "ax"
	.incbin "baserom.gba", 0x000ff496, 0x00000002
	.section .rom.000ff498, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x000ff498, 0x00000e38
	.section .rom.00100478, "ax"
	.incbin "baserom.gba", 0x00100478, 0x0000003c
	.section .rom.00100d12, "ax"
	.incbin "baserom.gba", 0x00100d12, 0x00000002
	.section .rom.00101140, "ax"
	.incbin "baserom.gba", 0x00101140, 0x0000031c
	.section .rom.0010159e, "ax"
	.incbin "baserom.gba", 0x0010159e, 0x00000002
	.section .rom.0010232c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0010232c, 0x000001a4
	.section .rom.00102b08, "ax"
	.incbin "baserom.gba", 0x00102b08, 0x00000034
	.section .rom.00102b3c, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x00102b3c, 0x00000198
	.section .rom.00103658, "ax"
	.global Func_080fd55c
	.type Func_080fd55c, %function
	.thumb_func
Func_080fd55c:
	.incbin "baserom.gba", 0x00103658, 0x00000188
	.section .rom.00103820, "ax"
	.global Func_080fd6f0
	.type Func_080fd6f0, %function
	.thumb_func
Func_080fd6f0:
	.incbin "baserom.gba", 0x00103820, 0x0000011c
	.section .rom.00103c08, "ax"
	.global Func_080fdad4
	.type Func_080fdad4, %function
	.thumb_func
Func_080fdad4:
	.incbin "baserom.gba", 0x00103c08, 0x00000690
	.section .rom.001049c6, "ax"
	.incbin "baserom.gba", 0x001049c6, 0x00000002
	.section .rom.00105326, "ax"
	.incbin "baserom.gba", 0x00105326, 0x00000002
	.section .rom.00105a80, "ax"
	.incbin "baserom.gba", 0x00105a80, 0x00000210
	.section .rom.00105e08, "ax"
	.global Func_080ffcd4
	.type Func_080ffcd4, %function
	.thumb_func
Func_080ffcd4:
	.incbin "baserom.gba", 0x00105e08, 0x00000230
	.section .rom.0010688c, "ax"
	.balign 4
	.global Func_08100738
	.type Func_08100738, %function
	.thumb_func
Func_08100738:
	.incbin "baserom.gba", 0x0010688c, 0x000000c0
	.section .rom.00106e92, "ax"
	.incbin "baserom.gba", 0x00106e92, 0x00000002
	.section .rom.001071c0, "ax"
	.global Func_0810106c
	.type Func_0810106c, %function
	.thumb_func
Func_0810106c:
	.incbin "baserom.gba", 0x001071c0, 0x00000544
	.section .rom.0010778c, "ax"
	.incbin "baserom.gba", 0x0010778c, 0x00000228
	.section .rom.00107c1c, "ax"
	.global DjinnMenu_DrawPage
DjinnMenu_DrawPage:
	.incbin "baserom.gba", 0x00107c1c, 0x00000148
	.section .rom.00108160, "ax"
	.incbin "baserom.gba", 0x00108160, 0x00001050
	.section .rom.00109362, "ax"
	.incbin "baserom.gba", 0x00109362, 0x00000002
	.global Func_08103218
	.type Func_08103218, %function
	.thumb_func
Func_08103218:
	.incbin "baserom.gba", 0x00109364, 0x000007e4
	.section .rom.0010a68c, "ax"
	.incbin "baserom.gba", 0x0010a68c, 0x000003e8
	.section .rom.0010ab12, "ax"
	.incbin "baserom.gba", 0x0010ab12, 0x00000002
	.section .rom.0010b042, "ax"
	.incbin "baserom.gba", 0x0010b042, 0x00000002
	.section .rom.0010b12a, "ax"
	.incbin "baserom.gba", 0x0010b12a, 0x00000002
	.section .rom.0010b2f2, "ax"
	.incbin "baserom.gba", 0x0010b2f2, 0x00000002
	.section .rom.0010cb70, "ax"
	.global Func_08108b70
	.type Func_08108b70, %function
	.thumb_func
Func_08108b70:
	.incbin "baserom.gba", 0x0010cb70, 0x000004f8
	.section .rom.0010d186, "ax"
	.incbin "baserom.gba", 0x0010d186, 0x00000002
	.section .rom.0010d3a4, "ax"
	.incbin "baserom.gba", 0x0010d3a4, 0x0000020c
	.section .rom.0010dad8, "ax"
	.global Func_08109ad8
	.type Func_08109ad8, %function
	.thumb_func
Func_08109ad8:
	.incbin "baserom.gba", 0x0010dad8, 0x000001d4
	.section .rom.0010dcac, "ax"
	.global Func_08109cac
	.type Func_08109cac, %function
	.thumb_func
Func_08109cac:
	.incbin "baserom.gba", 0x0010dcac, 0x0000035c
	.section .rom.0010e494, "ax"
	.incbin "baserom.gba", 0x0010e494, 0x000001dc
	.section .rom.0010ebf4, "ax"
	.global Func_0810abf0
	.type Func_0810abf0, %function
	.thumb_func
Func_0810abf0:
	.incbin "baserom.gba", 0x0010ebf4, 0x000002b4
	.section .rom.0010f04e, "ax"
	.incbin "baserom.gba", 0x0010f04e, 0x00000002
	.section .rom.0010f7b8, "ax"
	.balign 4
	.global Func_0810b7b4
	.type Func_0810b7b4, %function
	.thumb_func
Func_0810b7b4:
	.incbin "baserom.gba", 0x0010f7b8, 0x000004fc
	.section .rom.0011843c, "ax"
	.balign 4
	.global Func_0811843c
	.type Func_0811843c, %function
	.thumb_func
Func_0811843c:
	.incbin "baserom.gba", 0x0011843c, 0x00000188
	.section .rom.00118958, "ax"
	.balign 4
	.global Func_08118958
	.type Func_08118958, %function
	.thumb_func
Func_08118958:
	.incbin "baserom.gba", 0x00118958, 0x00000184
	.section .rom.00119052, "ax"
	.incbin "baserom.gba", 0x00119052, 0x00000002
	.section .rom.0011b392, "ax"
	.incbin "baserom.gba", 0x0011b392, 0x00000002
	.section .rom.0011b772, "ax"
	.incbin "baserom.gba", 0x0011b772, 0x00000002
	.section .rom.0011c54e, "ax"
	.incbin "baserom.gba", 0x0011c54e, 0x00000002
	.section .rom.0011c682, "ax"
	.incbin "baserom.gba", 0x0011c682, 0x00000002
	.section .rom.0011e382, "ax"
	.incbin "baserom.gba", 0x0011e382, 0x00000002
	.section .rom.0011e3c2, "ax"
	.incbin "baserom.gba", 0x0011e3c2, 0x00000002
	.section .rom.0011e7f2, "ax"
	.incbin "baserom.gba", 0x0011e7f2, 0x00000002
	.section .rom.0011ff1e, "ax"
	.incbin "baserom.gba", 0x0011ff1e, 0x00000002
	.section .rom.0012046c, "ax"
	.global BattleTarget_Execute
	.type BattleTarget_Execute, %function
	.thumb_func
BattleTarget_Execute:
	.incbin "baserom.gba", 0x0012046c, 0x0000206c
	.section .rom.0012365e, "ax"
	.incbin "baserom.gba", 0x0012365e, 0x00000002
	.section .rom.00126716, "ax"
	.incbin "baserom.gba", 0x00126716, 0x00000002
	.section .rom.0012695a, "ax"
	.incbin "baserom.gba", 0x0012695a, 0x00000002
	.section .rom.0012695c, "ax"
	.global Func_08126944
	.type Func_08126944, %function
	.thumb_func
Func_08126944:
	.incbin "baserom.gba", 0x0012695c, 0x00000034
	.section .rom.001269c2, "ax"
	.incbin "baserom.gba", 0x001269c2, 0x00000002
	.section .rom.00126be4, "ax"
	.global Graphics_AdvancePaletteCycle
	.type Graphics_AdvancePaletteCycle, %function
	.thumb_func
Graphics_AdvancePaletteCycle:
	.incbin "baserom.gba", 0x00126be4, 0x00000130
	.section .rom.00127080, "ax"
	.global Func_08127068
	.type Func_08127068, %function
	.thumb_func
Func_08127068:
	.incbin "baserom.gba", 0x00127080, 0x00000248
	.section .rom.00127cec, "ax"
	.global Func_08127cd4
	.type Func_08127cd4, %function
	.thumb_func
Func_08127cd4:
	.incbin "baserom.gba", 0x00127cec, 0x000003cc
	.section .rom.0013c0b8, "ax"
	.global BattleEffect_RunDouse
	.type BattleEffect_RunDouse, %function
	.thumb_func
BattleEffect_RunDouse:
	.incbin "baserom.gba", 0x0013c0b8, 0x000004f4
	.section .rom.0013cb34, "ax"
	.balign 4
	.global Func_0813cb34
	.type Func_0813cb34, %function
	.thumb_func
Func_0813cb34:
	.incbin "baserom.gba", 0x0013cb34, 0x0000044c
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
	.section .rom.00145df8, "ax"
	.global BattleEffect_RunBind
	.type BattleEffect_RunBind, %function
	.thumb_func
BattleEffect_RunBind:
	.incbin "baserom.gba", 0x00145df8, 0x00000484
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
	.section .rom.0014a814, "ax"
	.global BattleEffect_RunTargetMotes
	.type BattleEffect_RunTargetMotes, %function
	.thumb_func
BattleEffect_RunTargetMotes:
	.incbin "baserom.gba", 0x0014a814, 0x000006a8
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
	.section .rom.001504d8, "ax"
	.global SummonEffect_RunCanvasEffect
SummonEffect_RunCanvasEffect:
	.incbin "baserom.gba", 0x001504d8, 0x000004dc
	.section .rom.00150c74, "ax"
	.global BattleEffect_RunQuake
	.type BattleEffect_RunQuake, %function
	.thumb_func
BattleEffect_RunQuake:
	.incbin "baserom.gba", 0x00150c74, 0x000003cc
	.section .rom.0015265a, "ax"
	.incbin "baserom.gba", 0x0015265a, 0x00000002
	.section .rom.0015265c, "ax"
	.global BattleEffect_RunProjectileVolleyB
	.type BattleEffect_RunProjectileVolleyB, %function
	.thumb_func
BattleEffect_RunProjectileVolleyB:
	.incbin "baserom.gba", 0x0015265c, 0x00001374
	.section .rom.0015585c, "ax"
	.global Func_0815585c
	.type Func_0815585c, %function
	.thumb_func
Func_0815585c:
	.incbin "baserom.gba", 0x0015585c, 0x00000734
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
	.section .rom.0015a110, "ax"
	.global BattleEffect_RunBurstScene
	.type BattleEffect_RunBurstScene, %function
	.thumb_func
BattleEffect_RunBurstScene:
	.incbin "baserom.gba", 0x0015a110, 0x000008d4
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
	.section .rom.0016aa08, "ax"
	.global BattleEffect_RunPlumeEdge
	.type BattleEffect_RunPlumeEdge, %function
	.thumb_func
BattleEffect_RunPlumeEdge:
	.incbin "baserom.gba", 0x0016aa08, 0x00000438
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
	.section .rom.0016ec58, "ax"
	.global BattleEffect_RunCool
	.type BattleEffect_RunCool, %function
	.thumb_func
BattleEffect_RunCool:
	.incbin "baserom.gba", 0x0016ec58, 0x00000428
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
	.thumb_func
Func_08171bb0:
	.incbin "baserom.gba", 0x00171bb0, 0x00000f38
	.section .rom.00172b00, "ax"
	.balign 4
	.global Func_08172b00
Func_08172b00:
	.incbin "baserom.gba", 0x00172b00, 0x00000a70
	.section .rom.00175f74, "ax"
	.global BattleEffect_RunCharon
	.type BattleEffect_RunCharon, %function
	.thumb_func
BattleEffect_RunCharon:
	.incbin "baserom.gba", 0x00175f74, 0x0000270c
	.section .rom.00179394, "ax"
	.global BattleEffect_RunRagingHeat
	.type BattleEffect_RunRagingHeat, %function
	.thumb_func
BattleEffect_RunRagingHeat:
	.incbin "baserom.gba", 0x00179394, 0x00000600
	.section .rom.00179f16, "ax"
	.incbin "baserom.gba", 0x00179f16, 0x00000002
	.global BattleEffect_RunFlora
	.type BattleEffect_RunFlora, %function
	.thumb_func
BattleEffect_RunFlora:
	.incbin "baserom.gba", 0x00179f18, 0x000016bc
	.section .rom.0017bd88, "ax"
	.global BattleEffect_RunFirePuppet
	.type BattleEffect_RunFirePuppet, %function
	.thumb_func
BattleEffect_RunFirePuppet:
	.incbin "baserom.gba", 0x0017bd88, 0x000004f4
	.section .rom.0017c6ac, "ax"
	.global BattleEffect_RunFume
	.type BattleEffect_RunFume, %function
	.thumb_func
BattleEffect_RunFume:
	.incbin "baserom.gba", 0x0017c6ac, 0x00000584
	.section .rom.0017d174, "ax"
	.global BattleEffect_RunStarMine
	.type BattleEffect_RunStarMine, %function
	.thumb_func
BattleEffect_RunStarMine:
	.incbin "baserom.gba", 0x0017d174, 0x00000550
	.section .rom.0017d724, "ax"
	.balign 4
	.global BattleEffect_RunRisingDragonEnemy
	.type BattleEffect_RunRisingDragonEnemy, %function
	.thumb_func
BattleEffect_RunRisingDragonEnemy:
	.incbin "baserom.gba", 0x0017d724, 0x00000954
	.section .rom.0017e0c0, "ax"
	.balign 4
	.global Func_0817e0c0
Func_0817e0c0:
	.incbin "baserom.gba", 0x0017e0c0, 0x000005a8
	.section .rom.0017ea58, "ax"
	.global BattleEffect_RunDaedalus
	.type BattleEffect_RunDaedalus, %function
	.thumb_func
BattleEffect_RunDaedalus:
	.incbin "baserom.gba", 0x0017ea58, 0x0000223c
	.global BattleEffect_RunDaedalusAlt
	.type BattleEffect_RunDaedalusAlt, %function
	.thumb_func
BattleEffect_RunDaedalusAlt:
	.incbin "baserom.gba", 0x00180c94, 0x00000540
	.global BattleEffect_RunVengeance
	.type BattleEffect_RunVengeance, %function
	.thumb_func
BattleEffect_RunVengeance:
	.incbin "baserom.gba", 0x001811d4, 0x0000073c
	.section .rom.00181ed4, "ax"
	.balign 4
	.global Func_08181ed4
Func_08181ed4:
	.incbin "baserom.gba", 0x00181ed4, 0x000004bc
	.section .rom.00182aa8, "ax"
	.balign 4
	.global BattleEffect_RunCoatlicue
	.type BattleEffect_RunCoatlicue, %function
	.thumb_func
BattleEffect_RunCoatlicue:
	.incbin "baserom.gba", 0x00182aa8, 0x00001954
	.global BattleEffect_RunCatastrophe
	.type BattleEffect_RunCatastrophe, %function
	.thumb_func
BattleEffect_RunCatastrophe:
	.incbin "baserom.gba", 0x001843fc, 0x00002a00
	.global BattleEffect_RunHaures
	.type BattleEffect_RunHaures, %function
	.thumb_func
BattleEffect_RunHaures:
	.incbin "baserom.gba", 0x00186dfc, 0x000017f4
	.section .rom.00188714, "ax"
	.global BattleEffect_RunDeathPlunge
	.type BattleEffect_RunDeathPlunge, %function
	.thumb_func
BattleEffect_RunDeathPlunge:
	.incbin "baserom.gba", 0x00188714, 0x0000059c
	.global BattleEffect_RunHelmSplitter
	.type BattleEffect_RunHelmSplitter, %function
	.thumb_func
BattleEffect_RunHelmSplitter:
	.incbin "baserom.gba", 0x00188cb0, 0x00000498
	.global BattleEffect_RunDragonCloud
	.type BattleEffect_RunDragonCloud, %function
	.thumb_func
BattleEffect_RunDragonCloud:
	.incbin "baserom.gba", 0x00189148, 0x000009cc
	.global BattleEffect_RunLegend
	.type BattleEffect_RunLegend, %function
	.thumb_func
BattleEffect_RunLegend:
	.incbin "baserom.gba", 0x00189b14, 0x000006dc
	.section .rom.0018ad40, "ax"
	.balign 4
	.global Func_0818ad40
Func_0818ad40:
	.incbin "baserom.gba", 0x0018ad40, 0x00000b28
	.global BattleEffect_RunTripleChomp
	.type BattleEffect_RunTripleChomp, %function
	.thumb_func
BattleEffect_RunTripleChomp:
	.incbin "baserom.gba", 0x0018b868, 0x000003f8
	.section .rom.0018bc98, "ax"
	.balign 4
	.global Func_0818bc98
Func_0818bc98:
	.incbin "baserom.gba", 0x0018bc98, 0x00000b08
	.section .rom.0018c7b8, "ax"
	.balign 4
	.global Func_0818c7b8
Func_0818c7b8:
	.incbin "baserom.gba", 0x0018c7b8, 0x000002f0
	.section .rom.0018cb74, "ax"
	.global BattleEffect_RunTrident
	.type BattleEffect_RunTrident, %function
	.thumb_func
BattleEffect_RunTrident:
	.incbin "baserom.gba", 0x0018cb74, 0x00000a1c
	.section .rom.0018de3c, "ax"
	.incbin "baserom.gba", 0x0018de3c, 0x00000120
	.global BattleEffect_RunMeteorBlow
	.type BattleEffect_RunMeteorBlow, %function
	.thumb_func
BattleEffect_RunMeteorBlow:
	.incbin "baserom.gba", 0x0018df5c, 0x00000e48
	.global BattleEffect_RunLiquifier
	.type BattleEffect_RunLiquifier, %function
	.thumb_func
BattleEffect_RunLiquifier:
	.incbin "baserom.gba", 0x0018eda4, 0x00000828
	.section .rom.0018f620, "ax"
	.balign 4
	.global Func_0818f620
	.type Func_0818f620, %function
	.thumb_func
Func_0818f620:
	.incbin "baserom.gba", 0x0018f620, 0x000008dc
	.global BattleEffect_RunMegiddo
	.type BattleEffect_RunMegiddo, %function
	.thumb_func
BattleEffect_RunMegiddo:
	.incbin "baserom.gba", 0x0018fefc, 0x00000c78
	.global BattleEffect_RunOdyssey
	.type BattleEffect_RunOdyssey, %function
	.thumb_func
BattleEffect_RunOdyssey:
	.incbin "baserom.gba", 0x00190b74, 0x000009bc
	.section .rom.00191958, "ax"
	.incbin "baserom.gba", 0x00191958, 0x00000188
	.section .rom.0019284c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0019284c, 0x00000450
	.global BattleEffect_RunIris
	.type BattleEffect_RunIris, %function
	.thumb_func
BattleEffect_RunIris:
	.incbin "baserom.gba", 0x00192c9c, 0x00002e00
	.section .rom.00196404, "ax"
	.global Func_08196404
	.type Func_08196404, %function
	.thumb_func
Func_08196404:
	.incbin "baserom.gba", 0x00196404, 0x00000508
	.section .rom.001a06e0, "ax"
	.global Func_081a06e0
	.type Func_081a06e0, %function
	.thumb_func
Func_081a06e0:
	.incbin "baserom.gba", 0x001a06e0, 0x000001e4
	.section .rom.001a0be8, "ax"
	.balign 4
	.global DisplayScroll_DrawMessageLine
	.type DisplayScroll_DrawMessageLine, %function
	.thumb_func
DisplayScroll_DrawMessageLine:
	.incbin "baserom.gba", 0x001a0be8, 0x000000c4
	.section .rom.001a150c, "ax"
	.global Func_081a150c
	.type Func_081a150c, %function
	.thumb_func
Func_081a150c:
	.incbin "baserom.gba", 0x001a150c, 0x000000d0
	.section .rom.001a6638, "ax"
	.global Func_081a6638
	.type Func_081a6638, %function
	.thumb_func
Func_081a6638:
	.incbin "baserom.gba", 0x001a6638, 0x000000ec
	.global Func_081a6724
	.type Func_081a6724, %function
	.thumb_func
Func_081a6724:
	.incbin "baserom.gba", 0x001a6724, 0x00000190
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
	.section .rom.001ac15c, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x001ac15c, 0x000011ec
	.section .rom.001b243c, "ax"
	.balign 4
	.global Func_081b243c
	.type Func_081b243c, %function
	.thumb_func
Func_081b243c:
	.incbin "baserom.gba", 0x001b243c, 0x00000e98
	.section .rom.001b8272, "ax"
	.incbin "baserom.gba", 0x001b8272, 0x00000002
	.global Func_081b8274
	.type Func_081b8274, %function
	.thumb_func
Func_081b8274:
	.incbin "baserom.gba", 0x001b8274, 0x000000d8
	.section .rom.001c00b0, "ax"
	.incbin "baserom.gba", 0x001c00b0, 0x00000064
	.section .rom.001c0114, "ax"
	.global LeftoverSoundDriver_StaleCallTarget
LeftoverSoundDriver_StaleCallTarget:
	.incbin "baserom.gba", 0x001c0114, 0x00000054
	.section .rom.001c0168, "ax"
	.global Func_081c0168
	.type Func_081c0168, %function
	.thumb_func
Func_081c0168:
	.incbin "baserom.gba", 0x001c0168, 0x00000030
	.section .rom.001c0428, "ax"
	.global Func_081c0428
Func_081c0428:
	.incbin "baserom.gba", 0x001c0428, 0x00000064
	.section .rom.001c0490, "ax"
	.balign 4
	.global Func_081c0490
	.type Func_081c0490, %function
	.thumb_func
Func_081c0490:
	.incbin "baserom.gba", 0x001c0490, 0x00000158
	.section .rom.001c075a, "ax"
	.global Func_081c075a
	.type Func_081c075a, %function
	.thumb_func
Func_081c075a:
	.incbin "baserom.gba", 0x001c075a, 0x00000018
	.section .rom.006a5a6e, "ax"
	.incbin "baserom.gba", 0x006a5a6e, 0x00000002
	.section .rom.00859579, "ax"
	.incbin "baserom.gba", 0x00859579, 0x00000003
	.section .rom.0085be62, "ax"
	.incbin "baserom.gba", 0x0085be62, 0x00000002
	.section .rom.0089070d, "ax"
	.incbin "baserom.gba", 0x0089070d, 0x00000003
	.section .rom.008abec9, "ax"
	.incbin "baserom.gba", 0x008abec9, 0x00000003
	.section .rom.0093c7dd, "ax"
	.incbin "baserom.gba", 0x0093c7dd, 0x00000003
	.section .rom.0094a306, "ax"
	.incbin "baserom.gba", 0x0094a306, 0x00000002
	.section .rom.0096a179, "ax"
	.incbin "baserom.gba", 0x0096a179, 0x00000003
	.section .rom.0096a34e, "ax"
	.incbin "baserom.gba", 0x0096a34e, 0x00000002
	.section .rom.0096a837, "ax"
	.incbin "baserom.gba", 0x0096a837, 0x00000001
	.section .rom.00a3f496, "ax"
	.incbin "baserom.gba", 0x00a3f496, 0x00000002
	.section .rom.00b41906, "ax"
	.incbin "baserom.gba", 0x00b41906, 0x00000002
	.section .rom.00bada06, "ax"
	.incbin "baserom.gba", 0x00bada06, 0x00000002
	.section .rom.00c2e765, "ax"
	.incbin "baserom.gba", 0x00c2e765, 0x00000003
	.section .rom.00d3ff07, "ax"
	.incbin "baserom.gba", 0x00d3ff07, 0x00000001
	.section .rom.00d58282, "ax"
	.incbin "baserom.gba", 0x00d58282, 0x00000002
	.section .rom.00e0e3e1, "ax"
	.incbin "baserom.gba", 0x00e0e3e1, 0x00000003
	.section .rom.00eba7a1, "ax"
	.incbin "baserom.gba", 0x00eba7a1, 0x00000003
	.section .rom.0003aab0, "ax"
	.global Text_DrawWideStringAtOffset
	.thumb_func
Text_DrawWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aab0, 0x0000017c
	.section .rom.000b76e4, "ax"
	.global Func_080ae6e4
	.thumb_func
Func_080ae6e4:
	.incbin "baserom.gba", 0x000b76e4, 0x0000014c
