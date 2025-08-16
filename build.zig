const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // Build options for graphics backend selection
    const graphics_backend = b.option(
        enum { raylib, sdl3 },
        "graphics",
        "Graphics backend to use (raylib or sdl3)",
    ) orelse .sdl3;

    const exe = b.addExecutable(.{
        .name = "infinite-runner",
        .target = target,
        .optimize = optimize,
    });

    // Add C source files
    exe.addCSourceFiles(.{
        .files = &.{
            "src/main.c",
            "src/engine/graphics.c",
        },
        .flags = &.{ "-std=c99", "-Wall", "-Wextra" },
    });

    // Add platform-specific backend
    switch (graphics_backend) {
        .raylib => {
            exe.addCSourceFile(.{ .file = b.path("src/platform/raylib_impl.c") });
            exe.root_module.addCMacro("GRAPHICS_BACKEND_RAYLIB", "1");
            exe.linkSystemLibrary("raylib");
        },
        .sdl3 => {
            exe.addCSourceFile(.{ .file = b.path("src/platform/sdl3_impl.c") });
            exe.root_module.addCMacro("GRAPHICS_BACKEND_SDL3", "1");
            exe.linkSystemLibrary("SDL3");
        },
    }
    exe.linkLibC();

    b.installArtifact(exe);

    // SDL3 WebAssembly build using Emscripten
    const wasm_step = b.step("wasm", "Build SDL3 WebAssembly version using Emscripten");
    const emcc_cmd = b.addSystemCommand(&.{
        "emcc",
        "src/main.c",
        "src/engine/graphics.c",
        "src/platform/sdl3_impl.c",
        "-DGRAPHICS_BACKEND_SDL3=1",
        "-D__EMSCRIPTEN__=1",
        "-sUSE_SDL=3",
        "-sASSERTIONS=1",
        "-sWASM=1",
        "-sASYNCIFY",
        "-sEXPORTED_FUNCTIONS=[\"_main\"]",
        "-sEXPORTED_RUNTIME_METHODS=[\"ccall\",\"cwrap\"]",
        "-o",
        "web/game.js",
    });
    wasm_step.dependOn(&emcc_cmd.step);

    // Run command
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the game");
    run_step.dependOn(&run_cmd.step);
}
