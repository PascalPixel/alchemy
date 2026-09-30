.syntax unified
	.thumb
	.global Func_08105468
	.thumb_func
Func_08105468:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, .L_08105494
	ldr r5, [r3]
	bl Scheduler_RemoveCallback
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #8
	adds r5, r5, r3
	movs r6, #4
.L_08105482:
	ldrh r0, [r5]
	subs r6, #1
	adds r5, #2
	bl Resource_ResetEntry
	cmp r6, #0
	bge .L_08105482
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08105494:
	.4byte Func_08105370
