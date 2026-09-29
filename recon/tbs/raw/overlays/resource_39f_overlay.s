.syntax unified
	.thumb
	.section .text.x02008cd0,"ax",%progbits
	.balign 4
	.global SceneActor_TryRunSlotZeroMoveStep
	.thumb_func
SceneActor_TryRunSlotZeroMoveStep:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #0
	bl 0x0200adbc
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #85
	ldrb r3, [r6]
	adds r1, r7, #0
	mov r8, r3
	bl 0x0200ad54
	cmp r0, #0
	bne .L_02000cd0_0
	bl 0x0200ada4
	movs r1, #6
	adds r0, r5, #0
	bl 0x0200ad24
	movs r0, #6
	bl 0x0200acec
	movs r0, #152
	bl 0x0200aeb4
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200ad24
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldrb r2, [r6]
	movs r3, #126
	ands r3, r2
	strb r3, [r6]
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200ad5c
	movs r3, #10
	ldrsh r2, [r7, r3]
	movs r3, #2
	ldrsh r1, [r7, r3]
	movs r0, #0
	bl 0x0200add4
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200ad24
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200ad5c
	mov r3, r8
	strb r3, [r6]
	bl 0x0200adac
	movs r0, #1
	b .L_02000cd0_1
.L_02000cd0_0:
	movs r0, #0
.L_02000cd0_1:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.section .text.x02008d90,"ax",%progbits
	.balign 4
	.global FieldScene_RunScene39f_02000d90
	.thumb_func
FieldScene_RunScene39f_02000d90:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	adds r6, r0, #0
	mov r10, r2
	mov r9, r3
	mov r8, r1
	bl 0x0200adbc
	movs r1, #1
	adds r5, r0, #0
	adds r0, r6, #0
	bl 0x0200ae3c
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #9
	adds r0, r6, #0
	lsls r1, r1, #10
	bl 0x0200adc4
	movs r0, #152
	bl 0x0200aeb4
	mov r3, r9
	str r3, [r5, #40]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #72]
	movs r3, #0
	str r3, [r5, #68]
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200ad5c
	adds r0, r6, #0
	mov r1, r8
	mov r2, r10
	bl 0x0200add4
	mov r3, r8
	lsls r3, r3, #16
	mov r8, r3
	mov r3, r10
	lsls r3, r3, #16
	mov r10, r3
	adds r0, r6, #0
	mov r1, r8
	mov r2, r10
	bl 0x0200adf4
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200ad5c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #72]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.section .text.x02008ee0,"ax",%progbits
	.balign 4
	.global Func_02000ee0
	.thumb_func
Func_02000ee0:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000ee0_0
	ldr r0, [pc, #36]
	b .L_02000ee0_1
.L_02000ee0_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000ee0_2
	ldr r0, [pc, #36]
	b .L_02000ee0_1
.L_02000ee0_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000ee0_3
	ldr r0, [pc, #32]
	b .L_02000ee0_1
.L_02000ee0_3:
	ldr r0, [pc, #32]
.L_02000ee0_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000044
	.4byte 0x0200b0f4
	.4byte 0x00000045
	.4byte 0x0200b1e4
	.4byte 0x00000046
	.4byte 0x0200b334
	.4byte 0x0200b4b4
	.section .text.x02008f40,"ax",%progbits
	.balign 4
	.global Func_02000f40
	.thumb_func
Func_02000f40:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000f40_0
	ldr r0, [pc, #36]
	b .L_02000f40_1
.L_02000f40_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000f40_2
	ldr r0, [pc, #36]
	b .L_02000f40_1
.L_02000f40_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000f40_3
	ldr r0, [pc, #32]
	b .L_02000f40_1
.L_02000f40_3:
	ldr r0, [pc, #32]
.L_02000f40_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000044
	.4byte 0x0200b6a0
	.4byte 0x00000045
	.4byte 0x0200b790
	.4byte 0x00000046
	.4byte 0x0200b8b0
	.4byte 0x0200ba30
	.section .text.x02009244,"ax",%progbits
	.balign 4
	.global Func_02001244
	.thumb_func
Func_02001244:
	push {r5, lr}
	movs r0, #11
	sub sp, #16
	bl 0x0200adbc
	adds r5, r0, #0
	bl 0x0200ada4
	movs r0, #11
	movs r1, #0
	bl 0x02008ea8
	movs r1, #204
	movs r2, #228
	movs r3, #192
	lsls r1, r1, #1
	lsls r2, r2, #1
	lsls r3, r3, #11
	movs r0, #11
	bl 0x02008d90
	ldr r2, [r5, #16]
	movs r3, #192
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r3, #1
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r0, #11
	movs r1, #1
	bl 0x0200ae54
	movs r2, #0
	movs r1, #0
	movs r0, #11
	bl 0x0200ae1c
	movs r0, #30
	bl 0x0200ad9c
	movs r0, #11
	movs r1, #2
	bl 0x0200ae04
	movs r2, #0
	ldr r1, [pc, #104]
	movs r0, #11
	bl 0x0200ae44
	movs r0, #147
	bl 0x0200aeb4
	movs r0, #60
	bl 0x0200ad9c
	movs r0, #0
	bl 0x0200adbc
	movs r2, #10
	ldrsh r5, [r0, r2]
	movs r0, #0
	bl 0x0200adbc
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r3, #128
	lsls r3, r3, #11
	adds r1, r5, #0
	movs r0, #11
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
	ldr r0, [pc, #48]
	bl 0x0200ad94
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x0200adf4
	ldr r3, [pc, #36]
	ldr r2, [pc, #40]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #53
	movs r1, #0
	bl 0x0200ae7c
	bl 0x0200adac
	sub sp, #-16
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000103
	.4byte 0x00000301
	.4byte 0x02000240
	.4byte 0x0000022b
	.section .text.x02009454,"ax",%progbits
	.balign 4
	.global Func_02001454
	.thumb_func
Func_02001454:
	push {r5, lr}
	sub sp, #32
	bl 0x0200ada4
	add r5, sp, #8
	adds r0, r5, #0
	bl 0x02008474
	cmp r0, #0
	beq .L_02001454_0
	mov r3, sp
	add r2, sp, #24
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r3, [r5, #12]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	bl 0x02008608
	ldr r3, [r5, #4]
	cmp r3, #8
	bne .L_02001454_1
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #23
	bne .L_02001454_1
	movs r3, #35
	movs r2, #68
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #67
	movs r2, #4
	movs r3, #1
	bl 0x0200ad4c
	b .L_02001454_0
.L_02001454_1:
	ldr r3, [r5, #4]
	cmp r3, #10
	bne .L_02001454_0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #35
	bne .L_02001454_0
	ldr r0, [pc, #108]
	bl 0x0200ad94
	movs r0, #10
	movs r1, #3
	bl 0x0200adfc
	movs r1, #16
	movs r2, #6
	negs r1, r1
	movs r0, #10
	bl 0x0200ade4
	movs r0, #30
	bl 0x0200ad9c
	movs r1, #8
	movs r0, #10
	bl 0x0200adfc
	movs r0, #240
	bl 0x0200aeb4
	movs r0, #10
	bl 0x0200adbc
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r2, #30
	movs r3, #34
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #44
	movs r1, #30
	movs r2, #2
	movs r3, #4
	bl 0x0200ad4c
	movs r3, #4
	movs r5, #0
	str r3, [sp, #0]
	movs r0, #2
	movs r1, #35
	movs r2, #30
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02008244
.L_02001454_0:
	bl 0x0200adac
	sub sp, #-32
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000311
	.section .text.x020099b8,"ax",%progbits
	.balign 4
	.global Func_020019b8
	.thumb_func
Func_020019b8:
	push {r5, r6, lr}
	sub sp, #32
	bl 0x0200ada4
	add r5, sp, #8
	adds r0, r5, #0
	movs r6, #0
	bl 0x02008474
	cmp r0, #0
	beq .L_020019b8_0
	mov r2, sp
	add r3, sp, #24
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r3, [r5, #12]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	bl 0x02008608
	ldr r3, [r5, #4]
	cmp r3, #9
	beq .L_020019b8_1
	cmp r3, #11
	bne .L_020019b8_2
	b .L_020019b8_3
.L_020019b8_1:
	ldr r3, [r5, #8]
	movs r2, #68
	asrs r3, r3, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #4
	movs r2, #1
	movs r0, #38
	movs r1, #68
	bl 0x0200ad4c
	ldr r3, [r5, #8]
	asrs r2, r3, #20
	cmp r2, #42
	bne .L_020019b8_4
	movs r3, #23
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #20
	movs r2, #2
	movs r3, #4
	bl 0x0200ad4c
	movs r0, #9
	movs r1, #1
	bl 0x0200ae3c
	ldr r0, [pc, #152]
	movs r6, #1
	bl 0x0200ad94
	b .L_020019b8_4
.L_020019b8_3:
	ldr r3, [r5, #8]
	asrs r2, r3, #20
	cmp r2, #40
	bne .L_020019b8_4
	movs r3, #32
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #20
	movs r2, #2
	movs r3, #4
	bl 0x0200ad4c
	movs r0, #11
	movs r1, #1
	bl 0x0200ae3c
	ldr r0, [pc, #112]
	movs r6, #1
	bl 0x0200ad94
.L_020019b8_4:
	cmp r6, #0
	bne .L_020019b8_5
	bl 0x0200adac
	b .L_020019b8_6
.L_020019b8_5:
	ldr r0, [r5, #4]
	movs r1, #3
	bl 0x0200adfc
	movs r2, #6
	movs r1, #18
	ldr r0, [r5, #4]
	bl 0x0200ade4
	movs r0, #30
	bl 0x0200ad9c
	movs r1, #8
	ldr r0, [r5, #4]
	bl 0x0200adfc
	movs r0, #240
	bl 0x0200aeb4
	ldr r0, [r5, #4]
	bl 0x0200adbc
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	b .L_020019b8_0
.L_020019b8_2:
	cmp r3, #8
	bne .L_020019b8_0
	ldr r3, [r5, #8]
	movs r2, #49
	asrs r3, r3, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #49
	movs r2, #1
	movs r3, #4
	bl 0x0200ad4c
.L_020019b8_0:
	bl 0x0200adac
.L_020019b8_6:
	sub sp, #-32
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000312
	.4byte 0x00000313
	.section .text.x02009d04,"ax",%progbits
	.balign 4
	.global Func_02001d04
	.thumb_func
Func_02001d04:
	push {r5, r6, lr}
	movs r0, #15
	sub sp, #16
	bl 0x0200adbc
	movs r6, #128
	adds r5, r0, #0
	bl 0x0200ada4
	movs r0, #15
	movs r1, #0
	bl 0x02008ea8
	lsls r6, r6, #12
	movs r1, #236
	adds r3, r6, #0
	lsls r1, r1, #1
	movs r2, #104
	movs r0, #15
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
	ldr r2, [r5, #16]
	movs r3, #1
.L_02001d38:
	movs r4, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	adds r2, r2, r6
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #12]
	bl 0x02008ae8
	movs r0, #15
	movs r1, #1
	bl 0x0200ae54
	movs r2, #0
	movs r1, #0
	movs r0, #15
	bl 0x0200ae1c
	movs r0, #30
	bl 0x0200ad9c
	movs r0, #15
	movs r1, #2
	bl 0x0200ae04
	movs r2, #0
	ldr r1, [pc, #92]
.L_02001d72:
	movs r0, #15
	bl 0x0200ae44
	movs r0, #147
	bl 0x0200aeb4
	movs r0, #60
	bl 0x0200ad9c
	movs r0, #0
	bl 0x0200adbc
	movs r2, #10
	ldrsh r5, [r0, r2]
	movs r0, #0
	bl 0x0200adbc
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r3, #192
	lsls r3, r3, #11
	adds r1, r5, #0
	movs r0, #15
	bl 0x02008d90
	movs r0, #10
	bl 0x0200ad9c
	ldr r0, [pc, #40]
	bl 0x0200ad94
	ldr r3, [pc, #36]
	ldr r2, [pc, #40]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #53
	movs r1, #0
	bl 0x0200ae7c
	bl 0x0200adac
	sub sp, #-16
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0103
	.2byte 0x0000
	.4byte 0x00000307
	.4byte 0x02000240
	.4byte 0x0000022b
	.section .text.x0200a4ac,"ax",%progbits
	.global Func_020024ac
	.thumb_func
Func_020024ac:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020024ac_0
	ldr r0, [pc, #36]
	b .L_020024ac_1
.L_020024ac_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020024ac_2
	ldr r0, [pc, #36]
	b .L_020024ac_1
.L_020024ac_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020024ac_3
	ldr r0, [pc, #32]
	b .L_020024ac_1
.L_020024ac_3:
	ldr r0, [pc, #32]
.L_020024ac_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000044
	.4byte 0x0200ba48
	.4byte 0x00000045
	.4byte 0x0200bb20
	.4byte 0x00000046
	.4byte 0x0200bc1c
	.4byte 0x0200bd54
	.section .text.x02008ae8,"ax",%progbits
	.balign 4
	.p2align 2
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
	bl 0x0200adbc
	movs r3, #128
	lsls r3, r3, #13
	mov r2, r10
	ands r3, r2
	mov r9, r0
	cmp r3, #0
	beq .L_02000ae8_0
	cmp r7, #0
	beq .L_02000ae8_0
	movs r3, #24
	ldrsh r0, [r7, r3]
	adds r2, r6, #0
	b .L_02000ae8_1
.L_02000ae8_0:
	adds r2, r6, #0
	movs r0, #222
.L_02000ae8_1:
	adds r1, r5, #0
	mov r3, r8
	bl 0x0200ad34
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02000ae8_2
	b 0x02008ca2
.L_02000ae8_2:
	ldr r1, [r6, #80]
	mov r8, r1
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	bl 0x0200ad24
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x0200ad2c
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
	beq 0x02008ca2
	cmp r7, #0
	beq 0x02008ca2
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02000ae8_3
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl 0x0200ae2c
.L_02000ae8_3:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq 0x02008bf4
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
.L_02000bf0:
	mov r1, r8
	strb r3, [r1, #9]
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq 0x02008c08
	ldr r3, [r7, #8]
.L_02000c02:
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_02000c02_0
	ldr r3, [pc, #156]
	mov r1, r11
	ldr r5, [r3, r1]
	cmp r2, #0
	beq .L_02000c02_1
	ldr r0, [r7, #16]
	ldr r3, [r6, #24]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	bl 0x0200ace4
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02000c02_2
.L_02000c02_1:
	ldr r0, [r7, #16]
	ldr r2, [pc, #128]
	ldr r1, [r5, #12]
	adds r0, r0, r2
	bl 0x0200ace4
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02000c02_2:
	bl 0x0200ace4
	str r0, [r6, #52]
.L_02000c02_0:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_02000c02_3
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200ad24
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x0200ad2c
.L_02000c02_3:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02000c02_4
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #30]
.L_02000c02_4:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02000c02_5
	ldrh r3, [r7, #34]
	ldr r1, [sp, #0]
	strh r3, [r1]
.L_02000c02_5:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02000c02_6
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02000c02_6:
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200b058
	.2byte 0x8ab1
	.2byte 0x0200
	.4byte 0xffff0000
	.section .rodata.part1,"a",%progbits
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
	.4byte 0x0200afb0
	.4byte 0x0200afe8
	.4byte 0x0200b020
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000006
	.4byte 0x00000022
	.4byte 0x02008d6d
	.4byte 0x00000010
	.global MogoruMori_ActorScript
MogoruMori_ActorScript:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x0000000f
	.4byte 0x00000022
	.4byte 0x02008d6d
	.4byte 0x00000022
	.4byte 0x02008d81
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000012
	.4byte 0x80000000
	.4byte 0x00000022
	.4byte 0x02008d81
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000010
	.4byte 0xffff0001
	.4byte 0x00000088
	.4byte 0x40000018
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x00000018
	.4byte 0x00000080
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0003
	.4byte 0x000000f8
	.4byte 0x80000080
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x00000088
	.4byte 0xc00000c8
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0005
	.4byte 0x00000048
	.4byte 0x40000108
	.4byte 0x00000000
	.4byte 0x011000f0
	.4byte 0x000001d0
	.4byte 0xffff0006
	.4byte 0x000000c8
	.4byte 0xc00001b8
	.4byte 0x00000000
	.4byte 0x011000f0
	.4byte 0x000001d0
	.4byte 0xffff0007
	.4byte 0x000001a0
	.4byte 0x40000108
	.4byte 0x01200000
	.4byte 0x02a000f0
	.4byte 0x00000270
	.4byte 0xffff0008
	.4byte 0x00000230
	.4byte 0x40000108
	.4byte 0x01200000
	.4byte 0x02a000f0
	.4byte 0x00000270
	.4byte 0xffff0009
	.4byte 0x00000288
	.4byte 0x80000158
	.4byte 0x01200000
	.4byte 0x02a000f0
	.4byte 0x00000270
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000018
	.4byte 0x00000080
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x80000080
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0003
	.4byte 0x000001a8
	.4byte 0x40000018
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x00000138
	.4byte 0x00000088
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0005
	.4byte 0x00000218
	.4byte 0x80000078
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0006
	.4byte 0x000001a8
	.4byte 0xc00000c8
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0007
	.4byte 0x000002e8
	.4byte 0x40000018
	.4byte 0x02400000
	.4byte 0x03500000
	.4byte 0x000000e0
	.4byte 0xffff0008
	.4byte 0x000002e8
	.4byte 0xc00000c8
	.4byte 0x02400000
	.4byte 0x03500000
	.4byte 0x000000e0
	.4byte 0xffff0009
	.4byte 0x00000018
	.4byte 0x00000170
	.4byte 0x00000000
	.4byte 0x011000f0
	.4byte 0x000001d0
	.4byte 0xffff000a
	.4byte 0x000001e8
	.4byte 0x40000108
	.4byte 0x01200000
	.4byte 0x02e000f0
	.4byte 0x00000270
	.4byte 0xffff000b
	.4byte 0x00000138
	.4byte 0x00000210
	.4byte 0x01200000
	.4byte 0x02e000f0
	.4byte 0x00000270
	.4byte 0xffff000c
	.4byte 0x00000168
	.4byte 0xc0000258
	.4byte 0x01200000
	.4byte 0x02e000f0
	.4byte 0x00000270
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000018
	.4byte 0x00000080
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x80000080
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x000000e0
	.4byte 0xffff0003
	.4byte 0x000001a8
	.4byte 0x40000018
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x00000138
	.4byte 0x000000a0
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0005
	.4byte 0x00000218
	.4byte 0x800000a0
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0006
	.4byte 0x000001a8
	.4byte 0xc00000c8
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0007
	.4byte 0x00000338
	.4byte 0x80000088
	.4byte 0x02400000
	.4byte 0x03500000
	.4byte 0x000000e0
	.4byte 0xffff0008
	.4byte 0x000002d8
	.4byte 0x40000108
	.4byte 0x01900000
	.4byte 0x035000f0
	.4byte 0x00000280
	.4byte 0xffff0009
	.4byte 0x00000338
	.4byte 0x80000200
	.4byte 0x01900000
	.4byte 0x035000f0
	.4byte 0x00000280
	.4byte 0xffff000a
	.4byte 0x00000338
	.4byte 0x80000260
	.4byte 0x01900000
	.4byte 0x035000f0
	.4byte 0x00000280
	.4byte 0xffff000b
	.4byte 0x000002e0
	.4byte 0xc0000268
	.4byte 0x01900000
	.4byte 0x035000f0
	.4byte 0x00000280
	.4byte 0xffff000c
	.4byte 0x00000108
	.4byte 0x40000108
	.4byte 0x00000000
	.4byte 0x018000f0
	.4byte 0x000001f0
	.4byte 0xffff000d
	.4byte 0x00000088
	.4byte 0xc00001d8
	.4byte 0x00000000
	.4byte 0x018000f0
	.4byte 0x000001f0
	.4byte 0xffff000f
	.4byte 0x00000088
	.4byte 0xc00001b0
	.4byte 0x00000000
	.4byte 0x018000f0
	.4byte 0x000001f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0x00000080
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x80000170
	.4byte 0x00000000
	.4byte 0x011000f0
	.4byte 0x000001d0
	.4byte 0xffff0003
	.4byte 0x000001a8
	.4byte 0xc00001b8
	.4byte 0x01200000
	.4byte 0x023000f0
	.4byte 0x000001d0
	.4byte 0xffff0004
	.4byte 0x000002c8
	.4byte 0x40000108
	.4byte 0x02400000
	.4byte 0x035000f0
	.4byte 0x000001d0
	.4byte 0xffff0005
	.4byte 0x00000088
	.4byte 0xc00002a8
	.4byte 0x00000000
	.4byte 0x011001e0
	.4byte 0x000002c0
	.4byte 0xffff0006
	.4byte 0x00000138
	.4byte 0x00000228
	.4byte 0x01200000
	.4byte 0x023001e0
	.4byte 0x000002c0
	.4byte 0xffff0007
	.4byte 0x00000138
	.4byte 0x00000288
	.4byte 0x01200000
	.4byte 0x023001e0
	.4byte 0x000002c0
	.4byte 0xffff0008
	.4byte 0x00000288
	.4byte 0xc00002a8
	.4byte 0x02400000
	.4byte 0x035001e0
	.4byte 0x000002c0
	.4byte 0xffff0009
	.4byte 0x00000218
	.4byte 0x80000080
	.4byte 0x01200000
	.4byte 0x02300000
	.4byte 0x000000e0
	.4byte 0xffff000a
	.4byte 0x00000018
	.4byte 0x00000170
	.4byte 0x00000000
	.4byte 0x011000f0
	.4byte 0x000001d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global MogoruMori_SceneTable
MogoruMori_SceneTable:
	.4byte 0x00000044
	.4byte 0x0010d002
	.4byte 0x00202047
	.4byte 0x00301047
	.4byte 0x00405044
	.4byte 0x00504044
	.4byte 0x00607044
	.4byte 0x00706044
	.4byte 0x00808047
	.4byte 0x00901045
	.4byte 0x00000045
	.4byte 0x00109044
	.4byte 0x00204045
	.4byte 0x00303047
	.4byte 0x00402045
	.4byte 0x00509045
	.4byte 0x00607045
	.4byte 0x00706045
	.4byte 0x0080a045
	.4byte 0x00905045
	.4byte 0x00a08045
	.4byte 0x00b02046
	.4byte 0x00c04047
	.4byte 0x00000046
	.4byte 0x00105046
	.4byte 0x0020b045
	.4byte 0x00305047
	.4byte 0x00407046
	.4byte 0x00501046
	.4byte 0x00608046
	.4byte 0x00704046
	.4byte 0x00806046
	.4byte 0x00906047
	.4byte 0x00a07047
	.4byte 0x00b0c046
	.4byte 0x00c0b046
	.4byte 0x00d2b002
	.4byte 0x00000047
	.4byte 0x00103044
	.4byte 0x00202044
	.4byte 0x00303045
	.4byte 0x0040c045
	.4byte 0x00503046
	.4byte 0x00609046
	.4byte 0x0070a046
	.4byte 0x00808044
	.4byte 0x00b01047
	.4byte 0x00c02047
	.4byte 0x00d03047
	.4byte 0x00e04047
	.4byte 0x00f05047
	.4byte 0x01006047
	.4byte 0x01107047
	.4byte 0x01208047
	.4byte 0x01309047
	.4byte 0x0140a047
	.4byte 0x000001ff
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0x030000bf
	.4byte 0x0200b084
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x030100bf
	.4byte 0x0200b084
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x00024000
	.4byte 0x030200bf
	.4byte 0x0200b084
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x03000016
	.4byte 0x0200b0bc
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x03010016
	.4byte 0x0200b0bc
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x01024000
	.4byte 0x03020016
	.4byte 0x0200b0bc
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0x0fd40016
	.4byte 0x00000007
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x0200b064
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0x030300bf
	.4byte 0x0200b084
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0x030400bf
	.4byte 0x0200b084
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0x030600bf
	.4byte 0x0200b084
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00024000
	.4byte 0x03030016
	.4byte 0x0200b0bc
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0x03040016
	.4byte 0x0200b0bc
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x01024000
	.4byte 0x03060016
	.4byte 0x0200b0bc
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x01024000
	.4byte 0x0032005a
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x0200b064
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x0200b064
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0x030700bf
	.4byte 0x0200b084
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0x030800bf
	.4byte 0x0200b084
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x030900bf
	.4byte 0x0200b084
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0x030b00bf
	.4byte 0x0200b084
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00024000
	.4byte 0x03070016
	.4byte 0x0200b0bc
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0x03080016
	.4byte 0x0200b0bc
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x01024000
	.4byte 0x03090016
	.4byte 0x0200b0bc
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x01024000
	.4byte 0x030b0016
	.4byte 0x0200b0bc
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02008f95
	.4byte 0x0000c602
	.4byte 0xffff0019
	.4byte 0x0200903d
	.4byte 0x00004602
	.4byte 0xffff001a
	.4byte 0x02009079
	.4byte 0x00000003
	.4byte 0xffff001b
	.4byte 0x0200912d
	.4byte 0x00008e15
	.4byte 0x0300000d
	.4byte 0x02009151
	.4byte 0x00008e15
	.4byte 0x0301000e
	.4byte 0x02009245
	.4byte 0x00008e15
	.4byte 0x0302000f
	.4byte 0x02009329
	.4byte 0x00009415
	.4byte 0x0fd40010
	.4byte 0x02009421
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02009455
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x02009521
	.4byte 0x00008e15
	.4byte 0x0303000f
	.4byte 0x020095d1
	.4byte 0x00008e15
	.4byte 0x03040010
	.4byte 0x020096f1
	.4byte 0x00008e15
	.4byte 0x03050011
	.4byte 0x02009819
	.4byte 0x00008e15
	.4byte 0x03060011
	.4byte 0x02009881
	.4byte 0x00000013
	.4byte 0x0f1b0064
	.4byte 0x001000c1
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008cc1
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x020099b9
	.4byte 0x00008602
	.4byte 0xffff0019
	.4byte 0x02009ac9
	.4byte 0x00000002
	.4byte 0x089d0016
	.4byte 0x0200a1b1
	.4byte 0x00000002
	.4byte 0x089e0017
	.4byte 0x0200a2c1
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x02009b85
	.4byte 0x00008c15
	.4byte 0x0214000e
	.4byte 0x02009c35
	.4byte 0x00008e15
	.4byte 0x03070013
	.4byte 0x02009d05
	.4byte 0x00008e15
	.4byte 0x03080014
	.4byte 0x02009de1
	.4byte 0x00008e15
	.4byte 0x03090015
	.4byte 0x02009ef1
	.4byte 0x00008e15
	.4byte 0x030a0016
	.4byte 0x0200a005
	.4byte 0x00008e15
	.4byte 0x030b0016
	.4byte 0x0200a079
	.4byte 0x00000013
	.4byte 0x0f1a0064
	.4byte 0x0010005d
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x00000013
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000003
	.4byte 0x03500064
	.4byte 0x00300000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
