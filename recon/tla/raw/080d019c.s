.syntax unified
	.thumb
	.global Func_080d019c
	.thumb_func
Func_080d019c:
	push {lr}
	movs r1, #168
	lsls r1, r1, #3
	movs r0, #124
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #0
	adds r4, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r4, #0
	ldr r2, .L_080d01c8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r4, #0
	add sp, #4
	pop {pc}
	.2byte 0x0000
.L_080d01c8:
	.4byte 0x85000150
