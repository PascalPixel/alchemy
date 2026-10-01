.syntax unified
	.thumb
	.global Scheduler_SetCallbackMask
	.thumb_func
Scheduler_SetCallbackMask:
	push {r5, r6, lr}
	adds r6, r1, #0
	ldr r1, .L_08014754
	movs r5, #1
	negs r5, r5
	ldr r3, .L_08014758
	ldrh r2, [r3]
	strh r3, [r3]
	movs r4, #0
	ldr r3, [r1]
	cmp r3, r0
	bne .L_0801473a
	strb r6, [r1, #6]
	movs r5, #0
	b .L_0801474c
.L_0801473a:
	adds r4, #1
	adds r1, #8
	cmp r4, #23
	bgt .L_0801474c
	ldr r3, [r1]
	cmp r3, r0
	bne .L_0801473a
	strb r6, [r1, #6]
	adds r5, r4, #0
.L_0801474c:
	ldr r3, .L_08014758
	strh r2, [r3]
	adds r0, r5, #0
	pop {r5, r6, pc}
.L_08014754:
	.4byte gSchedulerTaskTable
.L_08014758:
	.4byte 0x04000208
