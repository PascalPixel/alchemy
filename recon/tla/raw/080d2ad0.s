.syntax unified
	.thumb
	.global EventRuntime_SetSavedDestination
	.thumb_func
EventRuntime_SetSavedDestination:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	strh r2, [r3]
	ldr r2, .L_080d2af8
	movs r4, #242
	lsls r4, r4, #1
	adds r3, r2, r4
	strh r0, [r3]
	movs r0, #243
	lsls r0, r0, #1
	adds r3, r2, r0
	strh r1, [r3]
	bx lr
.L_080d2af8:
	.4byte gPartyState
