.syntax unified
	.thumb
	.global Func_080df8e0
	.thumb_func
Func_080df8e0:
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
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r2, r10
	ands r3, r2
	mov r11, r0
	cmp r3, #0
	beq .L_080df91e
	cmp r7, #0
	beq .L_080df91e
	movs r3, #24
	ldrsh r0, [r7, r3]
	adds r2, r6, #0
	b .L_080df924
.L_080df91e:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
.L_080df924:
	adds r1, r5, #0
	mov r3, r8
	bl Func_080200c0
	adds r6, r0, #0
	cmp r6, #0
	bne .L_080df934
	b .L_080dfa9a
.L_080df934:
	ldr r1, [r6, #80]
	movs r5, #15
	mov r8, r1
	mov r1, r10
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	bl Object_SetMode
	ldr r2, .L_080dfaa8
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r9, r3
	bl Object_SetCallback
	adds r3, r6, #0
	movs r0, #0
	adds r3, #85
	strb r0, [r3]
	mov r3, r8
	strb r0, [r3, #26]
	ldr r3, .L_080dfaac
	mov r2, r11
	str r3, [r6, #108]
	ldr r1, [sp, #4]
	movs r5, #3
	str r1, [r6, #68]
	ldr r3, [sp, #40]
	str r3, [r6, #72]
	ldr r3, [sp, #44]
	str r3, [r6, #76]
	ldr r3, [r2, #80]
	movs r2, #12
	ldrb r3, [r3, #9]
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	mov r11, r3
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
	ldr r3, .L_080dfab0
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_080dfa9a
	cmp r7, #0
	beq .L_080dfa9a
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_080df9c0
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_080df9c0:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_080df9ec
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
	mov r3, r11
	ands r3, r1
	lsls r2, r2, #2
	orrs r3, r2
	mov r1, r8
	strb r3, [r1, #9]
.L_080df9ec:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_080dfa00
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_080dfa00:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_080dfa4a
	ldr r3, .L_080dfaa8
	mov r1, r9
	ldr r5, [r3, r1]
	cmp r2, #0
	beq .L_080dfa2e
	ldr r0, [r7, #16]
	ldr r3, [r6, #24]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	bl Math_Div
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_080dfa44
.L_080dfa2e:
	ldr r0, [r7, #16]
	ldr r2, .L_080dfab0
	ldr r1, [r5, #12]
	adds r0, r0, r2
	bl Math_Div
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_080dfab0
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_080dfa44:
	bl Math_Div
	str r0, [r6, #52]
.L_080dfa4a:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_080dfa66
	adds r0, r6, #0
	movs r1, #1
	bl Object_SetMode
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Object_SetCallback
.L_080dfa66:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_080dfa78
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #18]
.L_080dfa78:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_080dfa8a
	ldrh r3, [r7, #34]
	ldr r1, [sp, #0]
	strh r3, [r1]
.L_080dfa8a:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_080dfa9a
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_080dfa9a:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080dfaa8:
	.4byte Data_080f3978
.L_080dfaac:
	.4byte Func_080df8a8
.L_080dfab0:
	.4byte 0xffff0000
