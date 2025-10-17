#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include <iostream>
#include <cmath>
#include <string>

// To compile: c++ first_plots.cpp `root-config --cflags --libs` -o plots.out
static constexpr int NEntries = 1000000;

int main() 
{
    gStyle->SetOptStat(0);

    // ----------------------------------------------------------------------------------------------
    //                                   Open files and TTrees
    // ----------------------------------------------------------------------------------------------
    TFile *file_DUNE = TFile::Open("flat_Valencia_13815.root");
    TFile *file_T2K  = TFile::Open("flat_Valencia_2382.root");

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
    c1->SetTitle("DUNE neutrino flux");
    hFluxDUNE->Draw("hist");
    c1->SaveAs("first_plots/DUNE_flux.pdf");

    TCanvas *c1_T2K = new TCanvas("c1_T2K", "T2K flux", 800, 600);
    hFluxT2K->GetXaxis()->SetRangeUser(0, 9);
    c1_T2K->SetTitle("T2K neutrino flux");
    hFluxT2K->Draw("hist");
    c1_T2K->SaveAs("first_plots/T2K_flux.pdf");

    // -------------------------------------------------------------------------------------------------------------
    //           Histogram definitions and branch addresses for true and reconstructed neutrino energy
    // -------------------------------------------------------------------------------------------------------------

    TH1F *hEnuDUNE = new TH1F("hEnuDUNE", "True neutrino energy comparison;E_{#nu}^{true} [GeV];Number of events", 50, 0, 20000);
    TH1F *hEnuT2K  = new TH1F("hEnuT2K", "True neutrino energy comparison;E_{#nu}^{true} [GeV];Number of events", 50, 0, 20000);
    TH1F *hDeltaDUNE = new TH1F("hDeltaDUNE", "Comparison between true and reconstructed neutrino energy;#DeltaE [GeV];Entries", 50, -500, 2500);
    TH1F *hDeltaT2K  = new TH1F("hDeltaT2K",  "Comparison between true and reconstructed neutrino energy;#DeltaE [GeV];Entries", 50, -500, 2500);
    TH1F *hDeltaDUNE_Weighted = new TH1F("hDeltaDUNE_Weighted", "Weighted difference between true and reconstructed neutrino energy;#DeltaE/E_{#nu}^{true};Entries", 50, -1, 2);
    TH1F *hDeltaT2K_Weighted  = new TH1F("hDeltaT2K_Weighted",  "Weighted difference between true and reconstructed neutrino energy;#DeltaE/E_{#nu}^{true};Entries", 50, -1, 2);
    TH1F *hDUNE_CCQE  = new TH1F("hDUNE_CCQE", "DUNE events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 20000);
    TH1F *hDUNE_RES   = new TH1F("hDUNE_RES",  "DUNE events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 20000);
    TH1F *hDUNE_2p2h  = new TH1F("hDUNE_2p2h", "DUNE events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 20000);
    TH1F *hDUNE_Other = new TH1F("hDUNE_Other","DUNE events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 20000);
    TH1F *hT2K_CCQE   = new TH1F("hT2K_CCQE",  "T2K events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 12000);
    TH1F *hT2K_RES    = new TH1F("hT2K_RES",   "T2K events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 12000);
    TH1F *hT2K_2p2h   = new TH1F("hT2K_2p2h",  "T2K events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 12000);
    TH1F *hT2K_Other  = new TH1F("hT2K_Other", "T2K events divided by channels;E_{#nu}^{true} [GeV];Entries", 50, 0, 12000);

    int ModeDUNE, ModeT2K;
    Float_t Enu_true_DUNE, Enu_true_T2K, Erecoil_minerva, ELep, Enu_QE;
    tDUNE->SetBranchAddress("Mode", &ModeDUNE);
    tDUNE->SetBranchAddress("Enu_true", &Enu_true_DUNE);
    tDUNE->SetBranchAddress("Erecoil_minerva", &Erecoil_minerva);
    tDUNE->SetBranchAddress("ELep", &ELep);
    tT2K->SetBranchAddress("Mode", &ModeT2K);
    tT2K->SetBranchAddress("Enu_true", &Enu_true_T2K);
    tT2K->SetBranchAddress("Enu_QE", &Enu_QE);

    std::vector<Float_t> Enu_Difference_DUNE, Enu_Difference_T2K, Enu_diff_weighted_DUNE, Enu_diff_weighted_T2K;
    std::vector<Float_t> Enu_CCQE_DUNE, Enu_RES_DUNE, Enu_2p2h_DUNE, Enu_Other_DUNE;
    std::vector<Float_t> Enu_CCQE_T2K, Enu_RES_T2K, Enu_2p2h_T2K, Enu_Other_T2K;
    Enu_Difference_DUNE.resize(tDUNE->GetEntries());
    Enu_Difference_T2K.resize(tT2K->GetEntries());
    Enu_diff_weighted_DUNE.resize(tDUNE->GetEntries());
    Enu_diff_weighted_T2K.resize(tT2K->GetEntries());

    for (Long64_t i=0; i<tDUNE->GetEntries(); i++) 
    {
        // DUNE entries
        tDUNE->GetEntry(i);
        hEnuDUNE->Fill(Enu_true_DUNE);
        Enu_Difference_DUNE[i] = Enu_true_DUNE - (Erecoil_minerva + ELep);
        Enu_diff_weighted_DUNE[i] = (Enu_true_DUNE - (Erecoil_minerva + ELep))/Enu_true_DUNE;
        if (ModeDUNE == 1)
            Enu_CCQE_DUNE.push_back(Enu_true_DUNE);
        else if (ModeDUNE == 2)
            Enu_2p2h_DUNE.push_back(Enu_true_DUNE);
        else if (ModeDUNE == 11 || ModeDUNE == 12 || ModeDUNE == 13)
            Enu_RES_DUNE.push_back(Enu_true_DUNE);
        else
            Enu_Other_DUNE.push_back(Enu_true_DUNE);
        //hDeltaDUNE->Fill(Enu_true_DUNE-(Erecoil_minerva+ELep)); // Apparently this doesn't work?
    }
    for (Long64_t i=0; i<tT2K->GetEntries(); i++)
    {
        // T2K entries
        tT2K->GetEntry(i);
        hEnuT2K->Fill(Enu_true_T2K);
        //hDeltaT2K->Fill(Enu_true_T2K - Enu_QE);                 // Same here
        Enu_Difference_T2K[i] = Enu_true_T2K - Enu_QE;
        Enu_diff_weighted_T2K[i] = (Enu_true_T2K - Enu_QE)/Enu_true_T2K;
        if (ModeT2K == 1)
            Enu_CCQE_T2K.push_back(Enu_true_T2K);
        else if (ModeT2K == 2)
            Enu_2p2h_T2K.push_back(Enu_true_T2K);
        else if (ModeT2K == 11 || ModeT2K == 12 || ModeT2K == 13)
            Enu_RES_T2K.push_back(Enu_true_T2K);
        else
            Enu_Other_T2K.push_back(Enu_true_T2K);
    }

    // ----------------------------------------------------------------
    //             True neutrino energy comparison (works)
    // ----------------------------------------------------------------
    hEnuDUNE->SetLineColor(kRed);
    hEnuT2K->SetLineColor(kBlue);

    TCanvas *c_Enu_true = new TCanvas("c_Enu_true", "True neutrino energy comparison", 800, 600);
    hEnuT2K->Draw("hist");
    hEnuDUNE->Draw("hist same");

    auto leg_Enu_true = new TLegend(0.6,0.7,0.88,0.88);
    leg_Enu_true->AddEntry(hEnuDUNE, "DUNE");
    leg_Enu_true->AddEntry(hEnuT2K, "T2K");
    leg_Enu_true->Draw();
    c_Enu_true->SaveAs("first_plots/Enu_true_comparison.pdf");

    // ------------------------------------------------------------------
    //                 DeltaE = Etrue - Ereco (works)
    // ------------------------------------------------------------------
    for (int i = 0; i < tDUNE->GetEntries(); i++) 
    {
        hDeltaDUNE->Fill(Enu_Difference_DUNE[i]);
        hDeltaT2K->Fill(Enu_Difference_T2K[i]);
    }

    hDeltaDUNE->SetLineColor(kRed);
    hDeltaT2K->SetLineColor(kBlue);

    TCanvas *c_Reco_difference = new TCanvas("c_Reco_difference", "Reconstruction comparison", 800, 600);
    hDeltaDUNE->Draw("hist");
    hDeltaT2K->Draw("hist same");

    auto leg_Reco_diff = new TLegend(0.6,0.7,0.88,0.88);
    leg_Reco_diff->AddEntry(hDeltaDUNE, "DUNE difference between true and reco energy");
    leg_Reco_diff->AddEntry(hDeltaT2K, "T2K difference between true and reco energy");
    leg_Reco_diff->Draw();

    c_Reco_difference->SaveAs("first_plots/DeltaE_comparison.pdf");

    // ------------------------------------------------------------
    //                      DeltaE/Enu_true
    // ------------------------------------------------------------
    for(int i = 0; i < tDUNE->GetEntries(); i++) 
    {
        hDeltaDUNE_Weighted->Fill(Enu_diff_weighted_DUNE[i]);
        hDeltaT2K_Weighted->Fill(Enu_diff_weighted_T2K[i]);
    }

    hDeltaDUNE_Weighted->SetLineColor(kRed);
    hDeltaT2K_Weighted->SetLineColor(kBlue);

    TCanvas *c_Reco_Weighted = new TCanvas("c_Reco_Weighted", "Weighted difference between true and reco energy", 800, 600);
    hDeltaDUNE_Weighted->Draw("hist");
    hDeltaT2K_Weighted->Draw("hist same");

    auto leg_Reco_weighted = new TLegend(0.6,0.7,0.88,0.88);
    leg_Reco_weighted->AddEntry(hDeltaDUNE_Weighted, "DUNE");
    leg_Reco_weighted->AddEntry(hDeltaT2K_Weighted, "T2K");
    leg_Reco_weighted->Draw();

    c_Reco_Weighted->SaveAs("first_plots/Weighted_DeltaE_comparison.pdf");

    // --------------------------------------------------
    //           Mode-separated plots for DUNE
    // --------------------------------------------------
    TCanvas *c_DUNE_Enu_Modes = new TCanvas("c_DUNE_Enu_Modes", "Mode separation DUNE", 800, 600);
    for (int i = 0; i < Enu_CCQE_DUNE.size(); i++)
        hDUNE_CCQE->Fill(Enu_CCQE_DUNE[i]);
    for (int i = 0; i < Enu_2p2h_DUNE.size(); i++)
        hDUNE_2p2h->Fill(Enu_2p2h_DUNE[i]);
    for (int i = 0; i < Enu_RES_DUNE.size(); i++)
        hDUNE_RES->Fill(Enu_RES_DUNE[i]);
    for (int i = 0; i < Enu_Other_DUNE.size(); i++)
        hDUNE_Other->Fill(Enu_Other_DUNE[i]);

    hDUNE_CCQE->SetLineColor(kRed);
    hDUNE_2p2h->SetLineColor(kBlue);
    hDUNE_RES->SetLineColor(kGreen+2);
    hDUNE_Other->SetLineColor(kMagenta);

    hDUNE_Other->Draw("hist");
    hDUNE_2p2h->Draw("hist same");
    hDUNE_RES->Draw("hist same");
    hDUNE_CCQE->Draw("hist same");

    auto leg_DUNE_Enu_Modes = new TLegend(0.6,0.6,0.88,0.88);
    leg_DUNE_Enu_Modes->AddEntry(hDUNE_CCQE,"CCQE");
    leg_DUNE_Enu_Modes->AddEntry(hDUNE_2p2h,"2p2h");
    leg_DUNE_Enu_Modes->AddEntry(hDUNE_RES, "RES");
    leg_DUNE_Enu_Modes->AddEntry(hDUNE_Other,"Other");
    leg_DUNE_Enu_Modes->Draw();

    c_DUNE_Enu_Modes->SaveAs("first_plots/DUNE_CCQE_2p2h_RES_Other.pdf");

    // --------------------------------------------------
    //           Mode-separated plots for T2K
    // --------------------------------------------------
    TCanvas *c_T2K_Enu_Modes = new TCanvas("c_T2K_Enu_Modes", "Mode separation T2K", 800, 600);
    for (int i = 0; i < Enu_CCQE_T2K.size(); i++)
        hT2K_CCQE->Fill(Enu_CCQE_T2K[i]);
    for (int i = 0; i < Enu_2p2h_T2K.size(); i++)
        hT2K_2p2h->Fill(Enu_2p2h_T2K[i]);
    for (int i = 0; i < Enu_RES_T2K.size(); i++)
        hT2K_RES->Fill(Enu_RES_T2K[i]);
    for (int i = 0; i < Enu_Other_T2K.size(); i++)
        hT2K_Other->Fill(Enu_Other_T2K[i]);

    hT2K_CCQE->SetLineColor(kRed);
    hT2K_2p2h->SetLineColor(kBlue);
    hT2K_RES->SetLineColor(kGreen+2);
    hT2K_Other->SetLineColor(kMagenta);

    hT2K_CCQE->Draw("hist");
    hT2K_2p2h->Draw("hist same");
    hT2K_RES->Draw("hist same");
    hT2K_Other->Draw("hist same");

    auto leg_T2K_Enu_Modes = new TLegend(0.6,0.6,0.88,0.88);
    leg_T2K_Enu_Modes->AddEntry(hT2K_CCQE,"CCQE");
    leg_T2K_Enu_Modes->AddEntry(hT2K_2p2h,"2p2h");
    leg_T2K_Enu_Modes->AddEntry(hT2K_RES, "RES");
    leg_T2K_Enu_Modes->AddEntry(hT2K_Other,"Other");
    leg_T2K_Enu_Modes->Draw();

    c_T2K_Enu_Modes->SaveAs("first_plots/T2K_CCQE_2p2h_RES_Other.pdf");

    // Close files
    file_DUNE->Close();
    file_T2K->Close();

    return 0;
}