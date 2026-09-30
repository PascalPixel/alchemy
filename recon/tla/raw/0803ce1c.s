.syntax unified
	.thumb
	.global Func_0803ce1c
	.thumb_func
Func_0803ce1c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r2, #215
	lsls r2, r2, #3
	adds r5, r3, r2
	movs r6, #0
.L_0803ce2c:
	ldr r0, [r5]
	cmp r0, #0
	beq .L_0803ce52
	ldr r3, [r0, #24]
	cmp r3, #0
	bne .L_0803ce52
	ldrh r2, [r0, #22]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0803ce52
	ldrh r3, [r0, #20]
	cmp r3, #0
	beq .L_0803ce52
	movs r1, #2
	ands r1, r2
	lsls r1, r1, #16
	lsrs r1, r1, #16
	bl UiWork_Finalize
.L_0803ce52:
	adds r6, #1
	adds r5, #40
	cmp r6, #3
	bne .L_0803ce2c
	movs r0, #10
	bl WaitFrames
	pop {r5, r6, pc}
	.2byte 0x0000
