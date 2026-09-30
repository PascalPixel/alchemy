.syntax unified
	.thumb
	.global Func_080cc9fc
	.thumb_func
Func_080cc9fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #0
	movs r2, #1
	mov r9, r1
	ldr r0, .L_080cca34
	ldr r1, .L_080cca38
	sub sp, #12
	mov r10, r2
	mov r8, r2
	bl Resource_DecompressHalfwords
	movs r3, #0
	str r3, [sp, #8]
	mov r11, r3
	b .L_080cca48
.L_080cca26:
	mov r1, r9
	mov r2, r9
	lsls r1, r1, #12
	lsls r2, r2, #5
	str r1, [sp, #8]
	mov r11, r2
	b .L_080cca48
.L_080cca34:
	.4byte Debug_PaletteSwatchTiles
.L_080cca38:
	.4byte 0x06001a00
.L_080cca3c:
	mov r3, r9
	lsls r3, r3, #12
	mov r1, r9
	str r3, [sp, #8]
	lsls r1, r1, #5
.L_080cca46:
	mov r11, r1
.L_080cca48:
	ldr r3, .L_080cca90
	ldr r1, .L_080cca94
	add r3, r9
	ldr r0, [sp, #8]
	strh r3, [r1]
	ldr r3, .L_080cca7c
	adds r2, r1, #0
	adds r2, #64
	strh r3, [r2]
	ldr r3, .L_080cca80
	adds r2, #64
	strh r3, [r2]
	ldr r3, .L_080cca84
	movs r7, #160
	adds r2, #64
	lsls r7, r7, #19
	ldr r5, .L_080cca88
	ldr r4, .L_080cca8c
	adds r7, #2
	strh r3, [r2]
	movs r2, #1
	adds r0, #209
	add r7, r11
	adds r1, #2
	mov r12, r2
	b .L_080cca98
.L_080cca7c:
	.4byte 0x0000f052
.L_080cca80:
	.4byte 0x0000f047
.L_080cca84:
	.4byte 0x0000f042
.L_080cca88:
	.4byte 0x0000001f
.L_080cca8c:
	.4byte 0x0000f0e0
.L_080cca90:
	.4byte 0xfffff0e0
.L_080cca94:
	.4byte 0x0600205a
.L_080cca98:
	strh r0, [r1]
	adds r2, r1, #0
	ldrh r6, [r7]
	adds r2, #64
	adds r3, r6, #0
	ands r3, r5
	adds r3, r3, r4
	strh r3, [r2]
	lsrs r3, r6, #5
	ands r3, r5
	adds r2, #64
	adds r3, r3, r4
	strh r3, [r2]
	lsrs r3, r6, #10
	ands r3, r5
	adds r2, #64
	adds r3, r3, r4
	strh r3, [r2]
	movs r3, #1
	add r12, r3
	mov r2, r12
	adds r0, #1
	adds r7, #2
	adds r1, #2
	cmp r2, #15
	bls .L_080cca98
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080ccc3c
	movs r4, #31
.L_080ccad6:
	ldr r2, [r1, #12]
	movs r3, #64
	ands r2, r3
	cmp r2, #0
	beq .L_080ccaee
	subs r3, #65
	add r10, r3
	mov r2, r10
	cmp r2, #0
	bgt .L_080ccaee
	movs r3, #3
	mov r10, r3
.L_080ccaee:
	ldr r2, [r1, #12]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_080ccb04
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #3
	ble .L_080ccb04
	mov r10, r2
.L_080ccb04:
	ldr r2, [r1, #12]
	movs r3, #32
	ands r2, r3
	cmp r2, #0
	beq .L_080ccb1e
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r3, r8
	cmp r3, #0
	bgt .L_080ccb1e
	movs r2, #15
	mov r8, r2
.L_080ccb1e:
	ldr r2, [r1, #12]
	movs r3, #16
	ands r2, r3
	cmp r2, #0
	beq .L_080ccb34
	movs r3, #1
	add r8, r3
	mov r2, r8
	cmp r2, #15
	ble .L_080ccb34
	mov r8, r3
.L_080ccb34:
	ldr r2, [r1, #12]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080ccb5e
	movs r3, #1
	negs r3, r3
	add r9, r3
	mov r1, r9
	cmp r1, #0
	blt .L_080ccb4e
	b .L_080cca26
.L_080ccb4e:
	movs r2, #13
	mov r9, r2
	mov r3, r9
	mov r1, r9
	lsls r3, r3, #12
	lsls r1, r1, #5
	str r3, [sp, #8]
	b .L_080cca46
.L_080ccb5e:
	ldr r2, [r1, #12]
	movs r3, #128
	lsls r3, r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_080ccb7e
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #13
	bgt .L_080ccb76
	b .L_080cca3c
.L_080ccb76:
	movs r1, #0
	mov r9, r1
	str r1, [sp, #8]
	b .L_080cca46
.L_080ccb7e:
	ldr r2, [r1, #12]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_080ccbca
	mov r2, r9
	lsls r3, r2, #4
	add r3, r8
	movs r1, #160
	lsls r3, r3, #1
	lsls r1, r1, #19
	adds r7, r3, r1
	ldrh r6, [r7]
	mov r1, r10
	adds r5, r6, #0
	lsrs r2, r6, #5
	lsrs r3, r6, #10
	ands r5, r4
	ands r2, r4
	ands r3, r4
	cmp r1, #1
	bne .L_080ccbb0
	cmp r5, #30
	bhi .L_080ccbb0
	adds r5, #1
.L_080ccbb0:
	mov r1, r10
	cmp r1, #2
	bne .L_080ccbbc
	cmp r2, #30
	bhi .L_080ccbbc
	adds r2, #1
.L_080ccbbc:
	mov r1, r10
	cmp r1, #3
	bne .L_080ccc14
	cmp r3, #30
	bhi .L_080ccc14
	adds r3, #1
	b .L_080ccc14
.L_080ccbca:
	ldr r2, [r1, #12]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080ccc20
	mov r2, r9
	lsls r3, r2, #4
	add r3, r8
	movs r1, #160
	lsls r3, r3, #1
	lsls r1, r1, #19
	adds r7, r3, r1
	ldrh r6, [r7]
	mov r1, r10
	adds r5, r6, #0
	lsrs r2, r6, #5
	lsrs r3, r6, #10
	ands r5, r4
	ands r2, r4
	ands r3, r4
	cmp r1, #1
	bne .L_080ccbfc
	cmp r5, #0
	beq .L_080ccbfc
	subs r5, #1
.L_080ccbfc:
	mov r1, r10
	cmp r1, #2
	bne .L_080ccc08
	cmp r2, #0
	beq .L_080ccc08
	subs r2, #1
.L_080ccc08:
	mov r1, r10
	cmp r1, #3
	bne .L_080ccc14
	cmp r3, #0
	beq .L_080ccc14
	subs r3, #1
.L_080ccc14:
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r5
	strh r3, [r7]
	b .L_080cca48
.L_080ccc20:
	ldr r2, [r1, #12]
	movs r3, #8
	ands r2, r3
	cmp r2, #0
	beq .L_080ccc88
	mov r2, r9
	lsls r3, r2, #4
	add r3, r8
	movs r2, #160
	lsls r3, r3, #1
	lsls r2, r2, #19
	adds r7, r3, r2
	ldrh r6, [r7]
	b .L_080ccc6c
.L_080ccc3c:
	.4byte gInput
.L_080ccc40:
	cmp r5, #0
	bne .L_080ccc48
	ldr r3, .L_080ccc64
	strh r3, [r7]
.L_080ccc48:
	cmp r5, #10
	bne .L_080ccc4e
	strh r6, [r7]
.L_080ccc4e:
	cmp r5, #20
	bne .L_080ccc56
	ldr r3, .L_080ccc68
	strh r3, [r7]
.L_080ccc56:
	cmp r5, #30
	bne .L_080ccc5c
	strh r6, [r7]
.L_080ccc5c:
	adds r5, #1
	cmp r5, #39
	bls .L_080ccc6e
	b .L_080ccc6c
.L_080ccc64:
	.4byte 0x00007fff
.L_080ccc68:
	.4byte 0x00000000
.L_080ccc6c:
	movs r5, #0
.L_080ccc6e:
	movs r0, #1
	str r1, [sp, #4]
	str r4, [sp, #0]
	bl WaitFrames
	ldr r1, [sp, #4]
	movs r3, #8
	ldr r2, [r1]
	ldr r4, [sp, #0]
	ands r2, r3
	cmp r2, #0
	bne .L_080ccc40
	strh r6, [r7]
.L_080ccc88:
	ldr r2, [r1, #12]
	movs r3, #4
	ands r2, r3
	cmp r2, #0
	bne .L_080ccca2
	movs r0, #1
	str r1, [sp, #4]
	str r4, [sp, #0]
	bl WaitFrames
	ldr r1, [sp, #4]
	ldr r4, [sp, #0]
	b .L_080ccad6
.L_080ccca2:
	bl Func_08014bac
	bl Func_08014b70
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
