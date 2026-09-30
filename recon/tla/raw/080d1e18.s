.syntax unified
	.thumb
	.global Func_080d1e18
	.thumb_func
Func_080d1e18:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, .L_080d1e5c
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
	movs r0, #13
	ldrb r3, [r5, #9]
	negs r0, r0
	ldrb r2, [r5, #5]
	adds r1, r0, #0
	ands r1, r3
	movs r3, #33
	negs r3, r3
	ands r3, r2
	movs r2, #15
	ands r1, r2
	ands r3, r0
	movs r2, #4
	orrs r3, r2
	strb r1, [r5, #9]
	strb r3, [r5, #5]
	add sp, #4
	pop {r5, pc}
	.2byte 0x0000
.L_080d1e5c:
	.4byte ResourceTableEntries
