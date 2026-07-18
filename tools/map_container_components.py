#!/usr/bin/env python3
"""Exact semantic converters for map-container headers and components."""
import json
import struct
from pathlib import Path

from extract_resource import (
    decode_general_trace, decode_palette_trace, encode_general, encode_palette,
)
from tilemap import export_tilemap, import_tilemap


def general_plan(decoded, encoded, tokens, used, extra=None):
    replay = encode_general(decoded, tokens)
    if not encoded.startswith(replay) or used > len(encoded):
        raise ValueError("general-LZ token replay differs from component span")
    plan = {
        "format": 1,
        "codec": "golden-sun-general-lz",
        "decoded_size": len(decoded),
        "encoded_size": len(encoded),
        "tokens": tokens,
        "lookahead": encoded[len(replay):].hex(),
    }
    if extra:
        plan.update(extra)
    return plan


def tagged_palette_plan(decoded, encoded, tokens, used, extra=None):
    replay = b"\x01" + encode_palette(decoded, tokens)
    if not encoded.startswith(replay) or used > len(encoded):
        raise ValueError("tagged palette-LZ token replay differs from component span")
    plan = {
        "format": 1,
        "codec": "golden-sun-tagged-palette-lz",
        "tag": 1,
        "decoded_size": len(decoded),
        "encoded_size": len(encoded),
        "tokens": tokens,
        "lookahead": encoded[len(replay):].hex(),
    }
    if extra:
        plan.update(extra)
    return plan


def decode_component(encoded):
    if encoded and encoded[0] == 0:
        decoded, used, tokens = decode_general_trace(
            encoded, 0, len(encoded), 0x200000)
        return decoded, used, tokens, general_plan
    if encoded and encoded[0] == 1:
        decoded, used, tokens = decode_palette_trace(
            encoded, 1, len(encoded), 0x200000)
        return decoded, used, tokens, tagged_palette_plan
    raise ValueError("unsupported map-component compression tag")


def encode_plan(decoded, plan):
    if plan.get("format") != 1 or plan.get("codec") not in (
            "golden-sun-general-lz", "golden-sun-tagged-palette-lz"):
        raise ValueError("unsupported map-component LZ plan")
    if len(decoded) != int(plan["decoded_size"]):
        raise ValueError("map-component decoded size differs from plan")
    if plan["codec"] == "golden-sun-general-lz":
        encoded = encode_general(decoded, plan["tokens"])
    else:
        if int(plan.get("tag", -1)) != 1:
            raise ValueError("tagged palette-LZ plan is missing tag 1")
        encoded = b"\x01" + encode_palette(decoded, plan["tokens"])
    return encoded + bytes.fromhex(plan.get("lookahead", ""))


def export_header(container, source):
    header = container[:0x3c]
    if len(header) != 0x3c:
        raise ValueError("map container is shorter than its 0x3c header")
    document = {
        "format": 1,
        "parameters": list(header[:0x0c]),
        "records": [list(struct.unpack_from("<4H", header, 0x0c + 8 * index))
                    for index in range(3)],
        "component_offsets": [
            f"0x{value:x}"
            for value in struct.unpack_from("<6I", header, 0x24)],
    }
    source.write_text(json.dumps(document, indent=2) + "\n")
    if build_header(source) != header:
        raise AssertionError("container header does not round-trip")
    return [int(value, 0) for value in document["component_offsets"]]


def build_header(source, offsets_check=None):
    document = json.loads(source.read_text())
    if document.get("format") != 1:
        raise ValueError("unsupported map container header source")
    parameters = document["parameters"]
    records = document["records"]
    offsets = [int(value, 0) for value in document["component_offsets"]]
    if len(parameters) != 12 or any(not 0 <= int(value) <= 0xff
                                    for value in parameters):
        raise ValueError("container header requires twelve parameter bytes")
    if (len(records) != 3 or
            any(len(record) != 4 or any(not 0 <= int(value) <= 0xffff
                                        for value in record)
                for record in records)):
        raise ValueError("container header requires three four-u16 records")
    if len(offsets) != 6 or any(not 0 <= value <= 0xffffffff
                                for value in offsets):
        raise ValueError("container header requires six u32 offsets")
    if offsets_check is not None:
        for slot in range(6):
            if offsets[slot] != int(offsets_check.get(slot, 0)):
                raise ValueError(
                    f"header offset {slot} differs from the claimed "
                    "component span")
    return (bytes(int(value) for value in parameters) +
            b"".join(struct.pack("<4H", *(int(value) for value in record))
                     for record in records) +
            struct.pack("<6I", *offsets))


def decode_metatiles(decoded):
    if not decoded or decoded[0] not in (0, 1, 2) or (len(decoded) - 1) % 8:
        raise ValueError("invalid 2x2-metatile component")
    mode = decoded[0]
    if mode in (0, 2):
        count = (len(decoded) - 1) // 2
        entries = list(struct.unpack_from(f"<{count}H", decoded, 1))
        if mode == 2:
            previous = 0
            for index, value in enumerate(entries):
                entries[index] = value ^ previous
                previous = entries[index]
        return mode, entries

    # モード1は上位・下位バイトを別平面に置き、各値を
    # 直前の復元値から予測する。
    count = (len(decoded) - 1) // 2
    previous = 0
    entries = []
    for index in range(count):
        value = decoded[1 + index] << 8 | decoded[1 + count + index]
        if mode == 1:
            value ^= previous
        previous = value
        entries.append(value)
    return mode, entries


def encode_metatiles(entries, mode):
    if mode not in (0, 1, 2) or not entries or len(entries) % 4:
        raise ValueError("metatiles require mode 0/1/2 and groups of four u16 entries")
    if any(not 0 <= value <= 0xffff for value in entries):
        raise ValueError("metatile entry is outside u16")
    if mode in (0, 2):
        transformed = []
        previous = 0
        for value in entries:
            transformed.append(value ^ previous if mode == 2 else value)
            previous = value
        return bytes([mode]) + struct.pack(
            f"<{len(transformed)}H", *transformed)

    planar = []
    previous = 0
    for value in entries:
        planar.append(value ^ previous if mode == 1 else value)
        previous = value
    return (bytes([mode]) + bytes(value >> 8 for value in planar) +
            bytes(value & 0xff for value in planar))


def export_metatiles(encoded, source, plan_path):
    decoded, used, tokens, make_plan = decode_component(encoded)
    mode, entries = decode_metatiles(decoded)
    raw = struct.pack(f"<{len(entries)}H", *entries)
    source.write_text(export_tilemap(raw, 4))
    plan = make_plan(decoded, encoded, tokens, used, {
        "component": "map-metatiles-2x2",
        "transform_mode": mode,
        "metatiles": len(entries) // 4,
    })
    plan_path.write_text(json.dumps(plan, separators=(",", ":")) + "\n")
    if build_metatiles(source, plan_path) != encoded:
        raise AssertionError("metatile component does not round-trip")
    return len(entries) // 4, mode


def build_metatiles(source, plan_path):
    plan = json.loads(plan_path.read_text())
    raw = import_tilemap(source.read_text())
    entries = struct.unpack(f"<{len(raw) // 2}H", raw)
    decoded = encode_metatiles(entries, int(plan["transform_mode"]))
    return encode_plan(decoded, plan)


def export_descriptors(encoded, source, plan_path):
    decoded, used, tokens, make_plan = decode_component(encoded)
    if len(decoded) % 4:
        raise ValueError("descriptor component is not a sequence of four-byte records")
    records = [list(decoded[offset:offset + 4])
               for offset in range(0, len(decoded), 4)]
    source.write_text(json.dumps({
        "format": 1, "record_size": 4, "records": records,
    }, indent=2) + "\n")
    plan = make_plan(decoded, encoded, tokens, used, {
        "component": "map-descriptors-4byte", "records": len(records),
    })
    plan_path.write_text(json.dumps(plan, separators=(",", ":")) + "\n")
    if build_descriptors(source, plan_path) != encoded:
        raise AssertionError("descriptor component does not round-trip")
    return len(records)


def build_descriptors(source, plan_path):
    document = json.loads(source.read_text())
    if document.get("format") != 1 or document.get("record_size") != 4:
        raise ValueError("unsupported map descriptor source")
    records = document.get("records", [])
    if any(len(record) != 4 or any(not 0 <= int(value) <= 255 for value in record)
           for record in records):
        raise ValueError("map descriptors must contain four byte values")
    decoded = bytes(int(value) for record in records for value in record)
    return encode_plan(decoded, json.loads(plan_path.read_text()))


def decode_queues(decoded):
    if len(decoded) % 2:
        raise ValueError("animation queue stream has a partial u16")
    words = list(struct.unpack(f"<{len(decoded) // 2}H", decoded))
    queues = []
    cursor = 0
    while cursor < len(words) and words[cursor] != 0xffff:
        header = words[cursor]
        if header >> 8 != 0xfd:
            raise ValueError("animation queue is missing its FDxx header")
        cursor += 1
        commands = []
        while cursor < len(words) and words[cursor] != 0xfe00:
            if cursor + 1 >= len(words):
                raise ValueError("animation queue has a partial command pair")
            commands.append([words[cursor], words[cursor + 1]])
            cursor += 2
        if cursor >= len(words):
            raise ValueError("animation queue is missing FE00")
        cursor += 1
        queues.append({"header": f"0x{header:04x}", "commands": commands})
    if cursor != len(words) - 1 or words[cursor] != 0xffff:
        raise ValueError("animation queue stream is missing final FFFF")
    return queues


def encode_queues(queues):
    words = []
    for queue in queues:
        header = int(queue["header"], 0)
        if header >> 8 != 0xfd:
            raise ValueError("animation queue header must be FDxx")
        words.append(header)
        for command in queue["commands"]:
            if len(command) != 2 or any(not 0 <= int(value) <= 0xffff
                                        for value in command):
                raise ValueError("animation command must contain two u16 values")
            words.extend(map(int, command))
        words.append(0xfe00)
    words.append(0xffff)
    return struct.pack(f"<{len(words)}H", *words)


def word_list_document(decoded):
    if len(decoded) % 2:
        raise ValueError("component word stream has a partial u16")
    words = struct.unpack(f"<{len(decoded) // 2}H", decoded)
    return {"format": 1, "word_size": 2,
            "words": [f"0x{word:04x}" for word in words]}


def encode_word_list(document):
    words = [int(word, 0) for word in document["words"]]
    if any(not 0 <= word <= 0xffff for word in words):
        raise ValueError("component word is outside u16")
    return struct.pack(f"<{len(words)}H", *words)


def export_queues(encoded, source, plan_path):
    decoded, used, tokens, make_plan = decode_component(encoded)
    try:
        queues = decode_queues(decoded)
    except ValueError:
        document = word_list_document(decoded)
        report = {"component": "map-animation-words",
                  "words": len(document["words"])}
        counts = ("raw", len(document["words"]))
    else:
        document = {"format": 1, "terminators": ["0xfe00", "0xffff"],
                    "queues": queues}
        commands = sum(len(queue["commands"]) for queue in queues)
        report = {"component": "map-animation-queues",
                  "queues": len(queues), "commands": commands}
        counts = (len(queues), commands)
    source.write_text(json.dumps(document, indent=2) + "\n")
    plan = make_plan(decoded, encoded, tokens, used, report)
    plan_path.write_text(json.dumps(plan, separators=(",", ":")) + "\n")
    if build_queues(source, plan_path) != encoded:
        raise AssertionError("animation queue component does not round-trip")
    return counts


def build_queues(source, plan_path):
    document = json.loads(source.read_text())
    if document.get("format") != 1:
        raise ValueError("unsupported animation queue source")
    if "queues" in document:
        decoded = encode_queues(document["queues"])
    else:
        decoded = encode_word_list(document)
    return encode_plan(decoded, json.loads(plan_path.read_text()))


def decode_blend_commands(decoded):
    if len(decoded) % 2:
        raise ValueError("blend animation stream has a partial u16")
    words = struct.unpack(f"<{len(decoded) // 2}H", decoded)
    commands = []
    cursor = 0
    while cursor < len(words):
        word = words[cursor]
        cursor += 1
        if word == 0xffff:
            commands.append({"op": "reset"})
        elif word >> 8 == 0xfe:
            if word == 0xfeff:
                commands.append({"op": "stop"})
            else:
                commands.append({"op": "jump", "target_pair": word & 0xff})
        elif word & 0xf000 == 0x3000:
            commands.append({
                "op": "set_blend_control", "value": f"0x{word:04x}",
            })
        else:
            if cursor >= len(words):
                raise ValueError("blend value is missing its duration")
            commands.append({
                "op": "write_blend_value", "value": f"0x{word:04x}",
                "duration": words[cursor],
            })
            cursor += 1
    return commands


def encode_blend_commands(commands):
    words = []
    for command in commands:
        operation = command.get("op")
        if operation == "reset":
            words.append(0xffff)
        elif operation == "stop":
            words.append(0xfeff)
        elif operation == "jump":
            target = int(command["target_pair"])
            if not 0 <= target < 0xff:
                raise ValueError("blend jump target pair is outside 0..254")
            words.append(0xfe00 | target)
        elif operation == "set_blend_control":
            value = int(command["value"], 0)
            if not 0 <= value <= 0xffff or value & 0xf000 != 0x3000:
                raise ValueError("blend-control value must be a 3xxx u16")
            words.append(value)
        elif operation == "write_blend_value":
            value = int(command["value"], 0)
            duration = int(command["duration"])
            if (not 0 <= value <= 0xffff or value == 0xffff or
                    value >> 8 == 0xfe or value & 0xf000 == 0x3000):
                raise ValueError("blend value collides with a control command")
            if not 0 <= duration <= 0xffff:
                raise ValueError("blend duration is outside u16")
            words.extend((value, duration))
        else:
            raise ValueError(f"unsupported blend animation operation: {operation}")
    return struct.pack(f"<{len(words)}H", *words)


def export_blend_animation(encoded, source, plan_path):
    decoded, used, tokens, make_plan = decode_component(encoded)
    try:
        commands = decode_blend_commands(decoded)
    except ValueError:
        document = word_list_document(decoded)
        report = {"component": "map-blend-words",
                  "words": len(document["words"])}
        count = len(document["words"])
    else:
        document = {"format": 1, "word_size": 2, "commands": commands}
        report = {"component": "map-blend-animation",
                  "commands": len(commands)}
        count = len(commands)
    source.write_text(json.dumps(document, indent=2) + "\n")
    plan = make_plan(decoded, encoded, tokens, used, report)
    plan_path.write_text(json.dumps(plan, separators=(",", ":")) + "\n")
    if build_blend_animation(source, plan_path) != encoded:
        raise AssertionError("blend animation component does not round-trip")
    return count, plan["codec"]


def build_blend_animation(source, plan_path):
    document = json.loads(source.read_text())
    if document.get("format") != 1 or document.get("word_size") != 2:
        raise ValueError("unsupported blend animation source")
    if "commands" in document:
        decoded = encode_blend_commands(document["commands"])
    else:
        decoded = encode_word_list(document)
    return encode_plan(decoded, json.loads(plan_path.read_text()))


def export_sparse(encoded, source):
    terminator = encoded.find(b"\xff\xff\xff")
    if terminator < 0 or terminator % 3 or any(encoded[terminator + 3:]):
        raise ValueError("invalid sparse-cell triple stream")
    records = [list(encoded[offset:offset + 3])
               for offset in range(0, terminator, 3)]
    source.write_text(json.dumps({
        "format": 1, "record_size": 3, "terminator": "ffffff",
        "alignment_zeros": len(encoded) - terminator - 3, "records": records,
    }, indent=2) + "\n")
    if build_sparse(source) != encoded:
        raise AssertionError("sparse-cell component does not round-trip")
    return len(records)


def build_sparse(source):
    document = json.loads(source.read_text())
    if (document.get("format") != 1 or document.get("record_size") != 3 or
            document.get("terminator") != "ffffff"):
        raise ValueError("unsupported sparse-cell source")
    records = document.get("records", [])
    if any(len(record) != 3 or any(not 0 <= int(value) <= 255 for value in record)
           for record in records):
        raise ValueError("sparse-cell records must contain three byte values")
    padding = int(document["alignment_zeros"])
    if not 0 <= padding <= 3:
        raise ValueError("sparse-cell alignment is outside 0..3")
    return (bytes(int(value) for record in records for value in record) +
            b"\xff\xff\xff" + bytes(padding))
