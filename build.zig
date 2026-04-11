const std = @import("std");
const zcc = @import("compile_commands");

pub fn build(bld: *std.Build) void {
    const target   = bld.standardTargetOptions(.{});
    const optimize = bld.standardOptimizeOption(.{});

    const exe_mod = bld.createModule(.{
        .target    = target,
        .optimize  = optimize,
        .link_libcpp = true,
    });

    const exe = bld.addExecutable(.{
        .name        = "raytracer",
        .root_module = exe_mod,
    });

    exe_mod.addCSourceFiles(.{
        .files = &.{"src/main.cpp"},
        .flags = &.{"-std=c++17"},
    });

    exe_mod.addIncludePath(bld.path("include"));

    // ── System libraries ──────────────────────────────────────────────────────
    // Uncomment as needed when adding raylib or SDL.
    //
    // Windows:
    //   exe_mod.linkSystemLibrary("winmm",   .{});
    //   exe_mod.linkSystemLibrary("gdi32",   .{});
    //   exe_mod.linkSystemLibrary("opengl32",.{});
    //
    // Linux:
    //   exe_mod.linkSystemLibrary("GL",      .{});
    //   exe_mod.linkSystemLibrary("m",       .{});
    //   exe_mod.linkSystemLibrary("pthread", .{});
    //   exe_mod.linkSystemLibrary("dl",      .{});
    //   exe_mod.linkSystemLibrary("rt",      .{});
    //   exe_mod.linkSystemLibrary("X11",     .{});
    //
    // macOS:
    //   exe_mod.linkFramework("OpenGL",         .{});
    //   exe_mod.linkFramework("Cocoa",          .{});
    //   exe_mod.linkFramework("IOKit",          .{});
    //   exe_mod.linkFramework("CoreFoundation", .{});
    //   exe_mod.linkFramework("CoreAudio",      .{});
    //   exe_mod.linkFramework("AudioToolbox",   .{});

    bld.installArtifact(exe);

    // ── Compile commands DB ───────────────────────────────────────────────────
    var targets = std.ArrayListUnmanaged(*std.Build.Step.Compile){};
    targets.append(bld.allocator, exe) catch @panic("OOM");

    const cdb_step = zcc.createStep(bld, "cdb", targets.toOwnedSlice(bld.allocator) catch @panic("OOM"));
    cdb_step.dependOn(bld.getInstallStep());

    // ── Run step ──────────────────────────────────────────────────────────────
    const run_cmd = bld.addRunArtifact(exe);
    run_cmd.step.dependOn(bld.getInstallStep());
    if (bld.args) |args| run_cmd.addArgs(args);

    const run_step = bld.step("run", "Run the raytracer");
    run_step.dependOn(&run_cmd.step);
}
