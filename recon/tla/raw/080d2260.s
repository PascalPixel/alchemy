.syntax unified
	.thumb
	.global EventRuntime_PrepareCurrentObject
	.thumb_func
EventRuntime_PrepareCurrentObject:
	push {lr}
	bl EventRuntime_GetControlledOwner
	bl ObjectTable_Get
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #56]
	str r3, [r0, #64]
	movs r3, #0
	str r3, [r0, #36]
	str r3, [r0, #44]
	movs r2, #128
	ldr r3, .L_080d22a4
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	ldrb r3, [r3]
	cmp r3, #2
	bne .L_080d229c
	movs r1, #12
	bl Object_SetMode
	b .L_080d22a2
.L_080d229c:
	movs r1, #1
	bl Object_SetMode
.L_080d22a2:
	pop {pc}
.L_080d22a4:
	.4byte gPartyState
