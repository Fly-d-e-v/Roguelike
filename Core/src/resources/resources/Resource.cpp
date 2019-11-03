#include "Resource.h"

Resource::Resource() {
    IsLoaded = false;
}

EResourceType Resource::GetType() const {
    return _Type;
}
