.syntax unified
	.thumb
	.global Scheduler_DisableOverlayCallbacks
	.thumb_func
Scheduler_DisableOverlayCallbacks:
	push {r5, r6, lr}
	ldr r4, .L_080147d0
	movs r0, #1
	negs r0, r0
	ldr r3, .L_080147d4
	ldrh r2, [r3]
	adds r6, r2, #0
	strh r3, [r3]
	movs r1, #0
	movs r5, #254
.L_080147b0:
	ldrb r3, [r4, #3]
	cmp r3, #2
	bne .L_080147c0
	ldrb r2, [r4, #5]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r4, #5]
	adds r0, r1, #0
.L_080147c0:
	adds r1, #1
	adds r4, #8
	cmp r1, #23
	ble .L_080147b0
	ldr r3, .L_080147d4
	strh r6, [r3]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080147d0:
	.4byte gSchedulerTaskTable
.L_080147d4:
	.4byte 0x04000208
