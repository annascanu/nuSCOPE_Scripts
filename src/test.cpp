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

// To compile: c++ test.cpp `root-config --cflags --libs` -o test.out

int main(int argc, char ** argv) 
{
    gStyle->SetOptStat(0);

    // ----------------------------------------------------------------------------------------------
    //                                   Open files and TTrees
    // ----------------------------------------------------------------------------------------------

    if (argc < 3) 
    {
        std::cout << "Usage: \n- ./plots.out \n- name of the DUNE .root file \n- name of the T2K .root file" << std::endl;
        return 1;
    }

    //TFile *file_DUNE = TFile::Open("/Users/anna/Developing/PhD/nuSCOPE_Test/nuSCOPE_Scripts/DUNE_T2K_ROOT_Trees/flat_Valencia_13815.root");
    //TFile *file_T2K  = TFile::Open("/Users/anna/Developing/PhD/nuSCOPE_Test/nuSCOPE_Scripts/DUNE_T2K_ROOT_Trees/flat_Valencia_2382.root");

    TFile *file_DUNE = TFile::Open(argv[1]);
    TFile *file_T2K  = TFile::Open(argv[2]);

    if (!file_DUNE || !file_T2K) 
    {
        printf("Error: could not open files.\n");
        return 1;
    }

    TTree *tDUNE = (TTree*) file_DUNE->Get("FlatTree_VARS");
    TTree *tT2K  = (TTree*) file_T2K->Get("FlatTree_VARS");

    if (!tDUNE || !tT2K) 
    {
        printf("Error: could not find the TTree in one of the files.\n");
        return 1;
    }

    // ----------------------------------------------------------------------------------------------
    //                          Nu_mu flux comparison for DUNE and T2K
    // ----------------------------------------------------------------------------------------------
    TH1F *hFluxDUNE = (TH1F*) file_DUNE->Get("FlatTree_FLUX");
    TH1F *hFluxT2K  = (TH1F*) file_T2K->Get("FlatTree_FLUX");

    hFluxDUNE->SetLineColor(kRed);
    hFluxT2K->SetLineColor(kBlue);

    TCanvas *c1 = new TCanvas("c1", "DUNE flux", 800, 600);
    hFluxDUNE->GetXaxis()->SetRangeUser(0, 20);
    hFluxDUNE->SetTitle("DUNE neutrino flux;E_{#nu} [GeV];Unosc #nu_{#mu}/m^{2}/POT/GeV");
    c1->SetTitle("DUNE neutrino flux");
    hFluxDUNE->Draw("hist");
    c1->SaveAs("../Test_new_plots/DUNE_flux.pdf");

    TCanvas *c1_T2K = new TCanvas("c1_T2K", "T2K flux", 800, 600);
    hFluxT2K->GetXaxis()->SetRangeUser(0, 9);
    hFluxT2K->SetTitle("T2K neutrino flux;E_{#nu} [GeV];Unosc #nu_{#mu}/m^{2}/POT/GeV");
    c1_T2K->SetTitle("T2K neutrino flux");
    hFluxT2K->Draw("hist");
    c1_T2K->SaveAs("../Test_new_plots/T2K_flux.pdf");

    // -------------------------------------------------------------------------------------------------------------
    //                                   Histogram definitions 
    // -------------------------------------------------------------------------------------------------------------
    TH1F *hEnuDUNE = new TH1F("hEnuDUNE", "True neutrino energy comparison;E_{#nu}^{true} [GeV];Entries", 50, 0, 10);
    TH1F *hEnuT2K  = new TH1F("hEnuT2K", "True neutrino energy comparison;E_{#nu}^{true} [GeV];Entries", 50, 0, 10);
    TH1F *hDeltaDUNE = new TH1F("hDeltaDUNE", "Comparison between true and reconstructed neutrino energy;E_{#nu}^{true} - E_{nu}^{reco} [GeV];Entries", 50, -0.5, 2.5);
    TH1F *hDeltaT2K  = new TH1F("hDeltaT2K", "Comparison between true and reconstructed neutrino energy;E_{#nu}^{true} - E_{nu}^{reco} [GeV];Entries", 50, -0.5, 2.5);
    TH1F *hDeltaDUNE_Weighted = new TH1F("hDeltaDUNE_Weighted", "Weighted difference between true and reconstructed neutrino energy;(E_{#nu}^{true} - E_{nu}^{reco})/E_{#nu}^{true};Entries", 50, -1, 2);
    TH1F *hDeltaT2K_Weighted  = new TH1F("hDeltaT2K_Weighted", "Weighted difference between true and reconstructed neutrino energy;(E_{#nu}^{true} - E_{nu}^{reco})/E_{#nu}^{true};Entries", 50, -1, 2);

    TH1F *hDUNE_CCQE  = new TH1F("hDUNE_CCQE", "DUNE events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 10);
    TH1F *hDUNE_RES   = new TH1F("hDUNE_RES", "DUNE events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 10);
    TH1F *hDUNE_2p2h  = new TH1F("hDUNE_2p2h", "DUNE events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 10);
    TH1F *hDUNE_Other = new TH1F("hDUNE_Other","DUNE events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 10);

    TH1F *hT2K_CCQE   = new TH1F("hT2K_CCQE", "T2K events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 12);
    TH1F *hT2K_RES    = new TH1F("hT2K_RES", "T2K events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 12);
    TH1F *hT2K_2p2h   = new TH1F("hT2K_2p2h", "T2K events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 12);
    TH1F *hT2K_Other  = new TH1F("hT2K_Other","T2K events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 12);

    TH1F *hDUNE_Delta_CCQE = new TH1F("hDUNE_Delta_CCQE", "DUNE difference between true and reco #nu energy divided by channel;E_{#nu}^{true} - E_{nu}^{reco} [GeV];Entries", 100, 0, 1);
    TH1F *hDUNE_Delta_RES  = new TH1F("hDUNE_Delta_RES", "DUNE RES DeltaE;#DeltaE [GeV];Entries", 200, 0, 1);
    TH1F *hDUNE_Delta_2p2h = new TH1F("hDUNE_Delta_2p2h","DUNE 2p2h DeltaE;#DeltaE [GeV];Entries", 200, 0, 1);
    TH1F *hDUNE_Delta_Other= new TH1F("hDUNE_Delta_Other","DUNE Other DeltaE;#DeltaE [GeV];Entries", 200, 0, 1);

    TH1F *hT2K_Delta_CCQE  = new TH1F("hT2K_Delta_CCQE","T2K CCQE DeltaE;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 50, -0.5, 2.5);
    TH1F *hT2K_Delta_RES   = new TH1F("hT2K_Delta_RES","T2K RES DeltaE;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 50, -0.5, 2.5);
    TH1F *hT2K_Delta_2p2h  = new TH1F("hT2K_Delta_2p2h","T2K 2p2h DeltaE;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 50, -0.5, 2.5);
    TH1F *hT2K_Delta_Other = new TH1F("hT2K_Delta_Other","T2K Other DeltaE;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 50, -0.5, 2.5);

    TH1F *hDUNE_0pi0n = new TH1F("hDUNE_0pi0n", "DUNE 0pi0n energy bias;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 200, 0, 1);
    TH1F *hDUNE_0piNn = new TH1F("hDUNE_0piNn", "DUNE 0piNn energy bias;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 200, 0, 1);
    TH1F *hDUNE_Npi0n = new TH1F("hDUNE_Npi0n", "DUNE Npi0n energy bias;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 200, 0, 1);
    TH1F *hDUNE_NpiNn = new TH1F("hDUNE_NpiNn", "DUNE NpiNn energy bias;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 200, 0, 1);

    TH1F *hT2K_0pi0n = new TH1F("hT2K_0pi0n", "DUNE 0pi0n energy bias;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 50, -0.5, 2.5);
    TH1F *hT2K_0piNn = new TH1F("hT2K_0piNn", "DUNE 0piNn energy bias;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 50, -0.5, 2.5);
    TH1F *hT2K_Npi0n = new TH1F("hT2K_Npi0n", "DUNE Npi0n energy bias;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 50, -0.5, 2.5);
    TH1F *hT2K_NpiNn = new TH1F("hT2K_NpiNn", "DUNE NpiNn energy bias;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries", 50, -0.5, 2.5);

    // Testing another way to process trees. Example:
    //TH1D* h_test = new TH1D("h_test_name");
    //tree->Project("h_test_name", "Enu_QE/Enu_true", "flagCCINC*(Mode==2)", "hist")

    // True neutrino energy
    tDUNE  -> Project("hEnuDUNE", "Enu_true", "flagCCINC", "hist");
    tT2K   -> Project("hEnuT2K", "Enu_true", "flagCC0pi", "hist");

    // Energy bias
    tDUNE  -> Project("hDeltaDUNE", "Enu_true - (Erecoil_minerva+ELep)", "flagCCINC", "hist");
    tT2K   -> Project("hDeltaT2K", "Enu_true - Enu_QE", "flagCC0pi", "hist");

    // Weighted energy bias
    tDUNE  -> Project("hDeltaDUNE_Weighted", "(Enu_true-(Erecoil_minerva+ELep))/Enu_true", "flagCCINC", "hist");
    tT2K   -> Project("hDeltaT2K_Weighted", "(Enu_true-Enu_QE)/Enu_true", "flagCC0pi", "hist");

    // DUNE Mode separation
    tDUNE  -> Project("hDUNE_CCQE", "Enu_true", "flagCCINC * (Mode==1)", "hist"); // CCQE = Mode 1
    tDUNE  -> Project("hDUNE_2p2h", "Enu_true", "flagCCINC * (Mode==2)", "hist"); // 2p2h = Mode 2 
    tDUNE  -> Project("hDUNE_RES", "Enu_true", "flagCCINC && ( (Mode==11) || (Mode==12) || (Mode==13) )", "hist"); // RES = Mode 11, 12, 13
    tDUNE  -> Project("hDUNE_Other", "Enu_true", "flagCCINC * (Mode!=1) * (Mode!=2) * (Mode!=11) * (Mode!=12) * (Mode!=13)", "hist"); // "Other"

    // T2K Mode separation
    tT2K   -> Project("hT2K_CCQE", "Enu_true", "flagCC0pi * (Mode==1)", "hist");
    tT2K   -> Project("hT2K_2p2h", "Enu_true", "flagCC0pi * (Mode==2)", "hist");
    tT2K   -> Project("hT2K_RES", "Enu_true", "flagCC0pi && ( (Mode==11) || (Mode==12) || (Mode==13) )", "hist");
    tT2K   -> Project("hT2K_Other", "Enu_true", "flagCC0pi * (Mode!=1) * (Mode!=2) * (Mode!=11) * (Mode!=12) * (Mode!=13)", "hist");

    // DUNE Energy bias by mode or topology
    tDUNE  -> Project("hDUNE_Delta_CCQE", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC * (Mode==1)", "hist"); // CCQE = Mode 1
    tDUNE  -> Project("hDUNE_Delta_2p2h", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC * (Mode==2)", "hist"); // 2p2h = Mode 2 
    tDUNE  -> Project("hDUNE_Delta_RES", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC && ( (Mode==11) || (Mode==12) || (Mode==13) )", "hist"); // RES = Mode 11, 12, 13
    tDUNE  -> Project("hDUNE_Delta_Other", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC * (Mode!=1) * (Mode!=2) * (Mode!=11) * (Mode!=12) * (Mode!=13)", "hist"); // "Other"
    tDUNE  -> Project("hDUNE_0pi0n", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC && ((Sum$((abs(pdg)==2112))==0) && (Sum$((abs(pdg)==211))==0))", "hist"); // No pions (211), no neutrons (2112)
    tDUNE  -> Project("hDUNE_0piNn", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC && ((Sum$((abs(pdg)==2112))>0) && (Sum$((abs(pdg)==211))==0))", "hist"); // No pions, N neutrons
    tDUNE  -> Project("hDUNE_Npi0n", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC && ((Sum$((abs(pdg)==2112))==0) && (Sum$((abs(pdg)==211))>0))", "hist"); // N pions, no neutrons
    tDUNE  -> Project("hDUNE_NpiNn", "Enu_true-(Erecoil_minerva+ELep)", "flagCCINC && ((Sum$((abs(pdg)==2112))>0) && (Sum$((abs(pdg)==211))>0))", "hist"); // N pions, N neutrons

    // T2K Energy bias by mode or topology
    tT2K   -> Project("hT2K_Delta_CCQE", "Enu_true-Enu_QE", "flagCC0pi * (Mode==1)", "hist");
    tT2K   -> Project("hT2K_Delta_2p2h", "Enu_true-Enu_QE", "flagCC0pi * (Mode==2)", "hist");
    tT2K   -> Project("hT2K_Delta_RES", "Enu_true-Enu_QE", "flagCC0pi && ( (Mode==11) || (Mode==12) || (Mode==13) )", "hist");
    tT2K   -> Project("hT2K_Delta_Other", "Enu_true-Enu_QE", "flagCC0pi * (Mode!=1) * (Mode!=2) * (Mode!=11) * (Mode!=12) * (Mode!=13)", "hist");
    // tT2K   -> Project("hT2K_0pi0n", "Enu_true-(Erecoil_minerva+ELep)", "", "hist");
    // tT2K   -> Project("hT2K_0piNn", "Enu_true-(Erecoil_minerva+ELep)", "", "hist");
    // tT2K   -> Project("hT2K_Npi0n", "Enu_true-(Erecoil_minerva+ELep)", "", "hist");
    // tT2K   -> Project("hT2K_NpiNn", "Enu_true-(Erecoil_minerva+ELep)", "", "hist");

    // ----------------------------------------------------------------------------------------------
    //                                       Plotting
    // ----------------------------------------------------------------------------------------------

    // True E_nu comparison
    hEnuDUNE->SetLineColor(kRed);
    hEnuT2K->SetLineColor(kBlue);

    TCanvas *c2 = new TCanvas("c2", "True neutrino energy comparison", 800, 600);
    hEnuT2K->Draw("hist");
    hEnuDUNE->Draw("hist same");

    TLegend *leg2 = new TLegend(0.7, 0.75, 0.9, 0.9);
    leg2->AddEntry(hEnuDUNE, "DUNE", "l");
    leg2->AddEntry(hEnuT2K, "T2K", "l");
    leg2->Draw();

    c2->SaveAs("../Test_new_plots/true_energy_comparison.pdf");

    // DeltaE (E_true - E_reco) comparison
    hDeltaDUNE->SetLineColor(kRed);
    hDeltaT2K->SetLineColor(kBlue);

    TCanvas *c3 = new TCanvas("c3", "True - reconstructed energy comparison", 800, 600);
    hDeltaDUNE->Draw("hist");
    hDeltaT2K->Draw("hist same");

    TLegend *leg3 = new TLegend(0.7, 0.75, 0.9, 0.9);
    leg3->AddEntry(hDeltaDUNE, "DUNE", "l");
    leg3->AddEntry(hDeltaT2K, "T2K", "l");
    leg3->Draw();

    c3->SaveAs("../Test_new_plots/delta_energy_comparison.pdf");

    // Weighted DeltaE [(E_true - E_reco)/E_true] comparison
    hDeltaDUNE_Weighted->SetLineColor(kRed);
    hDeltaT2K_Weighted->SetLineColor(kBlue);

    TCanvas *c3_2 = new TCanvas("c3_2", "Weighted True - reconstructed energy comparison", 800, 600);
    hDeltaDUNE_Weighted->Draw("hist");
    hDeltaT2K_Weighted->Draw("hist same");

    TLegend *leg3_2 = new TLegend(0.7, 0.75, 0.9, 0.9);
    leg3_2->AddEntry(hDeltaDUNE_Weighted, "DUNE", "l");
    leg3_2->AddEntry(hDeltaT2K_Weighted, "T2K", "l");
    leg3_2->Draw();

    c3_2->SaveAs("../Test_new_plots/delta_energy_comparison_weighted.pdf");

    // Mode-separated DUNE plot
    hDUNE_CCQE->SetLineColor(kGreen+2);
    hDUNE_RES->SetLineColor(kBlue);
    hDUNE_2p2h->SetLineColor(kMagenta);
    hDUNE_Other->SetLineColor(kOrange);

    TCanvas *c4 = new TCanvas("c4", "DUNE events divided by channels", 800, 600);
    hDUNE_Other->Draw("hist");
    hDUNE_CCQE->Draw("hist same");
    hDUNE_RES->Draw("hist same");
    hDUNE_2p2h->Draw("hist same");

    TLegend *leg4 = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg4->AddEntry(hDUNE_CCQE, "CCQE", "l");
    leg4->AddEntry(hDUNE_RES, "RES", "l");
    leg4->AddEntry(hDUNE_2p2h, "2p2h", "l");
    leg4->AddEntry(hDUNE_Other, "Other", "l");
    leg4->Draw();

    c4->SaveAs("../Test_new_plots/DUNE_modes.pdf");

    // Mode-separated T2K plot
    hT2K_CCQE->SetLineColor(kGreen+2);
    hT2K_RES->SetLineColor(kBlue);
    hT2K_2p2h->SetLineColor(kMagenta);
    hT2K_Other->SetLineColor(kOrange);

    TCanvas *c5 = new TCanvas("c5", "T2K events divided by channels", 800, 600);
    hT2K_CCQE->Draw("hist");
    hT2K_RES->Draw("hist same");
    hT2K_2p2h->Draw("hist same");
    hT2K_Other->Draw("hist same");

    TLegend *leg5 = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg5->AddEntry(hT2K_CCQE, "CCQE", "l");
    leg5->AddEntry(hT2K_RES, "RES", "l");
    leg5->AddEntry(hT2K_2p2h, "2p2h", "l");
    leg5->AddEntry(hT2K_Other, "Other", "l");
    leg5->Draw();

    c5->SaveAs("../Test_new_plots/T2K_modes.pdf");

    // DUNE Energy bias split by mode of interaction and topology
    TCanvas *c6 = new TCanvas("c6", "DUNE DeltaE by channel", 1000, 800);
    c6->Divide(1, 2);

    // -------------------------
    // Pad 1: Interaction modes
    // -------------------------
    c6->cd(1);
    gPad->SetMargin(0.12, 0.05, 0.1, 0.08);

    hDUNE_Delta_CCQE->SetLineColor(kRed);
    hDUNE_Delta_RES->SetLineColor(kBlue);
    hDUNE_Delta_2p2h->SetLineColor(kGreen+2);
    hDUNE_Delta_Other->SetLineColor(kMagenta+1);

    hDUNE_Delta_CCQE->SetLineWidth(1);
    hDUNE_Delta_RES->SetLineWidth(1);
    hDUNE_Delta_2p2h->SetLineWidth(1);
    hDUNE_Delta_Other->SetLineWidth(1);

    hDUNE_Delta_CCQE->GetXaxis()->SetTitle("E_{true} - E_{reco} [GeV]");
    hDUNE_Delta_CCQE->GetYaxis()->SetTitle("Events");
    hDUNE_Delta_CCQE->GetXaxis()->SetTitleSize(0.05);
    hDUNE_Delta_CCQE->GetYaxis()->SetTitleSize(0.05);
    hDUNE_Delta_CCQE->GetYaxis()->SetTitleOffset(1.2);

    hDUNE_Delta_CCQE->SetTitle("");
    hDUNE_Delta_CCQE->Draw("hist");
    hDUNE_Delta_RES->Draw("hist same");
    hDUNE_Delta_2p2h->Draw("hist same");
    hDUNE_Delta_Other->Draw("hist same");

    TLegend *leg_modes = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg_modes->SetTextSize(0.035);
    leg_modes->SetBorderSize(0);
    leg_modes->SetFillStyle(0);
    leg_modes->AddEntry(hDUNE_Delta_CCQE, "CCQE", "l");
    leg_modes->AddEntry(hDUNE_Delta_RES, "RES", "l");
    leg_modes->AddEntry(hDUNE_Delta_2p2h, "2p2h", "l");
    leg_modes->AddEntry(hDUNE_Delta_Other, "Other", "l");
    leg_modes->Draw();

    TLatex label1;
    label1.SetNDC();
    label1.SetTextSize(0.04);
    label1.DrawLatex(0.15, 0.93, "DUNE: Energy bias by interaction mode");

    // ---------------------------------
    //   Pad 2: Final-state topology
    // ---------------------------------
    c6->cd(2);
    gPad->SetMargin(0.12, 0.05, 0.15, 0.08);

    hDUNE_0pi0n->SetLineColor(kOrange+7);
    hDUNE_0piNn->SetLineColor(kCyan+1);
    hDUNE_Npi0n->SetLineColor(kViolet-6);
    hDUNE_NpiNn->SetLineColor(kTeal+3);

    hDUNE_0pi0n->SetLineWidth(1);
    hDUNE_0piNn->SetLineWidth(1);
    hDUNE_Npi0n->SetLineWidth(1);
    hDUNE_NpiNn->SetLineWidth(1);

    hDUNE_0pi0n->GetXaxis()->SetTitle("E_{true} - E_{reco} [GeV]");
    hDUNE_0pi0n->GetYaxis()->SetTitle("Events");
    hDUNE_0pi0n->GetXaxis()->SetTitleSize(0.05);
    hDUNE_0pi0n->GetYaxis()->SetTitleSize(0.05);
    hDUNE_0pi0n->GetYaxis()->SetTitleOffset(1.2);

    hDUNE_0pi0n->SetTitle("");
    hDUNE_0pi0n->Draw("hist");
    hDUNE_0piNn->Draw("hist same");
    hDUNE_Npi0n->Draw("hist same");
    hDUNE_NpiNn->Draw("hist same");

    TLegend *leg_topo = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg_topo->SetTextSize(0.035);
    leg_topo->SetBorderSize(0);
    leg_topo->SetFillStyle(0);
    leg_topo->AddEntry(hDUNE_0pi0n, "0 #pi, 0 neutrons", "l");
    leg_topo->AddEntry(hDUNE_0piNn, "0 #pi, N neutrons", "l");
    leg_topo->AddEntry(hDUNE_Npi0n, "N #pi, 0 neutrons", "l");
    leg_topo->AddEntry(hDUNE_NpiNn, "N #pi, N neutrons", "l");
    leg_topo->Draw();

    TLatex label2;
    label2.SetNDC();
    label2.SetTextSize(0.04);
    label2.DrawLatex(0.15, 0.93, "DUNE: Energy bias by final-state topology");

    // Save plot
    c6->SaveAs("../Test_new_plots/DUNE_deltaE_modes_split.pdf");

    // T2K DeltaE (E_true - E_reco) by mode
    hT2K_Delta_CCQE->SetLineColor(kGreen+2);
    hT2K_Delta_RES->SetLineColor(kBlue);
    hT2K_Delta_2p2h->SetLineColor(kMagenta+1);
    hT2K_Delta_Other->SetLineColor(kOrange);

    TCanvas *c7 = new TCanvas("c7", "T2K DeltaE by channel", 800, 600);
    hT2K_Delta_CCQE->SetTitle("T2K #DeltaE = E_{#nu}^{true} - E_{#nu}^{reco} by channel;E_{#nu}^{true} - E_{#nu}^{reco} [GeV];Entries");
    hT2K_Delta_CCQE->Draw("hist");
    hT2K_Delta_RES->Draw("hist same");
    hT2K_Delta_2p2h->Draw("hist same");
    hT2K_Delta_Other->Draw("hist same");

    TLegend *leg7 = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg7->AddEntry(hT2K_Delta_CCQE, "CCQE", "l");
    leg7->AddEntry(hT2K_Delta_RES, "RES", "l");
    leg7->AddEntry(hT2K_Delta_2p2h, "2p2h", "l");
    leg7->AddEntry(hT2K_Delta_Other, "Other", "l");
    leg7->Draw();

    c7->SaveAs("../Test_new_plots/T2K_deltaE_modes.pdf");

    // ----------------------------------------------------------------------------------------------
    //                          Comparison DUNE & T2K separated by mode
    // ----------------------------------------------------------------------------------------------
    TCanvas *c8 = new TCanvas("c8", "Mode comparison DUNE vs T2K", 800, 600);
    hDUNE_CCQE->SetLineColor(kRed);
    hT2K_CCQE->SetLineColor(kBlue);
    hDUNE_2p2h->SetLineColor(kGreen+2);
    hT2K_2p2h->SetLineColor(kMagenta+1);
    hDUNE_RES->SetLineColor(kOrange+7);
    hT2K_RES->SetLineColor(kCyan);
    hDUNE_Other->SetLineColor(kViolet-6);
    hT2K_Other->SetLineColor(kTeal+3);

    hT2K_CCQE->SetTitle("E_{#nu}^{true} mode comparison DUNE vs T2K;E_{#nu}^{true} [GeV];Entries");

    hT2K_CCQE->Draw("hist");
    hDUNE_Other->Draw("hist same");
    hDUNE_CCQE->Draw("hist same");
    // hT2K_CCQE->Draw("hist same");
    hDUNE_2p2h->Draw("hist same");
    hT2K_2p2h->Draw("hist same");
    hDUNE_RES->Draw("hist same");
    hT2K_RES->Draw("hist same");
    // hDUNE_Other->Draw("hist same");
    hT2K_Other->Draw("hist same");

    TLegend *leg8 = new TLegend(0.6, 0.6, 0.88, 0.88);
    leg8->AddEntry(hDUNE_CCQE, "DUNE CCQE", "l");
    leg8->AddEntry(hT2K_CCQE, "T2K CCQE", "l");
    leg8->AddEntry(hDUNE_2p2h, "DUNE 2p2h", "l");
    leg8->AddEntry(hT2K_2p2h, "T2K 2p2h", "l");
    leg8->AddEntry(hDUNE_RES, "DUNE RES", "l");
    leg8->AddEntry(hT2K_RES, "T2K RES", "l");
    leg8->AddEntry(hDUNE_Other, "DUNE Other", "l");
    leg8->AddEntry(hT2K_Other, "T2K Other", "l");
    leg8->Draw();

    c8->SaveAs("../Test_new_plots/DUNE_T2K_modes_comparison.pdf");

    // Close files
    file_DUNE->Close();
    file_T2K->Close();

    return 0;
}