.syntax unified
	.thumb
	.global Func_08196248
	.thumb_func
Func_08196248:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	str r1, [sp, #16]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #100]
	adds r6, r0, #0
	str r2, [sp, #12]
	ldr r3, [r3, #92]
	mov r8, r3
	lsls r3, r6, #1
	adds r6, r3, r6
	movs r3, #0
	str r3, [sp, #8]
	cmp r6, #0
	beq .L_0819628c
	ldr r7, .L_081963e4
	adds r5, r2, #0
.L_08196278:
	adds r0, r5, #0
	adds r1, r5, #0
	mov lr, r7
	.2byte 0xf800
	ldr r4, [sp, #8]
	adds r5, #12
	adds r4, #3
	str r4, [sp, #8]
	cmp r4, r6
	bne .L_08196278
.L_0819628c:
	ldr r3, [sp, #16]
	movs r2, #0
	str r2, [sp, #8]
	cmp r3, #0
	bne .L_08196298
	b .L_081963d6
.L_08196298:
	ldr r4, [sp, #12]
	lsls r3, r6, #2
	adds r6, r3, r4
	movs r2, #12
	adds r4, r6, #0
	adds r2, r2, r6
	adds r4, #28
	mov r9, r2
	movs r3, #24
	movs r2, #16
	str r4, [sp, #4]
	adds r3, r3, r6
	adds r2, r2, r6
	mov r10, r3
	adds r7, r6, #4
	mov r11, r2
.L_081962b8:
	mov r3, r8
	ldr r0, [r3]
	ldr r4, [sp, #12]
	movs r3, #128
	movs r2, #132
	lsls r0, r0, #2
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r0, r4
	adds r1, r6, #0
	adds r2, #3
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r2, r8
	ldr r0, [r2, #4]
	movs r2, #132
	lsls r0, r0, #2
	lsls r2, r2, #24
	adds r0, r0, r4
	mov r1, r9
	adds r2, #3
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r3, r8
	ldr r0, [r3, #8]
	movs r2, #132
	movs r3, #128
	lsls r0, r0, #2
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r0, r4
	mov r1, r10
	adds r2, #3
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r4, r9
	ldr r2, [sp, #4]
	ldr r0, [r4]
	ldr r3, [r6]
	ldr r1, [r2]
	subs r0, r0, r3
	ldr r3, [r7]
	subs r1, r1, r3
	ldr r3, .L_081963e8
	mov lr, r3
	.2byte 0xf800
	mov r4, r10
	ldr r3, [r6]
	adds r5, r0, #0
	ldr r0, [r4]
	mov r2, r11
	subs r0, r0, r3
	ldr r1, [r2]
	ldr r3, [r7]
	subs r1, r1, r3
	ldr r3, .L_081963e8
	mov lr, r3
	.2byte 0xf800
	subs r5, r5, r0
	cmp r5, #0
	blt .L_081963c4
	ldr r3, [r6]
	movs r2, #128
	lsls r2, r2, #15
	adds r3, r3, r2
	mov r4, r8
	ldrb r5, [r4, #24]
	str r3, [r6]
	ldr r3, [r7]
	mov r4, r9
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r4]
	adds r3, r3, r2
	str r3, [r4]
	mov r2, r11
	ldr r3, [r2]
	movs r4, #128
	lsls r4, r4, #15
	adds r3, r3, r4
	str r3, [r2]
	mov r2, r10
	ldr r3, [r2]
	adds r3, r3, r4
	str r3, [r2]
	ldr r4, [sp, #4]
	movs r2, #128
	ldr r3, [r4]
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r4]
	movs r4, #2
	ldrsh r1, [r7, r4]
	mov r4, r9
	movs r3, #2
	ldrsh r2, [r4, r3]
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r3, r11
	movs r4, #2
	ldrsh r3, [r3, r4]
	str r5, [sp, #0]
	bl Func_08143eb4
	mov r2, r9
	movs r4, #2
	ldrsh r0, [r2, r4]
	mov r4, r11
	movs r3, #2
	ldrsh r1, [r4, r3]
	mov r4, r10
	movs r3, #2
	ldrsh r2, [r4, r3]
	ldr r3, [sp, #4]
	movs r4, #2
	ldrsh r3, [r3, r4]
	str r5, [sp, #0]
	bl Func_08143eb4
	movs r4, #2
	ldrsh r0, [r6, r4]
	mov r4, r10
	movs r2, #2
	ldrsh r1, [r7, r2]
	movs r3, #2
	ldrsh r2, [r4, r3]
	ldr r3, [sp, #4]
	movs r4, #2
	ldrsh r3, [r3, r4]
	str r5, [sp, #0]
	bl Func_08143eb4
.L_081963c4:
	ldr r2, [sp, #8]
	ldr r3, [sp, #16]
	movs r4, #28
	adds r2, #1
	add r8, r4
	str r2, [sp, #8]
	cmp r2, r3
	beq .L_081963d6
	b .L_081962b8
.L_081963d6:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081963e4:
	.4byte IwramTransformVector
.L_081963e8:
	.4byte IwramMulQ16
