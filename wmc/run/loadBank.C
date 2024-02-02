//
{
    gSystem->Load("libPhysics.so");
    gSystem->Load("$WASA_ROOT/../RWlib/lib/libBanks.so");
    TFile *test = TFile::Open("test.root");
    T->Print();
}
