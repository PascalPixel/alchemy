.syntax unified
	.thumb
	.global Func_080d1dac
	.thumb_func
Func_080d1dac:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, .L_080d1dec
	movs r4, #189
	lsls r4, r4, #1
	adds r0, r0, r4
	ldrh r0, [r0]
	sub sp, #4
	lsrs r0, r0, #5
	str r0, [sp, #0]
	adds r0, r5, #0
	bl Func_080eaf98
	movs r3, #0
	strh r3, [r5, #30]
	ldrb r3, [r5, #9]
	movs r2, #13
	negs r2, r2
	ldrb r1, [r5, #5]
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	movs r3, #33
	negs r3, r3
	ands r3, r1
	strb r3, [r5, #5]
	movs r3, #15
	ands r2, r3
	strb r2, [r5, #9]
	add sp, #4
	pop {r5, pc}
	.2byte 0x0000
.L_080d1dec:
	.4byte ResourceTableEntries
