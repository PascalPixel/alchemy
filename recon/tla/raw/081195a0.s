.syntax unified
	.thumb
	.global Func_081195a0
	.thumb_func
Func_081195a0:
	push {r5, r6, lr}
	sub sp, #16
	mov r5, sp
	movs r3, #192
	lsls r3, r3, #18
	adds r0, r5, #0
	ldr r6, [r3, #36]
	bl Func_0811a038
	movs r1, #0
	adds r4, r0, #0
	cmp r1, r4
	bge .L_081195ce
	adds r0, r5, #0
.L_081195bc:
	ldrh r2, [r0]
	adds r3, r1, #0
	adds r2, #72
	subs r3, #128
	adds r1, #1
	adds r0, #2
	strb r3, [r6, r2]
	cmp r1, r4
	blt .L_081195bc
.L_081195ce:
	add sp, #16
	pop {r5, r6, pc}
	.2byte 0x0000
