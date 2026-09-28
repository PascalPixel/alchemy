.syntax unified
	.thumb
	.section .text.x0200813c,"ax",%progbits
	.p2align 2
	.global Func_0200013c
	.thumb_func
Func_0200013c:
	.global Effect_Spawn
	.thumb_func
Effect_Spawn:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r1, #0
	ldr r1, [sp, #48]
	adds r5, r0, #0
	movs r0, #0
	mov r8, r2
	str r3, [sp, #4]
	mov r10, r1
	ldr r7, [sp, #52]
	bl 0x02009534
	movs r3, #128
	lsls r3, r3, #13
	mov r2, r10
	ands r3, r2
	mov r9, r0
	cmp r3, #0
	beq .L_0200013c_0
	cmp r7, #0
	beq .L_0200013c_0
	movs r3, #24
	ldrsh r0, [r7, r3]
	adds r2, r6, #0
	b .L_0200013c_1
.L_0200013c_0:
	adds r2, r6, #0
	movs r0, #222
.L_0200013c_1:
	adds r1, r5, #0
	mov r3, r8
	bl 0x020094b4
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200013c_2
	b .L_0200013c_3
.L_0200013c_2:
	ldr r1, [r6, #80]
	mov r8, r1
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	bl 0x020094a4
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x020094ac
	adds r3, r6, #0
	movs r0, #0
	adds r3, #85
	strb r0, [r3]
	mov r3, r8
	adds r3, #38
	strb r0, [r3]
	ldr r3, [pc, #328]
	str r3, [r6, #108]
	ldr r3, [sp, #4]
	str r3, [r6, #68]
	ldr r3, [sp, #40]
	str r3, [r6, #72]
	ldr r3, [sp, #44]
	mov r1, r9
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	mov r9, r3
	ands r3, r1
	orrs r3, r2
	adds r2, r6, #0
	mov r1, r8
	adds r2, #100
	strb r3, [r1, #9]
	adds r3, r2, #0
	str r0, [r6, #48]
	str r0, [r6, #52]
	str r2, [sp, #0]
	strh r0, [r3]
	ldr r3, [pc, #276]
	mov r1, r10
	ands r3, r1
	movs r5, #3
	cmp r3, #0
	beq .L_0200013c_3
	cmp r7, #0
	beq .L_0200013c_3
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_4
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl 0x02009584
.L_0200013c_4:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_5
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	mov r3, r8
	ldrb r2, [r7]
	ldrb r1, [r3, #9]
	ands r2, r5
	mov r3, r9
	ands r3, r1
	lsls r2, r2, #2
	orrs r3, r2
	mov r1, r8
	strb r3, [r1, #9]
.L_0200013c_5:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_0200013c_6
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200013c_6:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_7
	ldr r3, [pc, #156]
	mov r1, r11
	ldr r5, [r3, r1]
	cmp r2, #0
	beq .L_0200013c_8
	ldr r0, [r7, #16]
	ldr r3, [r6, #24]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	bl 0x02009484
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200013c_9
.L_0200013c_8:
	ldr r0, [r7, #16]
	ldr r2, [pc, #128]
	ldr r1, [r5, #12]
	adds r0, r0, r2
	bl 0x02009484
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x02009484
	str r0, [r6, #52]
.L_0200013c_7:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_10
	adds r0, r6, #0
	movs r1, #1
	bl 0x020094a4
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x020094ac
.L_0200013c_10:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_11
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #30]
.L_0200013c_11:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_12
	ldrh r3, [r7, #34]
	ldr r1, [sp, #0]
	strh r3, [r1]
.L_0200013c_12:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_3
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200013c_3:
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02009778
	.4byte 0x02008105
	.4byte 0xffff0000
	.section .text.x02008cc0,"ax",%progbits
	.global Func_02000cc0
	.thumb_func
Func_02000cc0:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000cc0_0
	ldr r0, [pc, #36]
	b .L_02000cc0_1
.L_02000cc0_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000cc0_2
	ldr r0, [pc, #36]
	b .L_02000cc0_1
.L_02000cc0_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000cc0_3
	ldr r0, [pc, #32]
	b .L_02000cc0_1
.L_02000cc0_3:
	ldr r0, [pc, #32]
.L_02000cc0_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000098
	.4byte 0x020097b4
	.4byte 0x0000009d
	.4byte 0x020097fc
	.4byte 0x0000009e
	.4byte 0x02009874
	.4byte 0x02009784
	.section .text.x02008d20,"ax",%progbits
	.p2align 2
	.global Func_02000d20
	.thumb_func
Func_02000d20:
	push {lr}
	ldr r3, [pc, #60]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000d20_0
	ldr r0, [pc, #48]
	bl 0x020094fc
	cmp r0, #0
	beq .L_02000d20_1
	ldr r0, [pc, #44]
	b .L_02000d20_2
.L_02000d20_1:
	ldr r0, [pc, #44]
	b .L_02000d20_2
.L_02000d20_0:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_02000d20_3
	ldr r0, [pc, #40]
	b .L_02000d20_2
.L_02000d20_3:
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000d20_4
	ldr r0, [pc, #40]
	b .L_02000d20_2
.L_02000d20_4:
	ldr r0, [pc, #40]
.L_02000d20_2:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000098
	.4byte 0x0000096f
	.4byte 0x020099d4
	.4byte 0x02009974
	.4byte 0x0000009d
	.4byte 0x02009a4c
	.4byte 0x0000009e
	.4byte 0x02009aac
	.4byte 0x0200995c
	.global Func_02000d88
	.thumb_func
Func_02000d88:
	push {lr}
	ldr r3, [pc, #60]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #52]
	cmp r2, r3
	bne 0x02008dae
	ldr r0, [pc, #48]
	bl 0x020094fc
	cmp r0, #0
	beq 0x02008daa
.L_02000da6:
	ldr r0, [pc, #44]
	b .L_02000da6_0
	.2byte 0x480b
	.2byte 0xe00a
	.2byte 0x4b0b
	.2byte 0x429a
	.2byte 0xd101
	.2byte 0x480a
	.2byte 0xe005
	.2byte 0x4b0a
	.2byte 0x429a
	.2byte 0xd101
	.2byte 0x480a
	.2byte 0xe000
	.2byte 0x480a
.L_02000da6_0:
	pop {r1}
	bx r1
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0098
	.2byte 0x0000
	.2byte 0x096f
	.2byte 0x0000
	.4byte 0x02009bcc
	.2byte 0x9b48
	.2byte 0x0200
	.2byte 0x009d
	.2byte 0x0000
	.2byte 0x9c80
	.2byte 0x0200
	.2byte 0x009e
	.2byte 0x0000
	.2byte 0x9ce0
	.2byte 0x0200
	.2byte 0x9b3c
	.2byte 0x0200
	.global Func_02000df0
	.thumb_func
Func_02000df0:
	push {lr}
	ldr r0, [pc, #316]
	bl 0x020094fc
	cmp r0, #0
	beq .L_02000df0_0
	b 0x02008f2c
.L_02000df0_0:
	movs r0, #154
	lsls r0, r0, #4
	bl 0x020094fc
	cmp r0, #0
	bne .L_02000df0_1
	b 0x02008f2c
.L_02000df0_1:
	bl 0x0200951c
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200953c
	movs r0, #0
	bl 0x02009534
	cmp r0, #0
	beq .L_02000df0_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #11
	bl 0x02009564
.L_02000df0_2:
	movs r1, #8
	negs r1, r1
	movs r2, #16
	movs r0, #11
	bl 0x02009554
	movs r0, #11
	bl 0x0200955c
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl 0x020095a4
	movs r0, #10
	bl 0x02009514
	movs r1, #11
	movs r2, #0
	movs r0, #0
	bl 0x02009574
	ldr r0, [pc, #208]
	bl 0x0200958c
	movs r1, #0
	movs r0, #11
	bl 0x02009594
	movs r0, #0
	movs r1, #0
	bl 0x0200952c
	cmp r0, #0
	bne .L_02000df0_3
	movs r0, #11
	movs r1, #0
	bl 0x0200959c
	movs r2, #232
	movs r1, #152
	movs r0, #11
	bl 0x0200954c
	movs r0, #154
	lsls r0, r0, #4
	bl 0x0200950c
	movs r0, #11
	bl 0x0200955c
	movs r0, #11
	movs r1, #1
	bl 0x0200956c
	ldr r1, [pc, #148]
	movs r0, #226
	ldr r3, [pc, #148]
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
	movs r3, #227
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #30
	strh r3, [r2]
	b 0x02008f28
.L_02000df0_3:
	ldr r3, [pc, #132]
	movs r0, #236
	ldr r2, [r3]
	lsls r0, r0, #1
	adds r2, r2, r0
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000eca:
	movs r0, #11
	movs r1, #0
	bl 0x0200959c
	movs r0, #11
	movs r1, #2
	bl 0x0200956c
	movs r0, #0
	bl 0x02009534
	cmp r0, #0
	beq .L_02000eca_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #11
	bl 0x02009544
.L_02000eca_0:
	movs r0, #11
	bl 0x0200955c
	movs r2, #0
	movs r1, #0
	movs r0, #11
	bl 0x02009564
	movs r0, #30
	bl 0x02009514
	movs r0, #0
	movs r1, #2
	bl 0x0200956c
	movs r1, #0
	movs r0, #0
	movs r2, #16
	bl 0x02009554
	movs r0, #0
	bl 0x0200955c
	movs r0, #0
	movs r1, #1
	bl 0x0200956c
	bl 0x02009524
	pop {r0}
	bx r0
	.2byte 0x098a
	.2byte 0x0000
	.2byte 0x23da
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0088
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.section .text.x02008fd0,"ax",%progbits
	.p2align 2
	.global Func_02000fd0
	.thumb_func
Func_02000fd0:
	push {r5, lr}
	ldr r5, [pc, #96]
	adds r0, r5, #0
	bl 0x0200958c
	movs r1, #0
	movs r0, #8
	bl 0x02009594
	movs r0, #0
	movs r1, #0
	bl 0x0200952c
	cmp r0, #0
	bne .L_02000fd0_0
	movs r0, #149
	lsls r0, r0, #4
	bl 0x020094fc
	cmp r0, #0
	beq .L_02000fd0_1
	ldr r0, [pc, #60]
	bl 0x020094fc
	cmp r0, #0
	bne .L_02000fd0_1
	adds r0, r5, #0
	adds r0, #8
	bl 0x0200958c
.L_02000fd0_1:
	movs r0, #8
	movs r1, #0
	bl 0x0200959c
	b .L_02000fd0_2
.L_02000fd0_0:
	ldr r3, [pc, #36]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r1, #0
	bl 0x0200959c
.L_02000fd0_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x000023cc
	.4byte 0x0000096f
	.4byte 0x03001ebc
	.section .text.x02009394,"ax",%progbits
	.p2align 2
	.global Func_02001394
	.thumb_func
Func_02001394:
	push {r5, lr}
	ldr r5, [pc, #216]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #208]
	sub sp, #8
	cmp r2, r3
	bne .L_02001394_0
	movs r0, #162
	lsls r0, r0, #1
	bl 0x02009504
	movs r0, #154
	lsls r0, r0, #4
	bl 0x020094fc
	cmp r0, #0
	beq .L_02001394_0
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x02009564
.L_02001394_0:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #164]
	cmp r2, r3
	bne 0x02009464
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #1
	bne .L_02001394_1
	movs r3, #107
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #108
	movs r1, #17
	movs r2, #1
	movs r3, #1
	bl 0x020094cc
.L_02001394_1:
	ldr r0, [pc, #128]
	bl 0x020094fc
	cmp r0, #0
	beq .L_02001394_2
	movs r1, #220
	movs r2, #154
	lsls r2, r2, #17
	movs r0, #8
	lsls r1, r1, #17
	bl 0x02009564
	movs r0, #8
	movs r1, #2
	bl 0x0200956c
	movs r3, #27
	movs r2, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #29
	movs r1, #19
	movs r2, #1
	movs r3, #1
	bl 0x020094cc
.L_02001394_2:
	ldr r0, [pc, #80]
	bl 0x020094fc
	cmp r0, #0
	beq 0x02009458
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x02009564
.L_02001442:
	movs r1, #174
	movs r2, #144
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x02009564
	movs r0, #10
	movs r1, #2
	bl 0x0200956c
	movs r0, #12
	bl 0x02009534
	movs r1, #0
	bl 0x020094dc
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0098
	.2byte 0x0000
	.2byte 0x009e
	.2byte 0x0000
	.2byte 0x09a2
	.2byte 0x0000
	.2byte 0x09a5
	.2byte 0x0000
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
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
	.4byte 0x02009618
	.4byte 0x02009650
	.4byte 0x02009688
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000098
	.4byte 0x40000078
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x000001e0
	.4byte 0xffff0002
	.4byte 0x00000098
	.4byte 0xc00001b8
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x000001e0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000002d8
	.4byte 0xc0000138
	.4byte 0x00600000
	.4byte 0x03000030
	.4byte 0x000001b0
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0xc0000198
	.4byte 0x00600000
	.4byte 0x03000030
	.4byte 0x000001b0
	.4byte 0xffff0003
	.4byte 0x000000a8
	.4byte 0x40000208
	.4byte 0x00700000
	.4byte 0x024001d0
	.4byte 0x000002c0
	.4byte 0xffff0004
	.4byte 0x000001f8
	.4byte 0xc0000298
	.4byte 0x00700000
	.4byte 0x024001d0
	.4byte 0x000002c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000318
	.4byte 0xc00001c8
	.4byte 0x00f00000
	.4byte 0x03600030
	.4byte 0x000001d0
	.4byte 0xffff0002
	.4byte 0x00000168
	.4byte 0x40000098
	.4byte 0x00f00000
	.4byte 0x03600030
	.4byte 0x000001d0
	.4byte 0xffff0003
	.4byte 0x00000158
	.4byte 0xc00001b8
	.4byte 0x00f00000
	.4byte 0x03600030
	.4byte 0x000001d0
	.4byte 0xffff0004
	.4byte 0x00000158
	.4byte 0x40000248
	.4byte 0x00300000
	.4byte 0x019801b0
	.4byte 0x00000380
	.4byte 0xffff0005
	.4byte 0x00000088
	.4byte 0x400001f8
	.4byte 0x00300000
	.4byte 0x019801b0
	.4byte 0x00000380
	.4byte 0xffff0006
	.4byte 0x000000d8
	.4byte 0xc0000378
	.4byte 0x00300000
	.4byte 0x019801b0
	.4byte 0x00000380
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000098
	.4byte 0x00119002
	.4byte 0x00236002
	.4byte 0x0000009d
	.4byte 0x0010209e
	.4byte 0x0020309d
	.4byte 0x0030209d
	.4byte 0x0040509e
	.4byte 0x0000009e
	.4byte 0x0011a002
	.4byte 0x0020109d
	.4byte 0x0030409e
	.4byte 0x0040309e
	.4byte 0x0050409d
	.4byte 0x00629002
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0096
	.4byte 0x00000002
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00003000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00013000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00015000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00003000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00013000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00015000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00015000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00004000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d4
	.4byte 0x00000001
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00d4
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x00015000
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008fd1
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000023cf
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000023d0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000023d1
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x02009041
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000023d3
	.4byte 0x00000013
	.4byte 0x0fa00064
	.4byte 0x001000c1
	.4byte 0x00000013
	.4byte 0x0fa10065
	.4byte 0x001000e5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008df1
	.4byte 0x00000002
	.4byte 0x09a0000b
	.4byte 0x02008f45
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000023d6
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000023d7
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000023d8
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008f45
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000023dd
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000023de
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000023df
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000023e0
	.4byte 0x00000013
	.4byte 0x0fa00064
	.4byte 0x001000c1
	.4byte 0x00000013
	.4byte 0x0fa10065
	.4byte 0x001000e5
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
	.4byte 0x00008f15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00000013
	.4byte 0x0fa00064
	.4byte 0x001000c1
	.4byte 0x00000013
	.4byte 0x0fa10065
	.4byte 0x001000e5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02009081
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
	.4byte 0x00008f15
	.4byte 0xffff000b
	.4byte 0x02009385
	.4byte 0x00008c15
	.4byte 0x09a20008
	.4byte 0x020090fd
	.4byte 0x00008c15
	.4byte 0x02050009
	.4byte 0x02009159
	.4byte 0x00008602
	.4byte 0xffff000b
	.4byte 0x020090f1
	.4byte 0x00000602
	.4byte 0xffff000a
	.4byte 0x020090f1
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte 0x02009149
	.4byte 0x00000013
	.4byte 0x0fa00064
	.4byte 0x001000c1
	.4byte 0x00000013
	.4byte 0x0fa10065
	.4byte 0x001000e5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
