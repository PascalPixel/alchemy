.syntax unified
	.thumb
	.global Func_08144aac
	.thumb_func
Func_08144aac:
	push {r5, r6, lr}
	sub sp, #4
	adds r6, r1, #0
	cmp r0, #0
	bne .L_08144acc
	movs r5, #192
	movs r1, #19
	lsls r5, r5, #18
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r0, #188
	str r3, [r6]
	movs r1, #3
	b .L_08144ae0
.L_08144acc:
	movs r5, #192
	movs r1, #23
	lsls r5, r5, #18
	movs r0, #104
	bl Func_081963ec
	ldr r3, [r5, #104]
	movs r0, #188
	str r3, [r6]
	movs r1, #7
.L_08144ae0:
	adds r5, #188
	bl Func_081963ec
	ldr r3, [r5]
	str r3, [r6, #4]
	add sp, #4
	pop {r5, r6, pc}
	.2byte 0x0000
