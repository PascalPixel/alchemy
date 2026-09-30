.syntax unified
	.thumb
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	push {lr}
	movs r1, #192
	lsls r1, r1, #18
	adds r3, r0, #3
	ldr r2, [r1, #4]
	lsrs r3, r3, #2
	ldr r4, .L_08014da8
	lsls r0, r3, #2
	adds r3, r2, r0
	cmp r3, r4
	bls .L_08014da2
	ldr r2, [r1]
	movs r3, #129
	adds r0, r2, r0
	lsls r3, r3, #18
	cmp r0, r3
	bcc .L_08014d9e
	movs r0, #0
	b .L_08014da6
.L_08014d9e:
	str r0, [r1]
	b .L_08014da4
.L_08014da2:
	str r3, [r1, #4]
.L_08014da4:
	adds r0, r2, #0
.L_08014da6:
	pop {pc}
.L_08014da8:
	.4byte Data_03006fbf
