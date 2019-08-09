char RSLibDir[] = {"/home/tsl0/petukhov/wasa@cosy/core/lib/"};
void mdc()
{
char line[128];
strcpy(line, ".L ");
strcat(line, RSLibDir);
strcat(line, "libRSCore.so");
gROOT->ProcessLine(line);
strcpy(line, ".L ");
strcat(line, RSLibDir);
strcat(line, "libRSClient.so");
gROOT->ProcessLine(line);
gSClient->GetSorterList();
gSClient->Fetch(0);
gROOT->ProcessLine("TBrowser b;");
}
