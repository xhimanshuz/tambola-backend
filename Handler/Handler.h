#ifndef HANDLER_H
#define HANDLER_H

class HandlerManager;

class Handler {
  HandlerManager* _parent;

 public:
  explicit Handler(HandlerManager* parent);
  virtual ~Handler();
  HandlerManager* parent();
};

#endif  // HANDLER_H
