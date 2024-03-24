// https://adventofcode.com/2023/day/1

const std = @import("std");

fn maybeDigit(char: u8) ?u8 {
    if (std.ascii.isDigit(char)) {
        return char - '0';
    }
    return null;
}

test maybeDigit {
    try std.testing.expect(maybeDigit('0').? == 0);
    try std.testing.expect(maybeDigit('9').? == 9);
    try std.testing.expect(maybeDigit('1').? == 1);
}

// TODO: solve for one O(n) pass next time?
fn getTwoDigits(line: []const u8) !u8 {
    var l: usize = 0;
    var r: usize = line.len - 1;
    var result: u8 = 0;
    while (l <= line.len - 1) {
        if (maybeDigit(line[l])) |digit| {
            result = digit * 10;
            break;
        }
        l += 1;
    }
    while (r >= 0) {
        if (maybeDigit(line[r])) |digit| {
            result += digit;
            break;
        }
        r -= 1;
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
        // std.debug.print("{d}\n", .{value});
        answer += digits;
    }
    std.debug.print("Answer: {d}\n", .{answer});
}
