.syntax unified
	.thumb
	.global Func_080425fc
	.thumb_func
Func_080425fc:
	push {r5, r6, lr}
	adds r5, r3, #0
	movs r4, #0
	ldr r6, [sp, #12]
	ldr r0, .L_0804262c
	cmp r4, r5
	bcs .L_08042628
	movs r3, #32
	subs r3, r3, r2
	lsls r3, r3, #1
.L_08042610:
	movs r1, #0
	cmp r1, r2
	bcs .L_08042620
.L_08042616:
	adds r1, #1
	strh r6, [r0]
	adds r0, #2
	cmp r1, r2
	bcc .L_08042616
.L_08042620:
	adds r4, #1
	adds r0, r0, r3
	cmp r4, r5
	bcc .L_08042610
.L_08042628:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0804262c:
	.4byte 0x06002000
