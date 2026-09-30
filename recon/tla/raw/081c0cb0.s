.syntax unified
	.thumb
	.global Func_081c0cb0
	.thumb_func
Func_081c0cb0:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	bl Func_081c11ac
	ldr r3, .L_081c0dc8
	movs r5, #240
	ldrb r3, [r3, #3]
	lsls r5, r5, #8
	adds r7, r3, #0
	ands r5, r6
	cmp r7, #0
	beq .L_081c0cca
	b .L_081c0e2c
.L_081c0cca:
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	ands r6, r3
	cmp r6, #78
	bne .L_081c0cf4
	ldr r5, .L_081c0dcc
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_081c0ce0
	b .L_081c0e2c
.L_081c0ce0:
	ldr r0, .L_081c0dd0
	movs r1, #7
	bl MusicPlayer_FadeOut
	ldrb r3, [r5]
	adds r3, #1
	strb r3, [r5]
	ldr r3, .L_081c0dd4
	strh r7, [r3]
	b .L_081c0e2c
.L_081c0cf4:
	movs r3, #195
	lsls r3, r3, #1
	cmp r6, r3
	bne .L_081c0d0e
	ldr r5, .L_081c0dd8
	ldr r0, .L_081c0ddc
	movs r1, #3
	strh r7, [r5, #6]
	bl MusicPlayer_FadeOut
	strh r7, [r5, #10]
	ldr r0, .L_081c0de0
	b .L_081c0d1c
.L_081c0d0e:
	movs r3, #140
	adds r3, #255
	cmp r6, r3
	bne .L_081c0d24
	ldr r3, .L_081c0dd8
	ldr r0, .L_081c0de0
	strh r7, [r3, #10]
.L_081c0d1c:
	movs r1, #3
	bl MusicPlayer_FadeOut
	b .L_081c0e2c
.L_081c0d24:
	movs r3, #175
	lsls r3, r3, #2
	cmp r6, r3
	bge .L_081c0d96
	cmp r6, #99
	ble .L_081c0d6e
	ldr r7, .L_081c0de4
	lsls r4, r6, #3
	adds r3, r4, #4
	ldrh r2, [r7, r3]
	cmp r2, #7
	bne .L_081c0d58
	ldr r1, .L_081c0de8
.L_081c0d3e:
	lsls r5, r2, #1
	adds r3, r5, r2
	lsls r3, r3, #2
	ldr r3, [r1, r3]
	ldrb r3, [r3, #4]
	cmp r3, #0
	beq .L_081c0d5c
	subs r2, #1
	cmp r2, #3
	bgt .L_081c0d3e
	movs r2, #7
	movs r5, #14
	b .L_081c0d5c
.L_081c0d58:
	ldr r1, .L_081c0de8
	lsls r5, r2, #1
.L_081c0d5c:
	adds r3, r5, r2
	lsls r3, r3, #2
	ldr r0, [r1, r3]
	ldr r1, [r7, r4]
	bl MusicPlayer_StartSong
	ldr r3, .L_081c0dd8
	strh r6, [r3, r5]
	b .L_081c0e2c
.L_081c0d6e:
	cmp r6, #79
	ble .L_081c0d92
	movs r2, #0
	ldr r0, .L_081c0dd0
	movs r1, #255
	bl MusicPlayer_SetVolume
	ldr r3, .L_081c0dec
	lsls r0, r6, #16
	strh r7, [r3]
	ldr r3, .L_081c0df0
	lsrs r0, r0, #16
	strh r7, [r3]
	bl Audio_PlaySound
	ldr r2, .L_081c0df4
	movs r3, #10
	b .L_081c0e0e
.L_081c0d92:
	cmp r6, #79
	beq .L_081c0e2c
.L_081c0d96:
	ldr r2, .L_081c0dd4
	ldrh r3, [r2]
	cmp r6, r3
	beq .L_081c0e2c
	strh r6, [r2]
	adds r0, r6, #0
	bl Func_081c0cac
	bl Func_08013b30
	lsls r0, r6, #16
	lsrs r0, r0, #16
	bl Audio_PlaySound
	movs r3, #128
	lsls r3, r3, #5
	ands r3, r5
	cmp r3, #0
	beq .L_081c0df8
	ldr r2, .L_081c0df0
	ldr r3, .L_081c0dc4
	b .L_081c0dfc
	.2byte 0x0000
.L_081c0dc4:
	.4byte 0x00000000
.L_081c0dc8:
	.4byte Data_03001138
.L_081c0dcc:
	.4byte Data_02005814
.L_081c0dd0:
	.4byte gMusicPlayerBgm
.L_081c0dd4:
	.4byte Data_02005830
.L_081c0dd8:
	.4byte Data_02005820
.L_081c0ddc:
	.4byte Data_02006b60
.L_081c0de0:
	.4byte Data_02006990
.L_081c0de4:
	.4byte Sound_SongTable
.L_081c0de8:
	.4byte Sound_PlayerSlots
.L_081c0dec:
	.4byte gMusicVolumeTarget
.L_081c0df0:
	.4byte gMusicVolume
.L_081c0df4:
	.4byte gMusicRestoreDelay
.L_081c0df8:
	ldr r2, .L_081c0e1c
	ldr r3, .L_081c0e14
.L_081c0dfc:
	strh r3, [r2]
	ldr r2, .L_081c0e20
	ldr r3, .L_081c0e14
	strh r3, [r2]
	ldr r2, .L_081c0e24
	ldr r3, .L_081c0e18
	strh r3, [r2]
	ldr r2, .L_081c0e28
	movs r3, #0
.L_081c0e0e:
	strb r3, [r2]
	b .L_081c0e2c
	.2byte 0x0000
.L_081c0e14:
	.4byte 0x00000100
.L_081c0e18:
	.4byte 0x00000004
.L_081c0e1c:
	.4byte gMusicVolume
.L_081c0e20:
	.4byte gMusicVolumeTarget
.L_081c0e24:
	.4byte Data_02005810
.L_081c0e28:
	.4byte Data_02005814
.L_081c0e2c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
