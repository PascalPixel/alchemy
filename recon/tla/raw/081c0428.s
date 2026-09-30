.syntax unified
	.thumb
	.global Func_081c0428
Func_081c0428:
	.global Pcm_KeyToFrequencyWorkCopy
	.thumb_func
Pcm_KeyToFrequencyWorkCopy:
	push {r4, r5, r6, r7, lr}
	mov r12, r0
	lsls r1, r1, #24
	lsrs r6, r1, #24
	lsls r7, r2, #24
	cmp r6, #178
	bls .L_081c043c
	movs r6, #178
	movs r7, #255
	lsls r7, r7, #24
.L_081c043c:
	ldr r3, .L_081c0484
	adds r0, r6, r3
	ldrb r5, [r0]
	ldr r4, .L_081c0488
	movs r2, #15
	adds r0, r5, #0
	ands r0, r2
	lsls r0, r0, #2
	adds r0, r0, r4
	lsrs r1, r5, #4
	ldr r5, [r0]
	lsrs r5, r1
	adds r0, r6, #1
	adds r0, r0, r3
	ldrb r1, [r0]
	adds r0, r1, #0
	ands r0, r2
	lsls r0, r0, #2
	adds r0, r0, r4
	lsrs r1, r1, #4
	ldr r0, [r0]
	lsrs r0, r1
	mov r1, r12
	ldr r4, [r1, #4]
	subs r0, r0, r5
	adds r1, r7, #0
	bl Func_081bf80c
	adds r1, r0, #0
	adds r1, r5, r1
	adds r0, r4, #0
	bl Func_081bf80c
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
.L_081c0484:
	.4byte 0x00000000
.L_081c0488:
	.4byte 0x00000000
