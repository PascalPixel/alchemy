.syntax unified
	.thumb
	.global Func_080eb3d0
	.thumb_func
Func_080eb3d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r9, r2
	mov r10, r9
	sub sp, #8
	adds r7, r3, #0
	movs r2, #0
	mov r3, r10
	str r0, [sp, #4]
	str r1, [sp, #0]
	mov r11, r2
	cmp r3, #0
	blt .L_080eb492
.L_080eb3f4:
	ldr r2, [sp, #0]
	ldr r5, [sp, #4]
	add r2, r11
	mov r8, r2
	add r5, r10
	adds r0, r5, #0
	mov r1, r8
	adds r2, r7, #0
	bl Func_080eb2d8
	ldr r3, [sp, #0]
	mov r2, r11
	subs r6, r3, r2
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_080eb2d8
	ldr r3, [sp, #4]
	mov r2, r10
	subs r5, r3, r2
	adds r0, r5, #0
	mov r1, r8
	adds r2, r7, #0
	bl Func_080eb2d8
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_080eb2d8
	ldr r3, [sp, #0]
	ldr r5, [sp, #4]
	add r3, r10
	mov r8, r3
	add r5, r11
	adds r0, r5, #0
	mov r1, r8
	adds r2, r7, #0
	bl Func_080eb2d8
	ldr r2, [sp, #0]
	mov r3, r10
	subs r6, r2, r3
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_080eb2d8
	ldr r2, [sp, #4]
	mov r3, r11
	subs r5, r2, r3
	adds r0, r5, #0
	mov r1, r8
	adds r2, r7, #0
	bl Func_080eb2d8
	adds r2, r7, #0
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_080eb2d8
	mov r2, r11
	lsls r3, r2, #1
	mov r2, r9
	subs r3, r2, r3
	adds r3, #1
	mov r9, r3
	cmp r3, #0
	bge .L_080eb48a
	mov r2, r10
	subs r2, #1
	lsls r3, r2, #1
	add r9, r3
	mov r10, r2
.L_080eb48a:
	movs r3, #1
	add r11, r3
	cmp r10, r11
	bge .L_080eb3f4
.L_080eb492:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
