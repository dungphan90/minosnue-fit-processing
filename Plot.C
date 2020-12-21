void Plot() {
    gStyle->SetOptStat(0);
    TFile* infile = new TFile("ProfiledCombineSurface.root", "READ");
    TGraph2D* gr = (TGraph2D*) infile->Get("Chi2Surface");

    double minchi2 = 99999;
    double* z_array = gr->GetZ();
    for (unsigned int i = 0; i < gr->GetN(); ++i) {
        if (z_array[i] < 0) continue;
        minchi2 = minchi2 < z_array[i] ? minchi2 : z_array[i];
    }
    std::cout << minchi2 << std::endl;
    
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

    TH2D* surfaceth2 = new TH2D("surfaceth2", "Chi2 Surface", 200, &x_edges[0], 200, &y_edges[0]);
    for (unsigned int i = 0; i < surfaceth2->GetSize(); ++i) {
        int ix;
        int iy;
        int iz;
        surfaceth2->GetBinXYZ(i, ix, iy, iz);
        double x_center = surfaceth2->GetXaxis()->GetBinCenter(ix);
        double y_center = surfaceth2->GetYaxis()->GetBinCenter(iy);
        surfaceth2->SetBinContent(i, gr->Interpolate(x_center, y_center));
    }

    // TH2D* surfaceth2 = gr->GetHistogram();
    for (unsigned int i = 0; i < surfaceth2->GetSize(); ++i) {
        surfaceth2->SetBinContent(i, surfaceth2->GetBinContent(i) - minchi2);
    }

    TCanvas* c = new TCanvas();
    c->SetLogx();
    c->SetLogy();
    surfaceth2->GetXaxis()->SetTitle(Form("sin^{2}#theta_{14}"));
    surfaceth2->GetYaxis()->SetTitle(Form("sin^{2}#theta_{24}"));
    // c->SetLogz();
    // surfaceth2->GetXaxis()->SetRangeUser(1E-3, 2E-1);
    // surfaceth2->GetYaxis()->SetRangeUser(1E-3, 2E-1);
    surfaceth2->GetZaxis()->SetRangeUser(0, 10);
    surfaceth2->DrawCopy("colz");

    double conts[] = {4.61};  
    surfaceth2->SetContour(1,conts);
    surfaceth2->Draw("cont3 same");
    surfaceth2->SetLineColor(kRed);
    c->Update();
    c->SaveAs(Form("Chi2.pdf"));
}