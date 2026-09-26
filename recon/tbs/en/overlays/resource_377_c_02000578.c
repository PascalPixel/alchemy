/* NONMATCHING: 1172 bytes, candidate 1176, 551 differing halfwords, 142
 * wrong instructions, 216 halfword edits. FieldScene_RunComplexActorSequence
 * targets FIELD/COMMON/HAIDIA_BABI/F_00578.C as a single-overlay unit binding its
 * names at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Pointer-taking child-flag and palette calls now consume the actor
 * lookup result; the palette call receives 226. The sprite pointer is now
 * captured before calls, matching the reference's read lifetime. A typed
 * pointer bank shares map/event/auxiliary reads and removes the extra middle
 * pool; its retained anchor is +0 rather than +76. Existing typed inline
 * speed/render services reproduce all their argument constant reloads.
 * Other call constants and sprite/actor registers differ; topology is equal.
 * WALL: structural-topology: shared workspace lifetimes and actor setup */
#include "FIELD_EVENT.H"

struct SceneMapState {
    u8 unknown_0000[0x1f84];
    u16 active;
};

struct ScenePointerBank {
    u8 *map_work;
    u8 unknown_04[0x48];
    struct EventWork *event_work;
    u8 unknown_50[8];
    struct SceneMapState *map_state;
};

extern struct ScenePointerBank gScenePointers;

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunComplexActorSequence. */

void Local_020017e4();
void Main_080000c0();
void Main_08009128();
void Main_08009188();
void Main_08009190();
void Main_080091e0(struct FieldActor *actor, s32 flags);
void Main_08009208();
void Main_08009210();
void Main_08009228(struct FieldActor *actor, s32 palette);
void Main_08015210();
void Main_080770c8();
void Main_0808a010();
void Main_0808a018();
s32 Main_0808a070();
struct FieldActor *Main_0808a080(s32 id);
void Main_0808a098();
void Main_0808a0a0();
void Main_0808a0c0();
void Main_0808a0d0();
void Main_0808a0f0();
void Main_0808a100();
void Main_0808a110();
void Main_0808a128();
void Main_0808a130();
void Main_0808a138();
void Main_0808a158();
void Main_0808a170();
void Main_0808a178();
void Main_0808a180();
void Main_0808a188();
void Main_0808a1b8();
void Main_0808a1d8();
void Main_0808a1e0();
void Main_0808a200();
void Main_0808a248();
void Main_0808a2c8();
void Main_0808a2d8();
void Main_0808a2e0();
void Main_0808a360();
void Main_0808a368();
void Main_0808a370();
void Main_080f9010();

void FieldScene_RunComplexActorSequence(void)
{
    s32 base;
    struct FieldSprite *sprite;
    struct FieldActor *p12;
    struct FieldActor *p67;
    struct FieldActor *p89;
    u8 *work;
    struct FieldActor *scene_actor;

    work = gScenePointers.map_work;
    scene_actor = gScenePointers.event_work->view_center;
    sprite = Main_0808a080(17)->sprite;
    Main_0808a018();
    Main_0808a0f0(11, 0, 0);
    Main_0808a0f0(12, 0, 0);
    Main_0808a0f0(13, 0, 0);
    Main_0808a0f0(14, 0, 0);
    Main_0808a0f0(15, 0, 0);
    Main_0808a0f0(16, 0, 0);
    Main_080091e0(Main_0808a080(0), 0);
    Main_0808a100(0, 18);
    *(u16 *)((u8 *)sprite + 30) = 1365;
    p12 = Main_0808a080(17);
    p12->motion_flags = 0;
    Main_080091e0(Main_0808a080(17), 0);
    Main_0808a0f0(17, 37748736, 42598400);
    Main_08009188(7);
    Main_0808a0f0(8, 34996224, 45088768);
    Main_08009208();
    Main_0808a1d8(8);
    base = 3666;
    Main_08015210(base, 1, 0);
    Main_0808a010(40);
    MapRender_SetValues(65536, 65536, 65536);
    Main_0808a1d8(8);
    Main_08015210(base + 1, 1, 0);
    Main_08009210();
    Main_0808a010(40);
    *(u32 *)(work + 236) = 0x01480000;
    *(u32 *)(work + 240) = 0x02580000;
    *(u32 *)(work + 244) = 0x02700000;
    *(u32 *)(work + 248) = 0x03300000;
    scene_actor->x.fixed = 0x02340000;
    scene_actor->y.fixed = 0;
    scene_actor->z.fixed = 0x02b30000;
    Main_08009128();
    Main_080000c0(1);
    gScenePointers.event_work->start_transition = 521;
    gScenePointers.event_work->transition_frames = 64;
    Main_0808a2c8();
    gScenePointers.map_state->active = 1;
    Main_0808a2d8();
    Main_080000c0(30);
    Main_0808a360();
    Main_0808a370();
    Main_0808a2e0();
    Main_0808a110(8, 4);
    Main_0808a170(base + 2);
    Main_0808a188(36872, 0, 60);
    Main_0808a138(0, 2);
    Main_0808a010(40);
    Main_0808a138(8, 1);
    Main_0808a010(40);
    Main_0808a188(36872, 0, 20);
    Main_0808a138(0, 2);
    Main_08009190(7);
    Main_0808a010(20);
    Main_08009188(8);
    Actor_SetSpeed(0, 65536, 32768);
    Main_0808a100(0, 19);
    Main_0808a0c0(0, 557, 679);
    Main_08009190(8);
    Main_08009188(9);
    Main_0808a0c0(0, 555, 680);
    Main_0808a010(30);
    Main_0808a1b8(8, 53248, 0);
    Main_080091e0(Main_0808a080(0), 1);
    Main_0808a128(0, 4, 0);
    Main_0808a0d0(0, 543, 674);
    Main_0808a1e0(0, 3);
    Main_0808a1b8(0, 16384, 40);
    Main_0808a110(8, 4);
    Main_0808a010(20);
    Main_0808a180(36872, 0);
    Local_020017e4();
    Main_0808a130(8, 2);
    Main_0808a188(36872, 0, 20);
    p67 = Main_0808a080(8);
    *(u8 *)((u8 *)(p67) + 90) &= 0xfe;
    Main_0808a0d0(8, 542, 680);
    Main_0808a010(1);
    p67 = Main_0808a080(8);
    *(u8 *)((u8 *)p67 + 90) |= 0x1;
    Main_0808a010(10);
    Main_0808a138(8, 2);
    Main_08009228(Main_0808a080(0), 226);
    Main_080770c8(33);
    Main_080f9010(126);
    Main_0808a158(0, 7);
    Main_0808a010(10);
    Main_0808a158(0, 0);
    Main_0808a010(20);
    p67 = Main_0808a080(8);
    *(u8 *)((u8 *)p67 + 90) &= 0xfe;
    Main_0808a0d0(8, 534, 688);
    Main_0808a010(1);
    p67 = Main_0808a080(8);
    *(u8 *)((u8 *)p67 + 90) |= 0x1;
    Main_0808a010(20);
    Actor_SetSpeed(8, 98304, 49152);
    Actor_SetSpeed(0, 98304, 49152);
    Main_0808a200(8, 1);
    p89 = Main_0808a080(0);
    p89->priority_flags |= 0x1;
    Main_0808a098(8, 33594036);
    Main_0808a010(20);
    Main_0808a098(0, 33594036);
    Main_0808a0a0(8);
    Main_0808a0d0(8, 419, 661);
    Main_0808a0d0(8, 408, 661);
    Main_0808a100(8, 1);
    Main_0808a100(0, 1);
    Main_0808a1b8(8, 16384, 10);
    Main_0808a178(32776, 0);
    if (Main_0808a070(0, 0) == 0) {
        gScenePointers.event_work->message++;
    }
    Main_0808a010(20);
    Main_0808a188(32776, 0, 20);
    Main_0808a100(0, 3);
    Main_0808a110(8, 3);
    Main_0808a010(20);
    Main_0808a098(8, 33594116);
    Main_0808a098(0, 33594164);
    Main_0808a010(20);
    gScenePointers.event_work->start_transition = 513;
    gScenePointers.event_work->transition_frames = 16;
    Main_0808a368();
    Main_0808a370();
    Main_0808a248(20);
}
