.syntax unified
	.thumb
	.section .text.x0200847c,"ax",%progbits
	.balign 4
	.global Func_0200047c
	.thumb_func
Func_0200047c:
	push {r5, lr}
	ldr r2, [r0, #104]
	cmp r2, #0
	beq .L_020084b4
	ldr r3, [r2, #8]
	movs r1, #128
	str r3, [r0, #8]
	lsls r1, r1, #13
	ldr r3, [r2, #12]
	adds r5, r0, #0
	adds r3, r3, r1
	str r3, [r0, #12]
	adds r5, #98
	ldr r3, [r2, #16]
	adds r2, r0, #0
	str r3, [r0, #16]
	adds r2, #8
	ldrb r3, [r5]
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r0, #192
	adds r1, r3, #0
	lsls r0, r0, #11
	bl Vector_AddPolarOffsetFar
	ldrb r3, [r5]
	adds r3, #16
	strb r3, [r5]
.L_020084b4:
	movs r0, #1
	pop {r5, pc}
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
	.section .text.x02008784,"ax",%progbits
	.balign 4
	.global Func_02000784
	.thumb_func
Func_02000784:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_02008968
	sub sp, #36
	ldr r3, [r5]
	cmp r3, #0
	bne .L_0200879e
	b .L_0200895a
.L_0200879e:
	bl Func_0200584c
	bl Actor_GetById
	movs r1, #102
	adds r1, #255
	adds r7, r0, #0
	bl Func_020005b4
	ldr r2, [r5]
	ldr r3, .L_0200896c
	mov r9, r0
	adds r1, r2, #0
	muls r1, r3
	str r1, [sp, #20]
	cmp r0, #0
	bne .L_020087c2
	b .L_02008952
.L_020087c2:
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r7, #12]
	cmp r3, r2
	ble .L_020087d2
	b .L_0200895a
.L_020087d2:
	ldrh r3, [r0, #6]
	movs r1, #128
	lsls r1, r1, #8
	cmp r3, r1
	bne .L_020087e2
	ldr r2, [sp, #20]
	negs r2, r2
	str r2, [sp, #20]
.L_020087e2:
	mov r1, r9
	ldr r3, [r1, #8]
	ldr r2, [r7, #8]
	subs r2, r2, r3
	str r2, [sp, #8]
	ldr r2, [r7, #16]
	ldr r3, [r1, #16]
	subs r2, r2, r3
	str r2, [sp, #4]
	ldr r2, [sp, #20]
	negs r2, r2
	adds r0, r2, #0
	str r2, [sp, #0]
	bl Math_Sine
	adds r6, r0, #0
	ldr r0, [sp, #0]
	bl Math_Cosine
	ldr r3, .L_02008970
	ldr r1, [sp, #8]
	mov r8, r0
	mov lr, r3
	.2byte 0xf800
	ldr r2, .L_02008970
	ldr r1, [sp, #4]
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r2
	.2byte 0xf800
	mov r1, r9
	ldr r3, [r1, #8]
	ldr r2, .L_02008970
	adds r3, r3, r5
	adds r3, r3, r0
	str r3, [sp, #16]
	ldr r1, [sp, #8]
	adds r0, r6, #0
	mov lr, r2
	.2byte 0xf800
	ldr r3, .L_02008970
	adds r5, r0, #0
	ldr r1, [sp, #4]
	mov r0, r8
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	ldr r3, [r1, #16]
	ldr r1, [sp, #16]
	subs r3, r3, r5
	adds r3, r3, r0
	str r3, [sp, #12]
	ldr r3, [r7, #36]
	add r2, sp, #24
	adds r3, r1, r3
	str r3, [r2]
	mov r10, r2
	mov r2, r9
	ldr r3, [r2, #12]
	mov r1, r10
	str r3, [r1, #4]
	ldr r3, [r7, #44]
	ldr r2, [sp, #12]
	adds r0, r7, #0
	adds r3, r2, r3
	str r3, [r1, #8]
	bl Func_02005684
	cmp r0, #0
	bne .L_02008952
	ldr r3, [sp, #20]
	movs r5, #168
	lsls r5, r5, #5
	adds r5, #85
	subs r5, r5, r3
	adds r0, r5, #0
	bl Math_Sine
	adds r6, r0, #0
	adds r0, r5, #0
	bl Math_Cosine
	ldr r2, .L_02008970
	ldr r1, [sp, #8]
	mov r8, r0
	mov lr, r2
	.2byte 0xf800
	ldr r3, .L_02008970
	ldr r1, [sp, #4]
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	ldr r3, [r1, #8]
	ldr r2, .L_02008970
	adds r3, r3, r5
	adds r3, r3, r0
	ldr r1, [sp, #8]
	adds r0, r6, #0
	mov r11, r3
	mov lr, r2
	.2byte 0xf800
	ldr r3, .L_02008970
	adds r5, r0, #0
	ldr r1, [sp, #4]
	mov r0, r8
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	ldr r3, [r1, #16]
	mov r2, r10
	subs r3, r3, r5
	adds r0, r3, r0
	ldr r3, [r7, #36]
	mov r1, r10
	add r3, r11
	str r3, [r2]
	ldr r3, [r7, #44]
	adds r3, r0, r3
	str r3, [r2, #8]
	adds r0, r7, #0
	bl Func_02005684
	cmp r0, #0
	bne .L_02008952
	ldr r3, [sp, #0]
	ldr r1, .L_02008974
	adds r5, r3, r1
	adds r0, r5, #0
	bl Math_Sine
	adds r6, r0, #0
	adds r0, r5, #0
	bl Math_Cosine
	ldr r2, .L_02008970
	ldr r1, [sp, #8]
	mov r8, r0
	mov lr, r2
	.2byte 0xf800
	ldr r3, .L_02008970
	ldr r1, [sp, #4]
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	ldr r3, [r1, #8]
	ldr r2, .L_02008970
	adds r3, r3, r5
	adds r3, r3, r0
	ldr r1, [sp, #8]
	adds r0, r6, #0
	mov r11, r3
	mov lr, r2
	.2byte 0xf800
	ldr r3, .L_02008970
	adds r5, r0, #0
	ldr r1, [sp, #4]
	mov r0, r8
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	ldr r3, [r1, #16]
	mov r2, r10
	subs r3, r3, r5
	adds r0, r3, r0
	ldr r3, [r7, #36]
	mov r1, r10
	add r3, r11
	str r3, [r2]
	ldr r3, [r7, #44]
	adds r3, r0, r3
	str r3, [r2, #8]
	adds r0, r7, #0
	bl Func_02005684
	cmp r0, #0
	bne .L_02008952
	ldr r3, [sp, #16]
	str r3, [r7, #8]
	ldr r1, [sp, #12]
	str r1, [r7, #16]
.L_02008952:
	ldrh r3, [r7, #6]
	ldr r2, [sp, #20]
	adds r3, r3, r2
	strh r3, [r7, #6]
.L_0200895a:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008968:
	.4byte gData_02005af0
.L_0200896c:
	.4byte 0xfffffe94
.L_02008970:
	.4byte IwramMulQ16
.L_02008974:
	.4byte 0xffffeaab
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
	.section .text.x02008ad4,"ax",%progbits
	.balign 4
	.global Func_02000ad4
	.thumb_func
Func_02000ad4:
	push {r5, r6, lr}
	ldr r6, .L_02008c28
	ldr r3, [r6]
	cmp r3, #0
	bne .L_02008ae0
	b .L_02008c24
.L_02008ae0:
	bl Func_0200584c
	bl Actor_GetById
	ldr r3, [r6]
	movs r1, #180
	lsls r1, r1, #1
	adds r5, r0, #0
	lsls r6, r3, #15
	bl Func_020005b4
	cmp r0, #0
	beq .L_02008b68
	ldr r2, [r0, #8]
	ldr r3, [r5, #8]
	ldr r1, .L_02008c2c
	subs r3, r2, r3
	cmp r3, r1
	bge .L_02008b0e
	movs r1, #128
	lsls r1, r1, #10
	adds r3, r2, r1
	str r3, [r5, #8]
.L_02008b0e:
	ldr r2, [r0, #8]
	ldr r3, [r5, #8]
	movs r1, #128
	subs r3, r2, r3
	lsls r1, r1, #10
	cmp r3, r1
	ble .L_02008b22
	ldr r1, .L_02008c2c
	adds r3, r2, r1
	str r3, [r5, #8]
.L_02008b22:
	ldr r3, [r5, #16]
	adds r3, r3, r6
	str r3, [r5, #16]
	ldr r0, [r0, #16]
	subs r2, r0, r3
	cmp r2, #0
	blt .L_02008b3a
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	bge .L_02008b44
	b .L_02008b68
.L_02008b3a:
	movs r1, #160
	subs r3, r3, r0
	lsls r1, r1, #13
	cmp r3, r1
	blt .L_02008b68
.L_02008b44:
	ldr r3, [r5, #16]
	movs r1, #0
	subs r0, r3, r0
	cmp r0, #0
	beq .L_02008b56
	movs r1, #1
	cmp r0, #0
	bgt .L_02008b56
	negs r1, r1
.L_02008b56:
	lsls r2, r1, #1
	adds r2, r2, r1
	movs r3, #128
	lsls r2, r2, #18
	lsls r3, r3, #10
	adds r0, r5, #0
	movs r1, #0
	bl Func_020006ac
.L_02008b68:
	movs r1, #106
	adds r1, #255
	adds r0, r5, #0
	bl Func_020005b4
	cmp r0, #0
	beq .L_02008c24
	ldr r2, [r0, #8]
	ldr r3, [r5, #8]
	ldr r1, .L_02008c2c
	subs r3, r2, r3
	cmp r3, r1
	bge .L_02008b8a
	movs r1, #128
	lsls r1, r1, #10
	adds r3, r2, r1
	str r3, [r5, #8]
.L_02008b8a:
	ldr r2, [r0, #8]
	ldr r3, [r5, #8]
	movs r1, #128
	subs r3, r2, r3
	lsls r1, r1, #10
	cmp r3, r1
	ble .L_02008b9e
	ldr r1, .L_02008c2c
	adds r3, r2, r1
	str r3, [r5, #8]
.L_02008b9e:
	ldr r3, [r5, #16]
	subs r3, r3, r6
	str r3, [r5, #16]
	ldr r0, [r0, #16]
	subs r2, r0, r3
	cmp r2, #0
	blt .L_02008bb6
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	bge .L_02008bc0
	b .L_02008be4
.L_02008bb6:
	movs r1, #160
	subs r3, r3, r0
	lsls r1, r1, #13
	cmp r3, r1
	blt .L_02008be4
.L_02008bc0:
	ldr r3, [r5, #16]
	movs r1, #0
	subs r0, r3, r0
	cmp r0, #0
	beq .L_02008bd2
	movs r1, #1
	cmp r0, #0
	bgt .L_02008bd2
	negs r1, r1
.L_02008bd2:
	lsls r2, r1, #1
	adds r2, r2, r1
	movs r3, #128
	lsls r2, r2, #18
	lsls r3, r3, #10
	adds r0, r5, #0
	movs r1, #0
	bl Func_020006ac
.L_02008be4:
	ldr r3, .L_02008c30
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008c34
	cmp r2, r3
	bne .L_02008c24
	ldr r3, [r5, #8]
	asrs r2, r3, #20
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #15
	bne .L_02008c24
	cmp r2, #28
	bne .L_02008c14
	adds r0, r5, #0
	bl Func_02000a20
	movs r0, #5
	bl Func_020057c4
	b .L_02008c24
.L_02008c14:
	cmp r2, #30
	bne .L_02008c24
	adds r0, r5, #0
	bl Func_02000a20
	movs r0, #6
	bl Func_020057c4
.L_02008c24:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008c28:
	.4byte gData_02005af0
.L_02008c2c:
	.4byte 0xfffe0000
.L_02008c30:
	.4byte gMainRoutineWork
.L_02008c34:
	.4byte 0x00000076
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
	.section .text.x02009c10,"ax",%progbits
	.balign 4
	.global Func_02001c10
	.thumb_func
Func_02001c10:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	movs r4, #158
	lsls r4, r4, #1
	adds r3, r2, r4
	ldr r3, [r3]
	movs r6, #186
	lsls r6, r6, #1
	mov r9, r3
	adds r3, r2, r6
	ldr r3, [r3]
	subs r4, #4
	mov r10, r3
	adds r3, r2, r4
	ldr r3, [r3]
	subs r6, #4
	sub sp, #4
	mov r8, r3
	adds r3, r2, r6
	movs r2, #0
	ldr r5, [r3]
	ldr r0, .L_0200a000
	ldr r1, .L_0200a004
	str r2, [sp, #0]
.L_02009c50:
	ldmia r1!, {r3}
	stmia r0!, {r3}
	ldr r3, [sp, #0]
	adds r3, #1
	str r3, [sp, #0]
	cmp r3, #11
	bls .L_02009c50
	movs r4, #8
	str r4, [sp, #0]
.L_02009c62:
	ldr r0, [sp, #0]
	bl Actor_GetById
	mov r11, r0
	cmp r0, #0
	bne .L_02009c70
	b .L_0200a04a
.L_02009c70:
	mov r3, r11
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	beq .L_02009c7c
	b .L_0200a04a
.L_02009c7c:
	mov r6, r11
	ldr r3, [r6, #80]
	ldr r0, [r3, #40]
	cmp r0, #0
	bne .L_02009c88
	b .L_0200a04a
.L_02009c88:
	ldr r3, [r6, #8]
	movs r2, #0
	asrs r1, r3, #20
	ldr r3, [r6, #16]
	asrs r4, r3, #20
	movs r6, #0
	ldrsh r3, [r0, r6]
	ldr r0, .L_0200a008
	adds r3, r3, r0
	cmp r3, #5
	bls .L_02009ca0
	b .L_0200a02e
.L_02009ca0:
	ldr r2, .L_0200a00c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02009ca8:
	.4byte .L_02009ce8
	.4byte .L_02009cc0
	.4byte .L_02009e02
	.4byte .L_02009f4a
	.4byte .L_02009ea6
	.4byte .L_0200a02c
.L_02009cc0:
	lsls r3, r4, #7
	adds r6, r1, r3
	movs r3, #116
	mov r1, r9
	strb r3, [r1, r6]
	mov r2, r10
	movs r3, #117
	strb r3, [r2, r6]
	lsls r1, r6, #2
	mov r3, r8
	adds r7, r3, r1
	ldr r3, [r7]
	ldr r2, .L_0200a010
.L_02009cda:
	orrs r3, r2
	str r3, [r7]
	adds r7, r5, r1
	ldr r3, [r7]
	orrs r3, r2
	str r3, [r7]
	b .L_0200a02c
.L_02009ce8:
	lsls r3, r4, #7
	adds r1, r1, r3
	mov r12, r1
	mov r6, r12
	subs r6, #129
	lsls r2, r6, #2
	movs r3, #118
	mov r0, r10
	mov r4, r9
	mov r1, r8
	strb r3, [r4, r6]
	adds r7, r1, r2
	strb r3, [r0, r6]
	ldr r3, [r7]
	movs r4, #162
	lsls r4, r4, #15
	orrs r3, r4
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	movs r1, #117
	adds r6, #1
	orrs r3, r4
	mov r2, r9
	str r3, [r7]
	strb r1, [r2, r6]
	mov r3, r8
	lsls r2, r6, #2
	strb r1, [r0, r6]
	adds r7, r3, r2
	ldr r3, [r7]
	ldr r0, .L_0200a014
	adds r6, #1
	orrs r3, r0
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r0
	str r3, [r7]
	movs r3, #119
	strb r3, [r2, r6]
	mov r2, r10
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	adds r6, #126
	orrs r3, r4
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r4
	str r3, [r7]
	mov r3, r10
	strb r1, [r2, r6]
	strb r1, [r3, r6]
	lsls r2, r6, #2
	mov r6, r8
	adds r7, r6, r2
	ldr r3, [r7]
	mov r6, r12
	orrs r3, r0
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r0
	str r3, [r7]
	mov r3, r10
	strb r1, [r2, r6]
	strb r1, [r3, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	adds r6, #1
	orrs r3, r4
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r4
	str r3, [r7]
	mov r3, r10
	strb r1, [r2, r6]
	strb r1, [r3, r6]
	lsls r2, r6, #2
	mov r6, r8
	adds r7, r6, r2
	ldr r3, [r7]
	mov r6, r12
	orrs r3, r0
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	adds r6, #127
	orrs r3, r0
	str r3, [r7]
	mov r2, r9
	movs r3, #120
	strb r3, [r2, r6]
	mov r2, r10
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	adds r6, #1
	orrs r3, r4
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r4
	str r3, [r7]
	mov r3, r10
	strb r1, [r2, r6]
	strb r1, [r3, r6]
	lsls r2, r6, #2
	mov r6, r8
	adds r7, r6, r2
	ldr r3, [r7]
	mov r6, r12
	orrs r3, r0
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	adds r6, #129
	orrs r3, r0
	str r3, [r7]
	mov r0, r9
	movs r3, #121
	mov r1, r10
	strb r3, [r0, r6]
	lsls r2, r6, #2
	strb r3, [r1, r6]
	mov r3, r8
	b .L_02009fec
.L_02009e02:
	lsls r4, r4, #7
	adds r4, r1, r4
	subs r6, r4, #2
	movs r0, #127
	mov r1, r9
	mov r2, r10
	strb r0, [r1, r6]
	mov r3, r8
	strb r0, [r2, r6]
	lsls r2, r6, #2
	adds r7, r3, r2
	ldr r3, [r7]
	movs r6, #168
	lsls r6, r6, #15
	orrs r3, r6
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov lr, r6
	orrs r3, r6
	str r3, [r7]
	subs r6, r4, #1
	movs r3, #122
	strb r3, [r1, r6]
	lsls r2, r6, #2
	mov r12, r0
	mov r1, r8
	mov r0, r10
	strb r3, [r0, r6]
	adds r7, r1, r2
	ldr r3, [r7]
	ldr r0, .L_0200a018
	adds r6, r4, #0
	orrs r3, r0
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r0
	str r3, [r7]
	mov r1, r10
	movs r3, #117
	strb r3, [r2, r6]
	strb r3, [r1, r6]
	mov r2, r8
	lsls r1, r6, #2
	adds r7, r2, r1
	ldr r3, [r7]
	ldr r2, .L_0200a01c
	adds r6, #1
	orrs r3, r2
	str r3, [r7]
	adds r7, r5, r1
	ldr r3, [r7]
	mov r1, r9
	orrs r3, r2
	str r3, [r7]
	mov r2, r10
	movs r3, #123
	strb r3, [r1, r6]
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	adds r6, r4, #2
	orrs r3, r0
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r4, r12
	orrs r3, r0
	str r3, [r7]
	lsls r2, r6, #2
	strb r4, [r1, r6]
	mov r0, r10
	mov r1, r8
	strb r4, [r0, r6]
	adds r7, r1, r2
	ldr r3, [r7]
	mov r4, lr
	b .L_02009ff0
.L_02009ea6:
	lsls r3, r4, #7
	adds r0, r1, r3
	ldr r1, .L_0200a020
	movs r3, #127
	adds r6, r0, r1
	mov r4, r10
	mov r2, r9
	strb r3, [r2, r6]
	lsls r1, r6, #2
	strb r3, [r4, r6]
	mov r6, r8
	adds r7, r6, r1
	ldr r3, [r7]
	movs r2, #168
	lsls r2, r2, #15
	orrs r3, r2
	str r3, [r7]
	adds r7, r5, r1
	ldr r3, [r7]
	adds r6, r0, #0
	orrs r3, r2
	str r3, [r7]
	subs r6, #128
	movs r3, #124
	mov r1, r9
	strb r3, [r1, r6]
	lsls r2, r6, #2
	strb r3, [r4, r6]
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	ldr r1, .L_0200a024
	adds r6, r0, #0
	orrs r3, r1
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r4, r9
	orrs r3, r1
	str r3, [r7]
	mov r2, r10
	movs r3, #117
	strb r3, [r4, r6]
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	adds r6, #128
	orrs r3, r1
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	lsls r2, r6, #2
	orrs r3, r1
	str r3, [r7]
	movs r3, #125
	strb r3, [r4, r6]
	mov r4, r10
	strb r3, [r4, r6]
	mov r6, r8
	adds r7, r6, r2
	ldr r3, [r7]
	orrs r3, r1
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r1
	movs r1, #128
	lsls r1, r1, #1
	str r3, [r7]
	adds r6, r0, r1
	movs r3, #126
	strb r3, [r2, r6]
	lsls r1, r6, #2
	strb r3, [r4, r6]
	mov r3, r8
	adds r7, r3, r1
	ldr r3, [r7]
	ldr r2, .L_0200a028
	b .L_02009cda
.L_02009f4a:
	lsls r3, r4, #7
	ldr r6, .L_0200a020
	adds r1, r1, r3
	mov r12, r1
	add r6, r12
	movs r0, #127
	lsls r2, r6, #2
	mov r4, r9
	mov r1, r10
	mov r3, r8
	strb r0, [r4, r6]
	adds r7, r3, r2
	strb r0, [r1, r6]
	ldr r3, [r7]
	movs r4, #168
	lsls r4, r4, #15
	orrs r3, r4
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r6, r12
	orrs r3, r4
	str r3, [r7]
	subs r6, #128
	movs r3, #124
	mov r1, r9
	mov r2, r10
	strb r3, [r1, r6]
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	ldr r1, .L_0200a024
	mov r6, r12
	orrs r3, r1
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r1
	str r3, [r7]
	movs r3, #117
	strb r3, [r2, r6]
	mov r2, r10
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	adds r6, #128
	orrs r3, r1
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r1
	str r3, [r7]
	movs r3, #125
	strb r3, [r2, r6]
	mov r2, r10
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	movs r6, #128
	orrs r3, r1
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	lsls r6, r6, #1
	orrs r3, r1
	add r6, r12
	mov r2, r10
	mov r1, r9
	str r3, [r7]
	strb r0, [r1, r6]
	mov r3, r8
	strb r0, [r2, r6]
	lsls r2, r6, #2
.L_02009fec:
	adds r7, r3, r2
	ldr r3, [r7]
.L_02009ff0:
	orrs r3, r4
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	movs r2, #1
	orrs r3, r4
	str r3, [r7]
	b .L_0200a02e
.L_0200a000:
	.4byte Data_0202c001 + 0x1cf
.L_0200a004:
	.4byte gData_02005910
.L_0200a008:
	.4byte 0xfffffe9b
.L_0200a00c:
	.4byte .L_02009ca8
.L_0200a010:
	.4byte 0x80500000
.L_0200a014:
	.4byte 0x80510000
.L_0200a018:
	.4byte 0x00529000
.L_0200a01c:
	.4byte 0x80526000
.L_0200a020:
	.4byte 0xffffff00
.L_0200a024:
	.4byte 0x00539000
.L_0200a028:
	.4byte 0x80550000
.L_0200a02c:
	movs r2, #1
.L_0200a02e:
	cmp r2, #0
	beq .L_0200a04a
	mov r2, r11
	movs r3, #0
	adds r2, #89
	strb r3, [r2]
	subs r2, #4
	strb r3, [r2]
	subs r2, #50
	strb r3, [r2]
	mov r0, r11
	movs r1, #0
	bl Actor_SetSingleChildFlags
.L_0200a04a:
	ldr r4, [sp, #0]
	adds r4, #1
	str r4, [sp, #0]
	cmp r4, #63
	bgt .L_0200a056
	b .L_02009c62
.L_0200a056:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a070
	bl Task_Add
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a070:
	.4byte Func_02001b98
	.section .text.x0200a0f0,"ax",%progbits
	.balign 4
	.global Func_020020f0
	.thumb_func
Func_020020f0:
	push {r5, r6, lr}
	ldr r3, .L_0200a2b0
	adds r5, r0, #0
	ldr r6, .L_0200a2b4
	str r5, [r3]
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a2b8
	sub sp, #8
	cmp r2, r3
	bne .L_0200a172
	movs r0, #14
	adds r1, r5, #0
	movs r2, #0
	bl Func_020020b0
	movs r0, #21
	adds r1, r5, #0
	movs r2, #1
	bl Func_020020b0
	adds r1, r5, #0
	movs r2, #2
	movs r0, #26
	bl Func_020020b0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrh r3, [r3]
	movs r1, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r1, r1, #9
	cmp r3, r1
	bls .L_0200a148
	movs r0, #27
	adds r1, r5, #0
	movs r2, #3
	bl Func_020020b0
.L_0200a148:
	cmp r5, #0
	bne .L_0200a15a
	movs r3, #23
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #23
	movs r1, #21
	b .L_0200a284
.L_0200a15a:
	movs r2, #1
	negs r2, r2
	cmp r5, r2
	beq .L_0200a164
	b .L_0200a2aa
.L_0200a164:
	movs r3, #23
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #24
	movs r1, #21
	b .L_0200a284
.L_0200a172:
	ldr r3, .L_0200a2bc
	cmp r2, r3
	bne .L_0200a1a4
	movs r0, #8
	adds r1, r5, #0
	movs r2, #0
	bl Func_020020b0
	movs r0, #15
	adds r1, r5, #0
	movs r2, #1
	bl Func_020020b0
	movs r0, #20
	adds r1, r5, #0
	movs r2, #2
	bl Func_020020b0
	movs r0, #25
	adds r1, r5, #0
	movs r2, #3
	bl Func_020020b0
	movs r0, #29
	b .L_0200a1d4
.L_0200a1a4:
	ldr r3, .L_0200a2c0
	cmp r2, r3
	bne .L_0200a1de
	movs r0, #8
	adds r1, r5, #0
	movs r2, #0
	bl Func_020020b0
	movs r0, #21
	adds r1, r5, #0
	movs r2, #1
	bl Func_020020b0
	movs r0, #29
	adds r1, r5, #0
	movs r2, #2
	bl Func_020020b0
	movs r0, #35
	adds r1, r5, #0
	movs r2, #3
	bl Func_020020b0
	movs r0, #38
.L_0200a1d4:
	adds r1, r5, #0
	movs r2, #4
	bl Func_020020b0
	b .L_0200a2aa
.L_0200a1de:
	ldr r3, .L_0200a2c4
	cmp r2, r3
	bne .L_0200a220
	movs r0, #10
	adds r1, r5, #0
	movs r2, #0
	bl Func_020020b0
	movs r0, #12
	adds r1, r5, #0
	movs r2, #1
	bl Func_020020b0
	cmp r5, #0
	bne .L_0200a20a
	movs r3, #27
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #34
	movs r1, #12
	b .L_0200a284
.L_0200a20a:
	movs r3, #1
	negs r3, r3
	cmp r5, r3
	bne .L_0200a2aa
	movs r3, #27
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #12
	b .L_0200a284
.L_0200a220:
	ldr r3, .L_0200a2c8
	cmp r2, r3
	bne .L_0200a23c
	movs r1, #5
	movs r0, #13
	bl Actor_SetModeById
	movs r0, #13
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	b .L_0200a2aa
.L_0200a23c:
	ldr r3, .L_0200a2cc
	cmp r2, r3
	bne .L_0200a2aa
	movs r0, #8
	adds r1, r5, #0
	movs r2, #0
	bl Func_020020b0
	movs r0, #9
	adds r1, r5, #0
	movs r2, #1
	bl Func_020020b0
	movs r0, #10
	adds r1, r5, #0
	movs r2, #2
	bl Func_020020b0
	movs r0, #11
	adds r1, r5, #0
	movs r2, #3
	bl Func_020020b0
	movs r0, #12
	adds r1, r5, #0
	movs r2, #4
	bl Func_020020b0
	cmp r5, #0
	bne .L_0200a28e
	movs r3, #9
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
.L_0200a284:
	movs r2, #1
	movs r3, #3
	bl Func_02005674
	b .L_0200a2aa
.L_0200a28e:
	movs r1, #1
	negs r1, r1
	cmp r5, r1
	bne .L_0200a2aa
	movs r3, #9
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #1
	movs r1, #0
	movs r2, #1
	movs r3, #3
	bl Func_02005674
.L_0200a2aa:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a2b0:
	.4byte gData_02005af0
.L_0200a2b4:
	.4byte gMainRoutineWork
.L_0200a2b8:
	.4byte 0x00000074
.L_0200a2bc:
	.4byte 0x00000075
.L_0200a2c0:
	.4byte 0x00000076
.L_0200a2c4:
	.4byte 0x00000077
.L_0200a2c8:
	.4byte 0x00000079
.L_0200a2cc:
	.4byte 0x0000007b
	.section .text.x0200a394,"ax",%progbits
	.balign 4
	.global Func_02002394
	.thumb_func
Func_02002394:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a528
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Actor_GetById
	adds r7, r0, #0
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_0200564c
	movs r0, #1
	bl wait
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r7, #0
	str r3, [r7, #72]
	adds r5, #85
	movs r3, #0
	str r3, [r7, #68]
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r4, #214
	lsls r4, r4, #1
	movs r2, #128
	adds r3, r3, r4
	lsls r2, r2, #1
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #204
	bl Func_02005854
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl FieldEvent_Wait
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200a52c
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200a42e:
	mov r4, r10
	lsls r5, r4, #12
	adds r0, r5, #0
	bl Math_Cosine
	add r6, sp, #16
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl Math_Sine
	ldr r3, [r6]
	str r0, [r6, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200a530
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200a534
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r4, [sp, #0]
	ldr r4, .L_0200a538
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_020000b8
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200a42e
	movs r0, #188
	bl Func_02005854
	ldr r5, .L_0200a528
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02005794
	ldr r0, [r5]
	movs r1, #49
	bl Actor_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005694
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02005694
	bl Func_0200569c
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02005794
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl wait
	ldr r0, [r5]
	movs r1, #1
	bl Actor_SetModeById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Func_020056bc
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a528:
	.4byte gMainRoutineWork
.L_0200a52c:
	.4byte Effect_AdvanceMotion
.L_0200a530:
	.4byte 0xffffa000
.L_0200a534:
	.4byte 0xffffd000
.L_0200a538:
	.4byte 0x01090001
	.section .text.x0200aa64,"ax",%progbits
	.balign 4
	.global Func_02002a64
	.thumb_func
Func_02002a64:
	push {r5, r6, lr}
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	ldr r3, .L_0200ac1c
	movs r2, #133
	lsls r2, r2, #2
	movs r5, #192
	adds r6, r3, r2
	lsls r5, r5, #8
	movs r1, #174
	movs r2, #156
	adds r3, r5, #0
	ldr r0, [r6]
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200571c
	movs r1, #165
	movs r2, #156
	adds r3, r5, #0
	movs r0, #7
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200571c
	movs r3, #176
	movs r1, #216
	movs r2, #156
	lsls r3, r3, #8
	movs r0, #23
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200571c
	movs r1, #224
	movs r2, #156
	adds r3, r5, #0
	movs r0, #6
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200571c
	movs r1, #234
	movs r2, #156
	lsls r1, r1, #17
	adds r3, r5, #0
	lsls r2, r2, #17
	movs r0, #5
	movs r5, #192
	bl Func_0200571c
	lsls r5, r5, #18
	movs r0, #1
	bl wait
	ldr r3, [r5, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #23
	bl Func_0200578c
	ldr r0, .L_0200ac20
	bl Func_02005754
	movs r1, #0
	movs r0, #23
	bl Func_02005764
	bl Func_020057bc
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #200
	movs r0, #204
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_020057a4
	movs r0, #196
	movs r1, #1
	movs r2, #146
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	bl Func_020057b4
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r2, #0
	movs r0, #7
	movs r1, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_02005764
	movs r0, #192
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #5
	bl FieldMessage_ShowAtActor
	ldr r0, [r6]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200ac24
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #7
	bl Func_0200578c
	movs r1, #128
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Actor_SetSpeedParameters
	movs r1, #170
	movs r2, #148
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #177
	movs r2, #148
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #182
	movs r2, #154
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #224
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	bl Func_02005774
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02005764
	ldr r2, [r5, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200ac94
	.2byte 0x0000
.L_0200ac1c:
	.4byte gMainRoutineWork
.L_0200ac20:
	.4byte MsgGabombaUnderstandsChannel
.L_0200ac24:
	ldr r3, [r5, #108]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	movs r1, #128
	adds r2, #1
	strh r2, [r3]
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Actor_SetSpeedParameters
	movs r1, #170
	movs r2, #148
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #177
	movs r2, #148
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #182
	movs r2, #154
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #224
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	bl Func_02005774
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02005764
.L_0200ac94:
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #23
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #23
	movs r1, #0
	bl Func_02005764
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #7
	bl Func_0200578c
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #23
	movs r1, #0
	bl Func_02005764
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #23
	bl Func_0200578c
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #23
	movs r1, #0
	bl Func_02005764
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02005764
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #7
	bl Func_0200578c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02005764
	movs r0, #23
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #23
	movs r1, #0
	bl Func_02005764
	movs r1, #10
	movs r2, #10
	adds r1, #255
	movs r0, #7
	bl Func_0200578c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02005764
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #6
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r1, #220
	movs r2, #148
	lsls r2, r2, #1
	movs r0, #6
	lsls r1, r1, #1
	bl Actor_SetPositionAndReset
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_02005764
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #7
	bl Func_0200578c
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	ldr r5, .L_0200af60
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #0
	movs r2, #80
	bl Actor_ArmCallback
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #7
	bl Func_0200578c
	movs r0, #7
	movs r1, #0
	bl Func_02005774
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02005764
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02005774
	ldr r0, [r5]
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #23
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #176
	movs r0, #23
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #23
	bl Func_0200578c
	movs r1, #208
	movs r2, #20
	movs r0, #23
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r1, #176
	movs r0, #23
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200af64
	adds r1, #153
	bl Func_020057a4
	movs r0, #196
	movs r1, #1
	movs r2, #176
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl Camera_MoveWithinMapBounds
	bl Func_020057b4
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r0, #6
	movs r1, #0
	bl Func_02005764
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r0, #7
	movs r1, #0
	bl Func_02005764
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02005764
	movs r0, #196
	movs r1, #1
	movs r2, #146
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #17
	negs r1, r1
	bl Camera_MoveWithinMapBounds
	bl Func_020057b4
	movs r0, #23
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200af68
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200af68
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #23
	ldr r1, .L_0200af68
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #7
	adds r1, #204
	bl Actor_SetSpeedParameters
	ldr r5, .L_0200af6c
	movs r0, #7
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #23
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #6
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #5
	bl Actor_SetMovementTypeAndWaitById
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #246
	bl GameFlag_SetBit
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_0200581c
	movs r0, #13
	bl Func_020057c4
	pop {r5, r6, pc}
.L_0200af60:
	.4byte gMainRoutineWork
.L_0200af64:
	.4byte 0x0004cccc
.L_0200af68:
	.4byte 0x00019999
.L_0200af6c:
	.4byte gData_02005c40
	.section .text.x0200b974,"ax",%progbits
	.balign 4
	.global Func_02003974
	.thumb_func
Func_02003974:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r7, .L_0200ba60
	movs r0, #0
	sub sp, #8
	mov r10, r0
.L_0200b984:
	movs r1, #0
	mov r8, r1
.L_0200b988:
	movs r3, #11
	mov r2, r10
	muls r2, r3
	adds r3, r2, #0
	add r3, r8
	movs r0, #128
	lsls r3, r3, #2
	lsls r0, r0, #4
	adds r5, r7, r3
	adds r0, #251
	ldrb r6, [r5, #3]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b9b2
	ldrb r3, [r5, #3]
	cmp r3, #2
	bne .L_0200b9b2
	movs r3, #1
	strb r3, [r5, #3]
	movs r6, #1
.L_0200b9b2:
	cmp r6, #2
	bne .L_0200b9d6
	movs r0, #182
	lsls r0, r0, #1
	adds r3, r7, r0
	adds r0, #2
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r7, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	add r2, r8
	add r3, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #11
	b .L_0200ba06
.L_0200b9d6:
	cmp r6, #1
	bne .L_0200ba10
	movs r3, #11
	mov r2, r10
	muls r2, r3
	adds r3, r2, #0
	add r3, r8
	movs r0, #182
	lsls r0, r0, #1
	lsls r3, r3, #2
	ldrb r1, [r7, r3]
	adds r3, r7, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r0, #183
	lsls r0, r0, #1
	adds r3, r7, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	add r2, r8
	add r3, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #1
.L_0200ba06:
	movs r2, #1
	movs r3, #1
	bl Func_0200566c
	b .L_0200ba44
.L_0200ba10:
	movs r3, #11
	mov r1, r10
	muls r1, r3
	adds r3, r1, #0
	add r3, r8
	movs r2, #182
	lsls r3, r3, #2
	lsls r2, r2, #1
	ldrb r1, [r7, r3]
	adds r3, r7, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r0, #183
	lsls r0, r0, #1
	adds r3, r7, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	add r2, r8
	add r3, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r6, #0
	movs r2, #1
	movs r3, #1
	bl Func_0200566c
.L_0200ba44:
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #10
	bls .L_0200b988
	add r10, r1
	mov r3, r10
	cmp r3, #7
	bls .L_0200b984
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200ba60:
	.4byte gSceneState
	.section .text.x0200ba64,"ax",%progbits
	.balign 4
	.global Func_02003a64
	.thumb_func
Func_02003a64:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200bdec
	movs r3, #176
	mov r11, r1
	lsls r3, r3, #1
	add r3, r11
	subs r0, #1
	str r0, [r3]
	sub sp, #24
.L_0200ba82:
	movs r1, #176
	lsls r1, r1, #1
	mov r3, r11
	adds r2, r3, r1
	ldr r3, [r2]
	mov r0, r11
	adds r3, #1
	str r3, [r2]
	ldr r2, .L_0200bdf0
	movs r7, #0
	str r3, [r2]
	ldr r3, .L_0200bdf4
	mov lr, r3
	.2byte 0xf800
	mov r3, r11
	movs r2, #10
	adds r3, #152
	strb r2, [r3]
	movs r3, #156
	lsls r3, r3, #1
	add r3, r11
	strb r2, [r3]
	movs r3, #172
	lsls r3, r3, #1
	add r3, r11
	strb r2, [r3]
	movs r2, #54
	adds r2, #255
	movs r3, #1
	add r2, r11
	strb r3, [r2]
	movs r2, #62
	adds r2, #255
	add r2, r11
	strb r3, [r2]
	movs r2, #86
	adds r2, #255
	add r2, r11
	strb r3, [r2]
	movs r2, #94
	adds r2, #255
	add r2, r11
	strb r3, [r2]
.L_0200bad8:
	movs r4, #0
	mov r8, r4
.L_0200badc:
	movs r3, #11
	muls r3, r7
	add r3, r8
	mov r0, r11
	lsls r3, r3, #2
	ldrb r1, [r0, r3]
	movs r3, #182
	lsls r3, r3, #1
	add r3, r11
	movs r4, #0
	ldrsh r2, [r3, r4]
	movs r3, #183
	lsls r3, r3, #1
	add r3, r11
	movs r0, #0
	ldrsh r3, [r3, r0]
	add r2, r8
	adds r3, r3, r7
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r0, #0
	movs r3, #1
	bl Func_0200566c
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #10
	ble .L_0200badc
	adds r7, #1
	cmp r7, #7
	ble .L_0200bad8
	movs r3, #0
	mov r9, r3
	bl Random16Far
	movs r4, #128
	lsls r4, r4, #8
	cmp r0, r4
	bcs .L_0200bbb8
	movs r0, #4
	mov r2, r9
	movs r1, #0
	str r0, [sp, #0]
	str r2, [sp, #4]
	movs r5, #2
	movs r6, #3
	mov r8, r0
	mov r10, r1
	movs r2, #5
	movs r1, #7
	movs r3, #7
	movs r0, #2
	str r6, [sp, #8]
	str r5, [sp, #12]
	bl Func_02003fc0
	mov r3, r8
	str r3, [sp, #0]
	mov r9, r0
	movs r1, #7
	movs r2, #6
	movs r3, #6
	movs r0, #8
	str r5, [sp, #4]
	str r5, [sp, #8]
	str r6, [sp, #12]
	bl Func_02003fc0
	mov r4, r9
	orrs r4, r0
	movs r1, #6
	movs r0, #5
	movs r2, #9
	movs r3, #12
	mov r9, r4
	bl Func_02004270
	movs r3, #12
	mov r0, r10
	str r3, [sp, #0]
	str r0, [sp, #8]
	movs r1, #6
	movs r2, #4
	movs r3, #3
	movs r0, #4
	str r5, [sp, #4]
	str r5, [sp, #12]
	bl Func_02003fc0
	mov r1, r9
	movs r3, #14
	orrs r1, r0
	str r3, [sp, #0]
	movs r3, #1
	mov r9, r1
	str r3, [sp, #8]
	movs r2, #5
	movs r0, #5
	movs r1, #5
	movs r3, #2
	str r6, [sp, #4]
	str r6, [sp, #12]
	bl Func_02003fc0
	mov r2, r9
	orrs r2, r0
	mov r9, r2
	b .L_0200bc3e
.L_0200bbb8:
	movs r3, #0
	mov r4, r9
	movs r5, #4
	mov r8, r3
	movs r6, #2
	movs r1, #7
	movs r2, #4
	movs r3, #6
	movs r0, #2
	str r5, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #8]
	str r6, [sp, #12]
	bl Func_02003fc0
	movs r1, #7
	str r5, [sp, #0]
	mov r9, r0
	movs r5, #3
	movs r2, #5
	movs r3, #7
	movs r0, #8
	str r6, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl Func_02003fc0
	mov r1, r9
	orrs r1, r0
	mov r9, r1
	movs r0, #5
	movs r1, #6
	movs r2, #8
	movs r3, #12
	bl Func_02004270
	movs r3, #12
	mov r2, r8
	str r3, [sp, #0]
	str r2, [sp, #8]
	movs r1, #5
	movs r2, #4
	movs r3, #3
	movs r0, #5
	str r5, [sp, #4]
	str r6, [sp, #12]
	bl Func_02003fc0
	mov r3, r9
	orrs r3, r0
	mov r9, r3
	movs r3, #14
	str r3, [sp, #0]
	movs r3, #1
	mov r4, r8
	str r3, [sp, #8]
	movs r1, #6
	movs r0, #6
	movs r2, #5
	movs r3, #2
	str r4, [sp, #4]
	str r5, [sp, #12]
	bl Func_02003fc0
	mov r1, r9
	orrs r1, r0
	mov r9, r1
.L_0200bc3e:
	mov r2, r9
	cmp r2, #0
	beq .L_0200bc46
	b .L_0200ba82
.L_0200bc46:
	movs r3, #19
	str r3, [sp, #0]
	movs r3, #3
	str r3, [sp, #8]
	str r2, [sp, #12]
	movs r5, #2
	movs r1, #7
	movs r2, #5
	movs r3, #4
	movs r0, #0
	str r5, [sp, #4]
	bl Func_02003fc0
	movs r3, #17
	str r3, [sp, #0]
	movs r3, #1
	mov r9, r0
	str r3, [sp, #12]
	movs r6, #0
	movs r3, #3
	movs r0, #10
	movs r1, #7
	movs r2, #6
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl Func_02003fc0
	mov r3, r9
	orrs r3, r0
	cmp r3, #0
	beq .L_0200bc86
	b .L_0200ba82
.L_0200bc86:
	movs r2, #0
	movs r1, #0
	movs r0, #0
	movs r7, #0
.L_0200bc8e:
	movs r4, #0
	mov r8, r4
.L_0200bc92:
	movs r3, #11
	muls r3, r7
	add r3, r8
	lsls r3, r3, #2
	mov r4, r11
	ldrb r5, [r4, r3]
	add r3, r11
	ldrb r6, [r3, #2]
	cmp r6, #5
	bne .L_0200bca8
	adds r2, #1
.L_0200bca8:
	cmp r6, #10
	bne .L_0200bcae
	adds r1, #1
.L_0200bcae:
	cmp r6, #12
	bne .L_0200bcbc
	adds r3, r5, #0
	subs r3, #8
	cmp r3, #1
	bhi .L_0200bcbc
	adds r0, #1
.L_0200bcbc:
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #10
	ble .L_0200bc92
	adds r7, #1
	cmp r7, #7
	ble .L_0200bc8e
	cmp r2, #0
	bne .L_0200bcd2
	b .L_0200ba82
.L_0200bcd2:
	cmp r1, #0
	bne .L_0200bcd8
	b .L_0200ba82
.L_0200bcd8:
	cmp r0, #0
	bne .L_0200bcde
	b .L_0200ba82
.L_0200bcde:
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	mov r10, r0
	movs r1, #1
	movs r0, #7
	str r0, [sp, #16]
	str r1, [sp, #20]
.L_0200bcf0:
	bl Random16Far
	movs r5, #11
	adds r3, r0, #0
	muls r3, r5
	lsrs r3, r3, #16
	mov r8, r3
	bl Random16Far
	lsls r3, r0, #1
	adds r3, r3, r0
	lsrs r3, r3, #16
	adds r7, r3, #5
	mov r0, r8
	adds r1, r7, #0
	bl Func_02003934
	cmp r0, #0
	bne .L_0200bd64
	adds r3, r7, #0
	muls r3, r5
	add r3, r8
	lsls r2, r3, #2
	mov r3, r11
	ldrb r5, [r3, r2]
	subs r3, r5, #1
	cmp r3, #1
	bhi .L_0200bd64
	mov r4, r11
	adds r3, r4, r2
	mov r0, r10
	ldrb r6, [r3, #2]
	cmp r0, #0
	bne .L_0200bd3c
	movs r3, #6
	ands r3, r6
	cmp r3, #0
	bne .L_0200bd4a
.L_0200bd3c:
	mov r1, r10
	cmp r1, #1
	bne .L_0200bd64
	movs r3, #9
	ands r3, r6
	cmp r3, #0
	beq .L_0200bd64
.L_0200bd4a:
	movs r3, #11
	muls r3, r7
	add r3, r8
	lsls r3, r3, #2
	movs r2, #10
	mov r4, r11
	strb r2, [r4, r3]
	movs r0, #6
	add r3, r11
	movs r2, #1
	strb r2, [r3, #3]
	str r0, [sp, #16]
	b .L_0200bd6e
.L_0200bd64:
	ldr r1, [sp, #20]
	adds r1, #1
	str r1, [sp, #20]
	cmp r1, #88
	ble .L_0200bcf0
.L_0200bd6e:
	movs r2, #3
	mov r9, r2
.L_0200bd72:
	movs r3, #1
	str r3, [sp, #20]
.L_0200bd76:
	bl Random16Far
	movs r5, #11
	adds r3, r0, #0
	muls r3, r5
	lsrs r3, r3, #16
	mov r8, r3
	bl Random16Far
	lsls r0, r0, #3
	lsrs r7, r0, #16
	adds r1, r7, #0
	mov r0, r8
	bl Func_02003934
	cmp r0, #0
	bne .L_0200be30
	adds r3, r7, #0
	muls r3, r5
	add r3, r8
	lsls r3, r3, #2
	mov r4, r11
	ldrb r5, [r4, r3]
	cmp r5, #3
	beq .L_0200bdb0
	cmp r5, #8
	beq .L_0200bdb0
	cmp r5, #9
	bne .L_0200be30
.L_0200bdb0:
	movs r3, #11
	muls r3, r7
	add r3, r8
	lsls r3, r3, #2
	add r3, r11
	ldrb r6, [r3, #2]
	mov r10, r3
	cmp r6, #6
	beq .L_0200be30
	cmp r6, #9
	beq .L_0200be30
	cmp r5, #8
	beq .L_0200bdf8
	cmp r5, #8
	bgt .L_0200bdd4
	cmp r5, #3
	beq .L_0200bdda
	b .L_0200be1a
.L_0200bdd4:
	cmp r5, #9
	beq .L_0200be0a
	b .L_0200be1a
.L_0200bdda:
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	adds r0, #1
	mov r1, r10
	strb r0, [r1]
	b .L_0200be1a
	.2byte 0x0000
.L_0200bdec:
	.4byte gSceneState
.L_0200bdf0:
	.4byte gRandomState
.L_0200bdf4:
	.4byte IwramClearWords
.L_0200bdf8:
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	lsls r0, r0, #1
	adds r0, #4
	mov r2, r10
	strb r0, [r2]
	b .L_0200be1a
.L_0200be0a:
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	lsls r0, r0, #1
	adds r0, #5
	mov r3, r10
	strb r0, [r3]
.L_0200be1a:
	movs r3, #11
	muls r3, r7
	add r3, r8
	lsls r3, r3, #2
	add r3, r11
	movs r2, #1
	strb r2, [r3, #3]
	ldr r4, [sp, #16]
	subs r4, #1
	str r4, [sp, #16]
	b .L_0200be3a
.L_0200be30:
	ldr r0, [sp, #20]
	adds r0, #1
	str r0, [sp, #20]
	cmp r0, #88
	ble .L_0200bd76
.L_0200be3a:
	movs r1, #1
	negs r1, r1
	add r9, r1
	mov r2, r9
	cmp r2, #0
	bge .L_0200bd72
	movs r3, #1
	str r3, [sp, #20]
.L_0200be4a:
	bl Random16Far
	movs r5, #11
	adds r3, r0, #0
	muls r3, r5
	lsrs r3, r3, #16
	mov r8, r3
	bl Random16Far
	lsls r0, r0, #3
	lsrs r7, r0, #16
	adds r1, r7, #0
	mov r0, r8
	bl Func_02003934
	cmp r0, #0
	bne .L_0200be9e
	adds r3, r7, #0
	muls r3, r5
	add r3, r8
	lsls r3, r3, #2
	mov r4, r11
	ldrb r5, [r4, r3]
	cmp r5, #4
	beq .L_0200be88
	cmp r5, #6
	beq .L_0200be88
	cmp r5, #5
	beq .L_0200be88
	cmp r5, #7
	bne .L_0200be9e
.L_0200be88:
	movs r3, #11
	muls r3, r7
	add r3, r8
	lsls r3, r3, #2
	add r3, r11
	movs r2, #1
	strb r2, [r3, #3]
	ldr r0, [sp, #16]
	subs r0, #1
	str r0, [sp, #16]
	b .L_0200bea8
.L_0200be9e:
	ldr r1, [sp, #20]
	adds r1, #1
	str r1, [sp, #20]
	cmp r1, #88
	ble .L_0200be4a
.L_0200bea8:
	ldr r2, [sp, #16]
	cmp r2, #0
	ble .L_0200bf06
	mov r9, r2
.L_0200beb0:
	movs r3, #1
	str r3, [sp, #20]
.L_0200beb4:
	bl Random16Far
	movs r5, #11
	adds r3, r0, #0
	muls r3, r5
	lsrs r3, r3, #16
	mov r8, r3
	bl Random16Far
	lsls r0, r0, #3
	lsrs r7, r0, #16
	adds r1, r7, #0
	mov r0, r8
	bl Func_02003934
	cmp r0, #0
	bne .L_0200bef0
	adds r3, r7, #0
	muls r3, r5
	add r3, r8
	lsls r0, r3, #2
	mov r4, r11
	ldrb r5, [r4, r0]
	subs r3, r5, #1
	cmp r3, #1
	bhi .L_0200bef0
	adds r2, r4, r0
	movs r3, #1
	strb r3, [r2, #3]
	b .L_0200befa
.L_0200bef0:
	ldr r0, [sp, #20]
	adds r0, #1
	str r0, [sp, #20]
	cmp r0, #88
	ble .L_0200beb4
.L_0200befa:
	movs r1, #1
	negs r1, r1
	add r9, r1
	mov r2, r9
	cmp r2, #0
	bne .L_0200beb0
.L_0200bf06:
	movs r7, #0
.L_0200bf08:
	movs r3, #0
	mov r8, r3
.L_0200bf0c:
	movs r3, #11
	muls r3, r7
	add r3, r8
	lsls r3, r3, #2
	mov r4, r11
	ldrb r5, [r4, r3]
	add r3, r11
	ldrb r6, [r3, #3]
	cmp r6, #0
	beq .L_0200bf6e
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	cmp r0, #6
	bhi .L_0200bf6e
	ldr r2, .L_0200bfbc
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200bf34:
	.4byte .L_0200bf62
	.4byte .L_0200bf62
	.4byte .L_0200bf62
	.4byte .L_0200bf50
	.4byte .L_0200bf50
	.4byte .L_0200bf50
	.4byte .L_0200bf58
.L_0200bf50:
	mov r0, r8
	adds r1, r7, #0
	movs r2, #1
	b .L_0200bf68
.L_0200bf58:
	adds r1, r7, #0
	movs r2, #0
	mov r0, r8
	bl Func_02004380
.L_0200bf62:
	mov r0, r8
	adds r1, r7, #0
	movs r2, #0
.L_0200bf68:
	bl Func_02004380
	adds r5, r0, #0
.L_0200bf6e:
	movs r3, #182
	lsls r3, r3, #1
	add r3, r11
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r3, #183
	lsls r3, r3, #1
	add r3, r11
	movs r1, #0
	ldrsh r3, [r3, r1]
	add r2, r8
	adds r3, r3, r7
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0200566c
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #10
	ble .L_0200bf0c
	adds r7, #1
	cmp r7, #7
	ble .L_0200bf08
	movs r3, #176
	lsls r3, r3, #1
	add r3, r11
	ldr r0, [r3]
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bfbc:
	.4byte .L_0200bf34
	.section .text.x0200bfc0,"ax",%progbits
	.balign 4
	.global Func_02003fc0
	.thumb_func
Func_02003fc0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r2, [sp, #12]
	adds r7, r0, #0
	mov r8, r1
	ldr r0, [sp, #48]
	subs r2, r3, r1
	ldr r1, [sp, #12]
	subs r0, #1
	subs r6, r1, r7
	str r3, [sp, #8]
	str r0, [sp, #48]
	cmp r6, #0
	bge .L_0200bfea
	negs r6, r6
.L_0200bfea:
	ldr r0, [sp, #48]
	subs r3, r0, r6
	adds r6, r2, #0
	cmp r6, #0
	bge .L_0200bff6
	negs r6, r6
.L_0200bff6:
	ldr r5, .L_0200c1fc
	movs r1, #32
	ldr r0, .L_0200c200
	subs r6, r3, r6
	mov lr, r5
	.2byte 0xf800
	movs r1, #32
	ldr r0, .L_0200c204
	mov lr, r5
	.2byte 0xf800
	movs r1, #32
	ldr r0, .L_0200c208
	mov lr, r5
	.2byte 0xf800
	movs r1, #32
	ldr r0, .L_0200c20c
	mov lr, r5
	.2byte 0xf800
	movs r1, #32
	ldr r0, .L_0200c210
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_0200c214
	movs r1, #32
	mov lr, r5
	.2byte 0xf800
	movs r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_0200c038
.L_0200c032:
	movs r0, #1
	negs r0, r0
	b .L_0200c1ec
.L_0200c038:
	movs r5, #0
	str r5, [sp, #0]
	mov r9, r7
.L_0200c03e:
	ldr r1, [sp, #0]
	movs r2, #160
	adds r1, #1
	lsls r2, r2, #3
	str r1, [sp, #0]
	cmp r1, r2
	bhi .L_0200c032
	movs r3, #11
	mov r0, r8
	muls r0, r3
	adds r3, r0, #0
	ldr r0, .L_0200c218
	add r3, r9
	lsls r3, r3, #2
	ldrb r2, [r0, r3]
	ldr r1, .L_0200c20c
	adds r3, r0, r3
	strb r2, [r1, r5]
	ldrb r3, [r3, #2]
	ldr r2, .L_0200c214
	mov r0, r8
	strb r3, [r2, r5]
	ldr r3, .L_0200c204
	mov r2, r9
	strb r2, [r3, r5]
	ldr r3, .L_0200c208
	add r2, sp, #52
	strb r0, [r3, r5]
	ldrb r2, [r2]
	ldr r3, .L_0200c200
	ldr r0, .L_0200c210
	strb r2, [r3, r5]
	ldr r2, .L_0200c21c
	ldrb r3, [r1, r5]
	ldrb r1, [r0, r5]
	ldrb r3, [r2, r3]
	ldr r2, [sp, #52]
	orrs r1, r3
	adds r2, #2
	movs r3, #3
	ands r2, r3
	movs r3, #1
	lsls r3, r2
	orrs r1, r3
	movs r3, #0
	strb r1, [r0, r5]
	str r3, [sp, #4]
.L_0200c09c:
	ldr r0, [sp, #4]
	adds r0, #1
	str r0, [sp, #4]
	cmp r0, #63
	bhi .L_0200c188
	bl Random16Far
	movs r1, #254
	lsls r1, r1, #7
	adds r1, #255
	ldr r6, [sp, #52]
	cmp r0, r1
	bls .L_0200c0be
	bl Random16Far
	lsls r0, r0, #2
	lsrs r6, r0, #16
.L_0200c0be:
	ldr r2, .L_0200c210
	movs r1, #1
	ldrb r0, [r2, r5]
	adds r3, r0, #0
	asrs r3, r6
	ands r3, r1
	cmp r3, #0
	bne .L_0200c09c
	ldr r1, .L_0200c220
	lsls r3, r6, #1
	ldrsb r2, [r1, r3]
	adds r3, #1
	ldrsb r3, [r1, r3]
	add r2, r9
	add r3, r8
	mov r11, r2
	mov r10, r3
	cmp r2, #10
	bhi .L_0200c172
	cmp r3, #7
	bhi .L_0200c172
	ldr r3, [sp, #48]
	subs r3, #1
	cmp r5, r3
	bne .L_0200c0fc
	ldr r2, [sp, #12]
	cmp r11, r2
	bne .L_0200c172
	ldr r3, [sp, #8]
	cmp r10, r3
	bne .L_0200c172
.L_0200c0fc:
	movs r1, #11
	mov r3, r10
	muls r3, r1
	ldr r1, .L_0200c218
	add r3, r11
	lsls r3, r3, #2
	adds r2, r1, r3
	ldrb r3, [r2, #1]
	cmp r3, #0
	bne .L_0200c172
	ldrb r7, [r2, #2]
	ldr r3, [sp, #60]
	movs r1, #1
	asrs r7, r3
	ands r7, r1
	cmp r7, #0
	bne .L_0200c172
	ldrb r3, [r2]
	cmp r3, #10
	beq .L_0200c172
	ldr r2, .L_0200c210
	movs r3, #1
	lsls r3, r6
	orrs r0, r3
	strb r0, [r2, r5]
	ldr r3, .L_0200c20c
	ldr r0, [sp, #52]
	ldrb r2, [r3, r5]
	adds r1, r6, #0
	bl Func_02004224
	adds r4, r0, #0
	movs r0, #11
	mov r3, r8
	muls r3, r0
	ldr r1, .L_0200c218
	add r3, r9
	lsls r3, r3, #2
	ldr r0, [sp, #60]
	adds r3, r1, r3
	ldrb r3, [r3, #2]
	movs r2, #1
	lsls r2, r0
	mov r1, r8
	orrs r3, r2
	mov r0, r9
	adds r2, r4, #0
	bl Func_02004270
	ldr r1, [sp, #48]
	adds r5, #1
	mov r9, r11
	mov r8, r10
	str r6, [sp, #52]
	cmp r5, r1
	beq .L_0200c1ba
	ldr r2, .L_0200c210
	strb r7, [r2, r5]
	b .L_0200c03e
.L_0200c172:
	ldr r0, .L_0200c210
	movs r3, #1
	ldrb r2, [r0, r5]
	lsls r3, r6
	orrs r3, r2
	movs r1, #240
	strb r3, [r0, r5]
	lsls r1, r1, #20
	lsls r3, r3, #24
	cmp r3, r1
	bne .L_0200c09c
.L_0200c188:
	ldr r2, .L_0200c210
	movs r3, #0
	strb r3, [r2, r5]
	cmp r5, #0
	bne .L_0200c194
	b .L_0200c032
.L_0200c194:
	ldr r3, .L_0200c204
	subs r5, #1
	ldrb r3, [r3, r5]
	mov r9, r3
	ldr r3, .L_0200c208
	mov r0, r9
	ldrb r3, [r3, r5]
	mov r8, r3
	ldr r3, .L_0200c200
	mov r1, r8
	ldrb r3, [r3, r5]
	str r3, [sp, #52]
	ldr r3, .L_0200c20c
	ldrb r2, [r3, r5]
	ldr r3, .L_0200c214
	ldrb r3, [r3, r5]
	bl Func_02004270
	b .L_0200c03e
.L_0200c1ba:
	movs r3, #11
	mov r5, r10
	muls r5, r3
	ldr r3, .L_0200c218
	add r5, r11
	lsls r5, r5, #2
	ldrb r2, [r3, r5]
	ldr r1, [sp, #56]
	adds r0, r6, #0
	bl Func_02004224
	adds r4, r0, #0
	ldr r0, .L_0200c218
	ldr r1, [sp, #60]
	adds r5, r0, r5
	ldrb r3, [r5, #2]
	movs r2, #1
	lsls r2, r1
	orrs r3, r2
	mov r0, r11
	mov r1, r10
	adds r2, r4, #0
	bl Func_02004270
	movs r0, #0
.L_0200c1ec:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c1fc:
	.4byte IwramClearWords
.L_0200c200:
	.4byte Data_02006fc8
.L_0200c204:
	.4byte Data_02006fe8
.L_0200c208:
	.4byte Data_02007008
.L_0200c20c:
	.4byte Data_02007028
.L_0200c210:
	.4byte Data_02007048
.L_0200c214:
	.4byte gData_02007068
.L_0200c218:
	.4byte gSceneState
.L_0200c21c:
	.4byte gData_02005ac8 + 0x1
.L_0200c220:
	.4byte gData_02005ac0 + 0x1
	.section .text.x0200c288,"ax",%progbits
	.balign 4
	.global Func_02004288
	.thumb_func
Func_02004288:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r5, .L_0200c37c
	mov r10, r0
	movs r0, #184
	lsls r0, r0, #1
	mov r9, r1
	movs r2, #178
	adds r3, r5, r0
	mov r1, r9
	lsls r2, r2, #1
	strh r1, [r3]
	mov r4, r10
	adds r3, r5, r2
	strh r4, [r3]
	movs r0, #0
	sub sp, #8
	mov r8, r0
.L_0200c2b2:
	movs r7, #0
.L_0200c2b4:
	movs r3, #11
	mov r1, r8
	muls r1, r3
	adds r3, r1, #0
	adds r3, r3, r7
	lsls r3, r3, #2
	adds r6, r5, r3
	ldrb r3, [r6, #3]
	cmp r3, #0
	beq .L_0200c356
	mov r0, r10
	bl Actor_GetById
	mov r2, r9
	cmp r2, #0
	beq .L_0200c322
	movs r3, #182
	lsls r3, r3, #1
	adds r1, r5, r3
	movs r4, #0
	ldrsh r3, [r1, r4]
	movs r2, #128
	adds r3, r3, r7
	lsls r2, r2, #12
	lsls r3, r3, #20
	adds r3, r3, r2
	str r3, [r0, #8]
	movs r3, #183
	lsls r3, r3, #1
	adds r3, r3, r5
	mov r12, r3
	movs r4, #0
	ldrsh r3, [r3, r4]
	add r3, r8
	lsls r3, r3, #20
	adds r3, r3, r2
	str r3, [r0, #16]
	movs r4, #0
	ldrsh r3, [r1, r4]
	mov r1, r12
	movs r0, #0
	ldrsh r2, [r1, r0]
	adds r3, r3, r7
	add r2, r8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #0
	movs r1, #11
	movs r2, #1
	bl Func_0200566c
	movs r3, #2
	strb r3, [r6, #3]
	b .L_0200c352
.L_0200c322:
	mov r2, r9
	movs r3, #1
	str r2, [r0, #8]
	str r2, [r0, #16]
	movs r4, #182
	strb r3, [r6, #3]
	lsls r4, r4, #1
	adds r3, r5, r4
	adds r4, #2
	movs r0, #0
	ldrsh r2, [r3, r0]
	adds r3, r5, r4
	movs r0, #0
	ldrsh r3, [r3, r0]
	adds r2, r2, r7
	add r3, r8
	ldrb r1, [r6]
	movs r0, #1
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	bl Func_0200566c
.L_0200c352:
	movs r1, #1
	add r10, r1
.L_0200c356:
	adds r7, #1
	cmp r7, #10
	ble .L_0200c2b4
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #7
	ble .L_0200c2b2
	movs r4, #179
	lsls r4, r4, #1
	adds r3, r5, r4
	mov r0, r10
	strh r0, [r3]
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200c37c:
	.4byte gSceneState
	.section .text.x0200c3ec,"ax",%progbits
	.balign 4
	.global Func_020043ec
	.thumb_func
Func_020043ec:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	sub sp, #8
	cmp r6, #1
	bne .L_0200c4a6
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	ldr r7, .L_0200c4b0
	movs r1, #26
	ldrsh r0, [r3, r1]
	bl Actor_GetById
	movs r3, #182
	adds r5, r0, #0
	lsls r3, r3, #1
	ldr r4, [r5, #8]
	adds r2, r7, r3
	movs r1, #0
	ldrsh r3, [r2, r1]
	asrs r4, r4, #20
	subs r4, r4, r3
	movs r3, #183
	lsls r3, r3, #1
	ldr r0, [r5, #16]
	adds r3, r3, r7
	mov r12, r3
	movs r1, #0
	ldrsh r3, [r3, r1]
	asrs r0, r0, #20
	subs r0, r0, r3
	movs r3, #11
	muls r3, r0
	adds r3, r3, r4
	lsls r3, r3, #2
	ldrb r1, [r7, r3]
	adds r3, r3, r7
	mov lr, r3
	movs r3, #0
	mov r8, r3
	mov r3, lr
	strb r6, [r3, #3]
	movs r3, #0
	ldrsh r6, [r2, r3]
	mov r3, r12
	mov lr, r6
	movs r6, #0
	ldrsh r2, [r3, r6]
	mov r6, lr
	adds r3, r6, r4
	adds r2, r2, r0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #1
	movs r2, #1
	movs r3, #1
	bl Func_0200566c
	mov r1, r8
	str r1, [r5, #8]
	str r1, [r5, #16]
	movs r0, #0
.L_0200c46e:
	movs r3, #11
	muls r3, r1
	lsls r3, r3, #2
	adds r3, r7, r3
	ldrb r3, [r3, #3]
	movs r2, #0
	b .L_0200c48e
.L_0200c47c:
	adds r2, #1
	cmp r2, #10
	bhi .L_0200c494
	movs r3, #11
	muls r3, r1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r7, r3
	ldrb r3, [r3, #3]
.L_0200c48e:
	cmp r3, #2
	bne .L_0200c47c
	movs r0, #1
.L_0200c494:
	cmp r0, #0
	bne .L_0200c4a6
	adds r1, #1
	cmp r1, #7
	bls .L_0200c46e
	movs r2, #184
	lsls r2, r2, #1
	adds r3, r7, r2
	strh r0, [r3]
.L_0200c4a6:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c4b0:
	.4byte gSceneState
	.section .text.x0200c710,"ax",%progbits
	.balign 4
	.global Func_02004710
	.thumb_func
Func_02004710:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #0
	sub sp, #12
	str r0, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r6, .L_0200c894
	adds r3, #228
	movs r1, #188
	ldr r2, [r3]
	lsls r1, r1, #1
	adds r1, r1, r6
	mov r8, r1
	ldr r1, .L_0200c898
	mov r9, r2
	ldr r3, [r3, #4]
	mov r5, r9
	ands r5, r1
	mov r10, r3
	mov r9, r5
	movs r5, #180
	mov r0, r10
	lsls r5, r5, #1
	ands r0, r1
	adds r3, r6, r5
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	ldr r2, .L_0200c89c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	movs r2, #186
	lsrs r3, r3, #5
	lsls r2, r2, #1
	mov r11, r3
	adds r3, r6, r2
	ldr r2, [r3]
	cmp r2, #0
	beq .L_0200c79c
	add r0, sp, #4
	ldr r3, [r0, #4]
	lsls r2, r2, #16
	lsrs r2, r2, #16
	ands r3, r1
	orrs r3, r2
	str r3, [r0, #4]
	ldr r3, [sp, #4]
	movs r2, #128
	ands r3, r1
	lsls r2, r2, #1
	orrs r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #17
	orrs r3, r2
	str r3, [sp, #4]
	bl AffineMatrix_BuildForEffect
	str r0, [sp, #0]
.L_0200c79c:
	movs r5, #178
	lsls r5, r5, #1
	movs r1, #179
	adds r3, r6, r5
	lsls r1, r1, #1
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r7, r3
	bge .L_0200c884
.L_0200c7b4:
	adds r0, r7, #0
	bl Actor_GetById
	ldr r5, [r0, #8]
	cmp r5, #0
	beq .L_0200c874
	mov r3, r9
	subs r4, r5, r3
	ldr r3, [r0, #12]
	movs r1, #128
	ldr r0, [r0, #16]
	lsls r1, r1, #13
	adds r3, r3, r1
	mov r1, r10
	subs r2, r0, r1
	subs r1, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	asrs r2, r5, #20
	movs r5, #182
	lsls r5, r5, #1
	adds r3, #58
	mov r12, r3
	adds r3, r6, r5
	movs r5, #0
	ldrsh r3, [r3, r5]
	movs r5, #183
	lsls r5, r5, #1
	subs r2, r2, r3
	adds r3, r6, r5
	movs r5, #0
	ldrsh r3, [r3, r5]
	asrs r0, r0, #20
	subs r0, r0, r3
	movs r3, #11
	muls r3, r0
	adds r3, r3, r2
	lsls r3, r3, #2
	ldrb r2, [r6, r3]
	movs r3, #128
	asrs r4, r4, #16
	lsls r3, r3, #1
	adds r3, #255
	subs r4, #8
	asrs r1, r1, #16
	ands r4, r3
	subs r1, #8
	movs r3, #255
	mov r0, r8
	ands r1, r3
	movs r3, #0
	stmia r0!, {r3}
	ldr r3, .L_0200c8a0
	lsls r4, r4, #16
	orrs r1, r4
	orrs r1, r3
	stmia r0!, {r1}
	lsls r2, r2, #3
	movs r3, #128
	lsls r3, r3, #4
	add r2, r11
	orrs r2, r3
	str r2, [r0]
	movs r0, #186
	lsls r0, r0, #1
	adds r3, r6, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200c868
	mov r1, r8
	ldrb r3, [r1, #5]
	movs r5, #4
	negs r5, r5
	adds r2, r5, #0
	ands r3, r2
	movs r2, #1
	orrs r3, r2
	strb r3, [r1, #5]
	ldr r2, [sp, #0]
	movs r3, #31
	ands r2, r3
	movs r0, #63
	ldrb r3, [r1, #7]
	negs r0, r0
	adds r1, r0, #0
	ands r3, r1
	lsls r2, r2, #1
	orrs r3, r2
	mov r1, r8
	strb r3, [r1, #7]
.L_0200c868:
	mov r0, r8
	mov r1, r12
	bl Func_020055ec
	movs r2, #12
	add r8, r2
.L_0200c874:
	movs r5, #179
	lsls r5, r5, #1
	adds r3, r6, r5
	movs r0, #0
	ldrsh r3, [r3, r0]
	adds r7, #1
	cmp r7, r3
	blt .L_0200c7b4
.L_0200c884:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c894:
	.4byte gSceneState
.L_0200c898:
	.4byte 0xffff0000
.L_0200c89c:
	.4byte gResourceTableEntries
.L_0200c8a0:
	.4byte 0x40002000
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
	.section .text.x0200d3bc,"ax",%progbits
	.balign 4
	.global Func_020053bc
	.thumb_func
Func_020053bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_0200d420
	adds r7, r0, #0
.L_0200d3d2:
	ldrh r3, [r7]
	mov r8, r3
	mov r0, r8
	bl Actor_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #89
	movs r2, #2
	ldrsh r6, [r7, r2]
	ldrb r2, [r1]
	movs r3, #4
	ldrsh r4, [r7, r3]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r1, #0
	str r4, [sp, #0]
	bl Actor_SetSingleChildFlags
	ldr r4, [sp, #0]
	lsls r6, r6, #16
	lsls r4, r4, #16
	lsrs r4, r4, #16
	lsrs r6, r6, #16
	adds r5, #34
	ldrb r3, [r5]
	adds r2, r4, #0
	mov r0, r8
	adds r1, r6, #0
	adds r7, #6
	bl Func_020054ac
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200d3d2
.L_0200d420:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200d428,"ax",%progbits
	.balign 4
	.global Func_02005428
	.thumb_func
Func_02005428:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r5, r0, #0
	mov r9, r1
	mov r10, r2
	movs r1, #255
	ldr r2, [r3]
	b .L_0200d494
.L_0200d444:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_0200d490
	adds r0, r7, #0
	bl Actor_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_0200d46c
	ldr r3, [r6, #28]
	ldr r1, .L_0200d4a8
	adds r3, r3, r1
	str r3, [r6, #28]
.L_0200d46c:
	mov r2, r9
	cmp r2, #1
	bne .L_0200d49e
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_020054ac
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_0200d49e
.L_0200d490:
	adds r5, #6
	movs r1, #255
.L_0200d494:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_0200d444
.L_0200d49e:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200d4a8:
	.4byte 0xffffe100
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
