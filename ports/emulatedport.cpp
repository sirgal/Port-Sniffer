#include "ports/emulatedport.h"

// SUCH BEAUTY
EmulatedPort::EmulatedPortFeeder EmulatedPort::feeder;
std::shared_ptr<QFile> EmulatedPort::file = nullptr;
