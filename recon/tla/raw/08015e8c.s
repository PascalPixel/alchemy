.syntax unified
	.thumb
	.global Func_08015e8c
	.thumb_func
Func_08015e8c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	ldr r5, [r3]
	bl Func_08015f0c
	cmp r0, #14
	bls .L_08015ea2
	movs r0, #1
	b .L_08015ec4
.L_08015ea2:
	movs r3, #197
	lsls r3, r3, #6
	adds r2, r5, r3
	movs r3, #0
	strh r3, [r2]
	movs r3, #196
	lsls r3, r3, #6
	adds r3, #66
	adds r2, r5, r3
	movs r3, #3
	strh r3, [r2]
	bl Func_08015f48
	adds r3, r0, #0
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
.L_08015ec4:
	pop {r5, pc}
	.2byte 0x0000
