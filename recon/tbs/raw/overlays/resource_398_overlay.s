.syntax unified
	.thumb
	.section .text.x02008040,"ax",%progbits
	.balign 4
	.global SceneData_SelectByRuntimeSelector
	.thumb_func
SceneData_SelectByRuntimeSelector:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000040_0
	ldr r0, [pc, #36]
	b .L_02000040_1
.L_02000040_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000040_2
	ldr r0, [pc, #36]
	b .L_02000040_1
.L_02000040_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000040_3
	ldr r0, [pc, #32]
	b .L_02000040_1
.L_02000040_3:
	ldr r0, [pc, #32]
.L_02000040_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000031
	.4byte 0x020089ec
	.4byte 0x00000030
	.4byte 0x02008a64
	.4byte 0x0000002f
	.4byte 0x02008b24
	.4byte 0x020089bc
	.section .text.x020080a0,"ax",%progbits
	.balign 4
	.global SceneData_SelectSecondaryDataByRuntimeSelector
	.thumb_func
SceneData_SelectSecondaryDataByRuntimeSelector:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020000a0_0
	ldr r0, [pc, #36]
	b .L_020000a0_1
.L_020000a0_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020000a0_2
	ldr r0, [pc, #36]
	b .L_020000a0_1
.L_020000a0_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020000a0_3
	ldr r0, [pc, #32]
	b .L_020000a0_1
.L_020000a0_3:
	ldr r0, [pc, #32]
.L_020000a0_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000031
	.4byte 0x02008c2c
	.4byte 0x00000030
	.4byte 0x02008c5c
	.4byte 0x0000002f
	.4byte 0x02008cbc
	.4byte 0x02008c14
	.global SceneData_SelectDataByRuntimeSelector
	.thumb_func
SceneData_SelectDataByRuntimeSelector:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020000f4_0
	ldr r0, [pc, #36]
	b .L_020000f4_1
.L_020000f4_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020000f4_2
	ldr r0, [pc, #36]
	b .L_020000f4_1
.L_020000f4_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020000f4_3
	ldr r0, [pc, #32]
	b .L_020000f4_1
.L_020000f4_3:
	ldr r0, [pc, #32]
.L_020000f4_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000031
	.4byte 0x02008ea8
	.4byte 0x00000030
	.4byte 0x02008efc
	.4byte 0x0000002f
	.4byte 0x02008f80
	.4byte 0x02008e9c
	.section .text.x0200846c,"ax",%progbits
	.balign 4
	.global FieldScene_DispatchByScenarioId
	.thumb_func
FieldScene_DispatchByScenarioId:
	push {lr}
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_0200046c_0
	bl 0x020084b4
	b .L_0200046c_1
.L_0200046c_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200046c_2
	bl 0x020084e8
	b .L_0200046c_1
.L_0200046c_2:
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_0200046c_1
	bl 0x02008538
.L_0200046c_1:
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000031
	.4byte 0x00000030
	.4byte 0x0000002f
	.section .rodata,"a",%progbits
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x00000048
	.4byte 0xc00001e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000174
	.4byte 0xc0000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000063
	.4byte 0xc00001bf
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000156
	.4byte 0x400000fe
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000001b8
	.4byte 0x40000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000360
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0xc00002f4
	.4byte 0x00280000
	.4byte 0x0276003c
	.4byte 0x00000348
	.4byte 0xffff0002
	.4byte 0x000001d8
	.4byte 0xc00002a8
	.4byte 0x00280000
	.4byte 0x0276003c
	.4byte 0x00000348
	.4byte 0xffff0003
	.4byte 0x00000188
	.4byte 0x400000c8
	.4byte 0x00280000
	.4byte 0x0276003c
	.4byte 0x00000348
	.4byte 0xffff0004
	.4byte 0x00000218
	.4byte 0x400001aa
	.4byte 0x00280000
	.4byte 0x0276003c
	.4byte 0x00000348
	.4byte 0xffff0005
	.4byte 0x00000358
	.4byte 0x4000007c
	.4byte 0x02f80000
	.4byte 0x039d0041
	.4byte 0x0000014a
	.4byte 0xffff0006
	.4byte 0x00000328
	.4byte 0xc00000f8
	.4byte 0x02f80000
	.4byte 0x039d0041
	.4byte 0x0000014a
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x4000012c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0xc00001d8
	.4byte 0x003c0000
	.4byte 0x0316003c
	.4byte 0x00000258
	.4byte 0xffff0002
	.4byte 0x00000138
	.4byte 0xc000020c
	.4byte 0x003c0000
	.4byte 0x0316003c
	.4byte 0x00000258
	.4byte 0xffff0003
	.4byte 0x0000021a
	.4byte 0xc0000224
	.4byte 0x003c0000
	.4byte 0x0316003c
	.4byte 0x00000258
	.4byte 0xffff0004
	.4byte 0x00000158
	.4byte 0x400000b0
	.4byte 0x003c0000
	.4byte 0x0316003c
	.4byte 0x00000258
	.4byte 0xffff0005
	.4byte 0x00000358
	.4byte 0xc0000278
	.4byte 0x02e40000
	.4byte 0x03d401fe
	.4byte 0x000002b2
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBiribinoDouExits
gBiribinoDouExits:
	.4byte 0x00000031
	.4byte 0x00123002
	.4byte 0x00201030
	.4byte 0x00302030
	.4byte 0x00000030
	.4byte 0x00102031
	.4byte 0x00203031
	.4byte 0x0030102f
	.4byte 0x00406030
	.4byte 0x0050202f
	.4byte 0x00604030
	.4byte 0x0000002f
	.4byte 0x00103030
	.4byte 0x00205030
	.4byte 0x00330002
	.4byte 0x0040502f
	.4byte 0x0050402f
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff00f7
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f7
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x01024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x020089b0
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00e60000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x020089b0
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x020089b0
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008149
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008169
	.4byte 0x00000c15
	.4byte 0x03050008
	.4byte 0x02008425
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000013
	.4byte 0x0f5d0064
	.4byte 0x00100016
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008189
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020081bd
	.4byte 0x00002115
	.4byte 0x08820009
	.4byte 0x020081f1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x0200844d
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x0200845d
	.4byte 0x00002115
	.4byte 0x08830008
	.4byte 0x02008215
	.4byte 0x00001815
	.4byte 0x0883000f
	.4byte 0x02008281
	.4byte 0x00001815
	.4byte 0xffff0010
	.4byte 0x020082ad
	.4byte 0x00001815
	.4byte 0x13020011
	.4byte 0x020082d9
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x02008305
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x02008305
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x020087f9
	.4byte 0x00008602
	.4byte 0xffff000b
	.4byte 0x020087f9
	.4byte 0x00000013
	.4byte 0x0f5e0064
	.4byte 0x001000b6
	.4byte 0x00000013
	.4byte 0x0fc60065
	.4byte 0x001000ba
	.4byte 0x00000013
	.4byte 0x0fc70066
	.4byte 0x001000bd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
