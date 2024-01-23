#pragma once

#include "grpc++/grpc++.h"
#include "raft.h"
#include "../rpc/raft.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

namespace craft {
    class Raft;

    class RpcServiceImpl : public RaftRPC::Service {
    public:
        RpcServiceImpl() = delete;

        explicit RpcServiceImpl(Raft *raft);

        /* RPC defination*/


        Status requestVoteRPC(::grpc::ServerContext *context,
                              const ::RequestVoteArgs *request,
                              ::RequestVoteReply *response) override;

        Status appendEntries(::grpc::ServerContext *context,
                             const ::AppendEntriesArgs *request,
                             ::AppendEntriesReply *response) override;

        Status submitCommand(::grpc::ServerContext *context,
                             const ::Command *request,
                             ::ResultPackge *response) override;

        Status installSnapshot(::grpc::ServerContext *context, const ::InstallSnapshotArgs *request,
                               ::InstallSnapshotReply *response) override;

        Status
        TransferSnapShotFile(::grpc::ServerContext *context, ::grpc::ServerReader<::TransferSnapShotFileArgs> *reader,
                             ::TransferSnapShotFileReply *response) override;


        void publishRpcService();

        /*add some rpc functions*/
    private:
        std::string m_addr_;
        Raft *m_rf_{};
    };

}  // namespace craft