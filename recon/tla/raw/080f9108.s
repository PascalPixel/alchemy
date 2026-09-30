.syntax unified
	.thumb
	.global Func_080f9108
	.thumb_func
Func_080f9108:
	push {r5, lr}
	movs r3, #160
	lsls r0, r0, #5
	lsls r3, r3, #19
	adds r5, r0, r3
	movs r2, #128
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_080f9160
	adds r1, r5, #0
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, .L_080f9160
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldrh r2, [r5, #8]
	movs r0, #31
	lsls r3, r2, #16
	lsrs r4, r3, #26
	lsrs r1, r3, #21
	ldr r3, .L_080f915c
	adds r4, #9
	ands r1, r3
	ands r0, r2
	cmp r4, #31
	bls .L_080f914a
	movs r4, #31
.L_080f914a:
	adds r1, #9
	cmp r1, #31
	bls .L_080f9152
	movs r1, #31
.L_080f9152:
	adds r0, #9
	cmp r0, #31
	bls .L_080f9164
	movs r0, #31
	b .L_080f9164
.L_080f915c:
	.4byte 0x0000001f
.L_080f9160:
	.4byte 0x050001e0
.L_080f9164:
	lsls r3, r4, #10
	lsls r2, r1, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r5, #8]
	pop {r5, pc}
