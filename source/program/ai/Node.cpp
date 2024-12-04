#include "Node.h"
#include "AIFactoryMgr.h"

namespace ai {

template <>
void Output<const char*>::setOutput(const char* value)
{
    if (isTaggedToPulseTLS() && getPtr() != nullptr && *getPtr() != value)
        AIFactoryMgr::instance()->tryPulseTLS();

    *getPtr() = value;
}

} // namespace ai