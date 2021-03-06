#ifndef __PROCESS_TIMER_HPP__
#define __PROCESS_TIMER_HPP__

#include <stdint.h>
#include <stdlib.h> // For abort.

#include <process/timeout.hpp>

#include <stout/duration.hpp>
#include <stout/lambda.hpp>

namespace process {

// Timer represents a delayed thunk, that can get created (scheduled)
// and canceled using the Clock.

class Timer
{
public:
  Timer() : id(0), pid(process::UPID()), thunk(&abort) {}

  bool operator == (const Timer& that) const
  {
    return id == that.id;
  }

  // Invokes this timer's thunk.
  void operator () () const
  {
    thunk();
  }

  // Returns the timeout associated with this timer.
  Timeout timeout() const
  {
    return t;
  }

  // Returns the PID of the running process when this timer was
  // created (via timers::create) or an empty PID if no process was
  // running when this timer was created.
  process::UPID creator() const
  {
    return pid;
  }

private:
  friend class Clock;

  Timer(long _id,
        const Timeout& _t,
        const process::UPID& _pid,
        const lambda::function<void(void)>& _thunk)
    : id(_id), t(_t), pid(_pid), thunk(_thunk)
  {}

  uint64_t id; // Used for equality.

  Timeout t;

  // We store the PID of the "issuing" (i.e., "running") process (if
  // there is one). We don't store a pointer to the process because we
  // can't dereference it since it might no longer be valid. (Instead,
  // the PID can be used internally to check if the process is still
  // valid and get a refernce to it.)
  process::UPID pid;

  lambda::function<void(void)> thunk;
};

} // namespace process {

#endif // __PROCESS_TIMER_HPP__
