.syntax unified
	.thumb
	.global Func_0804bbd0
	.thumb_func
Func_0804bbd0:
	push {r5, r6, lr}
	mov r6, r9
	push {r6}
	sub sp, #132
	mov r2, r9
	mov r6, sp
	add r3, sp, #128
	adds r5, r2, #0
	str r2, [r3]
	adds r1, r6, #0
	movs r2, #52
	subs r5, #8
	ldr r0, .L_0804bc04
	bl UiText_CopyMessageString
	ldr r3, [r5]
	adds r0, r6, #0
	ldr r1, [r3, #68]
	movs r2, #0
	movs r3, #4
	bl Func_0803aae4
	add sp, #132
	pop {r3}
	mov r9, r3
	pop {r5, r6, pc}
.L_0804bc04:
	.4byte 0x00000c59
