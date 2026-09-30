.syntax unified
	.thumb
	.global Func_080d1760
	.thumb_func
Func_080d1760:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r3, [r3]
	ldr r6, .L_080d1780
	adds r0, r3, #0
	ldr r5, .L_080d1784
	movs r3, #224
	movs r7, #248
	movs r4, #224
	lsls r3, r3, #4
	lsls r7, r7, #7
	lsls r4, r4, #1
	adds r1, r0, r3
	b .L_080d1788
.L_080d1780:
	.4byte 0x000003e0
.L_080d1784:
	.4byte 0x0000001f
.L_080d1788:
	ldrh r3, [r1]
	adds r2, r7, #0
	ands r2, r3
	ldrh r3, [r1, #2]
	subs r4, #1
	lsrs r3, r3, #5
	ands r3, r6
	orrs r2, r3
	ldrh r3, [r1, #4]
	adds r1, #6
	lsrs r3, r3, #10
	ands r3, r5
	orrs r2, r3
	strh r2, [r0]
	adds r0, #2
	cmp r4, #0
	bne .L_080d1788
	pop {r5, r6, r7, pc}
