#pragma  once
#include <utility>

#include "grpc++/grpc++.h"
#include "memory"
#include "public.h"
#include "../rpc/raft.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class RpcClients {
   public:
    static RpcClients* getInstance(std::vector<std::string> peersAddr) {
        static RpcClients peerClients(std::move(peersAddr));
        return &peerClients;
    }

    std::vector<std::unique_ptr<RaftRPC::Stub>>& getPeerStubs(){
        return m_peerStubs;
    }


    int numPeers() const{
        return m_num_peers_;
    }

   private:
    explicit RpcClients(std::vector<std::string> peersAddr) {

        m_peerAddr = std::move(peersAddr);
        m_num_peers_ = m_peerAddr.size();
        for (int i = 0; i < m_peerAddr.size(); i++) {
            spdlog::info("server[{}] addr:{}",i, m_peerAddr[i]);
            auto channel = grpc::CreateChannel(
                m_peerAddr[i], grpc::InsecureChannelCredentials());
            m_peerStubs.emplace_back(RaftRPC::NewStub(channel));
            m_peerChannels.emplace_back(grpc::CreateChannel(m_peerAddr[i], grpc::InsecureChannelCredentials()));
        }
    }

   private:
    int m_num_peers_{};
     std::vector<std::unique_ptr<RaftRPC::Stub>> m_peerStubs;
     std::vector<std::shared_ptr<::grpc::Channel>> m_peerChannels;
     std::vector<std::string> m_peerAddr ;
    ClientContext m_clientContext;
};