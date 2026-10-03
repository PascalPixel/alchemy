.syntax unified
	.thumb
	.global Runtime_SetIrqHandler
	.thumb_func
Runtime_SetIrqHandler:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r1, r2, #0
	cmp r0, #13
	bhi .L_0801349c
	ldr r3, .L_080134a0
	ldrh r2, [r3]
	adds r6, r2, #0
	strh r3, [r3]
	movs r2, #1
	ldr r4, .L_080134a4
	lsls r2, r0
	ldrh r3, [r4]
	bics r3, r2
	cmp r1, #0
	beq .L_0801345a
	orrs r3, r2
.L_0801345a:
	strh r3, [r4]
	cmp r0, #2
	bhi .L_08013484
	movs r4, #8
	lsls r4, r0
	mvns r2, r4
	cmp r0, #2
	bne .L_08013472
	lsls r3, r5, #8
	orrs r4, r3
	movs r3, #255
	ands r2, r3
.L_08013472:
	movs r5, #128
	lsls r5, r5, #19
	adds r5, #4
	ldrh r3, [r5]
	ands r3, r2
	cmp r1, #0
	beq .L_08013482
	orrs r3, r4
.L_08013482:
	strh r3, [r5]
.L_08013484:
	cmp r1, #0
	beq .L_08013490
	ldr r2, .L_080134a8
	lsls r3, r0, #2
	str r1, [r2, r3]
	b .L_08013498
.L_08013490:
	ldr r1, .L_080134a8
	ldr r3, .L_080134ac
	lsls r2, r0, #2
	str r3, [r1, r2]
.L_08013498:
	ldr r3, .L_080134a0
	strh r6, [r3]
.L_0801349c:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080134a0:
	.4byte 0x04000208
.L_080134a4:
	.4byte 0x04000200
.L_080134a8:
	.4byte gIrqHandlers
.L_080134ac:
	.4byte Runtime_IgnoreInterrupt
