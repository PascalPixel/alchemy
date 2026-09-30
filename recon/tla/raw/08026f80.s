.syntax unified
	.thumb
	.global Func_08026f80
	.thumb_func
Func_08026f80:
	push {r5, r6, lr}
	ldmia r0!, {r4}
	ldmia r2!, {r5}
	adds r1, r1, r3
	subs r4, r4, r5
	asrs r6, r4, #16
	ldmia r2!, {r5}
	ldmia r0!, {r4}
	ldr r2, [r2]
	ldr r0, [r0]
	movs r3, #128
	subs r4, r4, r5
	subs r0, r0, r2
	lsls r3, r3, #15
	asrs r4, r4, #16
	asrs r0, r0, #16
	cmp r6, r3
	bgt .L_08026fc2
	cmp r0, r3
	bgt .L_08026fc2
	adds r2, r4, #0
	muls r2, r4
	adds r3, r6, #0
	muls r3, r6
	adds r3, r3, r2
	adds r2, r0, #0
	muls r2, r0
	adds r3, r3, r2
	adds r2, r1, #0
	muls r2, r1
	movs r0, #0
	cmp r3, r2
	blt .L_08026fc6
.L_08026fc2:
	movs r0, #1
	negs r0, r0
.L_08026fc6:
	pop {r5, r6, pc}
