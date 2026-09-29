.syntax unified
	.thumb
	.section .text.x0200813c,"ax",%progbits
	.align 2
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
	bl 0x020093d0
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
	bl 0x02009370
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
	bl 0x02009360
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x02009368
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
	bl 0x02009458
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
	bl 0x02009330
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
	bl 0x02009330
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x02009330
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
	bl 0x02009360
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x02009368
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
	.4byte 0x020095a4
	.4byte 0x02008105
	.4byte 0xffff0000
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
	.4byte 0x020094fc
	.4byte 0x02009534
	.4byte 0x0200956c
	.global ShianMura_ActionTable
ShianMura_ActionTable:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x000000b4
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000010
	.global ShianMura_Actor19Motion
ShianMura_Actor19Motion:
	.4byte 0x00000015
	.4byte 0x0000001d
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00030000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00018000
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00300000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00030000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000015
	.4byte 0x0000001d
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000010
	.global ShianMura_EffectScript
ShianMura_EffectScript:
	.4byte 0x00000000
	.4byte 0x00000012
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global ShianMura_EffectConfig
ShianMura_EffectConfig:
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global ShianMura_GateSteps1
ShianMura_GateSteps1:
	.4byte 0x00300040
	.4byte 0x00020001
	.4byte 0x003f0004
	.4byte 0x00010030
	.4byte 0x00040002
	.2byte 0xffff
	.global ShianMura_GateSteps2
ShianMura_GateSteps2:
	.2byte 0x0045
	.4byte 0x00020032
	.4byte 0x00040002
	.4byte 0x00320041
	.4byte 0x00020002
	.4byte 0xffff0004
	.global ShianMura_GateSteps3
ShianMura_GateSteps3:
	.4byte 0x002e0042
	.4byte 0x00020003
	.4byte 0x003f0004
	.4byte 0x0003002e
	.4byte 0x00040002
	.2byte 0xffff
	.global ShianMura_GateSteps4
ShianMura_GateSteps4:
	.2byte 0x0043
	.4byte 0x00040032
	.4byte 0x00040002
	.4byte 0x0032003f
	.4byte 0x00020004
	.4byte 0xffff0004
	.global ShianMura_GateSteps5
ShianMura_GateSteps5:
	.4byte 0x002c0042
	.4byte 0x00020003
	.4byte 0x003f0004
	.4byte 0x0003002c
	.4byte 0x00040002
	.4byte 0x0000ffff
	.global gShianMuraEntrances
gShianMuraEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000178
	.4byte 0xc00001f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000158
	.4byte 0x40000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000178
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x000001a8
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000198
	.4byte 0x400001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x000000d8
	.4byte 0x400000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000188
	.4byte 0x400000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x000000b0
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gShianMuraExits
gShianMuraExits:
	.4byte 0x00000048
	.4byte 0x00101049
	.4byte 0x00202049
	.4byte 0x00303049
	.4byte 0x00404049
	.4byte 0x00506049
	.4byte 0x00620009
	.4byte 0x0070203d
	.4byte 0x0080e002
	.4byte 0x000001ff
	.global gShianMuraPlacements
gShianMuraPlacements:
	.4byte 0xffff009c
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0xffff009f
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00002000
	.4byte 0xffff00a0
	.4byte 0x00000001
	.4byte 0x01a40000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00004000
	.4byte 0xffff00a4
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00003000
	.4byte 0xffff00a6
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0xffff00a8
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00013000
	.4byte 0xffff00a9
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00028000
	.4byte 0xffff00aa
	.4byte 0x00000001
	.4byte 0x01ac0000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x00008000
	.4byte 0xffff009f
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0000c000
	.4byte 0x189a0027
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01f00000
	.4byte 0x00033000
	.4byte 0xffff009f
	.4byte 0x020095b0
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00028000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00008000
	.4byte 0xffff00e3
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00008000
	.4byte 0x0046005b
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00008000
	.4byte 0xffff00df
	.4byte 0x00000001
	.4byte 0x01aa0000
	.4byte 0x00000000
	.4byte 0x01ac0000
	.4byte 0x00008000
	.4byte 0xffff00df
	.4byte 0x00000001
	.4byte 0x012b0000
	.4byte 0x00000000
	.4byte 0x00de0000
	.4byte 0x01008000
	.4byte 0xffff00df
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gShianMuraEvents
gShianMuraEvents:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008e4d
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008e4d
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008e4d
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008e4d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008e4d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008e4d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008e4d
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000202
	.4byte 0x02010014
	.4byte 0x02008d0d
	.4byte 0x00004602
	.4byte 0x02010015
	.4byte 0x02008d0d
	.4byte 0x00004602
	.4byte 0x02010002
	.4byte 0x02008d0d
	.4byte 0x00004602
	.4byte 0x02010003
	.4byte 0x02008d0d
	.4byte 0x00000000
	.4byte 0x08950008
	.4byte 0x000017e7
	.4byte 0x00000000
	.4byte 0x08950009
	.4byte 0x020085d5
	.4byte 0x00000000
	.4byte 0x0895000a
	.4byte 0x000017eb
	.4byte 0x00000000
	.4byte 0x0895000b
	.4byte 0x000017ef
	.4byte 0x00000000
	.4byte 0x0895000c
	.4byte 0x000017f0
	.4byte 0x00000000
	.4byte 0x0895000d
	.4byte 0x000017f1
	.4byte 0x00000000
	.4byte 0x0895000e
	.4byte 0x020089dd
	.4byte 0x00000000
	.4byte 0x0895000f
	.4byte 0x000017f5
	.4byte 0x00000000
	.4byte 0x08950010
	.4byte 0x000017f6
	.4byte 0x00000000
	.4byte 0x08950011
	.4byte 0x02008ced
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x020085f5
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001a24
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a25
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001a26
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001a27
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001a28
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001a29
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001a2a
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001a2b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001a2c
	.4byte 0x10001815
	.4byte 0x02010014
	.4byte 0x0200894d
	.4byte 0x00001815
	.4byte 0x02010014
	.4byte 0x02008969
	.4byte 0x00008e15
	.4byte 0xffff0015
	.4byte 0x02008f31
	.4byte 0x00008d15
	.4byte 0x08950008
	.4byte 0x000017ff
	.4byte 0x00008d15
	.4byte 0x08950009
	.4byte 0x00001800
	.4byte 0x00008d15
	.4byte 0x0895000a
	.4byte 0x00001801
	.4byte 0x00008d15
	.4byte 0x0895000b
	.4byte 0x00001803
	.4byte 0x00008d15
	.4byte 0x0895000c
	.4byte 0x00001804
	.4byte 0x00008d15
	.4byte 0x0895000d
	.4byte 0x00001805
	.4byte 0x00008d15
	.4byte 0x0895000e
	.4byte 0x00001806
	.4byte 0x00008d15
	.4byte 0x0895000f
	.4byte 0x00001807
	.4byte 0x00008d15
	.4byte 0x08950010
	.4byte 0x00001808
	.4byte 0x00008d15
	.4byte 0x08950011
	.4byte 0x00001809
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000180a
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a2d
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a2e
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a2f
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001a30
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a31
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a32
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001a33
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001a34
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001a35
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001a36
	.4byte 0x00000023
	.4byte 0x0f6e0065
	.4byte 0x001000e5
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008315
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x02009061
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
