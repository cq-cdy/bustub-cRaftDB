include(common.cmake)
set(SHELL_SOURCES raftDB.cpp)
add_executable(raftDB ${SHELL_SOURCES})

target_link_libraries(raftDB craft
absl::flags
absl::flags_parse
${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF}
libgo
dl
pthread
bustub bustub_linenoise argparse utf8proc)
set_target_properties(raftDB PROPERTIES OUTPUT_NAME raftDB)
