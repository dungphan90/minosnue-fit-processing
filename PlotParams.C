void PlotParams(int param_code) {
    /*
    param_code
    1: delta_cp
    2: th34
    3: th13
    4: th23
    5: dm23
    */

    gStyle->SetOptStat(0);
    TFile* infile = new TFile("ProfiledCombineSurface.root", "READ");
    std::string param_surface_name;
    switch (param_code) {
        case (1): {
            param_surface_name = "DeltaCP_atBF_Surface";
            break;
        }
        case (2): {
            param_surface_name = "Theta34_atBF_Surface";
            break;
        }
        case (3): {
            param_surface_name = "Theta13_atBF_Surface";
            break;
        }
        case (4): {
            param_surface_name = "Theta23_atBF_Surface";
            break;
        }
        case (5): {
            param_surface_name = "DeltaM32_atBF_Surface";
            break;
        }
    }
    TGraph2D* gr = (TGraph2D*) infile->Get(param_surface_name.c_str());

    double min_log = TMath::Log10(1.0E-3);
    double max_log = TMath::Log10(0.5);
    double step_log_lower = (max_log - min_log) / 200.;
    double x_edges[201];
    double y_edges[201];
    for (unsigned int i = 0; i < 201; ++i) {
        double logx = min_log + step_log_lower * (double) i;
        x_edges[i] = TMath::Power(10, logx);
        y_edges[i] = TMath::Power(10, logx);
    }

    TH2D* surfaceth2 = new TH2D("surfaceth2", param_surface_name.c_str(), 200, &x_edges[0], 200, &y_edges[0]);
    for (unsigned int i = 0; i < surfaceth2->GetSize(); ++i) {
        int ix;
        int iy;
        int iz;
        surfaceth2->GetBinXYZ(i, ix, iy, iz);
        double x_center = surfaceth2->GetXaxis()->GetBinCenter(ix);
        double y_center = surfaceth2->GetYaxis()->GetBinCenter(iy);
        surfaceth2->SetBinContent(i, gr->Interpolate(x_center, y_center));
    }

    TCanvas* c = new TCanvas();
    c->SetLogx();
    c->SetLogy();
    surfaceth2->GetXaxis()->SetTitle(Form("sin^{2}#theta_{14}"));
    surfaceth2->GetYaxis()->SetTitle(Form("sin^{2}#theta_{24}"));
    // surfaceth2->GetZaxis()->SetRangeUser(0.14, 0.16);
    surfaceth2->Draw("COLZ");
    c->SaveAs(Form("%s.pdf", param_surface_name.c_str()));
}