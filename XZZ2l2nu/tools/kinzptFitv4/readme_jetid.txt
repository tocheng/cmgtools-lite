

tree->Scan("evt:met_para:met_perp:met:llnunu_l1_pt:njet:jet_pt[]:njet_corr:jet_corr_pt[0]:jet_pt[jet_corr_sel_idx[0]]:jet_corr_reso[0]:llnunu_l1_mass:genZ_pt","evt==62948089")

tree->Scan("evt:met_para:llnunu_l1_pt:njet:jet_pt[]:jet_para_old:jet_perp_old:jet_eta[]:njet_corr:jet_corr_pt[0]:jet_pt[jet_corr_sel_idx[0]]:jet_corr_reso[0]","evt==62948089")
tree->Scan("evt:met_para:llnunu_l1_pt:njet:jet_pt[]:jet_para_old:jet_perp_old:jet_eta[]:njet_corr:jet_para:jet_pt[jet_corr_sel_idx[0]]:jet_corr_reso[0]","evt==62948089")

tree->Scan("evt:met_para:llnunu_l1_pt:llnunu_l1_l1_pt:llnunu_l1_l2_pt:njet:jet_pt[]:jet_para_old:jet_perp_old:jet_eta[]:njet_corr:jet_para:jet_pt[jet_corr_sel_idx[0]]:jet_corr_reso[0]","evt==62948089")

tree->SetAlias("l1_para","llnunu_l1_l1_pt*cos(llnunu_l1_l1_phi-llnunu_l1_phi)");
tree->SetAlias("l2_para","llnunu_l1_l2_pt*cos(llnunu_l1_l2_phi-llnunu_l1_phi)");

tree->SetAlias("zpt", "llnunu_l1_pt");
  tree->SetAlias("jet_corr_para","(jet_corr_pt[]*cos(jet_corr_phi[]-llnunu_l1_phi[0]))");
  tree->SetAlias("jet_corr_perp","(jet_corr_pt[]*sin(jet_corr_phi[]-llnunu_l1_phi[0]))");

tree->Scan("evt:met_para:zpt:l1_para:l2_para:njet:jet_pt:jet_para_old:jet_perp_old:jet_eta[]:njet_corr:jet_para:jet_pt[jet_corr_sel_idx[0]]:jet_corr_reso[0]","evt==62948089")
tree->Scan("evt:met_para:zpt:l1_para:l2_para:njet:jet_pt:jet_para:jet_perp:jet_eta[]:njet_corr:jet_corr_para:jet_pt[jet_corr_sel_idx[0]]:jet_corr_reso[0]","evt==62948089")



  tree->SetAlias("ut_hard_para","(ut_hard_pt[0]*cos(ut_hard_phi[0]-llnunu_l1_phi[0]))");
  tree->SetAlias("ut_hard_perp","(ut_hard_pt[0]*sin(ut_hard_phi[0]-llnunu_l1_phi[0]))");
  tree->SetAlias("ut_hard_para_old","(ut_hard_pt_old[0]*cos(ut_hard_phi_old[0]-llnunu_l1_phi[0]))");
  tree->SetAlias("ut_hard_perp_old","(ut_hard_pt_old[0]*sin(ut_hard_phi_old[0]-llnunu_l1_phi[0]))");

  tree->SetAlias("met_para","(llnunu_l2_pt[0]*cos(llnunu_l2_phi[0]-llnunu_l1_phi[0]))");
  tree->SetAlias("met_perp","(llnunu_l2_pt[0]*sin(llnunu_l2_phi[0]-llnunu_l1_phi[0]))");
  tree->SetAlias("met_para_old","(llnunu_old_l2_pt[0]*cos(llnunu_old_l2_phi[0]-llnunu_l1_phi[0]))");
  tree->SetAlias("met_perp_old","(llnunu_old_l2_pt[0]*sin(llnunu_old_l2_phi[0]-llnunu_l1_phi[0]))");

  tree->SetAlias("jetTightId","(jet_id[]>=3)");
  tree->SetAlias("jetLepVeto","(jet_chargedEmEnergyFraction[]<0.8&&jet_muonEnergyFraction[]<0.8)");
  tree->SetAlias("jet_para_raw","(jet_rawPt[]*cos(jet_phi[]-llnunu_l1_phi[0]))");
  tree->SetAlias("jet_perp_raw","(jet_rawPt[]*sin(jet_phi[]-llnunu_l1_phi[0]))");

  tree->SetAlias("ut_hard_para_raw","Sum$((jet_rawPt[]*cos(jet_phi[]-llnunu_l1_phi[0]))*(jetTightId&&jetLepVeto))");
  tree->SetAlias("ut_hard_perp_raw","Sum$((jet_rawPt[]*sin(jet_phi[]-llnunu_l1_phi[0]))*(jetTightId&&jetLepVeto))");
  tree->SetAlias("met_para_raw","(llnunu_l2_rawPt[0]*cos(llnunu_l2_rawPhi[0]-llnunu_l1_phi[0]))");
  tree->SetAlias("met_perp_raw","(llnunu_l2_rawPt[0]*sin(llnunu_l2_rawPhi[0]-llnunu_l1_phi[0]))");
  tree->SetAlias("ut_soft_para_raw","-met_para_raw-ut_hard_para_raw-llnunu_l1_pt[0]");
  tree->SetAlias("ut_soft_perp_raw","-met_perp_raw-ut_hard_perp_raw");

tree->Draw("ut_soft_para_raw")
tree->Draw("ut_soft_para_raw>>h_ut_soft_para_raw(200,-200,200)")
tree->Draw("ut_hard_para_raw>>h_ut_hard_para_raw(200,-200,200)")
tree->Draw("met_para_raw>>h_met_para_raw(200,-200,200)")
tree->Draw("met_para_old>>h_met_para_old(200,-200,200)")
tree->Draw("met_para>>h_met_para_new(200,-200,200)")
h_ut_soft_para_raw->SetLineColor(2)
h_ut_hard_para_raw->SetLineColor(4)
h_met_para_raw->SetLineColor(8)
h_met_para_old->SetLineColor(6)
h_met_para_new->SetLineColor(9)
h_met_para_raw->Draw("")
h_ut_hard_para_raw->Draw("same")
h_ut_soft_para_raw->Draw("same")


  tree->SetAlias("jetTightId","(jet_id[]>=3)");
  tree->SetAlias("jetLepVeto","(jet_chargedEmEnergyFraction[]<0.9&&jet_muonEnergyFraction[]<0.8)");
  tree->SetAlias("jet_para","(jet_pt[]*cos(jet_phi[]-llnunu_l1_phi[0]))");
  tree->SetAlias("jet_perp","(jet_pt[]*sin(jet_phi[]-llnunu_l1_phi[0]))");
tree->SetAlias("jetTightCHEF", "(jet_chargedHadronEnergyFraction[]>0.1)");
tree->SetAlias("jetTightNHEF", "(jet_neutralHadronEnergyFraction[]>0.01)");


tree->SetAlias("jetTightId","(jet_id[]>=3)");
tree->SetAlias("jetLepVeto","(jet_chargedEmEnergyFraction[]<0.9&&jet_muonEnergyFraction[]<0.8)");
tree->SetAlias("jet_para","(jet_pt[]*cos(jet_phi[]-llnunu_l1_phi[0]))");
tree->SetAlias("jet_perp","(jet_pt[]*sin(jet_phi[]-llnunu_l1_phi[0]))");

tree->SetAlias("jetCorrTightId","(jet_corr_id[]>=3)");
tree->SetAlias("jetCorrLepVeto","(jet_corr_chargedEmEnergyFraction[]<0.9&&jet_corr_muonEnergyFraction[]<0.8)");
tree->SetAlias("jet_corr_para","(jet_corr_pt[]*cos(jet_corr_phi[]-llnunu_l1_phi[0]))");
tree->SetAlias("jet_corr_perp","(jet_corr_pt[]*sin(jet_corr_phi[]-llnunu_l1_phi[0]))");

tree->SetAlias("jet_min_dR","Min$(sqrt(pow(jet_corr_eta[Iteration$]-jet_eta[],2)+pow(TVector2::Phi_mpi_pi(jet_corr_phi[Iteration$]-jet_phi[]),2)))")

tree->SetAlias("deltaR1","sqrt(pow(jet_eta-llnunu_l1_l1_eta,2)+pow(TVector2::Phi_mpi_pi(jet_phi-llnunu_l1_l1_phi),2))")
tree->SetAlias("deltaR2","sqrt(pow(jet_eta-llnunu_l1_l2_eta,2)+pow(TVector2::Phi_mpi_pi(jet_phi-llnunu_l1_l2_phi),2))")

tree->SetAlias("NHF", "jet_neutralHadronEnergyFraction");
tree->SetAlias("NEMF", "jet_neutralEmEnergyFraction");
tree->SetAlias("CHF", "jet_chargedHadronEnergyFraction");
tree->SetAlias("MUF","jet_muonEnergyFraction");
tree->SetAlias("CEMF","jet_chargedEmEnergyFraction");
tree->SetAlias("NC", "(jet_chargedMultiplicity+jet_neutralMultiplicity)");
tree->SetAlias("NM","jet_neutralMultiplicity");
tree->SetAlias("CM","jet_chargedMultiplicity");
tree->SetAlias("CHM","jet_chargedHadronMultiplicity");
tree->SetAlias("jet_corr_para","(jet_corr_pt[]*cos(jet_corr_phi[]-llnunu_l1_phi[0]))");
tree->SetAlias("jet_corr_perp","(jet_corr_pt[]*sin(jet_corr_phi[]-llnunu_l1_phi[0]))");
tree->SetAlias("jet_para","(jet_pt[]*cos(jet_phi[]-llnunu_l1_phi[0]))");
tree->SetAlias("jet_perp","(jet_pt[]*sin(jet_phi[]-llnunu_l1_phi[0]))");
tree->SetAlias("l1_para","llnunu_l1_l1_pt*cos(llnunu_l1_l1_phi-llnunu_l1_phi)");
tree->SetAlias("l2_para","llnunu_l1_l2_pt*cos(llnunu_l1_l2_phi-llnunu_l1_phi)");
tree->SetAlias("l1_gen_para","llnunu_l1_l1_gen_pt*cos(llnunu_l1_l1_gen_phi-llnunu_l1_phi)");
tree->SetAlias("l2_gen_para","llnunu_l1_l2_gen_pt*cos(llnunu_l1_l2_gen_phi-llnunu_l1_phi)");

tree->SetAlias("metFilter","(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_CSCTightHalo2015Filter&&Flag_eeBadScFilter)");

tree->SetAlias("zpt", "llnunu_l1_pt");
  tree->SetAlias("jet_corr_para","(jet_corr_pt[]*cos(jet_corr_phi[]-llnunu_l1_phi[0]))");
  tree->SetAlias("jet_corr_perp","(jet_corr_pt[]*sin(jet_corr_phi[]-llnunu_l1_phi[0]))");
tree->SetAlias("jet_corr_para_old","(jet_pt[jet_corr_sel_idx]*cos(jet_phi[jet_corr_sel_idx]-llnunu_l1_phi[0]))");


tree->Scan("evt:met_para:zpt:l1_para:jet_pt:jet_para:jet_perp:jet_eta[]:NHF:CHF:MUF:CEMF:NEMF:CHM:CM:NM:NC:jet_corr_para","evt==62948089")
tree->Scan("evt:met_para:zpt:l1_para:jet_pt:jet_para:jet_perp:jet_eta[]:NHF:CHF:MUF:CEMF:NEMF:CHM:CM:NM:NC","evt==62948089")
tree->Scan("evt:met_para:zpt:l1_para:jet_pt:jet_para:jet_perp:jet_eta[]:NHF:CHF:MUF:CEMF:NEMF:CHM:CM:NM:NC:jet_corr_para","evt==62948089")
tree->Scan("evt:met_para:zpt:l1_para:jet_id:jet_pt:jet_para:jet_perp:jet_eta[]:NHF:CHF:MUF:CEMF:NEMF:CHM:CM:NM:NC:njet_corr:jet_corr_para","evt==62948089")

tree->Scan("evt:met_para_old:met_para:zpt:jet_id:jet_pt:jet_para:jet_perp:jet_eta[]:NHF:CHF:MUF:CEMF:NEMF:CM:NM:njet_corr:jet_corr_sel_idx","evt==16299580")

# bad jet 1T = 40% muon + 60% NHF
tree->Scan("evt:met_para_old:met_para:zpt:l1_para:l2_para:jet_id:jet_pt:jet_para:jet_perp:jet_eta[]:jet_res:NHF:CHF:MUF:CEMF:NEMF:CM:NM:njet_corr:jet_corr_sel_idx","evt==8701027")


62859

jet_chargedHadronEnergyFraction
jet_neutralHadronEnergyFraction
jet_neutralEmEnergyFraction
jet_muonEnergyFraction
jet_chargedEmEnergyFraction 
jet_chargedHadronMultiplicity
jet_chargedMultiplicity
jet_neutralMultiplicity
