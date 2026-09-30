.syntax unified
	.thumb
	.global Func_080e0cac
	.thumb_func
Func_080e0cac:
	push {lr}
	ldr r1, .L_080e0ce8
	ldr r3, [r0, #28]
	ldr r2, [r0, #24]
	adds r3, r3, r1
	str r3, [r0, #28]
	ldrh r3, [r0, #6]
	adds r2, r2, r1
	movs r1, #128
	lsls r1, r1, #6
	adds r3, r3, r1
	strh r3, [r0, #6]
	ldr r3, [r0, #12]
	movs r1, #128
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r0, #12]
	movs r3, #192
	lsls r3, r3, #6
	str r2, [r0, #24]
	cmp r2, r3
	bge .L_080e0ce0
	ldr r3, .L_080e0ce4
	adds r2, r0, #0
	adds r2, #84
	strb r3, [r2]
.L_080e0ce0:
	pop {pc}
	.2byte 0x0000
.L_080e0ce4:
	.4byte 0x00000000
.L_080e0ce8:
	.4byte 0xfffffd80
