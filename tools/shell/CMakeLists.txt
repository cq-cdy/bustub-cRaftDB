include(common.cmake)
set(SHELL_SOURCES shell.cpp)
add_executable(shell ${SHELL_SOURCES})

target_link_libraries(shell craft
absl::flags
absl::flags_parse
${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF}
libgo
dl
pthread
libgo 
bustub bustub_linenoise argparse utf8proc)
set_target_properties(shell PROPERTIES OUTPUT_NAME bustub-shell)
