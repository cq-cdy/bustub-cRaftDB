#pragma once

#include <grpc++/grpc++.h>
#include "craft/peers.h"
#include "craft/utils/commonUtil.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using namespace std::chrono;

class CRaftClient {
public:
    CRaftClient() {
        ConfigReader reader((std::string(getenv("HOME")) + "/craft/craft.conf"));
        auto configMap = reader.getMap();
        auto range = configMap.equal_range("servers");
        if (range.first != range.second) {
            std::vector<std::string> servers;
            for (auto it = range.first; it != range.second; it++) {
                m_peersAddr.push_back(it->second);
            }
        }
        if(m_peersAddr.empty()){
            spdlog::error("not fount any addr from ~/craft/craft.conf");
            exit(1);
        }
        for(auto addr:m_peersAddr){
            if(!isValidIpPort(addr)){
                spdlog::error("addr error:{}",addr);
                exit(1);
            }
        }
    }
    ClientResult submitCommand(std::string command ,uint timeout  = 200 /*ms*/){
        int trt_count = 0;
        for(int i = m_leaderId ; ;(++i)%=m_peersAddr.size()){
            auto channel = grpc::CreateChannel(m_peersAddr[i],
                                               grpc::InsecureChannelCredentials());
            auto stub = RaftRPC::NewStub(channel);
            Command args;
            ::ResultPackge reply;
            args.set_content(command);
            ClientContext context;
            std::chrono::system_clock::time_point deadline_ =
                    std::chrono::system_clock::now() +
                    std::chrono::milliseconds(timeout);
            context.set_deadline(deadline_);
            spdlog::debug("client to [{}]:{}",i,m_peersAddr[i]);
            Status ok = stub->submitCommand(&context, args, &reply);

            if (ok.ok()) {
                if (reply.isleader()) {
                    m_leaderId = i;
                    return {reply.term(),reply.index(),false};
                }else{
                    trt_count ++ ;
                    continue;
                }
            }
            trt_count ++ ;
            if(trt_count >= m_peersAddr.size()){
                return {-1,-1,true};
            }
        }
    }


private:
    std::vector<std::string> m_peersAddr;
    int m_leaderId{};
};
