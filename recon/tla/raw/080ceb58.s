.syntax unified
	.thumb
	.global Func_080ceb58
	.thumb_func
Func_080ceb58:
	push {r5, lr}
	adds r5, r0, #0
	lsls r5, r5, #16
	lsrs r5, r5, #16
	adds r0, r5, #0
	bl Func_080ce31c
	adds r2, r0, #0
	movs r0, #240
	lsls r0, r0, #24
	adds r1, r5, #0
	adds r0, #5
	bl Func_080ce458
	adds r3, r0, #0
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	pop {r5, pc}
	.2byte 0x0000
