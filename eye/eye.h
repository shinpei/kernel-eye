#ifndef __EYE_H
#define __EYE_H

#include <linux/module.h>
#include <linux/security.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/relay.h>
#include <linux/kprobes.h>
#include <linux/debugfs.h>
#include <linux/kernel.h>

#include "eye_debug.h"

struct kernel_eye  {
  struct rchan *chan;
  struct eye_funcs *funcs;
  int* counter;
  int* interval;
};


#define keye_get_counter(eye)  *(eye->counter)
#define keye_get_interval(eye) *(eye->interval)


/* internal functions */
void *keye_malloc(size_t size);
void keye_free(void *ptr, size_t size);
void keye_new_counter(struct kernel_eye * eye);
void keye_new_counter_with_init (struct kernel_eye * eye, int initiate);
void keye_new_interval(struct kernel_eye *eye);
void keye_new_interval_with_init (struct kernel_eye *eye, int initiate);

/* kernel eye API */
extern int keye_write(struct kernel_eye *eye, char *line, size_t size);
extern struct kernel_eye *new_kerneleye(void);
extern int register_kerneleye_func(void);
extern void free_kerneleye(struct kernel_eye *eye);
extern void keye_increment_counter(struct kernel_eye *eye);
extern void keye_decrement_couner(struct kernel_eye *eye);



#endif /* __EYE_H */
