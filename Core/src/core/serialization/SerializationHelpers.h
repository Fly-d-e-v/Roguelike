#pragma once

#define SERIALIZE_CLASS_BODY_ONE(x)     \
template<class Archive>             \
void serialize(Archive& archive){   \
    archive(x);                     \
};

#define SERIALIZE_CLASS_BODY_TWO(x,y)   \
template<class Archive>             \
void serialize(Archive& archive){   \
    archive(x,y);                   \
};

#define SERIALIZE_CLASS_BODY_THREE(x,y,z) \
template<class Archive>             \
void serialize(Archive& archive){   \
    archive(x,y,z);                 \
};

#define SERIALIZE_CLASS_BODY_FOUR(x,y,z,q) \
template<class Archive>               \
void serialize(Archive& archive){     \
    archive(x,y,z,q);                 \
};

#define SERIALIZE_CLASS_BODY_FIVE(x,y,z,q,r) \
template<class Archive>                 \
void serialize(Archive& archive){       \
    archive(x,y,z,q,r);                 \
};