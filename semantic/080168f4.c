#include "layout_guard.h"
#include "types.h"

enum FieldCommand_080168f4 {
    FIELD_COMMAND_END = 0,
    FIELD_COMMAND_OPEN = 1,
    FIELD_COMMAND_CLOSE = 2,
    FIELD_COMMAND_RISE = 3,
    FIELD_COMMAND_WAIT_SHORT = 4,
    FIELD_COMMAND_WAIT_INPUT_SHORT = 5,
    FIELD_COMMAND_WAIT_INPUT_LONG = 6,
    FIELD_COMMAND_RESET_TEXT_OPTIONS = 7,
    FIELD_COMMAND_SET_TEXT_STYLE = 8,
    FIELD_COMMAND_SET_TEXT_POSITION = 9,
    FIELD_COMMAND_SET_TEXT_CONTEXT = 10,
    FIELD_COMMAND_SET_WINDOW_VALUE = 15,
    FIELD_COMMAND_STOP = 30,
    FIELD_COMMAND_SPACE = 0x20,
    FIELD_COMMAND_MODIFIER_4000 = 0xde,
    FIELD_COMMAND_MODIFIER_8000 = 0xdf
};

enum FieldResult_080168f4 {
    FIELD_RESULT_NONE = 0,
    FIELD_RESULT_FINISHED = 8,
    FIELD_RESULT_CANCELLED = 9
};

struct WindowWork_080168f4 {
    s32 reserved00;
    struct WindowWork_080168f4 *self;
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u16 reserved10;
    u16 value;
    u16 reserved14;
    u16 flags;
};

struct FieldScriptChannel_080168f4 {
    struct WindowWork_080168f4 *window;
    u16 x_accumulator;
    u16 y_accumulator;
    u16 text_measurement[4];
    u16 animation_phase;
    u16 command_index;
    u16 command_delay;
    u16 text_style;
    u16 text_position;
    u16 text_context;
    u16 forced_delay;
    u16 x_origin;
    u16 stopped;
    u16 movement_delay;
    u16 preserve_geometry;
    u16 reserved26;
};

struct FieldEngine_080168f4 {
    u8 tilemap_and_work[0xea4];
    u8 alternate_layout;
    u8 rapid_mode;
    u8 reserved_ea6[2];
    u16 active_text_context;
    u16 reserved_eaa;
    u16 active_text_position;
    u16 active_text_style;
    u16 command_queue[0x200];
    u8 reserved_12b0[6];
    u16 active_sound;
    u8 reserved_12b8[0x3c];
    u16 movement_sound;
    u16 movement_sound_delay;
    u8 preview_active;
};

struct GlyphMetrics_080168f4 {
    u16 advance;
    u8 reserved02[0x1e];
};

LAYOUT_OFFSET_GUARD(
    WindowWork080168f4_Width,
    struct WindowWork_080168f4,
    width,
    0x08);
LAYOUT_OFFSET_GUARD(
    WindowWork080168f4_X,
    struct WindowWork_080168f4,
    x,
    0x0c);
LAYOUT_OFFSET_GUARD(
    WindowWork080168f4_Value,
    struct WindowWork_080168f4,
    value,
    0x12);
LAYOUT_OFFSET_GUARD(
    WindowWork080168f4_Flags,
    struct WindowWork_080168f4,
    flags,
    0x16);
LAYOUT_SIZE_GUARD(
    FieldScriptChannel080168f4_Size,
    struct FieldScriptChannel_080168f4,
    0x28);
LAYOUT_OFFSET_GUARD(
    FieldScriptChannel080168f4_CommandIndex,
    struct FieldScriptChannel_080168f4,
    command_index,
    0x12);
LAYOUT_OFFSET_GUARD(
    FieldScriptChannel080168f4_ForcedDelay,
    struct FieldScriptChannel_080168f4,
    forced_delay,
    0x1c);
LAYOUT_OFFSET_GUARD(
    FieldScriptChannel080168f4_Stopped,
    struct FieldScriptChannel_080168f4,
    stopped,
    0x20);
LAYOUT_OFFSET_GUARD(
    FieldScriptChannel080168f4_PreserveGeometry,
    struct FieldScriptChannel_080168f4,
    preserve_geometry,
    0x24);
LAYOUT_OFFSET_GUARD(
    FieldEngine080168f4_AlternateLayout,
    struct FieldEngine_080168f4,
    alternate_layout,
    0xea4);
LAYOUT_OFFSET_GUARD(
    FieldEngine080168f4_RapidMode,
    struct FieldEngine_080168f4,
    rapid_mode,
    0xea5);
LAYOUT_OFFSET_GUARD(
    FieldEngine080168f4_CommandQueue,
    struct FieldEngine_080168f4,
    command_queue,
    0xeb0);
LAYOUT_OFFSET_GUARD(
    FieldEngine080168f4_ActiveSound,
    struct FieldEngine_080168f4,
    active_sound,
    0x12b6);
LAYOUT_OFFSET_GUARD(
    FieldEngine080168f4_MovementSound,
    struct FieldEngine_080168f4,
    movement_sound,
    0x12f4);
LAYOUT_OFFSET_GUARD(
    FieldEngine080168f4_PreviewActive,
    struct FieldEngine_080168f4,
    preview_active,
    0x12f8);
LAYOUT_SIZE_GUARD(
    GlyphMetrics080168f4_Size,
    struct GlyphMetrics_080168f4,
    0x20);

extern struct FieldEngine_080168f4 *Data_03001e8c;
extern volatile s32 Data_03001ae8;
extern volatile s32 Data_03001af8;
extern volatile u16 Data_03001cd0;
extern u8 Data_0200044c;
extern const u8 Data_08073808[];
extern const u8 Data_0807380b[];
extern const u8 Data_0807380e[];
extern const struct GlyphMetrics_080168f4 Data_08032224[];

void Func_08003f3c(u32 sound);
void Func_08016178(u32 x, u32 y, u32 width, u32 height);
void Func_08016478(struct WindowWork_080168f4 *window);
void Func_080167ac(struct FieldScriptChannel_080168f4 *channel);
void Func_080167d8(struct FieldScriptChannel_080168f4 *channel);
void Func_080167e0(s32 active);
void Func_080170f8(u16 x, u16 y, u16 width, u16 height);
void Func_0801868c(
    s32 text,
    s32 *x,
    s32 *y,
    s32 *width,
    s32 *height,
    void *measurement,
    s32 flags);
s32 Func_08018cac(
    struct WindowWork_080168f4 *window,
    u32 command,
    s32 x,
    s32 y,
    s32 flags);
void Func_0801999c(struct FieldScriptChannel_080168f4 *channel);
s32 Func_080199ec(struct FieldScriptChannel_080168f4 *channel);
void Func_080f9010(s32 sound);

/*
 * Consume queued commands for one field window. The queue mixes control
 * commands with printable tokens; printable pairs are packed when their
 * measured advances fit on one step.
 */
s32 Func_080168f4(struct FieldScriptChannel_080168f4 *channel) {
    struct FieldEngine_080168f4 *engine = Data_03001e8c;
    struct WindowWork_080168f4 *window;
    s32 budget = Data_0807380b[Data_0200044c];

    if (engine->rapid_mode != 0) {
        u16 speed = Data_03001cd0;

        if (speed > 2) {
            speed = 2;
        }
        budget = speed * 5 + 3;
    }

    if (channel->forced_delay != 0) {
        Func_080167e0(1);
        channel->forced_delay--;
        return FIELD_RESULT_NONE;
    }

    if (Data_03001ae8 == 0 && channel->movement_delay != 0) {
        channel->movement_delay--;
        return FIELD_RESULT_NONE;
    }

    while (budget != 0) {
        u16 command = FIELD_COMMAND_END;

        if (channel->stopped == 0) {
            command = engine->command_queue[channel->command_index];
        }

        switch (command) {
        case FIELD_COMMAND_RISE:
            channel->x_accumulator = channel->x_origin;
            if (channel->window->flags & 8) {
                if (channel->y_accumulator > 0xcff) {
                    Func_080167d8(channel);
                    budget = 1;
                } else {
                    channel->y_accumulator += 0xd00;
                }
            } else {
                channel->y_accumulator += 0xf00;
                if (channel->animation_phase <= 2) {
                    channel->animation_phase++;
                }
            }
            goto known_command;

        case 11:
        case 12:
        case 13:
        case 14:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
            goto known_command;

        case FIELD_COMMAND_OPEN:
            if (engine->alternate_layout != 0 &&
                channel->command_delay < 0x384) {
                Data_03001af8 = 0;
            }
            channel->command_delay = 0x397;

            if (Func_080199ec(channel) == 0) {
                window = channel->window;
                if (window->width != 0 &&
                    window->height != 0 &&
                    engine->preview_active == 0) {
                    Func_08018cac(
                        window,
                        1,
                        window->width * 4 - 8,
                        window->height * 8 - 0x10,
                        1);
                    engine->preview_active = 1;
                }
            } else {
                s32 x;
                s32 y;
                s32 width;
                s32 height;
                s32 old_width;
                s32 old_height;
                u16 next_index;

                window = channel->window;
                x = window->x;
                y = window->y;
                width = window->width;
                height = window->height;
                old_width = width;
                old_height = height;
                next_index = channel->command_index;
                engine->preview_active = 0;

                Func_08016478(window);
                if (channel->preserve_geometry == 0 &&
                    (window->width | window->height) != 0) {
                    Func_08016178(
                        window->x,
                        window->y,
                        window->width,
                        window->height);
                }

                next_index = (next_index + 1) & 0x1ff;
                if (engine->command_queue[next_index] != 0 &&
                    (window->width | window->height) != 0) {
                    if (channel->preserve_geometry != 0) {
                        Func_08016178(
                            window->x,
                            window->y,
                            window->width,
                            window->height);
                    } else {
                        s32 width_adjustment;

                        Func_0801868c(
                            next_index,
                            &x,
                            &y,
                            &width,
                            &height,
                            channel->text_measurement,
                            0);

                        if (window->flags & 0x80) {
                            if (old_height != height) {
                                y -= height - old_height;
                            }
                            if (y < 0) {
                                y = 0;
                            }
                        }

                        if ((window->flags & 0x100) == 0) {
                            width_adjustment = old_width - width;
                            if (width_adjustment < 0) {
                                width_adjustment += 3;
                            }
                            x += width_adjustment >> 2;
                            Func_0801868c(
                                next_index,
                                &x,
                                &y,
                                &width,
                                &height,
                                channel->text_measurement,
                                2);
                        }

                        window->x = x;
                        window->y = y;
                        window->width = width;
                        window->height = height;
                    }

                    Func_080170f8(
                        window->x,
                        window->y,
                        window->width,
                        window->height);
                }

                channel->x_accumulator = channel->x_origin;
                channel->y_accumulator = 0;
                channel->animation_phase = 0;
                Func_08003f3c(engine->active_sound);
                engine->active_sound = 0x63;
            }
            goto known_command;

        case FIELD_COMMAND_CLOSE:
            if (engine->alternate_layout != 0 &&
                channel->command_delay < 0x384) {
                Data_03001af8 = 0;
            }
            if (Func_080199ec(channel) != 0) {
                return FIELD_RESULT_CANCELLED;
            }
            channel->command_delay = 0x397;
            goto known_command;

        case FIELD_COMMAND_WAIT_INPUT_SHORT:
            if (channel->command_delay == 0) {
                channel->command_delay = 0x14;
            }
            engine->movement_sound_delay = 0;
            Func_0801999c(channel);
            goto known_command;

        case FIELD_COMMAND_WAIT_INPUT_LONG:
            if (channel->command_delay == 0) {
                channel->command_delay = 0x78;
            }
            engine->movement_sound_delay = 0;
            Func_0801999c(channel);
            goto known_command;

        case FIELD_COMMAND_WAIT_SHORT:
            if (channel->command_delay == 0) {
                channel->command_delay = 0x3c;
            }
            engine->movement_sound_delay = 0;
            goto known_command;

        case FIELD_COMMAND_SET_TEXT_STYLE:
            channel->command_index =
                (channel->command_index + 1) & 0x1ff;
            channel->text_style =
                engine->command_queue[channel->command_index];
            Func_080167ac(channel);
            goto known_command;

        case FIELD_COMMAND_SET_TEXT_POSITION:
            channel->command_index =
                (channel->command_index + 1) & 0x1ff;
            channel->text_position =
                engine->command_queue[channel->command_index];
            Func_080167ac(channel);
            goto known_command;

        case FIELD_COMMAND_SET_TEXT_CONTEXT:
            channel->command_index =
                (channel->command_index + 1) & 0x1ff;
            channel->text_context =
                engine->command_queue[channel->command_index];
            Func_080167ac(channel);
            goto known_command;

        case FIELD_COMMAND_RESET_TEXT_OPTIONS:
            channel->text_style = 0xf;
            channel->text_position = 0;
            channel->text_context = 0xa;
            Func_080167ac(channel);
            goto known_command;

        case FIELD_COMMAND_SET_WINDOW_VALUE:
            channel->command_index =
                (channel->command_index + 1) & 0x1ff;
            channel->window->value =
                engine->command_queue[channel->command_index];
            channel->command_delay = 0xa;
            channel->command_index =
                (channel->command_index + 1) & 0x1ff;
            goto known_command;

        case FIELD_COMMAND_END:
        case FIELD_COMMAND_STOP:
            channel->stopped = 1;
            return FIELD_RESULT_FINISHED;

        default:
        {
            u16 following;
            s32 x = (channel->x_accumulator + 0x80) >> 8;
            s32 y = (channel->y_accumulator + 0x80) >> 8;
            s32 movement;

            if (engine->alternate_layout != 0) {
                x += 8;
            }

            following = engine->command_queue[
                (channel->command_index + 1) & 0x1ff];
            if (following == FIELD_COMMAND_MODIFIER_4000) {
                command |= 0x4000;
                channel->command_index =
                    (channel->command_index + 1) & 0x1ff;
            } else if (following == FIELD_COMMAND_MODIFIER_8000) {
                command |= 0x8000;
                channel->command_index =
                    (channel->command_index + 1) & 0x1ff;
            }

            window = channel->window;
            if ((window->flags & 8) == 0 &&
                command > FIELD_COMMAND_SPACE &&
                following > FIELD_COMMAND_SPACE &&
                (u32)((Data_08032224[command - FIELD_COMMAND_SPACE].advance +
                       Data_08032224[following - FIELD_COMMAND_SPACE].advance)
                      << 16) <= 0x000f0000) {
                command |= following << 8;
                channel->command_index =
                    (channel->command_index + 1) & 0x1ff;
            }

            movement = Func_08018cac(window, command, x, y, 0);
            channel->movement_delay = Data_08073808[Data_0200044c];
            if (movement != 0) {
                if (engine->movement_sound != 0) {
                    if (engine->movement_sound_delay == 0) {
                        if (command != FIELD_COMMAND_SPACE) {
                            Func_080f9010(
                                engine->movement_sound + (command & 3));
                            engine->movement_sound_delay =
                                Data_0807380e[Data_0200044c];
                        }
                    } else {
                        engine->movement_sound_delay--;
                    }
                }

                movement <<= 8;
                if (command == FIELD_COMMAND_SPACE) {
                    movement += channel->text_measurement[
                        channel->animation_phase];
                }
                channel->x_accumulator += movement;
            }

            if (command == FIELD_COMMAND_SPACE && engine->rapid_mode == 0) {
                budget = 1;
            }
            goto finish_command;
        }
        }

known_command:
        if (engine->rapid_mode == 0) {
            budget = 1;
        }

finish_command:
        if (channel->command_delay == 0 || --channel->command_delay == 0) {
            channel->command_index =
                (channel->command_index + 1) & 0x1ff;
        }
        budget--;
    }

    return FIELD_RESULT_NONE;
}
