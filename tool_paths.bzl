load("@bazel_tools//tools/cpp:toolchain_utils.bzl", "find_cpp_toolchain")

def _find_llvm_cov_aspect_impl(_target, ctx):
    cc_toolchain = find_cpp_toolchain(ctx)
    feature_configuration = cc_common.configure_features(
        ctx = ctx,
        cc_toolchain = cc_toolchain,
        requested_features = ctx.features,
        unsupported_features = ctx.disabled_features,
    )
    tool = cc_common.get_tool_for_action(
        feature_configuration=feature_configuration,
        action_name="llvm-cov",
    )
    llvm_cov_file = ctx.actions.declare_file("cc_toolchain_lcov_tool")
    ctx.actions.write(llvm_cov_file, tool)
    return [
        OutputGroupInfo(tool_paths = depset([llvm_cov_file])),
    ]

find_llvm_cov = aspect(
    implementation = _find_llvm_cov_aspect_impl,
    attr_aspects = [],
    required_providers = [CcInfo],
    attrs = {
        "_cc_toolchain": attr.label(
            default = Label("@bazel_tools//tools/cpp:current_cc_toolchain"),
        ),
    },
    fragments = ["cpp"],
    toolchains = ["@bazel_tools//tools/cpp:toolchain_type"],
)
