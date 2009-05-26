#include<konoha.h>

#define KEYE_STR_MAX 4096
/* String Kernel.keyeRead() */

METHOD Kernel_keyeRead(Ctx* ctx, knh_sfp_t* sfp)
{
  char *str;
  FILE *fp;
  fpos_t *pos;
  long size;

  if((fp = fopen("/mnt/debugfs/keye0","r")) == NULL){
    fprintf(stderr,"Cannot open keye\n"); 
    exit(1);
  }
  
 
  if(fseek(fp,0L,SEEK_END) != 0){
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

  str = (char*)malloc(sizeof(char)*size);
 
  if(read(fileno(fp),str,size)<0){
    fprintf(stderr,"read error\n");
    exit(1);
  }

  fclose(fp);

  KNH_RETURN_String(ctx,sfp,new_String(ctx,B(str),NULL));
}
