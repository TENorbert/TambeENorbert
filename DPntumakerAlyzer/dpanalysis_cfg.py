import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
     'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/002CDB97-4588-E111-8BF4-003048D37694.root',

#Running on Local Files
#   'file:44GMSB100250.root'

#Running @ fnal lpc Data Files
     #  'dcache:/pnfs/cms/WAX/11/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/002CDB97-4588-E111-8BF4-003048D37694.root',
     #   'dcache:/pnfs/cms/WAX/11/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/069259D0-2C88-E111-9729-5404A63886C3.root'
        #'dcache:/pnfs/cms/WAX/11/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/'

# Running @CERN Data Files
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/085BD727-5588-E111-A1AB-003048D2BE06.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/1636AE64-3A88-E111-A085-0025901D624E.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/10D76EB4-4888-E111-9921-5404A63886C4.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/1815307C-5188-E111-8F54-5404A63886B6.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/1C8A921B-4F88-E111-A15C-0025901D626C.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/1CC8247F-3E88-E111-97C7-0025901D5DEE.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/20C5581B-4F88-E111-B5D6-0025901D629C.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/2A1EAED2-6188-E111-81FC-001D09F2983F.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/2E4E3D7D-5888-E111-A4D7-001D09F248F8.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/3E5E434B-2888-E111-91C5-001D09F25109.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/4E1C53FE-2888-E111-A17C-001D09F25460.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/56975C35-2E88-E111-9F10-BCAEC518FF67.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/60C5AA4D-2B88-E111-BADC-0025901D5DB8.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/6274944E-5888-E111-B0FC-5404A63886E6.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/6CCDA25C-3D88-E111-A8D2-0025901D5C86.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/6E15F68C-5688-E111-9188-003048D37456.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/6E6FFCD6-3D88-E111-960C-001D09F292D1.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/7A0041C3-3388-E111-9C6F-BCAEC5329703.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/7E4065C1-3688-E111-A09F-E0CB4E4408C4.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/7ED49E5B-3D88-E111-ACD4-5404A63886D4.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/86CE3F34-5388-E111-B8DC-BCAEC518FF54.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/92E25AC4-6488-E111-BD47-003048D374F2.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/9681417F-3E88-E111-A387-5404A63886CE.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/984E994E-5288-E111-A802-5404A63886D6.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/A8177F54-5188-E111-B95E-0025901D6288.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/A82E44C4-6488-E111-831E-002481E94C7E.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/AC4EB3F3-4288-E111-8841-003048D2BC52.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/B615548A-5888-E111-98F6-5404A63886C0.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/B6C11740-6A88-E111-8C61-001D09F24D4E.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/DC84213C-4488-E111-917E-001D09F27067.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/DCAED04E-5888-E111-96BA-5404A63886EC.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/DE8AD1D5-3D88-E111-998A-001D09F241B9.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/E0BB9364-2D88-E111-8638-BCAEC518FF6B.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/E8522469-3A88-E111-A0C1-002481E0D90C.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/F0A49E53-2B88-E111-A83E-003048D2C0F2.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/F0A98B54-5188-E111-8D9C-5404A63886C4.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/FC8B463F-6388-E111-97A3-001D09F2915A.root',
'rfio:/castor/cern.ch/cms/store/data/Run2012A/Photon/AOD/PromptReco-v1/000/191/247/FE15CABD-5788-E111-83F4-0025901D624E.root'
    )
)

process.demo = cms.EDAnalyzer('DPntumakerAlyzer',
    rootFileName     = cms.untracked.string('Photon_2012A_AOD_ALL_SMPTrg.root'), #HLT_Photon26_Photon18_v8
    triggerName      = cms.untracked.string('HLT_Photon75_CaloIdVL_v8'), #HLT_Photon75_CaloIdVL_IsoL_v16, Use di-photon Trigger instead!
    isData           = cms.untracked.bool(True),
    trigSource = cms.InputTag("TriggerResults","","HLT"),
    jetSource   = cms.InputTag("ak5PFJets"),
    metSource   = cms.InputTag("pfMet"),
    muonSource  = cms.InputTag("muons"),
    electronSource   = cms.InputTag("gsfElectrons"),
    photonSource     = cms.InputTag("photons"),
    pvSource         = cms.InputTag("offlinePrimaryVertices"),
    beamSpotSource   = cms.InputTag("offlineBeamSpot"),
    EBRecHitCollection = cms.InputTag("reducedEcalRecHitsEB"),
    EERecHitCollection = cms.InputTag("reducedEcalRecHitsEE"),
    ctau                = cms.double( 1000 ), 
    genParticles        = cms.InputTag("genParticles","","HLT"),
    seedGenNum          = cms.double("117"),
    # Set up cuts for physics objects
    # vertex cuts                z     ndof   d0 
    vtxCuts       = cms.vdouble( 20 ,   3 ,    2 ),
    # jet cuts                   pt    eta     dR,  nJets
    jetCuts       = cms.vdouble( 30. , 1.479,  0.3,    0 ),
    # photon cuts                pt   eta    sMajMax,  sMinMin, sMinMax,    Num  
    photonCuts    = cms.vdouble( 50,  1.479,    0.35,      0.1,     0.6,    1  ),
    # photon isolation           trk,  ecalSumEt, ecalR, hcalSumEt, hcalR 
    photonIso     = cms.vdouble(  0.2,   3.0,      0.1,    4.0,      0.1 ),
    # electron cuts              pt  eta  EBIso  EEIso nLostHit  
    electronCuts  = cms.vdouble( 25, 2.4,  0.15,   0.1,      2 ),
    metCuts       = cms.vdouble(  25. ),
    muonCuts      = cms.vdouble( 25, 2.1, 0.2, 0.3 ),

)


process.p = cms.Path(process.demo)
