.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, r6, lr}
	ldr r2, [pc, #328]
	movs r1, #225
	lsls r1, r1, #1
	adds r5, r2, r1
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #10
	bne .L_02000054_0
	movs r1, #250
	lsls r1, r1, #1
	adds r3, r2, r1
	ldr r0, [r3]
	bl 0x020085b0
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #75
	bl 0x020085f0
	movs r0, #0
	bl 0x020082e8
	movs r0, #120
	bl 0x02008538
	ldr r2, [pc, #280]
	ldr r3, [r2]
	movs r5, #0
	cmp r3, #0
	bne .L_02000054_1
	adds r6, r2, #0
.L_02000054_2:
	movs r0, #1
	bl 0x02008538
	ldr r2, [pc, #264]
	adds r5, #1
	cmp r5, r2
	bgt .L_02000054_1
	ldr r3, [r6]
	cmp r3, #0
	beq .L_02000054_2
.L_02000054_1:
	ldr r0, [pc, #256]
	movs r1, #2
	bl 0x020085b8
	b .L_02000054_3
.L_02000054_0:
	cmp r3, #9
	bne .L_02000054_4
	movs r0, #67
	bl 0x020085f0
	movs r0, #0
	bl 0x020085d0
	movs r0, #17
	bl 0x020085f0
	movs r0, #60
	bl 0x02008588
	bl 0x02008590
	movs r0, #240
	bl 0x020085a8
	movs r0, #19
	bl 0x020085f0
	ldr r0, [pc, #204]
	movs r1, #2
	bl 0x020085b8
	b .L_02000054_3
.L_02000054_4:
	ldr r0, [pc, #200]
	bl 0x02008580
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #2
	bne .L_02000054_5
.L_02000054_7:
	movs r0, #19
	bl 0x020085f0
	movs r0, #0
	bl 0x020085e0
	movs r0, #0
	bl 0x020085e8
	bl 0x02008598
	cmp r0, #0
	ble .L_02000054_6
	movs r0, #70
	bl 0x020085f0
	movs r0, #1
	bl 0x020085d8
	cmp r0, #0
	bne .L_02000054_6
	movs r0, #17
	bl 0x020085f0
	movs r0, #30
	bl 0x02008588
	bl 0x02008590
	ldr r2, [pc, #132]
	ldr r3, [r2]
	movs r5, #0
	cmp r3, #0
	bne .L_02000054_7
	adds r6, r2, #0
.L_02000054_8:
	movs r0, #1
	adds r5, #1
	bl 0x02008538
	cmp r5, #119
	bgt .L_02000054_7
	ldr r3, [r6]
	cmp r3, #0
	beq .L_02000054_8
	b .L_02000054_7
.L_02000054_6:
	ldr r0, [pc, #92]
	movs r1, #1
	bl 0x020085b8
	b .L_02000054_9
.L_02000054_5:
	movs r0, #64
	bl 0x020085f0
	movs r0, #0
	bl 0x020085d8
	bl 0x020085a0
	ldr r0, [pc, #76]
	movs r1, #16
	bl 0x020085b8
	movs r0, #17
	bl 0x020085f0
.L_02000054_9:
	movs r0, #17
	bl 0x020085f0
	movs r0, #30
	bl 0x02008588
	bl 0x02008590
	movs r0, #60
	bl 0x020085a8
	movs r0, #19
	bl 0x020085f0
.L_02000054_3:
	movs r0, #0
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x03001c94
	.4byte 0x00000e0f
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x03001ae8
	.4byte 0x00000004
	.global Func_020001c0
	.thumb_func
Func_020001c0:
	.global Title_LoadSprites
	.thumb_func
Title_LoadSprites:
	push {r5, r6, lr}
	movs r0, #164
	lsls r0, r0, #3
	bl 0x02008548
	ldr r6, [pc, #88]
	movs r2, #0
	ldrsh r3, [r6, r2]
	movs r2, #1
	negs r2, r2
	adds r5, r0, #0
	cmp r3, r2
	bne .L_020001c0_0
	bl 0x02008568
	strh r0, [r6]
.L_020001c0_0:
	ldr r0, [pc, #68]
	bl 0x02008578
	adds r1, r5, #0
	bl 0x02008558
	ldr r3, [pc, #60]
	adds r0, r5, #0
	ldr r1, [pc, #60]
	ldr r2, [pc, #64]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r2, r5, #0
	movs r1, #160
	adds r2, #32
	lsls r1, r1, #3
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl 0x02008560
	movs r2, #128
	ldr r1, [pc, #32]
	lsls r2, r2, #24
.L_020001c0_1:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_020001c0_1
	adds r0, r5, #0
	bl 0x02008550
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02008650
	.4byte 0x0000001c
	.4byte 0x040000d4
	.4byte 0x050003e0
	.4byte 0x84000008
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
