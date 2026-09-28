.syntax unified
	.thumb
	.section .text.x02008b8c,"ax",%progbits
	.align 2
	.global KorimaPalette_SaveFirst
	.thumb_func
KorimaPalette_SaveFirst:
	.global Func_02000b8c
	.thumb_func
Func_02000b8c:
	ldr r2, [pc, #12]
	ldr r3, [pc, #16]
	ldr r0, [r2]
	ldr r1, [pc, #16]
	ldr r2, [pc, #16]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x020090b0
	.4byte 0x840000e0
	.global KorimaPalette_SaveSecond
	.thumb_func
KorimaPalette_SaveSecond:
	.global Func_02000bac
	.thumb_func
Func_02000bac:
	ldr r2, [pc, #12]
	ldr r3, [pc, #16]
	ldr r0, [r2]
	ldr r1, [pc, #16]
	ldr r2, [pc, #16]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x020097b0
	.4byte 0x840000e0
	.global KorimaPalette_Capture
	.thumb_func
KorimaPalette_Capture:
	.global Func_02000bcc
	.thumb_func
Func_02000bcc:
	push {lr}
	ldr r3, [pc, #44]
	ldr r4, [r3]
	movs r0, #160
	ldr r3, [pc, #40]
	lsls r0, r0, #19
	adds r1, r4, #0
	ldr r2, [pc, #40]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r4, r2
	ldr r0, [pc, #32]
	ldr r2, [pc, #24]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
.L_02000bf0:
	lsls r0, r0, #9
	movs r1, #0
	bl 0x02008e6c
	pop {r0}
	bx r0
	.2byte 0x1ed0
	.2byte 0x0300
	.2byte 0x00d4
	.2byte 0x0400
	.2byte 0x0070
	.2byte 0x8400
	.2byte 0x0200
	.2byte 0x0500
	.global Func_02000c0c
	.thumb_func
Func_02000c0c:
	push {lr}
	ldr r3, [pc, #40]
	ldr r1, [r3]
	cmp r0, #0
	beq .L_02000c0c_0
	ldr r3, [pc, #36]
	ldr r0, [pc, #36]
	b .L_02000c0c_1
.L_02000c0c_0:
	ldr r3, [pc, #28]
	ldr r0, [pc, #36]
.L_02000c0c_1:
	ldr r2, [pc, #36]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x02008e64
	bl 0x02008bcc
	pop {r0}
	bx r0
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x020097b0
	.4byte 0x020090b0
	.4byte 0x840000e0
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
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
	.global StagedActor_FootprintKinds
StagedActor_FootprintKinds:
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.global StagedActor_FootprintBounds
StagedActor_FootprintBounds:
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.global KorimaHiroba_Scripts
KorimaHiroba_Scripts:
	.4byte 0xffff0000
	.4byte 0x000000d1
	.4byte 0x40000117
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001e0
	.4byte 0x80000178
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000138
	.4byte 0x40000028
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KorimaHiroba_Messages
KorimaHiroba_Messages:
	.4byte 0x00000029
	.4byte 0x0010102a
	.4byte 0x00202028
	.4byte 0x000001ff
	.global KorimaHiroba_Actors
KorimaHiroba_Actors:
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0x0fd00016
	.4byte 0x00000007
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KorimaHiroba_Extras
KorimaHiroba_Extras:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x020089f9
	.4byte 0x00009415
	.4byte 0x0fd3000b
	.4byte 0x02008a2d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
