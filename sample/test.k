class Kernel;

void Kernel.insmod( String filename );
void Kernel.rmmod( String modname );
void Kernel.buildModule( String modname, String source);
String Kernel.keyeRead();