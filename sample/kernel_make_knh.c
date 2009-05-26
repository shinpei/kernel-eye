#include<konoha.h>

o//#ifdef Linux?

static char *TEMP_MAKEFILE_1 =
  "ifneq ($(KERNELRELEASE),)\n"
  "\tobj-m := ";
static char *TEMP_MAKEFILE_2 =
  ".o\nelse\n"
  "\tKERNELDIR = /lib/modules/$(shell uname -r)/build\n"
  "\tPWD := $(shell pwd)\n"
  "default:\n"
  "\t$(MAKE) -C $(KERNELDIR) M=$(PWD) modules\n"
  "endif\n";

/* not use */
static char *TEMP_C =
  "#include <linux/init.h>\n"
  "#include <linux/module.h>\n"
  "MODULE_LICENSE(\"Dual BSD/GPL\");\n\n"
  "static int hello_init(void)\n"
  "{\n"
  "printk(KERN_ALERT \"Hello, world!\\n\");\n"
  "return 0;\n"
  "}\n\n"
  "static void hello_exit(void)\n"
  "{\n"
  "printk(KERN_ALERT \"Goodbye...\\n\");\n"
  "}\n\n"
  "module_init(hello_init);\n"
  "module_exit(hello_exit);\n";


/* void Kernel.buildModule(String modname, String source) */

METHOD Kernel_buildModule(Ctx *ctx, knh_sfp_t* sfp)
{
  FILE *fp_mf, *fp_tf;
  char *modulename,*filename;
  char *source;
  //char *cmd;
  int len;

  modulename = p_char(sfp[1]);
  source = p_char(sfp[2]);
  len = strlen(modulename);

  filename = (char*)alloca(len+3);
  strncpy(filename,modulename,len+3);
  filename[len] = '.';
  filename[len+1] = 'c';

  /* make directry & move */
  //system("mkdir .temp_make");
  //system("cd temp_make");

  /* file open */
  /*
    Kernel buildsystem has to read "Makefile" file.
    thats why we stick to this filename 
  */
  if((fp_mf = fopen("Makefile","w")) == NULL){
    fprintf(stderr,"Error:open Makefile\n");
    exit(1);
  }
  
  if((fp_tf = fopen(filename,"w")) == NULL){	
    fprintf(stderr,"Error:open %s\n",filename);
    exit(1);
  }
  
  /* write */
  if(fprintf(fp_mf,"%s%s%s",TEMP_MAKEFILE_1,modulename,TEMP_MAKEFILE_2) < 0){
    fprintf(stderr,"Error:write makefile\n");
    exit(1);
  }
	
  if(fprintf(fp_tf,"%s",source) < 0){
    fprintf(stderr,"Error:write %s\n",filename);
    exit(1);
  }
  
  /* close */
  fclose(fp_mf);
  fclose(fp_tf);
  
  system("make");

  //snprintf(cmd, len+10,"mv %s.ko ..",moldulename);
  //system("mv ???.ko ..");
  

  /*
    remove files
    list: Makefile modulename.* 
  */
  remove(filename);
  remove("Makefile");
  //system("cd ..");
  //remove("./temp_make");
  
  KNH_RETURN_void(ctx,sfp);
}