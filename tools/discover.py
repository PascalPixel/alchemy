#!/usr/bin/env python3
import argparse
import json
import struct
from collections import deque
from pathlib import Path

ROM_BASE = 0x08000000


def sx(value, bits):
    sign = 1 << (bits - 1)
    return (value ^ sign) - sign


class Discovery:
    def __init__(self, data):
        self.data = data
        self.limit = ROM_BASE + len(data)
        self.functions = {}
        self.instructions = {}
        self.calls = set()
        self.unresolved = set()
        self.conflicts = []
        self.data_refs = set()
        self.pointer_tables = {}

    def inside(self, address, size=1):
        return ROM_BASE <= address and address + size <= self.limit

    def u16(self, address):
        return struct.unpack_from("<H", self.data, address - ROM_BASE)[0]

    def u32(self, address):
        return struct.unpack_from("<I", self.data, address - ROM_BASE)[0]

    def add_seed(self, address, mode, source):
        if mode == "thumb":
            address &= ~1
        if not self.inside(address, 2 if mode == "thumb" else 4):
            return False
        old = self.functions.get(address)
        if old and old["mode"] != mode:
            self.conflicts.append({
                "address": address, "old": old["mode"], "new": mode,
                "source": source,
            })
            return False
        if old:
            old["sources"].add(source)
            return False
        self.functions[address] = {
            "mode": mode, "sources": {source}, "blocks": set(),
            "instructions": set(), "callees": set(), "unresolved": set(),
        }
        return True

    def initial_seeds(self):
        reset = self.u32(ROM_BASE)
        if reset >> 24 != 0xEA:
            raise ValueError("reset vector is not an unconditional ARM branch")
        entry = ROM_BASE + 8 + (sx(reset & 0xFFFFFF, 24) << 2)
        self.add_seed(entry, "arm", "reset")

        for address in range(entry, min(entry + 0x100, self.limit), 4):
            value = self.u32(address)
            if value & 1 and self.inside(value & ~1, 2):
                self.add_seed(value, "thumb", "startup-literal")

        veneer = ROM_BASE + 0xC0
        while veneer + 8 <= entry and self.u32(veneer) == 0x47204C00:
            self.add_seed(veneer, "thumb", "header-veneer")
            target = self.u32(veneer + 4)
            if target & 1:
                self.add_seed(target, "thumb", "header-veneer-target")
            veneer += 8
        return entry

    def note_instruction(self, function, address, size, kind):
        old = self.instructions.get(address)
        if old and old["mode"] != function["mode"]:
            self.conflicts.append({
                "address": address, "old": old["mode"],
                "new": function["mode"], "source": "flow",
            })
            return False
        self.instructions[address] = {
            "size": size, "mode": function["mode"], "kind": kind,
        }
        function["instructions"].add(address)
        return True

    def call(self, function, source, target, mode):
        if mode == "thumb":
            target &= ~1
        if not self.inside(target, 2 if mode == "thumb" else 4):
            function["unresolved"].add(source)
            self.unresolved.add(source)
            return
        function["callees"].add(target)
        self.calls.add((source, target, mode))
        self.add_seed(target, mode, f"call:{source:08x}")

    def walk_arm(self, start, function, queue, seen, initial_constants):
        pc = start
        constants = dict(initial_constants)
        while self.inside(pc, 4) and pc not in seen:
            seen.add(pc)
            word = self.u32(pc)
            cond = word >> 28
            kind = "linear"
            successors = []
            stop = False

            if ((word >> 26) & 3) == 1 and not word & (1 << 25):
                load = bool(word & (1 << 20))
                base_register = (word >> 16) & 15
                destination = (word >> 12) & 15
                preindexed = bool(word & (1 << 24))
                writeback = bool(word & (1 << 21))
                if load:
                    constants.pop(destination, None)
                if load and base_register == 15 and preindexed and not writeback:
                    displacement = word & 0xFFF
                    literal = pc + 8
                    literal += displacement if word & (1 << 23) else -displacement
                    if self.inside(literal, 4):
                        value = self.u32(literal)
                        constants[destination] = value
                        if self.inside(value & ~1, 2):
                            self.data_refs.add(value & ~1)

            if (word & 0x0E000000) == 0x0A000000:
                link = bool(word & 0x01000000)
                target = pc + 8 + (sx(word & 0xFFFFFF, 24) << 2)
                kind = "call" if link else "branch"
                if link:
                    self.call(function, pc, target, "arm")
                    for register in (0, 1, 2, 3, 12, 14):
                        constants.pop(register, None)
                else:
                    successors.append(target)
                    stop = cond == 0xE
                if cond != 0xE:
                    successors.append(pc + 4)
                    stop = True
            elif (word & 0x0FFFFFF0) == 0x012FFF10:
                register = word & 15
                if register == 14:
                    kind = "return"
                else:
                    kind = "indirect"
                    value = constants.get(register)
                    if value is not None and self.inside(value & ~1, 2):
                        self.call(
                            function, pc, value,
                            "thumb" if value & 1 else "arm")
                    else:
                        function["unresolved"].add(pc)
                        self.unresolved.add(pc)
                stop = True
            elif (word & 0x0FFFFFFF) == 0x01A0F00E:
                kind = "return"
                stop = True
            elif (word & 0x0E10F000) == 0x0410F000:
                kind = "indirect"
                function["unresolved"].add(pc)
                self.unresolved.add(pc)
                stop = True
            elif (word & 0x0E100000) == 0x08100000 and word & (1 << 15):
                kind = "return"
                stop = True
            elif ((word >> 26) & 3) == 0:
                opcode = (word >> 21) & 15
                destination = (word >> 12) & 15
                if destination == 15 and opcode not in (8, 9, 10, 11):
                    kind = "indirect"
                    function["unresolved"].add(pc)
                    self.unresolved.add(pc)
                    stop = True

            if not self.note_instruction(function, pc, 4, kind):
                return
            for target in successors:
                if self.inside(target, 4):
                    queue.append((target, dict(constants)))
            if stop:
                return
            pc += 4

    def walk_thumb(self, start, function, queue, seen, initial_constants):
        pc = start
        constants = dict(initial_constants)
        while self.inside(pc, 2) and pc not in seen:
            seen.add(pc)
            half = self.u16(pc)
            kind = "linear"
            size = 2
            successors = []
            stop = False

            written = set()
            if (half & 0xE000) == 0x0000:
                written.add(half & 7)
            elif (half & 0xE000) == 0x2000:
                if (half & 0xF800) != 0x2800:
                    written.add((half >> 8) & 7)
            elif (half & 0xFC00) == 0x4000:
                operation = (half >> 6) & 15
                if operation not in (8, 10, 11):
                    written.add(half & 7)
            elif (half & 0xFC00) == 0x4400:
                operation = (half >> 8) & 3
                if operation != 1:
                    written.add((half & 7) | ((half >> 4) & 8))
            elif (half & 0xF000) == 0x5000:
                operation = (half >> 9) & 7
                if operation >= 3:
                    written.add(half & 7)
            elif (half & 0xE000) == 0x6000 and half & 0x0800:
                written.add(half & 7)
            elif (half & 0xF000) == 0x8000 and half & 0x0800:
                written.add(half & 7)
            elif (half & 0xF000) == 0x9000 and half & 0x0800:
                written.add((half >> 8) & 7)
            elif (half & 0xFF00) == 0xBC00:
                written.update(r for r in range(8) if half & (1 << r))
            elif (half & 0xF800) == 0xC800:
                written.update(r for r in range(8) if half & (1 << r))
            for register in written:
                constants.pop(register, None)

            if (half & 0xF800) == 0x4800:
                register = (half >> 8) & 7
                literal = ((pc + 4) & ~3) + ((half & 0xFF) << 2)
                if self.inside(literal, 4):
                    value = self.u32(literal)
                    constants[register] = value
                    if self.inside(value & ~1, 2):
                        self.data_refs.add(value & ~1)
            elif (half & 0xF800) == 0xA000:
                register = (half >> 8) & 7
                constants[register] = ((pc + 4) & ~3) + ((half & 0xFF) << 2)
            elif (half & 0xF800) == 0x6800:
                destination = half & 7
                base_register = (half >> 3) & 7
                address = constants.get(base_register)
                if address is not None:
                    address += ((half >> 6) & 0x1F) << 2
                    if self.inside(address, 4):
                        value = self.u32(address)
                        constants[destination] = value
                        if self.inside(value & ~1, 2):
                            self.data_refs.add(value & ~1)
            elif (half & 0xFE00) == 0x5800:
                destination = half & 7
                base_register = (half >> 3) & 7
                offset_register = (half >> 6) & 7
                base_value = constants.get(base_register)
                offset_value = constants.get(offset_register)
                if base_value is not None and offset_value is not None:
                    address = base_value + offset_value
                    if self.inside(address, 4):
                        value = self.u32(address)
                        constants[destination] = value
                        if self.inside(value & ~1, 2):
                            self.data_refs.add(value & ~1)

            if (half & 0xF800) == 0xF000 and self.inside(pc, 4):
                low = self.u16(pc + 2)
                if (low & 0xF800) == 0xF800:
                    offset = sx(((half & 0x7FF) << 12) |
                                ((low & 0x7FF) << 1), 23)
                    target = pc + 4 + offset
                    kind = "call"
                    size = 4
                    self.call(function, pc, target, "thumb")
                    for register in (0, 1, 2, 3, 12, 14):
                        constants.pop(register, None)
            elif (half & 0xF800) == 0xE000:
                target = pc + 4 + (sx(half & 0x7FF, 11) << 1)
                kind = "branch"
                successors.append(target)
                stop = True
            elif (half & 0xF000) == 0xD000:
                condition = (half >> 8) & 0xF
                if condition < 0xE:
                    target = pc + 4 + (sx(half & 0xFF, 8) << 1)
                    kind = "conditional"
                    successors.extend((target, pc + 2))
                    stop = True
                elif condition == 0xF:
                    kind = "software-interrupt"
            elif (half & 0xFF87) == 0x4700:
                register = (half >> 3) & 15
                previous = self.u16(pc - 2) if self.inside(pc - 2, 2) else 0
                popped_return = (
                    register < 8 and
                    (previous & 0xFF00) == 0xBC00 and
                    previous & (1 << register)
                )
                if register == 14 or popped_return:
                    kind = "return"
                else:
                    kind = "indirect"
                    value = constants.get(register)
                    if value is not None and self.inside(value & ~1, 2):
                        self.call(
                            function, pc, value,
                            "thumb" if value & 1 else "arm")
                    else:
                        function["unresolved"].add(pc)
                        self.unresolved.add(pc)
                stop = True
            elif (half & 0xFF00) == 0xBD00:
                kind = "return"
                stop = True
            elif (half & 0xFC00) == 0x4400 and (half & 0x0087) == 0x0087:
                kind = "indirect"
                function["unresolved"].add(pc)
                self.unresolved.add(pc)
                stop = True

            if (half & 0xFC00) == 0x4600:
                destination = (half & 7) | ((half >> 4) & 8)
                source = (half >> 3) & 15
                constants.pop(destination, None)
                if source in constants:
                    constants[destination] = constants[source]

            if not self.note_instruction(function, pc, size, kind):
                return
            for target in successors:
                if self.inside(target, 2):
                    queue.append((target, dict(constants)))
            if stop:
                return
            pc += size

    def walk_function(self, address):
        function = self.functions[address]
        queue = deque([(address, {})])
        seen = set()
        while queue:
            block, constants = queue.popleft()
            if block in seen:
                continue
            function["blocks"].add(block)
            if function["mode"] == "arm":
                self.walk_arm(block, function, queue, seen, constants)
            else:
                self.walk_thumb(block, function, queue, seen, constants)

    def discover_global_pointer_tables(self):
        entries_by_bank = {}
        for entry in self.functions:
            bank = (entry - ROM_BASE) // 0x10000
            entries_by_bank[bank] = entries_by_bank.get(bank, 0) + 1
        code_banks = {bank for bank, count in entries_by_bank.items() if count >= 5}

        def plausible_thumb_entry(address):
            half = self.u16(address)
            return (
                (half & 0xFF00) == 0xB500 or
                half == 0x4770 or
                (half & 0xF800) == 0x2000
            )

        added = False
        offset = 0
        while offset + 8 <= len(self.data):
            source_bank = offset // 0x10000
            if source_bank not in code_banks:
                offset += 4
                continue
            targets = []
            cursor = offset
            while cursor + 4 <= len(self.data) and len(targets) < 1024:
                value = struct.unpack_from("<I", self.data, cursor)[0]
                target = value & ~1
                target_bank = (target - ROM_BASE) // 0x10000
                if (
                    not value & 1 or
                    not self.inside(target, 2) or
                    target_bank not in code_banks or
                    not plausible_thumb_entry(target)
                ):
                    break
                targets.append(target)
                cursor += 4
            if len(targets) < 2:
                offset += 4
                continue
            table = ROM_BASE + offset
            self.pointer_tables[table] = targets
            for target in targets:
                if target not in self.instructions:
                    added |= self.add_seed(
                        target, "thumb", f"global-pointer-table:{table:08x}")
            offset = cursor
        return added

    def discover_individual_function_pointers(self):
        entries_by_bank = {}
        for entry in self.functions:
            bank = (entry - ROM_BASE) // 0x10000
            entries_by_bank[bank] = entries_by_bank.get(bank, 0) + 1
        code_banks = {bank for bank, count in entries_by_bank.items() if count >= 5}
        added = False
        for offset in range(0, len(self.data) - 3, 4):
            if offset // 0x10000 not in code_banks:
                continue
            value = struct.unpack_from("<I", self.data, offset)[0]
            target = value & ~1
            if (
                not value & 1 or
                not self.inside(target, 2) or
                (target - ROM_BASE) // 0x10000 not in code_banks or
                (self.u16(target) & 0xFF00) != 0xB500 or
                target in self.instructions
            ):
                continue
            source = ROM_BASE + offset
            added |= self.add_seed(
                target, "thumb", f"function-pointer:{source:08x}")
        return added

    def discover_prologue_boundaries(self):
        entries_by_bank = {}
        for entry in self.functions:
            bank = (entry - ROM_BASE) // 0x10000
            entries_by_bank[bank] = entries_by_bank.get(bank, 0) + 1
        code_banks = {bank for bank, count in entries_by_bank.items() if count >= 5}
        added = False
        for bank in code_banks:
            start = ROM_BASE + bank * 0x10000
            end = min(start + 0x10000, self.limit)
            for address in range(start + 2, end, 2):
                if (
                    address in self.instructions or
                    (self.u16(address) & 0xFF00) != 0xB500
                ):
                    continue
                previous = self.u16(address - 2)
                after_padding = previous in (0, 0xFFFF)
                after_return = previous == 0x4770 or (previous & 0xFF00) == 0xBD00
                if after_padding or after_return:
                    added |= self.add_seed(
                        address, "thumb", f"prologue-boundary:{address:08x}")
        return added

    def run(self):
        entry = self.initial_seeds()
        walked = set()
        while True:
            pending = deque(sorted(set(self.functions) - walked))
            while pending:
                address = pending.popleft()
                if address in walked:
                    continue
                self.walk_function(address)
                walked.add(address)
                for discovered in sorted(set(self.functions) - walked):
                    if discovered not in pending:
                        pending.append(discovered)
            added = False
            for table in sorted(self.data_refs):
                if table & 3 or not self.inside(table, 8):
                    continue
                targets = []
                cursor = table
                while self.inside(cursor, 4) and len(targets) < 1024:
                    value = self.u32(cursor)
                    if not value & 1 or not self.inside(value & ~1, 2):
                        break
                    targets.append(value & ~1)
                    cursor += 4
                if len(targets) < 2:
                    continue
                self.pointer_tables[table] = targets
                for target in targets:
                    added |= self.add_seed(
                        target, "thumb", f"pointer-table:{table:08x}")
            added |= self.discover_global_pointer_tables()
            added |= self.discover_individual_function_pointers()
            added |= self.discover_prologue_boundaries()
            if not added:
                break
        return entry

    def report(self, entry, details=False):
        functions = []
        for address, function in sorted(self.functions.items()):
            insns = sorted(function["instructions"])
            functions.append({
                "entry": address,
                "mode": function["mode"],
                "sources": sorted(function["sources"]),
                "blocks": len(function["blocks"]),
                "instruction_count": len(insns),
                "min_address": insns[0] if insns else None,
                "max_address": (
                    insns[-1] + self.instructions[insns[-1]]["size"]
                    if insns else None
                ),
                "callees": sorted(function["callees"]),
                "unresolved": sorted(function["unresolved"]),
            })
        report = {
            "rom_base": ROM_BASE,
            "rom_size": len(self.data),
            "reset_entry": entry,
            "function_count": len(functions),
            "instruction_count": len(self.instructions),
            "call_count": len(self.calls),
            "unresolved_count": len(self.unresolved),
            "pointer_table_count": len(self.pointer_tables),
            "conflicts": self.conflicts,
            "functions": functions,
        }
        if details:
            report["instructions"] = [
                {"address": address, **instruction}
                for address, instruction in sorted(self.instructions.items())
            ]
            report["calls"] = [
                {"source": source, "target": target, "mode": mode}
                for source, target, mode in sorted(self.calls)
            ]
            report["unresolved"] = sorted(self.unresolved)
            report["data_refs"] = sorted(self.data_refs)
            report["pointer_tables"] = [
                {"address": address, "targets": targets}
                for address, targets in sorted(self.pointer_tables.items())
            ]
        return report


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path)
    parser.add_argument("-o", "--output", type=Path, required=True)
    parser.add_argument("--details", action="store_true")
    args = parser.parse_args()
    discovery = Discovery(args.rom.read_bytes())
    entry = discovery.run()
    report = discovery.report(entry, args.details)
    args.output.write_text(json.dumps(report, indent=2) + "\n")
    print(
        f"functions={report['function_count']} "
        f"instructions={report['instruction_count']} "
        f"calls={report['call_count']} "
        f"unresolved={report['unresolved_count']} "
        f"conflicts={len(report['conflicts'])}"
    )


if __name__ == "__main__":
    main()
