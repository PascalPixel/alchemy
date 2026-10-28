.syntax unified
	.thumb
	.section .text.x02008250,"ax",%progbits
	.balign 4
	.global Func_02000250
	.thumb_func
Func_02000250:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r5, .L_020083dc
	adds r2, #93
	str r2, [r3]
	movs r3, #241
	lsls r3, r3, #1
	adds r6, r5, r3
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #99
	bne .L_02008296
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #170
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_SetBit
	movs r3, #245
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #3
	strh r3, [r2]
	strh r3, [r6]
	bl Func_02000c5c
.L_02008296:
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082c6
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #9
	b .L_02008302
.L_020082c6:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200830c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008320
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #11
.L_02008302:
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	b .L_02008320
.L_0200830c:
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02003198
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02003198
.L_02008320:
	movs r0, #144
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200833a
	movs r1, #196
	movs r2, #162
	movs r0, #13
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003198
.L_0200833a:
	movs r0, #13
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r1, #202
	strb r3, [r0]
	adds r1, #255
	movs r0, #15
	bl Func_02003298
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083a2
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083d6
	movs r0, #14
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #128
	orrs r3, r2
	strb r3, [r0]
	movs r0, #15
	bl Actor_GetById
	adds r1, r0, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #64
	orrs r3, r2
	strb r3, [r1]
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r3, [r0, #20]
	adds r3, r3, r2
	str r3, [r0, #20]
	b .L_020083d6
.L_020083a2:
	movs r1, #136
	movs r0, #14
	lsls r1, r1, #16
	ldr r2, .L_020083e0
	bl Func_02003198
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083cc
	movs r1, #240
	movs r2, #236
	movs r0, #15
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_02003198
	b .L_020083d6
.L_020083cc:
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02003198
.L_020083d6:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020083dc:
	.4byte gAdventure
.L_020083e0:
	.4byte 0x01db0000
