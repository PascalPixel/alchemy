.syntax unified
	.thumb
	.section .text.x02008570,"ax",%progbits
	.balign 4
	.global Func_02000570
	.thumb_func
Func_02000570:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085b4
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #76
	movs r0, #144
	adds r3, r3, r2
	lsls r0, r0, #4
	movs r2, #3
	strh r2, [r3]
	adds r0, #17
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085a6
	bl Func_020007dc
	b .L_02008614
.L_020085a6:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	movs r0, #10
	b .L_020085e2
.L_020085b4:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085ec
	movs r3, #224
	lsls r3, r3, #8
	movs r0, #9
	ldr r1, .L_02008618
	ldr r2, .L_0200861c
	bl Func_02001dc0
	movs r3, #192
	movs r1, #236
	movs r0, #10
	lsls r1, r1, #17
	ldr r2, .L_02008620
	lsls r3, r3, #8
	bl Func_02001dc0
	movs r0, #8
.L_020085e2:
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	b .L_02008614
.L_020085ec:
	movs r3, #160
	lsls r3, r3, #8
	movs r0, #9
	ldr r1, .L_02008618
	ldr r2, .L_0200861c
	bl Func_02001dc0
	movs r3, #192
	movs r1, #236
	movs r0, #10
	lsls r1, r1, #17
	ldr r2, .L_02008620
	lsls r3, r3, #8
	bl Func_02001dc0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
.L_02008614:
	pop {pc}
	.2byte 0x0000
.L_02008618:
	.4byte gEwramHeap
.L_0200861c:
	.4byte 0x03490000
.L_02008620:
	.4byte 0x036d0000
	.section .text.x02008624,"ax",%progbits
	.global Func_02000624
	.thumb_func
Func_02000624:
	push {r5, r6, lr}
	movs r6, #192
	lsls r6, r6, #18
	ldr r3, [r6, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	ldr r5, .L_0200879c
	adds r2, #255
	str r2, [r3]
	adds r2, #11
	adds r3, r5, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #0
	bl Func_02001e80
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087a0
	cmp r2, r3
	bne .L_020086e6
	ldr r3, [r6, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #76
	movs r0, #128
	adds r3, r3, r2
	lsls r0, r0, #4
	movs r2, #3
	strh r2, [r3]
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200869a
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	b .L_020086b2
.L_0200869a:
	movs r0, #29
	bl Object_GetById
	movs r1, #3
	bl Object_SetPartAttribute
	movs r0, #30
	bl Object_GetById
	movs r1, #5
	bl Object_SetPartAttribute
.L_020086b2:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086e6
	movs r3, #64
	movs r5, #16
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #110
	movs r2, #16
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02001d28
	movs r3, #0
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #46
	movs r2, #16
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02001d20
.L_020086e6:
	ldr r5, .L_0200879c
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #20
	bne .L_020086fa
	bl Func_02000570
.L_020086fa:
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087a4
	cmp r2, r3
	bne .L_0200876a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008724
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	b .L_0200876a
.L_02008724:
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #10
	bl Func_02001ef0
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #10
	bl Object_GetById
	ldr r1, [r0, #80]
	movs r3, #8
	ldrb r2, [r1, #26]
	movs r0, #10
	orrs r3, r2
	strb r3, [r1, #26]
	bl Object_GetById
	ldr r1, [r0, #80]
	movs r3, #3
	ldrb r2, [r1, #17]
	movs r0, #10
	ands r3, r2
	movs r2, #32
	orrs r3, r2
	strb r3, [r1, #17]
	bl Object_GetById
	ldr r2, [r0, #80]
	movs r3, #1
	strb r3, [r2, #25]
.L_0200876a:
	bl Func_02000534
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200878a
	movs r0, #33
	bl Object_GetById
	movs r1, #2
	bl Object_SetPartAttribute
	b .L_02008796
.L_0200878a:
	movs r0, #28
	bl Object_GetById
	movs r1, #2
	bl Object_SetPartAttribute
.L_02008796:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
.L_0200879c:
	.4byte gPartyState
.L_020087a0:
	.4byte 0x0000002d
.L_020087a4:
	.4byte 0x0000002e
