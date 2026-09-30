.syntax unified
	.thumb
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	push {lr}
	movs r1, #192
	lsls r1, r1, #18
	adds r3, r0, #3
	ldr r2, [r1]
	lsrs r3, r3, #2
	lsls r0, r3, #2
	movs r4, #129
	adds r3, r2, r0
	lsls r4, r4, #18
	cmp r3, r4
	bcc .L_08014dd6
	ldr r2, [r1, #4]
	ldr r3, .L_08014ddc
	adds r0, r2, r0
	cmp r0, r3
	bls .L_08014dd2
	movs r0, #0
	b .L_08014dda
.L_08014dd2:
	str r0, [r1, #4]
	b .L_08014dd8
.L_08014dd6:
	str r3, [r1]
.L_08014dd8:
	adds r0, r2, #0
.L_08014dda:
	pop {pc}
.L_08014ddc:
	.4byte Data_03006fbf
	.4byte 0x00004770
