#define RELAYNAME "keye"

#include "eye.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shinpei Nakata");
MODULE_DESCRIPTION("Kernel eye v0.01");


#define EYE_MAX 100

static int mymallocsize;
static struct kernel_eye* eye_table[EYE_MAX];
static int registered_eyes;

struct rchan *g_chan;

#define _keye_malloc(size) \
  kmalloc(size, GFP_KERNEL); \
  mymallocsize += size;

void *keye_malloc(size_t size)
{
  void *p;
  p = kmalloc(size, GFP_KERNEL);
  mymallocsize += size;
  return p;
}
EXPORT_SYMBOL(keye_malloc);

void keye_free(void *ptr, size_t size)
{
  kfree(ptr);
  mymallocsize -= size;
}

#define _keye_zalloc(size) \
  kzalloc(size, GFP_KERNEL)

struct rchan *chan;

static struct dentry *create_buf_file_handler(const char *filename,
					      struct dentry *parent,
					      int mode,
					      struct rchan_buf *buf,
					      int *is_global)
{
  return debugfs_create_file(filename, mode, parent, buf,
			     &relay_file_operations);
}

static int remove_buf_file_handler(struct dentry *dentry)
{
  debugfs_remove(dentry);
  return 0;
}

static struct rchan_callbacks relay_callbacks = 
  {
    .create_buf_file = create_buf_file_handler, 
    .remove_buf_file = remove_buf_file_handler,
  };


static
void keye_register_eye(struct kernel_eye *eye)
{
  eye_table[registered_eyes] = eye;
  registered_eyes++;
}

static 
void keye_delete_eye (struct kernel_eye *eye)
{

}

struct kernel_eye *new_kerneleye(void)
{
  struct kernel_eye *ret;
  ret = (struct kernel_eye *)keye_malloc(sizeof(struct kernel_eye));
  keye_register_eye(ret);
  ret->chan = g_chan;
  keye_new_counter(ret);
  keye_new_interval(ret);
  return ret;
}
EXPORT_SYMBOL(new_kerneleye);

void free_kerneleye(struct kernel_eye *eye)
{
  /* freeing kernel_eye instance */
  keye_free(eye->counter, sizeof(int));
  keye_free(eye->interval, sizeof(int));
  keye_free(eye, sizeof(struct kernel_eye));
}
EXPORT_SYMBOL(free_kerneleye);

int keye_write(struct kernel_eye *eye, char *line, size_t size)
{
  relay_write(eye->chan, line, size);
  return 0;
}
EXPORT_SYMBOL(keye_write);

/* -------------------------------------------
 *
 *  couter methods
 *
 * ------------------------------------------*/


void keye_increment_counter (struct kernel_eye *eye)
{
  /* TODO: overflow check shold implement here */
  *(eye->counter) = *(eye->counter) + *(eye->interval);
}

EXPORT_SYMBOL(keye_increment_counter);


void keye_decrement_coutner(struct kernel_eye *eye)
{
  /* TODO: overflow (under zero?) detection shold be placed here. */
  *(eye->counter) = *(eye->counter) - *(eye->interval);
}

void keye_new_counter(struct kernel_eye *eye)
{
  eye->counter = (int *)keye_malloc(sizeof(int));
  *(eye->counter) = 0;

}
EXPORT_SYMBOL(keye_new_counter);

void keye_new_counter_with_init (struct kernel_eye *eye, int initiate)
{
  eye->counter = (int *)keye_malloc(sizeof(int));
  *(eye->counter) = initiate;
}

void keye_new_interval (struct kernel_eye *eye)
{
  eye->interval = (int *)keye_malloc(sizeof(int));
  *(eye->interval) = 1;
}

void keye_new_interval_with_init(struct kernel_eye *eye, int initiate)
{
  eye->interval = (int *)keye_malloc(sizeof(int));
  *(eye->interval)  = initiate;
}

static int init_keye(void)
{
  int ret;
  
  registered_eyes = 0;
  g_chan = relay_open(RELAYNAME, NULL, 1024, 4, &relay_callbacks, NULL);

  KEYE_DEBUG("create relay !");
  return 0;
}

static void exit_keye(void)
{
  relay_close(g_chan);
}

module_init(init_keye);
module_exit(exit_keye);
