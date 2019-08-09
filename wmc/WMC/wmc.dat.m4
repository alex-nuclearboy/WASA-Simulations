 LIST
C
C GEANT MC example job 
C Created: 13/11/05 AK  for WASA-at-COSY
C  
C 
NOLI
C include standard mc processing parameters
READ 11 DEFAULT ALIGNMENT FILE
READ 12 SPECIAL ALIGNMENT FILE FOR THIS RUN  $$$
READ 13 SPECIAL ALIGNMENT FILE FOR THIS RUN  $$$
LIST

WPLT  0  3 'WASA' 0 0.040 2  0. 0. 0. 0. 0. 0.
C WPLT = 1 plot setup (interactive) ; = 0 batch 
NOLI
TRIG 3000000  NB OF EVENTS TO PROCESS $$$
C RUNG 78 37    RUN AND EVENT NB
TIME 10. 10. 0  AFTER INIT / FOR TERM / TIME CHECK INTERVAL IN EVENTS 
DEBU 0 0 500  FIRST,LAST EVENT FOR USER DEBUG AND GTRIGI PRINT FREQUENCY
PREV 1 3  FIRST,LAST EVENT TO PRINT / NEG NBS MEANS NOT TO PRINT
DBEV 0 3   FIRST,LAST EVENT TO DEBUG / NEG NBS MEANS NOT TO DEBUG
C HSTA STANDARD HISTOGRAMS   'TIME' 'SIZE' 'MULT' 'NTRA' 'STAK'
PRIN PRINT DATA STRUCTURES :
  'MATE' 'TMED'  'VOLU'
C  'ROTM'  'SETS' 'PART'   'LOSS'
  'VERT' 'KINE' 'HITS' 'DIGI'
PFLG WASA PRINT FLAGS LPRXXX
C   MTL EQM MFL CBM TGT EGN TRI TRC ERC HIS  ZZ(1-5)
     1   1   1   1   1   1   8   0   0  -1    5*-1
C SAVE $$$ DATA STRUCTURES TO FZ FILE :  'INIT' 
C 
C SAVE $$$ DATA STRUCTURES TO FZ FILE :  'INIT' 
C 'KINE' 'HITS'
C GET $$$ DATA FROM FZ FILE (SET KINE TO ZERO FOR NO GEN): 'INIT' 'KINE' 'HITS'
C RSAV 'INIT'
C GET 'INIT'

C INPUT MODES
C KINE: Event Generator mode
C      10 External ascii file
C       1 Internal event generator
C      50 Reading from pluto file (pluto.root)
C
C Structure:
C KINE X TRACK GENERATION MODE $$$
C 1.   3.    0.   0.   0.   0.   0.  0.   1.  1.
C
C Always comment/uncomment the line starting with KINE and
C one option line with 10 numbers
C The variables in the option line have the following meaning
C RE1   RE2   RE3  RE4  RE5  RE6  RE7 RE8 NROT Nfile 

C KINE 0 TRACK GENERATION MODE $$$
C 1.   3.    0.   0.   0.   0.   0.  0.   1.  1.

C KINE 10 TRACK GENERATION MODE $$$
C 1.   3.    0.   0.   0.   0.   0.  0.   1.  1.

C For KINE 50 RE1*1000 gives the event number to start with
KINE 50 TRACK GENERATION MODE $$$
  0.   3.    0.   0.   0.   0.   0.  0.   1.  1.

C For KINE 1 option line has the following meaning
C THETA(DEG),  PHI(DEG) ,  EKIN(GEV) , NB.  , PART. , NB. , REACTION
C  LO    UP ,  LO   UP  ,  LO     UP ,TRACKS, TYPE  ,ROT. ,  TYPE
C KINE 1 TRACK GENERATION MODE $$$
C   0.  160.    5.   10.   .800 .800    3.    14.     0.    11.
C   4.   18.    0.  360.   .500 .900    3.    14.     0.    11.
C  21.   25. -175. -185.  0.250 .350    1.    14.     0.    11.
C  13.5  17.3  15.   75.  0.050 .500    1.     3.     0.    11.
C   0.  180.    0.  360.  0.30  .200    5.     8.     0.    11.

NOLIST

C For that version one has to disable EPIO:
EPIO 0 0 

C +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
C  Put your local cards below before END
C +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
C #m4_include(User_local_cards.m4)

NOLIST
END
