using test.*;
String modname = "temp";

String src = '''#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/eye.h>

struct kernel_eye *eye;

int pre_handler(struct kprobe *p, struct pt_regs *regs){
  eye = new_kerneleye();
  keye_write(eye, "hello world\\n\", sizeof("hello world\\n\"));
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
  keye_write(eye, "Good bye kernel world\\n\", sizeof("Good bye kernel world\\n\"));
  free_kerneleye(eye);
  unregister_kprobe(&kp);
}

module_exit(test_exit);

MODULE_LICENSE("GPL");
'''
  print "make start";
Kernel.buildModule(modname,src);
Kernel.insmod("temp.ko")
Kernel.rmmod("temp.ko")
String s = Kernel.keyeRead()

