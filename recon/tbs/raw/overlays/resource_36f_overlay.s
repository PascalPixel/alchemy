.syntax unified
	.thumb
	.section .text.x02008454,"ax",%progbits
	.global Func_02000454
	.thumb_func
Func_02000454:
	.global Title_LoadBackground
	.thumb_func
Title_LoadBackground:
	push {r5, r6, lr}
	movs r0, #0
	ldr r5, [pc, #64]
	bl 0x02008588
	ldr r2, [pc, #64]
	ldr r3, [pc, #52]
	strh r3, [r2]
	ldr r2, [pc, #60]
	movs r3, #0
	strh r3, [r2, #10]
	adds r0, r5, #0
	bl 0x02008578
	movs r1, #160
	ldr r6, [pc, #52]
	adds r4, r0, #0
	ldr r3, [pc, #52]
	lsls r1, r1, #19
	ldr r2, [pc, #52]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #224
	lsls r3, r3, #1
	adds r4, r4, r3
	adds r0, r4, #0
	ldr r1, [pc, #40]
	bl 0x02008558
	ldr r3, [pc, #28]
	ldr r0, [pc, #32]
	ldr r1, [pc, #36]
	ldr r2, [pc, #36]
	b .L_02000454_0
	.4byte 0x00000681
	.4byte 0x0000001a
	.4byte 0x0400000c
	.4byte 0x03001ad0
	.4byte 0x000001ff
	.4byte 0x040000d4
	.4byte 0x84000070
	.4byte 0x02010000
	.4byte 0x06006800
	.4byte 0x84002580
.L_02000454_0:
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #208
	ldr r1, [pc, #88]
	lsls r3, r3, #1
	movs r4, #0
.L_02000454_2:
	movs r0, #0
.L_02000454_1:
	adds r2, r3, #0
	movs r5, #128
	lsls r3, r2, #16
	lsls r5, r5, #9
	adds r3, r3, r5
	adds r0, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r0, #29
	bls .L_02000454_1
	strh r6, [r1]
	adds r4, #1
	adds r1, #2
	strh r6, [r1]
	adds r1, #2
	cmp r4, #19
	bls .L_02000454_2
	ldr r3, [pc, #48]
	movs r4, #0
	movs r2, #0
.L_02000454_3:
	adds r4, #1
	strh r2, [r3, #2]
	strh r2, [r3]
	adds r3, #4
	cmp r4, #3
	bls .L_02000454_3
	ldr r3, [pc, #32]
	ldr r0, [pc, #28]
	ldr r1, [pc, #32]
	ldr r2, [pc, #36]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [pc, #32]
	ldr r2, [r3]
	movs r3, #160
	lsls r3, r3, #5
	strh r3, [r2, #20]
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x06003000
	.4byte 0x03001ad0
	.4byte 0x040000d4
	.4byte 0x04000010
	.4byte 0x84000004
	.4byte 0x03001e70
	.section .rodata,"a",%progbits
	.global gTitleEntrances
gTitleEntrances:
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x40000000
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTitleExits
gTitleExits:
	.4byte 0x000001ff
	.global gTitlePlacements
gTitlePlacements:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTitleEvents
gTitleEvents:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTitleVramBlock
gTitleVramBlock:
	.2byte 0xffff
	.section .bss,"aw",%nobits
	.space 58
	.global gTitleRevealFrame
gTitleRevealFrame:
	.space 20
	.global gTitleSprites
gTitleSprites:
	.space 216
