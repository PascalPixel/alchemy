.syntax unified
	.thumb
	.global Func_0810a6b8
	.thumb_func
Func_0810a6b8:
	push {r5, lr}
	lsls r3, r0, #5
	adds r5, r1, #0
	ldr r1, .L_0810a6f4
	adds r3, r3, r0
	lsls r2, r3, #1
	ldrsh r3, [r1, r2]
	movs r4, #0
	cmp r3, #0
	beq .L_0810a6e6
	adds r0, r5, #0
	adds r2, r2, r1
.L_0810a6d0:
	ldrh r3, [r2]
	adds r4, #1
	strh r3, [r0]
	adds r2, #2
	adds r0, #2
	cmp r4, #23
	bgt .L_0810a6e6
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, #0
	bne .L_0810a6d0
.L_0810a6e6:
	ldr r3, .L_0810a6f0
	lsls r2, r4, #1
	strh r3, [r2, r5]
	adds r0, r4, #0
	pop {r5, pc}
.L_0810a6f0:
	.4byte 0x00000000
.L_0810a6f4:
	.4byte Data_0810c3f4
