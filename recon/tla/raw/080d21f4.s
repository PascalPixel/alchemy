.syntax unified
	.thumb
	.global EventRuntime_UpdateWaitMode
	.thumb_func
EventRuntime_UpdateWaitMode:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	ldr r3, .L_080d2238
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080d2234
	ldr r0, .L_080d223c
	movs r2, #128
	ldr r3, [r0, #4]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080d221c
	movs r3, #220
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #0
	str r3, [r2]
.L_080d221c:
	ldr r3, [r0, #4]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080d2234
	movs r3, #220
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #1
	negs r3, r3
	str r3, [r2]
.L_080d2234:
	pop {pc}
	.2byte 0x0000
.L_080d2238:
	.4byte gDebugMode
.L_080d223c:
	.4byte gInput
