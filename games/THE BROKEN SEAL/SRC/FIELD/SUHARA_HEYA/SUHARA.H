#ifndef SUHARA_H
#define SUHARA_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SCENE.H"

enum RecordSetupMessage {
    MSG_GET_SICK_THINKING_ABOUT_LALIVERO = 0x25d5
};


#define STEP_COUNTER (*(u16 *)(*(u8 **)&gEventWork + 0x1d8))

struct SceneRecord {
    u8 unk_00[9];
    u8 flags;
    u8 unk_0a[20];
    u16 angle;
    u8 unk_20[6];
    u8 field_26;
};

/*
 * Byte and field views of one actor share this union so that stores through
 * the linked record do not make the pointer read disappear through alias
 * analysis.
 */
union SceneActor {
    struct {
        u8 unk_00[12];
        s32 y;
        u8 unk_10[64];
        struct SceneRecord *record;
        u8 unk_54;
        u8 mode;
        u8 unk_56[3];
        u8 flags;
    } fields;
    u8 bytes[90];
};

/* The room's scene tables, in the overlay's data; flag 0x96f selects the
 * later placements and events. */
extern u8 gSuharaHeyaEntrances[];
extern u8 gSuharaHeyaRegions[];
extern u8 gSuharaHeyaExits[];
extern u8 gSuharaHeyaPlacements[];
extern u8 gSuharaHeyaPlacements96f[];
extern u8 gSuharaHeyaEvents[];
extern u8 gSuharaHeyaEvents96f[];

/* Main-image services reached through the overlay's import veneers. */
void Shop_ConfirmAct();
void Inn_CheckIn();

#endif
