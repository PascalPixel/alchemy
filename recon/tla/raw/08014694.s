.syntax unified
	.thumb
	.global Func_08014694
	.thumb_func
Func_08014694:
	push {r5, r6, lr}
	ldr r4, .L_080146cc
	movs r5, #1
	negs r5, r5
	ldr r3, .L_080146d0
	ldrh r2, [r3]
	adds r6, r2, #0
	strh r3, [r3]
	movs r1, #0
	movs r2, #1
.L_080146a8:
	cmp r0, #0
	beq .L_080146b2
	ldr r3, [r4]
	cmp r3, r0
	bne .L_080146ba
.L_080146b2:
	ldrb r3, [r4, #5]
	orrs r3, r2
	strb r3, [r4, #5]
	adds r5, r1, #0
.L_080146ba:
	adds r1, #1
	adds r4, #8
	cmp r1, #23
	ble .L_080146a8
	ldr r3, .L_080146d0
	strh r6, [r3]
	adds r0, r5, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080146cc:
	.4byte gSchedulerTaskTable
.L_080146d0:
	.4byte 0x04000208
