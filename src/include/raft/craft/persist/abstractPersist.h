#pragma once

#include <fstream>

#include "map"
#include "../public.h"
#include <cstdio>
#include "string"
#include "vector"

namespace craft {

    class AbstractPersist {
    public:
        AbstractPersist() = delete;

        AbstractPersist(std::string absPersistPath,std::string snapshotFileName_);

        virtual void deserialization(const char *filename) = 0;

        virtual void serialization() = 0;

        virtual int getLastLogIndex() const final;

        virtual int getVotedFor() const final;

        virtual int getCommitIndex() const final;

        virtual int getCurrentTerm() const final;

        virtual int getLastSnapshotTerm() const final;
        virtual int getLastSnapshotIndex() const final;


        virtual std::vector<std::pair<int, std::string>> getLogEntries() final;

        virtual ~AbstractPersist();

        std::string absPersistPath_;
        std::string snapshotFileName_;
    protected:

        int lastlogindex_ = 0;
        int votedFor_ = -1;
        int commitIndex_ = 0;
        int currentTerm_ = 1;
        int lastSnapshotTerm_ = 0;
        int lastSnapshotIndex_ = 0;

        // term and command
        std::vector<std::pair<int, std::string>> logEntry_;


    private:
        std::vector<std::string> readLines(const std::string &filename);  //{
    };
}  // namespace craft

