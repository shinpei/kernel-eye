#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

int pre_handler(struct kprobe *p, struct pt_regs *regs){
  printk("Hello kernel world\n");
  return 0;
}

static struct kprobe kp = {
  .pre_handler = pre_handler,
  .addr = (kprobe_opcode_t *)0xc042d479
};

static __init int test_init(void){
  register_kprobe(&kp);
  return 0;
}

module_init(test_init);

static __exit void test_exit(void){
  unregister_kprobe(&kp);
  printk(KERN_ALERT "Good bye kernel world\n");
}

module_exit(test_exit);

MODULE_LICENSE("GPL");
