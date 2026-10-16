.syntax unified
	.thumb
	.section .text.x0200806c,"ax",%progbits
	.balign 4
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_02002b28
	movs r0, #0
	bl Func_02002c58
	movs r5, #8
.L_02008080:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008092
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008092:
	adds r5, #1
	cmp r5, #63
	bls .L_02008080
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r5, [r6, r3]
	movs r0, #158
	bl Func_02002c90
	subs r5, #1
	ldr r0, .L_02008100
	lsls r5, r5, #3
	adds r3, r5, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r5]
	bl Func_02002ae8
	ldr r5, .L_02008104
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	ldr r0, [r5]
	bl Object_SetModeById
	movs r2, #8
	movs r1, #2
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_CenterOnColumn
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_02002c30
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02002b30
	pop {r5, r6, pc}
.L_02008100:
	.4byte gData_02003308
.L_02008104:
	.4byte gPartyState
	.section .text.x020086ac,"ax",%progbits
	.balign 4
	.global Func_020006ac
	.thumb_func
Func_020006ac:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	movs r0, #128
	adds r3, r3, r2
	lsls r0, r0, #4
	subs r2, #172
	str r2, [r3]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086d2
	bl Func_0200068c
	b .L_020087a0
.L_020086d2:
	movs r0, #26
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #26
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #26
	bl Object_GetById
	ldrh r0, [r0, #32]
	movs r1, #3
	lsls r0, r0, #2
	bl __divsi3
	strh r0, [r6, #32]
	movs r0, #26
	bl Object_GetById
	ldr r1, [r0, #80]
	movs r3, #8
	ldrb r2, [r1, #26]
	movs r0, #26
	orrs r3, r2
	strb r3, [r1, #26]
	bl Object_GetById
	ldr r2, [r0, #80]
	movs r5, #3
	ldrb r3, [r2, #17]
	movs r0, #26
	ands r5, r3
	movs r3, #32
	orrs r5, r3
	strb r5, [r2, #17]
	bl Object_GetById
	ldr r2, [r0, #80]
	movs r0, #128
	movs r3, #1
	lsls r0, r0, #4
	strb r3, [r2, #25]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008770
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008770
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	movs r0, #160
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020087a0
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	b .L_020087a0
.L_02008770:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008796
	movs r0, #145
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008796
	movs r0, #160
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020087a0
.L_02008796:
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
.L_020087a0:
	ldr r3, .L_02008834
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	bne .L_020087c2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #19
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020087c2
	bl Func_02000f80
.L_020087c2:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008824
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #137
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008824
	ldr r3, .L_02008834
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #7
	bne .L_0200880a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #142
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200880a
	bl Func_020019d8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #142
	bl GameFlag_SetBit
.L_0200880a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #142
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200882e
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	b .L_0200882e
.L_02008824:
	movs r0, #66
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
.L_0200882e:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008834:
	.4byte gPartyState
