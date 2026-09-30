.syntax unified
	.thumb
	.global Menu_ReleaseEntryObjects
	.thumb_func
Menu_ReleaseEntryObjects:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r6, #7
	adds r5, r3, #0
	adds r5, #248
.L_0810509c:
	ldmia r5!, {r0}
	cmp r0, #0
	beq .L_081050a6
	bl ResourceObject_ReleaseFar
.L_081050a6:
	subs r6, #1
	cmp r6, #0
	bge .L_0810509c
	ldr r0, .L_081050b4
	bl Scheduler_RemoveCallback
	pop {r5, r6, pc}
.L_081050b4:
	.4byte Menu_UpdateEntryObjectTransforms
