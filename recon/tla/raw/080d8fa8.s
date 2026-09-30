.syntax unified
	.thumb
	.global Func_080d8fa8
	.thumb_func
Func_080d8fa8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	adds r0, r1, #0
	adds r5, r2, #0
	sub sp, #16
	adds r7, r3, #0
	bl ObjectTable_Get
	adds r6, r0, #0
	adds r0, r5, #0
	bl ObjectTable_Get
	adds r5, r0, #0
	cmp r6, #0
	beq .L_080d8ff0
	cmp r5, #0
	beq .L_080d8ff0
	ldr r0, [r5, #8]
	ldr r2, [r6, #12]
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	str r0, [sp, #0]
	movs r4, #128
	ldr r0, [r5, #12]
	lsls r4, r4, #12
	adds r0, r0, r4
	str r0, [sp, #4]
	adds r2, r2, r4
	ldr r0, [r5, #16]
	str r7, [sp, #12]
	str r0, [sp, #8]
	mov r0, r8
	bl Func_080d8ff8
.L_080d8ff0:
	add sp, #16
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
