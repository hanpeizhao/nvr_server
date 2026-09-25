#ifndef _INCLUDE_CORE_THREADMGR_HPP_
#define _INCLUDE_CORE_THREADMGR_HPP_

#include <pthread.h>

namespace core {
namespace threadmgr {

/* Start a detached thread.  All handler threads in ztnvr run detached and
 * are never joined; this helper keeps that behaviour in one place.
 * Returns the pthread_create result code (0 on success). */
inline int spawn_detached(pthread_t &tid, void *(*handler)(void *),
                          void *arg) {
  pthread_attr_t attr;
  int retcd;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  retcd = pthread_create(&tid, &attr, handler, arg);
  pthread_attr_destroy(&attr);

  return retcd;
}

} /* namespace threadmgr */
} /* namespace core */

#endif /* _INCLUDE_CORE_THREADMGR_HPP_ */
