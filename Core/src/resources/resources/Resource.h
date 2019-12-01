#pragma once

#include "core/serialization/SerializationHelpers.h"

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

    SERIALIZE_CLASS_BODY_TWO(GUID, Path)

    virtual void SetPath(const std::string& path) = 0;
    EResourceType GetType() const;

    virtual void ImGuiDisplay();
    virtual void ImGuiNewDisplay();

    long long GUID;
    std::string Path;

    bool IsLoaded;

protected:

    EResourceType _Type = EResourceType::None;

};

