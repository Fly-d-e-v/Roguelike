#pragma once

#include "Resource.h"

class Shader : public Resource {
public:
    
    SERIALIZE_CLASS_BODY_FOUR(GUID, Path, VertexPath, FragmentPath)

    Shader();

    void SetPath(const std::string& path);
    virtual void ImGuiDisplay() override;

	//Program ID
	unsigned int ID = 0;

    std::string VertexPath;
    std::string FragmentPath;

	void Use();

};