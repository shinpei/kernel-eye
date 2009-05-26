using test.*;
String modname = "sample";
String source = "#include <linux/init.h>\n#include <linux/module.h>\nMODULE_LICENSE(\"Dual BSD/GPL\");\n\nstatic int hello_init(void)\n{\nprintk(KERN_ALERT \"Hello, world!\\n\");\nreturn 0;\n}\n\nstatic void hello_exit(void)\n{\nprintk(KERN_ALERT \"Goodbye...\\n\");\n}\n\nmodule_init(hello_init);\nmodule_exit(hello_exit);\n";

%man(Kernel);

print "make start!";
Kernel.buildModule(modname,source);
print "finish";
