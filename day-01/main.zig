// https://adventofcode.com/2023/day/1

const std = @import("std");
const words = std.ComptimeStringMap(u8, .{ .{ "one", 1 }, .{ "two", 2 }, .{ "three", 3 }, .{ "four", 4 }, .{ "five", 5 }, .{ "six", 6 }, .{ "seven", 7 }, .{ "eight", 8 }, .{ "nine", 9 } });

fn maybeWord(chars: []const u8) ?u8 {
    for (words.kvs) |kv| {
        if (std.mem.indexOf(u8, chars, kv.key) != null) return words.get(kv.key);
    }
    return null;
}

fn maybeDigit(char: u8) ?u8 {
    if (std.ascii.isDigit(char)) {
        return char - '0';
    }
    return null;
}

test maybeDigit {
    try std.testing.expectEqual(maybeDigit('0').?, 0);
    try std.testing.expectEqual(maybeDigit('1').?, 1);
    try std.testing.expectEqual(maybeDigit('2').?, 2);
    try std.testing.expectEqual(maybeDigit('3').?, 3);
    try std.testing.expectEqual(maybeDigit('4').?, 4);
    try std.testing.expectEqual(maybeDigit('5').?, 5);
    try std.testing.expectEqual(maybeDigit('6').?, 6);
    try std.testing.expectEqual(maybeDigit('7').?, 7);
    try std.testing.expectEqual(maybeDigit('8').?, 8);
    try std.testing.expectEqual(maybeDigit('9').?, 9);
    try std.testing.expectEqual(maybeDigit('o'), null);
}

// TODO: get better time complexity?
fn getTwoDigits(line: []const u8) !u8 {
    var l: usize = 0;
    var r: usize = line.len - 1;
    var result: u8 = 0;
    while (l <= line.len - 1) {
        if (maybeWord(line[0..l]) orelse maybeDigit(line[l])) |digit| {
            result = digit * 10;
            break;
        }
        l += 1;
    }
    while (r >= 0) {
        if (maybeWord(line[r..]) orelse maybeDigit(line[r])) |digit| {
            result += digit;
            break;
        }
        r -%= 1;
    }
    return result;
}

pub fn main() !void {
    const path = "day-01/input.txt";
    var file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    const alloc = arena.allocator();

    // TODO: how to read the file line by line and process it simultaneously?
    var input = try file.readToEndAlloc(alloc, 1024 * 1024);
    var answer: u64 = 0;
    var lines = std.mem.split(u8, input, "\n");
    while (lines.next()) |line| {
        if (line.len == 0) continue;
        var digits = try getTwoDigits(line);
        // std.debug.print("{s}\n -> {d}\n", .{ line, digits });
        answer += digits;
    }
    std.debug.print("Answer: {d}\n", .{answer});
}
