#include<konoha.h>

#define KEYE_STR_MAX 4096

/* String Kernel.keyeRead() */

METHOD Kernel_keyeRead(Ctx* ctx, knh_sfp_t* sfp)
{
  char *str,*str2;
  FILE *fp;
  fpos_t *pos;
  long size;

  if((fp = fopen("/sys/kernel/debug/keye0","r")) == NULL){
    fprintf(stderr,"Cannot open keye\n"); 
    exit(1);
  }
  
 
  /*  if(fseek(fp,0L,SEEK_END) != 0){
    fprintf(stderr,"seek error\n");
    exit(1);
  }

  if(fgetpos(fp,pos) != 0){
    fpritnf(stderr,"get pos error\n");
    exit(1);
  }

  if(fseek(fp,0L,SEEK_SET) != 0){
    fprintf(stderr,"seek error\n");
    exit(1);
  }

  size = (long)pos;
  
  if(size>KEYE_STR_MAX){
    fprintf(stderr,"keye message is too long\n");
    size = KEYE_STR_MAX;
  }
  */
  str = (char*)malloc(sizeof(char)*128);  str2 = (char*)malloc(sizeof(char)*128);
  memset(str,0,128);
  memset(str, 0, 128);
  size_t len = 0;
  if((len = read(fileno(fp), str, 128)) < 0){
    fprintf(stderr,"read error\n");
    exit(1);
  }
  //  fprintf(stderr, "len is %d\n", len);
  //  str[len] = '\0';
  int i;
  for (i = 0 ; i < len; i++) fprintf(stderr, "%c", str[i]);
  len = 0;
  if((len = read(fileno(fp), str2, 128)) < 0){
    fprintf(stderr,"read error\n");
    exit(1);
  }
  fprintf(stderr, "len is %d\n", len);
  str2[len] = '\0';

  fprintf(stderr,"str is %s\n",str);
  fprintf(stderr,"str2 is %s\n",str2);

  fclose(fp);

  KNH_RETURN(ctx,sfp,new_String(ctx,B(str),NULL));
}

//#ifdef Linux?

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
  //  remove(filename);
  //  remove("Makefile");
  //system("cd ..");
  //remove("./temp_make");
  
  KNH_RETURN_void(ctx,sfp);
}


/* void Kernel.insmod(String filename) */

METHOD Kernel_insmod(Ctx* ctx, knh_sfp_t* sfp)
{
  char *cmd = alloca(sizeof(char*));
  char *str = "insmod";
  char *filename = p_char(sfp[1]);
  int len = strlen(str)+strlen(filename)+2;
  snprintf(cmd, len, "%s %s", str, filename);
  system(cmd);

  KNH_RETURN_void(ctx, sfp);
}

/*void Kernel.rmmod(String modname) */

METHOD Kernel_rmmod(Ctx* ctx, knh_sfp_t* sfp)
{
  char *cmd = alloca(sizeof(char*));
  char *str = "rmmod";
  char *modname = p_char(sfp[1]);
  int len = strlen(str)+strlen(modname)+2;
  snprintf(cmd, len, "%s %s", str, modname);
  system(cmd);

  KNH_RETURN_void(ctx, sfp);
}
