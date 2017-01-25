from ROOT import *
from tdrStyle import *
setTDRStyle()
        
import os,sys
from array import array

mass = array('d',[])
zeros = array('d',[])
exp_p2 = array('d',[])
exp_p1 = array('d',[])
exp = array('d',[])
exp_m1 = array('d',[])
exp_m2 = array('d',[])

exp_sim = array('d',[])
exp_sim_up = array('d',[])
exp_sim_dn = array('d',[])

exp_sim1 = array('d',[])

output="gjet_zpt100met50_unSmooth/"

for i in range(0,8):

    m = int(600+i*200)
    if(m==1600) : continue

    scale = 24.7549/1000
    mass.append(float(m))
    zeros.append(0.0)

    cut = 'zpt100met50'
    f = TFile(output+"higgsCombine"+cut+".Asymptotic.mH"+str(m)+".root","READ")
    t = f.Get("limit")
    
    t.GetEntry(2)
    thisexp = t.limit*scale
    exp.append(thisexp)
    t.GetEntry(0)
    exp_m2.append(thisexp-t.limit*scale)
    t.GetEntry(1)
    exp_m1.append(thisexp-t.limit*scale)
    t.GetEntry(3)
    exp_p1.append(t.limit*scale-thisexp)
    t.GetEntry(4)
    print 'error',t.limit*scale-thisexp
    exp_p2.append(t.limit*scale-thisexp)

    cut = 'zpt100met50'
    f = TFile(output+"higgsCombine"+cut+".Asymptotic.mH"+str(m)+".root","READ")
    t = f.Get("limit")

    t.GetEntry(2)
    thisexp = t.limit*scale
    exp_sim.append(thisexp)
    t.GetEntry(1)
    exp_sim_dn.append(t.limit*scale)
    t.GetEntry(3)
    exp_sim_up.append(t.limit*scale)

    cut = 'zpt100met50'
    f = TFile(output+"higgsCombine"+cut+".Asymptotic.mH"+str(m)+".root","READ")
    t = f.Get("limit")

    t.GetEntry(2)
    thisexp = t.limit*scale
    exp_sim1.append(thisexp)

print 'mass',mass
print 'exp',exp
print 'exp_sim',exp_sim
print 'exp_sim1',exp_sim1

v_mass = TVectorD(len(mass),mass)
v_zeros = TVectorD(len(zeros),zeros)
v_exp_p2 = TVectorD(len(exp_p2),exp_p2)
v_exp_p1 = TVectorD(len(exp_p1),exp_p1)
v_exp = TVectorD(len(exp),exp)
v_exp_m1 = TVectorD(len(exp_m1),exp_m1)
v_exp_m2 = TVectorD(len(exp_m2),exp_m2)

v_exp_sim = TVectorD(len(exp_sim),exp_sim)
v_exp_sim_up = TVectorD(len(exp_sim_up),exp_sim_up)
v_exp_sim_dn = TVectorD(len(exp_sim_dn),exp_sim_dn)

v_exp_sim1 = TVectorD(len(exp_sim1),exp_sim1)

c = TCanvas("c","c",800, 800)
c.SetLogy()
c.SetGridx()
c.SetGridy()

c.SetRightMargin(0.06)
c.SetLeftMargin(0.2)

dummy = TH1D("dummy","dummy", 1, 600,2000)
dummy.SetBinContent(1,0.0)
dummy.GetXaxis().SetTitle('m_{G} [GeV]')   
dummy.GetYaxis().SetTitle('#sigma(pp#rightarrowG)#timesBR(G#rightarrowZZ) [pb]')   
dummy.SetLineColor(0)
dummy.SetLineWidth(0)
dummy.SetFillColor(0)
dummy.SetMinimum(0.001)
dummy.SetMaximum(10.0)
dummy.GetXaxis().SetMoreLogLabels(kTRUE)
dummy.GetXaxis().SetNoExponent(kTRUE)
dummy.Draw()

gr_exp2 = TGraphAsymmErrors(v_mass,v_exp,v_zeros,v_zeros,v_exp_m2,v_exp_p2)
gr_exp2.SetLineColor(kYellow)
gr_exp2.SetFillColor(kYellow)
gr_exp2.Draw("e3same")

gr_exp1 = TGraphAsymmErrors(v_mass,v_exp,v_zeros,v_zeros,v_exp_m1,v_exp_p1)
gr_exp1.SetLineColor(kGreen)
gr_exp1.SetFillColor(kGreen)
gr_exp1.Draw("e3same")

gr_exp = TGraphAsymmErrors(v_mass,v_exp,v_zeros,v_zeros,v_zeros,v_zeros)
gr_exp.SetLineColor(1)
gr_exp.SetLineWidth(2)
gr_exp.SetLineStyle(2)
gr_exp.Draw("Lsame")

gr_exp_sim = TGraphAsymmErrors(v_mass,v_exp_sim,v_zeros,v_zeros,v_zeros,v_zeros)
gr_exp_sim.SetLineColor(6)
gr_exp_sim.SetLineWidth(2)
gr_exp_sim.SetLineStyle(2)
#gr_exp_sim.Draw("Lsame")

gr_exp_sim_up = TGraphAsymmErrors(v_mass,v_exp_sim_up,v_zeros,v_zeros,v_zeros,v_zeros)
gr_exp_sim_up.SetLineColor(6)
gr_exp_sim_up.SetLineWidth(2)
gr_exp_sim_up.SetLineStyle(1)
#gr_exp_sim_up.Draw("Lsame")

gr_exp_sim_dn = TGraphAsymmErrors(v_mass,v_exp_sim_dn,v_zeros,v_zeros,v_zeros,v_zeros)
gr_exp_sim_dn.SetLineColor(6)
gr_exp_sim_dn.SetLineWidth(2)
gr_exp_sim_dn.SetLineStyle(1)
#gr_exp_sim_dn.Draw("Lsame")

gr_exp_sim1 = TGraphAsymmErrors(v_mass,v_exp_sim1,v_zeros,v_zeros,v_zeros,v_zeros)
gr_exp_sim1.SetLineColor(2)
gr_exp_sim1.SetLineWidth(2)
gr_exp_sim1.SetLineStyle(2)
#gr_exp_sim1.Draw("Lsame")



latex2 = TLatex()
latex2.SetNDC()
latex2.SetTextSize(0.5*c.GetTopMargin())
latex2.SetTextFont(42)
latex2.SetTextAlign(31) # align right
latex2.DrawLatex(0.87, 0.95,"36.46 fb^{-1} (13 TeV)")
latex2.SetTextSize(0.7*c.GetTopMargin())
latex2.SetTextFont(62)
latex2.SetTextAlign(11) # align right
latex2.DrawLatex(0.25, 0.85, "CMS")
latex2.SetTextSize(0.5*c.GetTopMargin())
latex2.SetTextFont(52)
latex2.SetTextAlign(11)
latex2.DrawLatex(0.25, 0.8, "Preliminary")

legend = TLegend(.45,.70,.90,.90)
legend.AddEntry(gr_exp , "Expected 95% CL baseline", "l")
#legend.AddEntry(gr_exp_sim , "Expected 95% CL baseline sr/cr_{1-3} region sim fit", "l")
#legend.AddEntry(gr_exp_sim1 , "Expected 95% CL baseline sr/cr_{1} region sim fit", "l")
legend.AddEntry(gr_exp1 , "#pm 1#sigma", "f")
legend.AddEntry(gr_exp2 , "#pm 2#sigma", "f")
legend.SetShadowColor(0)
legend.SetFillColor(0)
legend.SetLineColor(0)            
legend.Draw("same")
                                                            
gPad.RedrawAxis()

c.SaveAs("xzz2l2nu_limit_13TeV_new.pdf")
c.SaveAs("xzz2l2nu_limit_13TeV_new.png")
