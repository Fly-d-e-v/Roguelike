#pragma once

#include <string>
#include <vector>

class Resource {

public:

    template<class Archive>
    void serialize(Archive& ar) {
        ar(GUID, Path);
    }

    virtual void SetPath(const std::string& path) = 0;

    long long GUID;
    std::string Path;

    bool IsLoaded;

};
