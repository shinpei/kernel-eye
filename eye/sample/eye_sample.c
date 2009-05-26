#include "../eye.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shinpei Nakata");

struct kernel_eye *eye;

static int kp_pre_handler (struct kprobe *p, struct pt_regs *regs)
{
  keye_increment_counter(eye);

  return 0;
}

static struct kprobe kp = {
  .addr = 0xc0481815,
  .pre_handler = kp_pre_handler,
};


static int init_eye_sample(void)
{
  eye = new_kerneleye();
  //keye_write(eye, "hello world\n", sizeof("hello world\n"));
  //  kp.addr = (kprobe_opcode_t *)kallsyms_lookup_name("sys_open");

  register_kprobe(&kp);
  return 0;
}

static void exit_eye_sample(void)
{
  printk("count:%d\n", keye_get_counter(eye));
  free_kerneleye(eye);
  unregister_kprobe(&kp);
}
			     
module_init(init_eye_sample);
module_exit(exit_eye_sample);
