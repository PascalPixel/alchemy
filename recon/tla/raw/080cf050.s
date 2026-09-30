.syntax unified
	.thumb
	.global Func_080cf050
	.thumb_func
Func_080cf050:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl Func_080cdf5c
	bl ObjectTable_Get
	ldr r2, .L_080cf0c8
	movs r3, #128
	lsls r3, r3, #24
	adds r5, r0, #0
	movs r7, #0
	mov r8, r2
	mov r10, r3
.L_080cf06e:
	adds r0, r7, #0
	adds r0, #64
	bl ObjectTable_Get
	adds r4, r0, #0
	ldr r3, [r5, #8]
	ldr r1, [r4, #8]
	ldr r2, [r5, #16]
	subs r6, r3, r1
	ldr r3, [r4, #16]
	subs r0, r2, r3
	ldr r2, .L_080cf0cc
	adds r3, r6, r2
	cmp r3, r8
	bhi .L_080cf0ba
	adds r3, r0, r2
	cmp r3, r8
	bhi .L_080cf0ba
	str r1, [r5, #8]
	adds r1, r6, #0
	ldr r3, [r4, #16]
	str r3, [r5, #16]
	bl ArcTan2
	adds r1, r0, #0
	lsls r1, r1, #16
	movs r0, #160
	adds r2, r5, #0
	lsrs r1, r1, #16
	lsls r0, r0, #13
	adds r2, #8
	bl Vector_AddPolarOffset
	mov r3, r10
	str r3, [r5, #56]
	str r3, [r5, #60]
	str r3, [r5, #64]
	b .L_080cf0c0
.L_080cf0ba:
	adds r7, #1
	cmp r7, #15
	ble .L_080cf06e
.L_080cf0c0:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080cf0c8:
	.4byte 0x001ffffe
.L_080cf0cc:
	.4byte 0x000fffff
