#ifndef _INCLUDE_CORE_EVENTBUS_HPP_
#define _INCLUDE_CORE_EVENTBUS_HPP_

#include <atomic>

namespace core {

/* Minimal synchronous event bus.
 *
 * Events are identified by a small integer id.  publish() is lock-free and
 * async-signal-safe so it may be called from a POSIX signal handler.  The
 * pending events are dispatched (each subscriber invoked on the calling
 * thread) only when the main loop calls drain(), never in signal context.
 *
 * Intended usage is a single producer (the signal handler) and a single
 * consumer (the main loop).  Subscribers must be registered before events
 * are published; subscribing to an event id that already has a handler
 * overwrites it, which makes re-initialization idempotent.
 */
class EventBus {
public:
  using Handler = void (*)(void *user);

  static const int MAX_EVENTS = 32;

  bool subscribe(int ev, Handler fn, void *user) {
    if ((ev < 0) || (ev >= MAX_EVENTS)) {
      return false;
    }
    subs[ev].fn = fn;
    subs[ev].user = user;
    return true;
  }

  /* Async-signal-safe: mark the event as pending for the next drain(). */
  void publish(int ev) {
    if ((ev >= 0) && (ev < MAX_EVENTS)) {
      pending.fetch_or(1u << ev);
    }
  }

  /* Invoke the handlers of all pending events and clear the pending set. */
  void drain() {
    unsigned pend = pending.exchange(0);
    for (int ev = 0; (pend != 0) && (ev < MAX_EVENTS); ev++) {
      if ((pend & (1u << ev)) != 0) {
        pend &= ~(1u << ev);
        if (subs[ev].fn != nullptr) {
          subs[ev].fn(subs[ev].user);
        }
      }
    }
  }

private:
  struct Sub {
    Handler fn;
    void *user;
  };

  Sub subs[MAX_EVENTS] = {};
  std::atomic<unsigned> pending{0};

  static_assert(std::atomic<unsigned>::is_always_lock_free,
                "EventBus::publish must be lock-free");
};

} /* namespace core */

#endif /* _INCLUDE_CORE_EVENTBUS_HPP_ */
