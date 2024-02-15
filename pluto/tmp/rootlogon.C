{
    gSystem->Load("libMatrix.so");
    gSystem->Load("libHist.so");
    gSystem->Load("libPhysics.so");
    gSystem->Load("libRIO.so");
    gSystem->Load("libTree.so");
    gSystem->Load("libTreeViewer");

    gSystem->Load("$PLUTOSYS/libPluto.so");
    gSystem->SetIncludePath("-I/home/WASA-software/pluto/src");

    //in case of background process we can attach 2 lines below and in terminal write: "nohup root -b -q rootlogon.C &"
    //gROOT->ProcessLine(".L eventgenerator.C+");
    //gROOT->ProcessLine(".x eventgenerator.C+");
}
