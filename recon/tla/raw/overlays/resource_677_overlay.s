.syntax unified
	.thumb
	.section .text.x0200886c,"ax",%progbits
	.global Func_0200086c
	.thumb_func
Func_0200086c:
	.ifdef TLA_EDITION_JA
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3, #0]
	ldr r3, .L_ja_02008940
	subs r2, #41
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_ja_02008944
	cmp r2, r3
	bne .L_ja_020088de
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020088c2
	movs r1, #150
	movs r2, #180
	movs r0, #14
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020009f8
	movs r1, #158
	movs r2, #173
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020009f8
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_ja_020088ca
.L_ja_020088c2:
	movs r0, #14
	movs r1, #0
	bl Func_02000a60
.L_ja_020088ca:
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #11
	movs r1, #2
	bl ObjectMotion_SetActionVariant
.L_ja_020088de:
	ldr r3, .L_ja_02008940
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_ja_02008948
	cmp r2, r3
	bne .L_ja_0200893a
	movs r0, #0
	bl Func_02000a48
	movs r1, #2
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0, #0]
	movs r5, #4
	orrs r3, r5
	strb r3, [r0, #0]
	movs r1, #2
	movs r0, #10
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0, #0]
	movs r1, #2
	orrs r3, r5
	strb r3, [r0, #0]
	movs r0, #11
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0, #0]
	orrs r5, r3
	strb r5, [r0, #0]
.L_ja_0200893a:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_ja_02008940:
	.4byte gPartyState
.L_ja_02008944:
	.4byte 0x0000008d
.L_ja_02008948:
	.4byte 0x0000008e
	.else
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	ldr r3, .L_02008950
	subs r2, #41
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008954
	cmp r2, r3
	bne .L_020088f0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088d4
	movs r1, #150
	movs r2, #180
	movs r0, #14
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020009f8
	movs r1, #158
	movs r2, #173
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020009f8
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_020009f8
	b .L_020088dc
.L_020088d4:
	movs r0, #14
	movs r1, #0
	bl Func_02000a60
.L_020088dc:
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #11
	movs r1, #2
	bl ObjectMotion_SetActionVariant
.L_020088f0:
	ldr r3, .L_02008950
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008958
	cmp r2, r3
	bne .L_0200894c
	movs r0, #0
	bl Func_02000a48
	movs r1, #2
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #4
	orrs r3, r5
	strb r3, [r0]
	movs r1, #2
	movs r0, #10
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r1, #2
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
.L_0200894c:
	movs r0, #0
	pop {r5, pc}
.L_02008950:
	.4byte gPartyState
.L_02008954:
	.4byte 0x0000008d
.L_02008958:
	.4byte 0x0000008e
	.endif
