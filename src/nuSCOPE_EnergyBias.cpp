#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLatex.h"
#include <iostream>
#include <cmath>
#include <string>

// To compile: c++ nuSCOPE_EnergyBias.cpp `root-config --cflags --libs` -o nuscope_energybias.out

int main(int argc, char ** argv) 
{
    gStyle->SetOptStat(0);

    // ----------------------------------------------------------------------------------------------
    //                                   Open file and TTree 
    // ----------------------------------------------------------------------------------------------

    if (argc < 2) 
    {
        std::cout << "Usage: \n- ./nuscope_energybias.out \n- name of the nuSCOPE .root file\n - name of the tagging .root file" << std::endl;
        return 1;
    }

    // /eos/project-n/neutrino-generators/generatorOutput/GENIE/nuSCOPE/nuSCOPE_LAr_total/3_04_02/AR23_20i_00_000/flat_vec_AR23_20i_00_000_14_3_04_02_nuSCOPE_WC_total_0000.root

    TFile *file_NuSCOPE = TFile::Open(argv[1]);

    if (!file_NuSCOPE) 
    {
        printf("Error: could not open one of the files.\n");
        return 1;
    }

    TTree *tNuSCOPE = (TTree*) file_NuSCOPE->Get("FlatTree_VARS");

    if (!tNuSCOPE) 
    {
        printf("Error: could not find the TTree in one of the files.\n");
        return 1;
    }

    // ----------------------------------------------------------------------------------------------
    //                                    Nu_mu flux for nuSCOPE
    // ----------------------------------------------------------------------------------------------
    TH1F *hFluxNuSCOPE = (TH1F*) file_NuSCOPE->Get("FlatTree_FLUX");

    hFluxNuSCOPE->SetLineColor(kRed);

    TCanvas *c1 = new TCanvas("c1", "NuSCOPE flux", 800, 600);
    hFluxNuSCOPE->GetXaxis()->SetRangeUser(0, 20);
    hFluxNuSCOPE->SetTitle("nuSCOPE neutrino flux;E_{#nu} [GeV];Unosc #nu_{#mu}/m^{2}/POT/GeV");
    c1->SetTitle("nuSCOPE neutrino flux");
    hFluxNuSCOPE->Draw("hist");
    c1->SaveAs("../nuSCOPE_Plots/noTaggingEfficiency/nuSCOPE_flux.pdf");

    // -------------------------------------------------------------------------------------------------------------
    //                                         Histogram definitions 
    // -------------------------------------------------------------------------------------------------------------
    TH1F *hEnuNuSCOPE = new TH1F("hEnuNuSCOPE", "True neutrino energy comparison;E_{#nu}^{true} [GeV];Entries", 50, 0, 10);
    TH1F *hDeltaNuSCOPE = new TH1F("hDeltaNuSCOPE", "Comparison between true and reconstructed neutrino energy;E_{#nu}^{true} - E_{nu}^{reco} [GeV];Entries", 50, -0.5, 2.5);
    TH1F *hDeltaNuSCOPE_Weighted = new TH1F("hDeltaNuSCOPE_Weighted", "Weighted difference between true and reconstructed neutrino energy;(E_{#nu}^{true} - E_{nu}^{reco})/E_{#nu}^{true};Entries", 50, -1, 2);
    TH1F *hLepEnergyNuSCOPE = new TH1F("hLepEnergyNuSCOPE", "Lepton energy;E_{lep} [GeV];Entries", 50, 0, 10);

    TH1F *hNuSCOPE_CCQE  = new TH1F("hNuSCOPE_CCQE", "nuSCOPE events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 10);
    TH1F *hNuSCOPE_RES   = new TH1F("hNuSCOPE_RES", "nuSCOPE events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 10);
    TH1F *hNuSCOPE_2p2h  = new TH1F("hNuSCOPE_2p2h", "nuSCOPE events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 10);
    TH1F *hNuSCOPE_Other = new TH1F("hNuSCOPE_Other","nuSCOPE events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 10);

    TH1F *hNuSCOPE_Delta_CCQE = new TH1F("hNuSCOPE_Delta_CCQE", "nuSCOPE difference between true and reco #nu energy divided by channel;E_{#nu}^{true} - E_{nu}^{reco} [GeV];Entries", 100, 0, 1);
    TH1F *hNuSCOPE_Delta_RES  = new TH1F("hNuSCOPE_Delta_RES", "nuSCOPE RES DeltaE;#DeltaE [GeV];Entries", 200, 0, 1);
    TH1F *hNuSCOPE_Delta_2p2h = new TH1F("hNuSCOPE_Delta_2p2h","nuSCOPE 2p2h DeltaE;#DeltaE [GeV];Entries", 200, 0, 1);
    TH1F *hNuSCOPE_Delta_Other= new TH1F("hNuSCOPE_Delta_Other","nuSCOPE Other DeltaE;#DeltaE [GeV];Entries", 200, 0, 1);

    TH1F *hNuSCOPE_0pi0n = new TH1F("hNuSCOPE_0pi0n", "nuSCOPE 0pi0n energy bias;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 200, 0, 1);
    TH1F *hNuSCOPE_0piNn = new TH1F("hNuSCOPE_0piNn", "nuSCOPE 0piNn energy bias;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 200, 0, 1);
    TH1F *hNuSCOPE_Npi0n = new TH1F("hNuSCOPE_Npi0n", "nuSCOPE Npi0n energy bias;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 200, 0, 1);
    TH1F *hNuSCOPE_NpiNn = new TH1F("hNuSCOPE_NpiNn", "nuSCOPE NpiNn energy bias;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 200, 0, 1);

    // -------------------------------------------------------------------------------------------------------------
    //                                    Filling histograms from TTree
    // -------------------------------------------------------------------------------------------------------------
    // True neutrino energy
    tNuSCOPE -> Project("hEnuNuSCOPE", "Enu_true", "flagCCINC", "hist");
    tNuSCOPE -> Project("hLepEnergyNuSCOPE", "ELep", "flagCCINC", "hist");

    // Energy bias
    tNuSCOPE -> Project("hDeltaNuSCOPE", "Enu_true - (Erecoil_minerva+ELep)", "flagCCINC", "hist");

    // Weighted energy bias
    tNuSCOPE -> Project("hDeltaNuSCOPE_Weighted", "(Enu_true-(Erecoil_minerva+ELep))/Enu_true", "flagCCINC", "hist");

    // NuSCOPE Mode separation
    tNuSCOPE -> Project("hNuSCOPE_CCQE", "Enu_true", "flagCCINC * (Mode==1)", "hist"); // CCQE = Mode 1
    tNuSCOPE -> Project("hNuSCOPE_2p2h", "Enu_true", "flagCCINC * (Mode==2)", "hist"); // 2p2h = Mode 2 
    tNuSCOPE -> Project("hNuSCOPE_RES", "Enu_true", "flagCCINC && ( (Mode==11) || (Mode==12) || (Mode==13) )", "hist"); // RES = Mode 11, 12, 13
    tNuSCOPE -> Project("hNuSCOPE_Other", "Enu_true", "flagCCINC * (Mode!=1) * (Mode!=2) * (Mode!=11) * (Mode!=12) * (Mode!=13)", "hist"); // "Other"

    // NuSCOPE Energy bias by mode or topology
    tNuSCOPE -> Project("hNuSCOPE_Delta_CCQE", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC * (Mode==1)", "hist"); // CCQE = Mode 1
    tNuSCOPE -> Project("hNuSCOPE_Delta_2p2h", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC * (Mode==2)", "hist"); // 2p2h = Mode 2 
    tNuSCOPE -> Project("hNuSCOPE_Delta_RES", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC && ( (Mode==11) || (Mode==12) || (Mode==13) )", "hist"); // RES = Mode 11, 12, 13
    tNuSCOPE -> Project("hNuSCOPE_Delta_Other", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC * (Mode!=1) * (Mode!=2) * (Mode!=11) * (Mode!=12) * (Mode!=13)", "hist"); // "Other"
    tNuSCOPE -> Project("hNuSCOPE_0pi0n", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC && ((Sum$((abs(pdg)==2112))==0) && (Sum$((abs(pdg)==211))==0))", "hist"); // No pions (211), no neutrons (2112)
    tNuSCOPE -> Project("hNuSCOPE_0piNn", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC && ((Sum$((abs(pdg)==2112))>0) && (Sum$((abs(pdg)==211))==0))", "hist"); // No pions, N neutrons
    tNuSCOPE -> Project("hNuSCOPE_Npi0n", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC && ((Sum$((abs(pdg)==2112))==0) && (Sum$((abs(pdg)==211))>0))", "hist"); // N pions, no neutrons
    tNuSCOPE -> Project("hNuSCOPE_NpiNn", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC && ((Sum$((abs(pdg)==2112))>0) && (Sum$((abs(pdg)==211))>0))", "hist"); // N pions, N neutrons

    // ----------------------------------------------------------------------------------------------
    //                                       Plotting
    // ----------------------------------------------------------------------------------------------

    hLepEnergyNuSCOPE->SetLineColor(kRed);
    TCanvas *cLep = new TCanvas("cLep", "Lepton energy", 800, 600);
    hLepEnergyNuSCOPE->Draw("hist");
    cLep->SaveAs("../nuSCOPE_Plots/noTaggingEfficiency/lepton_energy.pdf");

    // True E_nu
    hEnuNuSCOPE->SetLineColor(kRed);

    TCanvas *c2 = new TCanvas("c2", "True neutrino energy comparison", 800, 600);
    hEnuNuSCOPE->Draw("hist");

    TLegend *leg2 = new TLegend(0.7, 0.75, 0.9, 0.9);
    leg2->AddEntry(hEnuNuSCOPE, "nuSCOPE", "l");
    leg2->Draw();

    c2->SaveAs("../nuSCOPE_Plots/noTaggingEfficiency/true_energy.pdf");

    // Energy bias (E_true - E_reco) 
    hDeltaNuSCOPE->SetLineColor(kRed);

    TCanvas *c3 = new TCanvas("c3", "True - reconstructed energy", 800, 600);
    hDeltaNuSCOPE->Draw("hist");

    TLegend *leg3 = new TLegend(0.7, 0.75, 0.9, 0.9);
    leg3->AddEntry(hDeltaNuSCOPE, "nuSCOPE", "l");
    leg3->Draw();

    c3->SaveAs("../nuSCOPE_Plots/noTaggingEfficiency/energy_bias.pdf");

    // Weighted energy bias [(E_true - E_reco)/E_true]
    hDeltaNuSCOPE_Weighted->SetLineColor(kRed);

    TCanvas *c3_2 = new TCanvas("c3_2", "Weighted True - reconstructed energy", 800, 600);
    hDeltaNuSCOPE_Weighted->Draw("hist");

    TLegend *leg3_2 = new TLegend(0.7, 0.75, 0.9, 0.9);
    leg3_2->AddEntry(hDeltaNuSCOPE_Weighted, "nuSCOPE", "l");
    leg3_2->Draw();

    c3_2->SaveAs("../nuSCOPE_Plots/noTaggingEfficiency/delta_energy_weighted.pdf");

    // Mode-separated NuSCOPE plot
    hNuSCOPE_CCQE->SetLineColor(kGreen+2);
    hNuSCOPE_RES->SetLineColor(kBlue);
    hNuSCOPE_2p2h->SetLineColor(kMagenta);
    hNuSCOPE_Other->SetLineColor(kOrange);

    TCanvas *c4 = new TCanvas("c4", "nuSCOPE events divided by channels", 800, 600);
    hNuSCOPE_Other->Draw("hist");
    hNuSCOPE_CCQE->Draw("hist same");
    hNuSCOPE_RES->Draw("hist same");
    hNuSCOPE_2p2h->Draw("hist same");

    TLegend *leg4 = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg4->AddEntry(hNuSCOPE_CCQE, "CCQE", "l");
    leg4->AddEntry(hNuSCOPE_RES, "RES", "l");
    leg4->AddEntry(hNuSCOPE_2p2h, "2p2h", "l");
    leg4->AddEntry(hNuSCOPE_Other, "Other", "l");
    leg4->Draw();

    c4->SaveAs("../nuSCOPE_Plots/noTaggingEfficiency/nuSCOPE_modes.pdf");

    // NuSCOPE Energy bias split by mode of interaction and topology
    TCanvas *c6 = new TCanvas("c6", "nuSCOPE DeltaE by channel", 1000, 800);
    c6->Divide(1, 2);

    // -------------------------
    // Pad 1: Interaction modes
    // -------------------------
    c6->cd(1);
    gPad->SetMargin(0.12, 0.05, 0.1, 0.08);

    hNuSCOPE_Delta_CCQE->SetLineColor(kRed);
    hNuSCOPE_Delta_RES->SetLineColor(kBlue);
    hNuSCOPE_Delta_2p2h->SetLineColor(kGreen+2);
    hNuSCOPE_Delta_Other->SetLineColor(kMagenta+1);

    hNuSCOPE_Delta_CCQE->SetLineWidth(1);
    hNuSCOPE_Delta_RES->SetLineWidth(1);
    hNuSCOPE_Delta_2p2h->SetLineWidth(1);
    hNuSCOPE_Delta_Other->SetLineWidth(1);

    hNuSCOPE_Delta_CCQE->GetXaxis()->SetTitle("E_{true} - E_{reco} [GeV]");
    hNuSCOPE_Delta_CCQE->GetYaxis()->SetTitle("Events");
    hNuSCOPE_Delta_CCQE->GetXaxis()->SetTitleSize(0.05);
    hNuSCOPE_Delta_CCQE->GetYaxis()->SetTitleSize(0.05);
    hNuSCOPE_Delta_CCQE->GetYaxis()->SetTitleOffset(1.2);

    hNuSCOPE_Delta_CCQE->SetTitle("");
    hNuSCOPE_Delta_CCQE->Draw("hist");
    hNuSCOPE_Delta_RES->Draw("hist same");
    hNuSCOPE_Delta_2p2h->Draw("hist same");
    hNuSCOPE_Delta_Other->Draw("hist same");

    TLegend *leg_modes = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg_modes->SetTextSize(0.035);
    leg_modes->SetBorderSize(0);
    leg_modes->SetFillStyle(0);
    leg_modes->AddEntry(hNuSCOPE_Delta_CCQE, "CCQE", "l");
    leg_modes->AddEntry(hNuSCOPE_Delta_RES, "RES", "l");
    leg_modes->AddEntry(hNuSCOPE_Delta_2p2h, "2p2h", "l");
    leg_modes->AddEntry(hNuSCOPE_Delta_Other, "Other", "l");
    leg_modes->Draw();

    TLatex label1;
    label1.SetNDC();
    label1.SetTextSize(0.04);
    label1.DrawLatex(0.15, 0.93, "nuSCOPE: Energy bias by interaction mode");

    // ---------------------------------
    //   Pad 2: Final-state topology
    // ---------------------------------
    c6->cd(2);
    gPad->SetMargin(0.12, 0.05, 0.15, 0.08);

    hNuSCOPE_0pi0n->SetLineColor(kOrange+7);
    hNuSCOPE_0piNn->SetLineColor(kCyan+1);
    hNuSCOPE_Npi0n->SetLineColor(kViolet-6);
    hNuSCOPE_NpiNn->SetLineColor(kTeal+3);

    hNuSCOPE_0pi0n->SetLineWidth(1);
    hNuSCOPE_0piNn->SetLineWidth(1);
    hNuSCOPE_Npi0n->SetLineWidth(1);
    hNuSCOPE_NpiNn->SetLineWidth(1);

    hNuSCOPE_0pi0n->GetXaxis()->SetTitle("E_{true} - E_{reco} [GeV]");
    hNuSCOPE_0pi0n->GetYaxis()->SetTitle("Events");
    hNuSCOPE_0pi0n->GetXaxis()->SetTitleSize(0.05);
    hNuSCOPE_0pi0n->GetYaxis()->SetTitleSize(0.05);
    hNuSCOPE_0pi0n->GetYaxis()->SetTitleOffset(1.2);

    hNuSCOPE_0pi0n->SetTitle("");
    hNuSCOPE_0pi0n->Draw("hist");
    hNuSCOPE_0piNn->Draw("hist same");
    hNuSCOPE_Npi0n->Draw("hist same");
    hNuSCOPE_NpiNn->Draw("hist same");

    TLegend *leg_topo = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg_topo->SetTextSize(0.035);
    leg_topo->SetBorderSize(0);
    leg_topo->SetFillStyle(0);
    leg_topo->AddEntry(hNuSCOPE_0pi0n, "0 #pi, 0 neutrons", "l");
    leg_topo->AddEntry(hNuSCOPE_0piNn, "0 #pi, N neutrons", "l");
    leg_topo->AddEntry(hNuSCOPE_Npi0n, "N #pi, 0 neutrons", "l");
    leg_topo->AddEntry(hNuSCOPE_NpiNn, "N #pi, N neutrons", "l");
    leg_topo->Draw();

    TLatex label2;
    label2.SetNDC();
    label2.SetTextSize(0.04);
    label2.DrawLatex(0.15, 0.93, "nuSCOPE: Energy bias by final-state topology");

    c6->SaveAs("../nuSCOPE_Plots/noTaggingEfficiency/nuSCOPE_deltaE_modes_split.pdf");

    // Close files
    file_NuSCOPE->Close();

    return 0;
}