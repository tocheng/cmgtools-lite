import math
import ROOT
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi

class Pair(object):
    def __init__(self,leg1,leg2,pdg = 0):
        self.l1=ROOT.TLorentzVector()
        self.useTuneP=3
        if hasattr(leg1,'TuneP_pt'):self.l1.SetPtEtaPhiM(leg1.TuneP_pt(),leg1.TuneP_eta(),leg1.TuneP_phi(),leg1.TuneP_m())
        else: 
            self.l1.SetPtEtaPhiM(leg1.p4().pt(),leg1.p4().eta(),leg1.p4().phi(),leg1.p4().mass())
            self.useTuneP-=1
        self.l2=ROOT.TLorentzVector()
        if hasattr(leg2,'TuneP_pt'):self.l2.SetPtEtaPhiM(leg2.TuneP_pt(),leg2.TuneP_eta(),leg2.TuneP_phi(),leg2.TuneP_m())
        else: 
            self.l2.SetPtEtaPhiM(leg2.p4().pt(),leg2.p4().eta(),leg2.p4().phi(),leg2.p4().mass())
            self.useTuneP-=2
        self.leg1 = leg1
        self.leg2 = leg2
        self.pdg = pdg
        self.LV = leg1.p4()+leg2.p4()
        self.TuneP_LV = self.l1+self.l2
        et1 = math.sqrt(leg1.mass()*leg1.mass()+leg1.pt()*leg1.pt())
        et2 = math.sqrt(leg1.mass()*leg1.mass()+leg2.pt()*leg2.pt())
        TuneP_et1 = math.sqrt(leg1.mass()*leg1.mass()+self.l1.Pt()*self.l1.Pt())
        TuneP_et2 = math.sqrt(leg1.mass()*leg1.mass()+self.l2.Pt()*self.l2.Pt())

        self.MT  =math.sqrt(self.leg1.p4().mass()*self.leg1.p4().mass()+\
            self.leg1.p4().mass()*self.leg1.p4().mass()+2*(et1*et2-self.leg1.p4().px()*self.leg2.p4().px()-self.leg1.p4().py()*self.leg2.p4().py()))
        et2a = math.sqrt(91.188**2+leg2.pt()*leg2.pt())  
        self.TuneP_MT  =math.sqrt(self.l1.M()*self.l1.M()+\
            self.l1.M()*self.l1.M()+2*(TuneP_et1*TuneP_et2-self.l1.Px()*self.l2.Px()-self.l1.Py()*self.l2.Py()))

        self.MTa = math.sqrt(self.leg1.p4().mass()*self.leg1.p4().mass()+\
            91.188**2+2*(et1*et2a-self.leg1.p4().px()*self.leg2.p4().px()-self.leg1.p4().py()*self.leg2.p4().py()))

        et1b = leg1.p4().Et()
        et2b = math.sqrt(91.188**2+leg2.pt()**2)
        self.MTb = math.sqrt( (et1b+et2b)**2 - (leg1.pt()**2+leg2.pt()**2+2.0*(self.leg1.p4().px()*self.leg2.p4().px()+self.leg1.p4().py()*self.leg2.p4().py())) ) 

        et1c = math.sqrt((leg1.mass())**2+leg1.pt()**2)
        et2c = math.sqrt((leg1.mass())**2+leg2.pt()**2)
        self.MTc = math.sqrt( (et1c+et2c)**2 - (leg1.pt()**2+leg2.pt()**2+2.0*(self.leg1.p4().px()*self.leg2.p4().px()+self.leg1.p4().py()*self.leg2.p4().py())) )

    def rawP4(self):
        return self.leg1.p4()+self.leg2.p4()

    def p4(self):
        return self.LV

    def TuneP_p4(self):
        return self.TuneP_LV
    
    def m(self):
        return self.LV.mass()
    
    def TuneP_m(self):
        return self.TuneP_LV.M()

    def pdgId(self):
        return self.pdg
    
    def mt2(self):
        return self.MT*self.MT

    def mt(self):
        return self.MT

    def TuneP_mt(self):
        return self.TuneP_MT

    def mta(self):
        return self.MTa

    def mtb(self):
        return self.MTb

    def mtc(self):
        return self.MTc
 
    def pt(self):
        return self.LV.pt()

    def px(self):
        return self.LV.px()

    def py(self):
        return self.LV.py()

    def pz(self):
        return self.LV.pz()

    def eta(self):
        return self.LV.eta()

    def phi(self):
        return self.LV.phi()

    def TuneP_pt(self):
        return self.TuneP_LV.Pt()

    def TuneP_px(self):
        return self.TuneP_LV.Px()

    def TuneP_py(self):
        return self.TuneP_LV.Py()

    def TuneP_pz(self):
        return self.TuneP_LV.Pz()

    def TuneP_eta(self):
        return self.TuneP_LV.Eta()

    def TuneP_phi(self):
        return self.TuneP_LV.Phi()

    def AbsdeltaPhi(self):
        return abs(deltaPhi(self.leg1.phi(),self.leg2.phi()))

    def AbsdeltaR(self):
        return abs(deltaR(self.leg1.eta(),self.leg1.phi(),self.leg2.eta(),self.leg2.phi()))

    def deltaPhi(self):
        return deltaPhi(self.leg1.phi(),self.leg2.phi())

    def TuneP_deltaPhi(self):
        return deltaPhi(self.l1.Phi(),self.l2.Phi())

    def deltaR(self):
        return deltaR(self.leg1.eta(),self.leg1.phi(),self.leg2.eta(),self.leg2.phi())

    def TuneP_deltaR(self):
        return deltaR(self.l1.Eta(),self.l1.Phi(),self.l2.Eta(),self.l2.Phi())

    def __getattr__(self, name):
        return getattr(self.LV,name)

