.syntax unified
	.thumb
	.global Func_080cc9c8
	.thumb_func
Func_080cc9c8:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r0, #1
	bl Func_080cc67c
	movs r2, #197
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_080cc9ee
	cmp r0, #0
	bne .L_080cc9f8
	bl Func_080cc7c4
.L_080cc9ee:
	cmp r0, #0
	bne .L_080cc9f8
	movs r0, #0
	bl Func_080cc67c
.L_080cc9f8:
	pop {r5, pc}
	.2byte 0x0000
