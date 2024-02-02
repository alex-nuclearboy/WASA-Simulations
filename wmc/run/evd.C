TCanvas *EvDisCan = 0;
TMultiGraph *mg = 0;

void evd()
{
    mg = (TMultiGraph *)gSClient->GetObject("MDC_Event_Display");
    if (!EvDisCan)
    {
        EvDisCan = new TCanvas("MDC_Event_Display","MDC Event Display",-600,600);
        mg->Draw("AP");
    }
    else
    {
        EvDisCan->Clear();
        mg->Draw("P");
    }
    gSClient->Start(1);
}
