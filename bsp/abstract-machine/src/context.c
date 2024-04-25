#include <am.h>
#include <klib.h>
#include <rtthread.h>

static Context* ev_handler(Event e, Context *c) {
  rt_thread_t thread;
  thread = rt_thread_self();
  rt_ubase_t to = (rt_ubase_t)thread->user_data;

  rt_uint8_t **pp = (rt_uint8_t **)to;
  rt_uint8_t *t = *pp;
  Context* cc = (Context*) t;
  
  switch (e.event) {
    case 0x1 : c = cc; break;
    default: printf("Unhandled event ID = %d\n", e.event); assert(0);
  }
  return c;
}

void __am_cte_init() {
  cte_init(ev_handler);
}

void rt_hw_context_switch_to(rt_ubase_t to) {
  rt_thread_t thread;
  thread = rt_thread_self();
  rt_uint32_t temp = thread->user_data;
  thread->user_data = (rt_uint32_t) to;
  yield();
  thread->user_data = temp;
}

void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to) {
  rt_thread_t thread;
  thread = rt_thread_self();
  
  rt_uint8_t **pp = (rt_uint8_t **)from;
  *pp = (rt_uint8_t *)thread->sp;

  rt_uint32_t temp = thread->user_data;
  thread->user_data = (rt_uint32_t) to;
  yield();
  thread->user_data = temp;
  assert(0);
}

void rt_hw_context_switch_interrupt(void *context, rt_ubase_t from, rt_ubase_t to, struct rt_thread *to_thread) {
  assert(0);
}
typedef struct {
    void* tentry_;
    void* parameter_;
    void* texit_;
} CombinedArgs;

static void entry(void *args){
  CombinedArgs* arg = (CombinedArgs*)args;
  void* parameter = arg->parameter_;
  void (*tentry)(void *) = (void (*)(void *))arg->tentry_;
  void (*texit)(void) = arg->texit_;
  tentry(parameter);
  texit();
}

rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter, rt_uint8_t *stack_addr, void *texit) {
  uintptr_t addr = (uintptr_t)stack_addr;
  uintptr_t align = sizeof(uintptr_t);
  uintptr_t aligned_addr = (addr + align - 1) & ~(align - 1);
  stack_addr = (rt_uint8_t *)aligned_addr;

  CombinedArgs* args = (CombinedArgs*)malloc(sizeof(CombinedArgs));
  if (args == NULL) {
    assert(0);
  }
  args->tentry_ = tentry;
  args->parameter_ = parameter;
  args->texit_ = texit;
  Context *kcontextt = kcontext((Area) { stack_addr-2048, stack_addr }, entry, (void *)args);
  //assert(0);
  return (rt_uint8_t *)kcontextt;
}
