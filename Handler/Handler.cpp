#include "Handler.h"
#include "HandlerManager.h"

Handler::Handler(HandlerManager* parent) : _parent{parent} {}

Handler::~Handler() {}

HandlerManager* Handler::parent() {
  return _parent;
}
