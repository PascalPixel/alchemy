.syntax unified
	.thumb
	.global Func_08104d5c
	.thumb_func
Func_08104d5c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r5, r1, #0
	adds r3, #220
	ldr r7, [r3]
	adds r1, r2, #0
	adds r3, r5, #0
	cmp r5, #0
	bge .L_08104d72
	adds r3, r5, #3
.L_08104d72:
	asrs r3, r3, #2
	adds r0, r5, #0
	lsls r6, r3, #2
	bl Func_08104ccc
	movs r3, #148
	ldr r4, .L_08104d94
	ldr r0, .L_08104d98
	lsls r3, r3, #1
	movs r1, #0
	adds r2, r7, r3
.L_08104d88:
	adds r3, r6, r1
	cmp r3, r5
	bne .L_08104d9c
	strh r4, [r2]
	b .L_08104d9e
	.2byte 0x0000
.L_08104d94:
	.4byte 0x0000001e
.L_08104d98:
	.4byte 0x0000001a
.L_08104d9c:
	strh r0, [r2]
.L_08104d9e:
	adds r1, #1
	adds r2, #2
	cmp r1, #3
	ble .L_08104d88
	pop {r5, r6, r7, pc}
