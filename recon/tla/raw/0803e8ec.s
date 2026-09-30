.syntax unified
	.thumb
	.global NodeChain_GetNodeAtCount
	.thumb_func
NodeChain_GetNodeAtCount:
	push {lr}
	movs r2, #210
	movs r4, #192
	lsls r2, r2, #2
	lsls r4, r4, #2
	adds r3, r0, r2
	adds r4, #158
	ldr r2, [r3]
	adds r3, r0, r4
	ldrh r3, [r3]
	movs r1, #0
	cmp r3, #0
	beq .L_0803e912
	adds r3, r0, r4
	ldrh r0, [r3]
.L_0803e90a:
	adds r1, #1
	ldr r2, [r2, #4]
	cmp r1, r0
	bne .L_0803e90a
.L_0803e912:
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
