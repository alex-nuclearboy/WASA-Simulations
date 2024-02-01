//
{
  gSystem->Load("libPhysics.so");
  gSystem->Load("/home/tsl0/kupsc/NewCode/RWlib/lib/libBanks.so");
  TFile *test = TFile::Open("test.root");
  T->Print();
}
