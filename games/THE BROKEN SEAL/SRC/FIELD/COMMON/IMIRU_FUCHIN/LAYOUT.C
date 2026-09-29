/* The service step and the cave's dialogue layouts. */
#include "IMIRU_FUCHIN.H"

void SceneState_SetServiceZeroValue06(void)
{
    struct SceneService *work;

    Event_Begin();
    work = Engine_ActorGet(0);
    work->value06 = 0x4000;
    Audio_PlayCue(123);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(1);
}

void FieldScene_RunSingleStep(void)
{
    SceneActor_StepSubjectAlongHeading();
}

void SceneActor_PlaceAtTileAndMark(s32 id, s32 x, s32 y)
{
    struct Rec_39a *rec = Engine_ActorGet(id);

    if (rec != 0) {
        Actor_SetSpritePriority(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}

/* Imports; the queried ones are typed for their return value. */
void DialogueLayout_ConfigureGroupOne(void)
{
    { s32 f1 = 8; s32 g1 = 29; Map_CopyCellAttributes(8, 42, 15, 5,  f1, g1); }

    if (GameFlag_IsSet((s32)0x301) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 22, 31);
        { s32 f2 = 8; s32 g2 = 30; Map_CopyCellAttributes(9, 30, 1, 3,  f2, g2); }
    } else {
        SceneActor_PlaceAtTileAndMark(8, 8, 31);
        { s32 f3 = 22; s32 g3 = 30; Map_CopyCellAttributes(9, 30, 1, 3,  f3, g3); }
    }

    if (GameFlag_IsSet((s32)0x302) != 0) {
        SceneActor_PlaceAtTileAndMark(9, 12, 29);
        { s32 f4 = 11; s32 g4 = 33; Map_CopyCellAttributes(14, 33, 3, 1,  f4, g4); }
    } else {
        SceneActor_PlaceAtTileAndMark(9, 12, 33);
        { s32 f5 = 11; s32 g5 = 29; Map_CopyCellAttributes(14, 29, 3, 1,  f5, g5); }
    }

    if (GameFlag_IsSet((s32)0x303) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 18, 29);
        { s32 f6 = 17; s32 g6 = 33; Map_CopyCellAttributes(14, 33, 3, 1,  f6, g6); }
    } else {
        SceneActor_PlaceAtTileAndMark(10, 18, 33);
        { s32 f7 = 17; s32 g7 = 29; Map_CopyCellAttributes(14, 29, 3, 1,  f7, g7); }
    }
}

void DialogueLayout_ConfigureGroupTwo(void)
{
    { s32 f1 = 12; s32 g1 = 8; Map_CopyCellAttributes(0, 28, 10, 18,  f1, g1); }

    if (GameFlag_IsSet((s32)0x304) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 21, 20);
        { s32 f2 = 13; s32 g2 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f2, g2); }
    } else {
        SceneActor_PlaceAtTileAndMark(8, 13, 20);
        { s32 f3 = 21; s32 g3 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f3, g3); }
    }

    if (GameFlag_IsSet((s32)0x305) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 12, 20);
        { s32 f4 = 12; s32 g4 = 19; Map_CopyCellAttributes(5, 19, 1, 3,  f4, g4); }
        { s32 f5 = 13; s32 g5 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f5, g5); }
        if (GameFlag_IsSet((s32)0x304) != 0) {
            SceneActor_PlaceAtTileAndMark(8, 21, 20);
            { s32 f6 = 13; s32 g6 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f6, g6); }
            { s32 f7 = 12; s32 g7 = 19; Map_CopyCellAttributes(20, 19, 1, 3,  f7, g7); }
        }
    }

    if (GameFlag_IsSet((s32)0x306) != 0) {
        SceneActor_PlaceAtTileAndMark(9, 15, 21);
        { s32 f8 = 14; s32 g8 = 17; Map_CopyCellAttributes(14, 18, 3, 1,  f8, g8); }
    } else {
        SceneActor_PlaceAtTileAndMark(9, 15, 17);
        { s32 f9 = 14; s32 g9 = 21; Map_CopyCellAttributes(14, 18, 3, 1,  f9, g9); }
    }

    if (GameFlag_IsSet((s32)0x307) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 19, 8);
        { s32 f10 = 18; s32 g10 = 25; Map_CopyCellAttributes(14, 18, 3, 1,  f10, g10); }
    } else {
        SceneActor_PlaceAtTileAndMark(10, 19, 25);
        { s32 f11 = 18; s32 g11 = 8; Map_CopyCellAttributes(14, 18, 3, 1,  f11, g11); }
    }
}

void DialogueLayout_ConfigureGroupThree(void)
{
    { s32 k5 = 12, k6 = 21; Map_CopyCellAttributes(12, 3, 9, 16, k5, k6); }

    if (GameFlag_IsSet((s32)0x308) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 14, 25);
        { s32 k5 = 20, k6 = 24; Map_CopyCellAttributes(16, 24, 1, 3, k5, k6); }
    } else if (GameFlag_IsSet((s32)0x309) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 17, 25);
        { s32 k6 = 24;
          Map_CopyCellAttributes(18, 24, 1, 3, 20, k6);
          Map_CopyCellAttributes(18, 24, 1, 3, 14, k6);
          Map_CopyCellAttributes(8, 41, 1, 3, 17, k6);
        }
    } else {
        SceneActor_PlaceAtTileAndMark(8, 20, 25);
        { s32 k5 = 14, k6 = 24; Map_CopyCellAttributes(16, 24, 1, 3, k5, k6); }
    }

    if (GameFlag_IsSet((s32)0x30a) != 0) {
        SceneActor_PlaceAtTileAndMark(9, 13, 35);
        { s32 k5 = 15, k6 = 34; Map_CopyCellAttributes(14, 34, 1, 3, k5, k6); }
    } else {
        SceneActor_PlaceAtTileAndMark(9, 15, 35);
        { s32 k5 = 13, k6 = 34; Map_CopyCellAttributes(14, 34, 1, 3, k5, k6); }
    }

    if (GameFlag_IsSet((s32)0x30b) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 15, 22);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 30);
          Map_CopyCellAttributes(5, 41, 3, 1, k5, 22);
        }
    } else if (GameFlag_IsSet((s32)0x30c) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 15, 23);
        { s32 k5 = 14;
          Map_CopyCellAttributes(5, 42, 3, 1, k5, 23);
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 30);
          Map_CopyCellAttributes(10, 44, 3, 1, k5, 21);
        }
    } else if (GameFlag_IsSet((s32)0x30d) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 15, 26);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 22);
          Map_CopyCellAttributes(5, 43, 3, 1, k5, 26);
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 30);
        }
    } else if (GameFlag_IsSet((s32)0x30e) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 15, 27);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 22);
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 30);
          Map_CopyCellAttributes(5, 44, 3, 1, k5, 27);
        }
    } else {
        SceneActor_PlaceAtTileAndMark(10, 15, 30);
    }

    if (GameFlag_IsSet((s32)0x30f) != 0) {
        SceneActor_PlaceAtTileAndMark(11, 15, 23);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 31);
          Map_CopyCellAttributes(10, 40, 3, 1, k5, 23);
        }
    } else if (GameFlag_IsSet((s32)0x310) != 0) {
        SceneActor_PlaceAtTileAndMark(11, 15, 24);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 31);
          Map_CopyCellAttributes(10, 41, 3, 1, k5, 24);
        }
    } else if (GameFlag_IsSet((s32)0x311) != 0) {
        SceneActor_PlaceAtTileAndMark(11, 15, 27);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 31);
          Map_CopyCellAttributes(10, 42, 3, 1, k5, 27);
        }
    } else if (GameFlag_IsSet((s32)0x312) != 0) {
        SceneActor_PlaceAtTileAndMark(11, 15, 28);
        { s32 k5 = 14;
          Map_CopyCellAttributes(14, 29, 3, 1, k5, 31);
          Map_CopyCellAttributes(10, 43, 3, 1, k5, 28);
        }
    } else {
        SceneActor_PlaceAtTileAndMark(11, 15, 31);
    }
}

/*
 * Four flag-branched layout steps.  Nothing is returned; the three pool words
 * after the return belong to the owner.  The eight bytes of frame are the
 * fifth and sixth arguments of the six-argument layout calls.  Imports are
 * named by the address their call site computes, and are old-style because
 * arity varies between sites.
 */
void FieldScene_RunFlagBranchedLayoutSteps(void)
{

    /*
     * The byte offset 450 is built by shifting, giving entry 225.  The test
     * is (entry - 1) << 16 against 0x10000 with an unsigned compare, which
     * selects exactly entries 1 and 2.
     */
    if ((u32)((u32)((u16)gGameState.entrance - 1) << 16) <= (u32)0x10000) {
        { s32 f1 = 14; s32 g1 = 10; Map_CopyCellAttributes(22, 20, 9, 8,  f1, g1); }
    } else {
        { s32 f2 = 7; s32 g2 = 45; Map_CopyCellAttributes(20, 45, 11, 4,  f2, g2); }
    }

    if (GameFlag_IsSet((s32)0x313) != 0) {
        SceneActor_PlaceAtTileAndMark(8, 20, 17);
        { s32 f3 = 19; s32 g3 = 10; Map_CopyCellAttributes(19, 11, 3, 1,  f3, g3); }
    } else {
        SceneActor_PlaceAtTileAndMark(8, 20, 10);
        { s32 f4 = 19; s32 g4 = 17; Map_CopyCellAttributes(19, 11, 3, 1,  f4, g4); }
    }

    /* 0x314 is built by shifting. */
    if (GameFlag_IsSet((s32)0x314) != 0) {
        SceneActor_PlaceAtTileAndMark(9, 14, 16);
        { s32 f5 = 22; s32 g5 = 15; Map_CopyCellAttributes(16, 15, 1, 3,  f5, g5); }
    } else {
        SceneActor_PlaceAtTileAndMark(9, 22, 16);
        { s32 f6 = 14; s32 g6 = 15; Map_CopyCellAttributes(16, 15, 1, 3,  f6, g6); }
    }

    if (GameFlag_IsSet((s32)0x315) != 0) {
        SceneActor_PlaceAtTileAndMark(10, 17, 46);
        { s32 f7 = 7; s32 g7 = 45; Map_CopyCellAttributes(15, 15, 1, 3,  f7, g7); }
    } else {
        SceneActor_PlaceAtTileAndMark(10, 7, 46);
        { s32 f8 = 17; s32 g8 = 45; Map_CopyCellAttributes(15, 15, 1, 3,  f8, g8); }
    }
}
