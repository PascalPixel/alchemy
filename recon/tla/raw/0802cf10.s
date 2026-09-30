.syntax unified
	.thumb
	.global Func_0802cf10
	.thumb_func
Func_0802cf10:
	push {r5, r6, r7, lr}
	lsls r2, r2, #16
	lsls r3, r3, #16
	asrs r7, r2, #16
	asrs r2, r3, #16
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #112]
	lsls r1, r1, #16
	adds r5, r4, #0
	adds r5, #176
	asrs r6, r1, #16
	ldrh r1, [r5]
	lsls r0, r0, #16
	asrs r0, r0, #16
	cmp r1, #3
	bls .L_0802cf38
	movs r0, #1
	negs r0, r0
	b .L_0802cf7a
.L_0802cf38:
	movs r3, #44
	muls r1, r3
	lsls r0, r0, #16
	lsls r3, r6, #16
	lsrs r3, r3, #16
	lsrs r0, r0, #12
	adds r0, r0, r3
	movs r3, #160
	lsls r3, r3, #19
	lsls r0, r0, #1
	adds r1, r4, r1
	adds r0, r0, r3
	movs r3, #0
	strh r3, [r1, #4]
	strh r3, [r1, #6]
	lsls r2, r2, #16
	movs r4, #128
	movs r3, #128
	lsrs r2, r2, #16
	lsls r4, r4, #24
	lsls r3, r3, #19
	strh r2, [r1, #10]
	str r0, [r1]
	strh r7, [r1, #8]
	adds r3, #212
	adds r1, #12
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldrh r3, [r5]
	movs r0, #0
	adds r3, #1
	strh r3, [r5]
.L_0802cf7a:
	pop {r5, r6, r7, pc}
