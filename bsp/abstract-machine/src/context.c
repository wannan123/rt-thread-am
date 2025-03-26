#include <am.h>
#include <klib.h>
#include <rtthread.h>

typedef struct {
  rt_ubase_t from;
  rt_ubase_t to;
  bool has_from;
  int a;
} From_to;

typedef struct {
    void* tentry_;
    void* parameter_;
    void* texit_;
    int a ;
} CombinedArgs;

static Context* ev_handler(Event e, Context *c) {
  rt_thread_t thread;
  thread = rt_thread_self();
  From_to* tmp = (From_to *)thread->user_data;
  rt_ubase_t to = tmp->to;


  if (tmp->has_from)
  {
    Context** from = (Context**)tmp->from;
    *from = c;
  }
  
  rt_uint8_t **pp = (rt_uint8_t **)to;
  rt_uint8_t *t = *pp;
  Context* cc = (Context*) t;
  // printf("%d\n",e.event);
  switch (e.event) {
    case 0x1 : c = cc; break;
    case 0x5 : c = c; break;
    default: printf("Unhandled event ID = %d\n", e.event); assert(0);
  }

  return c;
}

void __am_cte_init() {
  cte_init(ev_handler);
}

// void rt_hw_context_switch_to(rt_ubase_t to) {
//   rt_thread_t thread;
//   thread = rt_thread_self();
//   rt_uint8_t *stack = (rt_uint8_t * ) thread->sp;
//   stack -= sizeof(CombinedArgs); 
//   stack -= sizeof(From_to); 

//   From_to* args = (From_to*)stack;
//   args->to = to;
//   args->has_from = false;
//   args->a = 3;
//   uintptr_t  temp = thread->user_data;
//   thread->user_data = (uintptr_t ) args;

//   stack += sizeof(From_to); 
//   stack += sizeof(CombinedArgs); 
//   yield();
//   thread->user_data = temp;
// }
void rt_hw_context_switch_to(rt_ubase_t to) {
  rt_thread_t thread;
  thread = rt_thread_self();
  From_to* args = (From_to*)rt_malloc(sizeof(From_to));
  memset(args, 0, sizeof(From_to));
  args->to = to;
  args->has_from = false;
  args->a = 3;
  uintptr_t  temp = thread->user_data;
  thread->user_data = (uintptr_t ) args;
  yield();
  thread->user_data = temp;
  rt_free(args);
}

// void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to) {
//   rt_thread_t thread;
//   thread = rt_thread_self();
//   rt_uint8_t *stack = (rt_uint8_t * ) thread->sp;
//   stack -= sizeof(CombinedArgs); 
//   stack -= sizeof(From_to); 

//   From_to* args = (From_to*)rt_malloc();
//   args->from = from;
//   args->to = to;
//   args->has_from = true;
//   args->a = 4;

//   uintptr_t  temp = thread->user_data;
//   uintptr_t  arg = (uintptr_t ) args;
//   thread->user_data = arg;

//   stack += sizeof(From_to); 
//   stack += sizeof(CombinedArgs); 
//   yield();
//   thread->user_data = temp;
//   // assert(0);
// }
void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to) {
  rt_thread_t thread;
  thread = rt_thread_self();
  From_to* args = (From_to*)rt_malloc(sizeof(From_to));
  memset(args, 0, sizeof(From_to));

  args->from = from;
  args->to = to;
  args->has_from = true;
  args->a = 4;
  uintptr_t  temp = thread->user_data;
  uintptr_t  arg = (uintptr_t ) args;
  thread->user_data = arg;
  yield();
  thread->user_data = temp;
  rt_free(args);
}

void rt_hw_context_switch_interrupt(void *context, rt_ubase_t from, rt_ubase_t to, struct rt_thread *to_thread) {
  assert(0);
}


static void entry(void *args){
  
  CombinedArgs* arg = (CombinedArgs*)args;
  
  
  void (*tentry)(void *) = (void (*)(void *))arg->tentry_;
  // printf("%d\n",a);
  // printf("%d\n",a);
  void* parameter = arg->parameter_;
  
  void (*texit)(void) = arg->texit_;
  
  tentry(parameter);
  texit();
  
}

// rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter, rt_uint8_t *stack_addr, void *texit) {
//   uintptr_t addr = (uintptr_t)stack_addr;
//   uintptr_t align = sizeof(uintptr_t);
//   uintptr_t aligned_addr = (addr + align - 1) & ~(align - 1);
//   stack_addr = (rt_uint8_t *)aligned_addr;

//   stack_addr -= sizeof(CombinedArgs); 
//   CombinedArgs* args = (CombinedArgs*)stack_addr;

//   if (args == NULL) {
//     assert(0);
//   }
//   args->tentry_ = tentry;
//   args->parameter_ = parameter;
//   args->texit_ = texit;
//   args->a = 9;
//   stack_addr += sizeof(CombinedArgs); 

//   Context *kcontextt = kcontext((Area) { stack_addr-2048, stack_addr }, entry, (void *)args);
//   //assert(0);
//   return (rt_uint8_t *)kcontextt;
// }
void *rt_hw_stack_init(void *tentry, void *parameter, rt_uint8_t *stack_addr, void *texit) {
  CombinedArgs* args = (CombinedArgs*)rt_malloc(sizeof(CombinedArgs));
  if (args == NULL) {
    assert(0);
  }
  memset(args, 0, sizeof(CombinedArgs));

  args->tentry_ = tentry;
  args->parameter_ = parameter;
  args->texit_ = texit;
  args->a = 9;
  
  Context *kcontextt = kcontext((Area) { stack_addr-2048, stack_addr }, entry, (void *)args);
  return (rt_uint8_t *)kcontextt;
}