#pragma once

#include <string>
#include <vector>

enum class EResourceType {
    None = -1,
    Texture,
    Shader,
    Audio,
};

class Resource {

public:

    Resource();

    template<class Archive>
    void serialize(Archive& ar) {
        ar(GUID, Path);
    }

    virtual void SetPath(const std::string& path) = 0;
    EResourceType GetType() const;

    long long GUID;
    std::string Path;

    bool IsLoaded;

protected:

    EResourceType _Type = EResourceType::None;

};
