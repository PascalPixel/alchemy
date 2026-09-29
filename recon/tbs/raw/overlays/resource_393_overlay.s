.syntax unified
	.thumb
	.section .text.x02008d5c,"ax",%progbits
	.global KorimaPalette_SaveFirst
	.thumb_func
KorimaPalette_SaveFirst:
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
	.4byte 0x020090e0
	.4byte 0x840000e0
	.global KorimaPalette_SaveSecond
	.thumb_func
KorimaPalette_SaveSecond:
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
	.4byte 0x020097e0
	.4byte 0x840000e0
	.global KorimaPalette_Capture
	.thumb_func
KorimaPalette_Capture:
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
	lsls r0, r0, #9
	movs r1, #0
	bl 0x02008ebc
	pop {r0}
	bx r0
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x84000070
	.4byte 0x05000200
	.global KorimaPalette_Restore
	.thumb_func
KorimaPalette_Restore:
	push {lr}
	ldr r3, [pc, #40]
	ldr r1, [r3]
	cmp r0, #0
	beq .L_02000ddc_0
	ldr r3, [pc, #36]
	ldr r0, [pc, #36]
	b .L_02000ddc_1
.L_02000ddc_0:
	ldr r3, [pc, #28]
	ldr r0, [pc, #36]
.L_02000ddc_1:
	ldr r2, [pc, #36]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl 0x02008eb4
	bl 0x02008d9c
	pop {r0}
	bx r0
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x020097e0
	.4byte 0x020090e0
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
	.global Data_02008fc8
Data_02008fc8:
	.4byte 0xffff0000
	.4byte 0x000000d1
	.4byte 0x40000117
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000140
	.4byte 0xc00001e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001e0
	.4byte 0x80000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02009028
Data_02009028:
	.4byte 0x0000002a
	.4byte 0x0010102b
	.4byte 0x00202029
	.4byte 0x000001ff
	.global Data_02009038
Data_02009038:
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02009098
Data_02009098:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x020089f9
	.4byte 0x00000602
	.4byte 0xffff000b
	.4byte 0x02008ba5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
