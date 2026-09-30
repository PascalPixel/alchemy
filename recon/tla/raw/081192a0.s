.syntax unified
	.thumb
	.global Func_081192a0
	.thumb_func
Func_081192a0:
	push {r5, r6, lr}
	sub sp, #8
	mov r5, sp
	movs r6, #95
	adds r4, r5, #0
	adds r0, r5, #0
	movs r1, #3
.L_081192ae:
	ldrh r3, [r2]
	adds r2, #2
	strb r3, [r0]
	lsls r3, r3, #24
	adds r0, #1
	cmp r3, #0
	bne .L_081192be
	strb r6, [r4]
.L_081192be:
	subs r1, #1
	adds r4, #1
	cmp r1, #0
	bge .L_081192ae
	movs r3, #0
	strb r3, [r5, #4]
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
