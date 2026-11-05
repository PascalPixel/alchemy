.syntax unified
	.thumb
	.section .text.x02008624,"ax",%progbits
	.balign 4
	.global Func_02000624
	.thumb_func
Func_02000624:
	push {r5, r6, lr}
	ldr r3, [r0, #8]
	sub sp, #12
	mov r2, sp
	str r3, [r2]
	adds r6, r1, #0
	ldr r3, [r0, #16]
	movs r5, #8
	str r3, [r2, #8]
	ldrh r1, [r0, #6]
	movs r0, #128
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
.L_02008640:
	adds r0, r5, #0
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200869a
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_0200869a
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	cmp r3, #0
	beq .L_0200869a
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r6
	bne .L_0200869a
	mov r4, sp
	ldr r1, [r0, #8]
	ldr r3, [r4]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_02008678
	ldr r3, .L_020086a8
	cmp r2, r3
	ble .L_02008680
	b .L_0200869a
.L_02008678:
	ldr r2, .L_020086a8
	subs r3, r3, r1
	cmp r3, r2
	bgt .L_0200869a
.L_02008680:
	ldr r1, [r0, #16]
	ldr r3, [r4, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_02008692
	ldr r3, .L_020086a8
	cmp r2, r3
	ble .L_020086a2
	b .L_0200869a
.L_02008692:
	ldr r2, .L_020086a8
	subs r3, r3, r1
	cmp r3, r2
	ble .L_020086a2
.L_0200869a:
	adds r5, #1
	cmp r5, #63
	ble .L_02008640
	movs r0, #0
.L_020086a2:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020086a8:
	.4byte 0x0017ffff
	.section .text.x02008978,"ax",%progbits
	.balign 4
	.global Func_02000978
	.thumb_func
Func_02000978:
	push {r5, r6, lr}
	ldr r6, .L_02008a18
	ldr r3, [r6]
	cmp r3, #0
	beq .L_02008a16
	bl Func_0200584c
	bl Actor_GetById
	movs r1, #104
	adds r1, #255
	adds r5, r0, #0
	bl Func_020005b4
	ldr r3, [r6]
	lsls r1, r3, #15
	cmp r0, #0
	beq .L_02008a16
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_020089a8
	negs r1, r1
.L_020089a8:
	ldr r2, [r0, #16]
	ldr r3, [r5, #16]
	ldr r4, .L_02008a1c
	subs r3, r2, r3
	cmp r3, r4
	bge .L_020089bc
	movs r4, #128
	lsls r4, r4, #10
	adds r3, r2, r4
	str r3, [r5, #16]
.L_020089bc:
	ldr r2, [r0, #16]
	ldr r3, [r5, #16]
	movs r4, #128
	subs r3, r2, r3
	lsls r4, r4, #10
	cmp r3, r4
	ble .L_020089d0
	ldr r4, .L_02008a1c
	adds r3, r2, r4
	str r3, [r5, #16]
.L_020089d0:
	ldr r3, [r5, #8]
	adds r3, r3, r1
	str r3, [r5, #8]
	ldr r0, [r0, #8]
	subs r2, r0, r3
	cmp r2, #0
	blt .L_020089e8
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	bge .L_020089f2
	b .L_02008a16
.L_020089e8:
	movs r4, #160
	subs r3, r3, r0
	lsls r4, r4, #13
	cmp r3, r4
	blt .L_02008a16
.L_020089f2:
	ldr r3, [r5, #8]
	movs r2, #0
	subs r0, r3, r0
	cmp r0, #0
	beq .L_02008a04
	movs r2, #1
	cmp r0, #0
	bgt .L_02008a04
	negs r2, r2
.L_02008a04:
	lsls r1, r2, #1
	adds r1, r1, r2
	movs r3, #128
	lsls r1, r1, #18
	lsls r3, r3, #10
	adds r0, r5, #0
	movs r2, #0
	bl Func_020006ac
.L_02008a16:
	pop {r5, r6, pc}
.L_02008a18:
	.4byte gData_02005af0
.L_02008a1c:
	.4byte 0xfffe0000
	.section .text.x020091bc,"ax",%progbits
	.balign 4
	.global Func_020011bc
	.thumb_func
Func_020011bc:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	ldr r5, .L_0200927c
	movs r3, #192
	lsls r3, r3, #18
	movs r2, #133
	ldr r3, [r3, #108]
	lsls r2, r2, #2
	adds r5, r5, r2
	adds r6, r0, #0
	ldr r0, [r5]
	mov r8, r1
	mov r9, r3
	bl Actor_GetById
	mov r10, r0
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	mov r2, r10
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	ldr r0, [r5]
	adds r1, r6, #0
	mov r2, r8
	bl Actor_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	ldr r0, [r5]
	bl Actor_ArmCallback
	ldr r0, [r5]
	bl Actor_RefreshSelectorById
	ldr r0, [r5]
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r0, [r5]
	movs r1, #13
	bl Actor_SetModeById
	lsls r6, r6, #16
	mov r3, r8
	lsls r3, r3, #16
	ldr r2, .L_02009280
	adds r1, r6, #0
	mov r0, r10
	mov r8, r3
	bl Func_0200565c
	mov r0, r10
	bl Func_02005664
	movs r1, #10
	ldr r0, [r5]
	bl Actor_SetModeById
	movs r0, #123
	bl Func_02005854
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r2, #170
	lsls r2, r2, #1
	add r9, r2
	mov r2, r9
	movs r3, #0
	ldrsh r0, [r2, r3]
	bl Func_020057c4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200927c:
	.4byte gMainRoutineWork
.L_02009280:
	.4byte 0xfff40000
	.section .text.x0200c938,"ax",%progbits
	.balign 4
	.global Func_02004938
	.thumb_func
Func_02004938:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	mov r11, r0
	str r1, [sp, #36]
	mov r1, r11
	adds r1, #100
	str r1, [sp, #32]
	ldr r5, .L_0200cc88
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r1, #2
	str r1, [sp, #28]
	mov r10, r2
	movs r3, #0
	ldrsh r2, [r1, r3]
	subs r1, #4
	str r1, [sp, #24]
	mov r8, r2
	movs r3, #11
	ldrb r2, [r1]
	mov r1, r8
	muls r1, r3
	mov r9, r2
	lsls r2, r2, #14
	str r2, [sp, #20]
	adds r3, r1, #0
	add r3, r10
	lsls r3, r3, #2
	ldrb r2, [r5, r3]
	adds r3, r3, r5
	ldrb r3, [r3, #3]
	mov r1, r9
	str r3, [sp, #16]
	lsls r3, r1, #8
	adds r7, r3, r2
	ldr r3, [sp, #36]
	ldr r2, [sp, #36]
	lsls r3, r3, #10
	subs r2, #8
	adds r0, r3, #0
	str r2, [sp, #12]
	str r3, [sp, #8]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_0200cc8c
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	adds r4, r0, #0
	ldr r0, [sp, #8]
	str r4, [sp, #0]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	movs r1, #182
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r2, #128
	add r3, r10
	lsls r2, r2, #12
	lsls r3, r3, #20
	adds r1, #2
	adds r6, r3, r2
	adds r3, r5, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	ldr r4, [sp, #0]
	add r3, r8
	lsls r3, r3, #20
	adds r5, r3, r2
	ldr r3, [sp, #16]
	mov r2, r11
	adds r2, #99
	str r2, [sp, #4]
	cmp r3, #2
	bne .L_0200ca14
	ldrb r3, [r2]
	movs r1, #0
	mov r10, r1
	mov r8, r1
	mov r9, r1
	cmp r3, #1
	beq .L_0200c9fc
	b .L_0200ccf6
.L_0200c9fc:
	movs r0, #204
	bl Func_02005854
	ldr r1, .L_0200cc90
	mov r0, r11
	bl Func_02005634
	add r2, sp, #16
	ldrb r2, [r2]
	ldr r3, [sp, #4]
	strb r2, [r3]
	b .L_0200ccf6
.L_0200ca14:
	movs r3, #12
	adds r3, #255
	cmp r7, r3
	bne .L_0200ca1e
	b .L_0200cc74
.L_0200ca1e:
	cmp r7, r3
	bgt .L_0200ca9c
	cmp r7, #11
	bne .L_0200ca28
	b .L_0200cc6a
.L_0200ca28:
	cmp r7, #11
	bgt .L_0200ca5e
	cmp r7, #6
	bne .L_0200ca32
	b .L_0200cbb4
.L_0200ca32:
	cmp r7, #6
	bgt .L_0200ca4a
	cmp r7, #2
	bge .L_0200ca3c
	b .L_0200ccd2
.L_0200ca3c:
	cmp r7, #3
	bgt .L_0200ca42
	b .L_0200cb54
.L_0200ca42:
	cmp r7, #5
	bne .L_0200ca48
	b .L_0200cc30
.L_0200ca48:
	b .L_0200ccd2
.L_0200ca4a:
	cmp r7, #9
	bne .L_0200ca50
	b .L_0200cc30
.L_0200ca50:
	cmp r7, #9
	ble .L_0200ca56
	b .L_0200cc98
.L_0200ca56:
	cmp r7, #8
	bne .L_0200ca5c
	b .L_0200cbb4
.L_0200ca5c:
	b .L_0200ccd2
.L_0200ca5e:
	movs r3, #8
	adds r3, #255
	cmp r7, r3
	bne .L_0200ca68
	b .L_0200cbd0
.L_0200ca68:
	cmp r7, r3
	bgt .L_0200ca8a
	subs r3, #4
	cmp r7, r3
	beq .L_0200cb60
	cmp r7, r3
	bgt .L_0200ca7e
	subs r3, #2
	cmp r7, r3
	beq .L_0200cb60
	b .L_0200ccd2
.L_0200ca7e:
	movs r1, #131
	lsls r1, r1, #1
	cmp r7, r1
	bne .L_0200ca88
	b .L_0200cc10
.L_0200ca88:
	b .L_0200ccd2
.L_0200ca8a:
	movs r3, #10
	adds r3, #255
	cmp r7, r3
	bne .L_0200ca94
	b .L_0200cbd0
.L_0200ca94:
	cmp r7, r3
	ble .L_0200ca9a
	b .L_0200cc98
.L_0200ca9a:
	b .L_0200cc10
.L_0200ca9c:
	movs r3, #134
	lsls r3, r3, #1
	adds r3, #255
	cmp r7, r3
	bne .L_0200caa8
	b .L_0200cc7e
.L_0200caa8:
	cmp r7, r3
	bgt .L_0200cae4
	subs r3, #4
	cmp r7, r3
	bne .L_0200cab4
	b .L_0200cbf2
.L_0200cab4:
	cmp r7, r3
	bgt .L_0200cad0
	movs r2, #129
	lsls r2, r2, #1
	adds r2, #255
	cmp r7, r2
	bgt .L_0200cac4
	b .L_0200ccd2
.L_0200cac4:
	subs r3, #3
	cmp r7, r3
	blt .L_0200cb46
	cmp r7, r3
	beq .L_0200cb7a
	b .L_0200ccd2
.L_0200cad0:
	movs r3, #133
	lsls r3, r3, #1
	adds r3, #255
	cmp r7, r3
	bne .L_0200cadc
	b .L_0200cbf2
.L_0200cadc:
	cmp r7, r3
	ble .L_0200cae2
	b .L_0200cc98
.L_0200cae2:
	b .L_0200cb7a
.L_0200cae4:
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #5
	cmp r7, r3
	beq .L_0200cb94
	cmp r7, r3
	bgt .L_0200cb0a
	subs r3, #2
	cmp r7, r3
	beq .L_0200cb6c
	cmp r7, r3
	ble .L_0200cafe
	b .L_0200cc4a
.L_0200cafe:
	movs r1, #192
	lsls r1, r1, #2
	adds r1, #1
	cmp r7, r1
	beq .L_0200cb6c
	b .L_0200ccd2
.L_0200cb0a:
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #9
	cmp r7, r3
	beq .L_0200cb94
	cmp r7, r3
	bgt .L_0200cb24
	movs r2, #194
	lsls r2, r2, #2
	cmp r7, r2
	bne .L_0200cb22
	b .L_0200cc4a
.L_0200cb22:
	b .L_0200ccd2
.L_0200cb24:
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #10
	cmp r7, r3
	bne .L_0200cb30
	b .L_0200cc98
.L_0200cb30:
	movs r2, #131
	lsls r2, r2, #2
	movs r1, #0
	adds r2, #255
	mov r10, r1
	mov r8, r1
	mov r9, r1
	cmp r7, r2
	bne .L_0200cb44
	b .L_0200ccf6
.L_0200cb44:
	b .L_0200ccd2
.L_0200cb46:
	ldr r1, [sp, #12]
	movs r3, #1
	negs r3, r3
	add r10, r3
	lsls r3, r1, #16
	subs r6, r6, r3
	b .L_0200ccf6
.L_0200cb54:
	ldr r1, [sp, #12]
	movs r2, #1
	lsls r3, r1, #16
	add r10, r2
	adds r6, r6, r3
	b .L_0200ccf6
.L_0200cb60:
	ldr r1, [sp, #12]
	movs r2, #1
	lsls r3, r1, #16
	add r8, r2
	adds r5, r5, r3
	b .L_0200ccf6
.L_0200cb6c:
	ldr r1, [sp, #12]
	movs r2, #1
	negs r2, r2
	lsls r3, r1, #16
	add r8, r2
	subs r5, r5, r3
	b .L_0200ccf6
.L_0200cb7a:
	movs r2, #1
	add r8, r2
	mov r9, r2
	movs r2, #128
	subs r3, r6, r4
	lsls r2, r2, #12
	adds r6, r3, r2
	ldr r1, [sp, #8]
	subs r3, r5, r0
	adds r5, r3, r2
	movs r3, #128
	lsls r3, r3, #8
	b .L_0200cbec
.L_0200cb94:
	ldr r1, .L_0200cc94
	movs r2, #1
	negs r2, r2
	movs r3, #2
	add r10, r2
	mov r9, r3
	movs r2, #128
	adds r3, r6, r0
	adds r6, r3, r1
	lsls r2, r2, #12
	subs r3, r5, r4
	ldr r1, [sp, #8]
	adds r5, r3, r2
	movs r3, #192
	lsls r3, r3, #8
	b .L_0200cbec
.L_0200cbb4:
	movs r2, #1
	negs r2, r2
	add r8, r2
	ldr r1, [sp, #8]
	ldr r2, .L_0200cc94
	movs r3, #3
	mov r9, r3
	adds r3, r6, r4
	adds r6, r3, r2
	negs r1, r1
	adds r3, r5, r0
	adds r5, r3, r2
	str r1, [sp, #20]
	b .L_0200ccf6
.L_0200cbd0:
	movs r2, #1
	movs r3, #0
	add r10, r2
	movs r1, #128
	ldr r2, .L_0200cc94
	lsls r1, r1, #12
	mov r9, r3
	subs r3, r6, r0
	adds r6, r3, r1
	adds r3, r5, r4
	ldr r1, [sp, #8]
	adds r5, r3, r2
	movs r3, #128
	lsls r3, r3, #7
.L_0200cbec:
	subs r3, r3, r1
	str r3, [sp, #20]
	b .L_0200ccf6
.L_0200cbf2:
	movs r2, #1
	negs r2, r2
	movs r3, #3
	add r8, r2
	movs r1, #128
	ldr r2, .L_0200cc94
	lsls r1, r1, #12
	mov r9, r3
	subs r3, r6, r4
	adds r6, r3, r1
	adds r3, r5, r0
	adds r5, r3, r2
	ldr r3, [sp, #8]
	movs r1, #128
	b .L_0200cc62
.L_0200cc10:
	movs r2, #1
	negs r2, r2
	add r10, r2
	ldr r2, .L_0200cc94
	movs r3, #2
	mov r9, r3
	adds r3, r6, r0
	adds r6, r3, r2
	ldr r1, [sp, #8]
	adds r3, r5, r4
	adds r5, r3, r2
	movs r2, #128
	lsls r2, r2, #7
	adds r2, r1, r2
	str r2, [sp, #20]
	b .L_0200ccf6
.L_0200cc30:
	ldr r1, .L_0200cc94
	movs r3, #1
	add r8, r3
	mov r9, r3
	movs r2, #128
	adds r3, r6, r4
	adds r6, r3, r1
	lsls r2, r2, #12
	subs r3, r5, r0
	adds r5, r3, r2
	ldr r3, [sp, #8]
	str r3, [sp, #20]
	b .L_0200ccf6
.L_0200cc4a:
	movs r2, #0
	mov r9, r2
	movs r2, #128
	subs r3, r6, r0
	lsls r2, r2, #12
	adds r6, r3, r2
	subs r3, r5, r4
	movs r1, #1
	adds r5, r3, r2
	ldr r3, [sp, #8]
	add r10, r1
	movs r1, #192
.L_0200cc62:
	lsls r1, r1, #8
	adds r1, r3, r1
	str r1, [sp, #20]
	b .L_0200ccf6
.L_0200cc6a:
	movs r2, #0
	mov r10, r2
	mov r8, r2
	mov r9, r2
	b .L_0200ccf6
.L_0200cc74:
	movs r3, #0
	mov r10, r3
	mov r8, r3
	mov r9, r3
	b .L_0200ccf6
.L_0200cc7e:
	movs r1, #0
	mov r10, r1
	mov r8, r1
	mov r9, r1
	b .L_0200ccf6
.L_0200cc88:
	.4byte gSceneState
.L_0200cc8c:
	.4byte IwramMulQ16
.L_0200cc90:
	.4byte gData_02006f38
.L_0200cc94:
	.4byte 0xfff80000
.L_0200cc98:
	ldr r2, [sp, #4]
	ldrb r3, [r2]
	cmp r3, #1
	bne .L_0200ccf6
	add r2, sp, #20
	mov r3, r11
	ldrh r2, [r2]
	str r6, [r3, #8]
	mov r1, r11
	movs r3, #0
	str r3, [r1, #12]
	mov r3, r11
	str r5, [r1, #16]
	strh r2, [r3, #6]
	asrs r3, r6, #20
	cmp r3, #24
	bne .L_0200ccc8
	asrs r3, r5, #20
	cmp r3, #14
	bne .L_0200ccc8
	ldr r1, [sp, #4]
	movs r3, #7
	strb r3, [r1]
	b .L_0200ccf6
.L_0200ccc8:
	movs r0, #204
	bl Func_02005854
	ldr r1, .L_0200cd40
	b .L_0200ccea
.L_0200ccd2:
	ldr r1, [sp, #4]
	movs r3, #0
	mov r10, r3
	mov r8, r3
	mov r9, r3
	ldrb r3, [r1]
	cmp r3, #1
	bne .L_0200ccf6
	movs r0, #132
	bl Func_02005854
	ldr r1, .L_0200cd44
.L_0200ccea:
	mov r0, r11
	bl Func_02005634
	ldr r2, [sp, #4]
	movs r3, #2
	strb r3, [r2]
.L_0200ccf6:
	ldr r3, [sp, #36]
	cmp r3, #15
	bne .L_0200cd0e
	ldr r2, [sp, #32]
	mov r1, r10
	strh r1, [r2]
	ldr r1, [sp, #28]
	mov r3, r8
	strh r3, [r1]
	ldr r3, [sp, #24]
	mov r2, r9
	strb r2, [r3]
.L_0200cd0e:
	mov r3, r11
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_0200cd30
	mov r1, r11
	movs r3, #0
	add r2, sp, #20
	str r3, [r1, #12]
	str r6, [r1, #8]
	str r5, [r1, #16]
	ldrh r2, [r2]
	mov r3, r11
	strh r2, [r3, #6]
	ldr r3, [r3, #76]
	adds r3, #1
	str r3, [r1, #76]
.L_0200cd30:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200cd40:
	.4byte gData_02006f38
.L_0200cd44:
	.4byte gData_02006ec0
	.section .text.x0200cd48,"ax",%progbits
	.balign 4
	.global Func_02004d48
	.thumb_func
Func_02004d48:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r2, r11
	sub sp, #8
	adds r2, #1
	str r1, [sp, #4]
	str r2, [sp, #0]
	bl Actor_GetById
	movs r2, #0
	adds r7, r0, #0
	mov r9, r2
	adds r3, r7, #0
	adds r3, #100
	mov r2, r9
	strh r2, [r3]
	movs r2, #7
	mov r10, r2
	mov r2, r10
	adds r3, #2
	strh r2, [r3]
	movs r2, #1
	mov r8, r2
	subs r3, #4
	movs r5, #2
	mov r2, r8
	strb r5, [r3]
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	movs r1, #2
	adds r0, r7, #0
	bl Func_02005624
	ldr r0, [sp, #0]
	ldr r6, [sp, #0]
	bl Actor_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #100
	strh r5, [r3]
	mov r2, r10
	adds r3, #2
	strh r2, [r3]
	subs r3, #4
	movs r2, #0
	strb r2, [r3]
	mov r2, r8
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	adds r6, #1
	movs r1, #2
	adds r0, r7, #0
	bl Func_02005624
	adds r0, r6, #0
	bl Actor_GetById
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #100
	movs r3, #8
	strh r3, [r2]
	adds r3, r7, #0
	mov r2, r10
	adds r3, #102
	strh r2, [r3]
	subs r3, #4
	movs r2, #2
	strb r2, [r3]
	mov r2, r8
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	adds r6, #1
	movs r1, #2
	adds r0, r7, #0
	bl Func_02005624
	adds r0, r6, #0
	bl Actor_GetById
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #100
	movs r3, #10
	strh r3, [r2]
	adds r3, r7, #0
	mov r2, r10
	adds r3, #102
	strh r2, [r3]
	subs r3, #4
	movs r2, #0
	strb r2, [r3]
	mov r2, r8
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	adds r0, r7, #0
	movs r1, #2
	bl Func_02005624
	movs r2, #0
	mov r10, r2
.L_0200ce40:
	movs r3, #1
	mov r9, r3
	movs r5, #0
.L_0200ce46:
	movs r2, #4
	add r2, r11
	mov r8, r2
	mov r6, r11
	cmp r11, r8
	bge .L_0200ce7c
.L_0200ce52:
	adds r0, r6, #0
	bl Actor_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200ce6c
	cmp r3, #7
	beq .L_0200ce6c
	movs r3, #0
	mov r9, r3
.L_0200ce6c:
	adds r2, r6, #0
	adds r0, r7, #0
	adds r1, r5, #0
	adds r6, #1
	bl Func_02004938
	cmp r6, r8
	blt .L_0200ce52
.L_0200ce7c:
	movs r0, #1
	adds r5, #1
	bl wait
	cmp r5, #15
	ble .L_0200ce46
	mov r2, r9
	cmp r2, #0
	bne .L_0200ce98
	movs r3, #1
	add r10, r3
	mov r2, r10
	cmp r2, #127
	ble .L_0200ce40
.L_0200ce98:
	movs r0, #20
	bl wait
	ldr r3, [sp, #4]
	movs r0, #3
	cmp r3, #0
	beq .L_0200cf32
	mov r0, r11
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r6, #0
	movs r5, #0
	cmp r3, #0
	beq .L_0200ceb8
	ldr r6, [r0, #76]
.L_0200ceb8:
	ldr r0, [sp, #0]
	bl Actor_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200ced0
	ldr r0, [r0, #76]
	cmp r6, #0
	beq .L_0200cece
	cmp r6, r0
	bls .L_0200ced0
.L_0200cece:
	adds r6, r0, #0
.L_0200ced0:
	mov r0, r11
	adds r0, #2
	bl Actor_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200cee0
	ldr r5, [r0, #76]
.L_0200cee0:
	mov r0, r11
	adds r0, #3
	bl Actor_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200cefa
	ldr r0, [r0, #76]
	cmp r5, #0
	beq .L_0200cef8
	cmp r5, r0
	bls .L_0200cefa
.L_0200cef8:
	adds r5, r0, #0
.L_0200cefa:
	cmp r6, #0
	beq .L_0200cf2a
	movs r0, #1
	cmp r5, #0
	beq .L_0200cf32
	movs r0, #0
	cmp r6, r5
	beq .L_0200cf32
	cmp r6, r5
	bcs .L_0200cf1c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #235
	bl GameFlag_SetBit
	movs r0, #1
	b .L_0200cf32
.L_0200cf1c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #235
	bl GameFlag_SetBit
	movs r0, #2
	b .L_0200cf32
.L_0200cf2a:
	movs r0, #2
	cmp r5, #0
	bne .L_0200cf32
	movs r0, #3
.L_0200cf32:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x0200d090,"ax",%progbits
	.balign 4
	.global Func_02005090
	.thumb_func
Func_02005090:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200d1f0
	movs r0, #24
	adds r0, r0, r5
	mov r9, r0
	ldr r0, .L_0200d1f4
	ldr r2, .L_0200d1f8
	ldr r3, [r0]
	sub sp, #20
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	ldr r1, [r5]
	lsrs r3, r3, #5
	ldr r7, [r5, #4]
	str r3, [sp, #8]
	movs r2, #132
	adds r2, r2, r5
	ldr r0, [r2]
	mov r10, r1
	movs r1, #32
	adds r1, r1, r5
	mov r11, r2
	adds r0, #50
	mov r8, r1
	mov r1, r9
	bl Func_02004f84
	mov r3, r11
	ldr r0, [r3]
	ldr r1, .L_0200d1fc
	adds r0, #60
	bl Func_02004f84
	mov r1, r11
	ldr r0, [r1]
	ldr r1, .L_0200d200
	bl Func_02004f84
	movs r6, #4
.L_0200d0ec:
	mov r2, r9
	ldrb r1, [r2]
	ldr r3, .L_0200d204
	lsls r1, r1, #6
	add r1, r10
	adds r1, r1, r3
	adds r0, r7, #0
	ldr r3, .L_0200d208
	movs r2, #64
	mov lr, r3
	.2byte 0xf800
	subs r6, #1
	movs r0, #1
	adds r7, #64
	add r9, r0
	cmp r6, #0
	bge .L_0200d0ec
	ldr r1, .L_0200d1f4
	ldr r2, [r5, #4]
	ldr r0, [r1]
	movs r1, #160
	lsls r1, r1, #1
	bl VramCache_Load
	add r5, sp, #12
	ldr r3, [r5, #4]
	ldr r2, .L_0200d20c
	adds r0, r5, #0
	ands r3, r2
	str r3, [r5, #4]
	ldr r3, [sp, #12]
	movs r6, #0
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #2
	orrs r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #18
	orrs r3, r2
	str r3, [sp, #12]
	bl AffineMatrix_BuildForEffect
	str r0, [sp, #4]
	mov r2, r11
	ldr r0, [r2]
	bl Func_02005020
	movs r3, #0
	strh r3, [r5, #4]
	strh r0, [r5]
	strh r0, [r5, #2]
	adds r0, r5, #0
	bl AffineMatrix_BuildForEffect
	ldr r3, .L_0200d1fc
	str r0, [sp, #0]
	ldr r7, .L_0200d200
	mov r10, r3
.L_0200d168:
	mov r1, r8
	movs r3, #0
	stmia r1!, {r3}
	movs r2, #128
	lsls r2, r2, #8
	lsls r3, r6, #20
	orrs r3, r2
	stmia r1!, {r3}
	ldr r0, [sp, #8]
	movs r3, #128
	lsls r3, r3, #3
	orrs r3, r0
	str r3, [r1]
	mov r1, r8
	ldrb r3, [r1, #5]
	movs r2, #3
	orrs r3, r2
	strb r3, [r1, #5]
	mov r0, r10
	ldrb r3, [r0]
	ldrb r2, [r7]
	mov r12, r3
	ldr r5, [sp, #8]
	adds r3, r6, #1
	mov r11, r3
	movs r3, #12
	movs r1, #1
	mov r0, r8
	add r3, r8
	add r10, r1
	adds r7, #1
	ldrb r1, [r0, #7]
	adds r5, #2
	mov r9, r3
	cmp r12, r2
	bne .L_0200d1b4
	cmp r6, #2
	bne .L_0200d1b8
.L_0200d1b4:
	ldr r2, [sp, #0]
	b .L_0200d1ba
.L_0200d1b8:
	ldr r2, [sp, #4]
.L_0200d1ba:
	movs r0, #63
	movs r3, #31
	negs r0, r0
	ands r2, r3
	adds r3, r0, #0
	ands r3, r1
	lsls r2, r2, #1
	orrs r3, r2
	mov r1, r8
	strb r3, [r1, #7]
	mov r0, r8
	movs r1, #236
	bl Func_020055ec
	mov r6, r11
	str r5, [sp, #8]
	mov r8, r9
	cmp r6, #4
	ble .L_0200d168
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d1f0:
	.4byte gData_020023c4 + 0x288
.L_0200d1f4:
	.4byte gData_020023c4 + 0x308
.L_0200d1f8:
	.4byte gResourceTableEntries
.L_0200d1fc:
	.4byte gData_020023c4 + 0x290
.L_0200d200:
	.4byte gData_020023c4 + 0x298
.L_0200d204:
	.4byte 0xfffff400
.L_0200d208:
	.4byte IwramCopyWords
.L_0200d20c:
	.4byte 0xffff0000
	.section .bss,"aw",%nobits
	.global Data_02006fc8
Data_02006fc8:
	.space 0x00000020
	.global Data_02006fe8
Data_02006fe8:
	.space 0x00000020
	.global Data_02007008
Data_02007008:
	.space 0x00000020
	.global Data_02007028
Data_02007028:
	.space 0x00000020
	.global Data_02007048
Data_02007048:
	.space 0x00000020
	.global gData_02007068
gData_02007068:
