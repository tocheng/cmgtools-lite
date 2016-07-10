



useful alias:

root [5] tree->SetAlias("newMt_B",
"TMath::Sqrt(llnunu_l1_mass*llnunu_l1_mass+91.188*91.188+2.0*(et1*et2-llnunu_l1_pt*llnunu_l2_pt*cos(abs(llnunu_deltaPhi)+TMath::Pi()*3/4)))")
(Bool_t) true
root [6] tree->SetAlias("newMt_D",
"TMath::Sqrt(llnunu_l1_mass*llnunu_l1_mass+91.188*91.188+2.0*(et1*et2-llnunu_l1_pt*llnunu_l2_pt*cos(abs(llnunu_deltaPhi)+TMath::Pi()/2)))")
(Bool_t) true
root [7] tree->SetAlias("newMt_C",
"TMath::Sqrt(llnunu_l1_mass*llnunu_l1_mass+91.188*91.188+2.0*(et1*et2-llnunu_l1_pt*llnunu_l2_pt*cos(abs(llnunu_deltaPhi)+TMath::Pi()/4)))")
(Bool_t) true
root [8] tree->SetAlias("newMt_A",
"TMath::Sqrt(llnunu_l1_mass*llnunu_l1_mass+91.188*91.188+2.0*(et1*et2-llnunu_l1_pt*llnunu_l2_pt*cos(abs(llnunu_deltaPhi))))")
(Bool_t) true
root [9] tree->SetAlias("regA",
"(cos(llnunu_deltaPhi)/abs(cos(llnunu_deltaPhi))<0&&abs(abs(llnunu_deltaPhi)-TMath::Pi()/2)>TMath::Pi()/4)")
(Bool_t) true
root [10] tree->SetAlias("regC",
"(cos(llnunu_deltaPhi)/abs(cos(llnunu_deltaPhi))<0&&abs(abs(llnunu_deltaPhi)-TMath::Pi()/2)<TMath::Pi()/4)")
(Bool_t) true
root [11] tree->SetAlias("regD",
"(cos(llnunu_deltaPhi)/abs(cos(llnunu_deltaPhi))>0&&abs(abs(llnunu_deltaPhi)-TMath::Pi()/2)<TMath::Pi()/4)")
(Bool_t) true
root [12] tree->SetAlias("regB",
"(cos(llnunu_deltaPhi)/abs(cos(llnunu_deltaPhi))>0&&abs(abs(llnunu_deltaPhi)-TMath::Pi()/2)>TMath::Pi()/4)")



tree->SetAlias("NewdeltaR1", "sqrt(pow(jet_corr_eta-llnunu_l1_l1_eta,2)+pow(TVector2::Phi_mpi_pi(jet_corr_phi-llnunu_l1_l1_phi),2))")
tree->SetAlias("NewdeltaR2", "sqrt(pow(jet_corr_eta-llnunu_l1_l2_eta,2)+pow(TVector2::Phi_mpi_pi(jet_corr_phi-llnunu_l1_l2_phi),2))")

tree->SetAlias("new_ut_soft_para","-Sum$((jet_corr_pt*cos(jet_corr_phi-llnunu_l1_phi))*(NewdeltaR1>0.4&&NewdeltaR2>0.4&&jet_corr_pt>0))-llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi)-llnunu_l1_pt")
tree->SetAlias("new_ut_soft_perp","-Sum$((jet_corr_pt*sin(jet_corr_phi-llnunu_l1_phi))*(NewdeltaR1>0.4&&NewdeltaR2>0.4&&jet_corr_pt>0))-llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi)")
tree->SetAlias("new_ut_soft","sqrt(new_ut_soft_perp*new_ut_soft_perp+new_ut_soft_para*new_ut_soft_para)")

tree->SetAlias("ut_hard_para",
"Sum$((jet_pt*cos(jet_phi-llnunu_l1_phi))*(deltaR1>0.4&&deltaR2>0.4&&jet_pt>0))")
tree->SetAlias("ut_hard_perp",
"Sum$((jet_pt*sin(jet_phi-llnunu_l1_phi))*(deltaR1>0.4&&deltaR2>0.4&&jet_pt>0))")
tree->SetAlias("ut_hard",
"sqrt(ut_hard_perp*ut_hard_perp+ut_hard_para*ut_hard_para)")

tree->SetAlias("drMatch","sqrt(pow(lep_eta[Iteration$]-genLep_eta,2)+pow(TVector2::Phi_mpi_pi(lep_phi[Iteration$]-genLep_phi),2))
tree->SetAlias("deltaR1", "sqrt(pow(jet_eta-llnunu_l1_l1_eta,2)+pow(TVector2::Phi_mpi_pi(jet_phi-llnunu_l1_l1_phi),2))")
tree->SetAlias("deltaR2", "sqrt(pow(jet_eta-llnunu_l1_l2_eta,2)+pow(TVector2::Phi_mpi_pi(jet_phi-llnunu_l1_l2_phi),2))")

tree->SetAlias("ut_soft_para","-Sum$((jet_pt*cos(jet_phi-llnunu_l1_phi))*(deltaR1>0.4&&deltaR2>0.4&&jet_pt>0))-llnunu_old_l2_pt*cos(llnunu_old_l2_phi-llnunu_l1_phi)-llnunu_l1_pt")
tree->SetAlias("ut_soft_perp","-Sum$((jet_pt*sin(jet_phi-llnunu_l1_phi))*(deltaR1>0.4&&deltaR2>0.4&&jet_pt>0))-llnunu_old_l2_pt*sin(llnunu_old_l2_phi-llnunu_l1_phi)")
tree->SetAlias("ut_soft","sqrt(ut_soft_perp*ut_soft_perp+ut_soft_para*ut_soft_para)")

tree->SetAlias("ut_soft_para", "-Sum$((jet_pt*cos(jet_phi-llnunu_l1_phi))*(deltaR1>0.4&&deltaR2>0.4&&jet_pt>0))-llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi)-llnunu_l1_pt")
tree->SetAlias("ut_soft_perp", "-Sum$((jet_pt*sin(jet_phi-llnunu_l1_phi))*(deltaR1>0.4&&deltaR2>0.4&&jet_pt>0))-llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi)")
tree->SetAlias("ut_soft", "sqrt(ut_soft_perp*ut_soft_perp+ut_soft_para*ut_soft_para)")

tree->SetAlias("ut_hard_para", "Sum$((jet_pt*cos(jet_phi-llnunu_l1_phi))*(deltaR1>0.4&&deltaR2>0.4&&jet_pt>0))")
tree->SetAlias("ut_hard_perp", "Sum$((jet_pt*sin(jet_phi-llnunu_l1_phi))*(deltaR1>0.4&&deltaR2>0.4&&jet_pt>0))")
tree->SetAlias("ut_hard", "sqrt(ut_hard_perp*ut_hard_perp+ut_hard_para*ut_hard_para)")

root [135] tree->Draw("abs(ut_soft_para)>>h_ut_soft_para(1000,0,1000)","", "")
(Long64_t) 1928520
root [136] tree->Draw("abs(ut_soft_perp)>>h_ut_soft_perp(1000,0,1000)","", "")
(Long64_t) 1928520
root [137] h_ut_soft_para->GetMean()
(Double_t) 35.8757
root [138] h_ut_soft_perp->GetMean()
(Double_t) 14.2911


