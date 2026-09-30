.syntax unified
	.thumb
	.global Scheduler_RemoveCallback
	.thumb_func
Scheduler_RemoveCallback:
	push {r5, lr}
	ldr r4, .L_0801468c
	movs r5, #1
	negs r5, r5
	ldr r3, .L_08014690
	ldrh r2, [r3]
	strh r3, [r3]
	movs r1, #0
	ldr r3, [r4]
	cmp r3, r0
	bne .L_08014668
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	str r1, [r4]
	strh r3, [r4, #4]
	movs r5, #0
	b .L_08014684
.L_08014668:
	adds r1, #1
	adds r4, #8
	cmp r1, #23
	bgt .L_08014684
	ldr r3, [r4]
	cmp r3, r0
	bne .L_08014668
	movs r3, #0
	str r3, [r4]
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	strh r3, [r4, #4]
	adds r5, r1, #0
.L_08014684:
	ldr r3, .L_08014690
	strh r2, [r3]
	adds r0, r5, #0
	pop {r5, pc}
.L_0801468c:
	.4byte gSchedulerTaskTable
.L_08014690:
	.4byte 0x04000208
