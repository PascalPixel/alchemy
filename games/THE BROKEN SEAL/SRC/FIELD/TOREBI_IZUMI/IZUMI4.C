#include "TOPIC.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"

void OverlayObject_SetField54(s32 actor, s32 value);

/* The ride's work, laid out in order just past the overlay's image, with the
 * actor records FOUR_ACTORS.C sets up. */
struct SpringRide TorebiIzumi_Ride = { 0 };

s32 TorebiIzumi_RideSide = 0;

s32 TorebiIzumi_RideUnknown[3] = { 0 };

struct SpringActor TorebiIzumi_ActorRecords[4] = { { 0 } };

s32 TorebiIzumi_RideFrame = 0;
s32 TorebiIzumi_RideEnded = 0;
s32 TorebiIzumi_RideResult = 0;

/*
 * The ride's frame task: ages the trail, moves the body and keeps it inside
 * the fountain's trapezoid, moves the four actors and lets them strike the
 * body, then places everything.
 */
void FieldScene_RunSecondaryScript(void)
{
    struct SpringRide *ride;
    struct SpringActor *rec;
    s32 i;
    s32 y;
    s32 tmp;
    s32 step;
    s32 mode;
    s32 phase;
    s32 xlo;
    s32 xhi;
    s32 zlo;
    s32 zhi;
    /* FAKEMATCH: never used; the reference's frame keeps twelve bytes that no instruction touches. */
    struct SpringPoint spare;

    ride = &TorebiIzumi_Ride;
    i = 3;
    do {
        ride->trail[i].x = ride->trail[i - 1].x;
        ride->trail[i].y = ride->trail[i - 1].y;
        ride->trail[i].z = ride->trail[i - 1].z;
        i--;
    } while (i != 0);

    if (ride->hold > 31) {
        ride->trail[0].x += ride->velocity.x;
        y = ride->trail[0].y + ride->velocity.y;
        ride->trail[0].y = y;
        ride->trail[0].z += ride->velocity.z;

        if (y <= 0) {
            ride->trail[0].y = 0;
            if (ride->velocity.y != 0) {
                /* Landing frame. */
                ride->velocity.y = 0;
                if (TorebiIzumi_RideSide == 1) {
                    Object_SetMode(Object_GetById(17), 1);
                } else {
                    Object_SetMode(Object_GetById(12), 1);
                }
            }

            if (ride->frames > 0) {
                s32 dx;
                s32 dz;
                s32 len;

                /* Chase the fixed target at (0x780000, 0x470000). */
                dx = (0x780000 - ride->trail[0].x) >> 8;
                dz = (0x470000 - ride->trail[0].z) >> 8;
                len = Iwram_Sqrt(dx * dx + dz * dz);
                ride->velocity.x += 6553 * dx / len;
                ride->velocity.z += 6553 * dz / len;
                ride->velocity.x = ride->velocity.x * 253 / 256;
                ride->velocity.z = ride->velocity.z * 253 / 256;
                ride->frames = ride->frames - 1;
            } else {
                /* Budget spent: coast to a stop. */
                ride->velocity.x = ride->velocity.x * 220 / 256;
                ride->velocity.z = ride->velocity.z * 220 / 256;
                if (ride->velocity.x > -1024 && ride->velocity.x < 1024) {
                    ride->velocity.x = 0;
                }
                if (ride->velocity.z > -1024 && ride->velocity.z < 1024) {
                    ride->velocity.z = 0;
                }
                if (ride->velocity.x == 0 && ride->velocity.z == 0) {
                    if (TorebiIzumi_RideSide == 1) {
                        Object_SetMode(Object_GetById(17), 2);
                        OverlayObject_SetField54(15, 0);
                        OverlayObject_SetField54(14, 0);
                        OverlayObject_SetField54(13, 0);
                    } else {
                        Object_SetMode(Object_GetById(12), 2);
                        OverlayObject_SetField54(10, 0);
                        OverlayObject_SetField54(9, 0);
                        OverlayObject_SetField54(8, 0);
                    }
                    {
                        s32 dx = (0x780000 - ride->trail[0].x) >> 16;
                        s32 dz = (0x470000 - ride->trail[0].z) >> 16;
                        s32 dist = dx * dx + dz * dz;

                        TorebiIzumi_RideEnded = 1;
                        if (dist <= 224) {
                            TorebiIzumi_RideResult = 0;
                        } else if (dist <= 624) {
                            TorebiIzumi_RideResult = 1;
                        } else if (dist <= 1088) {
                            TorebiIzumi_RideResult = 2;
                        } else if (dist <= 1680) {
                            TorebiIzumi_RideResult = 3;
                        } else {
                            TorebiIzumi_RideResult = 4;
                        }
                    }
                }
            }

            /* Trapezoid bounds: the near and far edges pull the x limits in,
             * the left and right edges pull the z limits in. */
            xlo = 0x300000;
            xhi = 0xc00000;
            zlo = 0x180000;
            zhi = 0x780000;
            if (ride->trail[0].z < 0x2a0000) {
                tmp = (0x2a0000 - ride->trail[0].z) * 42 / 18;
                xlo = 0x300000 + tmp;
                if (xlo > 0x5a0000) {
                    xlo = 0x5a0000;
                }
                xhi = 0xc00000 - tmp;
                if (xhi < 0x960000) {
                    xhi = 0x960000;
                }
            }
            if (ride->trail[0].z > 0x660000) {
                tmp = (ride->trail[0].z * 42 - 0x10bc0000) / 18;
                xlo = 0x300000 + tmp;
                if (xlo > 0x5a0000) {
                    xlo = 0x5a0000;
                }
                xhi = 0xc00000 - tmp;
                if (xhi < 0x960000) {
                    xhi = 0x960000;
                }
            }
            if (ride->trail[0].x < 0x5a0000) {
                tmp = (0x5a0000 - ride->trail[0].x) * 18 / 42;
                zlo = 0x180000 + tmp;
                if (zlo > 0x2a0000) {
                    zlo = 0x2a0000;
                }
                zhi = 0x780000 - tmp;
                if (zhi < 0x660000) {
                    zhi = 0x660000;
                }
            }
            if (ride->trail[0].x > 0x960000) {
                tmp = (ride->trail[0].x * 18 - 0xa8c0000) / 42;
                zlo = 0x180000 + tmp;
                if (zlo > 0x2a0000) {
                    zlo = 0x2a0000;
                }
                zhi = 0x780000 - tmp;
                if (zhi < 0x660000) {
                    zhi = 0x660000;
                }
            }

            /* Bounce off each edge with half the incoming speed. */
            if (ride->trail[0].x < xlo) {
                ride->trail[0].x = xlo;
                if (ride->velocity.x < 0) {
                    ride->velocity.x = -ride->velocity.x / 2;
                }
            }
            if (ride->trail[0].x > xhi) {
                ride->trail[0].x = xhi;
                if (ride->velocity.x > 0) {
                    ride->velocity.x = -ride->velocity.x / 2;
                }
            }
            if (ride->trail[0].z < zlo) {
                ride->trail[0].z = zlo;
                if (ride->velocity.z < 0) {
                    ride->velocity.z = -ride->velocity.z / 2;
                }
            }
            if (ride->trail[0].z > zhi) {
                ride->trail[0].z = zhi;
                if (ride->velocity.z > 0) {
                    ride->velocity.z = -ride->velocity.z / 2;
                }
            }
        } else {
            /* Still airborne: keep falling. */
            ride->velocity.y -= 0x4000;
        }
    }

    i = 0;
    do {
        rec = &TorebiIzumi_ActorRecords[i];

        if (rec->react > 0) {
            rec->react = rec->react - 1;
        }
        if (rec->cool > 0) {
            rec->cool = rec->cool - 1;
        }

        if (i <= 1) {
            /* Records 0 and 1 slide back and forth along x. */
            mode = rec->mode;
            step = 0x10000;
            if (mode == 1) {
                step = step * 2;
            }
            if (mode == 2) {
                step = step * 3;
            }
            if (rec->react > 0) {
                if (i == 0) {
                    Object_SetMode(Object_GetById(18), 3);
                } else {
                    Object_SetMode(Object_GetById(19), 3);
                }
            } else {
                if (i == 0) {
                    Object_SetMode(Object_GetById(18), 1);
                } else {
                    Object_SetMode(Object_GetById(19), 1);
                }
                if (rec->hold == 0) {
                    if (rec->heading == 0) {
                        rec->x = rec->x + step;
                    } else {
                        rec->x = rec->x - step;
                    }
                    if (rec->x <= 0x400000) {
                        rec->heading = 0;
                        if (i == 1) {
                            rec->hold = 30;
                        }
                    }
                    if (rec->x > 0xafffff) {
                        rec->heading = 1;
                        if (i == 1) {
                            rec->hold = 30;
                        }
                    }
                } else {
                    rec->hold = rec->hold - 1;
                }
            }
        } else if (i == 2) {
            /* Record 2 turns clockwise on a 48 x 40 ellipse. */
            mode = rec->mode;
            step = -64;
            if (mode == 1) {
                step = step * 2;
            }
            if (mode == 2) {
                step = step * 3;
            }
            if (rec->react > 0) {
                Object_SetMode(Object_GetById(20), 3);
            } else {
                Object_SetMode(Object_GetById(20), 2);
                rec->x = Engine_MathSin(rec->heading) * 48 + 0x700000;
                rec->z = Engine_MathCos(rec->heading) * 40 + 0x480000;
                phase = (u16)rec->heading + step;
                rec->heading = phase;
                rec->hold = rec->hold + 1;
            }
        } else {
            /* Record 3 turns the other way and rests for the last 128 counts
             * of every 512-count lap. */
            phase = rec->hold & 511;
            mode = rec->mode;
            step = 64;
            if (mode == 1) {
                step = step * 2;
            }
            if (mode == 2) {
                step = step * 3;
            }
            if (rec->react > 0) {
                Object_SetMode(Object_GetById(21), 3);
            } else if (phase <= 383) {
                rec->x = Engine_MathSin(rec->heading) * 52 + 0x700000;
                rec->z = Engine_MathCos(rec->heading) * 24 + 0x480000;
                rec->heading = rec->heading + step;
                Object_SetMode(Object_GetById(21), 2);
            } else {
                Object_SetMode(Object_GetById(21), 3);
            }
            rec->hold = rec->hold + 1;
        }

        /* Contact test against the chased body. */
        if (rec->cool == 0 && ride->trail[0].y == 0) {
            s32 dx;
            s32 dz;
            s32 dist;

            dx = (rec->x - ride->trail[0].x) >> 16;
            dz = (rec->z - ride->trail[0].z) >> 16;
            dist = dx * dx + dz * dz;
            if (dist <= 119 && ride->frames > 30) {
                s32 speed;
                s32 next_mode;

                speed = 0x30000;
                if (i <= 1) {
                    if (rec->heading == 0) {
                        if (ride->velocity.x < speed) {
                            ride->velocity.x = speed;
                            ride->frames = ride->frames - 100;
                        }
                    } else {
                        if (ride->velocity.x > -speed) {
                            ride->velocity.x = -speed;
                            ride->frames = ride->frames - 100;
                        }
                    }
                } else {
                    s32 len;

                    len = Iwram_Sqrt(dist);
                    ride->velocity.x = -dx * speed / len;
                    ride->velocity.z = -dz * speed / len;
                    ride->frames = ride->frames - 100;
                }
                Engine_AudioPlayCue(301);
                next_mode = (rec->mode + 1) % 3;
                rec->react = 36;
                rec->mode = next_mode;
                rec->cool = 30;
            }
        }

        switch (i) {
        case 0:
            TorebiIzumi_PlaceActor(18, &rec->x, 0, TorebiIzumi_SliderFrames[rec->mode],
                                    (rec->mode << 4) + 16);
            break;
        case 1:
            TorebiIzumi_PlaceActor(19, &rec->x, 0, TorebiIzumi_SliderFrames[rec->mode],
                                    (rec->mode << 4) + 16);
            break;
        case 2:
            TorebiIzumi_PlaceActor(20, &rec->x, 0x8000 - rec->heading,
                                    TorebiIzumi_CircleFrames[rec->mode],
                                    (rec->mode << 4) + 16);
            break;
        case 3:
            TorebiIzumi_PlaceActor(21, &rec->x, 0xffff - rec->heading,
                                    TorebiIzumi_CircleFrames[rec->mode],
                                    (rec->mode << 4) + 16);
            break;
        }

        i++;
    } while (i != 4);

    /* Flattened copy of the current position, then publish the body and its
     * three trailing samples. */
    ride->shadow.x = ride->trail[0].x;
    ride->shadow.y = 0;
    ride->shadow.z = ride->trail[0].z;

    if (TorebiIzumi_RideSide == 1) {
        TorebiIzumi_PlaceActor(17, &ride->trail[0].x, 0, 0, 16);
        TorebiIzumi_PlaceActor(16, &ride->shadow.x, 0, 0, 16);
        TorebiIzumi_PlaceActor(15, &ride->trail[1].x, 0, 0, 16);
        TorebiIzumi_PlaceActor(14, &ride->trail[2].x, 0, 0, 16);
        TorebiIzumi_PlaceActor(13, &ride->trail[3].x, 0, 0, 16);
        Object_SetMode(Object_GetById(15), 4);
        Object_SetMode(Object_GetById(14), 4);
        Object_SetMode(Object_GetById(13), 4);
    } else {
        TorebiIzumi_PlaceActor(12, &ride->trail[0].x, 0, 0, 16);
        TorebiIzumi_PlaceActor(11, &ride->shadow.x, 0, 0, 16);
        TorebiIzumi_PlaceActor(10, &ride->trail[1].x, 0, 0, 16);
        TorebiIzumi_PlaceActor(9, &ride->trail[2].x, 0, 0, 16);
        TorebiIzumi_PlaceActor(8, &ride->trail[3].x, 0, 0, 16);
        Object_SetMode(Object_GetById(10), 4);
        Object_SetMode(Object_GetById(9), 4);
        Object_SetMode(Object_GetById(8), 4);
    }

    if (ride->hold != -1) {
        ride->hold = ride->hold + 1;
    }
}

/*
 * The 148-byte owner includes its eight-word literal pool: those words lie
 * past the return and are read only by the pc-relative loads.
 * Field names are descriptive only: the 24-byte record stride and the cleared
 * halfwords at +14..+20 are read off the stores alone, and the second heading
 * is 0x0001 rather than a multiple of 0x4000 -- the byte is certain, its
 * meaning is not.
 */
void SceneState_InitFourActorRecordsAndInstallTask(void)
{
    struct SpringRide *ride = &TorebiIzumi_Ride;
    s32 i = 0;
    u8 *xtbl;
    u16 *htbl;
    u8 *rec;
    u8 *ztbl;

    xtbl = (u8 *)TorebiIzumi_ActorTileX;
    rec = (u8 *)TorebiIzumi_ActorRecords;
    htbl = (u16 *)TorebiIzumi_ActorHeadings;
    ztbl = (u8 *)TorebiIzumi_ActorTileZ;

    do {
        *(s32 *)(rec + 0) = (s32)*xtbl << 16;
        *(s32 *)(rec + 8) = (s32)*ztbl << 16;
        *(s32 *)(rec + 4) = 0;
        *(u16 *)(rec + 12) = *htbl;
        *(u16 *)(rec + 14) = 0;
        *(u16 *)(rec + 16) = 0;
        *(u16 *)(rec + 18) = 0;
        *(u16 *)(rec + 20) = 0;

        i++;
        xtbl++;
        ztbl++;
        htbl++;
        rec += 24;
    } while (i != 4);

    ride->trail[0].x = (s32)0xffe20000;      /* -30.0 in 16.16 */
    ride->trail[0].y = 0;
    ride->trail[0].z = 0x640000;             /* 200 << 15, i.e. 100.0 */
    ride->velocity.x = 0;
    ride->velocity.y = 0;
    ride->velocity.z = 0;
    ride->frames = 0;

    /* r0 carries each lookup's result straight into the retag call. */
    Object_SetMode(Object_GetById(20), 2);
    Object_SetMode(Object_GetById(21), 2);

    /* The locals keep the task and its rate built rather than folded. */
    {
        s32 budget = 0xc83;
        void (*task)(void) = FieldScene_RunSecondaryScript;

        Engine_TaskAddCallback(task, budget);
    }
}

/* Launches the spring ride from side 0 or 1: plays the spring cue at frame
 * 50, starts the leader and the ride at frame 16, and waits for the ride to
 * report that it has ended; returns its result. */
s32 TorebiIzumi_RunSpringRide(s32 side)
{
    struct SpringRide *ride;

    ride = &TorebiIzumi_Ride;
    ride->trail[0].y = 0;
    ride->trail[1].y = 0;
    ride->trail[2].y = 0;
    ride->trail[3].y = 0;
    TorebiIzumi_RideSide = side;
    TorebiIzumi_RideEnded = 0;
    ride->hold = 0xffff;
    for (TorebiIzumi_RideFrame = 0;; TorebiIzumi_RideFrame++) {
        if (TorebiIzumi_RideFrame == 50) {
            Engine_AudioPlayCue(300);
        }
        if (TorebiIzumi_RideFrame == 16) {
            Engine_ActorSetAnimation(gGameState.selected_actor, 29);
            ride->hold = 0;
            ride->velocity.x = 0x14ccc;
            ride->velocity.y = 0x40000;
            ride->velocity.z = -0x20000;
            ride->trail[0].x = 0x780000;
            ride->trail[0].y = 0x100000;
            ride->trail[0].z = 0x980000;
            ride->frames = 300;
            if (TorebiIzumi_RideSide == 1) {
                Object_SetMode(Object_GetById(16), 3);
                Object_SetMode(Object_GetById(17), 0);
                OverlayObject_SetField54(15, 1);
                OverlayObject_SetField54(14, 1);
                OverlayObject_SetField54(13, 1);
            } else {
                Object_SetMode(Object_GetById(11), 3);
                Object_SetMode(Object_GetById(12), 0);
                OverlayObject_SetField54(10, 1);
                OverlayObject_SetField54(9, 1);
                OverlayObject_SetField54(8, 1);
            }
        }
        Engine_TaskWait(1);
        if (TorebiIzumi_RideEnded == 1) {
            break;
        }
    }
    return TorebiIzumi_RideResult;
}
