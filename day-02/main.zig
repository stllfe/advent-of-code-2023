// https://adventofcode.com/2023/day/2
//
// basically the Elf does a bootstrap sampling...
//
// part 1: we need to figure out the possibility of a Game
// games are totally valid if at each time the maximum number of cubes of a single color
// do not exceed the total amount available!
const std = @import("std");

const NUM_R_CUBES = 12;
const NUM_G_CUBES = 13;
const NUM_B_CUBES = 14;

const Color = enum { red, green, blue };
const Turn = struct { red: u8 = 0, green: u8 = 0, blue: u8 = 0 };
const Game = struct { id: u8, turns: []const Turn };

const GameParseError = error{};

// TODO make a struct method
// extract the shifting with pos += 1 etc into a separate method call
fn parseGame(game: []const u8, allocator: std.mem.Allocator) error{ GameParseError, OutOfMemory }!Game {
    var parser = std.fmt.Parser{ .buf = game };
    // parse game id
    var label = parser.until(':');
    var id = std.fmt.parseInt(u8, label[std.mem.indexOfScalar(u8, label, ' ').? + 1 ..], 10) catch |err| {
        std.debug.print("{any}\n", .{err});
        std.debug.print("-> {s}\n", .{game});
        return error.GameParseError;
    };
    // std.debug.print("id ->{d}\n", .{id});

    // parse turns
    var turns = std.ArrayList(Turn).init(allocator);
    defer turns.deinit();

    // shift to the next cube info
    // TODO rewrite shifts using `peek()`
    parser.pos += 1;
    if (parser.char() != ' ') return error.GameParseError;

    // this gets a single turn string
    var t = parser.until(';');
    while (t.len > 0) {
        var it = std.mem.splitBackwardsAny(u8, t, " ,");
        var turn = Turn{};
        // TODO I guess iterating exactly two times per color here would be cleaner
        // or choose another splitting logic
        while (it.next()) |chunk| {
            // skip meaningless chunks
            if (std.mem.eql(u8, chunk, " ") or chunk.len == 0) continue;
            // std.debug.print("  ->{s}\n", .{chunk});
            // the weird string-matching switch stolen from
            // https://www.reddit.com/r/Zig/comments/13buv9l/extended_switch_semantics_on_stringsarrays/
            // TODO is there anything better or am I approaching this problem incorrectly altogether?
            var color = std.meta.stringToEnum(Color, chunk) orelse return error.GameParseError;
            var value = std.fmt.parseInt(u8, it.next().?, 10) catch return error.GameParseError;
            switch (color) {
                .red => {
                    turn.red = value;
                },
                .green => {
                    turn.green = value;
                },
                .blue => {
                    turn.blue = value;
                },
            }
        }
        // std.debug.print("  ->{any}\n", .{turn});
        try turns.append(turn);

        // shift to the next turn (if any)
        parser.pos += 2;
        t = parser.until(';');
    }
    return Game{
        .id = id,
        .turns = try turns.toOwnedSlice(),
    };
}

fn isPossible(game: Game) bool {
    for (game.turns) |turn| {
        if (turn.green > NUM_G_CUBES or turn.red > NUM_R_CUBES or turn.blue > NUM_B_CUBES) return false;
    }
    return true;
}

fn getFewestNumberOfCubesPossible(turns: []const Turn) Turn {
    var r: u8 = 0;
    var g: u8 = 0;
    var b: u8 = 0;
    for (turns) |turn| {
        r = @max(turn.red, r);
        g = @max(turn.green, g);
        b = @max(turn.blue, b);
    }
    return Turn{ .red = r, .green = g, .blue = b };
}

pub fn main() !void {
    const path = "day-02/input.txt";
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    // fighting with line-by-line file reading in Zig, here we go...
    var line = std.ArrayList(u8).init(allocator);
    defer line.deinit();

    var buff = std.io.bufferedReader(file.reader());
    const reader = buff.reader();
    const writer = line.writer();

    var part_1_answer: u32 = 0;
    var part_2_answer: u32 = 0;

    while (reader.streamUntilDelimiter(writer, '\n', null)) {
        defer line.clearRetainingCapacity();
        // std.debug.print("{s}\n", .{line.items});
        var game = try parseGame(line.items, allocator);
        // std.debug.print("->{any}\n\n", .{game});
        if (isPossible(game)) part_1_answer += game.id;
        var turn = getFewestNumberOfCubesPossible(game.turns);
        // compute the power of the set of cubes
        // TODO: maybe we could just use u32 everywhere to avoid this conversion?
        part_2_answer += @as(u32, turn.red) * @as(u32, turn.green) * @as(u32, turn.blue);
    } else |err| switch (err) {
        error.EndOfStream => {}, // we just finished reading, no worries
        else => return err, // propogate error
    }
    std.debug.print("(P1) Answer: {d}\n", .{part_1_answer});
    std.debug.print("(P2) Answer: {d}\n", .{part_2_answer});
}
