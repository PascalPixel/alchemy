.syntax unified
	.thumb
	.global Func_0815b410
	.thumb_func
Func_0815b410:
	push {lr}
	movs r4, #160
	lsls r4, r4, #19
	movs r0, #0
	adds r4, #2
.L_0815b41a:
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	lsls r2, r3, #10
	lsls r1, r3, #5
	orrs r2, r1
	orrs r2, r3
	adds r0, #1
	strh r2, [r4]
	adds r4, #2
	cmp r0, #63
	bne .L_0815b41a
	pop {pc}
