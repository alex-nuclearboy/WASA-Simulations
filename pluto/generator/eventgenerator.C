#include <PBeamSmearing.h>
#include <PReaction.h>
#include <TF1.h>
#include <Riostream.h>

void eventgenerator() {

    PBeamSmearing *smear = new PBeamSmearing("beam_smear", "Beam smearing");
    TF1 *mf=new TF1("Uniform","1",1.426,1.635);
    smear->SetReaction("p + d");
    smear->SetMomentumFunction(mf);
    makeDistributionManager()->Add(smear);

    PReaction myReaction(1.5305,"p","d","p d", "${PLUTO_OUTPUT}/pd-pd-1",1,0,0,0);

    myReaction.Print();
    myReaction.Loop(1000000);

}

