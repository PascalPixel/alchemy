.syntax unified
	.thumb
	.section .text.x02008340,"ax",%progbits
	.balign 4
	.balign 4
	.global Func_02000340
	.thumb_func
Func_02000340:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	ldr r5, .L_02008444
	adds r2, #255
	str r2, [r3]
	adds r2, #11
	adds r3, r5, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	movs r1, #240
	orrs r3, r2
	lsls r1, r1, #1
	strb r3, [r0]
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008448
	cmp r2, r3
	bne .L_020083de
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #5
	bne .L_02008390
	bl Func_02000454
	b .L_0200843e
.L_02008390:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083c4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #57
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083ba
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #65
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083c4
.L_020083ba:
	movs r0, #34
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083d0
.L_020083c4:
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_020005e4
	b .L_0200843e
.L_020083d0:
	movs r0, #18
	bl Actor_GetById
	movs r1, #4
	bl Actor_SetPartAttribute
	b .L_0200843e
.L_020083de:
	ldr r3, .L_0200844c
	cmp r2, r3
	bne .L_0200843e
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #1
	bne .L_0200840c
	movs r0, #8
	bl Actor_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r2, #4
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #8
	bl Actor_SetSpritePriority
	b .L_0200843e
.L_0200840c:
	cmp r3, #5
	bne .L_02008432
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #9
	bl Actor_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r2, #4
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #9
	bl Actor_SetSpritePriority
	b .L_0200843e
.L_02008432:
	cmp r3, #4
	bne .L_0200843e
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200843e:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_02008444:
	.4byte gAdventure
.L_02008448:
	.4byte 0x00000039
.L_0200844c:
	.4byte 0x0000003a
