.syntax unified
	.thumb
	.global Func_080e0fb0
	.thumb_func
Func_080e0fb0:
	push {r5, r6, lr}
	ldr r3, .L_080e1018
	ldr r1, [r0, #20]
	movs r5, #253
	lsls r5, r5, #1
	movs r2, #160
	lsls r2, r2, #12
	adds r3, r3, r5
	adds r4, r1, r2
	movs r5, #0
	ldrsh r2, [r3, r5]
	ldr r3, .L_080e101c
	ldr r6, [r0, #104]
	cmp r2, r3
	bne .L_080e0fd4
	movs r2, #128
	lsls r2, r2, #11
	adds r4, r1, r2
.L_080e0fd4:
	ldr r5, [r0, #12]
	cmp r5, r4
	bgt .L_080e0fe0
	bl Func_080200c8
	b .L_080e1014
.L_080e0fe0:
	ldr r3, [r0, #24]
	movs r4, #192
	lsls r4, r4, #4
	movs r1, #128
	adds r2, r3, r4
	lsls r1, r1, #9
	cmp r2, r1
	ble .L_080e0ff2
	adds r2, r1, #0
.L_080e0ff2:
	negs r3, r2
	str r2, [r0, #24]
	str r3, [r0, #28]
	ldr r4, .L_080e1020
	ldr r3, [r6, #8]
	str r3, [r0, #8]
	adds r3, r5, r4
	str r3, [r0, #12]
	subs r3, r1, r2
	lsls r2, r3, #2
	adds r2, r2, r3
	ldr r3, [r6, #16]
	movs r5, #128
	subs r3, r3, r2
	lsls r5, r5, #13
	adds r3, r3, r5
	str r3, [r0, #16]
.L_080e1014:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080e1018:
	.4byte gPartyState
.L_080e101c:
	.4byte 0x00000001
.L_080e1020:
	.4byte 0xfffe0000
