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
	.section .rom.000139c8, "ax"
	.balign 4
	.global Func_0801399c
	.type Func_0801399c, %function
	.thumb_func
Func_0801399c:
	.incbin "baserom.gba", 0x000139c8, 0x00000194
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
	.section .rom.00030310, "ax"
	.incbin "baserom.gba", 0x00030310, 0x00000010
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
	.incbin "baserom.gba", 0x00039864, 0x0000062c
	.section .rom.00039f54, "ax"
	.global Func_08039f94
	.type Func_08039f94, %function
	.thumb_func
Func_08039f94:
	.incbin "baserom.gba", 0x00039f54, 0x000000c0
	.section .rom.0003a406, "ax"
	.incbin "baserom.gba", 0x0003a406, 0x00000002
	.section .rom.0003aa8a, "ax"
	.incbin "baserom.gba", 0x0003aa8a, 0x00000002
	.section .rom.0003ae60, "ax"
	.incbin "baserom.gba", 0x0003ae60, 0x0000016c
	.section .rom.0003afcc, "ax"
	.global Message_Decode
	.type Message_Decode, %function
	.thumb_func
Message_Decode:
	.incbin "baserom.gba", 0x0003afcc, 0x000007e0
	.section .rom.0003b930, "ax"
	.global Text_MeasureEntry
	.type Text_MeasureEntry, %function
	.thumb_func
Text_MeasureEntry:
	.incbin "baserom.gba", 0x0003b930, 0x00000234
	.global Func_0803bb58
	.type Func_0803bb58, %function
	.thumb_func
Func_0803bb58:
	.incbin "baserom.gba", 0x0003bb64, 0x0000027c
	.global Func_0803bde4
	.thumb_func
Func_0803bde4:
	.incbin "baserom.gba", 0x0003bde0, 0x00000284
	.section .rom.0003c16a, "ax"
	.incbin "baserom.gba", 0x0003c16a, 0x00000002
	.section .rom.0003c194, "ax"
	.balign 4
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c194, 0x000000dc
	.section .rom.0003c270, "ax"
	.global Func_0803c274
	.type Func_0803c274, %function
	.thumb_func
Func_0803c274:
	.incbin "baserom.gba", 0x0003c270, 0x00000104
	.section .rom.0003e81c, "ax"
	.balign 4
	.global Func_0803e7c8
	.type Func_0803e7c8, %function
	.thumb_func
Func_0803e7c8:
	.incbin "baserom.gba", 0x0003e81c, 0x00000124
	.section .rom.0003eb80, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003eb80, 0x000000b0
	.section .rom.0003ec30, "ax"
	.global Menu_StepRight
	.type Menu_StepRight, %function
	.thumb_func
Menu_StepRight:
	.incbin "baserom.gba", 0x0003ec30, 0x000001bc
	.section .rom.0003ef9a, "ax"
	.incbin "baserom.gba", 0x0003ef9a, 0x00000002
	.section .rom.0003f226, "ax"
	.incbin "baserom.gba", 0x0003f226, 0x00000002
	.section .rom.0003f41c, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0003f41c, 0x00000248
	.section .rom.0003fd7c, "ax"
	.global Menu_OpenWorkspaceOptions
	.type Menu_OpenWorkspaceOptions, %function
	.thumb_func
Menu_OpenWorkspaceOptions:
	.incbin "baserom.gba", 0x0003fd7c, 0x000003c0
	.section .rom.00040a9a, "ax"
	.incbin "baserom.gba", 0x00040a9a, 0x00000002
	.section .rom.0004171e, "ax"
	.incbin "baserom.gba", 0x0004171e, 0x00000002
	.section .rom.00042062, "ax"
	.incbin "baserom.gba", 0x00042062, 0x00000002
	.section .rom.000421da, "ax"
	.incbin "baserom.gba", 0x000421da, 0x00000002
	.section .rom.000422c8, "ax"
	.balign 4
	.global Text_DrawPrefixedNumberAtOffset
	.type Text_DrawPrefixedNumberAtOffset, %function
	.thumb_func
Text_DrawPrefixedNumberAtOffset:
	.incbin "baserom.gba", 0x000422c8, 0x000000a0
	.section .rom.000425ce, "ax"
	.incbin "baserom.gba", 0x000425ce, 0x00000002
	.section .rom.00042734, "ax"
	.global Func_080426e0
	.type Func_080426e0, %function
	.thumb_func
Func_080426e0:
	.incbin "baserom.gba", 0x00042734, 0x00000128
	.section .rom.00043d2c, "ax"
	.global Func_08043cd8
	.type Func_08043cd8, %function
	.thumb_func
Func_08043cd8:
	.incbin "baserom.gba", 0x00043d2c, 0x0000066c
	.section .rom.000445ac, "ax"
	.balign 4
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x000445ac, 0x0000051c
	.section .rom.00044e3c, "ax"
	.global Func_08044dc8
	.type Func_08044dc8, %function
	.thumb_func
Func_08044dc8:
	.incbin "baserom.gba", 0x00044e3c, 0x00000184
	.section .rom.0004532e, "ax"
	.incbin "baserom.gba", 0x0004532e, 0x00000002
	.section .rom.0004559a, "ax"
	.incbin "baserom.gba", 0x0004559a, 0x00000002
	.section .rom.000457f2, "ax"
	.incbin "baserom.gba", 0x000457f2, 0x00000002
	.section .rom.00045842, "ax"
	.incbin "baserom.gba", 0x00045842, 0x00000002
	.section .rom.00046486, "ax"
	.incbin "baserom.gba", 0x00046486, 0x00000002
	.section .rom.00046b7c, "ax"
	.incbin "baserom.gba", 0x00046b7c, 0x00002f60
	.section .rom.00049b2a, "ax"
	.incbin "baserom.gba", 0x00049b2a, 0x00000002
	.section .rom.0004a984, "ax"
	.incbin "baserom.gba", 0x0004a984, 0x00000f28
	.section .rom.0004bac2, "ax"
	.incbin "baserom.gba", 0x0004bac2, 0x00000206
	.section .rom.0004bdb2, "ax"
	.incbin "baserom.gba", 0x0004bdb2, 0x00000002
	.section .rom.0004bddc, "ax"
	.incbin "baserom.gba", 0x0004bddc, 0x00000038
	.section .rom.0004be14, "ax"
	.global Func_0804bc08
	.type Func_0804bc08, %function
	.thumb_func
Func_0804bc08:
	.incbin "baserom.gba", 0x0004be14, 0x000011a0
	.section .rom.0004d378, "ax"
	.global Menu_RunResourceSelectionLoop
	.type Menu_RunResourceSelectionLoop, %function
	.thumb_func
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x0004d378, 0x00000120
	.section .rom.0004d68e, "ax"
	.incbin "baserom.gba", 0x0004d68e, 0x00000002
	.section .rom.0004da08, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004da08, 0x0000016c
	.section .rom.0004dcc6, "ax"
	.incbin "baserom.gba", 0x0004dcc6, 0x00000002
	.section .rom.000ba3f6, "ax"
	.incbin "baserom.gba", 0x000ba3f6, 0x00000002
	.section .rom.000bb410, "ax"
	.balign 4
	.global Func_080ae410
	.type Func_080ae410, %function
	.thumb_func
Func_080ae410:
	.incbin "baserom.gba", 0x000bb410, 0x000001ec
	.section .rom.000bb868, "ax"
	.global Func_080ae868
	.type Func_080ae868, %function
	.thumb_func
Func_080ae868:
	.incbin "baserom.gba", 0x000bb868, 0x000001c8
	.section .rom.000bbc66, "ax"
	.incbin "baserom.gba", 0x000bbc66, 0x00000002
	.section .rom.000bbc68, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000bbc68, 0x0000003c
	.section .rom.000bc0e2, "ax"
	.incbin "baserom.gba", 0x000bc0e2, 0x00000002
	.section .rom.000bd144, "ax"
	.global Class_Determine
	.type Class_Determine, %function
	.thumb_func
Class_Determine:
	.incbin "baserom.gba", 0x000bd144, 0x00000144
	.section .rom.000d86c6, "ax"
	.incbin "baserom.gba", 0x000d86c6, 0x00000002
	.section .rom.000d891c, "ax"
	.global FieldSequence_Run
	.type FieldSequence_Run, %function
	.thumb_func
FieldSequence_Run:
	.incbin "baserom.gba", 0x000d891c, 0x00000c30
	.section .rom.000d97c4, "ax"
	.global Func_080cc7c4
Func_080cc7c4:
	.incbin "baserom.gba", 0x000d97c4, 0x000001d0
	.section .rom.000d9d76, "ax"
	.incbin "baserom.gba", 0x000d9d76, 0x00000002
	.section .rom.000d9d78, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000d9d78, 0x0000010c
	.section .rom.000d9ec8, "ax"
	.global Func_080ccec8
	.type Func_080ccec8, %function
	.thumb_func
Func_080ccec8:
	.incbin "baserom.gba", 0x000d9ec8, 0x000006bc
	.section .rom.000da808, "ax"
	.balign 4
	.global Func_080cd808
	.type Func_080cd808, %function
	.thumb_func
Func_080cd808:
	.incbin "baserom.gba", 0x000da808, 0x00000114
	.section .rom.000db31c, "ax"
	.global Func_080ce31c
	.type Func_080ce31c, %function
	.thumb_func
Func_080ce31c:
	.incbin "baserom.gba", 0x000db31c, 0x0000013c
	.section .rom.000db61c, "ax"
	.incbin "baserom.gba", 0x000db61c, 0x000004e0
	.section .rom.000dc554, "ax"
	.global Func_080cf554
	.type Func_080cf554, %function
	.thumb_func
Func_080cf554:
	.incbin "baserom.gba", 0x000dc554, 0x000001a8
	.section .rom.000dd1cc, "ax"
	.global ScreenTransition_Start
	.type ScreenTransition_Start, %function
	.thumb_func
ScreenTransition_Start:
	.incbin "baserom.gba", 0x000dd1cc, 0x00000354
	.section .rom.000dde1c, "ax"
	.global BattleEffect_BuildBuffer
	.type BattleEffect_BuildBuffer, %function
	.thumb_func
BattleEffect_BuildBuffer:
	.incbin "baserom.gba", 0x000dde1c, 0x00000868
	.section .rom.000de760, "ax"
	.balign 4
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000de760, 0x0000004c
	.section .rom.000de83e, "ax"
	.incbin "baserom.gba", 0x000de83e, 0x00000002
	.section .rom.000dea16, "ax"
	.incbin "baserom.gba", 0x000dea16, 0x00000002
	.section .rom.000def20, "ax"
	.global Func_080d1f20
	.type Func_080d1f20, %function
	.thumb_func
Func_080d1f20:
	.incbin "baserom.gba", 0x000def20, 0x00000164
	.section .rom.000e0376, "ax"
	.incbin "baserom.gba", 0x000e0376, 0x00000002
	.section .rom.000e045e, "ax"
	.incbin "baserom.gba", 0x000e045e, 0x00000002
	.section .rom.000e11e6, "ax"
	.incbin "baserom.gba", 0x000e11e6, 0x00000002
	.section .rom.000e1d08, "ax"
	.balign 4
	.global Func_080d4d08
	.type Func_080d4d08, %function
	.thumb_func
Func_080d4d08:
	.incbin "baserom.gba", 0x000e1d08, 0x000003f0
	.section .rom.000e23b6, "ax"
	.incbin "baserom.gba", 0x000e23b6, 0x00000002
	.section .rom.000e266e, "ax"
	.incbin "baserom.gba", 0x000e266e, 0x00000002
	.section .rom.000e27e2, "ax"
	.incbin "baserom.gba", 0x000e27e2, 0x00000002
	.section .rom.000e2a9e, "ax"
	.incbin "baserom.gba", 0x000e2a9e, 0x00000002
	.section .rom.000e3674, "ax"
	.global Func_080d6674
	.type Func_080d6674, %function
	.thumb_func
Func_080d6674:
	.incbin "baserom.gba", 0x000e3674, 0x00000174
	.section .rom.000e39f4, "ax"
	.global Func_080d69f4
	.type Func_080d69f4, %function
	.thumb_func
Func_080d69f4:
	.incbin "baserom.gba", 0x000e39f4, 0x0000019c
	.section .rom.000e4524, "ax"
	.global Func_080d7524
	.type Func_080d7524, %function
	.thumb_func
Func_080d7524:
	.incbin "baserom.gba", 0x000e4524, 0x00000264
	.section .rom.000e484c, "ax"
	.incbin "baserom.gba", 0x000e484c, 0x000000f0
	.section .rom.000e5a00, "ax"
	.global Func_080d8a00
	.type Func_080d8a00, %function
	.thumb_func
Func_080d8a00:
	.incbin "baserom.gba", 0x000e5a00, 0x00000210
	.section .rom.000e5c10, "ax"
	.global Func_080d8c10
	.type Func_080d8c10, %function
	.thumb_func
Func_080d8c10:
	.incbin "baserom.gba", 0x000e5c10, 0x00000130
	.section .rom.000e5e08, "ax"
	.global Func_080d8e08
	.type Func_080d8e08, %function
	.thumb_func
Func_080d8e08:
	.incbin "baserom.gba", 0x000e5e08, 0x000001a0
	.section .rom.000e5ff8, "ax"
	.global Func_080d8ff8
	.type Func_080d8ff8, %function
	.thumb_func
Func_080d8ff8:
	.incbin "baserom.gba", 0x000e5ff8, 0x0000010c
	.section .rom.000e6194, "ax"
	.global Func_080d9194
	.type Func_080d9194, %function
	.thumb_func
Func_080d9194:
	.incbin "baserom.gba", 0x000e6194, 0x00000110
	.section .rom.000e6790, "ax"
	.global Func_080d9790
	.type Func_080d9790, %function
	.thumb_func
Func_080d9790:
	.incbin "baserom.gba", 0x000e6790, 0x000001e4
	.section .rom.000e6974, "ax"
	.incbin "baserom.gba", 0x000e6974, 0x00000004
	.section .rom.000e6f1a, "ax"
	.incbin "baserom.gba", 0x000e6f1a, 0x00000002
	.section .rom.000e70b0, "ax"
	.global Func_080da0b0
	.type Func_080da0b0, %function
	.thumb_func
Func_080da0b0:
	.incbin "baserom.gba", 0x000e70b0, 0x00000108
	.section .rom.000e722a, "ax"
	.incbin "baserom.gba", 0x000e722a, 0x00000002
	.section .rom.000e722c, "ax"
	.global Func_080da22c
	.type Func_080da22c, %function
	.thumb_func
Func_080da22c:
	.incbin "baserom.gba", 0x000e722c, 0x00000108
	.section .rom.000e80b0, "ax"
	.global Func_080db0b0
	.type Func_080db0b0, %function
	.thumb_func
Func_080db0b0:
	.incbin "baserom.gba", 0x000e80b0, 0x00000394
	.section .rom.000e89a6, "ax"
	.incbin "baserom.gba", 0x000e89a6, 0x00000002
	.section .rom.000e8b78, "ax"
	.balign 4
	.global Func_080dbb78
Func_080dbb78:
	.incbin "baserom.gba", 0x000e8b78, 0x0000008c
	.section .rom.000e8ed0, "ax"
	.balign 4
	.global Func_080dbed0
	.type Func_080dbed0, %function
	.thumb_func
Func_080dbed0:
	.incbin "baserom.gba", 0x000e8ed0, 0x000000c4
	.section .rom.000e96d8, "ax"
	.global Func_080dc6d8
	.type Func_080dc6d8, %function
	.thumb_func
Func_080dc6d8:
	.incbin "baserom.gba", 0x000e96d8, 0x000000f4
	.section .rom.000e97e8, "ax"
	.balign 4
	.global BattleEffect_CleanupSceneObjects
	.type BattleEffect_CleanupSceneObjects, %function
	.thumb_func
BattleEffect_CleanupSceneObjects:
	.incbin "baserom.gba", 0x000e97e8, 0x0000016c
	.section .rom.000e9b44, "ax"
	.balign 4
	.global Func_080dcb44
	.type Func_080dcb44, %function
	.thumb_func
Func_080dcb44:
	.incbin "baserom.gba", 0x000e9b44, 0x00000284
	.section .rom.000ea054, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
	.incbin "baserom.gba", 0x000ea054, 0x000003e8
	.section .rom.000ea526, "ax"
	.incbin "baserom.gba", 0x000ea526, 0x00000002
	.section .rom.000eaa2e, "ax"
	.incbin "baserom.gba", 0x000eaa2e, 0x00000002
	.section .rom.000ec8de, "ax"
	.incbin "baserom.gba", 0x000ec8de, 0x00000002
	.section .rom.000ed3c2, "ax"
	.incbin "baserom.gba", 0x000ed3c2, 0x00000002
	.section .rom.000ed618, "ax"
	.global Func_080e0618
	.type Func_080e0618, %function
	.thumb_func
Func_080e0618:
	.incbin "baserom.gba", 0x000ed618, 0x00000360
	.section .rom.000ee448, "ax"
	.global BattleEffect_SpawnScaledArcObjects
	.type BattleEffect_SpawnScaledArcObjects, %function
	.thumb_func
BattleEffect_SpawnScaledArcObjects:
	.incbin "baserom.gba", 0x000ee448, 0x000000c4
	.section .rom.000ee50c, "ax"
	.global BattleEffect_UpdateArcEffect
	.type BattleEffect_UpdateArcEffect, %function
	.thumb_func
BattleEffect_UpdateArcEffect:
	.incbin "baserom.gba", 0x000ee50c, 0x000000f0
	.section .rom.000eef2c, "ax"
	.global Func_080e1f2c
	.type Func_080e1f2c, %function
	.thumb_func
Func_080e1f2c:
	.incbin "baserom.gba", 0x000eef2c, 0x000006bc
	.section .rom.000ef718, "ax"
	.incbin "baserom.gba", 0x000ef718, 0x0000005c
	.section .rom.000ef774, "ax"
	.global Func_080e2774
	.type Func_080e2774, %function
	.thumb_func
Func_080e2774:
	.incbin "baserom.gba", 0x000ef774, 0x00000160
	.section .rom.000efacc, "ax"
	.incbin "baserom.gba", 0x000efacc, 0x0000016c
	.section .rom.000efc38, "ax"
	.global Func_080e2c38
	.type Func_080e2c38, %function
	.thumb_func
Func_080e2c38:
	.incbin "baserom.gba", 0x000efc38, 0x00000108
	.section .rom.000efd64, "ax"
	.incbin "baserom.gba", 0x000efd64, 0x000002fc
	.section .rom.000f0074, "ax"
	.global Func_080e3074
	.type Func_080e3074, %function
	.thumb_func
Func_080e3074:
	.incbin "baserom.gba", 0x000f0074, 0x00000624
	.section .rom.000f08c8, "ax"
	.global Func_080e38c8
	.type Func_080e38c8, %function
	.thumb_func
Func_080e38c8:
	.incbin "baserom.gba", 0x000f08c8, 0x00000434
	.section .rom.000f146c, "ax"
	.incbin "baserom.gba", 0x000f146c, 0x000000a4
	.section .rom.000f1730, "ax"
	.balign 4
	.global Func_080e4730
	.type Func_080e4730, %function
	.thumb_func
Func_080e4730:
	.incbin "baserom.gba", 0x000f1730, 0x000002fc
	.section .rom.000f1a34, "ax"
	.global Func_080e4a34
	.type Func_080e4a34, %function
	.thumb_func
Func_080e4a34:
	.incbin "baserom.gba", 0x000f1a34, 0x00000cf0
	.section .rom.000f272c, "ax"
	.global Func_080e572c
	.type Func_080e572c, %function
	.thumb_func
Func_080e572c:
	.incbin "baserom.gba", 0x000f272c, 0x000005c4
	.section .rom.000f2d5c, "ax"
	.global Func_080e5d5c
	.type Func_080e5d5c, %function
	.thumb_func
Func_080e5d5c:
	.incbin "baserom.gba", 0x000f2d5c, 0x0000062c
	.section .rom.000f3400, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f3400, 0x000004c8
	.section .rom.000f38d0, "ax"
	.global Func_080e68d0
	.type Func_080e68d0, %function
	.thumb_func
Func_080e68d0:
	.incbin "baserom.gba", 0x000f38d0, 0x00000828
	.section .rom.000f4844, "ax"
	.global Func_080e7844
	.type Func_080e7844, %function
	.thumb_func
Func_080e7844:
	.incbin "baserom.gba", 0x000f4844, 0x00000750
	.section .rom.000f507c, "ax"
	.incbin "baserom.gba", 0x000f507c, 0x00000250
	.global Func_080e82cc
	.type Func_080e82cc, %function
	.thumb_func
Func_080e82cc:
	.incbin "baserom.gba", 0x000f52cc, 0x000004f8
	.section .rom.000f57c4, "ax"
	.global Func_080e87c4
	.type Func_080e87c4, %function
	.thumb_func
Func_080e87c4:
	.incbin "baserom.gba", 0x000f57c4, 0x00000220
	.section .rom.000f5c9c, "ax"
	.global Func_080e8c9c
	.type Func_080e8c9c, %function
	.thumb_func
Func_080e8c9c:
	.incbin "baserom.gba", 0x000f5c9c, 0x00000060
	.section .rom.000f6090, "ax"
	.global Func_080e9090
	.type Func_080e9090, %function
	.thumb_func
Func_080e9090:
	.incbin "baserom.gba", 0x000f6090, 0x000001c0
	.section .rom.000f6940, "ax"
	.global Func_080e9940
	.type Func_080e9940, %function
	.thumb_func
Func_080e9940:
	.incbin "baserom.gba", 0x000f6940, 0x000001ac
	.section .rom.000f6af4, "ax"
	.global Func_080e9af4
	.type Func_080e9af4, %function
	.thumb_func
Func_080e9af4:
	.incbin "baserom.gba", 0x000f6af4, 0x000002c0
	.section .rom.000f6f68, "ax"
	.incbin "baserom.gba", 0x000f6f68, 0x000001e4
	.section .rom.000f714c, "ax"
	.global Func_080ea14c
	.type Func_080ea14c, %function
	.thumb_func
Func_080ea14c:
	.incbin "baserom.gba", 0x000f714c, 0x00000674
	.section .rom.000f77c0, "ax"
	.global Func_080ea7c0
	.type Func_080ea7c0, %function
	.thumb_func
Func_080ea7c0:
	.incbin "baserom.gba", 0x000f77c0, 0x000000e8
	.section .rom.000f78d2, "ax"
	.incbin "baserom.gba", 0x000f78d2, 0x00000002
	.section .rom.000f78d4, "ax"
	.global Func_080ea8d4
	.type Func_080ea8d4, %function
	.thumb_func
Func_080ea8d4:
	.incbin "baserom.gba", 0x000f78d4, 0x00000140
	.section .rom.000f7a14, "ax"
	.global Func_080eaa14
	.type Func_080eaa14, %function
	.thumb_func
Func_080eaa14:
	.incbin "baserom.gba", 0x000f7a14, 0x0000015c
	.section .rom.000f7bd0, "ax"
	.global Func_080eabd0
	.type Func_080eabd0, %function
	.thumb_func
Func_080eabd0:
	.incbin "baserom.gba", 0x000f7bd0, 0x00000118
	.section .rom.000f7dfc, "ax"
	.global Func_080eadfc
	.type Func_080eadfc, %function
	.thumb_func
Func_080eadfc:
	.incbin "baserom.gba", 0x000f7dfc, 0x000000b8
	.section .rom.000f8296, "ax"
	.incbin "baserom.gba", 0x000f8296, 0x00000002
	.section .rom.000f8824, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f8824, 0x0000010c
	.section .rom.000f8960, "ax"
	.global Func_080eb960
	.type Func_080eb960, %function
	.thumb_func
Func_080eb960:
	.incbin "baserom.gba", 0x000f8960, 0x000002d0
	.section .rom.000f91d0, "ax"
	.global Func_080ec1d0
	.type Func_080ec1d0, %function
	.thumb_func
Func_080ec1d0:
	.incbin "baserom.gba", 0x000f91d0, 0x000000c8
	.section .rom.000f92b8, "ax"
	.incbin "baserom.gba", 0x000f92b8, 0x00000cbc
	.section .rom.000fa2a4, "ax"
	.incbin "baserom.gba", 0x000fa2a4, 0x000004e4
	.section .rom.000fc81c, "ax"
	.incbin "baserom.gba", 0x000fc81c, 0x00000008
	.section .rom.00100984, "ax"
	.global Data_080f3984
Data_080f3984:
	.incbin "baserom.gba", 0x00100984, 0x00000040
	.section .rom.00102170, "ax"
	.global Func_080f8170
	.type Func_080f8170, %function
	.thumb_func
Func_080f8170:
	.incbin "baserom.gba", 0x00102170, 0x000004a0
	.section .rom.001033b6, "ax"
	.incbin "baserom.gba", 0x001033b6, 0x00000002
	.section .rom.001034a2, "ax"
	.incbin "baserom.gba", 0x001034a2, 0x00000002
	.section .rom.001034a4, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x001034a4, 0x00000e38
	.section .rom.00104484, "ax"
	.incbin "baserom.gba", 0x00104484, 0x0000003c
	.section .rom.001048b8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001048b8, 0x00000370
	.section .rom.00104d1e, "ax"
	.incbin "baserom.gba", 0x00104d1e, 0x00000002
	.section .rom.0010514c, "ax"
	.incbin "baserom.gba", 0x0010514c, 0x0000031c
	.section .rom.001055aa, "ax"
	.incbin "baserom.gba", 0x001055aa, 0x00000002
	.section .rom.001063a4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001063a4, 0x000001a4
	.section .rom.00106b80, "ax"
	.incbin "baserom.gba", 0x00106b80, 0x00000034
	.section .rom.00106bb4, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x00106bb4, 0x00000198
	.section .rom.001076d0, "ax"
	.global Func_080fd55c
	.type Func_080fd55c, %function
	.thumb_func
Func_080fd55c:
	.incbin "baserom.gba", 0x001076d0, 0x00000188
	.section .rom.00107898, "ax"
	.global Func_080fd6f0
	.type Func_080fd6f0, %function
	.thumb_func
Func_080fd6f0:
	.incbin "baserom.gba", 0x00107898, 0x0000011c
	.section .rom.00107c80, "ax"
	.global Func_080fdad4
	.type Func_080fdad4, %function
	.thumb_func
Func_080fdad4:
	.incbin "baserom.gba", 0x00107c80, 0x00000690
	.section .rom.00108a3e, "ax"
	.incbin "baserom.gba", 0x00108a3e, 0x00000002
	.section .rom.0010939e, "ax"
	.incbin "baserom.gba", 0x0010939e, 0x00000002
	.section .rom.00109af8, "ax"
	.incbin "baserom.gba", 0x00109af8, 0x00000210
	.section .rom.00109e80, "ax"
	.global Func_080ffcd4
	.type Func_080ffcd4, %function
	.thumb_func
Func_080ffcd4:
	.incbin "baserom.gba", 0x00109e80, 0x00000230
	.section .rom.0010a904, "ax"
	.balign 4
	.global Func_08100738
	.type Func_08100738, %function
	.thumb_func
Func_08100738:
	.incbin "baserom.gba", 0x0010a904, 0x000000c0
	.section .rom.0010af0a, "ax"
	.incbin "baserom.gba", 0x0010af0a, 0x00000002
	.section .rom.0010b238, "ax"
	.global Func_0810106c
	.type Func_0810106c, %function
	.thumb_func
Func_0810106c:
	.incbin "baserom.gba", 0x0010b238, 0x00000544
	.section .rom.0010b804, "ax"
	.incbin "baserom.gba", 0x0010b804, 0x00000228
	.section .rom.0010d22e, "ax"
	.incbin "baserom.gba", 0x0010d22e, 0x00000002
	.section .rom.0010d3e2, "ax"
	.incbin "baserom.gba", 0x0010d3e2, 0x00000002
	.global Func_08103218
	.type Func_08103218, %function
	.thumb_func
Func_08103218:
	.incbin "baserom.gba", 0x0010d3e4, 0x000007e4
	.section .rom.0010e70c, "ax"
	.incbin "baserom.gba", 0x0010e70c, 0x000003e8
	.section .rom.0010eb92, "ax"
	.incbin "baserom.gba", 0x0010eb92, 0x00000002
	.section .rom.0010f0c2, "ax"
	.incbin "baserom.gba", 0x0010f0c2, 0x00000002
	.section .rom.0010f1aa, "ax"
	.incbin "baserom.gba", 0x0010f1aa, 0x00000002
	.section .rom.0010f372, "ax"
	.incbin "baserom.gba", 0x0010f372, 0x00000002
	.section .rom.00110b70, "ax"
	.global Func_08108b70
	.type Func_08108b70, %function
	.thumb_func
Func_08108b70:
	.incbin "baserom.gba", 0x00110b70, 0x000004f8
	.section .rom.00111186, "ax"
	.incbin "baserom.gba", 0x00111186, 0x00000002
	.section .rom.001113a4, "ax"
	.incbin "baserom.gba", 0x001113a4, 0x0000020c
	.section .rom.00111ad8, "ax"
	.global Func_08109ad8
	.type Func_08109ad8, %function
	.thumb_func
Func_08109ad8:
	.incbin "baserom.gba", 0x00111ad8, 0x000001d4
	.section .rom.00111cac, "ax"
	.global Func_08109cac
	.type Func_08109cac, %function
	.thumb_func
Func_08109cac:
	.incbin "baserom.gba", 0x00111cac, 0x0000035c
	.section .rom.00112494, "ax"
	.incbin "baserom.gba", 0x00112494, 0x000001dc
	.section .rom.00112bf4, "ax"
	.global Func_0810abf0
	.type Func_0810abf0, %function
	.thumb_func
Func_0810abf0:
	.incbin "baserom.gba", 0x00112bf4, 0x000002b4
	.section .rom.0011304e, "ax"
	.incbin "baserom.gba", 0x0011304e, 0x00000002
	.section .rom.001137b8, "ax"
	.balign 4
	.global Func_0810b7b4
	.type Func_0810b7b4, %function
	.thumb_func
Func_0810b7b4:
	.incbin "baserom.gba", 0x001137b8, 0x000004fc
	.section .rom.00113e08, "ax"
	.global Func_0810bdf4
	.type Func_0810bdf4, %function
	.thumb_func
Func_0810bdf4:
	.incbin "baserom.gba", 0x00113e08, 0x00000048
	.section .rom.0011843c, "ax"
	.balign 4
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
	.section .rom.00119808, "ax"
	.global Func_081197f0
	.type Func_081197f0, %function
	.thumb_func
Func_081197f0:
	.incbin "baserom.gba", 0x00119808, 0x00000848
	.section .rom.0011b392, "ax"
	.incbin "baserom.gba", 0x0011b392, 0x00000002
	.section .rom.0011b772, "ax"
	.incbin "baserom.gba", 0x0011b772, 0x00000002
	.section .rom.0011b774, "ax"
	.global Func_0811b75c
	.type Func_0811b75c, %function
	.thumb_func
Func_0811b75c:
	.incbin "baserom.gba", 0x0011b774, 0x00000278
	.section .rom.0011ba14, "ax"
	.incbin "baserom.gba", 0x0011ba14, 0x00000230
	.section .rom.0011c54e, "ax"
	.incbin "baserom.gba", 0x0011c54e, 0x00000002
	.section .rom.0011c682, "ax"
	.incbin "baserom.gba", 0x0011c682, 0x00000002
	.section .rom.0011d264, "ax"
	.incbin "baserom.gba", 0x0011d264, 0x000001c8
	.section .rom.0011d42c, "ax"
	.global Battle_AppendLinkedActions
	.type Battle_AppendLinkedActions, %function
	.thumb_func
Battle_AppendLinkedActions:
	.incbin "baserom.gba", 0x0011d42c, 0x0000018c
	.section .rom.0011d9e4, "ax"
	.incbin "baserom.gba", 0x0011d9e4, 0x0000040c
	.section .rom.0011e382, "ax"
	.incbin "baserom.gba", 0x0011e382, 0x00000002
	.section .rom.0011e3c2, "ax"
	.incbin "baserom.gba", 0x0011e3c2, 0x00000002
	.section .rom.0011e7f2, "ax"
	.incbin "baserom.gba", 0x0011e7f2, 0x00000002
	.section .rom.0011e848, "ax"
	.balign 4
	.global Func_0811e830
	.type Func_0811e830, %function
	.thumb_func
Func_0811e830:
	.incbin "baserom.gba", 0x0011e848, 0x000001dc
	.section .rom.0011ff1e, "ax"
	.incbin "baserom.gba", 0x0011ff1e, 0x00000002
	.section .rom.0012046c, "ax"
	.global BattleTarget_Execute
	.type BattleTarget_Execute, %function
	.thumb_func
BattleTarget_Execute:
	.incbin "baserom.gba", 0x0012046c, 0x0000206c
	.section .rom.001224f0, "ax"
	.incbin "baserom.gba", 0x001224f0, 0x0000003c
	.section .rom.0012252c, "ax"
	.global Func_08122514
	.type Func_08122514, %function
	.thumb_func
Func_08122514:
	.incbin "baserom.gba", 0x0012252c, 0x00000704
	.section .rom.00122ce8, "ax"
	.incbin "baserom.gba", 0x00122ce8, 0x00000040
	.section .rom.00122d28, "ax"
	.global BattleEvent_Playback
	.type BattleEvent_Playback, %function
	.thumb_func
BattleEvent_Playback:
	.incbin "baserom.gba", 0x00122d28, 0x00000794
	.section .rom.0012365e, "ax"
	.incbin "baserom.gba", 0x0012365e, 0x000011ca
	.section .rom.00125bd0, "ax"
	.global Func_08125bb8
	.type Func_08125bb8, %function
	.thumb_func
Func_08125bb8:
	.incbin "baserom.gba", 0x00125bd0, 0x00000054
	.section .rom.00125d8c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00125d8c, 0x000004a4
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
	.section .rom.00126e18, "ax"
	.global Func_08126e00
	.type Func_08126e00, %function
	.thumb_func
Func_08126e00:
	.incbin "baserom.gba", 0x00126e18, 0x00000238
	.section .rom.00127080, "ax"
	.global Func_08127068
	.type Func_08127068, %function
	.thumb_func
Func_08127068:
	.incbin "baserom.gba", 0x00127080, 0x00000248
	.section .rom.00127cec, "ax"
	.incbin "baserom.gba", 0x00127cec, 0x000003cc
	.section .rom.0013c0b8, "ax"
	.global BattleEffect_RunDouse
	.type BattleEffect_RunDouse, %function
	.thumb_func
BattleEffect_RunDouse:
	.incbin "baserom.gba", 0x0013c0b8, 0x000004f4
	.section .rom.0013c5f4, "ax"
	.balign 4
	.global Func_0813c5f4
	.type Func_0813c5f4, %function
	.thumb_func
Func_0813c5f4:
	.incbin "baserom.gba", 0x0013c5f4, 0x0000051c
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
	.global BattleEffect_RunProcne
	.type BattleEffect_RunProcne, %function
	.thumb_func
BattleEffect_RunProcne:
	.incbin "baserom.gba", 0x00146e00, 0x00000cd4
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
	.section .rom.001587e4, "ax"
	.global BattleEffect_RunGround
	.type BattleEffect_RunGround, %function
	.thumb_func
BattleEffect_RunGround:
	.incbin "baserom.gba", 0x001587e4, 0x000004fc
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
	.section .rom.00164c0a, "ax"
	.incbin "baserom.gba", 0x00164c0a, 0x000000ba
	.section .rom.00164cc4, "ax"
	.global BattleEffect_RunParticleStreams
BattleEffect_RunParticleStreams:
	.incbin "baserom.gba", 0x00164cc4, 0x00000df0
	.section .rom.0016729c, "ax"
	.global BattleEffect_RunJudgment
	.type BattleEffect_RunJudgment, %function
	.thumb_func
BattleEffect_RunJudgment:
	.incbin "baserom.gba", 0x0016729c, 0x000017a4
	.section .rom.001693d0, "ax"
	.global BattleEffect_RunNeptune
	.type BattleEffect_RunNeptune, %function
	.thumb_func
BattleEffect_RunNeptune:
	.incbin "baserom.gba", 0x001693d0, 0x00000e48
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
	.global BattleEffect_RunStoneJustice
	.type BattleEffect_RunStoneJustice, %function
	.thumb_func
BattleEffect_RunStoneJustice:
	.incbin "baserom.gba", 0x0016d3d8, 0x0000034c
	.global BattleEffect_RunSoulShatter
	.type BattleEffect_RunSoulShatter, %function
	.thumb_func
BattleEffect_RunSoulShatter:
	.incbin "baserom.gba", 0x0016d724, 0x0000052c
	.global BattleEffect_RunSerac
	.type BattleEffect_RunSerac, %function
	.thumb_func
BattleEffect_RunSerac:
	.incbin "baserom.gba", 0x0016dc50, 0x000009f4
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
	.section .rom.0016f928, "ax"
	.global BattleEffect_RunHurricane
	.type BattleEffect_RunHurricane, %function
	.thumb_func
BattleEffect_RunHurricane:
	.incbin "baserom.gba", 0x0016f928, 0x00000368
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
	.global BattleEffect_RunVoidBeam
	.type BattleEffect_RunVoidBeam, %function
	.thumb_func
BattleEffect_RunVoidBeam:
	.incbin "baserom.gba", 0x00170dac, 0x00000684
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
	.section .rom.00173aac, "ax"
	.balign 4
	.global Func_08173aac
Func_08173aac:
	.incbin "baserom.gba", 0x00173aac, 0x000024c8
	.global BattleEffect_RunCharon
	.type BattleEffect_RunCharon, %function
	.thumb_func
BattleEffect_RunCharon:
	.incbin "baserom.gba", 0x00175f74, 0x0000270c
	.section .rom.00178710, "ax"
	.balign 4
	.global BattleEffect_RunRisingDragon
	.type BattleEffect_RunRisingDragon, %function
	.thumb_func
BattleEffect_RunRisingDragon:
	.incbin "baserom.gba", 0x00178710, 0x000007d4
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
	.section .rom.0017b66c, "ax"
	.balign 4
	.global BattleEffect_RunSabreDance
	.type BattleEffect_RunSabreDance, %function
	.thumb_func
BattleEffect_RunSabreDance:
	.incbin "baserom.gba", 0x0017b66c, 0x00000304
	.section .rom.0017b9b8, "ax"
	.balign 4
	.global BattleEffect_RunBackstab
	.type BattleEffect_RunBackstab, %function
	.thumb_func
BattleEffect_RunBackstab:
	.incbin "baserom.gba", 0x0017b9b8, 0x000003d0
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
	.section .rom.0017e698, "ax"
	.balign 4
	.global Func_0817e698
Func_0817e698:
	.incbin "baserom.gba", 0x0017e698, 0x0000037c
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
	.incbin "baserom.gba", 0x00192c9c, 0x000035ac
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
	.section .rom.001a15dc, "ax"
	.global Func_081a15dc
	.type Func_081a15dc, %function
	.thumb_func
Func_081a15dc:
	.incbin "baserom.gba", 0x001a15dc, 0x000001f4
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
	.section .rom.001a729c, "ax"
	.balign 4
	.global Func_081a729c
	.type Func_081a729c, %function
	.thumb_func
Func_081a729c:
	.incbin "baserom.gba", 0x001a729c, 0x00000298
	.section .rom.001a7534, "ax"
	.global Title_NoOpPalette
	.type Title_NoOpPalette, %function
	.thumb_func
Title_NoOpPalette:
	.incbin "baserom.gba", 0x001a7534, 0x00000004
	.section .rom.001a7718, "ax"
	.global Func_081a76f8
	.type Func_081a76f8, %function
	.thumb_func
Func_081a76f8:
	.incbin "baserom.gba", 0x001a7718, 0x00000174
	.section .rom.001a788c, "ax"
	.global Data_081a786c
	.type Data_081a786c, %function
	.thumb_func
Data_081a786c:
	.incbin "baserom.gba", 0x001a788c, 0x00000004
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
	.incbin "baserom.gba", 0x001b243c, 0x00000e58
	.section .rom.001b8272, "ax"
	.incbin "baserom.gba", 0x001b8272, 0x000000da
	.section .rom.001b83c4, "ax"
	.global Func_081b83c4
	.type Func_081b83c4, %function
	.thumb_func
Func_081b83c4:
	.incbin "baserom.gba", 0x001b83c4, 0x00001f44
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
	.section .rom.00858d86, "ax"
	.incbin "baserom.gba", 0x00858d86, 0x00000002
	.section .rom.008591a9, "ax"
	.incbin "baserom.gba", 0x008591a9, 0x00000003
	.section .rom.00890481, "ax"
	.incbin "baserom.gba", 0x00890481, 0x00000003
	.section .rom.008abc3d, "ax"
	.incbin "baserom.gba", 0x008abc3d, 0x00000003
	.section .rom.0093c551, "ax"
	.incbin "baserom.gba", 0x0093c551, 0x00000003
	.section .rom.0094a07a, "ax"
	.incbin "baserom.gba", 0x0094a07a, 0x00000002
	.section .rom.00969e81, "ax"
	.incbin "baserom.gba", 0x00969e81, 0x00000003
	.section .rom.0096a056, "ax"
	.incbin "baserom.gba", 0x0096a056, 0x00000002
	.section .rom.0096a53f, "ax"
	.incbin "baserom.gba", 0x0096a53f, 0x00000001
	.section .rom.00a3f19e, "ax"
	.incbin "baserom.gba", 0x00a3f19e, 0x00000002
	.section .rom.00b4160e, "ax"
	.incbin "baserom.gba", 0x00b4160e, 0x00000002
	.section .rom.00bad70e, "ax"
	.incbin "baserom.gba", 0x00bad70e, 0x00000002
	.section .rom.00c2e46d, "ax"
	.incbin "baserom.gba", 0x00c2e46d, 0x00000003
	.section .rom.00d3fc0f, "ax"
	.incbin "baserom.gba", 0x00d3fc0f, 0x00000001
	.section .rom.00d57f8a, "ax"
	.incbin "baserom.gba", 0x00d57f8a, 0x00000002
	.section .rom.00e0e0e9, "ax"
	.incbin "baserom.gba", 0x00e0e0e9, 0x00000003
	.section .rom.00eba4a9, "ax"
	.incbin "baserom.gba", 0x00eba4a9, 0x00000003
