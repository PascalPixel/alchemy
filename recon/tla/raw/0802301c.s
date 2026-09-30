.syntax unified
	.thumb
	.global Func_0802301c
	.thumb_func
Func_0802301c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r0, [sp, #0]
	mov r11, r1
	movs r7, #0
.L_08023032:
	mov r3, r11
	cmp r3, #0
	ble .L_0802306e
	adds r3, r7, #1
	mov r9, r3
	ldr r5, [sp, #0]
	adds r3, r7, #2
	mov r10, r3
	adds r3, r7, #3
	mov r8, r3
	mov r6, r11
.L_08023048:
	ldr r0, [r5]
	adds r1, r7, #0
	bl Func_08022f64
	ldr r0, [r5]
	mov r1, r9
	bl Func_08022f64
	ldr r0, [r5]
	mov r1, r10
	bl Func_08022f64
	subs r6, #1
	ldmia r5!, {r0}
	mov r1, r8
	bl Func_08022f64
	cmp r6, #0
	bne .L_08023048
.L_0802306e:
	movs r0, #1
	adds r7, #4
	bl WaitFrames
	cmp r7, #127
	bls .L_08023032
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
