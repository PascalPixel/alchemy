	.section .rom.000133f0, "ax"
	.balign 4
	.global System_InstallIwramAndIrqs
	.type System_InstallIwramAndIrqs, %function
	.thumb_func
System_InstallIwramAndIrqs:
	.incbin "baserom.gba", 0x000133f0, 0x00000074
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
	.section .rom.0002be4c, "ax"
	.balign 4
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
	.section .rom.0003939a, "ax"
	.incbin "baserom.gba", 0x0003939a, 0x00000002
	.section .rom.00039452, "ax"
	.incbin "baserom.gba", 0x00039452, 0x00000002
	.section .rom.000394e6, "ax"
	.incbin "baserom.gba", 0x000394e6, 0x00000002
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
	.incbin "baserom.gba", 0x0003aea0, 0x00000144
	.section .rom.0003afe4, "ax"
	.global Message_Decode
	.type Message_Decode, %function
	.thumb_func
Message_Decode:
	.incbin "baserom.gba", 0x0003afe4, 0x000007cc
	.section .rom.0003b934, "ax"
	.global Text_MeasureEntry
	.type Text_MeasureEntry, %function
	.thumb_func
Text_MeasureEntry:
	.incbin "baserom.gba", 0x0003b934, 0x00000234
	.global Func_0803bb58
	.type Func_0803bb58, %function
	.thumb_func
Func_0803bb58:
	.incbin "baserom.gba", 0x0003bb68, 0x0000027c
	.global Func_0803bde4
	.thumb_func
Func_0803bde4:
	.incbin "baserom.gba", 0x0003bde4, 0x00000284
	.section .rom.0003c16e, "ax"
	.incbin "baserom.gba", 0x0003c16e, 0x00000002
	.section .rom.0003c198, "ax"
	.balign 4
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c198, 0x000000dc
	.section .rom.0003ef9e, "ax"
	.incbin "baserom.gba", 0x0003ef9e, 0x00000002
	.section .rom.0003f22a, "ax"
	.incbin "baserom.gba", 0x0003f22a, 0x00000002
	.section .rom.0003fd80, "ax"
	.global Menu_OpenWorkspaceOptions
	.type Menu_OpenWorkspaceOptions, %function
	.thumb_func
Menu_OpenWorkspaceOptions:
	.incbin "baserom.gba", 0x0003fd80, 0x000003c0
	.section .rom.00040a9e, "ax"
	.incbin "baserom.gba", 0x00040a9e, 0x00000002
	.section .rom.00041722, "ax"
	.incbin "baserom.gba", 0x00041722, 0x00000002
	.section .rom.00041724, "ax"
	.global Text_DrawStringAsTiles
	.type Text_DrawStringAsTiles, %function
	.thumb_func
Text_DrawStringAsTiles:
	.incbin "baserom.gba", 0x00041724, 0x000003fc
	.section .rom.00042072, "ax"
	.incbin "baserom.gba", 0x00042072, 0x00000002
	.section .rom.000421ea, "ax"
	.incbin "baserom.gba", 0x000421ea, 0x00000002
	.section .rom.000422d8, "ax"
	.balign 4
	.global Text_DrawPrefixedNumberAtOffset
	.type Text_DrawPrefixedNumberAtOffset, %function
	.thumb_func
Text_DrawPrefixedNumberAtOffset:
	.incbin "baserom.gba", 0x000422d8, 0x000000a0
	.section .rom.000425de, "ax"
	.incbin "baserom.gba", 0x000425de, 0x00000002
	.section .rom.00042744, "ax"
	.global Func_080426e0
	.type Func_080426e0, %function
	.thumb_func
Func_080426e0:
	.incbin "baserom.gba", 0x00042744, 0x00000128
	.section .rom.00043d3c, "ax"
	.global Func_08043cd8
	.type Func_08043cd8, %function
	.thumb_func
Func_08043cd8:
	.incbin "baserom.gba", 0x00043d3c, 0x0000066c
	.section .rom.000445bc, "ax"
	.balign 4
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x000445bc, 0x000004fc
	.section .rom.0004531e, "ax"
	.incbin "baserom.gba", 0x0004531e, 0x00000002
	.section .rom.0004558a, "ax"
	.incbin "baserom.gba", 0x0004558a, 0x00000002
	.section .rom.000457e2, "ax"
	.incbin "baserom.gba", 0x000457e2, 0x00000002
	.section .rom.00045832, "ax"
	.incbin "baserom.gba", 0x00045832, 0x00000002
	.section .rom.00046476, "ax"
	.incbin "baserom.gba", 0x00046476, 0x00000002
	.section .rom.00046b6c, "ax"
	.global Func_08046b08
	.type Func_08046b08, %function
	.thumb_func
Func_08046b08:
	.incbin "baserom.gba", 0x00046b6c, 0x00001c24
	.global Func_0804868c
	.type Func_0804868c, %function
	.thumb_func
Func_0804868c:
	.incbin "baserom.gba", 0x00048790, 0x00000ad4
	.section .rom.00049b12, "ax"
	.incbin "baserom.gba", 0x00049b12, 0x00000002
	.section .rom.0004a96c, "ax"
	.global Func_0804a85c
	.type Func_0804a85c, %function
	.thumb_func
Func_0804a85c:
	.incbin "baserom.gba", 0x0004a96c, 0x00000f28
	.section .rom.0004baaa, "ax"
	.incbin "baserom.gba", 0x0004baaa, 0x00000002
	.section .rom.0004bd9a, "ax"
	.incbin "baserom.gba", 0x0004bd9a, 0x00000002
	.section .rom.0004d676, "ax"
	.incbin "baserom.gba", 0x0004d676, 0x00000002
	.section .rom.0004d9c8, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004d9c8, 0x0000016c
	.section .rom.0004dc86, "ax"
	.incbin "baserom.gba", 0x0004dc86, 0x00000002
	.section .rom.000ad3f6, "ax"
	.incbin "baserom.gba", 0x000ad3f6, 0x00000002
	.section .rom.000ae830, "ax"
	.incbin "baserom.gba", 0x000ae830, 0x0000000c
	.section .rom.000aec72, "ax"
	.incbin "baserom.gba", 0x000aec72, 0x00000002
	.section .rom.000af0ee, "ax"
	.incbin "baserom.gba", 0x000af0ee, 0x00000002
	.section .rom.000cb6c6, "ax"
	.incbin "baserom.gba", 0x000cb6c6, 0x00000002
	.section .rom.000cb91c, "ax"
	.global FieldSequence_Run
	.type FieldSequence_Run, %function
	.thumb_func
FieldSequence_Run:
	.incbin "baserom.gba", 0x000cb91c, 0x00000c30
	.section .rom.000ccd76, "ax"
	.incbin "baserom.gba", 0x000ccd76, 0x00000002
	.section .rom.000ce0aa, "ax"
	.incbin "baserom.gba", 0x000ce0aa, 0x00000002
	.section .rom.000ce31c, "ax"
	.global Func_080ce31c
	.type Func_080ce31c, %function
	.thumb_func
Func_080ce31c:
	.incbin "baserom.gba", 0x000ce31c, 0x0000013c
	.section .rom.000cef22, "ax"
	.incbin "baserom.gba", 0x000cef22, 0x00000002
	.section .rom.000d175e, "ax"
	.incbin "baserom.gba", 0x000d175e, 0x00000002
	.section .rom.000d183e, "ax"
	.incbin "baserom.gba", 0x000d183e, 0x00000002
	.section .rom.000d1a16, "ax"
	.incbin "baserom.gba", 0x000d1a16, 0x00000002
	.section .rom.000d2c90, "ax"
	.balign 4
	.global Func_080d2c9c
	.type Func_080d2c9c, %function
	.thumb_func
Func_080d2c9c:
	.incbin "baserom.gba", 0x000d2c90, 0x00000028
	.section .rom.000d2cb8, "ax"
	.global Func_080d2cc4
	.type Func_080d2cc4, %function
	.thumb_func
Func_080d2cc4:
	.incbin "baserom.gba", 0x000d2cb8, 0x00000044
	.section .rom.000d2cfc, "ax"
	.global Func_080d2d08
	.type Func_080d2d08, %function
	.thumb_func
Func_080d2d08:
	.incbin "baserom.gba", 0x000d2cfc, 0x00000068
	.section .rom.000d336a, "ax"
	.incbin "baserom.gba", 0x000d336a, 0x00000002
	.section .rom.000d3452, "ax"
	.incbin "baserom.gba", 0x000d3452, 0x00000002
	.section .rom.000d3932, "ax"
	.incbin "baserom.gba", 0x000d3932, 0x00000002
	.section .rom.000d41da, "ax"
	.incbin "baserom.gba", 0x000d41da, 0x00000002
	.section .rom.000d53aa, "ax"
	.incbin "baserom.gba", 0x000d53aa, 0x00000002
	.section .rom.000d5662, "ax"
	.incbin "baserom.gba", 0x000d5662, 0x00000002
	.section .rom.000d57d6, "ax"
	.incbin "baserom.gba", 0x000d57d6, 0x00000002
	.section .rom.000d5a92, "ax"
	.incbin "baserom.gba", 0x000d5a92, 0x00000002
	.section .rom.000d7518, "ax"
	.global Func_080d7524
	.type Func_080d7524, %function
	.thumb_func
Func_080d7524:
	.incbin "baserom.gba", 0x000d7518, 0x00000264
	.section .rom.000d7840, "ax"
	.incbin "baserom.gba", 0x000d7840, 0x000000f0
	.section .rom.000d9f0e, "ax"
	.incbin "baserom.gba", 0x000d9f0e, 0x00000002
	.section .rom.000da21e, "ax"
	.incbin "baserom.gba", 0x000da21e, 0x00000002
	.section .rom.000da95a, "ax"
	.incbin "baserom.gba", 0x000da95a, 0x00000002
	.section .rom.000db99a, "ax"
	.incbin "baserom.gba", 0x000db99a, 0x00000002
	.section .rom.000dbd4e, "ax"
	.incbin "baserom.gba", 0x000dbd4e, 0x00000002
	.section .rom.000dbd9c, "ax"
	.global Func_080dbda8
	.type Func_080dbda8, %function
	.thumb_func
Func_080dbda8:
	.incbin "baserom.gba", 0x000dbd9c, 0x00000020
	.section .rom.000dbdbc, "ax"
	.global Func_080dbdc8
	.type Func_080dbdc8, %function
	.thumb_func
Func_080dbdc8:
	.incbin "baserom.gba", 0x000dbdbc, 0x0000000c
	.section .rom.000dbdc8, "ax"
	.global Func_080dbdd4
	.type Func_080dbdd4, %function
	.thumb_func
Func_080dbdd4:
	.incbin "baserom.gba", 0x000dbdc8, 0x00000014
	.section .rom.000dbddc, "ax"
	.global Func_080dbde8
	.type Func_080dbde8, %function
	.thumb_func
Func_080dbde8:
	.incbin "baserom.gba", 0x000dbddc, 0x0000000c
	.section .rom.000dbde8, "ax"
	.global Func_080dbdf4
	.type Func_080dbdf4, %function
	.thumb_func
Func_080dbdf4:
	.incbin "baserom.gba", 0x000dbde8, 0x00000014
	.section .rom.000dbdfc, "ax"
	.global Func_080dbe08
	.type Func_080dbe08, %function
	.thumb_func
Func_080dbe08:
	.incbin "baserom.gba", 0x000dbdfc, 0x0000003c
	.section .rom.000dbe38, "ax"
	.global Func_080dbe44
	.type Func_080dbe44, %function
	.thumb_func
Func_080dbe44:
	.incbin "baserom.gba", 0x000dbe38, 0x0000003c
	.section .rom.000dbe74, "ax"
	.global Func_080dbe80
Func_080dbe80:
	.incbin "baserom.gba", 0x000dbe74, 0x00000050
	.section .rom.000dc7dc, "ax"
	.balign 4
	.global BattleEffect_CleanupSceneObjects
	.type BattleEffect_CleanupSceneObjects, %function
	.thumb_func
BattleEffect_CleanupSceneObjects:
	.incbin "baserom.gba", 0x000dc7dc, 0x0000016c
	.section .rom.000dd048, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
	.incbin "baserom.gba", 0x000dd048, 0x000003e8
	.section .rom.000dd51a, "ax"
	.incbin "baserom.gba", 0x000dd51a, 0x00000002
	.section .rom.000dda22, "ax"
	.incbin "baserom.gba", 0x000dda22, 0x00000002
	.section .rom.000df8d2, "ax"
	.incbin "baserom.gba", 0x000df8d2, 0x00000002
	.section .rom.000e03b6, "ax"
	.incbin "baserom.gba", 0x000e03b6, 0x00000002
	.section .rom.000e2258, "ax"
	.global Func_080e2264
	.type Func_080e2264, %function
	.thumb_func
Func_080e2264:
	.incbin "baserom.gba", 0x000e2258, 0x00000238
	.section .rom.000e2768, "ax"
	.global Func_080e2774
	.type Func_080e2774, %function
	.thumb_func
Func_080e2774:
	.incbin "baserom.gba", 0x000e2768, 0x00000160
	.section .rom.000e2c2c, "ax"
	.global Func_080e2c38
	.type Func_080e2c38, %function
	.thumb_func
Func_080e2c38:
	.incbin "baserom.gba", 0x000e2c2c, 0x00000108
	.section .rom.000e4724, "ax"
	.balign 4
	.global Func_080e4730
	.type Func_080e4730, %function
	.thumb_func
Func_080e4730:
	.incbin "baserom.gba", 0x000e4724, 0x000002fc
	.section .rom.000e5a58, "ax"
	.incbin "baserom.gba", 0x000e5a58, 0x0000028c
	.section .rom.000e5d50, "ax"
	.balign 4
	.global Func_080e5d5c
	.type Func_080e5d5c, %function
	.thumb_func
Func_080e5d5c:
	.incbin "baserom.gba", 0x000e5d50, 0x00000598
	.section .rom.000e63f4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e63f4, 0x000004c8
	.section .rom.000e68c4, "ax"
	.balign 4
	.global Func_080e68d0
	.type Func_080e68d0, %function
	.thumb_func
Func_080e68d0:
	.incbin "baserom.gba", 0x000e68c4, 0x00000828
	.section .rom.000e7838, "ax"
	.global Func_080e7844
	.type Func_080e7844, %function
	.thumb_func
Func_080e7844:
	.incbin "baserom.gba", 0x000e7838, 0x00000750
	.section .rom.000e87b8, "ax"
	.global Func_080e87c4
	.type Func_080e87c4, %function
	.thumb_func
Func_080e87c4:
	.incbin "baserom.gba", 0x000e87b8, 0x00000220
	.section .rom.000ea7b4, "ax"
	.global Func_080ea7c0
	.type Func_080ea7c0, %function
	.thumb_func
Func_080ea7c0:
	.global Func_080ea7c0
	.type Func_080ea7c0, %function
	.thumb_func
Func_080ea7c0:
	.incbin "baserom.gba", 0x000ea7b4, 0x000000e8
	.section .rom.000ea8c6, "ax"
	.incbin "baserom.gba", 0x000ea8c6, 0x00000002
	.section .rom.000ea8c8, "ax"
	.global Func_080ea8d4
	.type Func_080ea8d4, %function
	.thumb_func
Func_080ea8d4:
	.incbin "baserom.gba", 0x000ea8c8, 0x00000140
	.section .rom.000eabc4, "ax"
	.global Func_080eabd0
	.type Func_080eabd0, %function
	.thumb_func
Func_080eabd0:
	.incbin "baserom.gba", 0x000eabc4, 0x00000118
	.section .rom.000eadf0, "ax"
	.global Func_080eadfc
	.type Func_080eadfc, %function
	.thumb_func
Func_080eadfc:
	.incbin "baserom.gba", 0x000eadf0, 0x000000b8
	.section .rom.000eb28a, "ax"
	.incbin "baserom.gba", 0x000eb28a, 0x00000002
	.section .rom.000ec2ac, "ax"
	.incbin "baserom.gba", 0x000ec2ac, 0x00000b68
	.section .rom.000ed298, "ax"
	.incbin "baserom.gba", 0x000ed298, 0x000004e4
	.section .rom.000ef810, "ax"
	.incbin "baserom.gba", 0x000ef810, 0x00000008
	.section .rom.000f3978, "ax"
	.global Data_080f3984
Data_080f3984:
	.incbin "baserom.gba", 0x000f3978, 0x00000040
	.section .rom.000f93b6, "ax"
	.incbin "baserom.gba", 0x000f93b6, 0x00000002
	.section .rom.000f94a2, "ax"
	.incbin "baserom.gba", 0x000f94a2, 0x00000002
	.section .rom.000f94a4, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x000f94a4, 0x00000e38
	.section .rom.000fa484, "ax"
	.incbin "baserom.gba", 0x000fa484, 0x0000003c
	.section .rom.000fad1e, "ax"
	.incbin "baserom.gba", 0x000fad1e, 0x00000002
	.section .rom.000fb59a, "ax"
	.incbin "baserom.gba", 0x000fb59a, 0x00000002
	.section .rom.000fc328, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fc328, 0x000001a4
	.section .rom.000fcb04, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x000fcb04, 0x00000198
	.section .rom.000fd620, "ax"
	.global Func_080fd55c
	.type Func_080fd55c, %function
	.thumb_func
Func_080fd55c:
	.incbin "baserom.gba", 0x000fd620, 0x00000188
	.section .rom.000fe98e, "ax"
	.incbin "baserom.gba", 0x000fe98e, 0x00000002
	.section .rom.000ff2ee, "ax"
	.incbin "baserom.gba", 0x000ff2ee, 0x00000002
	.section .rom.000ffa48, "ax"
	.incbin "baserom.gba", 0x000ffa48, 0x00000210
	.section .rom.000ffdd0, "ax"
	.global Func_080ffcd4
	.type Func_080ffcd4, %function
	.thumb_func
Func_080ffcd4:
	.incbin "baserom.gba", 0x000ffdd0, 0x00000230
	.section .rom.00100854, "ax"
	.balign 4
	.global Func_08100738
	.type Func_08100738, %function
	.thumb_func
Func_08100738:
	.incbin "baserom.gba", 0x00100854, 0x000000c0
	.section .rom.00100e5a, "ax"
	.incbin "baserom.gba", 0x00100e5a, 0x00000002
	.section .rom.00101be4, "ax"
	.global DjinnMenu_DrawPage
DjinnMenu_DrawPage:
	.incbin "baserom.gba", 0x00101be4, 0x00000148
	.section .rom.00102128, "ax"
	.global Func_08102008
Func_08102008:
	.incbin "baserom.gba", 0x00102128, 0x00001050
	.section .rom.0010332a, "ax"
	.incbin "baserom.gba", 0x0010332a, 0x00000002
	.section .rom.00104ada, "ax"
	.incbin "baserom.gba", 0x00104ada, 0x00000002
	.section .rom.0010500a, "ax"
	.incbin "baserom.gba", 0x0010500a, 0x00000002
	.section .rom.001050f2, "ax"
	.incbin "baserom.gba", 0x001050f2, 0x00000002
	.section .rom.001052ba, "ax"
	.incbin "baserom.gba", 0x001052ba, 0x00000002
	.section .rom.00108b70, "ax"
	.global Func_08108b70
	.type Func_08108b70, %function
	.thumb_func
Func_08108b70:
	.incbin "baserom.gba", 0x00108b70, 0x000004f8
	.section .rom.00109186, "ax"
	.incbin "baserom.gba", 0x00109186, 0x00000002
	.section .rom.001093a4, "ax"
	.incbin "baserom.gba", 0x001093a4, 0x0000020c
	.section .rom.00109ad8, "ax"
	.global Func_08109ad8
	.type Func_08109ad8, %function
	.thumb_func
Func_08109ad8:
	.incbin "baserom.gba", 0x00109ad8, 0x000001d4
	.section .rom.0010a494, "ax"
	.incbin "baserom.gba", 0x0010a494, 0x000001dc
	.section .rom.0010abf4, "ax"
	.global Func_0810abf0
	.type Func_0810abf0, %function
	.thumb_func
Func_0810abf0:
	.incbin "baserom.gba", 0x0010abf4, 0x00000294
	.section .rom.0010b02e, "ax"
	.incbin "baserom.gba", 0x0010b02e, 0x00000002
	.section .rom.0010b9a4, "ax"
	.balign 4
	.global Func_0810b9c0
	.type Func_0810b9c0, %function
	.thumb_func
Func_0810b9c0:
	.incbin "baserom.gba", 0x0010b9a4, 0x000002f0
	.section .rom.0011843c, "ax"
	.balign 4
	.global Func_0811843c
	.type Func_0811843c, %function
	.thumb_func
Func_0811843c:
	.incbin "baserom.gba", 0x0011843c, 0x00000188
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
	.section .rom.0012365e, "ax"
	.incbin "baserom.gba", 0x0012365e, 0x00000002
	.section .rom.00126716, "ax"
	.incbin "baserom.gba", 0x00126716, 0x00000002
	.section .rom.0012695a, "ax"
	.incbin "baserom.gba", 0x0012695a, 0x00000002
	.section .rom.001269c2, "ax"
	.incbin "baserom.gba", 0x001269c2, 0x00000002
	.section .rom.00126be4, "ax"
	.global Graphics_AdvancePaletteCycle
	.type Graphics_AdvancePaletteCycle, %function
	.thumb_func
Graphics_AdvancePaletteCycle:
	.incbin "baserom.gba", 0x00126be4, 0x00000130
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
	.section .rom.0015f16c, "ax"
	.balign 4
	.global Func_0815f16c
	.type Func_0815f16c, %function
	.thumb_func
Func_0815f16c:
	.incbin "baserom.gba", 0x0015f16c, 0x00004764
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
	.section .rom.0016c6f8, "ax"
	.global BattleEffect_RunGeode
	.type BattleEffect_RunGeode, %function
	.thumb_func
BattleEffect_RunGeode:
	.incbin "baserom.gba", 0x0016c6f8, 0x00000a38
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
	.thumb_func
Func_08171bb0:
	.incbin "baserom.gba", 0x00171bb0, 0x00000f38
	.section .rom.00172b00, "ax"
	.balign 4
	.global Func_08172b00
Func_08172b00:
	.incbin "baserom.gba", 0x00172b00, 0x00000a70
	.section .rom.00179f16, "ax"
	.incbin "baserom.gba", 0x00179f16, 0x00000002
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
	.section .rom.0017d724, "ax"
	.balign 4
	.global BattleEffect_RunRisingDragonEnemy
	.type BattleEffect_RunRisingDragonEnemy, %function
	.thumb_func
BattleEffect_RunRisingDragonEnemy:
	.incbin "baserom.gba", 0x0017d724, 0x00000954
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
	.section .rom.001843fc, "ax"
	.global BattleEffect_RunCatastrophe
	.type BattleEffect_RunCatastrophe, %function
	.thumb_func
BattleEffect_RunCatastrophe:
	.incbin "baserom.gba", 0x001843fc, 0x00002a00
	.section .rom.00188714, "ax"
	.global BattleEffect_RunDeathPlunge
	.type BattleEffect_RunDeathPlunge, %function
	.thumb_func
BattleEffect_RunDeathPlunge:
	.incbin "baserom.gba", 0x00188714, 0x0000059c
	.section .rom.0018ad40, "ax"
	.balign 4
	.global Func_0818ad40
Func_0818ad40:
	.incbin "baserom.gba", 0x0018ad40, 0x00000b28
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
	.section .rom.0018df5c, "ax"
	.global BattleEffect_RunMeteorBlow
	.type BattleEffect_RunMeteorBlow, %function
	.thumb_func
BattleEffect_RunMeteorBlow:
	.incbin "baserom.gba", 0x0018df5c, 0x00000e48
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
	.section .rom.001b8272, "ax"
	.incbin "baserom.gba", 0x001b8272, 0x00000002
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
	.section .rom.0088fec5, "ax"
	.incbin "baserom.gba", 0x0088fec5, 0x00000003
	.section .rom.008ab681, "ax"
	.incbin "baserom.gba", 0x008ab681, 0x00000003
	.section .rom.0093bf95, "ax"
	.incbin "baserom.gba", 0x0093bf95, 0x00000003
	.section .rom.00949abe, "ax"
	.incbin "baserom.gba", 0x00949abe, 0x00000002
	.section .rom.00969741, "ax"
	.incbin "baserom.gba", 0x00969741, 0x00000003
	.section .rom.00969916, "ax"
	.incbin "baserom.gba", 0x00969916, 0x00000002
	.section .rom.00969dff, "ax"
	.incbin "baserom.gba", 0x00969dff, 0x00000001
	.section .rom.00a3ea5e, "ax"
	.incbin "baserom.gba", 0x00a3ea5e, 0x00000002
	.section .rom.00b40ece, "ax"
	.incbin "baserom.gba", 0x00b40ece, 0x00000002
	.section .rom.00bacfce, "ax"
	.incbin "baserom.gba", 0x00bacfce, 0x00000002
	.section .rom.00c2dd2d, "ax"
	.incbin "baserom.gba", 0x00c2dd2d, 0x00000003
	.section .rom.00d3f4cf, "ax"
	.incbin "baserom.gba", 0x00d3f4cf, 0x00000001
	.section .rom.00d5784a, "ax"
	.incbin "baserom.gba", 0x00d5784a, 0x00000002
	.section .rom.00e0d9a9, "ax"
	.incbin "baserom.gba", 0x00e0d9a9, 0x00000003
	.section .rom.00eb9d69, "ax"
	.incbin "baserom.gba", 0x00eb9d69, 0x00000003
	.section .rom.0003aab0, "ax"
	.global Text_DrawWideStringAtOffset
	.thumb_func
Text_DrawWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aab0, 0x0000017c
	.section .rom.000ae6e4, "ax"
	.global Func_080ae6e4
	.thumb_func
Func_080ae6e4:
	.incbin "baserom.gba", 0x000ae6e4, 0x0000014c
