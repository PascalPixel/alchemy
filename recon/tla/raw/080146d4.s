.syntax unified
	.thumb
	.global Scheduler_EnableUnmaskedOverlayCallbacks
	.thumb_func
Scheduler_EnableUnmaskedOverlayCallbacks:
	push {r5, r6, r7, lr}
	ldr r1, .L_08014714
	movs r5, #1
	negs r5, r5
	ldr r3, .L_08014718
	ldrh r2, [r3]
	adds r6, r2, #0
	strh r3, [r3]
	movs r4, #0
	movs r7, #1
.L_080146e8:
	ldrb r3, [r1, #3]
	cmp r3, #2
	bne .L_08014702
	ldrb r2, [r1, #6]
	adds r3, r7, #0
	ands r3, r2
	movs r0, #1
	cmp r3, #0
	bne .L_08014702
	ldrb r3, [r1, #5]
	orrs r3, r0
	strb r3, [r1, #5]
	adds r5, r4, #0
.L_08014702:
	adds r4, #1
	adds r1, #8
	cmp r4, #23
	ble .L_080146e8
	ldr r3, .L_08014718
	strh r6, [r3]
	adds r0, r5, #0
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08014714:
	.4byte gSchedulerTaskTable
.L_08014718:
	.4byte 0x04000208
