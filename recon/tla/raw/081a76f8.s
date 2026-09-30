.syntax unified
	.thumb
	.global Func_081a76f8
	.thumb_func
Func_081a76f8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r2, .L_081a7760
	movs r3, #1
	strb r3, [r2]
	ldr r6, .L_081a7764
	bl Func_080144c0
	movs r0, #1
	bl Blend_SetDarkenTarget16
	bl Func_08014b70
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_081a7758
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r3, .L_081a775c
	subs r2, #12
	strh r3, [r2]
	ldr r3, .L_081a7768
	movs r5, #0
	strh r5, [r3, #10]
	ldr r5, .L_081a776c
	adds r0, r6, #0
	mov r8, r3
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_0801587c
	movs r3, #128
	movs r2, #132
	adds r6, r5, #0
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r6, #0
	lsls r1, r1, #19
	adds r2, #112
	b .L_081a7770
	.2byte 0x0000
.L_081a7758:
	.4byte 0x00000685
.L_081a775c:
	.4byte 0x00001440
.L_081a7760:
	.4byte Data_0300120c
.L_081a7764:
	.4byte 0x00000021
.L_081a7768:
	.4byte Data_03001120
.L_081a776c:
	.4byte gMapCellBuffer
.L_081a7770:
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #224
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r6, #0
	ldr r1, .L_081a7808
	ldr r2, .L_081a780c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #128
	lsls r3, r3, #4
	adds r6, r6, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r6, #0
	ldr r1, .L_081a7810
	ldr r2, .L_081a7814
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #128
	lsls r3, r3, #7
	adds r6, r6, r3
	movs r5, #0
	movs r2, #0
	mov r3, r8
.L_081a77ac:
	adds r5, #1
	strh r2, [r3, #2]
	strh r2, [r3]
	adds r3, #4
	cmp r5, #3
	bls .L_081a77ac
	movs r3, #128
	movs r1, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_081a7818
	adds r1, #16
	adds r2, #4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Func_08014bac
	bl Func_08014b70
	movs r0, #1
	bl Func_08013f3c
	bl Func_08013fdc
	ldr r3, .L_081a7804
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_081a781c
	movs r2, #132
	ldr r0, [r3]
	movs r3, #3
	lsrs r0, r0, #3
	ands r0, r3
	movs r3, #128
	lsls r0, r0, #10
	lsls r3, r3, #19
	lsls r2, r2, #24
	movs r5, #0
	b .L_081a7840
	.2byte 0x0000
.L_081a7804:
	.4byte 0x00001540
.L_081a7808:
	.4byte 0x06003000
.L_081a780c:
	.4byte 0x84000200
.L_081a7810:
	.4byte 0x06004000
.L_081a7814:
	.4byte 0x84001000
.L_081a7818:
	.4byte Data_03001120
.L_081a781c:
	.4byte Data_0300122c
.L_081a7820:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #119
	bhi .L_081a7858
	ldr r3, .L_081a7860
	movs r2, #132
	ldr r0, [r3]
	movs r3, #3
	lsrs r0, r0, #3
	ands r0, r3
	movs r3, #128
	lsls r0, r0, #10
	lsls r3, r3, #19
	lsls r2, r2, #24
.L_081a7840:
	adds r3, #212
	adds r0, r0, r6
	ldr r1, .L_081a7864
	adds r2, #208
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_081a7868
	movs r2, #9
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_081a7820
.L_081a7858:
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_081a7860:
	.4byte Data_0300122c
.L_081a7864:
	.4byte 0x06004100
.L_081a7868:
	.4byte gInput
	.global Data_081a786c
Data_081a786c:
	.4byte 0x00004770
