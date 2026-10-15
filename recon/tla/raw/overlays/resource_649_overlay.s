.syntax unified
	.thumb
	.section .text.x0200805c,"ax",%progbits
	.global Func_0200005c
	.thumb_func
Func_0200005c:
	push {r5, r6, lr}
	ldr r5, .L_020081e8
	movs r2, #241
	lsls r2, r2, #1
	adds r6, r5, r2
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #10
	bne .L_020080ba
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #75
	bl Func_02000644
	movs r0, #0
	bl Func_0200031c
	movs r0, #120
	bl WaitFrames
	movs r5, #0
	b .L_02008098
.L_02008096:
	adds r5, #1
.L_02008098:
	movs r3, #209
	lsls r3, r3, #4
	adds r3, #255
	cmp r5, r3
	bgt .L_020080b0
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_020081ec
	ldr r3, [r3, #4]
	cmp r3, #0
	beq .L_02008096
.L_020080b0:
	ldr r0, .L_020081f0
	movs r1, #2
	bl Func_02000604
	b .L_020081e2
.L_020080ba:
	cmp r3, #8
	bne .L_020080f0
	movs r0, #68
	bl Func_02000644
	movs r0, #1
	bl Func_0200061c
	movs r0, #78
	bl Func_02000644
	movs r0, #60
	bl Blend_SetDarkenTarget16
	bl Blend_WaitForTransition
	movs r0, #240
	bl Battle_WaitMode0
	movs r0, #0
	bl Func_02000644
	ldr r0, .L_020081f0
	movs r1, #2
	bl Func_02000604
	b .L_020081e2
.L_020080f0:
	ldr r0, .L_020081f4
	bl Func_020005bc
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #1
	bne .L_02008162
.L_020080fe:
	movs r0, #0
	bl Func_02000644
	movs r0, #0
	bl Func_0200062c
	movs r0, #0
	bl Func_02000634
	bl Func_020005d4
	cmp r0, #0
	ble .L_02008158
	movs r0, #40
	bl Func_02000644
	movs r0, #1
	bl Func_0200063c
	movs r0, #1
	bl Func_02000624
	cmp r0, #0
	bne .L_02008158
	movs r0, #78
	bl Func_02000644
	movs r0, #30
	bl Blend_SetDarkenTarget16
	bl Blend_WaitForTransition
	movs r5, #0
	b .L_02008144
.L_02008142:
	adds r5, #1
.L_02008144:
	cmp r5, #119
	bgt .L_020080fe
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_020081ec
	ldr r3, [r3, #4]
	cmp r3, #0
	beq .L_02008142
	b .L_020080fe
.L_02008158:
	ldr r0, .L_020081f8
	movs r1, #1
	bl Func_02000604
	b .L_020081c6
.L_02008162:
	cmp r3, #2
	bne .L_02008192
	movs r0, #40
	bl Func_02000644
	movs r0, #0
	bl Func_0200063c
	movs r2, #147
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	adds r2, #1
	ldrb r0, [r3]
	adds r3, r5, r2
	ldrb r1, [r3]
	bl Func_020005dc
	bl Func_020005e4
	ldr r0, .L_020081fc
	movs r1, #99
	bl Func_02000604
.L_02008192:
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #3
	bne .L_020081c6
	movs r0, #75
	bl Func_02000644
	movs r0, #0
	bl Func_02000624
	movs r2, #147
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	adds r2, #1
	ldrb r0, [r3]
	adds r3, r5, r2
	ldrb r1, [r3]
	bl Func_020005dc
	bl Func_020005ec
	ldr r0, .L_02008200
	movs r1, #13
	bl Func_02000604
.L_020081c6:
	movs r0, #78
	bl Func_02000644
	movs r0, #30
	bl Blend_SetDarkenTarget16
	bl Blend_WaitForTransition
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #0
	bl Func_02000644
.L_020081e2:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020081e8:
	.4byte gPartyState
.L_020081ec:
	.4byte gInput
.L_020081f0:
	.4byte 0x00000000
.L_020081f4:
	.4byte 0x0000000b
.L_020081f8:
	.4byte 0x00000001
.L_020081fc:
	.4byte 0x00000005
.L_02008200:
	.4byte 0x00000009
	.section .text.x02008278,"ax",%progbits
	.global Func_02000278
	.thumb_func
Func_02000278:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008308
	ldr r2, .L_0200830c
	movs r1, #0
	ldrsh r3, [r3, r1]
	ldr r4, .L_02008310
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #4
	lsrs r5, r3, #5
	movs r7, #0
	adds r6, r4, #0
.L_02008292:
	movs r2, #18
	subs r2, r2, r7
	lsls r2, r2, #3
	movs r3, #232
	subs r3, r3, r2
	movs r2, #0
	movs r1, #136
	str r2, [r6]
	lsls r3, r3, #16
	movs r2, #132
	orrs r3, r1
	lsls r2, r2, #8
	orrs r3, r2
	str r3, [r6, #4]
	movs r3, #240
	lsls r3, r3, #8
	orrs r3, r5
	str r3, [r6, #8]
	ldr r3, .L_02008314
	adds r6, #12
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	subs r1, r2, r7
	cmp r1, #0
	bge .L_020082ce
	movs r1, #0
.L_020082ce:
	cmp r1, #2
	bgt .L_020082e0
	ldr r3, .L_02008318
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020082e0
	movs r1, #0
.L_020082e0:
	cmp r1, #0
	beq .L_020082f2
	adds r0, r4, #0
	movs r1, #255
	adds r4, #12
	str r4, [sp, #0]
	bl Func_020005ac
	ldr r4, [sp, #0]
.L_020082f2:
	adds r7, #1
	adds r5, #2
	cmp r7, #17
	ble .L_02008292
	ldr r2, .L_02008314
	add sp, #4
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008308:
	.4byte gTitleVramBlock
.L_0200830c:
	.4byte ResourceTableEntries
.L_02008310:
	.4byte Data_020006f0
.L_02008314:
	.4byte Data_020006dc
.L_02008318:
	.4byte gFrameCount
	.section .bss,"aw",%nobits
	.space 0x00000036
	.global Data_020006dc
Data_020006dc:
	.space 0x00000014
	.global Data_020006f0
Data_020006f0:
