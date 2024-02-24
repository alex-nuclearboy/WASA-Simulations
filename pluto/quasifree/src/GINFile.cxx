#include "../include/GINFile.hh"
GINFile::GINFile(char *conf)
{
    fMaxPart = 30;
    fPart = 0;
    fEvent = 0;
    if (conf) printf("Configuretion from: %s\n", conf);
    return;
}
int GINFile::Close()
{
    // Close the output file
    fclose(fout);
    return 0;
}
int GINFile::AddParticle(int type, const TLorentzVector *p)
{
    if (fPart >= fMaxPart) return 1;
    fPx[fPart] = p->Px();
    fPy[fPart] = p->Py();
    fPz[fPart] = p->Pz();
    fId[fPart] = type;
    fPart++;    // Increment the particle counter
    return 0;
}
int GINFile::WriteEvent(float weight)
{
    fEvent++;   // Increment event counter
    fprintf(fout, " %10d%10d%10d%10.4f%10.3E%10.3E\n",
        fEvent, fReac, fPart, fBeamPz, weight, weight);
    
    for(int i = 0; i < fPart; i++) {
        fprintf(fout, "%4d%10.4f%10.4f%10.4f%3d\n",
            i + 1, fPx[i], fPy[i], fPz[i], fId[i]);
    }
    return 0;
}
int GINFile::WriteHeader(const char *fnam, int reac, float pz, int nev)
{
    fout = fopen(fnam, "w");
    fReac = reac;
    fBeamPz = pz;
    int fform = 10000000 + reac;
    fprintf(fout, " %10d%10.2E%10.3f%10.3f%10.3f%10.3f%7d\n",
        fform, 0.5, pz, 0.f, 0.f, 0.f, nev);
    fprintf(fout, " REAC,CROSS(mb),B. MOM,  A1,    A2,    A3, # EVENTS\n");
    fprintf(fout, "  0.00000 0.00000 110.000 450.000  33.000  40.000 0.90000 2.30000\n");
    fprintf(fout, "  AFD7GH,ATS7GH,ZFD7GH,ZTS7GH,RCD7GH,ZCD7H,ASL7GH,ASU7GH\n");
    return 0;
}

