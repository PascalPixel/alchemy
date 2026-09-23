#include "TYPES.H"

struct FieldActor {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

/* One 12-byte map event record; the table ends at flags == -1. */
struct MapEventEntry {
    s32 flags;
    s16 id;
    s16 flag;
    s32 position;
};

struct FieldPartyState {
    u8 unknown_000[500];
    s32 leader;
};

struct FieldMapState {
    u8 unknown_00[36];
    s32 events;
};

extern struct FieldPartyState Data_02000240;
extern struct FieldMapState Data_02008000;

struct FieldActor *Func_0808ba1c(s32 index);
s32 Func_0808ed4c(s32 id);
struct MapEventEntry *Func_080072e4(s32 resource);
s32 Func_080770c0(s32 flag);
s32 Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
void Func_08009098(s32 object, s32 data);
void Func_080091e0(s32 object, s32 value);

/* Walks the area's (x, z, id) marker list, ending at 0xff 0xff, and for each
   marker within 8 by 5 tiles of the leader spawns a kind-22 effect object
   on every matching event whose flag is still clear. */
void FieldEffect_SpawnNearbyMarkers(void)
{
    u8 *list;
    struct FieldActor *actor;
    struct MapEventEntry *entry;
    s32 actor_x;
    s32 actor_z;
    s32 x;
    s32 z;
    s32 id;

    list = *(u8 **)(*(u8 **)0x03001e70 + 16);
    actor = Func_0808ba1c(Data_02000240.leader);
    actor_x = actor->x >> 20;
    actor_z = actor->z >> 20;
    if (list != 0) {
        while (x = *list++, z = *list++, x != 0xff || z != 0xff) {
            id = *list++;
            if (Func_0808ed4c(id) != 0)
                continue;
            if ((u32)(id - 100) > 139)
                continue;
            if (actor_x - x >= 0) {
                if (actor_x - x > 8)
                    continue;
            } else if (x - actor_x > 8)
                continue;
            if (actor_z - z >= 0) {
                if (actor_z - z > 5)
                    continue;
            } else if (z - actor_z > 5)
                continue;
            entry = Func_080072e4(Data_02008000.events);
            if (entry->flags == -1)
                continue;
            do {
                if (entry->id == id && (entry->flags & 15) == 3) {
                    switch (entry->position & 0xfff00000) {
                    case 0:
                    case 0x100000:
                    case 0x200000:
                    case 0x300000:
                    case 0x500000:
                        if (entry->flag != -1 && Func_080770c0(entry->flag) == 0) {
                            /* FAKEMATCH: the spawned object reuses the actor
                               variable, which keeps both in r5. */
                            actor = (struct FieldActor *)Func_080090c8(22, (x << 20) + 0x80000, 0, (z << 20) + 0x80000);
                            if (actor != 0) {
                                Func_08009098((s32)actor, 0x0809e8a0);
                                Func_080091e0((s32)actor, 0);
                                *(s32 *)((u8 *)actor + 108) = 0x0808f28d;
                            }
                        }
                        break;
                    }
                }
                entry++;
            } while (entry->flags != -1);
        }
    }
}
