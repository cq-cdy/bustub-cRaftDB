#pragma  once

#include "public.h"
#include "nocopyable.h"
#include "libgo/coroutine.h"
#include "grpc++/grpc++.h"
#include  "peers.h"
#include "utils/timer.h"
#include "startRpcService.h"
#include "persist/abstractPersist.h"

namespace craft {

    class Raft final : public noncopyable {

    public:
        explicit Raft(AbstractPersist *persister, co_chan<ApplyMsg> *applyCh) ;

        void launch() ;

        ~Raft();

    private:
        void co_launchRpcSevices();

        void co_startElection();

        void co_applyLogs();

        void co_appendAentries();

    public:

        void persist();
        bool saveSnapShot(int index);
        void loadFromPersist();

        void changeToState(STATE toState) ;
        ServerCallResult submitCommand(std::string command );
        void initFromConfig(const std::string& filename) ;
        void setClusterAddress(const std::vector<std::string> &clusterAddress) ;
        void setLeaderEelectionTimeOut(uint millisecond) ;
        void setRpcTimeOut(uint millisecond) ;
        void setHeatBeatTimeOut(uint millisecond) ;
        void setLogLevel(spdlog::level::level_enum loglevel) ;

        //some util function

        int getLastLogIndex() const ;

        int getLastLogTerm() const ;
        
        // use to debug
        static std::string stringState(STATE state);

    public:
        //some util function
        bool isOutOfArgsAppendEntries(const ::AppendEntriesArgs *args) const;
        int getStoreIndexByLogIndex(int logIndex);
        void tryCommitLog();

        std::tuple<int,int,std::vector<LogEntry>> getAppendLogs(int peerId);

    public:

        int m_me_{-1};
        int m_current_term_ = 0;
        int m_votedFor_ = -1;
        int m_commitIndex_ = 0;
        int m_lastApplied_ = 0;
        int m_snapShotIndex = 0;
        int m_snapShotTerm = 0;
        bool m_iskilled_ = false;
        std::vector<std::string> m_clusterAddress_;
        uint m_leaderEelectionTimeOut_ = 800;
        uint m_rpcTimeOut_ = 100;
        uint m_heatBeatInterVal = 200;
    public:

        std::vector<LogEntry> m_logs_;
        co_mutex co_mtx_;
        co_chan<ApplyMsg> *m_applyCh_ = nullptr;
        co_chan<void *> *m_notifyApplyCh_ = nullptr;
        co_chan<RETURN_TYPE> *m_StateChangedCh_ = nullptr;

        co_chan<void *> *m_stopCh_ = nullptr;

        RpcClients *m_peers_ = nullptr;
        AbstractPersist *m_persister_ = nullptr;
        STATE m_state_ = STATE::FOLLOWER;
        std::vector<int> m_nextIndex_;
        std::vector<int> m_matchIndex_;

        Timer *m_electionTimer = nullptr;
        Timer *m_applyTimer = nullptr;
        Timer *m_appendEntriesTimer = nullptr;
        std::vector<Timer *> m_appendEntriesTimers_{nullptr};
        co_chan<RETURN_TYPE>* isCompleteSnapFileInstallCh_;


    };
}
