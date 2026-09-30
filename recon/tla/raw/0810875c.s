.syntax unified
	.thumb
	.global Func_0810875c
	.thumb_func
Func_0810875c:
	push {lr}
	lsls r3, r1, #4
	adds r1, r3, #1
	cmp r0, #0
	ble .L_08108780
	ldr r4, .L_08108784
.L_08108768:
	ldrh r3, [r4]
	subs r0, #1
	adds r3, r2, r3
	adds r4, #2
	strb r1, [r3, #4]
	strb r1, [r3, #8]
	strb r1, [r3, #12]
	strb r1, [r3, #16]
	strb r1, [r3, #20]
	strb r1, [r3, #24]
	cmp r0, #0
	bne .L_08108768
.L_08108780:
	pop {pc}
	.2byte 0x0000
.L_08108784:
	.4byte Data_0810c348
