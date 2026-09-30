.syntax unified
	.thumb
	.global Func_08196a28
	.thumb_func
Func_08196a28:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	ldrb r3, [r5]
	movs r6, #1
	lsls r6, r3
	ldrb r3, [r5, #1]
	adds r7, r1, #0
	lsls r6, r3
	bl Func_08014ca0
	adds r2, r6, #0
	adds r2, #8
	cmp r0, r2
	bcs .L_08196a48
	movs r0, #0
	b .L_08196a74
.L_08196a48:
	ldrh r3, [r5]
	adds r0, r2, #0
	strh r3, [r7]
	bl Runtime_BumpAllocate
	str r0, [r7, #4]
	movs r3, #4
	ldr r1, [r5, #4]
	movs r5, #3
	ands r5, r1
	adds r2, r6, r5
	negs r3, r3
	adds r2, #3
	ands r2, r3
	subs r1, r1, r5
	ldr r3, .L_08196a78
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r7, #4]
	movs r0, #1
	adds r3, r3, r5
	str r3, [r7, #4]
.L_08196a74:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08196a78:
	.4byte IwramCopyWords
