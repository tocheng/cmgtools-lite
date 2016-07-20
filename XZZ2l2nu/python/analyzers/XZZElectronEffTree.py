import ROOT
import random
import math
from  itertools import combinations
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import *
import PhysicsTools.HeppyCore.framework.config as cfg
from CMGTools.XZZ2l2nu.tools.Pair import *

class XZZElectronEffTree( Analyzer ):

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(XZZElectronEffTree,self).__init__(cfg_ana,cfg_comp,looperName)
        self.genfilter=getattr(self.cfg_ana,"genfilter",False)
        self.checktag=getattr(cfg_ana,'checktag',False)
        self.pfbkg=getattr(cfg_ana,'pfbkg',False)

    def declareHandles(self): 
        super(XZZElectronEffTree, self).declareHandles()
        if self.pfbkg:
            self.handles['pfcandidate']=AutoHandle("packedPFCandidates","std::vector<pat::PackedCandidate>")

    def checkgen(self,lep):
        try:
            mom=lep.physObj.genParticle()
            while mom.pdgId()!=23:
                mom=mom.mother()
            return True
        except:
            return False

    def istag(self,el):
        itg=1
        if not el.physObj.electronID("cutBasedElectronID-Spring15-25ns-V1-standalone-loose"): itg-=1
        return itg

    def process(self, event):
        self.readCollections( event.input )
        event.llpair=[]
        if self.checktag:
            for i in event.selectedElectrons: i.istag=self.istag(i)
        if self.pfbkg:
            event.selectedhadrons=[i for i in self.handles['pfcandidate'].product() if i.pt()>20 and abs(i.pdgId())==211 and abs(i.eta())<2.4]
            if self.checktag: event.selectedElectronstagged=[i for i in event.selectedElectrons if i.istag==1]
            if not (event.selectedElectrons and event.selectedhadrons): return False
            for i in event.selectedElectrons:event.llpair.extend([Pair(i,j,23) for j in event.selectedhadrons])
        else:
            event.zllcandidates=event.selectedElectrons
            if len(event.zllcandidates)<2: return False
            for l1,l2 in combinations(event.zllcandidates,2):
                if l1.pdgId() == -l2.pdgId():
                    pair = Pair(l1,l2,23)
                    event.llpair.append(pair)
            if not event.llpair: return False
            event.llpair=[min(event.llpair,key = lambda x: abs(x.M()-91.118))]
        if self.genfilter and self.cfg_comp.isMC:
            for i in event.llpair:
                i.leg1.xdaughter=self.checkgen(i.leg1)
                i.leg2.xdaughter=self.checkgen(i.leg2)
        return True




        
            

        


                
                
