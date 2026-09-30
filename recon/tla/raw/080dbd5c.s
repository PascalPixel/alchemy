.syntax unified
	.thumb
	.global Func_080dbd5c
	.thumb_func
Func_080dbd5c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r4, r0, #0
	movs r0, #197
	lsls r0, r0, #1
	adds r3, r3, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	sub sp, #12
	cmp r3, #3
	bne .L_080dbd94
	mov r0, sp
	movs r3, #0
	str r3, [r0, #4]
	str r4, [r0]
	str r1, [r0, #8]
	bl Func_08020358
	subs r0, #2
	movs r3, #0
	cmp r0, #2
	bhi .L_080dbd90
	movs r3, #1
.L_080dbd90:
	adds r0, r3, #0
	b .L_080dbda4
.L_080dbd94:
	adds r0, r4, #0
	bl Func_080dbb78
	ldrb r3, [r0, #3]
	movs r0, #16
	ands r0, r3
	lsls r0, r0, #24
	lsrs r0, r0, #24
.L_080dbda4:
	add sp, #12
	pop {pc}
