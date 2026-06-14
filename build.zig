const std = @import("std");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const spng_setup = b.addSystemCommand(&(.{"cmake"} ++ .{
        "-S", "external/libspng/",
        "-B", "zig-out/cmake-build",
    }));

    const spng_build = b.addSystemCommand(&.{ "cmake", "--build", "zig-out/cmake-build" });
    spng_build.step.dependOn(&spng_setup.step);

    const shadowth_exe = b.addExecutable(.{
        .name = "shadowth",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        }),
    });

    std.Io.Dir.cwd().access(b.graph.io, "zig-out/cmake-build/libspng_static.a", .{}) catch |err| {
        if (err != error.FileNotFound) return err;
        shadowth_exe.step.dependOn(&spng_build.step);
    };

    shadowth_exe.root_module.addCSourceFiles(.{
        .language = .c,
        .files = &.{
            "src/main.c",
            "src/render/render.c",
            "src/render/block.c",
            "src/render/tri/fill.c",
            "src/render/type/type.c",
        },
    });

    shadowth_exe.root_module.addLibraryPath(b.path("zig-out/cmake-build/"));

    shadowth_exe.root_module.addIncludePath(b.path("external/libspng/spng/"));
    shadowth_exe.root_module.linkSystemLibrary("spng_static", .{});
    shadowth_exe.root_module.linkSystemLibrary("z", .{});

    b.installArtifact(shadowth_exe);
}
