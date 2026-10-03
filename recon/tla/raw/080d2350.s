.syntax unified
	.thumb
	.global EventRuntime_End
	.thumb_func
EventRuntime_End:
	push {lr}
	ldr r0, .L_080d2388
	bl Scheduler_RemoveCallback
	ldr r2, .L_080d238c
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #118
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_080d237a
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	b .L_080d2382
.L_080d237a:
	movs r0, #8
	movs r1, #1
	bl Object_AttachWorkTargetToObject
.L_080d2382:
	bl GameFlag_RefreshLureCapFar
	pop {pc}
.L_080d2388:
	.4byte EventRuntime_UpdateWaitMode
.L_080d238c:
	.4byte gPartyState
	.4byte 0x00004770
