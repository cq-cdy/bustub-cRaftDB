// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: raft.proto

#include "raft.pb.h"
#include "raft.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>

static const char* RaftRPC_method_names[] = {
  "/RaftRPC/installSnapshot",
  "/RaftRPC/TransferSnapShotFile",
  "/RaftRPC/submitCommand",
  "/RaftRPC/requestVoteRPC",
  "/RaftRPC/appendEntries",
};

std::unique_ptr< RaftRPC::Stub> RaftRPC::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< RaftRPC::Stub> stub(new RaftRPC::Stub(channel, options));
  return stub;
}

RaftRPC::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_installSnapshot_(RaftRPC_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_TransferSnapShotFile_(RaftRPC_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::CLIENT_STREAMING, channel)
  , rpcmethod_submitCommand_(RaftRPC_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_requestVoteRPC_(RaftRPC_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_appendEntries_(RaftRPC_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status RaftRPC::Stub::installSnapshot(::grpc::ClientContext* context, const ::InstallSnapshotArgs& request, ::InstallSnapshotReply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::InstallSnapshotArgs, ::InstallSnapshotReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_installSnapshot_, context, request, response);
}

void RaftRPC::Stub::async::installSnapshot(::grpc::ClientContext* context, const ::InstallSnapshotArgs* request, ::InstallSnapshotReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::InstallSnapshotArgs, ::InstallSnapshotReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_installSnapshot_, context, request, response, std::move(f));
}

void RaftRPC::Stub::async::installSnapshot(::grpc::ClientContext* context, const ::InstallSnapshotArgs* request, ::InstallSnapshotReply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_installSnapshot_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::InstallSnapshotReply>* RaftRPC::Stub::PrepareAsyncinstallSnapshotRaw(::grpc::ClientContext* context, const ::InstallSnapshotArgs& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::InstallSnapshotReply, ::InstallSnapshotArgs, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_installSnapshot_, context, request);
}

::grpc::ClientAsyncResponseReader< ::InstallSnapshotReply>* RaftRPC::Stub::AsyncinstallSnapshotRaw(::grpc::ClientContext* context, const ::InstallSnapshotArgs& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncinstallSnapshotRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::ClientWriter< ::TransferSnapShotFileArgs>* RaftRPC::Stub::TransferSnapShotFileRaw(::grpc::ClientContext* context, ::TransferSnapShotFileReply* response) {
  return ::grpc::internal::ClientWriterFactory< ::TransferSnapShotFileArgs>::Create(channel_.get(), rpcmethod_TransferSnapShotFile_, context, response);
}

void RaftRPC::Stub::async::TransferSnapShotFile(::grpc::ClientContext* context, ::TransferSnapShotFileReply* response, ::grpc::ClientWriteReactor< ::TransferSnapShotFileArgs>* reactor) {
  ::grpc::internal::ClientCallbackWriterFactory< ::TransferSnapShotFileArgs>::Create(stub_->channel_.get(), stub_->rpcmethod_TransferSnapShotFile_, context, response, reactor);
}

::grpc::ClientAsyncWriter< ::TransferSnapShotFileArgs>* RaftRPC::Stub::AsyncTransferSnapShotFileRaw(::grpc::ClientContext* context, ::TransferSnapShotFileReply* response, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncWriterFactory< ::TransferSnapShotFileArgs>::Create(channel_.get(), cq, rpcmethod_TransferSnapShotFile_, context, response, true, tag);
}

::grpc::ClientAsyncWriter< ::TransferSnapShotFileArgs>* RaftRPC::Stub::PrepareAsyncTransferSnapShotFileRaw(::grpc::ClientContext* context, ::TransferSnapShotFileReply* response, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncWriterFactory< ::TransferSnapShotFileArgs>::Create(channel_.get(), cq, rpcmethod_TransferSnapShotFile_, context, response, false, nullptr);
}

::grpc::Status RaftRPC::Stub::submitCommand(::grpc::ClientContext* context, const ::Command& request, ::ResultPackge* response) {
  return ::grpc::internal::BlockingUnaryCall< ::Command, ::ResultPackge, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_submitCommand_, context, request, response);
}

void RaftRPC::Stub::async::submitCommand(::grpc::ClientContext* context, const ::Command* request, ::ResultPackge* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::Command, ::ResultPackge, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_submitCommand_, context, request, response, std::move(f));
}

void RaftRPC::Stub::async::submitCommand(::grpc::ClientContext* context, const ::Command* request, ::ResultPackge* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_submitCommand_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::ResultPackge>* RaftRPC::Stub::PrepareAsyncsubmitCommandRaw(::grpc::ClientContext* context, const ::Command& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::ResultPackge, ::Command, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_submitCommand_, context, request);
}

::grpc::ClientAsyncResponseReader< ::ResultPackge>* RaftRPC::Stub::AsyncsubmitCommandRaw(::grpc::ClientContext* context, const ::Command& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncsubmitCommandRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status RaftRPC::Stub::requestVoteRPC(::grpc::ClientContext* context, const ::RequestVoteArgs& request, ::RequestVoteReply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::RequestVoteArgs, ::RequestVoteReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_requestVoteRPC_, context, request, response);
}

void RaftRPC::Stub::async::requestVoteRPC(::grpc::ClientContext* context, const ::RequestVoteArgs* request, ::RequestVoteReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::RequestVoteArgs, ::RequestVoteReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_requestVoteRPC_, context, request, response, std::move(f));
}

void RaftRPC::Stub::async::requestVoteRPC(::grpc::ClientContext* context, const ::RequestVoteArgs* request, ::RequestVoteReply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_requestVoteRPC_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::RequestVoteReply>* RaftRPC::Stub::PrepareAsyncrequestVoteRPCRaw(::grpc::ClientContext* context, const ::RequestVoteArgs& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::RequestVoteReply, ::RequestVoteArgs, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_requestVoteRPC_, context, request);
}

::grpc::ClientAsyncResponseReader< ::RequestVoteReply>* RaftRPC::Stub::AsyncrequestVoteRPCRaw(::grpc::ClientContext* context, const ::RequestVoteArgs& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncrequestVoteRPCRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status RaftRPC::Stub::appendEntries(::grpc::ClientContext* context, const ::AppendEntriesArgs& request, ::AppendEntriesReply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::AppendEntriesArgs, ::AppendEntriesReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_appendEntries_, context, request, response);
}

void RaftRPC::Stub::async::appendEntries(::grpc::ClientContext* context, const ::AppendEntriesArgs* request, ::AppendEntriesReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::AppendEntriesArgs, ::AppendEntriesReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_appendEntries_, context, request, response, std::move(f));
}

void RaftRPC::Stub::async::appendEntries(::grpc::ClientContext* context, const ::AppendEntriesArgs* request, ::AppendEntriesReply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_appendEntries_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::AppendEntriesReply>* RaftRPC::Stub::PrepareAsyncappendEntriesRaw(::grpc::ClientContext* context, const ::AppendEntriesArgs& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::AppendEntriesReply, ::AppendEntriesArgs, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_appendEntries_, context, request);
}

::grpc::ClientAsyncResponseReader< ::AppendEntriesReply>* RaftRPC::Stub::AsyncappendEntriesRaw(::grpc::ClientContext* context, const ::AppendEntriesArgs& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncappendEntriesRaw(context, request, cq);
  result->StartCall();
  return result;
}

RaftRPC::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftRPC_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaftRPC::Service, ::InstallSnapshotArgs, ::InstallSnapshotReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RaftRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::InstallSnapshotArgs* req,
             ::InstallSnapshotReply* resp) {
               return service->installSnapshot(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftRPC_method_names[1],
      ::grpc::internal::RpcMethod::CLIENT_STREAMING,
      new ::grpc::internal::ClientStreamingHandler< RaftRPC::Service, ::TransferSnapShotFileArgs, ::TransferSnapShotFileReply>(
          [](RaftRPC::Service* service,
             ::grpc::ServerContext* ctx,
             ::grpc::ServerReader<::TransferSnapShotFileArgs>* reader,
             ::TransferSnapShotFileReply* resp) {
               return service->TransferSnapShotFile(ctx, reader, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftRPC_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaftRPC::Service, ::Command, ::ResultPackge, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RaftRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::Command* req,
             ::ResultPackge* resp) {
               return service->submitCommand(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftRPC_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaftRPC::Service, ::RequestVoteArgs, ::RequestVoteReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RaftRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::RequestVoteArgs* req,
             ::RequestVoteReply* resp) {
               return service->requestVoteRPC(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftRPC_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaftRPC::Service, ::AppendEntriesArgs, ::AppendEntriesReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RaftRPC::Service* service,
             ::grpc::ServerContext* ctx,
             const ::AppendEntriesArgs* req,
             ::AppendEntriesReply* resp) {
               return service->appendEntries(ctx, req, resp);
             }, this)));
}

RaftRPC::Service::~Service() {
}

::grpc::Status RaftRPC::Service::installSnapshot(::grpc::ServerContext* context, const ::InstallSnapshotArgs* request, ::InstallSnapshotReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaftRPC::Service::TransferSnapShotFile(::grpc::ServerContext* context, ::grpc::ServerReader< ::TransferSnapShotFileArgs>* reader, ::TransferSnapShotFileReply* response) {
  (void) context;
  (void) reader;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaftRPC::Service::submitCommand(::grpc::ServerContext* context, const ::Command* request, ::ResultPackge* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaftRPC::Service::requestVoteRPC(::grpc::ServerContext* context, const ::RequestVoteArgs* request, ::RequestVoteReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaftRPC::Service::appendEntries(::grpc::ServerContext* context, const ::AppendEntriesArgs* request, ::AppendEntriesReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

