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
	.global Func_02000238
	.thumb_func
Func_02000238:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #148]
	movs r1, #0
	ldrsh r3, [r3, r1]
	ldr r2, [pc, #144]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r4, [pc, #144]
	ldrh r3, [r3, #2]
	movs r2, #136
	sub sp, #4
	adds r6, r4, #0
	lsrs r7, r3, #5
	movs r5, #0
	mov r8, r2
.L_02000238_3:
	movs r2, #18
	subs r2, r2, r5
	lsls r2, r2, #3
	movs r3, #232
	subs r3, r3, r2
	movs r2, #0
	stmia r6!, {r2}
	mov r1, r8
	lsls r3, r3, #16
	movs r2, #132
	orrs r3, r1
	lsls r2, r2, #8
	orrs r3, r2
	stmia r6!, {r3}
	movs r3, #240
	lsls r3, r3, #8
	orrs r3, r7
	stmia r6!, {r3}
	ldr r3, [pc, #96]
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	subs r1, r2, r5
	cmp r1, #0
	bge .L_02000238_0
	movs r1, #0
.L_02000238_0:
	cmp r1, #2
	bgt .L_02000238_1
	ldr r3, [pc, #72]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02000238_1
	movs r1, #0
.L_02000238_1:
	cmp r1, #0
	beq .L_02000238_2
	adds r0, r4, #0
	movs r1, #255
	adds r4, #12
	str r4, [sp, #0]
	bl 0x02008570
	ldr r4, [sp, #0]
.L_02000238_2:
	adds r5, #1
	adds r7, #2
	cmp r5, #17
	ble .L_02000238_3
	ldr r2, [pc, #28]
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	sub sp, #-4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02008650
	.4byte 0x03001b10
	.4byte 0x020086a0
	.4byte 0x0200868c
	.4byte 0x03001e40
	.global Func_020002e8
	.thumb_func
Func_020002e8:
	push {r5, r6, r7, lr}
	bl 0x02008454
	movs r0, #30
	bl 0x020085a8
	ldr r2, [pc, #40]
	ldr r3, [pc, #36]
	movs r0, #0
	strh r3, [r2]
	bl 0x020081c0
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #28]
	bl 0x02008540
	ldr r7, [pc, #28]
	ldr r5, [pc, #28]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r7]
	cmp r2, #31
	bgt .L_020002e8_0
	b .L_020002e8_1
	.4byte 0x00000000
	.4byte 0x0200868c
	.4byte 0x02008239
	.4byte 0x02002090
	.4byte 0x04000208
.L_020002e8_1:
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r7
	strh r2, [r7]
	movs r2, #170
	adds r3, #4
	lsls r2, r2, #5
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_020002e8_0:
	strh r1, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r7]
	cmp r2, #31
	bgt .L_020002e8_2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r7
	strh r2, [r7]
	ldr r2, [pc, #208]
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, [pc, #204]
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_020002e8_2:
	strh r1, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r7]
	cmp r2, #31
	bgt .L_020002e8_3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r7
	adds r3, #4
	strh r2, [r7]
	movs r2, #16
	stmia r3!, {r2}
	ldr r2, [pc, #168]
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_020002e8_3:
	strh r1, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r7]
	cmp r2, #31
	bgt .L_020002e8_4
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r7
	strh r2, [r7]
	ldr r2, [pc, #136]
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, [pc, #132]
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_020002e8_4:
	strh r1, [r5]
	movs r0, #120
	bl 0x020085a8
	movs r6, #0
.L_020002e8_6:
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r3, [r7]
	cmp r3, #31
	bgt .L_020002e8_5
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r3, #1
	adds r2, r7, r2
	strh r3, [r7]
	movs r3, #16
	adds r2, #4
	subs r3, r3, r6
	stmia r2!, {r3}
	ldr r3, [pc, #72]
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_020002e8_5:
	strh r1, [r5]
	movs r0, #3
	adds r6, #1
	bl 0x02008538
	cmp r6, #16
	ble .L_020002e8_6
	ldr r6, [pc, #60]
	movs r3, #224
	ldr r1, [r6]
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r5, #228
	movs r3, #0
	str r3, [r2]
	lsls r5, r5, #1
	movs r3, #1
	str r3, [r1, r5]
	bl 0x020085c0
	bl 0x020085c8
	ldr r2, [r6]
	movs r3, #60
	str r3, [r2, r5]
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002fce
	.4byte 0x04000050
	.4byte 0x04000054
	.4byte 0x00001010
	.4byte 0x04000052
	.4byte 0x03001ebc
	.global Func_02000454
	.thumb_func
Func_02000454:
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
	.2byte 0xffff
