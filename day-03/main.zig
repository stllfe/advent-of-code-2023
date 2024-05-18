// https://adventofcode.com/2023/day/3

const std = @import("std");

fn main() void {
    const path = "day-03/input.txt";
    const file = std.fs.cwd().openFile(path, .{});
    defer file.close();
}
