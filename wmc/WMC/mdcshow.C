char RSLibDir[] = {"/home/tsl0/petukhov/wasa@cosy/core/lib/"};
TControlBar *bar = 0;
void mdcshow()
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
gROOT->ProcessLine(".L evd.C");
bar = new TControlBar("vertical", "Event Display",10,10);
bar->AddButton("Draw next event","evd()","Analyze and show next event"); 
bar->AddButton("Quit           ","gROOT->ProcessLine(\".q\")","Quit ROOT"); 
bar->Show();
gROOT->SaveContext();
}
